#include "stdafx.h"
#include "hddio.h"
#include <string.h>
#include <atlconv.h>
#include <stdint.h>
#include <algorithm>

#define EOC 0x0FFFFFF8
#define BAD 0x0FFFFFF7

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20
#define ATTR_LONG_NAME  (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define ATTR_LONG_NAME_MASK     (ATTR_READ_ONLY |ATTR_HIDDEN |ATTR_SYSTEM |ATTR_VOLUME_ID |ATTR_DIRECTORY |ATTR_ARCHIVE)

#ifndef _tcscpy_s
#define _tcscpy_s( d,sz,s ) lstrcpyn( d, s, sz )
#endif

struct DOSDATE
{
	int day:5;
	int mon:4;
	int year:7;
};
union DOSDATEW
{
	DOSDATE d;
	WORD w;
};
struct DOSTIME
{
	int sec:5;
	int min:6;
	int hour:5;
};
union DOSTIMEW
{
	DOSTIME t;
	WORD w;
};




unsigned char ChkSum (PCHAR pFcbName)
{
	short FcbNameLen;
	unsigned char Sum;

	Sum = 0;
	for (FcbNameLen=11; FcbNameLen!=0; FcbNameLen--) {
		// NOTE: The operation is an unsigned char rotate right
		Sum = ((Sum & 1) ? 0x80 : 0) + (Sum >> 1) + *pFcbName++;
	}
	return (Sum);
}

void dosdate2st( SYSTEMTIME& st, WORD date)
{
	DOSDATEW x; x.w=date;
	st.wDay=x.d.day & 0x1F;
	st.wMonth=x.d.mon & 0x0F;
	st.wYear=(x.d.year & 0x7F)+1980;
}
void dostime2st( SYSTEMTIME& st, WORD time)
{
	DOSTIMEW x; x.w=time;

	st.wHour=x.t.hour & 0x1F;
	st.wMinute=x.t.min & 0x3F;
	st.wSecond=(x.t.sec & 0x1F)*2;
}

//2do: Û·‡Ú¸ Ì‡ıÂ
static bool isptable(BYTE* tmpbuff)
{
	return
		//tmpbuff[i+0x1BE]==0x80 && tmpbuff[i+0x1C0]==8 &&
		tmpbuff[0x1FE]==0x55
		&& tmpbuff[0x1FF]==0xAA

		&& (tmpbuff[0x1BE]==0 || tmpbuff[0x1BE]==0x80) //0 	byte 	Boot indicator bit flag: 0 = no, 0x80 = bootable (or "active")
		&& tmpbuff[0x1BF]<=16 //1 	byte 	Starting head

		&& tmpbuff[0x0]==0
		&& tmpbuff[0x1]==0
		//&& (tmpbuff[i+0x1FC]==0)
		//&& (tmpbuff[i+0x1FD]==0)
		;
}


HDDIo::HDDIo()
{
}

HDDIo::~HDDIo()
{
	CloseHandle(h);
	if(fat)delete [] fat;
}

unsigned long HDDIo::getfatitem(long cluster)
{
	long offset = cluster*fattype/8;
	long retcode;
	if(fattype==12)
	{
		int ptr = cluster * 3 / 2;
		retcode = fat[ ptr ] + fat[ ptr+1 ]*256;
		if( cluster & 1 )
			retcode >>= 4;
		else
			retcode &= 0xFFF;
		if(retcode >= 0xFF7)
			retcode |= 0x0FFFF000;
	}
	else
		if(fattype==16)
		{
			retcode=*(short*)(&fat[offset]);
		}
		else
			if(fattype==32)
				retcode=*(long*)(&fat[offset]);
	retcode &= 0x0FFFFFFF;
	if(retcode>=0x0FFFFFF8)retcode=EOC;
	return retcode;
}


bool HDDIo::gotocluster(long cluster)
{
	if(cluster<2)return false;
	return gotosector( ( (unpackitems?packedfatitems[cluster]:cluster) -2)*boot.BPB_SecPerClus+FirstDataSector);
}

bool HDDIo::gotosector(long long sector)
{
	uint64_t x = sector*boot.BPB_BytsPerSec + partitionstart;
	LONG xh = x>>32;
	return SetFilePointer(h,x & 0xFFFFFFFF, &xh, FILE_BEGIN) != INVALID_SET_FILE_POINTER;
}

BOOL HDDIo::opendrive(TCHAR driveletter)
{
	TCHAR tempfilename[MAX_PATH];
	_stprintf((TCHAR*)&tempfilename,_T("\\\\.\\%c:"),driveletter);
	return openfile((TCHAR*)&tempfilename);
}

