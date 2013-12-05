#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "maindisp.hpp"

class CMyBook:public CBook
{
	CGuiBase* gui;

	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
	static int FindBookExCallback(BOOK* book ,int * param);
public:
	CMyBook();


	static int TerminateElf(CBook** pbookmanbook, CBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CBook* book);

	static CMyBook* GetExisting();
};

#endif
