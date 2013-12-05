#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "main.h"
#include "menu.h"
#include "language.h"

#define MENU_ITEM_NUM   5

void AutoOKPressed(BOOK * bk, wchar_t *string, int len)
{
  wstrcpy(SIwstr, string);
  AutoLocation=true;
  CheckCurrentCell();
  BookObj_ReturnPage(bk,NIL_EVENT);
}

void AutoBackPressed(BOOK * bk, u16 *string, int len)
{
  BookObj_ReturnPage(bk,NIL_EVENT);
}

void CreateAutoLacationInput()
{
  MyBOOK * bk = (MyBOOK *) FindBook(isMiniGPSBook);
  FREE_GUI(bk->text_input);
  TEXTID text = TextID_Create(SIwstr,ENC_UCS2,TEXTID_ANY_LEN);
  bk->text_input = CreateStringInputVA(0,
                            VAR_BOOK(bk),
                            VAR_STRINP_FIXED_TEXT(TextID_Create(LG_CURRENTLOCATION,ENC_UCS2,TEXTID_ANY_LEN)),
                            VAR_STRINP_TEXT(text),
                            VAR_STRINP_NEW_LINE(0),
                            VAR_STRINP_ENABLE_EMPTY_STR(0),
                            VAR_STRINP_MAX_LEN(MAX_AUTOLOCATION_LEN - 1),
                            VAR_STRINP_MODE(IT_STRING),
                            VAR_OK_PROC(AutoOKPressed),
                            VAR_PREV_ACTION_PROC(AutoBackPressed),
                            0);
  BookObj_SetFocus( &bk->book,0);
  GUIObject_Show(bk->text_input);
}

int menu_callback(GUI_MESSAGE * msg)
{
  int str_id;
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int curitem = GUIonMessage_GetCreatedItemIndex(msg);
    {
      switch(curitem)
      {
      case 0:
        if(visible)
          str_id  = TextID_Create(LG_HIDE,ENC_UCS2,TEXTID_ANY_LEN);
        else
          str_id  = TextID_Create(LG_SHOW,ENC_UCS2,TEXTID_ANY_LEN);
        break;
      case 1:
        if(showCellID)
          str_id  = TextID_Create(LG_NAME,ENC_UCS2,TEXTID_ANY_LEN);
        else
          str_id  = TextID_Create(LG_CELLID,ENC_UCS2,TEXTID_ANY_LEN);
        break;
      case 2:
        if(AutoLocation)
          str_id  = TextID_Create(LG_AUTOLOCATION_OFF,ENC_UCS2,TEXTID_ANY_LEN);
        else
          str_id  = TextID_Create(LG_AUTOLOCATION_ON,ENC_UCS2,TEXTID_ANY_LEN);
        break;
      case 3:
        str_id  = TextID_Create(LG_SETTINGS,ENC_UCS2,TEXTID_ANY_LEN);
        break;
      case 4:
        str_id  = TextID_Create(LG_ABOUT,ENC_UCS2,TEXTID_ANY_LEN);
        break;
      }
    }
    GUIonMessage_SetMenuItemText(msg,str_id);
  }
  return(1);
}

void MenuOnBack( BOOK* book, GUI* )
{
  BookObj_ReturnPage(book,NIL_EVENT);
}

void MenuOnEnter( BOOK* book, GUI* )
{
  MyBOOK *bk = (MyBOOK *)book;
  int item_num = ListMenu_GetSelectedItem(bk->menu);
  switch(item_num){
  case 0:
    ShowHideProc();
    InvalidateAll();
    BookObj_ReturnPage(book,NIL_EVENT);
    break;
  case 1:
    CellIdProc();
    InvalidateAll();
    BookObj_ReturnPage(book,NIL_EVENT);
    break;
  case 2:
    if (AutoLocation == false)
    {
      CreateAutoLacationInput();
    }
    else
    {
      AutoLocation = false;
      MessageBox(EMPTY_TEXTID, TextID_Create(LG_AUTOLOCATIONOFF,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
      InvalidateAll();
      BookObj_ReturnPage(book,NIL_EVENT);
    }
    break;
  case 3:
    onBcfgConfig(0, book);
    BookObj_ReturnPage(book,NIL_EVENT);
    break;
  case 4:
    ShowAuthorInfo(0 , book);
    BookObj_ReturnPage(book,NIL_EVENT);
    break;
  }
}

void MenuOnKey(DISP_OBJ *db, int key, int unk, int repeat, int type)
{
  MyBOOK * bk = (MyBOOK *) FindBook(isMiniGPSBook);
  bk->oldOnKey(db, key, unk, repeat, type);
  if(type == KBD_SHORT_RELEASE)
  {
    int num = key - KEY_DIGITAL_0;
    if(num == 2)
    {
      int item = ListMenu_GetSelectedItem(bk->menu) - 1;
      if(item < 0) item = MENU_ITEM_NUM - 1;
      ListMenu_SetCursorToItem(bk->menu, item);
    }
    else if((num == 8) || (num == 0))
    {
      int item = ListMenu_GetSelectedItem(bk->menu) + 1;
      if(item >= MENU_ITEM_NUM) item = 0;
      ListMenu_SetCursorToItem(bk->menu, item);
    }
    else if(num == 5)
    {
      MenuOnEnter((BOOK*)bk, 0);
    }
  }
}

void CreateMenu(MyBOOK *mbk)
{
  mbk->menu = CreateListMenu(&mbk->book,0);

  GUIObject_SetTitleText(mbk->menu, TextID_Create(LELFNAME,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(mbk->menu, MENU_ITEM_NUM);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)mbk->menu, menu_callback);
  ListMenu_SetCursorToItem(mbk->menu,0);

//  ListMenu_SetItemStyle(mbk->menu, 4);

  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_BACK, MenuOnBack);
  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_LONG_BACK, MenuOnBack);
  GUIObject_SoftKeys_SetAction(mbk->menu,ACTION_SELECT1, MenuOnEnter);

  mbk->oldOnKey = DispObject_GetOnKey( GUIObject_GetDispObject(mbk->menu) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(mbk->menu) ), MenuOnKey );

  BookObj_SetFocus( &mbk->book,0);
  GUIObject_Show(mbk->menu);
}

static int MenuPageOnEnter(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  CreateMenu(mbk);
  return (1);
}

static int MenuPageOnExit(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->menu);
  FREE_GUI(mbk->text_input);
  BookObj_Hide(bk,0);
//  BookObj_SetFocus(Find_StandbyBook(),0);
  return (1);
}

const PAGE_MSG bk_msglst_menu[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    MenuPageOnEnter,
  PAGE_EXIT_EVENT_TAG,     MenuPageOnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_menu = {"MiniGPS_Menu_Page",0,bk_msglst_menu};

void ShowMenu(BOOK * book)
{
  BookObj_CallPage(book, &bk_menu);
}