BOOL HDDIo::initpartition(void)
{
	BOOL retcode = true;

	DWORD nread;

	byte ptable[512];

	unsigned long curoffset = SetFilePointer( h, 0, 0, FILE_CURRENT );

	if(!ReadFile(h, &ptable, sizeof(ptable), &nread, NULL))
	{
		retcode = false;
	}else
	{
		if( ptable[0]==0xEB && ptable[0x26]==0x29 && ptable[0x36]=='F')
		{
			partitionstart = curoffset;
		}else if( *(short*)&ptable[0x1FE]==(short)0xAA55 )
		{
			partitionstart= *(DWORD*)&ptable[0x1C6]*0x200 + curoffset;//!!!!!first partition
		}else 
		{
			retcode = false;
		}
		if( retcode )
		{
			//printf("\n\npartition at %x\n\n",partitionstart);
			gotosector( 0 );

			if(ReadFile(h,&boot,sizeof(boot),&nread,NULL))
			{
				if(boot.BPB_FATSz16)FATSz=boot.BPB_FATSz16; else FATSz=boot.b32part_.BPB_FATSz32;
				if(boot.BPB_TotSec16 != 0)TotSec = boot.BPB_TotSec16; else TotSec = boot.BPB_TotSec32;

				// Œ—“€À‹!!!!!!!!!
				if(!memcmp(boot.b32part_.BS_FilSysType,"FAT32   ",8))
					boot.BPB_RsvdSecCnt = boot.BPB_SecPerClus;

				RootDirSectors = ((boot.BPB_RootEntCnt * 32)
					+ (boot.BPB_BytsPerSec - 1))
					/ boot.BPB_BytsPerSec;
				FirstDataSector = boot.BPB_RsvdSecCnt + (boot.BPB_NumFATs * FATSz) + RootDirSectors;

				long DataSec = TotSec - (boot.BPB_RsvdSecCnt + (boot.BPB_NumFATs * FATSz) + RootDirSectors);
				long CountofClusters = DataSec / boot.BPB_SecPerClus;
				if(CountofClusters < 4085) {
					fattype=12;
				} else if(CountofClusters < 65525) {
					fattype=16;
				} else {
					fattype=32;
				}
				//printf("found fat%d\n",fattype);

				packedfatitems.clear();

				fatsize=FATSz*boot.BPB_BytsPerSec;
				fat=new unsigned char[fatsize];
				gotosector(boot.BPB_RsvdSecCnt);
				DWORD rez;
				if(ReadFile(h,fat,fatsize,&rez,NULL))
				{
					int cur=0;
					for(int i=0; i<CountofClusters; i++)
					{
						packedfatitems.push_back( cur );
						if( getfatitem(i) != 0 )
							cur++;
					}
					packedfatitems.push_back( cur );
				}

				detectedpartitionname = NULL;
				TCHAR* part = getpartition();
				unpackitems = !part || ( _tcscmp(_T("/BOOT"), part) != 0 );

			}else
			{
				retcode = false;
			}
		}
	}

	return retcode;
}

BOOL HDDIo::trynextpartition()
{
	if(! gotocluster( packedfatitems.size()-1 ) )
		return false;

	BYTE tmp[512];
	DWORD readed;
	while(ReadFile(h,tmp,512,&readed,NULL) && (readed==0x200))
	{
		if(isptable(tmp))
		{
			SetFilePointer(h,-0x200,NULL,FILE_CURRENT);
			return initpartition();
		}
	}
	return false;
}


BOOL HDDIo::openfile(TCHAR* filename)
{
	BOOL retcode = true;
	fat=NULL;
	h=CreateFile(filename
		,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ
		,NULL
		,OPEN_EXISTING
		,0
		,NULL);
	if( h == INVALID_HANDLE_VALUE )
	{
		retcode = false;
	}else
	{
		retcode = initpartition();
	}
	if( h != INVALID_HANDLE_VALUE && ! retcode )
	{
		CloseHandle( h );
		h = INVALID_HANDLE_VALUE;
	}
	return retcode;
}

