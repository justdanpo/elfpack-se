/*
 ****************************************************************************
 *                     Модуль выполнения событий EventControl'а
 ****************************************************************************
 */
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\lng.h"

extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);
extern char h,m,s,DayOfTheWeek,isWeekend;
extern DATETIME dt;
extern MyBOOK *ECBook;
extern wchar_t *extFolder;
extern wchar_t *intFolder;

int readConfig(MyBOOK *myBook, wchar_t *myFolder, wchar_t *fname);
void CreateReminder(REMINDER_STRUCT * rem_str);
void mbox_Create(BOOK * bk, wchar_t *text, int minutes, bool vibra);

void SUBPROC_(void(*PROC)(int,void*),int p1, void * p2);

#pragma swi_number=0x109
__swi __arm  void MMIPROC (void(*PROC)(EVENT *), EVENT *);

#pragma swi_number=0x109
__swi __arm  void MMIPROC (void(*PROC)(wchar_t *),wchar_t *);

#pragma swi_number=0x109
__swi __arm  void MMIPROC (void(*PROC)(REMINDER_STRUCT *),REMINDER_STRUCT *);

/*
*===========================================================================
*                        Ищем книгу по названию (C) den_po
*===========================================================================
*/
BOOK* findbook(char* name)
{
  int sesscount;
  BOOK * book;
  int sess,bk;
  UI_APP_SESSION * session; 
  sesscount=root_list_get_session_count(); 
  for(sess=0;sess<sesscount;sess++)
  {
    session=root_list_get_session(sess);
    for (bk=0;bk<session->listbook->FirstFree;bk++)
    {
      book=(BOOK*)List_Get(session->listbook,bk);
      if(!strcmp(book->xbook->name,name))return book;
    }   
  }
  return NULL; 
};

/*
*================================================================================
* Проверяем книгу на наличие в ней обработчика определённого события (C) Hussein
*================================================================================
*/
int CheckEv(BOOK * bk, int ev)
{
  PAGE_MSG * pm;
  if (bk->base_page)
  {
    pm=(PAGE_MSG *)bk->base_page->msg_lst;
    if (pm)
    {
      while ((pm->msg) && (pm->proc))
      {
        if (pm->msg==ev)
        {
          return(1);
        }
        pm++;
      }
    }
  }
  return(0);
};

/*
*===========================================================================
*     Функция сравнения названия явы из JavaDesc'а с нужным нам именем 
*===========================================================================
*/
int CheckName(void * JavaDesc, wchar_t *name)
{
  wchar_t * sp;
  JavaAppDesc_GetJavaAppInfo(JavaDesc,0,&sp);
  return(wstrcmp(sp,name));
};

/*
*===========================================================================
*              Функция запуска ява-приложения по его названию
*===========================================================================
*/
int LaunchJava(wchar_t *name)
{
  int ID=-1;
  char sp;
  void * JavaDesc;
  JavaDialog_Open(0,&sp,&JavaDesc);
  if (!JavaAppDesc_GetFirstApp(JavaDesc))
  {
    int result=0;
    while ((CheckName(JavaDesc,name))&&(!result))
    {
      result=JavaAppDesc_GetNextApp(JavaDesc);
    }
    if (!result) ID=JavaAppDesc_GetJavaAppID(JavaDesc);
  }
  JavaDialog_Close(sp);
  if (ID!=-1)
  {
    char sp_flag;
    REQUEST_UI_OAF_START_APPLICATION(ASYNC,ID+0x10000,&sp_flag);
  }
  return(ID);
};

/*
*===========================================================================
*             Функция определения книги ColorSlider для FindBook
*===========================================================================
*/
int isCSBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"ColorSlider")==0)return 1;
  return 0;
};

/*
*===========================================================================
*       Запускаем голосовой вызов
*===========================================================================
*/
void MakeVC(wchar_t *num)
{
  char vc[256];
  VCALL_Init(vc);
  VCALL_SetNumber(vc,num, wstrlen(num));
  MakeVoiceCall(0,vc,1);
};

