#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include <vector>
#include <string>

class CMyBook: public CBook
{
    DECLARE_PAGE_DESC_MEMBER(base_page);

protected:
    ~CMyBook();

    static int _PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
    static void _TimerProc( u16 timerID, CMyBook* );

    int PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event);
    void TimerProc( u16 timerID );

    static int FindBookExCB_Name(BOOK* book ,int* param);
    static int FindBookExCB_Book(BOOK* book ,int* param);

    static void dummy(BOOK*, GUI*);

public:
    CMyBook();

    static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
    static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);

    static CMyBook* GetExisting();

    u16 timerid;
    CBook* lastbook;
    CDispObjBase* lastdisp;
};

#endif
