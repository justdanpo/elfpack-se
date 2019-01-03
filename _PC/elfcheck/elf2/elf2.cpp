// elf2.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "elfhdr.h"

#define MAX_VALUE 0xFFF

char swi[MAX_VALUE+1];
int isfulldebug;

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


PBYTE elf;
long elfsize;

PBYTE of;
long ofs;


int _tmain(int argc, _TCHAR* argv[])
{

	if(argc<3)return printf("usage: elfcheck.exe elf library.vkp [f]\n");
	if(argc>3)isfulldebug= !strcmpi(argv[3],"f");


	elf=myreadfile(argv[1],elfsize);
	if(!elf)return printf("can't read elf\n");

	FILE* f=fopen(argv[2],"r");
	//	of=myreadfile(argv[2],ofs);
	if(!f)return printf("can't read library.vkp\n");

	//check elf anf fill patch data
	ELF* elfhdr=(ELF*)elf;
	if(elfhdr->Signature!=0x464c457f)return printf("bad elf\n");

	PBYTE tables=elf+elfhdr->Program_header_table;
	for(int i=0;i<elfhdr->Program_header_table_entry_count;i++,tables+=elfhdr->Program_header_table_entry_size)
	{
		PROGTBLENTRY* entryptr=(PROGTBLENTRY*)tables;
		if(isfulldebug)printf("entry %d: %08X/%X\n",i+1,entryptr->Virtual_address,entryptr->Size_in_file);
		if(entryptr->Size_in_file)
		{
			for(size_t j= (4-entryptr->Virtual_address)&3 ;j+3<entryptr->Size_in_file;j+=4)
			{
				if(elf[entryptr->Physical_offset+j+3]==0xEF)
				{
					int swinum=elf[entryptr->Physical_offset+j+2]*0x10000
						+elf[entryptr->Physical_offset+j+1]*0x100
						+elf[entryptr->Physical_offset+j+0];
					swinum&=~0x8000;
					if(swinum>=0x110 && swinum<=MAX_VALUE)
						swi[swinum]=1;
				}
			}
		}
	}
	//---------------------------------------------------------
	if(isfulldebug)
		for(int i=0x110;i<=MAX_VALUE;i++)
			if(swi[i])printf("swi 0x%X used\n",i);

	while(!feof(f))
	{
		char tmp[512];
		fgets(tmp,sizeof(tmp),f);
		int i;
		if(tmp[0]!='+' && tmp[0]!='-')
			if(sscanf(tmp,"%x:",&i))
			{
				i/=4;
				if(i>=0x110 && i<=MAX_VALUE)swi[i]=0;
			}
	}
	fclose(f);

	int allok=1;
	for(int i=0x110;i<=MAX_VALUE;i++)
		if(swi[i])
		{
			printf("swi 0x%X used but not found in lib\n",i);
			allok=0;
		}
	if(allok)printf("all ok\n");
	_getch();
	return 0;
}

