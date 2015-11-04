#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include <vector>
#include <string>

class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);

	struct STOREDBOOKNAME
	{
		BOOK* book;
		string name;
	};

protected:
	~CMyBook();

	static int _HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
	static int _HookCB_PageEnter2(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);

	static int _HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);

	int HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc);
	int HookCB_PageEnter2(void *msg, BOOK* book, PAGE_DESC * page_desc);

	int HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc);

	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);

	vector<STOREDBOOKNAME> books;
public:
	CMyBook();

	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
