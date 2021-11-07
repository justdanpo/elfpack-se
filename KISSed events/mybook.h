#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

class CMyBook:public CBook
{
	BOOL callestablished;
	u16 timer; // timer ID

	int eventmask;

	int evtdatastatepos;

	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
	static int FindBookExCallback(BOOK* book ,int * param);
	static void onTimer(u16 unk , CMyBook* book);
	void CheckEvents();
public:
	CMyBook();


	static int TerminateElf(CBook** pbookmanbook, CBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CBook* book);
	static int OnMissedEvent(void*, CMyBook* book);
	static int OnCallManagerEvent(char* evtdata, CMyBook* book);

	static CMyBook* GetExisting();


	enum
	{
		CALLMANAGER_IDLE            =0,
		CALLMANAGER_CALL_CONNECTED  =1,
		CALLMANAGER_CALL_SETUP      =2,
		CALLMANAGER_CALL_ALERT      =4,
		CALLMANAGER_CALL_WAITING    =5,
		CALLMANAGER_CALL_TERMINATED =6
	};

};

#endif
