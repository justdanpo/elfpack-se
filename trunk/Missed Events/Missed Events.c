#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "util.h"

#define ELFNAME "MissedEvents v.2.3"
#define LELFNAME L"Missed Events"
#define LELFVERSION L"\nv2.3\nby den_po\n\nMods: Ploik & BigHercules"

void (*LEDControl_W580)(int,int id,int RED,int GREEN,int BLUE, int br, int delay)=(void (*)(int,int id,int RED,int GREEN,int BLUE,int br,int delay))(0x4529BFA9);

#define COLOR_GET_A(x) ((unsigned int)x>>24)
#define COLOR_GET_R(x) (((unsigned int)x>>16)&0xFF)
#define COLOR_GET_G(x) (((unsigned int)x>>8)&0xFF)
#define COLOR_GET_B(x) ((unsigned int)x&0xFF)

static char myappname[]=ELFNAME;

bool disabled=false;
bool disabled_by_mode=false;

bool sound_disabled_by_time = false;
bool vibra_disabled_by_time = false;
bool flash_disabled_by_time = false;
bool redled_disabled_by_time = false;
bool led_disabled_by_time = false;
bool screen_disabled_by_time = false;

int lamp=0;
int screen=0;

int REDLED=0;
int LED=0;
int LED580=1;
int LEDnum=2;

wchar_t cfg_filepath[256];
wchar_t cfg_filename[256];

u16 timerFlash = 0;
u16 timerScreen = 0;
u16 timerREDLED = 0;
u16 timerLED = 0;
u16 timer = 0;
u16 offtimer = 0;
u16 offtimerScreen = 0;
u16 offtimerLED = 0;
u16 offtimerREDLED = 0;
u16 modetimer = 0;

int cfg_time1_flag = 1;
TIME cfg_time1_on   = {6,30,0,0};
TIME cfg_time1_off  = {23,0,0,0};

int cfg_time2_flag = 1;
TIME cfg_time2_on   = {6,30,0,0};
TIME cfg_time2_off  = {23,0,0,0};

int number = 0x7ffffff;

int skipevents;
int checkevents;

enum
{
  WAIT = 0,
  MISS = 1
};

int state = WAIT;

int timecmp(const TIME *t1, const TIME *t2)
{
    if(t1->hour > t2->hour) return 1;
    if(t1->hour < t2->hour) return -1;
    if(t1->min > t2->min) return 1;
    if(t1->min < t2->min) return -1;
    return 0;
}

void onTimerREDLED(u16 timerID, LPARAM lparam)
{
  if(REDLED == 0)
  {
    REDLED = 1;
    RedLED_On(0);
    Timer_ReSet(&timerREDLED,cfg_redled_ontime,onTimerREDLED,0);
  }
  else
  {
    REDLED = 0;
    RedLED_Off(0);
    Timer_ReSet(&timerREDLED,cfg_redled_offtime,onTimerREDLED,0);
  }
}

void onTimerLED(u16 timerID, LPARAM lparam)
{
  if(cfg_phone_type == 1)
  {
    Timer_ReSet(&timerLED,cfg_led_blink_speed,onTimerLED,0);
    if (cfg_led580_blink_mode == 2)
    {
      LEDControl_W580(1, 0, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580 ^= 1);
      LEDControl_W580(1, 1, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580);
    }
    else
    {
      LEDControl_W580(1, cfg_led580_blink_mode, COLOR_GET_R(cfg_led580_color),COLOR_GET_G(cfg_led580_color),COLOR_GET_B(cfg_led580_color),0,LED580 ^= 1);
    }
  }
  else if(cfg_phone_type == 2)
  {
    Timer_ReSet(&timerLED,100,onTimerLED,0);
    if (cfg_led_blink_mode)
    {
      OrangeLED_Control(1,LEDnum,0,cfg_led_blink_speed);
      if (LEDnum == 2) LEDnum=0;
      else LEDnum++;
      OrangeLED_Control(1,LEDnum,100,cfg_led_blink_speed);
    }
    else
      OrangeLED_Control(1,0,LED^=0x64,cfg_led_blink_speed);
  }
}

