// findhash.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <map>
#include <vector>
#include "..\fxacommon\mystring.h"
#include "..\fxacommon\fxacommon.h"

typedef std::pair<unsigned int,cstring> HASHMAPPAIR;
typedef std::map<unsigned int,cstring> HASHMAP;
typedef std::map<unsigned int,cstring>::iterator HASHMAPI;


std::set<int> hashes;
char checkflags[65536];

int checkhash(TCHAR* s, int p, int l)
{
	int hash=0x1505;
	for(int i=0;i<l;i++)
	{
		if( (unsigned)s[i+p]<32 || (unsigned)s[i+p]>127 || i>100)
			return 0;
		hash=hash*0x21+_totlower(s[i+p]);
		if(checkflags[hash&0xFFFF])
		{
			if(hashes.find(hash)!=hashes.end())
			{
				TCHAR tmp[101];
				_tcsncpy(tmp,s+p,i+1);
				tmp[i+1]=0;
				_tprintf(_T("%08x [h %08x] %s\n"), p*sizeof(TCHAR), hash,tmp);
			}
		}
	}
	return 1;
}


HASHMAP hashmap;

void rtrim(char* str)
{
	for( int l=strlen( str ); l > 0 && str[l-1]<' ' ; l--)
		str[l-1] = 0;
}

void createhashlist(TCHAR* arg0)
{
	cstring tmpfilename( arg0 );

	int pos=tmpfilename.rfind( _T('\\') );
	if( pos>0 )pos++;
	tmpfilename.erase( pos );
	tmpfilename += _T("names.txt");

	FILE* infile = _tfopen( tmpfilename.c_str(), _T("r") );

	if(infile!=NULL)
	{
		while(!feof(infile))
		{
			char temp[256];
			fgets( temp, sizeof(temp)/sizeof(temp[0]), infile );

			rtrim(temp);
			hashmap.insert( HASHMAPPAIR(gethash(toustring(temp).c_str()), tocstring(temp)) );
		}

		fclose(infile);

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	createhashlist( argv[0] );

	memset(checkflags, 0, sizeof(checkflags));
	FILE* f=_tfopen(_T("hashes.txt"),_T("r"));
	while(!feof(f))
	{
		int x;
		if(_ftscanf(f,_T("%x"),&x))
		{
			if(hashmap.find(x)==hashmap.end())
			{

				hashes.insert(x);
				checkflags[x&0xFFFF]=1;
			}
		}
	}
	fclose(f);

#ifdef UNICODE
	f=_tfopen(_T("phone_app.cxc"),_T("rb"));
#else
	f=_tfopen(_T("ARCHIVE.FXA"),_T("rb"));
#endif
	fseek(f,0,SEEK_END);
	int len=ftell(f)/sizeof(TCHAR);
	fseek(f,0,SEEK_SET);

	std::vector<TCHAR> indata;
	indata.resize(len);

	fread(&indata[0], sizeof(TCHAR), len, f);

	fclose(f);

	int max=indata.size();
	for(int i=0; i<max; i++)
	{
		if( (i&0xFFF) ==0 )
			_tprintf(_T("%08x/%08x\x0d"),i,max);

		checkhash(&indata[0],i,max-i);
	}


	return 0;
}

