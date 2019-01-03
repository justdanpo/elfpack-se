// read3150.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <direct.h>
#include <conio.h>
#include "hddio.h"
#include "main2raw.h"


#define HELLO _T("A2 fs extractor by den_po v0.7\n")


HDDIo hddio;

void extract(int cl,tcstring tc,tcstring sdirin)
{
	int partition = 1;
	do
	{
		vector<mydirlistitem> dl;
		vector<mydirlistitem>::iterator dli;
		dl=hddio.readdir(cl);
		if( dl.size() != 0 )
		{

			tcstring sdir = sdirin;
			if( !cl )
			{
				TCHAR* part = hddio.getpartition();
				if(part)
				{
					sdir += part;
				}else
				{
					TCHAR tmp[256];
					_stprintf( tmp, _T("Partition.%d"), partition );
					//_tcprintf( _T("found /%s\n"), tmp );
					sdir += tcstring(_T("/")) + tmp;
				}

			}

			_tmkdir(sdirin.c_str());
			_tmkdir(sdir.c_str());

			for(dli=dl.begin();dli!=dl.end();dli++)
			{
				if(_tcscmp(dli->fd.cFileName,_T(".")) &&_tcscmp(dli->fd.cFileName,_T("..")))
				{
					_tcprintf(_T("writing %-40.40s\r"),dli->fd.cFileName);
					tcstring newname=sdir+_T("\\")+dli->fd.cFileName;
					if(dli->fd.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY)
					{
						//if(cl==0)
						//	_tprintf(_T("\n /%-40.40s\n"),dli->fd.cFileName);
						extract(dli->firstcluster,tcstring(tc+_T("  ")),newname);

					}else
					{
						//if(cl==0)
						//	_tprintf(_T(" %-40.40s\n"),dli->fd.cFileName);
						PBYTE file=hddio.readfile(dli->firstcluster,dli->fd.nFileSizeLow);
						if(file)
						{
							HANDLE of=CreateFile(newname.c_str(),GENERIC_WRITE,0,0,CREATE_ALWAYS,
								dli->fd.dwFileAttributes,0);
							DWORD numwrite;
							WriteFile(of,file,dli->fd.nFileSizeLow,&numwrite,0);
							SetFileTime(of,&dli->fd.ftCreationTime,&dli->fd.ftLastAccessTime,
								&dli->fd.ftLastWriteTime);
							CloseHandle(of);
							delete file;
						}
					}
				}
			}
		}
		partition ++;
	}while( cl == 0 && hddio.trynextpartition() );
}

bool isptable(BYTE* tmpbuff)
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

bool isecchdr(BYTE* tmpbuff)
{
	return tmpbuff[0] ==0xFF
		&& tmpbuff[0x01]==0xFF
		&& tmpbuff[0x02]==0
		&& tmpbuff[0x03]==0xFF
		&& tmpbuff[0x04]==0xFF
		&& tmpbuff[0x05]==0xFF
		&& tmpbuff[0x09]==0xC0
		&& tmpbuff[0x0d]==0xC0
		&& tmpbuff[0x11]==0xC0
		&& tmpbuff[0x15]==0xC0;

}

//2do: временное решение
int readwindow(FILE* f, PBYTE dest, int size)
{
	static BYTE tmpbuff[0x1080];
	static int buffsize=0;
	static int windowsize=0x40;
	static bool skip=false;
	//printf(" position %x, size %x, buffsize %x\n",ftell(f),size,buffsize);
	if(size<=buffsize)
	{
		memcpy(dest, tmpbuff, size);
		memcpy(tmpbuff, &tmpbuff[size], buffsize-size);
		buffsize-=size;
		return size;
	}

	int retsize=0;
	while(retsize<size)
	{
		int nextsize = buffsize < size-retsize? buffsize : size-retsize;
		memcpy(&dest[retsize], tmpbuff, nextsize);
		memcpy(tmpbuff, &tmpbuff[nextsize], buffsize-nextsize);
		retsize+=nextsize;
		buffsize-=nextsize;

		nextsize = fread(&tmpbuff[buffsize], 1, windowsize, f);
		if(buffsize+nextsize==0)
			break;
		if( (windowsize == 0x40) && (nextsize == 0x40) && isecchdr(&tmpbuff[buffsize]))
		{
			windowsize = 0x840;
			nextsize = 0;
		}
		if(nextsize>0x800)
			nextsize=0x800;
		buffsize+=nextsize;
	}
	return retsize;
}


bool getpartition(tcstring rawname,tcstring dskname)
{
	FILE* inf;
	inf=_tfopen(rawname.c_str(),_T("rb"));

	BYTE tmpbuff[0x800];
	memset(tmpbuff, 0, sizeof(tmpbuff));

	int readed;
	for(;readed=readwindow(inf, &tmpbuff[0x200-0x80],0x80);)
	{
		if( isptable( tmpbuff ) )
		{
			FILE* of=_tfopen(dskname.c_str(),_T("wb"));
			fwrite(tmpbuff, 0x200-0x80+readed, 1, of);

			while(readed=readwindow(inf,tmpbuff,0x800))
			{
				fwrite(tmpbuff,readed,1,of);
			}

			fclose(of);
			fclose(inf);
			return true;
		}

		memcpy(tmpbuff,&tmpbuff[0x80],0x200-0x80);
	}

	fclose(inf);
	return false;
}


int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf(HELLO);
	if(argc<2)
	{
		_tprintf(_T("i need a file\n"));
	}else
	{

		if(hddio.openfile(argv[1]))
		{
			try
			{
				_tprintf(_T("processing raw\n"));
				extract(0/*rootdir*/,_T(""),tcstring(argv[1])+_T(".out"));
				_tprintf(_T("%-48.48s\n"),_T("done"));
				return 0;
			}catch(...)
			{
				_tprintf(_T("=(\n"));
			}
		}else
		{
			_tprintf(_T("converting to raw\n"));
			tcstring rawname=tcstring(argv[1])+_T(".raw");
			if(!convert2raw(argv[1],rawname.c_str()))
			{
				_tprintf(_T("error\n"));
			}else
			{
				try
				{
					if(hddio.openfile((TCHAR*)rawname.c_str()))
					{
						_tprintf(_T("processing\n"));
						extract(0/*rootdir*/,_T(""),rawname+_T(".out"));
						_tprintf(_T("%-48.48s\n"),_T("done"));
						return 0;
					}else
					{
						_tprintf(_T("extracting partition\n"));
						tcstring dskname=tcstring(argv[1])+_T(".dsk");
						try
						{
							if(getpartition(rawname,dskname))
							{
								if(hddio.openfile((TCHAR*)dskname.c_str()))
								{
									_tprintf(_T("processing\n"));
									extract(0/*rootdir*/,_T(""),rawname+_T(".out"));
									_tprintf(_T("%-48.48s\n"),_T("done"));
									return 0;
								}else
								{
									_tprintf(_T("bad file\n"));
								}
							}
						}catch(...)
						{
							_tprintf(_T("=(\n"));
						}
					}
				}catch(...)
				{
					_tprintf(_T("=(\n"));
				}
			}
		}
	}
	return 1;
}