void onTimerFlash(u16 timerID, LPARAM lparam)
{
  switch(GetChipID()&CHIPID_MASK)
  {
   case CHIPID_DB2000:
   case CHIPID_DB2010:
   {
    SetLampLevel(lamp^=0x50);
   }
   break;
   case CHIPID_DB2020:
   {
    SetLampLevel(lamp^=1);
   }
   break;
  }
  Timer_ReSet(&timerFlash,cfg_flash_blink_speed,onTimerFlash,0);
}

void onTimerScreen(u16 timerID, LPARAM lparam)
{
    IndicationDevice_Backlight_FadeToLevel(0,screen ^= cfg_screen_level);
    Timer_ReSet(&timerScreen,cfg_screen_blink_speed,onTimerScreen,0);
}

void offTimerREDLED(u16 timerID, LPARAM lparam)
{
   if(timerREDLED)
   {
     Timer_Kill(&timerREDLED);
     timerREDLED = 0;
     RedLED_Off(0);
   }
   REDLED = 0;
}

void offTimerLED(u16 timerID, LPARAM lparam)
{
   if(timerLED)
   {
     Timer_Kill(&timerLED);
     timerLED = 0;
   }
   if(cfg_phone_type == 1)
   {
     LEDControl_W580(1,0,0,0,0,0,1);
     LEDControl_W580(1,1,0,0,0,0,1);
   }
   else if(cfg_phone_type == 2)
   {
     for(int i=0;i<3;i++)
     OrangeLED_Control(1,i,0,50);
   }
   LEDnum = 0;
   LED    = 0;
   LED580 = 1;
}

void offTimerFlash(u16 timerID, LPARAM lparam)
{
  if(timerFlash)
  {
    Timer_Kill(&timerFlash);
    timerFlash = 0;
    SetLampLevel(0);
  }
}

void offTimerScreen(u16 timerID, LPARAM lparam)
{
  if(timerScreen)
  {
    Timer_Kill(&timerScreen);
    IndicationDevice_Backlight_FadeToLevel(0,cfg_screen_level);
    timerScreen = 0;
  }
  screen = 0;
}

void offModeTimer(u16 timerID, LPARAM lparam)
{
    disabled_by_mode = true;
}

