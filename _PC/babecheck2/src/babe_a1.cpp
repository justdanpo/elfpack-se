#include "stdafx.h"

#include <stdint.h>
#include "babeheaders\babe_a1.h"

#include "openssl/sha.h"

#include "certs\certz.h"

#include "rsadecode.h"

#include <string>
typedef std::basic_string<unsigned char> ucbuffer;

#pragma pack(push,1)
struct A1CERT
{
	uint32_t keysize;//1
	char name[0x40];
	uint8_t cert0[0x80];
	uint8_t cert80[0x80];
	uint32_t e;//5
	uint32_t serviceflag;//0000=blue , 01=red/brown
	uint32_t C1;//C1 00 00 00
	uint32_t color;//color: FFFF=blue , 62=brown , 04=red
	uint32_t FF;//FF 00 00 00
	uint32_t cid;
	uint32_t unk0;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint32_t FFFFFFFF;//FF FF FF FF
	uint8_t cert124[0x80];
};
#pragma pack(pop)

int isbabe_a1(TCHAR* infilename)
{
	int retvalue=0;
	BABEHDR hdr;

	FILE* f;
	if(_tfopen_s(&f,infilename,_T("rb")) == 0)
	{
		if(sizeof(hdr)==fread(&hdr,1,sizeof(hdr),f))
		{
			if(hdr.sig == 0xBEBA)
			{
				if(hdr.ver>=2 && hdr.ver<=4)
				{
					if(hdr.color ==COLOR_RED || hdr.color==COLOR_BROWN || hdr.color==COLOR_BLUE)
					{
						if(hdr.platform ==DB2000
							||hdr.platform == DB2001
							||hdr.platform == DB2010
							||hdr.platform == DB2012
							||hdr.platform == PNX5230
							||hdr.platform == DB2020
							)
						{
							retvalue = 1;
						}
					}
				}
			}
		}
		fclose(f);
	}else
	{
		retvalue = -1;
	}
	return retvalue;
}



A1CERT* findcert(int platform, int cid=0, int color=0)
{
	for(int i=0;i<sizeof(certz)/sizeof(certz[0]);i++)
	{
		if( (platform & certz[i].platform) && (certz[i].cid==cid) /*&& certz[i].color == color*/ )
		{
			if( (!cid && !color) || color == certz[i].color )
				return (A1CERT*)&certz[i].cert;
		}
	}
	return NULL;
}
A1CERT* findcert(int platform, char* name)
{
	for(int i=0;i<sizeof(certz)/sizeof(certz[0]);i++)
	{
		if( (platform & certz[i].platform) )
		{
			A1CERT* ptr=(A1CERT*)&certz[i].cert;
			if(strncmp(name, ptr->name, sizeof(ptr->name))==0)
				return ptr;
		}
	}
	return NULL;
}


int shaupdatefile(SHA_CTX& sha,FILE* f, int pos, int size)
{
	fseek(f,pos,SEEK_SET);
	while(size)
	{
		uint8_t tmp[64];
		int tmpsize=size>sizeof(tmp) ? sizeof(tmp): size;
		if(fread(tmp,1,tmpsize,f) !=tmpsize)
			return -1;
		SHA1_Update(&sha, &tmp[0],tmpsize);
		size-=tmpsize;
	}

	return 0;
}

static TCHAR* getplatformname(int platform)
{
	struct PN
	{
		int platform;
		TCHAR* name;
	} static const pn[]={
		DB2000   ,_T("DB2000")
		,DB2001   ,_T("DB2001")
		,DB2010   ,_T("DB2010")
		,DB2012   ,_T("DB2012")
		,PNX5230  ,_T("PNX5230")
		,DB2020   ,_T("DB2020")
	};

	for(int i=0;i<sizeof(pn)/sizeof(pn[0]);i++)
		if(pn[i].platform & platform)
			return pn[i].name;
	return _T("unknown");
}

static TCHAR* getcolorname(int color)
{
	struct PN
	{
		int color;
		TCHAR* name;
	} static const pn[]={
		COLOR_RED   ,_T("RED")
		,COLOR_BROWN,_T("BROWN")
		,COLOR_BLUE ,_T("BLUE")
		,COLOR_BLACK,_T("BLACK")
	};

	for(int i=0;i<sizeof(pn)/sizeof(pn[0]);i++)
		if(pn[i].color == color)
			return pn[i].name;
	return _T("unknown");
}

