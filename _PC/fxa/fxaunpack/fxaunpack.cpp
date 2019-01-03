// fxaunpack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <map>
#include "..\..\common\stdint.h"
#include "..\fxacommon\mystring.h"
#include "..\fxacommon\fxacommon.h"

typedef std::pair<unsigned int,cstring> HASHMAPPAIR;
typedef std::map<unsigned int,cstring> HASHMAP;
typedef std::map<unsigned int,cstring>::iterator HASHMAPI;

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

cstring hash2name(unsigned int hash)
{
	HASHMAPI i=hashmap.find( hash );
	if( i!=hashmap.end() )
	{
		return i->second;
	}
	TCHAR ou[9];
	_sntprintf(ou,sizeof(ou)/sizeof(ou[0]),_T("%08x"),hash);
	return ou;
}

cstring reslash(cstring s)
{
	int p;
	while(-1 != (p=s.find(_T('/'))) )
		s[p]='%';
	return s;
}

void enshuredir(cstring s)
{
	int p=-1;
	while(-1 != (p=s.find(_T('/'), p+1) ) )
	{
		cstring tmp=s.substr(0,p);
		_tmkdir(tmp.c_str());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	createhashlist( argv[0] );

	FILE* infile = _tfopen(_T("archive.fxa"),_T("rb"));
	if(!infile)
		return _tprintf(_T("can't open archive.fxa\n"));

	INTBE itemscount;

	fread(&itemscount, sizeof(itemscount), 1, infile);

	std::vector<FXAITEM> items;
	items.resize(itemscount.get());

	fread(&items[0], sizeof(FXAITEM), itemscount.get(), infile);

	for(uint32_t i=0; i<itemscount.get(); i++)
	{
		fseek( infile, items[i].offset.get(), SEEK_SET );
		std::vector<uint8_t> tmpfile;
		tmpfile.resize(items[i].size.get());
		fread(&tmpfile[0],sizeof(uint8_t),items[i].size.get(),infile);

		cstring tmpname=hash2name(items[i].path.get())
			+_T("/")
			+hash2name(items[i].file.get());
	
		if(tmpname.size()>0 && tmpname[0]==_T('/'))
			tmpname.erase(0,1);

		if(argc>1)
			tmpname=reslash(tmpname);

		cstring outfilename = _T("archive.fxa.out/") + tmpname;		

		enshuredir(outfilename);

		FILE* outf=_tfopen(outfilename.c_str(),_T("wb"));
		fwrite(&tmpfile[0],sizeof(uint8_t),items[i].size.get(),outf);
		fclose(outf);

	}

	fclose(infile);
	return 0;
}

