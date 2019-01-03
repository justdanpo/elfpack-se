// fxapack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <vector>
#include "..\..\common\stdint.h"
#include "..\fxacommon\fxacommon.h"
#include "..\fxacommon\mystring.h"

void collectdata(std::vector<FXAITEM> &fxai, std::vector<std::vector<uint8_t>> &data, cstring a_curdir)
{
	struct _tfinddata_t c_file;
	intptr_t hFile;

	cstring curdir = _T("archive.fxa.out")+a_curdir;
	cstring curmask = curdir+_T("/*.*");

	// Find first .c file in current directory 
	if( (hFile = _tfindfirst( curmask.c_str(), &c_file )) != -1L )
	{
		do {
			if( 0==(c_file.attrib & _A_SUBDIR ))
			{
				FXAITEM i;

				i.path.set(gethash(toustring(a_curdir).c_str()));
				i.file.set(gethash(toustring(c_file.name).c_str()));


				uint32_t temp;
				TCHAR tmpname[13];
				if(1==_stscanf_s(a_curdir.c_str(), _T("/%08X"), &temp, sizeof(temp)))
				{
					_stprintf_s( tmpname, sizeof(tmpname)/sizeof(tmpname[0]), _T("/%08X"), temp);
					if(0==_tcsicmp( tmpname, a_curdir.c_str() ))
						i.path.set(temp);
				}

				if(1==_stscanf_s(c_file.name, _T("%08X"), &temp, sizeof(temp)))
				{
					_stprintf_s( tmpname, sizeof(tmpname)/sizeof(tmpname[0]), _T("%08X"), temp);
					if(0==_tcsicmp( tmpname, c_file.name ))
						i.file.set(temp);
				}

				TCHAR fname[256];
				_stprintf_s(fname,sizeof(fname)/sizeof(fname[0]),_T("/%s"),c_file.name);

				FILE* f=_tfopen((curdir+fname).c_str(), _T("rb"));
				fseek(f,0,SEEK_END);
				int fsize=ftell(f);
				fseek(f,0,SEEK_SET);

				std::vector<uint8_t> tmpdata;
				tmpdata.resize(fsize);

				fread(&tmpdata[0],1,fsize,f);
				fclose(f);

				fxai.push_back(i);
				data.push_back(tmpdata);
			}else if( _tcscmp(c_file.name,_T(".")) && _tcscmp(c_file.name,_T("..")))
			{
				collectdata(fxai, data, a_curdir+_T("/")+c_file.name);
			}

		} while( _tfindnext( hFile, &c_file ) == 0 );
		_findclose( hFile );
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<FXAITEM> fxai;
	std::vector<std::vector<uint8_t>> data;

	collectdata(fxai,data, _T(""));

	if(fxai.size()>0)
	{
		INTBE fsize;
		fsize.set(fxai.size());

		uint32_t headersize = fxai.size()*sizeof(FXAITEM);
		uint32_t datasize = 0;

		for(int i=0;i<fxai.size();i++)
		{
			fxai[i].offset.set(4+headersize+datasize);
			fxai[i].size.set(data[i].size());
			datasize+=data[i].size();
		}

		FILE* f=_tfopen(_T("new archive.fxa"),_T("wb"));
		fwrite(&fsize, sizeof(fsize),1, f);
		fwrite(&fxai[0], 1, headersize, f);
		for(int i=0;i<data.size();i++)
		{
			fwrite(&data[i][0], 1, data[i].size(), f);
		}
		fclose(f);
		_tprintf(_T("file created\n"));
	}

	return 0;
}