int decodehashes(BABEHDR* hdr,FILE* f,unsigned char* hashes1,unsigned char* hashes2,A1CERT* &cer)
{
	A1CERT* emproot = findcert(hdr->platform, hdr->z1 & 0x0F, COLOR_BLUE);//emp root
	if(!emproot)
	{
		_tprintf(_T("can't find root cert\n"));
		return -1;
	}

	if( (hdr->z1 & 0xF0) == 0 )
		cer= findcert(hdr->platform, ((A1CERT*)&hdr->certplace)->name);
	else
		cer = findcert(hdr->platform, hdr->cid, hdr->color);

	if(!cer)
	{
		_tprintf(_T("can't find cert\n"));
		return -1;
	}

	if(decodeblock( hashes1, 0x80, hdr->hash1, emproot->cert0 ))
	{
		_tprintf(_T("can't decrypt hashtab1\n"));
		//              return -1;
	}
	if(decodeblock( hashes2, 0x80, hdr->hash2, cer->cert0 ))
	{
		_tprintf(_T("can't decrypt hashtab2\n"));
		return -1;
	}
	return 0;
}

int checka1loader(BABEHDR* hdr,FILE* f)
{
	uint8_t hashes1[0x80];
	uint8_t hashes2[0x80];
	A1CERT* cer;

	if(decodehashes(hdr,f,hashes1,hashes2,cer)<0)
		return -1;

	//--------------------------
	unsigned char hash[20];
	SHA_CTX sha;
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr, 0x3C);
	SHA1_Update(&sha, (unsigned char*)cer, 0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart, 0x1C);
	SHA1_Final(hash, &sha);
	if(memcmp(hash,&hashes1[0],20))
	{
		_tprintf(_T("bad hash1 (header)\n"));
		//              return -1;
	}

	//--------------------------
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	if(shaupdatefile(sha,f,0x380,hdr->prologuesize1))
	{
		_tprintf(_T("error while processing prologue\n"));
		return -1;
	}
	SHA1_Final(hash, &sha);
	if(memcmp(hash,&hashes1[20],20))
	{
		_tprintf(_T("bad hash2 (header + prologue)\n"));
		//              return -1;
	}

	//--------------------------
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	SHA1_Update(&sha, (unsigned char*)&hdr->hash1,0xC0);
	SHA1_Final(hash, &sha);
	if(memcmp(hash,&hashes2[2],20))
	{
		_tprintf(_T("bad hash3 (header full)\n"));
		return -1;
	}

	//--------------------------
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	SHA1_Update(&sha, (unsigned char*)&hdr->hash1,0xC0);
	if(shaupdatefile(sha,f,0x380+hdr->prologuesize1,hdr->payloadsize1))
	{
		_tprintf(_T("error while processing payload\n"));
		return -1;
	}
	SHA1_Final(hash, &sha);

	if(memcmp(hash,&hashes2[22],20))
	{
		_tprintf(_T("bad hash4 (header full + payload)\n"));
		return -1;
	}

	return 0;
}

int checka1ssw(BABEHDR* hdr,FILE* f)
{
	unsigned char hashes1[0x80];
	unsigned char hashes2[0x80];
	A1CERT* cer;

	if(decodehashes(hdr,f,hashes1,hashes2,cer)<0)
		return -1;

	//--------------------------
	unsigned char hash[20];
	SHA_CTX sha;
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	SHA1_Final(hash, &sha);
	if(memcmp(hash,&hashes1[0],20))
	{
		_tprintf(_T("bad hash1 (header)\n"));
		//              return -1;
	}

	//--------------------------
	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	SHA1_Update(&sha, (unsigned char*)&hdr->hash1,0xC0);
	SHA1_Final(hash, &sha);
	if(memcmp(hash,&hashes2[2],20))
	{
		_tprintf(_T("bad hash3 (header full)\n"));
		return -1;
	}

	int hashsize=1;
	int blocks = hdr->payloadsize1;
	int sbsize = hdr->payloadsize2;
	unsigned long hptr=0x380;
	unsigned long hashtablesize;

	switch(hdr->ver)
	{
	case 2:
		hashtablesize = 0x100;
		_tprintf(_T("static hash\n"));
		break;
	case 3:
		hashtablesize = hashsize*blocks;
		_tprintf(_T("dynamic hash\n"));
		break;
	case 4: 
		hashsize=20;
		hashtablesize = hashsize*blocks;
		_tprintf(_T("full hash\n"));
		break;
	default:
		_tprintf(_T("error! unknown header version %x\n"),hdr->ver);
	}
	_tprintf(_T("number of blocks: %d (0x%X)\n"),blocks,blocks);

	ucbuffer hashes;
	hashes.resize(hashtablesize);

	fseek(f, hptr, SEEK_SET);
	if(hashtablesize != fread(&hashes[0],1,hashtablesize,f))
	{
		_tprintf(_T("can't read hashtable\n"));
		return -1;
	}

	SHA1_Init(&sha);
	SHA1_Update(&sha, (unsigned char*)hdr,0x3C);
	SHA1_Update(&sha, (unsigned char*)cer,0x1E8);
	SHA1_Update(&sha, (unsigned char*)&hdr->prologuestart,0x1C);
	SHA1_Update(&sha, (unsigned char*)&hdr->hash1,0xC0);
	for(int curblock=1;curblock<=blocks;curblock++)
	{
		uint32_t offset;
		uint32_t addresssize[2];
		if( sizeof(addresssize) != fread(addresssize,1,sizeof(addresssize),f))
		{
			_tprintf(_T("error! block %d out of file!\n"),curblock);
			return 1;
		}
		if(curblock==1)
		{
			offset = addresssize[0];
			switch(offset & 0x00FFFFFF)
			{
			case 0x00000000:
			case 0x00020000:
			case 0x000A0000:
				_tprintf(_T("superblock addr: 0x%X\n"),offset);
				_tprintf(_T("superblock size: 0x%X\n"),sbsize);
				break;
			case 0x00140000:
			default:
				_tprintf(_T("1st block addr: 0x%X\n"),offset);
				_tprintf(_T("1st block size: 0x%X\n"),sbsize);
				break;
			}
		}
		//memleak
		ucbuffer block;
		block.resize(addresssize[1]);

		if(addresssize[1]!=fread(&block[0],1,addresssize[1],f))
		{
			_tprintf(_T("error! block %d out of file!\n"),curblock);
			return -1;
		}
		if(curblock>2)
		{
			if(addresssize[0]!=offset)
				_tprintf(_T("warning! block %d addr %08X size %08X, expected address: %08X\n")
				,curblock,addresssize[0],addresssize[1],offset);
		}
		SHA1_Update(&sha, (unsigned char*)addresssize,sizeof(addresssize));
		SHA1_Update(&sha, block.data(),block.size());

		SHA_CTX shacopy;
		memcpy(&shacopy, &sha, sizeof(sha));

		SHA1_Final(hash, &shacopy);
		if(memcmp(&hash[20-hashsize],&hashes[hashsize*(curblock-1)],hashsize))
		{
			_tprintf(_T("error! %dst block's hash doesn't match a hdr value\n"),curblock);
			return -1;
		}
		offset = (addresssize[0] + addresssize[1] + 3) &~3;

	}

	SHA1_Final(hash, &sha);

	if(memcmp( &hashes2[22],hash,20 ))
	{
		_tprintf(_T("bad hash4 (final hash)\n"));
		return -1;
	}

	_tprintf(_T("//2do: hash2+hash5\n"));//hashes1[20], hashes2[42]
	return 0;
}

