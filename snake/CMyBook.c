#include "myclass.h"
void elf_exit(void)
{
  DEBUG;
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};
int CMyBook::OnMainPage(void*,CMyBook* mbk)
{
  DEBUG;
  mbk->gui=new CGuiT<CGameDisp>(mbk,0);
  mbk->gui->SetStyle(4);
  mbk->gui->SetTitleType(1);
  mbk->gui->SoftKeys_Hide();
  mbk->gui->Show();
  return 0;
};
int CMyBook::TerminateElf(CBook** bm_book,CMyBook* mbk)
{
  DEBUG;
  mbk->Free();
  return 0;
};
int CMyBook::ShowAuthorInfo(CBook** bm_book,CMyBook* mbk)
{
  DEBUG;
  MESSAGE(L"Snake\n\n(c)mmcorp");
  return 0;
};
int CMyBook::OnMyKey(int key,int r,int mode,CMyBook* mybook, DISP_OBJ*)
{
  if(key==KEY_ESC || key==KEY_DEL) if(mybook==Display_GetTopBook(0)) mybook->Free();
  return 0;
};
CMyBook::~CMyBook()
{
  DEBUG;
  ModifyKeyHook(MKKEYHOOKPROC(OnMyKey),KEY_HOOK_REMOVE,0);
  gui->Free();
  elf_exit();
};
CMyBook::CMyBook()
:CBook("Snake",&base_page)
{
  DEBUG;
  ModifyKeyHook(MKKEYHOOKPROC(OnMyKey),KEY_HOOK_ADD,this);
  GotoPage(&main_page);
};

DECLARE_PAGE_DESC_BEGIN(CMyBook::main_page, "Snake_MainPage")
DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::OnMainPage )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "Snake_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END
