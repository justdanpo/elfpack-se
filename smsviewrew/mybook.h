#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include <set>

class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);

protected:
	~CMyBook();

	static int _HookCBKey( int key, int repeat_count, int mode, CMyBook* book, DISP_OBJ* disp );
	static int _HookAction(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);

	int HookCBKey( int key, int repeat_count, int mode, DISP_OBJ* disp );
	int HookAction( void *msg, BOOK* book, PAGE_DESC * page_desc, u16 event);

	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);

	TEXTID strid_viewmsg;
	TEXTID strid_view;
	TEXTID strid_forward;
	TEXTID strid_reply;
	TEXTID strid_stop;

	set<BOOK*> blockbooks;
public:
	CMyBook();

	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
