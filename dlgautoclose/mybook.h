#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include <map>

class CMyBook: public CBook
{
	struct BOOKANDGUI
	{
		const char* bookname;
		const char* pagename;
		int timeout;
	};

	static const BOOKANDGUI bkguis[];

	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();

	static int _HookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16);
	static void _TimerCB( u16 timerId, CMyBook* mybook);

	void TimerCB( u16 timerId );
	int HookCB(void *msg, CBook* book, PAGE_DESC * page_desc);

	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);

	map<u16,CBook*> bookstoclose;
public:
	CMyBook();

	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
