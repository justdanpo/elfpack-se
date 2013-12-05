#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "Tamagochi.h"
#include "language.h"

int ExitStatusList(void *data, BOOK * book);
void ViewSex(int indx);
void ViewAge(int indx);
void ViewFamilyLife(int indx);
void ViewHealth(int indx);
void ViewHunger(int indx);
void ViewHappiness(int indx);
void ViewDirtiness(int indx);
void ViewBoredom(int indx);
void ViewBehaviour(int indx);
void ViewFatigue(int indx);

static const wchar_t * const smenu_sextexts[3]=
{
  LG_FEMALE,
  LG_MALE,
  LG_NOTDEF
};

static int const smenu_sexiconsnum[3]=
{
  SM_FEMALE_ICN,
  SM_MALE_ICN,
  SM_SEX_ICN
};

int CountSex = 0;

#define STATUS_ITEMS_COUNT 10

enum item_nums{
  SM_SEX = 0,
  SM_AGE,
  SM_FAMILYLIFE,
  SM_HEALTH,
  SM_HUNGER,
  SM_HAPPINESS,
  SM_DIRTINESS,
  SM_BOREDOM,
  SM_BEHAVIOUR,
  SM_FATIGUE
};

static int const smenuiconsnum[STATUS_ITEMS_COUNT]=
{
  SM_SEX_ICN,
  SM_SMILE_ICN,
  SM_FAMILY_ICN,
  SM_HEALTH_ICN,
  SM_HUNGER_ICN,
  SM_HAPPINESS_ICN,
  SM_DIRTINESS_ICN,
  SM_BOREDOM_ICN,
  SM_BEHAVIOUR_ICN,
  SM_FATIGUE_ICN
};

static const wchar_t * const smenutexts[STATUS_ITEMS_COUNT]=
{
  LG_SEX,
  LG_AGE,
  LG_FAMILYLIFE,
  LG_HEALTH,
  LG_HUNGER,
  LG_HAPPINESS,
  LG_DIRTINESS,
  LG_BOREDOM,
  LG_BEHAVIOUR,
  LG_FATIGUE
};

static void (*smenuprocs[STATUS_ITEMS_COUNT])(int) =
{
  ViewSex,
  ViewAge,
  ViewFamilyLife,
  ViewHealth,
  ViewHunger,
  ViewHappiness,
  ViewDirtiness,
  ViewBoredom,
  ViewBehaviour,
  ViewFatigue
};

void OnCloseSetSex( BOOK* book, GUI* )
{
  CountSex = 0;
  MyBOOK * bk=(MyBOOK *)book;
  FREE_GUI(bk->cbox_gui);
}

void OnSelectSetSex( BOOK* book, GUI* )
{
  CountSex = 0;
  MyBOOK * bk=(MyBOOK *)book;
  int item=OneOfMany_GetSelected(bk->cbox_gui);
  int indx = bk->cur_indx;
  Pets[indx].Status.Sex = item;
  FREE_GUI(bk->cbox_gui);
//  CloseStatus(book);
//  ShowStatus(book);
}

void ViewSex(int indx)
{
  if((CountSex <= 10) && (Pets[indx].Status.Sex != 2))
  {
    wchar_t s[256];
    ++CountSex;
    snwprintf(s,255,L"%ls: %ls", LG_SEX, smenu_sextexts[Pets[indx].Status.Sex]);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[smenu_sexiconsnum[Pets[indx].Status.Sex]], 1, 5000*msgShowTime, 0);
  }
  else
  {
    CountSex = 0;
    MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
    bk->cur_indx = indx;
    GUI_ONEOFMANY *om=CreateOneOfMany(&bk->book);
    bk->cbox_gui=om;
    GUIObject_SetTitleText(om,TextID_Create(LG_SEX,ENC_UCS2,TEXTID_ANY_LEN));
    TEXTID strid[2];
    strid[0]=TextID_Create(LG_FEMALE,ENC_UCS2,TEXTID_ANY_LEN);
    strid[1]=TextID_Create(LG_MALE,ENC_UCS2,TEXTID_ANY_LEN);
    OneOfMany_SetTexts(om,strid,2);
    if(Pets[indx].Status.Sex != 2) OneOfMany_SetChecked(om,Pets[indx].Status.Sex);
    else OneOfMany_SetChecked(om,0);
    GUIObject_SoftKeys_SetAction(om,ACTION_BACK,OnCloseSetSex);
    GUIObject_SoftKeys_SetAction(om,ACTION_SELECT1,OnSelectSetSex);
    GUIObject_Show(om);
  }
}

