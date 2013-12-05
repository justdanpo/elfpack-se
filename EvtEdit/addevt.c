#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\externals.h"
#include "header\lng.h"

int isEvtEditBook(BOOK *);
#define ACTION_COUNT 50

enum ACTION_TYPES {
  AT_STRING=0,
  AT_MUSICPATH,
  AT_ELFPATH,
  AT_FILEPATH,
  AT_PROFILE,
  AT_INT,
  AT_EXECUTE,
  AT_SHORTCUT,
  AT_JAVA,
  AT_NULL,
  AT_MUSICPATH_VOLUME,
  AT_PICTUREPATH,
  AT_PROFILE_2,
  AT_VOLUME,
  AT_CHOOSE_MENU_0,
  AT_CHOOSE_MENU_1,
  AT_CHOOSE_MENU_2,
  AT_CHOOSE_MENU_3,
  AT_CHOOSE_MENU_4,
  AT_VCALL,
  AT_THEME,
};

wchar_t *items[] = {L"Сообщение", L"Запуск эльфа", L"Ярлык", L"Java",L"Выключение телефона", L"Яркость", L"Фонарик: уст. яркость", L"Профиль", L"Рестарт", L"Музыка: воспр.", 
L"Музыка: воспр. с громкостью", L"Обои: установить", L"Вибра", L"ГЭ: уст. заметку", L"ГЭ: уст. оператора", L"Закрыть книгу", L"Вкл. музыку/радио", L"Поменять расписание", L"Обновить", 
L"Звук: выкл", L"Звук: вкл", L"ИК: вкл", L"ИК: выкл", L"ИК: мигнуть", L"Фонарик: мигнуть", L"(W580): отключить диоды", L"(W580): включить диоды", L"Оранж.диод: мигнуть", L"Оранж.диод: вкл. ", L"Оранж.диод: выкл", L"Важное событие", 
L"Звонок: уст.громкость", L"Звонок: вибрация", L"Звонок: уст.файл", L"Звонок: способ приёма", L"Звонок: вкл.нарастание", L"Звонок: уст. приём вызовов", L"Голосовой вызов",L"Сменить тему",0,0,0};

int ittypes[] = {AT_STRING,AT_ELFPATH,AT_SHORTCUT,AT_JAVA,AT_NULL,AT_INT,AT_INT,AT_PROFILE_2,AT_NULL,AT_MUSICPATH,AT_MUSICPATH_VOLUME,AT_PICTUREPATH,AT_INT,AT_STRING,AT_STRING,AT_STRING,AT_NULL,AT_EXECUTE,AT_NULL,AT_PROFILE,AT_PROFILE,AT_NULL,AT_NULL,AT_NULL, AT_NULL,
AT_NULL, AT_NULL, AT_NULL, AT_NULL, AT_NULL, AT_STRING, AT_VOLUME, AT_CHOOSE_MENU_0, AT_MUSICPATH, AT_CHOOSE_MENU_1, AT_CHOOSE_MENU_2, AT_CHOOSE_MENU_3, AT_VCALL, AT_THEME};

#define MUSIC 1
#define ELF 2
#define IMAGE 3
#define EVT 4
#define THM 5