int notsupported=0;
int (*LED)(void *,int id,int RED,int GREEN,int BLUE, int delay, int onoff)=0;
void (*CONTROL)(int a, int id, int br, int c)=0;
#define LED_R8BA 0x4529A85D
#define CTRL_R8BA 0x4529A7FD
#define CTRL_R8BE 0x4529BF49
#define LED_R8BE 0x4529BFA9
#define CTRL_R6CA 0x452C9B09
#define LED_R6CA 0x452C9B69
#define CTRL_R8BE_Orange 0x452D7ACD
#define LED_R8BE_Orange 0x452D7B2D

int getfw()
{
  if (GetChipID()==0x9900)
  {
    char *test=(char*)(0x4529BFA8);
    if (test[0]==0x30 && test[1]==0xB5)
    {
      //W580 R8BE001
      LED=(int (*)(void *,int id,int RED,int GREEN,int BLUE,int delay,int onoff))(LED_R8BE);
      CONTROL=(void (*)(int a, int id, int br, int c))(CTRL_R8BE);
    }
    else if (test[0]==0xE5 && test[1]==0xFD)
    {
      //W580 R8BA024
      LED=(int (*)(void *,int id,int RED,int GREEN,int BLUE,int delay,int onoff))(LED_R8BA);
      CONTROL=(void (*)(int a, int id, int br, int c))(CTRL_R8BA);
    }
    else if (test[0]==0xA8 && test[1]==0x33)
    {
      //W580 R6CA001 Orange
      LED=(int (*)(void *,int id,int RED,int GREEN,int BLUE,int delay,int onoff))(LED_R6CA);
      CONTROL=(void (*)(int a, int id, int br, int c))(CTRL_R6CA);
    }
    else if (test[0]==0x00 && test[1]==0x28)
    {
      //W580 R8BE001 Orange
      LED=(int (*)(void *,int id,int RED,int GREEN,int BLUE,int delay,int onoff))(LED_R8BE_Orange);
      CONTROL=(void (*)(int a, int id, int br, int c))(CTRL_R8BE_Orange);
    }
    else
    {
      notsupported=1;
      return 0;
    }
  }
  else return 0;
  return 1;
};

void led(int,void*)
{
  for (int x=0;x<7;x++)
  {
    LED(0,1,0,100,0,100,0);
    LED(0,0,0,100,0,100,0);
    delay(500);
    LED(0,0,100,0,0,100,0);
    LED(0,1,100,0,0,100,0);
    delay(500);
    LED(0,1,0,0,100,100,0);
    LED(0,0,0,0,100,100,0);
    delay(500);
  }
  LED(0,1,0,0,100,100,1);
  LED(0,0,0,0,100,100,1);
};

void orangeled(int,void*)
{
  for (int x=0;x<6;x++)
  {
    OrangeLED_Control(1, 0, 0, 100);
    OrangeLED_Control(1, 1, 0, 100);
    OrangeLED_Control(1, 2, 0, 100);
    delay(50);
    OrangeLED_Control(1, 0, 100, 100);
    delay(25);
    OrangeLED_Control(1, 0, 0, 100);
    OrangeLED_Control(1, 1, 100, 100);
    delay(25);
    OrangeLED_Control(1, 1, 0, 100);
    OrangeLED_Control(1, 2, 100, 100);
    delay(25);
    OrangeLED_Control(1, 2, 0, 100);
    delay(25);
  }
};

void redled(int, void*)
{
  for (int x=0;x<6;x++)
  {
    RedLED_On(0);
    delay(500);
    RedLED_Off(0);
    delay(500);
    RedLED_On(0);
    delay(500);
    RedLED_Off(0);
    delay(500);
    RedLED_On(0);
    delay(500);
    RedLED_Off(0);
    delay(500);
  }
};

void vibrate_sub(int,void*)
{
  PAudioControl pAC = AudioControl_Init();
  if( !pAC )
    pAC = *GetAudioControlPtr();
  AudioControl_Vibrate(pAC, 500, 50, 3000);
};

