#include "mybook.h"

#define CHECKTIMEINTERVAL 500

#define MYBOOKNAME "AutoClose elf"
#define MYBOOKBASEPAGE "AutoClose_BasePage"
#define ABOUTTEXT "dlgautoclose"


extern void elf_exit(void);


const CMyBook::BOOKANDGUI CMyBook::bkguis[]=
{
	"USSDBook", "USSD_Notify_Page", 60000,
	"MSG UI Book", "MSG_UI_DetailedStatus_Page", 5000,
	"DataDownloadBook", "DataDownload_UseNow_Page", 60000
};


struct BOOKTOCLOSE
{
	BOOK* book;
	u16 timer;
};


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
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB, this, PAGE_HOOK_ADD_BEFORE);
}


CMyBook::~CMyBook()
{
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB, this, PAGE_HOOK_REMOVE);

	for( map<u16, CBook*>::iterator i = bookstoclose.begin(); i != bookstoclose.end(); ++i )
	{
		u16 timer = i->first;
		Timer_Kill( &timer );
	}

	SUBPROC( mfree_adr(), &ELF_BEGIN );
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


int CMyBook::_HookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16)
{
	return mybook->HookCB( msg, book, page_desc );
}


void CMyBook::_TimerCB(u16 timerId , CMyBook* mybook)
{
	mybook->TimerCB( timerId );
}


int CMyBook::HookCB(void *msg, CBook* book, PAGE_DESC * page_desc)
{
	if( book )
		for(int i=0; i<MAXELEMS(bkguis); i++)
			if( !strcmp( book->xbook->name, bkguis[i].bookname )
			   && !strcmp( page_desc->name, bkguis[i].pagename ) )
			{
				if( !bkguis[i].timeout )
				{
					book->Free();
					return BLOCK_EVENT_IN_THIS_SESSION;
				}
				else
					bookstoclose.insert( pair<u16, CBook*>( TIMER_SET(  bkguis[i].timeout, _TimerCB, this ), static_cast<CBook*>(book) ) );
				break;
			}
	return 1;
}

void CMyBook::TimerCB( u16 timerId )
{
	map<u16,CBook*>::iterator i = bookstoclose.find( timerId );
	if( i != bookstoclose.end() )
	{
		CBook* bk = static_cast<CBook*>( FindBookEx( FindBookExCB_Book, (int*)i->second ) );

		if( bk )
			bk->Free();

		bookstoclose.erase( i );
	}
}