int temp_act=0;
wchar_t *old=0;
void SelectEvent_OnEnter(BOOK *bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK *)bk;
  int item=ListMenu_GetSelectedItem(mbk->addgui);
  
  if (item!=mbk->selev->type)
  {
    mbk->selev->type=item;
    mbk->selev->param=new wchar_t[2];
    wstrcpy(mbk->selev->param,L"0");
  }
  
  if (ittypes[item]==AT_STRING)
  {
    mbk->inputType=IT_STRING;
    mbk->minint=-1;
    mbk->maxint=100;
    mbk->TextInputType=0;
    BookObj_CallPage(bk,&bk_StringInput);
  }
  else if (ittypes[item]==AT_EXECUTE)
  {
    mbk->selectfiletype=0;
    mbk->fType=EVT;
    mbk->dir1=DIR_OTHER+MEM_INTERNAL;
    mbk->dir2=DIR_OTHER+MEM_EXTERNAL;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else if (ittypes[item]==AT_INT)
  {
    mbk->TextInputType=0;
    mbk->inputType=IT_UNSIGNED_DIGIT;
    mbk->minint=0;
    mbk->maxint=100;
    BookObj_CallPage(bk,&bk_StringInput);
  }
  else if (ittypes[item]==AT_SHORTCUT)
  {
    BookObj_CallPage(bk,&SelectShortcut_page);
  }
  else if (ittypes[item]==AT_MUSICPATH)
  {
    mbk->selectfiletype=0;
    mbk->fType=MUSIC;
    mbk->dir1=DIR_AUDIO+MEM_INTERNAL;
    mbk->dir2=DIR_AUDIO+MEM_EXTERNAL;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else if (ittypes[item]==AT_THEME)
  {
    mbk->selectfiletype=0;
    mbk->fType=THM;
    mbk->dir1=DIR_THEME+MEM_INTERNAL;
    mbk->dir2=DIR_THEME+MEM_EXTERNAL;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else if (ittypes[item]==AT_ELFPATH)
  {
    mbk->selectfiletype=0;
    mbk->fType=ELF;
    mbk->dir1=DIR_ELFS+MEM_INTERNAL;
    mbk->dir2=DIR_ELFS+MEM_EXTERNAL;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else if (ittypes[item]==AT_PICTUREPATH)
  {
    mbk->selectfiletype=0;
    mbk->fType=IMAGE;
    mbk->dir1=DIR_IMAGE+MEM_INTERNAL;
    mbk->dir2=DIR_IMAGE+MEM_EXTERNAL;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else if (ittypes[item]==AT_JAVA)
  {
    BookObj_CallPage(bk,&SetJava_page);
  }
  else if (ittypes[item]==AT_PROFILE)
  {
    mbk->prof_type=0;
    BookObj_CallPage(bk,&bk_profiles);
  }
  else if (ittypes[item]==AT_PROFILE_2)
  {
    mbk->prof_type=1;
    BookObj_CallPage(bk,&bk_profiles);
  }
  else if (ittypes[item]==AT_MUSICPATH_VOLUME)
  {
    mbk->fType=MUSIC;
    mbk->dir1=DIR_AUDIO+MEM_INTERNAL;
    mbk->dir2=DIR_AUDIO+MEM_EXTERNAL;
    mbk->selectfiletype=1;
    mbk->curvol=6;
    mbk->maxvol=8;
    mbk->vol_mod=1;
    mbk->vol_type=0;
    BookObj_CallPage(bk,&bk_VolInput);
  }
  else if (ittypes[item]==AT_VOLUME)
  {
    mbk->curvol=4;
    mbk->maxvol=8;
    mbk->vol_mod=1;
    mbk->vol_type=1;
    BookObj_CallPage(bk,&bk_VolInput);
  }
  else if (ittypes[item]==AT_VCALL)
  {
    mbk->inputType=IT_PHONE_NUMBER;
    mbk->minint=-1;
    mbk->maxint=100;
    mbk->TextInputType=0;
    BookObj_CallPage(bk,&bk_StringInput);
  }
  else if (ittypes[item]==AT_CHOOSE_MENU_0)
  {
    extern const PAGE_DESC bk_Switch;
    if (!mbk->switch_lst)
      mbk->switch_lst=List_Create();
    wchar_t *n1=new wchar_t[30];
    wstrcpy(n1, lng[OFF]);
    wchar_t *n2=new wchar_t[30];
    wstrcpy(n2, lng[ON]);
    wchar_t *n3=new wchar_t[30];
    wstrcpy(n3, lng[ON_WOUT_SOUND]);
    List_InsertLast(mbk->switch_lst,n1);
    List_InsertLast(mbk->switch_lst,n2);
    List_InsertLast(mbk->switch_lst,n3);
    BookObj_CallPage(bk,&bk_Switch);
  }
  else if (ittypes[item]==AT_CHOOSE_MENU_1)
  {
    extern const PAGE_DESC bk_Switch;
    if (!mbk->switch_lst)
      mbk->switch_lst=List_Create();
    wchar_t *n1=new wchar_t[30];
    wstrcpy(n1, lng[TYPICAL]);
    wchar_t *n2=new wchar_t[30];
    wstrcpy(n2, lng[ANY_KEY]);
    wchar_t *n3=new wchar_t[30];
    wstrcpy(n3, lng[AUTO]);
    List_InsertLast(mbk->switch_lst,n1);
    List_InsertLast(mbk->switch_lst,n2);
    List_InsertLast(mbk->switch_lst,n3);
    BookObj_CallPage(bk,&bk_Switch);
  }
  else if (ittypes[item]==AT_CHOOSE_MENU_2)
  {
    extern const PAGE_DESC bk_Switch;
    if (!mbk->switch_lst)
      mbk->switch_lst=List_Create();
    wchar_t *n1=new wchar_t[30];
    wstrcpy(n1, lng[OFF]);
    wchar_t *n2=new wchar_t[30];
    wstrcpy(n2, lng[ON]);
    List_InsertLast(mbk->switch_lst,n1);
    List_InsertLast(mbk->switch_lst,n2);
    BookObj_CallPage(bk,&bk_Switch);
  }
  else if (ittypes[item]==AT_CHOOSE_MENU_3)
  {
    extern const PAGE_DESC bk_Switch;
    if (!mbk->switch_lst)
      mbk->switch_lst=List_Create();
    wchar_t *n1=new wchar_t[30];
    wstrcpy(n1, lng[ALLOW_ALL]);
    wchar_t *n2=new wchar_t[30];
    wstrcpy(n2, lng[FROM_LIST]);
    wchar_t *n3=new wchar_t[30];
    wstrcpy(n3, lng[NO]);
    List_InsertLast(mbk->switch_lst,n1);
    List_InsertLast(mbk->switch_lst,n2);
    List_InsertLast(mbk->switch_lst,n3);
    BookObj_CallPage(bk,&bk_Switch);
  }
  else
  {
    BookObj_CallPage(bk,&bk_main);
  }
};

void SelectEvent_OnBack(BOOK *bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK *)bk;
  mbk->selev->type=temp_act;
  if (old)
  {
    if (mbk->selev->param!=old)
    {
      delete(mbk->selev->param);
    }
    mbk->selev->param=old;
  }
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

int GetNumOfActions()
{
  int num=0;
  int x;
  for (x=0;x<ACTION_COUNT;x++)
  {
    if (items[x])
    {
      num++;
    }
    else
    {
      return num;
    }
  }
  return num;
};

int SelectEvent_onLBMessage(GUI_MESSAGE * msg)
{
  MyBOOK * myBook = (MyBOOK *) FindBook(isEvtEditBook);
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(items[item],ENC_UCS2,TEXTID_ANY_LEN));
    break;
  }
  return(1);
};

GUI_LIST * SelectEvent_CreateGui(MyBOOK * mbk)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(&mbk->book,0))
  {
    GUIObject_SetTitleText(lo,TextID_Create(lng[ACTIONS],ENC_UCS2,TEXTID_ANY_LEN));
    int num=GetNumOfActions();
    ListMenu_SetItemCount(lo,num);
    ListMenu_SetOnMessage(lo,SelectEvent_onLBMessage);
    ListMenu_SetItemStyle(lo,0);
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, SelectEvent_OnBack);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,SelectEvent_OnEnter);
    ListMenu_SetCursorToItem(lo,0);
  }
  return(lo);
};

GUI_LIST *SelectEvent_CreateList(BOOK *book)
{
  MyBOOK *mbk=(MyBOOK *)book;
  temp_act=mbk->selev->type;
  old=mbk->selev->param;
  mbk->addgui=SelectEvent_CreateGui(mbk);
  GUIObject_Show(mbk->addgui);
  return(mbk->addgui);
};

static int SEPage_OnEnter(void *,BOOK * bk)
{
  SelectEvent_CreateList(bk);
  return (1);
};

static int SEPage_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (old && mbk && mbk->selev && mbk->selev->param)
  {
    if (old!=mbk->selev->param)
    {
      delete(old);
    }
  }
  FREE_GUI(mbk->addgui);
  return (1);  
};

const PAGE_MSG bk_msglst_selectevent[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    SEPage_OnEnter,
  PAGE_EXIT_EVENT_TAG,     SEPage_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_selectevent = {"EvtEdit_SelectEvent_Page",0,bk_msglst_selectevent};