int checka1sfa(BABEHDR* hdr,FILE* f)
{
	/*
	unsigned char hashes1[0x80];
	unsigned char hashes2[0x80];
	A1CERT* cer;

	if(decodehashes(hdr,f,hashes1,hashes2,cer)<0)
	return -1;

	//--------------------------
	unsigned char hash[20];
	CSHA1 sha;
	sha.Reset();
	sha.Update((unsigned char*)hdr,0x3C);
	sha.Update((unsigned char*)cer,0x1E8);
	sha.Update((unsigned char*)&hdr->prologuestart,0x1C);
	sha.Final();
	sha.GetHash(hash);
	if(memcmp(hash,&hashes1[0],20))
	{
	_tprintf(_T("bad hash1 (header)\n"));
	//              return -1;
	}

	//--------------------------
	sha.Reset();
	sha.Update((unsigned char*)hdr,0x3C);
	sha.Update((unsigned char*)cer,0x1E8);
	sha.Update((unsigned char*)&hdr->prologuestart,0x1C);
	sha.Update((unsigned char*)&hdr->hash1,0xC0);
	sha.Final();
	sha.GetHash(hash);
	if(memcmp(hash,&hashes2[2],20))
	{
	_tprintf(_T("bad hash3 (header full)\n"));
	return -1;
	}


	*/
	_tprintf(_T("sfa unsupported\n"));
	return -1;
}

static TCHAR* getfiletypename(BABEHDR* hdr)
{
	switch(hdr->flags & 0x210)
	{       
	case 0:
		return _T("LOADER");
	case 0x200:
		return _T("SFA");
	case 0x210:
		return _T("SSW");
	}
	return _T("unknown");
}

int checkbabe_a1(TCHAR* infilename)
{
	int retvalue=0;
	FILE* f;
	if(_tfopen_s(&f,infilename,_T("rb")) == 0)
	{
		BABEHDR hdr;
		if(sizeof(hdr)==fread(&hdr,1,sizeof(hdr),f))
		{
			_tprintf(_T("platform %s, cid %d, color %s, filetype %s\n"),getplatformname(hdr.platform),hdr.cid, 
				getcolorname(hdr.color),getfiletypename(&hdr));

			switch(hdr.flags & 0x210)
			{
			case 0:
				retvalue = checka1loader(&hdr,f);
				break;
			case 0x200:
				retvalue = checka1sfa(&hdr,f);
				break;
			case 0x210:
				retvalue = checka1ssw(&hdr,f);
				break;
			}
		}
		fclose(f);
	}else
	{
		retvalue = -1;
	}


	return retvalue;
}