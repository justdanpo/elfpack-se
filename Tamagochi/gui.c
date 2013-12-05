#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "Tamagochi.h"
#include "language.h"
#include "misc_util.h"

#define MENU_ITEMS_COUNT 10

int ActiveTAB = 0;
int ActivePOS = 0;

void EditConfig(int indx, BOOK * book, int snd)
{
  FSTAT _fstat;
  wchar_t path[256];
  wchar_t name[64];

  extractdir(path, name, BCFGEDIT_PATH);

  if(fstat(path,name,&_fstat)==0)
  {
  }
  else if(fstat(GetDir(DIR_ELFS|MEM_INTERNAL),name,&_fstat)==0)
  {
    wstrcpy(path,GetDir(DIR_ELFS|MEM_INTERNAL));
  }
  else if(fstat(GetDir(DIR_ELFS|MEM_EXTERNAL),name,&_fstat)==0)
  {
    wstrcpy(path,GetDir(DIR_ELFS|MEM_EXTERNAL));
  }
  else
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LG_GFGBCFGEDITNOTFOUND,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  }
  wstrcat(path,L"/");
  wstrcat(path,name);
  elfload(path,(void*)successed_config_path,(void*)successed_config_name,0);
}

int  CountAbout=0;

int isAnyDeath()
{
  for(int i=0; i<Pets[0].Status.Count; i++)
    if(Pets[i].Status.StatusDeath == 1) return 1;

  return 0;
}

static void AboutDlg(int indx, BOOK * book, int snd)
{
#if (!defined(LANG_RU) && !defined(LANG_EN))
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO _T("\n") LTRANSLATED TRANSLATED_BY,0,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
#else
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
#endif
}

void RoomCommand(int indx, BOOK * book, int snd)
{
  wchar_t eat_path[256];
  getdir(eat_path, ROOM_PATH);
  const wchar_t * folder_list[2];
  folder_list[0]=eat_path;
  folder_list[1]= 0;
  int folders_cnt = 1;

  void * DB_Desc=DataBrowserDesc_Create();
  DataBrowserDesc_SetHeaderText(DB_Desc,TextID_Create(LG_ROOM,ENC_UCS2,TEXTID_ANY_LEN));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,folders_cnt);
  DataBrowserDesc_SetSelectAction(DB_Desc,0);
  DataBrowserDesc_Menu_AddFSFunctions(DB_Desc, 0);
  DataBrowserDesc_Menu_AddNewFolder(DB_Desc, 1);
  DataBrowserDesc_Menu_AddMarkFiles(DB_Desc, 1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);

}

enum MENU_CMD {
  PLAY_CMD,
  VACCINATE_CMD,
  DISCIPLINE_CMD,
  SLEEP_CMD,
  BATHE_CMD,
  ROOM_CMD,
  STATUS_CMD,
  NAME_CMD,
  SETTINGS_CMD,
  ABOUT_CMD
};

static int const menuiconsnum[MENU_ITEMS_COUNT]=
{
  PLAY_ICN,
  VACCINATE_ICN,
  DISCIPLINE_ICN,
  SLEEP_ICN,
  SM_BATHE_ICN,
  ROOM_ICN,
  STATUS_ICN,
  NAME_ICN,
  SETTINGS_ICN,
  ABOUT_ICN
};

static const wchar_t * const menutexts[MENU_ITEMS_COUNT]=
{
  LG_GAMEPLAY,
  LG_VACCINATE,
  LG_DISCIPLINE,
  LG_SLEEP,
  LG_BATHE,
  LG_ROOM,
  LG_STATUS,
  LG_NAME,
  LG_EDCFG,
  LG_ABOUT
};

static void (*menuprocs[MENU_ITEMS_COUNT])(int, BOOK *, int) =
{
  GamePlay,
  Vaccinate,
  Discipline,
  SleepCommand,
  BatheCommand,
  RoomCommand,
  InfoStatus,
  ChangeName,
  EditConfig,
  AboutDlg,
};

int menu_list_callback(GUI_MESSAGE * msg)
{
  int str_id;
  int icon_id;
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int curitem=GUIonMessage_GetCreatedItemIndex(msg);
//    MyBOOK * bk = (MyBOOK *)msg->book;
    MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
    int indx = TabMenuBar_GetFocusedTab(bk->gui);
    icon_id = img_menu[menuiconsnum[curitem]];
    if (curitem!= SLEEP_CMD){
      str_id  = TextID_Create(menutexts[curitem],ENC_UCS2,TEXTID_ANY_LEN);
    }
    else
    {
      if (Pets[indx].Sleep==0){
        str_id  = TextID_Create(LG_SLEEP,ENC_UCS2,TEXTID_ANY_LEN);
      }
      else
      {
        str_id  = TextID_Create(LG_NOTSLEEP,ENC_UCS2,TEXTID_ANY_LEN);
      }
    }
    int strID_array[3];
    strID_array[0]=icon_id+0x78000000;
    strID_array[1]=0x78000020;
    strID_array[2]=str_id;
    GUIonMessage_SetMenuItemText(msg,TextID_Create(strID_array,ENC_TEXTID,3));
  }
  return(1);
}

