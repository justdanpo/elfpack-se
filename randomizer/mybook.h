#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "..\include\cfg_items.h"

class CMyBook:public CBook
{
    int evtdatastatepos;

    DECLARE_PAGE_DESC_MEMBER(base_page);

    enum
    {
        CALLMANAGER_IDLE            =0,
        CALLMANAGER_CALL_CONNECTED  =1,
        CALLMANAGER_CALL_SETUP      =2,
        CALLMANAGER_CALL_ALERT      =4,
        CALLMANAGER_CALL_WAITING    =5,
        CALLMANAGER_CALL_TERMINATED =6
    };
protected:
    ~CMyBook();

    void UpdateRingtone();

    static int FindBookExCallback(BOOK* book ,int * param);

    static int ReconfigElf( RECONFIG_EVENT_DATA* mess, CMyBook* book );
    static int TerminateElf(CBook** pbookmanbook, CBook* book);
    static int ShowAuthorInfo(CBook** pbookmanbook, CBook* book);
    static int OnCallManagerEvent(char* evtdata, CMyBook* book);
public:
    CMyBook();

    static CMyBook* GetExisting();

};

#endif