void onTimer(u16 timerID, LPARAM lparam)
{
    bool time1_limit = false;
    bool time2_limit = false;

    if(cfg_time1_limit)
    {
        DATETIME dt;
        REQUEST_DATEANDTIME_GET(SYNC, &dt);
        if(cfg_time1_flag == 1)
        {
            if((timecmp(&(dt.time),&cfg_time1_on) < 0) && (timecmp(&(dt.time),&cfg_time1_off) >= 0)) time1_limit = true;
        }
        else
        {
            if((timecmp(&(dt.time),&cfg_time1_on) < 0) || (timecmp(&(dt.time),&cfg_time1_off) >= 0)) time1_limit = true;
        }
    }

    if(cfg_time2_limit)
    {
        DATETIME dt;
        REQUEST_DATEANDTIME_GET(SYNC, &dt);
        if(cfg_time2_flag == 1)
        {
            if((timecmp(&(dt.time),&cfg_time2_on) < 0) && (timecmp(&(dt.time),&cfg_time2_off) >= 0)) time2_limit = true;
        }
        else
        {
            if((timecmp(&(dt.time),&cfg_time2_on) < 0) || (timecmp(&(dt.time),&cfg_time2_off) >= 0)) time2_limit = true;
        }
    }

    if((!cfg_time1_limit) && (!cfg_time2_limit))
    {
      sound_disabled_by_time  =
      vibra_disabled_by_time  =
      flash_disabled_by_time  =
      redled_disabled_by_time =
      led_disabled_by_time    =
      screen_disabled_by_time = false;
    }
    else
    {
      sound_disabled_by_time  =
      vibra_disabled_by_time  =
      flash_disabled_by_time  =
      redled_disabled_by_time =
      led_disabled_by_time    =
      screen_disabled_by_time = true;

      if(!time1_limit)
      {
        sound_disabled_by_time  = (cfg_time1_sound)  ? false : true;
        vibra_disabled_by_time  = (cfg_time1_vibra)  ? false : true;
        flash_disabled_by_time  = (cfg_time1_flash)  ? false : true;
        redled_disabled_by_time = (cfg_time1_redled) ? false : true;
        led_disabled_by_time    = (cfg_time1_leds)   ? false : true;
        screen_disabled_by_time = (cfg_time1_screen) ? false : true;
      }
      if(!time2_limit)
      {
        sound_disabled_by_time  = (cfg_time2_sound)  ? false : true;
        vibra_disabled_by_time  = (cfg_time2_vibra)  ? false : true;
        flash_disabled_by_time  = (cfg_time2_flash)  ? false : true;
        redled_disabled_by_time = (cfg_time2_redled) ? false : true;
        led_disabled_by_time    = (cfg_time2_leds)   ? false : true;
        screen_disabled_by_time = (cfg_time2_screen) ? false : true;
      }
    }

    if(cfg_mode)
    {
        char* me=MissedEvents();
        char mestatus=*me;
        if( !(mestatus & checkevents) )
        {
            if(modetimer)
            {
                Timer_Kill(&modetimer);
                modetimer = 0;
            }
            disabled_by_mode = false;
        }
    }
    else disabled_by_mode = false;

    if((!disabled) && (!disabled_by_mode))
    {
        char* me=MissedEvents();
        char mestatus=*me;
        if( mestatus & checkevents )
        {
            if(state == WAIT)
            {
                state = MISS;
                Timer_ReSet(&timer,cfg_pretime*1000,onTimer,lparam);
                if(cfg_mode == 1) modetimer=Timer_Set((cfg_pretime + cfg_duration*60)*1000,offModeTimer,0);
                if(cfg_mode == 2) number = cfg_duration;
                return;
            }
            if(cfg_mode == 2)
            {
                if(--number <= 0) disabled_by_mode = true;
            }
            if((!GetSilent() || (cfg_silent==1)) && (cfg_sound==1) && (!sound_disabled_by_time))
            {
                char vol;
                GetAudioSettings(2,&vol);
                PlayFileV(cfg_filepath,cfg_filename,vol);
            }
            if(GetVibrator(0,0) && (cfg_vibra==1) && (!vibra_disabled_by_time))
            {
                PAudioControl pAC = AudioControl_Init();
                if( !pAC ) pAC = *GetAudioControlPtr();
                AudioControl_Vibrate(pAC, 400, 50, cfg_vibra_time*1000);
            }
            if((cfg_flash==1) && (!flash_disabled_by_time))
            {
                timerFlash=Timer_Set(cfg_flash_blink_speed,onTimerFlash,0);
                offtimer=Timer_Set(cfg_flash_time*1000,offTimerFlash,0);
            }
            if((cfg_screen==1) && (!screen_disabled_by_time))
            {
                BOOK * ScreenSaverBook = FindBook(get_IsScreenSaverBook());
                if (ScreenSaverBook)
                {
                  UI_Event_toBookID(UI_SCREENSAVER_DEACTIVATE_EVENT,BookObj_GetBookID(ScreenSaverBook));
                }
                timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
                offtimerScreen=Timer_Set(cfg_screen_time*1000,offTimerScreen,0);
            }
            if((cfg_led==1) && (!led_disabled_by_time))
            {
                timerLED=Timer_Set(70,onTimerLED,0);
                offtimerLED=Timer_Set(cfg_led_time*1000,offTimerLED,0);
            }
            if((cfg_redled==1) && (!redled_disabled_by_time))
            {
                timerREDLED=Timer_Set(70,onTimerREDLED,0);
                offtimerREDLED=Timer_Set(cfg_redled_time*1000,offTimerREDLED,0);
            }
        }
        else
        {
            state = WAIT;
        }
#ifdef DEBUG
        if( mestatus & ~(skipevents|checkevents))
        {
            u16 temp[256];
            snwprintf(temp, MAXELEMS(temp), _T("Неизвесный статус\n%x: %x"), me, mestatus &~(skipevents|checkevents));
            MessageBox(EMPTY_TEXTID, TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)), NOIMAGE, 1, 0, 0);
            skipevents|=mestatus;
        }
