#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"

#define ELFNAME "Management"
#define ABOUT L"Management\nv.3\n\n(c)Ploik & BigHercules"

enum mode_t {
    SHORT_PRESS  = 0,
    LONG_PRESS   = 1,
    DOUBLE_PRESS = 2
};

static char myappname[]=ELFNAME;

BOOK * AP = 0;
BOOK * FM = 0;

int dbl   = 0;
u16 timer = 0;
u16 timer_call_off = 0;
int Connect = 0;

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*);
int ControlKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*);

int isMTCallBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"MTCallBook")==0) { return 1; }
  else { return 0; }
};

int OnAccChangedEvent(void* r0,BOOK* b)
{
  AP = FindBook(get_IsAudioPlayerBook());
  FM = FindBook(get_IsFmRadioBook());
  
 //if (PHF_GetState ())
  //{
  if(!AP && !FM && turn_on==1)
  {
    if(turn_on_pr==0)
    {
       UI_Event(UI_MEDIAPLAYER_CONTROL_EVENT);
    }
     else
    {
       UI_Event(UI_FMRADIO_CONTROL_EVENT);
    }
   //}
 }
    return (1);      
}

int OffAccChangedEvent(void* r0,BOOK* b)
{
  AP = FindBook(get_IsAudioPlayerBook());
  FM = FindBook(get_IsFmRadioBook());
  
  if(AP && FM && turn_off==1)
  {
    GoMusic();
  }
    return (1);    
}

int isManagementBook(BOOK * book)
{
        if(!strcmp(book->xbook->name,myappname)) return true;
        return false;
}

BOOK * ManagementBook;

int TerminateElf(void * ,BOOK* book)
{
        FreeBook(book);
        return(1);
}

typedef struct
{
        BOOK * book;
}MSG;

int ShowAuthorInfo(void *mess ,BOOK* book)
{
         MSG * msg = (MSG*)mess;
         MessageBox(EMPTY_TEXTID, TextID_Create(ABOUT, ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 1, 5000, msg->book);
         return(1);
}

void onTimer_Call (u16 timerID, LPARAM lparam)
{
    ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
}

enum
{
         CALLMANAGER_IDLE            =0,
         CALLMANAGER_CALL_CONNECTED  =1, /*Поднятие трубки*/
         CALLMANAGER_CALL_SETUP      =2, /*Нажатие вызвать*/
         CALLMANAGER_CALL_ALERT      =4, /*Входящий вызов*/
         CALLMANAGER_CALL_WAITING    =5,
         CALLMANAGER_CALL_TERMINATED =6  /*Завершение соединения*/
};

int OnCallManagerEvent(void* r0,BOOK* b)
{
   int eventdata;
   
   switch (GetChipID()&CHIPID_MASK)
   {
   case CHIPID_DB2000:
   case CHIPID_DB2010:
   case CHIPID_DB2020:
                 eventdata= (((CALLMANAGER_EVENT_DATA*)r0)->CallState);
                 break;
   case CHIPID_DB3150:
   case CHIPID_DB3200:
   case CHIPID_DB3210:
   case CHIPID_DB3350:
                 eventdata=(((CALLMANAGER_EVENT_DATA_A2*)r0)->CallState);
   }
        switch (eventdata)
        {
         /*Поднятие трубки*/
         case CALLMANAGER_CALL_CONNECTED:
         {
           ModifyKeyHook(ControlKey,KEY_HOOK_REMOVE,NULL);
         }
         break;
         /*Входящий вызов*/
         case CALLMANAGER_CALL_ALERT:
         {
          ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
          ModifyKeyHook(ControlKey,KEY_HOOK_ADD,NULL);
          if(timer)
          {
           Timer_Kill(&timer);
           timer = 0;
           dbl = 0;
          }
         }
         break;
         /*Нажатие вызвать*/
         case CALLMANAGER_CALL_SETUP:
         {
          ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
          if(timer)
          {
           Timer_Kill(&timer);
           timer = 0;
           dbl = 0;
          }
         }
         break;
         /*Завершение соединения*/
         case CALLMANAGER_CALL_TERMINATED:
         {
          ModifyKeyHook(ControlKey,KEY_HOOK_REMOVE,NULL);
          timer_call_off=Timer_Set(2000,onTimer_Call,0);
         }
      break;
    }
  return 1;
}


int CheckConfig()
{
  wchar_t temp[256];
  char* err=NULL;

  int radio_short_press  = 0;
  int radio_long_press   = 0;
  int radio_double_press = 0;
  int player_short_press  = 0;
  int player_long_press   = 0;
  int player_double_press = 0;

  int radio_short_on_press  = 0;
  int radio_long_on_press   = 0;
  int player_short_on_press  = 0;
  int player_long_on_press   = 0;
  
  switch(radioOn)
  {
    case SHORT_PRESS: radio_short_on_press++;  break;
    case LONG_PRESS:  radio_long_on_press++;   break;
  }
  
  switch(radioOff)
  {
    case SHORT_PRESS: radio_short_press++;     break;
    case LONG_PRESS:  radio_long_press++;      break;
    case DOUBLE_PRESS: radio_double_press++;   break;
  }

  switch(radioNext)
  {
    case SHORT_PRESS: radio_short_press++;     break;
    case LONG_PRESS: radio_long_press++;       break;
    case DOUBLE_PRESS: radio_double_press++;   break;
  }

  switch(radioPrev)
  {
    case SHORT_PRESS: radio_short_press++;     break;
    case LONG_PRESS: radio_long_press++;       break;
    case DOUBLE_PRESS: radio_double_press++;   break;
  }

  switch(playerOn)
  {
    case SHORT_PRESS: player_short_on_press++; break;
    case LONG_PRESS: player_long_on_press++;   break;
  }
  
  switch(playerOff)
  {
    case SHORT_PRESS: player_short_press++;    break;
    case LONG_PRESS: player_long_press++;      break;
    case DOUBLE_PRESS: player_double_press++;  break;
  }

  switch(playerNext)
  {
    case SHORT_PRESS: player_short_press++;    break;
    case LONG_PRESS: player_long_press++;      break;
    case DOUBLE_PRESS: player_double_press++;  break;
  }

  switch(playerPrev)
  {
    case SHORT_PRESS: player_short_press++;    break;
    case LONG_PRESS: player_long_press++;      break;
    case DOUBLE_PRESS: player_double_press++;  break;
  }

  if((radio_short_press > 1) || (radio_long_press > 1) || (radio_double_press > 1))
  err="Неправильно сконфигурировано управление радио!";

  else if((player_short_press > 1) || (player_long_press > 1) || (player_double_press > 1))
  err="Неправильно сконфигурировано управление плеером!";

  else if((radio_short_on_press ==  1 && player_short_on_press ==  1) || (radio_long_on_press == 1 && player_long_on_press ==  1))
  err="Неправильно сконфигурирован запуск кнопкой!";
      
  if(err)
  {
    snwprintf(temp, MAXELEMS(temp), _T("Error!\n%s"),err);
    MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,0,0);
    return 1;
  }
  return 0;
}

