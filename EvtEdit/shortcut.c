#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\externals.h"

int isEvtEditBook(BOOK * struc);

void onShortcutSet(BOOK * MainMenu, GUI* )
{
  wchar_t*name_buf=MenuBook_Desktop_GetSelectedItemID(MainMenu);
  if (name_buf)
  {
    BOOK *EvtEdit=FindBook(isEvtEditBook);
    MyBOOK *mbk=(MyBOOK*)EvtEdit;
    DELETE(mbk->selev->param);
    mbk->selev->param=new wchar_t[wstrlen(name_buf)+1];
    wstrcpy(mbk->selev->param,name_buf);
    mfree(name_buf);
  }
  FreeBook(MainMenu);
  BookObj_ReturnPage(FindBook(isEvtEditBook),ACCEPT_EVENT);
  BookObj_CallPage(FindBook(isEvtEditBook),&bk_main);
};

int onPrevious_MainMenu_DB(void *data, BOOK * book)
{
  BookObj_ReturnPage(book,NIL_EVENT);
  return(1);
};


int onCancel_MainMenu_DB(void *data, BOOK * book)
{
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
  return(1);
};

int CreateMainMenu(void *data, BOOK * book)
{
  BOOK * MainMenu;
  if (MainMenu=MenuBook_Desktop(1,BookObj_GetBookID(book)))
  {
    
    int MainMenuID=BookObj_GetBookID(MainMenu);
    BookObj_SoftKeys_SetAction(MainMenu,0x0,onShortcutSet);
    int str_id;
    textidname2id(L"SHC_SET_SHORTCUT_SK",TEXTID_ANY_LEN,&str_id);
    BookObj_SoftKeys_SetText(MainMenu,0x0,str_id);
    return(1);
  }
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  return(0);
};

const PAGE_MSG SelectShortcut_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateMainMenu,
  PREVIOUS_EVENT_TAG,onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG,onCancel_MainMenu_DB,
  NIL_EVENT_TAG,0
};

const PAGE_DESC SelectShortcut_page ={"EvtEdit_SelectShortcut_Page",0,SelectShortcut_PageEvents};
