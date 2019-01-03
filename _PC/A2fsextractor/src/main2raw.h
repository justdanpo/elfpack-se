#include "stdafx.h"
#include <windows.h>

#include "..\..\common\babeheaders\babe_a2.h"

bool convert2raw(tcstring file,tcstring outfile);
/*
struct BABE3150
{
        WORD sig;
        BYTE verlo;
        BYTE verhi;
        DWORD color;
        DWORD platform; //0x200 - db3150
        DWORD some1;
        DWORD x[4];
        DWORD numblocks;
        DWORD x2;
        DWORD some12;
        DWORD some252;
        DWORD cid;
        DWORD x3[2];
        DWORD startaddr;
        DWORD x4[4];
        DWORD headersize;
};
*/