#endif
    }
    if(state == WAIT)
    {
        Timer_ReSet(&timer,cfg_checkperiod*1000,onTimer,lparam);
    }
    else
    {
        Timer_ReSet(&timer,cfg_period*1000,onTimer,lparam);
    }
}

int UpdateParams(void)
{
  wchar_t temp[256];
  char* err=NULL;
  FSTAT _fstat;
  TIME zero_time   = {0,0,0,0};

  extractdir(cfg_filepath, cfg_filename, cfg_music_file);

  cfg_time1_on  = cfg_on_time1;
  cfg_time1_off = cfg_off_time1;

  cfg_time2_on  = cfg_on_time2;
  cfg_time2_off = cfg_off_time2;

  if(timecmp(&cfg_time1_on, &zero_time) == 0) cfg_time1_on.hour = 24;
  cfg_time1_flag = timecmp(&cfg_time1_on, &cfg_time1_off);

  if(timecmp(&cfg_time2_on, &zero_time) == 0) cfg_time2_on.hour = 24;
  cfg_time2_flag = timecmp(&cfg_time2_on, &cfg_time2_off);

  if(cfg_sound && (fstat(cfg_filepath,cfg_filename,&_fstat)!=0))
    #ifndef ENG
      err="Звуковой файл не найден!";
    #else
      err="Notification file not found!";
    #endif
  else
    if(cfg_pretime < cfg_checkperiod) err="PRE_TIME < CHECK";
  else
    if(timecmp(&cfg_on_time1, &cfg_off_time1) == 0) err="ON_TIME1==OFF_TIME1";
  else
    if(timecmp(&cfg_on_time2, &cfg_off_time2) == 0) err="ON_TIME2==OFF_TIME2";

  if(err)
  {
    snwprintf(temp, MAXELEMS(temp), _T("Error!\n%s"),err);
    MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,0,0);
    return 1;
  }
  return 0;
}

void elf_exit(void)
{
        kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void bookOnDestroy(BOOK * book)
{

        Timer_Kill(&timer);
        if(timerFlash) offTimerFlash(0,0);
        if(timerScreen) offTimerScreen(0,0);
        if(offtimer) Timer_Kill(&offtimer);
        if(timerREDLED) offTimerREDLED(0,0);
        if(timerLED) offTimerLED(0,0);
        if(offtimerREDLED) Timer_Kill(&offtimerREDLED);
        if(offtimerLED) Timer_Kill(&offtimerLED);
        if(offtimerScreen) Timer_Kill(&offtimerScreen);
        if(modetimer) Timer_Kill(&modetimer);
        StatusIndication_ShowNotes(EMPTY_TEXTID);
        SUBPROC(elf_exit);
}

int onExit(void* r0,BOOK* b)
{
        FreeBook(b);
        return 1;
}

int onAbout(void* mess,BOOK* b)
{
        MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *(BOOK**)mess);
        return 1;
}

enum
{
        CALLMANAGER_IDLE            =0,
        CALLMANAGER_CALL_CONNECTED  =1,
        CALLMANAGER_CALL_SETUP      =2,
        CALLMANAGER_CALL_ALERT      =4,
        CALLMANAGER_CALL_WAITING    =5,
        CALLMANAGER_CALL_TERMINATED =6
};

int OnCallManagerEvent(void* r0,BOOK* b)
{
        switch(((CALLMANAGER_EVENT_DATA*)r0)->CallState)
        {
        case CALLMANAGER_CALL_SETUP:
        case CALLMANAGER_CALL_ALERT:
                disabled=true;
                break;
        case CALLMANAGER_CALL_TERMINATED:
                disabled=false;
                break;
        }
        return 1;
}

