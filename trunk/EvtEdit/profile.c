#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\lng.h"
#include "header\externals.h"
wchar_t *profiles[]={L"IA_AUTOMATIC_TXT",L"NORMAL_TXT",L"MEETING_TXT",L"IN_CAR_TXT",L"OUTDOORS_TXT",L"HANDSFREE_PROFILE_TXT",L"HOME_TXT",L"OFFICE_TXT"};
wchar_t *picns[]={L"JOYSTICK_DESKTOP_MENU_ICN",L"PROFILE_NORMAL_LIST_ICN", L"PROFILE_MEETING_LIST_ICN", L"PROFILE_INCAR_LIST_ICN", L"PROFILE_OUTDOORS_LIST_ICN", L"PROFILE_PHF_LIST_ICN", L"PROFILE_HOME_LIST_ICN", L"PROFILE_OFFICE_LIST_ICN"};

int isEvtEditBook(BOOK * struc);

void Profiles_OnSelect(BOOK *bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK *)bk;
  int item=ListMenu_GetSelectedItem(mbk->profiles);
  if (mbk->selev->param)delete(mbk->selev->param);
  mbk->selev->param=new wchar_t[2];
  snwprintf(mbk->selev->param,1,L"%d",item);
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
  BookObj_CallPage(bk,&bk_main);
};

int Profiles_onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    MyBOOK *mbk=(MyBOOK*)FindBook(isEvtEditBook);
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    int id=EMPTY_TEXTID;
    textidname2id(profiles[item],0xFFFFFFFF,&id);
    GUIonMessage_SetMenuItemText(msg,id);
    int ic=NOIMAGE;
    iconidname2id(picns[item],0xFFFFFFFF,&ic);
    GUIonMessage_SetMenuItemIcon (msg,0, ic);
    if (item==0 && mbk->prof_type==1)
    {
      GUIonMessage_SetItemDisabled(msg, 1);
    }
  }
  return(1);
};

void Profiles_OnBack(BOOK *bk, GUI* )
{
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

GUI_LIST * Profiles_CreateGuiList(BOOK * book)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    GUIObject_SetTitleText(lo,TextID_Create(lng[CHOOSE_PROF],ENC_UCS2,TEXTID_ANY_LEN));
    ListMenu_SetItemCount(lo,8);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,Profiles_onLBMessage);
    ListMenu_SetItemStyle(lo,0);
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, Profiles_OnBack);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,Profiles_OnSelect);
  }
  return(lo);
};

GUI_LIST *create_profiles(BOOK *book)
{
  MyBOOK *mbk=(MyBOOK *)book;
  mbk->profiles=Profiles_CreateGuiList(book);
  GUIObject_Show(mbk->profiles);
  return(mbk->profiles);
};


static int Profiles_OnEnter(void *, BOOK * bk)
{
  create_profiles(bk);
  return 1;
};

static int Profiles_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->profiles);
  return (1);  
};

const PAGE_MSG bk_msglst_profiles[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    Profiles_OnEnter,
  PAGE_EXIT_EVENT_TAG,     Profiles_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_profiles = {"EvtEdit_Profiles_Page",0,bk_msglst_profiles};
