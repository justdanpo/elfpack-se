#include "mybook.h"

extern void elf_exit(void);


int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID,STR("Hello CWordl!\n\nExample elf.."),NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}

int CMyBook::Accept(void *msg, CMyBook* book)
{
	book->Hide(0);
	return 1;
}

int CMyBook::Cancel(void *msg, CMyBook* book)
{
	book->Hide(0);
	book->gui->Free();
	book->gui=NULL;
	return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "HW_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::Accept )
DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG, CMyBook::Cancel )
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  :CBook("example",&base_page)
{
	lamp=0;

	StatusIndication_ShowNotes(STR("Hello World!"));
	timer=TIMER_SET(500,onTimer,this);
	gui = new CGuiT<CMyDisp>(this, 0);
	gui->SetStyle(UI_OverlayStyle_TrueFullScreen);
	gui->SetTitleType(UI_TitleMode_None);
	gui->SoftKeys_Hide();
	gui->Show();
}

//бука убивается
CMyBook::~CMyBook()
{
	Timer_Kill(&timer);
	if(gui)gui->Free();
	StatusIndication_ShowNotes(EMPTY_TEXTID);
	SetLampLevel(0);
	//убийство книги приводит к убийству эльфа
	elf_exit();
}


void CMyBook::onTimer (u16 unk , CMyBook* book)
{
	SetLampLevel(book->lamp^=0x10);

	TIMER_RESET(&book->timer, 500, onTimer, book);
}