void ViewAge(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d\n%d/%d",
            LG_AGE, Pets[indx].Status.Age, (int)Pets[indx].Status.TimeAge, Pets[indx].Status.Age*10+100 );
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_SMILE_ICN], 1, 5000*msgShowTime, 0);
}

void ViewFamilyLife(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d",
            LG_FAMILYLIFE, Pets[indx].Status.FamilyLife );
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_FAMILY_ICN], 1, 5000*msgShowTime, 0);
}

void ViewHealth(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%\n%d/%d",
            LG_HEALTH, (int)Pets[indx].Status.Health*100/Pets[indx].Status.MaxHealth, Pets[indx].Status.Health, Pets[indx].Status.MaxHealth );
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_HEALTH_ICN], 1, 5000*msgShowTime, 0);
}
void ViewHunger(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_HUNGER, (int)Pets[indx].Status.Hunger*100/Pets[indx].Status.MaxHunger);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_HUNGER_ICN], 1, 5000*msgShowTime, 0);
}
void ViewHappiness(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_HAPPINESS,(int)Pets[indx].Status.Happiness*100/Pets[indx].Status.MaxHappiness);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_HAPPINESS_ICN], 1, 5000*msgShowTime, 0);
}
void ViewDirtiness(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_DIRTINESS, 100-(int)Pets[indx].Status.Dirtiness*100/Pets[indx].Status.MaxDirtiness);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_DIRTINESS_ICN], 1, 5000*msgShowTime, 0);
}
void ViewBoredom(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_BOREDOM,(int)Pets[indx].Status.Boredom*100/Pets[indx].Status.MaxBoredom);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_BOREDOM_ICN], 1, 5000*msgShowTime, 0);
}
void ViewBehaviour(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_BEHAVIOUR,(int)Pets[indx].Status.Behaviour*100/Pets[indx].Status.MaxBehaviour);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_BEHAVIOUR_ICN], 1, 5000*msgShowTime, 0);
}

void ViewFatigue(int indx)
{
  wchar_t s[256];
  snwprintf(s,255,L"%ls:%d%%",
            LG_FATIGUE, (int)Pets[indx].Fatigue);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(s,ENC_UCS2,255), img_menu[SM_FATIGUE_ICN], 1, 5000*msgShowTime, 0);
}

int status_list_callback(GUI_MESSAGE * msg)
{
  TEXTID str_id;
  IMAGEID icon_id;
  wchar_t ws[128];
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int curitem=GUIonMessage_GetCreatedItemIndex(msg);
//    MyBOOK * bk = (MyBOOK *)msg->book;
    MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
    int indx = TabMenuBar_GetFocusedTab(bk->gui);
    icon_id = img_menu[smenuiconsnum[curitem]];
    {
      switch(curitem)
      {
      case SM_SEX:
        snwprintf(ws,127,L"%ls: %ls",smenutexts[curitem],smenu_sextexts[Pets[indx].Status.Sex]);
        break;
      case SM_AGE:
        snwprintf(ws,127,L"%ls: %d",smenutexts[curitem],Pets[indx].Status.Age);
        break;
      case SM_FAMILYLIFE:
        snwprintf(ws,127,L"%ls: %d",smenutexts[curitem],Pets[indx].Status.FamilyLife);
        break;
      case SM_HEALTH:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Status.Health*100/Pets[indx].Status.MaxHealth);
        break;
      case SM_HUNGER:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Status.Hunger*100/Pets[indx].Status.MaxHunger);
        break;
      case SM_HAPPINESS:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Status.Happiness*100/Pets[indx].Status.MaxHappiness);
        break;
      case SM_DIRTINESS:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],100-(int)Pets[indx].Status.Dirtiness*100/Pets[indx].Status.MaxDirtiness);
        break;
      case SM_BOREDOM:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Status.Boredom*100/Pets[indx].Status.MaxBoredom);
        break;
      case SM_BEHAVIOUR:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Status.Behaviour*100/Pets[indx].Status.MaxBehaviour);
        break;
      case SM_FATIGUE:
        snwprintf(ws,127,L"%ls:%d%%",smenutexts[curitem],(int)Pets[indx].Fatigue);
        break;
      }
    }
    str_id  = TextID_Create(ws,ENC_UCS2,TEXTID_ANY_LEN);
    int strID_array[3];
    strID_array[0]=icon_id+0x78000000;
    strID_array[1]=0x78000020;
    strID_array[2]=str_id;
    GUIonMessage_SetMenuItemText(msg,TextID_Create(strID_array,ENC_TEXTID,3));
  }
  return(1);
}

