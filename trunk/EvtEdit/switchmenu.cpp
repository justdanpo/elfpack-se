#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\externals.h"
#include "header\lng.h"

extern const PAGE_DESC bk_Switch;
extern int indic_last;

int isEvtEditBook(BOOK * struc);

void kill_switchlst(MyBOOK *mbk)
{
  if (mbk->switch_lst)
  {
    while (mbk->switch_lst->FirstFree)
    {
      wchar_t* str=(wchar_t*)List_RemoveAt(mbk->switch_lst,0);
      DELETE(str);
    }
    List_Destroy(mbk->switch_lst);
    mbk->switch_lst=0;
  }
};

int Switch_onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    MyBOOK *mbk=(MyBOOK*)FindBook(isEvtEditBook);
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    wchar_t* str=(wchar_t *)List_Get(mbk->switch_lst,item);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN));
    break;
  }
  return(1);
};

void Switch_OnSelect(BOOK *bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)bk;
  int item=ListMenu_GetSelectedItem(mbk->switch_gui);
  DELETE(mbk->selev->param);
  mbk->selev->param=new wchar_t[2];
  snwprintf(mbk->selev->param,1,L"%d", item);
  BookObj_CallPage(bk,&bk_main);
};
void Switch_OnBack(BOOK * bk, GUI* )
{
  indic_last=0;
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

GUI_LIST * CreateSwitchList(BOOK * book, int cnt)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    ListMenu_SetItemCount(lo, cnt);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,Switch_onLBMessage);
    ListMenu_SetItemStyle(lo,3);
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, Switch_OnBack);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,Switch_OnSelect);
  }
  return(lo);
};

static int SwitchPage_OnEnter(void *,BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK*)bk;
  mbk->switch_gui=CreateSwitchList(bk, mbk->switch_lst->FirstFree);
  GUIObject_SetTitleText(mbk->switch_gui, TextID_Create(lng[CHOOSE],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_Show(mbk->switch_gui);
  return 1;
};

static int SwitchPage_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  kill_switchlst(mbk);
  FREE_GUI(mbk->switch_gui);
  return 1;  
};

const PAGE_MSG bk_msglst_Switch[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    SwitchPage_OnEnter,
  PAGE_EXIT_EVENT_TAG,     SwitchPage_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_Switch = {"EvtEdit_SwitchMenu_Page",0,bk_msglst_Switch};
