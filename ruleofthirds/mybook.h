#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\include\cfg_items.h"
#include "..\classlib\cl.h"
#include <vector>
#include <string>
#include "Accelerometer\export\AccelerometerDll.h"

class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);
	
protected:
	~CMyBook();
	
	static int _PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
	static int _PageHookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
	
	int PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event);
	int PageHookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event);
	
	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);
	
	static int ReconfigElf( RECONFIG_EVENT_DATA* mess, CMyBook* book );
	
	ACCELEROMETER_DLL_DATA * acc_dll;
	int type;
	
public:
	CMyBook();
	
	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);
	
	static CMyBook* GetExisting();
	
	DISP_OBJ_ONREDRAW_METHOD oldOnRedraw;
	DISP_OBJ_ONKEY_METHOD oldOnKey;
	
	static void NewOnRedraw(CDispObjBase*, int, int, int);
	static void NewOnKey(CDispObjBase*, int scan_code, int unk, int repeat_num, int key_mode);
	
};

#endif