void DestroyStatusList( BOOK* book, GUI* )
{
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void CancelStatusList( BOOK* book, GUI* )
{
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
}

void Status_onEnter( BOOK* book, GUI* )
{
  MyBOOK *bk = (MyBOOK *)book;
  int item_num = ListMenu_GetSelectedItem(bk->stat_list);
  int indx = TabMenuBar_GetFocusedTab(bk->gui);
  smenuprocs[item_num](indx);
  //BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void Status_OnKey(DISP_OBJ* p, int i1, int i2, int i3, int i4)
{
  MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
  bk->Status_oldOnKey(p,i1,i2,i3,i4);
  if(i4==KBD_SHORT_RELEASE)
  {
    int num = i1 - KEY_DIGITAL_0;
    if(num == 2)
    {
      int item = ListMenu_GetSelectedItem(bk->stat_list) - 1;
      if(item < 0) item = STATUS_ITEMS_COUNT - 1;
      ListMenu_SetCursorToItem(bk->stat_list,item);
    }
    else if((num == 8) || (num == 0))
    {
      int item = ListMenu_GetSelectedItem(bk->stat_list) + 1;
      if(item >= STATUS_ITEMS_COUNT) item = 0;
      ListMenu_SetCursorToItem(bk->stat_list,item);
    }
    else if(num == 5)
    {
      Status_onEnter((BOOK*)bk,0);
    }
  }
};

int CreateStatusList(void *data, BOOK * book)
{
  int stat_pos=0;
  MyBOOK * bk = (MyBOOK *)book;
  int indx = TabMenuBar_GetFocusedTab(bk->gui);
  FREE_GUI(bk->stat_list);

  bk->stat_list=CreateListMenu(book,0);

  GUIObject_SetTitleText(bk->stat_list,TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN));

  ListMenu_SetItemCount(bk->stat_list,STATUS_ITEMS_COUNT);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)bk->stat_list,status_list_callback);

  ListMenu_SetCursorToItem(bk->stat_list,stat_pos);

  GUIObject_SoftKeys_SetAction(bk->stat_list,ACTION_BACK,DestroyStatusList);
  GUIObject_SoftKeys_SetAction(bk->stat_list,ACTION_LONG_BACK,CancelStatusList);
  GUIObject_SoftKeys_SetAction(bk->stat_list,ACTION_SELECT1,Status_onEnter);

  bk->Status_oldOnKey = DispObject_GetOnKey( GUIObject_GetDispObject(bk->stat_list) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(bk->stat_list) ), Status_OnKey );

  CountSex = 0;

  //  BookObj_SetFocus(book,0);
  GUIObject_Show(bk->stat_list);
  return(0);
}

int ExitStatusList(void *data, BOOK * book)
{
  MyBOOK * bk = (MyBOOK *)book;
  FREE_GUI(bk->stat_list);
  return(0);
}

const PAGE_MSG CreateStatus_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateStatusList,
  ACCEPT_EVENT_TAG,CreateStatusList,
  PAGE_EXIT_EVENT_TAG,ExitStatusList,
  NIL_EVENT_TAG,0
};

PAGE_DESC CreateStatus_page ={"Tamagochi_CreateStatus_Main_Page",0,CreateStatus_PageEvents};

void ShowStatus(BOOK * book)
{
  BookObj_CallPage(book,&CreateStatus_page);
}

void CloseStatus(BOOK * book)
{
  ExitStatusList(0,book);
}