static int onReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    CheckConfig();
    result=1;
  }
  return(result);
}

const PAGE_MSG Management_PageEvents[]@ "DYN_PAGE" =
{
         ELF_TERMINATE_EVENT,                          TerminateElf,
         ELF_SHOW_INFO_EVENT,                          ShowAuthorInfo,
         ELF_RECONFIG_EVENT,                           onReconfigElf,
         ON_CALLMANAGER_EVENT_TAG,                     OnCallManagerEvent,
         ACCESSORIES_ACCESSORY_CONNECTED_EVENT_TAG,    OnAccChangedEvent,
         ACCESSORIES_ACCESSORY_DISCONNECTED_EVENT_TAG, OffAccChangedEvent,
         NIL_EVENT_TAG,                                NULL
};

PAGE_DESC base_page ={"Management_BasePage",0,Management_PageEvents};


void elf_exit(void)
{
         kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void MakeAction(int mode)
{
  if (!AP && !FM)
  {
    if(mode == playerOn)   UI_Event(UI_MEDIAPLAYER_CONTROL_EVENT);
    if(mode == radioOn)    UI_Event(UI_FMRADIO_CONTROL_EVENT);
  }
  else if (AP)
  {
    if(mode == playerNext) UI_Event(UI_MEDIAPLAYER_NEXT_TRACK_EVENT);
    if(mode == playerPrev) UI_Event(UI_MEDIAPLAYER_PREV_TRACK_EVENT);
    if(mode == playerOff)  UI_Event(UI_MEDIAPLAYER_CONTROL_EVENT);
    return;
  }
  else if (FM)
  {
    if(mode == radioNext)  SwitchRadioStationFromList(FM, 0);
    if(mode == radioPrev)  SwitchRadioStationFromList(FM, 1);
    if(mode == radioOff)   UI_Event(UI_FMRADIO_CONTROL_EVENT);
  }
}

void onTimer(u16 timerID, LPARAM lparam)
{
  if (dbl==1)
  {
    MakeAction(SHORT_PRESS);
  }
  else
  {
    MakeAction(DOUBLE_PRESS);
  }
  dbl=0;
}

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
  AP = FindBook(get_IsAudioPlayerBook());
  FM = FindBook(get_IsFmRadioBook());
  if (key==KeyControl)
  {
    if (mode==KBD_SHORT_RELEASE)
    {
      if (!dbl) timer=Timer_Set(detectTime,onTimer,0);
      dbl++;
    }
    if (mode==KBD_LONG_PRESS)
    {
      MakeAction(LONG_PRESS);
    }
    return(-1);
  }
  return(0);
}

int ControlKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
  BOOK *bk = FindBook(isMTCallBook);
  if (key==KeyControl)
  {
    if (mode==KBD_SHORT_RELEASE)
    {
      IncommingCall_Accept(bk);
    }
    if (mode==KBD_LONG_PRESS)
    {
      IncommingCall_Reject(bk);
    }
    return(-1);
  }
  return(0);
}
void onCloseManagementBook(BOOK * book)
{
  ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
  ModifyKeyHook(ControlKey,KEY_HOOK_REMOVE,NULL);
  if(timer) Timer_Kill(&timer);
  if(timer_call_off) Timer_Kill(&timer_call_off);
  SUBPROC(elf_exit);
}

int main (void)
{
  BOOK* alreadyrunned=FindBook(isManagementBook);
  if(alreadyrunned)
  {
    MessageBox(EMPTY_TEXTID, STR("Management headset\nуже запущен"), NOIMAGE, 1 ,5000, 0);
    SUBPROC(elf_exit);
  }
  else
  {
    InitConfig();
    if(CheckConfig())
    {
      SUBPROC(elf_exit);
      return 0;
    }
    
    ManagementBook = new BOOK;
    if(!CreateBook(ManagementBook, onCloseManagementBook, &base_page, ELFNAME, -1, 0))
    {
      delete ManagementBook;
      SUBPROC(elf_exit);
      return 0;
    }
    ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
  }
  return(0);
}
