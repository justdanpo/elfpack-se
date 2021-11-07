#include "..\deleaker\mem2.h"
#include "mybook.h"
#include "consts.h"
#include <stddef.h>

extern void elf_exit(void);

int CMyBook::TerminateElf(CBook** pbookmanbook, CBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CBook* book)
{
	MessageBox(EMPTY_TEXTID,TextID_Create(WABOUTTEXT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}

int CMyBook::OnMissedEvent(void*, CMyBook* book)
{
	if(! book->callestablished )
		book->CheckEvents();
	return 1;
}

int CMyBook::OnCallManagerEvent(char* evtdata, CMyBook* book)
{
	switch(evtdata[book->evtdatastatepos])
	{
	case CALLMANAGER_CALL_SETUP:
	case CALLMANAGER_CALL_ALERT:
		if(book->timer)
			Timer_Kill(&book->timer);
		book->callestablished = true;
		break;
	case CALLMANAGER_CALL_TERMINATED:
		book->callestablished = false;
		book->CheckEvents();
		break;
	}
	return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "HW_BasePage")
	DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
	DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
	//DECLARE_PAGE_DESC_MSG( MISSED_CALL_EVENT_TAG, CMyBook::OnMissedEvent )
	DECLARE_PAGE_DESC_MSG( UI_MESSAGING_SMS_RECEIVED_EVENT_TAG, CMyBook::OnMissedEvent )
	DECLARE_PAGE_DESC_MSG( UI_MESSAGING_MMS_RECEIVED_EVENT_TAG, CMyBook::OnMissedEvent )
	DECLARE_PAGE_DESC_MSG( ON_CALLMANAGER_EVENT_TAG, CMyBook::OnCallManagerEvent )
DECLARE_PAGE_DESC_END

CMyBook::CMyBook()  :CBook(MAINBOOKNAME, &base_page)
{
	callestablished = false;
	timer = 0;

	switch(GetChipID() & CHIPID_MASK)
	{
	case CHIPID_DB2000:
	case CHIPID_DB2010:
		eventmask=1|2|4|8;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
		evtdatastatepos = offsetof(CALLMANAGER_EVENT_DATA, CallState);
		break;
	case CHIPID_DB2020:
		eventmask=4|8|0x10|0x20;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
		evtdatastatepos = offsetof(CALLMANAGER_EVENT_DATA, CallState);
		break;
	case CHIPID_DB3150:
	case CHIPID_DB3200:
	case CHIPID_DB3210:
	case CHIPID_DB3350:
		eventmask=4|8|0x10|0x20|0x40|0x80;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS|MISSED_CALENDAR|MISSED_TASKS
		evtdatastatepos = offsetof(CALLMANAGER_EVENT_DATA_A2, CallState);
		break;
	default:
		eventmask=0;
		evtdatastatepos = 0;
	}

	CheckEvents();
}

CMyBook::~CMyBook()
{
	if(timer)
		Timer_Kill(&timer);

	elf_exit();
}


void CMyBook::onTimer (u16 unk , CMyBook* book)
{
	char* me=MissedEvents();
	if( book->eventmask & *me )
	{
		PlaySystemSound(8);//reminder.3gp; should we add that to dyn_const?
	}
	
	book->timer = 0;
	book->CheckEvents();
}

int CMyBook::FindBookExCallback(BOOK* book ,int * param)
{
	return 0 == strcmp( book->xbook->name, (char*)param);
}

CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*)FindBookEx(FindBookExCallback, (int*)MAINBOOKNAME);
}

void CMyBook::CheckEvents()
{
	if(timer)
		Timer_Kill(&timer);

	char* me=MissedEvents();
	if( eventmask & *me )
		timer = TIMER_SET(60*1000, onTimer, this);
}
