#include "..\deleaker\mem2.h"
//#include "..\\include\Lib_Clara.h"

#include "mybook.h"

extern void elf_exit(void);

//эту структуру шлёт bookmanager
typedef struct
{
	BOOK * bookmanbook;
}BOOKMANMSG;



int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID,STR("Hello Wordl!\n\nExample elf.."),NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}

/*
int CMyBook::Accept(void *msg, CMyBook* book)
{
book->Hide(0);
return 1;
}

int CMyBook::Cancel(void *msg, CMyBook* book)
{
book->Free();
return 1;
}
*/

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "HW_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
//  DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG, CMyBook::Cancel )
//  DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::Accept )
DECLARE_PAGE_DESC_END

void CMyBook::OnBack(CBookBase* book, CGuiBase*)
{
	book->Free();
}

//бука создаётся
CMyBook::CMyBook() :CBook((char*)"example",&base_page)
{
	gui = new CGuiT<CMyDisp>(this, 0);
	
	gui->SetStyle(4);
	
	gui->SetTitleType(1);
	gui->SoftKeys_SetAction( ACTION_BACK, OnBack );
	
	gui->SoftKeys_Hide();
	gui->Show();
}

//бука убивается
CMyBook::~CMyBook()
{
	if(gui)gui->Free();
	//убийство книги приводит к убийству эльфа
	elf_exit();
}

