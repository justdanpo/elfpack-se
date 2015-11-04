#include "mybook.h"

#define MYBOOKNAME "book log elf"
#define MYBOOKBASEPAGE "book_log_BasePage"
#define ABOUTTEXT "book log"

extern void elf_exit();

int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}


int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID, STR(ABOUTTEXT), NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}


DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, MYBOOKBASEPAGE)
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook(MYBOOKNAME, &base_page)
{
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter, this, PAGE_HOOK_ADD_BEFORE);
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter2, this, PAGE_HOOK_ADD_AFTER);

	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _HookCB_PageExit, this, PAGE_HOOK_ADD_BEFORE);
}


CMyBook::~CMyBook()
{
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter, this, PAGE_HOOK_REMOVE);
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter2, this, PAGE_HOOK_REMOVE);

	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _HookCB_PageExit, this, PAGE_HOOK_REMOVE);
	elf_exit();
}


int CMyBook::FindBookExCB_Name( BOOK* book , int* param )
{
	return 0 == strcmp( book->xbook->name, (char*)param );
}

int CMyBook::FindBookExCB_Book( BOOK* book , int* param )
{
	return book == (BOOK*)param;
}


CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*) FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME );
}


int CMyBook::_HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB_PageEnter( msg, book, page_desc );
}

int CMyBook::_HookCB_PageEnter2(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB_PageEnter2( msg, book, page_desc );
}

int CMyBook::_HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB_PageExit( msg, book, page_desc );
}


int CMyBook::HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc)
{
	//store book ptr & name

	STOREDBOOKNAME tmp;

	tmp.book = book;
	tmp.name = book->xbook->name;

	for( int i=0; i< books.size(); i++ )
		tmp.name = "  "+tmp.name;

	books.push_back( tmp );

	debug_printf( "\n!!! page enter: book %x '%s', page '%s'\n", book, tmp.name.c_str(), page_desc->name );

	return 0;
}

int CMyBook::HookCB_PageEnter2(void *msg, BOOK* book, PAGE_DESC * page_desc)
{
	bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );

	STOREDBOOKNAME tmp = books.back();
	books.pop_back();

	debug_printf("\n!!! page enter end: book %x '%s', page '%s'%s\n", book, tmp.name.c_str(), page_desc->name, exists ? "." : " killed!");

	if( exists )
	{
		int sz = book->xguilist->guilist->FirstFree;
		debug_printf("\n!!! guilist size %d\n", sz);
		for(int i=0; i< book->xguilist->guilist->FirstFree; i++ )
		{
			debug_printf("\n!!!   guilist[%d]=%s\n", i, DispObject_GetName( GUIObject_GetDispObject( (GUI*) List_Get(book->xguilist->guilist, i) ) ) );
		}
	}

	return 0;
}

int CMyBook::HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc)
{
	debug_printf("\n!!! page exit: book %x '%s', page '%s'\n", book, book->xbook->name, page_desc->name);

	return 0;
}
