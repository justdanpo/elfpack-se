#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\..\include\Lib_Clara.h"
#include "..\..\classlib\cl.h"
#include "..\..\include\cfg_items.h"

class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
	static int FindBookExCallback(BOOK* book , int* param);
public:
	CMyBook();

	static int TerminateElf(CBook** pbookmanbook, CBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CBook* book);
	static int _OnInternetConnected(void*, CBook* book);
	static int ReconfigElf(RECONFIG_EVENT_DATA* mess, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