void exec_indication(int ind)
{
  if ((ind)&0xF)
  {
    if (LED)
    {
       SUBPROC_(led,0,0);
    }
  }
  if ((ind>>4)&0xF)
  {
    SUBPROC_(orangeled,0,0);
  }
  if ((ind>>8)&0xF)
  {
    SUBPROC_(redled,0,0);
  }
  if ((ind>>12)&0xF)
  {
    MMIPROC(vibrate_sub,0,0);
  }
  if ((ind>>16)&0xF)
  {
    if (ECBook->indpath && ECBook->indname)
      PlayFile(ECBook->indpath,ECBook->indname);
    else
    {
      FSTAT fst;
      if (fstat(extFolder,L"notific.mp3",&fst)==0)
      {
        PlayFile(extFolder,L"notific.mp3");
      }
      else if (fstat(intFolder,L"notific.mp3",&fst)==0)
      {
        PlayFile(intFolder,L"notific.mp3");
      }
    }
  }
};

void act_proc(EVENT *ev)
{
  int t=ev->type;
  if (t==0)
  {
    mbox_Create(0, ev->param, 0, false);
  }
  else if (t==1)
  {
    elfload(ev->param,0,0,0);
  }
  else if (t==2)
  {
    Shortcut_Run(ev->param);
  }
  else if (t==3)
  {
    LaunchJava(ev->param);
  }
  else if (t==4)
  {
    REQUEST_SYSTEM_SHUTDOWN();
  }
  else if (t==5)
  {
    Display_SetBrightness(0, wcstoul(ev->param,0,10));
  }
  else if (t==6)
  {
    SetLampLevel(wcstoul(ev->param,0,10));
  }
  else if (t==7)
  {
    int pr=0;
    REQUEST_PROFILE_GETACTIVEPROFILE(SYNC, &pr);
    int npr=wcstoul(ev->param,0,10);
    if (npr>0)
    {
      npr--;
      if (pr!=npr)
      {
        Profile_SetActive(npr, 1);
      }
    }
  }
  else if (t==8)
  {
    REQUEST_SYSTEM_RESTART();
  }
  else if (t==9)
  {
    wchar_t path[512];
    wchar_t name[512];
    wstrcpy(path,ev->param);
    *(wstrrchr(path,L'/'))=0;
    wstrcpy(name,wstrrchr(ev->param,L'/'));
    PlayFile(path,name);
  }
  else if (t==10)
  {
    wchar_t path[512];
    wchar_t name[512];
    wchar_t vol[4];
    wstrncpy(vol,ev->param,3);
    vol[3]=L'\0';
    int volume=wcstoul(vol,0,10);
    wchar_t *pth=wstrchr(ev->param,L'.');
    pth++;
    wstrcpy(path,pth);
    *(wstrrchr(path,L'/'))=0;
    wstrcpy(name,wstrrchr(ev->param,L'/'));
    PlayFileV(path,name,volume);
  }
  else if (t==11)
  {
    wchar_t path[512];
    wchar_t name[512];
    wstrcpy(path,ev->param);
    wchar_t *hz=wstrrchr(path,L'/');
    hz++;
    *hz=0;
    wchar_t *hz2=wstrrchr(ev->param,L'/');
    hz2++;
    wstrcpy(name,hz2);
    StandbyBackground_SetImage(6,0,0,path,name,0);
  }
  else if (t==12)
  {
    PAudioControl pAC = AudioControl_Init();
    if( !pAC )pAC = *GetAudioControlPtr();
    int v_sec=wcstoul(ev->param,0,10);
    if (v_sec<=0)v_sec=1;
    AudioControl_Vibrate(pAC, 500, 50, v_sec * 1000);
  }
  else if (t==13)
  {
    if (!ev->param || ev->param[0]==0)
      StatusIndication_ShowNotes(EMPTY_TEXTID);
    else
      StatusIndication_ShowNotes(TextID_Create(ev->param,ENC_UCS2,TEXTID_ANY_LEN));
  }
  else if (t==14)
  {
    if (!ev->param || ev->param[0]==0)
      StatusIndication_Item8_SetText(EMPTY_TEXTID);
    else
      StatusIndication_Item8_SetText(TextID_Create(ev->param,ENC_UCS2,TEXTID_ANY_LEN));
  }
  else if (t==15)
  {
    char *book=new char[wstrlen(ev->param)+1];
    wstr2strn(book,ev->param,wstrlen(ev->param));
    BOOK * bk = findbook(book);
    if (bk)
    {
      if (CheckEv(bk,TERMINATE_SESSION_EVENT))
      {
        UI_Event_toBookID(TERMINATE_SESSION_EVENT,BookObj_GetBookID(bk));
        UI_Event_toBookID(RETURN_TO_STANDBY_EVENT,BookObj_GetBookID(bk));
      }
      if (CheckEv(bk,ELF_TERMINATE_EVENT))
      {
        UI_Event_toBookID(ELF_TERMINATE_EVENT,BookObj_GetBookID(bk));
      }
    }
    delete(book);
  }
  else if (t==16)
  {
    GoMusic();
  }
  else if (t==17)
  {
    void DestroyEvents(MyBOOK *);
    DestroyEvents(ECBook);
    wchar_t path[512];
    wchar_t name[512];
    wstrcpy(path,ev->param);
    *(wstrrchr(path,L'/'))=0;
    wstrcpy(name,wstrrchr(ev->param,L'/'));
    readConfig(ECBook, path, name);
  }
  else if (t==18)
  {
    if (ECBook->path && ECBook->name)
      readConfig(ECBook, ECBook->path, ECBook->name);
  }
  else if (t==19)
  {
    int pr=wcstoul(ev->param,0,10);
    if (pr==0)
    {
      REQUEST_PROFILE_GETACTIVEPROFILE(SYNC , &pr);
    }
    else pr--;
    REQUEST_SETTING_SILENCE_SET(SYNC, pr, 1);
  }
  else if (t==20)
  {
    int pr=wcstoul(ev->param,0,10);
    if (pr==0)
    {
      REQUEST_PROFILE_GETACTIVEPROFILE(SYNC , &pr);
    }
    else pr--;
    REQUEST_SETTING_SILENCE_SET(SYNC, pr, 0);
  }
  else if (t==21)
  {
    RedLED_On(0);
  }
  else if (t==22)
  {
    RedLED_Off(0);
  }
  else if (t==23)
  {
    RedLED_On(0);
    delay(100);
    RedLED_Off(0);
  }
  else if (t==24)
  {
    SetLampLevel(1);
    delay(100);
    SetLampLevel(0);
  }
  else if (t==25)
  {
    CSBOOK *bk=(CSBOOK*)FindBook(isCSBook);
    if (bk)bk->changestate(1);
  }
  else if (t==26)
  {
    CSBOOK *bk=(CSBOOK*)FindBook(isCSBook);
    if (bk)bk->changestate(0);
  }
  else if (t==27)
  {
    OrangeLED_Control(1, 0, 0, 100);
    OrangeLED_Control(1, 1, 0, 100);
    OrangeLED_Control(1, 2, 0, 100);
    delay(50);
    OrangeLED_Control(1, 0, 100, 100);
    delay(25);
    OrangeLED_Control(1, 0, 0, 100);
    OrangeLED_Control(1, 1, 100, 100);
    delay(25);
    OrangeLED_Control(1, 1, 0, 100);
    OrangeLED_Control(1, 2, 100, 100);
    delay(25);
    OrangeLED_Control(1, 2, 0, 100);
  }
  else if (t==28)
  {
    OrangeLED_Control(1, 0, 100, 100);
    OrangeLED_Control(1, 1, 100, 100);
    OrangeLED_Control(1, 2, 100, 100);
  }
  else if (t==29)
  {
    OrangeLED_Control(1, 0, 0, 100);
    OrangeLED_Control(1, 1, 0, 100);
    OrangeLED_Control(1, 2, 0, 100);
  }
  else if (t==30)
  {
    REMINDER_STRUCT * rem_str = new REMINDER_STRUCT;
    rem_str->bk=&ECBook->bk;
    rem_str->text=ev->param;
    rem_str->utext=L" ";
    
    rem_str->time=new wchar_t[50];
    snwprintf(rem_str->time, 49, L"%02d:%02d:%02d", h, m, s);
    
    rem_str->vibra=true;
    rem_str->replace=false;
    rem_str->need_to_destroy=true;
    MMIPROC(CreateReminder, rem_str);
  }
  else if (t==31)
  {
    REQUEST_SETTING_RINGVOLUME_SET(SYNC, -3, 0, wcstoul(ev->param,0,10));
  }
  else if (t==32)
  {
    REQUEST_SETTING_VIBRATOR_SET(SYNC, -3, wcstoul(ev->param,0,10));
  }
  else if (t==33)
  {
    wchar_t path[512];
    wchar_t name[512];
    wstrcpy(path,ev->param);
    wchar_t *hz=wstrrchr(path,L'/');
    hz++;
    *hz=0;
    wchar_t *hz2=wstrrchr(ev->param,L'/');
    hz2++;
    wstrcpy(name,hz2);
    REQUEST_SETTING_RINGTYPESOUNDFILE_SET(SYNC, 0, path, name);
  }
  else if (t==34)
  {
    REQUEST_SETTING_ANSWERINGMODE_SET(SYNC, -3, wcstoul(ev->param,0,10));
  }
  else if (t==35)
  {
    REQUEST_SETTING_INCREASINGRING_SET(SYNC, -3, 0, wcstoul(ev->param,0,10));
  }
  else if (t==36)
  {
    REQUEST_SETTING_ALLOWEDCALLERS_SET(SYNC, -3, wcstoul(ev->param,0,10));
  }
  else if (t==37)
  {
    MMIPROC(MakeVC, ev->param);
  }
  else if (t==38)
  {
    BOOK *bk=SESSION_GetTopBook(root_list_get_session(root_list_get_session_count() - 1));
    if (bk)
    {
      wchar_t path[512];
      wchar_t name[512];
      wstrcpy(path,ev->param);
      wchar_t *hz=wstrrchr(path,L'/');
      hz++;
      *hz=0;
      wchar_t *hz2=wstrrchr(ev->param,L'/');
      hz2++;
      wstrcpy(name,hz2);
      SetTheme(path, name, BookObj_GetBookID(bk), 1);
      UI_Event(THEME_CHANGED_EVENT);
    }
  }
};
/*
*===========================================================================
*       Функция запуска событий разных видов
*===========================================================================
*/
void action_processor(EVENT *ev)
{
  if (ev->rem==0x55555555 && ECBook)
  {
    exec_indication(ECBook->rem_types);
  }
  else
  {
    exec_indication(ev->rem);
  }
  MMIPROC(act_proc,ev);
};
/*
inline int detect_missed(EVENT *ev, int now)
{
  if (ev->done && ev->before_done && ev->after_done)return 1;
  return 0;
};
*/
void reminder_sub(EVENT *ev)
{
  if (ev->rem==0x55555555 && ECBook)
  {
    exec_indication(ECBook->rem_types);
  }
  else
  {
    exec_indication(ev->rem);
  }
  wchar_t str[50];
  int min=ev->AskAfter;
  if (min<0)min=-min;
  snwprintf(str, 49, lng[LNG_MIN_AGO_MASK], min);
  wchar_t time[50];
  snwprintf(time, 49, L"%02d:%02d:%02d", h, m, s);
  REMINDER_STRUCT rem_str;
  rem_str.bk=&ECBook->bk;
  if (ev->txt) rem_str.text=ev->txt;
  else rem_str.text=lng[LNG_IMPORTANT_EVENT];
  rem_str.utext=str;
  rem_str.time=time;
  rem_str.vibra=true;
  rem_str.replace=false;
  rem_str.need_to_destroy=false;
  CreateReminder(&rem_str); 
  ev->after_done=true;
};

