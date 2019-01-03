#include "stdafx.h"
#include "main2raw.h"

PBYTE myreadfile(TCHAR* fname,long& fs)
{
        FILE* f;
        PBYTE r;
        if( (f=_tfopen(fname,_T("rb"))) == NULL )return NULL;
        fseek(f,0,SEEK_END);
        fs=ftell(f);
        fseek(f,0,SEEK_SET);
        r=(PBYTE)malloc(fs);
        fread(r,fs,1,f);
        fclose(f);
        return r;
}

bool convert2raw(tcstring file,tcstring outfile)
{
        long fs;
        PBYTE f=myreadfile((TCHAR*)file.c_str(),fs);
        if(!f)
        {
                _tprintf(_T("can't read file\n"));
                return false;
        }
        if(
                (((BABEHDR_A2*)f)->sig != 0xBEBA)
                ||
                (
                 (((BABEHDR_A2*)f)->targetchipclass !=DB3150)
                 &&
                 (((BABEHDR_A2*)f)->targetchipclass !=DB3200)
                 &&
                 (((BABEHDR_A2*)f)->targetchipclass !=DB3210)
                 &&
                 (((BABEHDR_A2*)f)->targetchipclass !=DB3350)
                )
        )
        {
                _tprintf(_T("bad file\n"));
                return false;
        }
        long startaddr = ((BABEHDR_A2*)f)->payload_destaddress;
        FILE* of=_tfopen(outfile.c_str(),_T("wb"));
        long p= ( ((BABEHDR_A2*)f)->hdrlen+7 )&0xFFFFFFF8;
		for(unsigned int i=0; i<((BABEHDR_A2*)f)->hashlist_numberofblocks_values; i++)
        {
                DWORD awaitedlen=*(DWORD*)(&f[p])-startaddr;
                fseek(of,awaitedlen,SEEK_SET);
                fwrite( &f[p+8], *(DWORD*)(&f[p+4]),1,of);
                p+=*(DWORD*)(&f[p+4])+8;
        }
        fclose(of);

        return true;
}