vector<mydirlistitem> HDDIo::readdir(long cluster)
{
	vector<mydirlistitem> rez;

	int numofdirs= (fattype!=32)&&(cluster==0)?
		boot.BPB_RootEntCnt
		:
	boot.BPB_SecPerClus*boot.BPB_BytsPerSec/sizeof(DIR);
	DIR* x=new DIR[numofdirs];

	tcstring lname;
	int lnamecount=0;
	int lnamechksum;

	//2 == check . and .. if it's not root dir
	int needtocheckpts= cluster?2:0;//!!!!!!!!(cluster==boot.b32part.BPB_RootClus?0:2);

	while( cluster>=0 && cluster<0x0FFFFFF0 ) //( (cluster!=EOC) && (cluster!=BAD) )
	{
		if(cluster==0)
		{
			if(fattype==32)
				gotocluster(cluster = 2);
			else
				gotosector(boot.BPB_RsvdSecCnt + (boot.BPB_NumFATs * FATSz));
		}
		else
			gotocluster(cluster);

		DWORD tmpdata;
		ReadFile(h,x,sizeof(DIR)*numofdirs,&tmpdata,NULL);
		for(int i=0;i<numofdirs;i++)
		{
			if(x[i].l.LDIR_Ord==0){ cluster=EOC; break; }
			if(x[i].l.LDIR_Ord==0xE5)
			{
				lnamecount=x[i].s.DirName[0]&(!0x40);
				lname=_T("");
			}else
				if( (x[i].l.LDIR_Attr&ATTR_LONG_NAME_MASK) == ATTR_LONG_NAME)
				{
					//lfn
					lnamechksum=x[i].l.LDIR_Chksum;
					if(x[i].l.LDIR_Ord&0x40)
					{
						lnamecount=x[i].l.LDIR_Ord -0x40;
						lname=_T("");
					}else
					{
						if(x[i].l.LDIR_Ord!=lnamecount)
						{
							lnamecount=0;
							lname=_T("");
						}
					}
					if(lnamecount)
					{
						WCHAR temp[16];
						memset(&temp,0,sizeof(temp));
						memcpy(&temp[0],&x[i].l.LDIR_Name1,10);
						memcpy(&temp[5],&x[i].l.LDIR_Name2,12);
						memcpy(&temp[11],&x[i].l.LDIR_Name3,4);
#ifdef _UNICODE
						lname=temp+lname;
#else
						TCHAR temp2[MAX_PATH];
						WideCharToMultiByte(CP_OEMCP,0,(PWCHAR)&temp,-1,(PCHAR)&temp2,sizeof(temp2),NULL,NULL);
						lname=temp2+lname;
#endif
						lnamecount--;
					}
				}else
					if( x[i].l.LDIR_Attr & ATTR_VOLUME_ID)
					{
						lname=_T("");
						lnamecount=0;
					}
					else
					{
						if(x[i].s.DirName[0]==0x5) x[i].s.DirName[0]=TCHAR(0xE5);

						mydirlistitem it;

						TCHAR name[9];
						TCHAR ext[4];
#ifdef _UNICODE
						MultiByteToWideChar(CP_OEMCP,0,x[i].s.DirName,8,name,8);
						MultiByteToWideChar(CP_OEMCP,0,&x[i].s.DirName[8],3,ext,3);
#else
						strncpy(name,x[i].s.DirName,9);
						strncpy(ext,&x[i].s.DirName[8],4);
#endif
						tcstring fname=tcstring(name,0,8);
						tcstring fext=tcstring(ext,0,3);

						while(fname.size()&&(fname[fname.size()-1]==_T(' ')))
							fname=tcstring(fname,0,fname.size()-1);
						while(fext.size()&&(fext[fext.size()-1]==_T(' ')))
							fext=tcstring(fext,0,fext.size()-1);

						if(fext.size())fname=fname+_T(".")+fext;

						if(lnamechksum!=ChkSum((PCHAR)&(x[i].s.DirName)) || (lname.size()==0))
						{
							lname=fname;
						}

						memset(&it,0,sizeof(it));

						_tcscpy_s(it.fd.cFileName,sizeof(it.fd.cFileName)/sizeof(it.fd.cFileName[0]),lname.c_str());
						_tcscpy_s(it.fd.cAlternateFileName,sizeof(it.fd.cAlternateFileName)/sizeof(it.fd.cAlternateFileName[0]),fname.c_str());

						it.fd.dwFileAttributes=x[i].s.DIR_Attr;
						it.firstcluster=MAKELONG(x[i].s.DIR_FstClusLO,x[i].s.DIR_FstClusHI);
						it.fd.nFileSizeLow=x[i].s.DIR_FileSize;
						it.fd.nFileSizeHigh=0;

						SYSTEMTIME st;
						memset(&st,0,sizeof(st));
						dosdate2st( st, x[i].s.DIR_CrtDate );
						dostime2st( st, x[i].s.DIR_CrtTime );
						st.wMilliseconds=x[i].s.DIR_CrtTimeTenth;
						st.wMinute+=st.wMilliseconds/100;
						st.wMilliseconds=(st.wMilliseconds%100)*10;
						SystemTimeToFileTime(&st,&it.fd.ftCreationTime);

						memset(&st,0,sizeof(st));
						dosdate2st( st, x[i].s.DIR_WrtDate );
						dostime2st( st, x[i].s.DIR_WrtTime );
						SystemTimeToFileTime(&st,&it.fd.ftLastWriteTime);

						memset(&st,0,sizeof(st));
						dosdate2st( st, x[i].s.DIR_LstAccDate );
						SystemTimeToFileTime(&st,&it.fd.ftLastAccessTime);

						if(needtocheckpts==1)
						{
							if( (_tcscmp((TCHAR*)&it.fd.cFileName,_T(".."))!=0)
								|| (it.fd.dwFileAttributes!=ATTR_DIRECTORY)
								)
							{
								rez.clear();
								delete x;
								return rez;
							}
							needtocheckpts--;
						}
						if(needtocheckpts==2)
						{
							if( (_tcscmp((TCHAR*)&it.fd.cFileName,_T("."))!=0)
								|| (it.fd.dwFileAttributes!=ATTR_DIRECTORY)
								)
							{
								rez.clear();
								delete x;
								return rez;
							}
							needtocheckpts--;
						}
						it.parent=NULL;
						rez.push_back(it);

						lname=_T("");
						lnamecount=0;
					}
		}
		if((unsigned)cluster<(unsigned)fatsize*fattype/8)cluster=getfatitem(cluster); else break;
	}
	delete x;
	return rez;
}