/*
*===========================================================================
*       Функция определения начала напоминания и вызова события
*===========================================================================
* 1) В случае необходимости напоминает пользователю о грядущем/прошедшем событии
* 2) Запускает выполнение действия
*/
void event_checktime(EVENT *ev, int mode, int rem1, int rem2)
{
  int now=(h*3600) + (m*60) + s;
  bool done=false;
  if (ev->ttype==0)
  {
    if (now==ev->tm_start && mode==0)
    {
      done=true;
      MMIPROC(action_processor,ev);
      ev->done=true;
    }
  }
  else if (ev->ttype==1)
  {
    if (now>=ev->tm_start && now<=ev->tm_end && mode==0)
    {
      done=true;
      MMIPROC(action_processor,ev);
      ev->done=true;
    }
  }
  if (ev->ask_before==now && ((mode==0 && rem1==0 && done==false) || (mode==1 && rem1!=0)))
  {
    wchar_t str[50];
    int min=ev->AskBefore;
    if (min<0)min=-min;
    snwprintf(str, 49, lng[LNG_MIN_REM_MASK],min);
    wchar_t time[50];
    snwprintf(time, 49, L"%02d:%02d:%02d", h, m, s);
    REMINDER_STRUCT * rem_str = new REMINDER_STRUCT;
    rem_str->bk=&ECBook->bk;
    if (ev->txt) rem_str->text=ev->txt;
    else rem_str->text=lng[LNG_IMPORTANT_EVENT];
    rem_str->utext=str;
    rem_str->time=time;
    rem_str->vibra=true;
    rem_str->replace=false;
    rem_str->need_to_destroy=true;
    MMIPROC(CreateReminder,rem_str);
    ev->before_done=true;
  }
  if (ev->ask_after==now && ((mode==0 && rem2==0 && done==false) || (mode==2 && rem2!=0)))
  {
    MMIPROC(reminder_sub, ev);
  }
};

