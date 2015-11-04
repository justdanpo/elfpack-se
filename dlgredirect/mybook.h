#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);

protected:
	~CMyBook();

	static int _HookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);

	int HookCB2(void *msg, CBook* book, PAGE_DESC * page_desc);

	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);

	void UpdateLanguage();

	wchar_t OBX_FILE_DELETED_TXT[64];
	wchar_t OBX_FOLDER_DELETED_TXT[64];
	wchar_t OBX_FOLDER_CREATED_TXT[64];

	CGuiBase* feedback;

	void ShowText( TEXTID strid );
	static void FeedbackTimeout( CMyBook* );

public:
	CMyBook();

	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);
	static int OnLangChanged(void* msg, CMyBook* book);

	static int Accept(void *msg, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