unsigned char* HDDIo::readfile(unsigned int cluster,int size)
{
	long clustersize=boot.BPB_BytsPerSec*boot.BPB_SecPerClus;
	char* x=new char[clustersize];
	unsigned char* ret=new unsigned char[size];
	long pos=0;

	while( (cluster<0x0FFFFFF0U) && size) //( (cluster!=EOC) && (cluster!=BAD) )
		//while( (cluster!=EOC) && (cluster!=BAD) &&(size))
	{
		gotocluster(cluster);

		DWORD tmpdata;
		ReadFile(h,x,clustersize,&tmpdata,NULL);
		tmpdata=size;
		if((unsigned)tmpdata>(unsigned)clustersize)tmpdata=clustersize;
		memcpy(&ret[pos],x,tmpdata);
		pos+=tmpdata;
		size-=tmpdata;
		if(cluster==EOC)break;
		if(cluster<fatsize*fattype/8)cluster=getfatitem(cluster); else break;
	}
	delete[] x;
	if(!size)return ret;
	delete[] ret;
	return NULL;
}


bool operator ==(const mydirlistitem &dl, const TCHAR* str)
{
	return 0 == _tcscmp( dl.fd.cFileName, str );
}

TCHAR* HDDIo::getpartition()
{
	if(detectedpartitionname)
		return detectedpartitionname;

	vector<mydirlistitem> dl;
	dl=readdir(0);
	if( dl.size() != 0 )
	{
		if( std::find( dl.begin(), dl.end(), _T("BOOT") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("IFS") ) != dl.end() )
		{
			//_tcprintf( _T("found /\n") );
			detectedpartitionname = _T("/");
		}else if( std::find( dl.begin(), dl.end(), _T("CAMERA") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("PRESET") ) != dl.end() )
		{
			//_tcprintf( _T("found /SYSTEM\n") );
			detectedpartitionname = _T("/SYSTEM");
		}else if( std::find( dl.begin(), dl.end(), _T("SYSTEM") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("OTHER") ) != dl.end() )
		{
			//_tcprintf( _T("found /USB\n") );
			detectedpartitionname = _T("/USB");
		}else if( std::find( dl.begin(), dl.end(), _T("PDH") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("FIFO") ) != dl.end() 
			&& std::find( dl.begin(), dl.end(), _T("OS") ) != dl.end() 
			)
		{
			//_tcprintf( _T("found /IFS\n") );
			detectedpartitionname = _T("/IFS");
		}else if( std::find( dl.begin(), dl.end(), _T("UIDEFS.TXT") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("UA.BIN") ) != dl.end() 
			&& std::find( dl.begin(), dl.end(), _T("SW.BIN") ) != dl.end() 
			)
		{
			//_tcprintf( _T("found /SYSTEM/FOTA\n") );
			detectedpartitionname = _T("/TPA/SYSTEM/FOTA");
		}else if( std::find( dl.begin(), dl.end(), _T("phone_app.cxc") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("phone_acc.cxc") ) != dl.end() 
			)
		{
			//_tcprintf( _T("found /BOOT\n") );
			detectedpartitionname = _T("/BOOT");
		}else if( std::find( dl.begin(), dl.end(), _T("fcl_internal.dbf") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("synccl_databank.dbf") ) != dl.end() 
			)
		{
			//_tcprintf( _T("found /TPA/USER/SYSTEM\n") );
			detectedpartitionname = _T("/TPA/USER/SYSTEM");
		}else if( std::find( dl.begin(), dl.end(), _T("gdfs-app0.ps") ) != dl.end()
			&& std::find( dl.begin(), dl.end(), _T("gdfs-app1.ps") ) != dl.end() 
			)
		{
			//_tcprintf( _T("found /SYS\n") );
			detectedpartitionname = _T("/SYS");
		}

	}

	return detectedpartitionname;
}