/*===========================================================================
*               Функция сравнения двух разных структур даты.
*===========================================================================
* 1) Если первая дата по календарю идёт позже второй, то возвращается 0.
* 2) Если вторая дата по календарю идёт позже первой, то возвращается 2.
* 3) Если даты равны, то возвращается 1.
*/
int cmpdate(DATE d1, DATE d2)
{
  if      (d1.year<d2.year)return 2;
  else if (d1.year>d2.year)return 0;
  else
  {
    if      (d1.mon<d2.mon) return 2;
    else if (d1.mon>d2.mon) return 0;
    else
    {
      if (d1.day<d2.day)return 2;
      else if (d1.day>d2.day)return 0;
      else return 1;
    }
  }
};

/*
*===========================================================================
*            Функция сравнения даты из структуры с текущей
*===========================================================================
* 1) Если текущая дата совпадает/попадает во временной интервал с датой из 
*    структуры события, то запускает проверку времени
* 2) Не совпадает/не попадает - выход
*/
void event_checkdate(EVENT *ev)
{
  if (ev->dtype==0)
  {
    if (ev->days[DayOfTheWeek])
    {
      event_checktime(ev, 0, ev->d_shift_0, ev->d_shift_1);
    }
    if (ev->remdays[DayOfTheWeek])
    {
      event_checktime(ev, 1, ev->d_shift_0, ev->d_shift_1);
    }
    if (ev->remdays2[DayOfTheWeek])
    {
      event_checktime(ev, 2, ev->d_shift_0, ev->d_shift_1);
    }
  }
  else if (ev->dtype==1)
  {
    if(cmpdate(ev->date, dt.date)>0 && cmpdate(ev->date2, dt.date)<2)
    {
      event_checktime(ev, 0, ev->d_shift_0, ev->d_shift_1);
    }
    else if (cmpdate(ev->date_0, dt.date)==1)
    {
      event_checktime(ev, 1, ev->d_shift_0, ev->d_shift_1);
    }
    else if (cmpdate(ev->date_1, dt.date)==1)
    {
      event_checktime(ev, 2, ev->d_shift_0, ev->d_shift_1);
    }
  }
};
