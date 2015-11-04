#include "mybook.h"
#include "../classlib/softkeyutil.hpp"

#define MYBOOKNAME "disableno"
#define MYBOOKBASEPAGE "disableno_BasePage"
#define ABOUTTEXT "disable no"


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


CMyBook::CMyBook()  :CBook(MYBOOKNAME, &base_page), timerid(0), lastbook(NULL), lastdisp(NULL)
{
    MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _PageHookCB, this, PAGE_HOOK_ADD_AFTER);
    MODIFYUIPAGEHOOK(SLIDER_CLOSE_EVENT, _PageHookCB, this, PAGE_HOOK_ADD_BEFORE);
}


CMyBook::~CMyBook()
{
    if(timerid)
        Timer_Kill( &timerid );
    MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _PageHookCB, this, PAGE_HOOK_REMOVE);
    MODIFYUIPAGEHOOK(SLIDER_CLOSE_EVENT, _PageHookCB, this, PAGE_HOOK_REMOVE);
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


int CMyBook::_PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
    return mybook->PageHookCB( msg, book, page_desc, event );
}


int CMyBook::PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event)
{
    bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );

    if( exists )
    {
        if(!strcmp(book->xbook->name, "MTCallBook") && !strcmp(page_desc->name,"AskAnswer_Page"))
        {
            if (event==SLIDER_CLOSE_EVENT)
            {
                if (timerid)
                    return BLOCK_EVENT_IN_THIS_SESSION;
            }
            else
            {
                lastbook = book;
                CGuiBase* gui = reinterpret_cast<CGuiBase*>( List_Get(book->xguilist->guilist, 0) );
                lastdisp = gui->GetDISPObj();
              
                timerid = TIMER_SET(3000, _TimerProc, this);
              
                CSoftKeyDesc* sk = FindActionSoftkey( lastdisp, book, ACTION_NO, 0 );
              
                if(sk)
                {
                    sk->GetDisabledAction() = dummy;
                    sk->GetEnabled() = false;
                }
              
                sk = FindActionSoftkey( lastdisp, book, 2000, 1 );
              
                if(sk)
                {
                    sk->GetDisabledAction() = dummy;
                    sk->GetEnabled() = false;
                }
            }
        }
    }

    return 0;
}



void CMyBook::_TimerProc( u16 timerID, CMyBook* book )
{
    book->TimerProc( timerID );
}

void CMyBook::TimerProc( u16 timerID  )
{
    Timer_Kill(&timerid);
    CDispObjBase* dobj = reinterpret_cast<CDispObjBase*>( Display_GetFocusedDispObject(0) );
    if(dobj = lastdisp)
    {
        if(UI_APP_SESSION * apsess=root_list_get_session(root_list_get_session_count()-1))
        {
            CBook* book = reinterpret_cast<CBook*>( SESSION_GetTopBook(apsess) );
            if(book == lastbook)
            {
                CSoftKeyDesc* sk = FindActionSoftkey( lastdisp, book, ACTION_NO, 0 );

                if(sk)
                {
                    sk->GetEnabled() = true;
                }

                sk = FindActionSoftkey( lastdisp, book, 2000, 1 );

                if(sk)
                {
                    sk->GetEnabled() = true;
                }
            }
        }
    }
}

void CMyBook::dummy(BOOK*, GUI*)
{
}
