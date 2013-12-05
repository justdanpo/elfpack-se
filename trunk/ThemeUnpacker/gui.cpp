#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "structs.h"
int tar_uncompress(wchar_t *path, wchar_t *name);
extern const PAGE_DESC bk_gui;
extern const PAGE_DESC bk_main;

void OnBackGui( BOOK* bk, GUI* )
{
  FreeBook(bk);
};
int isA2Book(BOOK *bk)
{
  if (!strcmp(bk->xbook->name,"Softkeys")) { return 1; }
  else { return 0; }
};

void OnEnterGui( BOOK* bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)bk;
  int x=ListMenu_GetSelectedItem(mbk->lst);
  if (x==0)
  {
    BOOK *bk=SESSION_GetTopBook(root_list_get_session(root_list_get_session_count() - 1));
    SetTheme(mbk->path, mbk->name, BookObj_GetBookID(bk), 1);
    UI_Event(THEME_CHANGED_EVENT);
    FreeBook(bk);
  }
  if (x==1)
  {
    int res=tar_uncompress(mbk->path,mbk->name);
    wchar_t ustr[64];
    if (res>0)
    {
      snwprintf(ustr,MAXELEMS(ustr)-1,L"Unpacked\nTotal files: %d", res);
    }
    else
    {
      snwprintf(ustr,MAXELEMS(ustr)-1,L"Unpacking failed\nTotal files: %d", res);
    }
    MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,MAXELEMS(ustr)-1), NOIMAGE, 1, 5000,0);
    FreeBook(bk);
  }
};
int onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    if (item==0)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Activate",ENC_UCS2,TEXTID_ANY_LEN));
    }
    else
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Unpack",ENC_UCS2,TEXTID_ANY_LEN));
    }
  }
  return(1);
};

GUI_LIST * CreateGuiList(BOOK * book)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    GUIObject_SetTitleText(lo,TextID_Create(L"Install theme",ENC_UCS2,TEXTID_ANY_LEN));
    ListMenu_SetItemCount(lo,2);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,onLBMessage);
    ListMenu_SetItemStyle(lo,0);
    GUIObject_SetStyle(lo,9);
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, OnBackGui);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1, OnEnterGui);
  }
  return(lo);
};

GUI_LIST *create_ed(BOOK *book)
{
  MyBOOK *mbk=(MyBOOK *)book;
  mbk->lst=CreateGuiList(book);
  GUIObject_Show(mbk->lst);
  return(mbk->lst);
};

int CreateGui(void *data, BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  mbk->lst=create_ed(book);
  return 1;
};

int RemoveGUI(void *data, BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  GUIObject_Destroy(mbk->lst);
  return 0;
};

const PAGE_MSG bk_msglst_gui[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,CreateGui,
  PAGE_EXIT_EVENT_TAG,RemoveGUI,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_gui = {"ThmUnpacker_GUI",0,bk_msglst_gui};