static int onReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    UpdateParams();
    result=1;
  }
  return(result);
}

const PAGE_MSG evtlist[] @ "DYN_PAGE"=
{
  ELF_TERMINATE_EVENT,      onExit,
  ELF_SHOW_INFO_EVENT,      onAbout,
  ELF_RECONFIG_EVENT,       onReconfigElf,
  ON_CALLMANAGER_EVENT_TAG, OnCallManagerEvent,
  NIL_EVENT_TAG,            NULL
};

const PAGE_DESC defaultpage = {"MissedEvents_base_page",0,evtlist};

int isMissedEventsBook(BOOK * book)
{
        if(!strcmp(book->xbook->name,myappname)) return true;
        return false;
}

int main(wchar_t* filename)
{
        BOOK* alreadyrunned=FindBook(isMissedEventsBook);
        if(alreadyrunned)
        {
            #ifndef ENG
                MessageBox(EMPTY_TEXTID, STR("MissedEvents\nуже запущен"), NOIMAGE, 1 ,5000, 0);
            #else
                MessageBox(EMPTY_TEXTID, STR("MissedEvents\nalready runned"), NOIMAGE, 1 ,5000, 0);
            #endif
            SUBPROC(elf_exit);
        }
         else
        {
                InitConfig();
                if(UpdateParams())
                {
                    SUBPROC(elf_exit);
                    return 0;
                }

                switch(GetChipID()&CHIPID_MASK)
                {
                case CHIPID_DB2000:
                case CHIPID_DB2010:
                        skipevents=0;
                        checkevents=1|2|4|8;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
                        break;
                case CHIPID_DB2020:
                case CHIPID_DB3150:
                case CHIPID_DB3200:
                case CHIPID_DB3210:
                case CHIPID_DB3350:
                        skipevents=1|2;//KEYLOCKED|NOSOUND
                        checkevents=4|8|0x10|0x20;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
                }

                BOOK *myBook=(BOOK*)malloc(sizeof(BOOK));
                memset(myBook,0,sizeof(BOOK));
                if(!CreateBook(myBook,bookOnDestroy,&defaultpage,myappname,-1,0))
                {
                  delete myBook;
                  SUBPROC(elf_exit);
                  return 0;
                }

                timer=Timer_Set(cfg_checkperiod*1000, onTimer, 0);
                if(!wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)))
                  StatusIndication_ShowNotes(EMPTY_TEXTID);
        }

        return 0;
}

/*
  Revision history
  2.3
     + Добавлено мигание красным светодиодом
  2.2
     + Использование функции AudioControl_Init
     + Устранена утечка памяти.
  2.1
     + Устранена утечка памяти
  2.0.9
     + Использование функции AudioControl_Vibrate вместо Vibra
  2.0.7
     + мелкие улучшения/исправления/оптимизация
  2.0.6
    + Изменения в структуре конфигурационного файла.
    + Два диапазона времени для режима ограничения по времени с
      возможностью выбора чем напоминать в том или ином диапазоне.
      В случае перекрытия диапазонов, приоритетным будет запрещение напоминать чем либо,
      если в одном случае данный вид напоминания разрешен, а в другом запрещен.
    + Исправлены небольшие ошибки.
  2.0.5
    + Исправлены небольшие ошибки в алгоритме работы.
  2.0.4
    + Выход из слипмода только в том случае, если включено мигать экраном.
  2.0.3
    + Изменения в структуре конфигурационного файла.
    + мигание светодиодами на W580
  2.0.2
    + Если находится в слипмоде, то перед напоминанием выход из него.
  2.0.1
    + Изменения в структуре конфигурационного файла.
    + мигание дисплея
    + мигание светодиодами
    + английская версия конфигурационного файла. (Вибирается конфигурацией проекта Release_en)
  2.0
    + переход на bcfg

*/
