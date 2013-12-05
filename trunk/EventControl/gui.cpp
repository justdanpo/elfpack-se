/*
*===========================================================================
*                       Модуль интерфейса проверки событий
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\lng.h"

extern LIST *events;

#pragma swi_number=0x109
__swi __arm  void MMIPROC (void(*PROC)(EVENT *), EVENT *);
void SUBPROC_(void(*PROC)(EVENT *),EVENT *p1, void * p2);

void action_processor(EVENT *ev);

GUI_LIST *g=0;

wchar_t *items[] = {
  L"Сообщение",
  L"Запуск эльфа",
  L"Ярлык",
  L"Java",
  L"Выключение телефона",
  L"Яркость", 
  L"Фонарик: уст. яркость",
  L"Профиль",
  L"Рестарт",
  L"Музыка: воспр.", 
  L"Музыка: воспр. с громкостью",
  L"Обои: установить",
  L"Вибра",
  L"ГЭ: уст. заметку",
  L"ГЭ: уст. оператора",
  L"Закрыть книгу",
  L"Вкл. музыку/радио",
  L"Поменять расписание",
  L"Обновить",
  L"Звук: выкл",
  L"Звук: вкл", 
  L"ИК: вкл",
  L"ИК: выкл", 
  L"ИК: мигнуть",
  L"Фонарик: мигнуть",
  L"(W580): отключить диоды",
  L"(W580): включить диоды", 
  L"Оранж.диод: мигнуть", 
  L"Оранж.диод: вкл. ",
  L"Оранж.диод: выкл",
  L"Важное событие", 
  L"Звонок: уст.громкость",
  L"Звонок: вибрация",
  L"Звонок: уст.файл", 
  L"Звонок: способ приёма",
  L"Звонок: вкл.нарастание",
  L"Звонок: уст. приём вызовов",
  L"Голосовой вызов",
  L"Сменить тему",
  0,0,0
};

int onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    EVENT* evt=(EVENT *)List_Get(events,item);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(items[evt->type],ENC_UCS2,TEXTID_ANY_LEN));
    break;
  }
  return(1);
};

void OnSelect1Gui(BOOK *bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(g);
  EVENT *ev=(EVENT*)List_Get(events,item);
  MMIPROC(action_processor,ev);
};

void OnSelect2Gui(BOOK *bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(g);
  wchar_t text[1024];
  EVENT *ev=(EVENT*)List_Get(events,item);
  snwprintf(text, 1023, L"%d-%d-%d-%d-%d-%d-%d\n\n%d-%d-%d-%d-%d-%d-%d\n\n%d-%d-%d-%d-%d-%d-%d\n%d:%d:%d", ev->days[0], ev->days[1], ev->days[2], ev->days[3], ev->days[4], ev->days[5], ev->days[6], ev->remdays[0], ev->remdays[1], ev->remdays[2], ev->remdays[3], ev->remdays[4], ev->remdays[5], ev->remdays[6], ev->remdays2[0], ev->remdays2[1], ev->remdays2[2], ev->remdays2[3], ev->remdays2[4], ev->remdays2[5], ev->remdays2[6], ev->tm_start, ev->ask_before, ev->ask_after);
  MessageBox(EMPTY_TEXTID,TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
};
void OnBackGui(BOOK * bk, GUI* )
{
  GUIObject_Destroy(g);
  g=0;
  MyBOOK *mbk=(MyBOOK*)bk;
  mbk->g_test=0;
  BookObj_Hide(bk, 0);
};

GUI_LIST * CreateGuiList(BOOK * book)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    ListMenu_SetItemCount(lo,events->FirstFree);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,onLBMessage);
    ListMenu_SetItemStyle(lo,3);
    GUIObject_SetTitleText(lo,TextID_Create(lng[LNG_EVENTS_TEST],ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, OnBackGui);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,OnSelect1Gui);
    GUIObject_SoftKeys_SetAction(lo,0,OnSelect2Gui);
    GUIObject_SoftKeys_SetText(lo,0,TextID_Create(lng[LNG_INFO],ENC_UCS2,TEXTID_ANY_LEN));
  }
  return(lo);
};

GUI_LIST *create_ed(BOOK *book)
{
  if (events)
  {
    MyBOOK *mbk=(MyBOOK*)book;
    g=CreateGuiList(book);
    mbk->g_test=g;
    GUIObject_Show(g);
    BookObj_Show(book, 0);
    BookObj_SetFocus(book, 0);
  }
  return(g);
};
