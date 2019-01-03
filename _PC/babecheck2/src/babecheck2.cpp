// babecheck2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "babe_a1.h"
#include "babe_a2.h"

int checkfile(TCHAR* infilename)
{
	int nErr = isbabe_a1(infilename);
	if(nErr<0)
		return nErr;
	if(nErr)
		return checkbabe_a1(infilename);

	nErr = isbabe_a2(infilename);
	if(nErr<0)
		return nErr;
	if(nErr)
		return checkbabe_a2(infilename);


	_tprintf(_T("unsupported file format\n"));
	return -1;
}

void showinfo()
{
	_tprintf(_T("some info here\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf(_T("babecheck [sswcheck] v2.1 by den_po\n\n"));
	if(argc<=1)
	{
		showinfo();
		return 0;
	}

	_tprintf(_T("checking file %s\n"),argv[1]);
	if(!checkfile(argv[1]))
	{
		_tprintf(_T("ok\n"));
		return 0;
	}
	return -1;
}

