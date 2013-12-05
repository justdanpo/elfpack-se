#include "..\deleaker\mem2.h"
#include "mybook.h"
#include "rand.h"

extern void elf_exit(void);


int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID,STR("tic tac toe\n\n(c) den_po "),NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}

/*
void CMyBook::OnAccept(CBookBase* book,void *)
{
((CGameDisp*) ((CMyBook*)book)->gui->GetDISPObj())->StartGame();
//book->Hide(0);
}
*/

/*
int CMyBook::Cancel(void *msg, CMyBook* book)
{
//      book->Hide(0);
//      book->gui->Free();
//      book->gui=NULL;
book->Free();
return 1;
}
*/
void CMyBook::OnBack(CBookBase* book, CGuiBase*)
{
	book->Free();
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "TTTAI_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
//DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::Accept )
//DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG, CMyBook::Cancel )
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  :CBook("TTTAI_Book",&base_page)
{
	DATETIME dt;
	REQUEST_DATEANDTIME_GET(0,&dt);

	srand( (dt.time.sec<<16) | (dt.time.min<<8) | (dt.time.hour) );

	gui = new CGuiT<CGameDisp>(this, 0);

	gui->SetTitleType(1);
	//  gui->SetStyle(4);

	gui->SoftKeys_SetAction( ACTION_BACK, OnBack );
	TEXTID strid_exit;
	textidname2id(L"GUI_EXIT_SK", -1, &strid_exit);
	gui->SoftKeys_SetText( ACTION_BACK, strid_exit );

	//gui->SoftKeys_Hide();

	gui->Show();
}

//бука убивается
CMyBook::~CMyBook()
{
	if(gui)gui->Free();
	//убийство книги приводит к убийству эльфа
	elf_exit();
}
