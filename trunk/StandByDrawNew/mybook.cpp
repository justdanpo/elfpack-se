#include "..\deleaker\mem2.h"
#include "mybook.h"
#include "consts.h"

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

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "HW_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  :CBook(MAINBOOKNAME, &base_page)
{
	gui = new CGuiT<CMainDisp>( static_cast<CBook*>( Find_StandbyBook() ), 0 );

	gui->SetStyle( UI_OverlayStyle_FullScreen );
	gui->SetTitleType( UI_TitleMode_None );

	gui->SetZOrder( GuiObjectZOrderBelow, static_cast<CGuiBase*>( SBY_GetMainInput(Find_StandbyBook()) ) );

	gui->GetDISPObj()->Show(1);//dispobj show instead of gui show to prevent focus change
}

//бука убивается
CMyBook::~CMyBook()
{
	if( gui )
		gui->Free();

	elf_exit();
}

int CMyBook::FindBookExCallback(BOOK* book ,int * param)
{
	return 0 == strcmp( book->xbook->name, (char*)param);
}

CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*)FindBookEx(FindBookExCallback, (int*)MAINBOOKNAME);
}