void DestroyMenuList( BOOK* book, GUI* )
{
  BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void CancelMenuList( BOOK* book, GUI* )
{
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
}

void Menu_onEnter( BOOK* book, GUI* )
{
  MyBOOK * bk = (MyBOOK *)book;
  int indx = TabMenuBar_GetFocusedTab(bk->gui);
  int item_num = ActivePOS = ListMenu_GetSelectedItem(bk->menu_list[indx]);
  menuprocs[item_num](indx, book,(!GetSilent() || sndOnSilent) && sndEnable);
  //BookObj_ReturnPage(book,ACCEPT_EVENT);
}

void Menu_OnKey(DISP_OBJ* p, int i1, int i2, int i3, int i4)
{
  MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
  int indx = TabMenuBar_GetFocusedTab(bk->gui);
  bk->Menu_oldOnKey[indx](p,i1,i2,i3,i4);
  if(i4==KBD_SHORT_RELEASE)
  {
    int num = i1 - KEY_DIGITAL_0;
    if(num == 2)
    {
      int item = ListMenu_GetSelectedItem(bk->menu_list[indx]) - 1;
      if(item < 0) item = MENU_ITEMS_COUNT - 1;
      ListMenu_SetCursorToItem(bk->menu_list[indx],item);
    }
    else if((num == 8) || (num == 0))
    {
      int item = ListMenu_GetSelectedItem(bk->menu_list[indx]) + 1;
      if(item >= MENU_ITEMS_COUNT) item = 0;
      ListMenu_SetCursorToItem(bk->menu_list[indx],item);
    }
    else if(num == 5)
    {
      Menu_onEnter((BOOK*)bk,0);
    }
  }
};

void onTabSwitch(BOOK * bk,int active_tab)
{
  ActiveTAB=active_tab;
}

int CreateMenuList(void *data, BOOK * book)
{
  int menu_pos = ActivePOS;
  int tab_pos = ActiveTAB;
  if(tab_pos >= Pets[0].Status.Count) tab_pos = 0;
  if(menu_pos >= MENU_ITEMS_COUNT) menu_pos = 0;
  MyBOOK * bk = (MyBOOK *)book;
  if(bk->gui) GUIObject_Destroy( bk->gui);
  FREE_GUI(bk->stat_list);

  bk->gui = CreateTabMenuBar(book);
  TabMenuBar_SetTabCount(bk->gui,Pets[0].Status.Count);
  for(int indx=0; indx < Pets[0].Status.Count; indx++)
  {
    if (bk->menu_list[indx]) GUIObject_Destroy( bk->menu_list[indx]);
    bk->menu_list[indx]=CreateListMenu(book,0);
    GUIObject_SetTitleText(bk->menu_list[indx],TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN));

    ListMenu_SetItemCount(bk->menu_list[indx],MENU_ITEMS_COUNT);
    OneOfMany_SetOnMessage((GUI_ONEOFMANY*)bk->menu_list[indx],menu_list_callback);
    ListMenu_SetCursorToItem(bk->menu_list[indx],menu_pos);

    GUIObject_SoftKeys_SetAction(bk->menu_list[indx],ACTION_BACK,DestroyMenuList);
    GUIObject_SoftKeys_SetAction(bk->menu_list[indx],ACTION_LONG_BACK,CancelMenuList);
    GUIObject_SoftKeys_SetAction(bk->menu_list[indx],ACTION_SELECT1,Menu_onEnter);

    bk->Menu_oldOnKey[indx] = DispObject_GetOnKey( GUIObject_GetDispObject(bk->menu_list[indx]) );
    DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(bk->menu_list[indx]) ), Menu_OnKey );

    TabMenuBar_SetTabGui(bk->gui,indx,bk->menu_list[indx]);

    TabMenuBar_SetTabIcon(bk->gui,indx,img_menu[SM_BOREDOM_ICN],0);
    TabMenuBar_SetTabIcon(bk->gui,indx,img_menu[SM_SMILE_ICN],1);
  }
  TabMenuBar_SetFocusedTab(bk->gui,tab_pos);
  TabMenuBar_SetOnTabSwitch(bk->gui,onTabSwitch);
  BookObj_SetFocus(book,0);
  GUIObject_Show(bk->gui);
  return(0);
}

int ExitMenuList(void *data, BOOK * book)
{
  MyBOOK * bk = (MyBOOK *)book;

  if (bk->stat_list)
  {
    GUIObject_Destroy( bk->stat_list);
    bk->stat_list=0;
  }

  for(int i=0; i < MAX_PET_COUNT_GLB; i++)
  {
    if (bk->menu_list[i])
    {
      GUIObject_Destroy( bk->menu_list[i]);
      bk->menu_list[i]=0;
    }
  }

  if (bk->gui)
  {
    ActiveTAB = TabMenuBar_GetFocusedTab(bk->gui);
    GUIObject_Destroy( bk->gui);
    bk->gui=0;
  }

  BookObj_Hide(book,0);

  return(0);
}

const PAGE_MSG CreateMenu_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateMenuList,
  ACCEPT_EVENT_TAG,CreateMenuList,
  PAGE_EXIT_EVENT_TAG,ExitMenuList,
  NIL_EVENT_TAG,0
};

PAGE_DESC CreateMenu_page ={"Tamagochi_CreateMenu_Main_Page",0,CreateMenu_PageEvents};

void ShowMenu(BOOK * book)
{
  BookObj_CallPage(book,&CreateMenu_page);
}

void CloseMenu(BOOK * book)
{
  ExitMenuList(0,book);
}
