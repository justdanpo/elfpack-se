#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\lng.h"
#include "header\process.h"

extern int fsize;
extern wchar_t *extFolder;
extern wchar_t *intFolder;
extern MyBOOK *ECBook;
extern LIST *events;

void action_processor(EVENT *ev);
void event_checkdate(EVENT *ev);

int lng_load(wchar_t *path, wchar_t *name);
void destroy_innative_lng();

void InitializeFolders();
int InitializeEvents(MyBOOK *myBook);

MyBOOK * EC_Create();
void ECBook_OnClose(BOOK * book);
void elf_exit(void);

GUI_LIST *create_ed(BOOK *book);
void mbox_Create(BOOK * bk, wchar_t *text, int min, bool vibra);
void CreateReminder(BOOK * bk, wchar_t *text, wchar_t *utext, wchar_t *time, bool vibra, bool);

PROCESS proc_=0;

wchar_t oldcell[10]=L"old";
wchar_t ccell[10];

DATETIME dt;
wchar_t year;
char month, day, DayOfTheWeek, h, m, s, isWeekend;

u16 EventTimer;

void SUBPROC_(void(*PROC)(int,void*),int p1 , void * p2);
#pragma swi_number=0x109
__swi __arm  void MMIPROC (void(*PROC)(EVENT *), EVENT *);

/*
*===========================================================================
*                 Функция обновления текущего времени
*===========================================================================
* 1) Получение времени и запись в глобальные переменные
* 2) Определение дня недели и запись в глобальные переменные
*/
void DateAndTime_Update(int fullinit)
{
  REQUEST_DATEANDTIME_GET(0,&dt);
  h=dt.time.hour;
  m=dt.time.min;
  s=dt.time.sec;
  if (fullinit)
  {
    year=dt.date.year;
    month=dt.date.mon;
    day=dt.date.day;
    int a;
    int b;
    int c;
    a=(14-month)/12;
    b=year-a;
    c=month+12*a-2;
    DayOfTheWeek=(day+b+(b/4)-(b/100)+(b/400)+(31*c/12))%7;
    if (DayOfTheWeek>0)
      DayOfTheWeek--;
    else if (DayOfTheWeek==0)
      DayOfTheWeek=6;
    if (DayOfTheWeek==5||DayOfTheWeek==6)
      isWeekend=true;
    else 
      isWeekend=false;
  }
};

/*
*===========================================================================
*               Функция определения текущих событий (по таймеру)
*===========================================================================
* 1) Определение GPS-координат и подборка соответствующих событий
* 2) Вызов выполнения событий по времени
*/ 
void sub_launcher(int need_to_watch, void *)
{
  if (!events) return;
  if (events->FirstFree)
  {
    int x=0;
    for (x=0;x<events->FirstFree;x++)
    {
      if (EVENT *ev=(EVENT*)List_Get(events,x))
      {
        if (ev->dtype==2 && need_to_watch && ev->days[DayOfTheWeek])
        {
          wchar_t str[10];
          str2wstr(str,ev->gps);
          if (wstrcmp(ccell,str)==0)
            MMIPROC(action_processor, ev);
          need_to_watch=false;
        }
        else
        {
          if (ev->done==false || ev->after_done==false || ev->before_done==false)
            event_checkdate(ev);
        }
      }
    }
  }
};


void onEventTimer(u16 timerID, LPARAM lparam)
{
  if (!events)goto loc_ret;
  if (m==0 && s==0)
    DateAndTime_Update(1);
  else 
    DateAndTime_Update(0);
  
  bool need_to_watch=false;
  PLMN_LAC_DESC plmn_lac;
  RAT_CI_DESC rat_ci;
  char CSReg;
  if (get_CellData(&plmn_lac,&rat_ci,&CSReg))
  {
    snwprintf(ccell,9,L"%04X-%04X", plmn_lac.LAC[0]<<8|plmn_lac.LAC[1], rat_ci.CI);
    if (wstrcmp(oldcell,ccell))
    {
      need_to_watch=true;
      wstrcpy(oldcell,ccell);
    }
  }
  SUBPROC_(sub_launcher,need_to_watch,0);
loc_ret:
  Timer_ReSet(&EventTimer, 1000, onEventTimer, 0);
};

int isEVBookByName(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"EventControl")==0)return 1;
  return 0;
};


inline void getfilesize(wchar_t *elfpath)
{
  wchar_t path[512];
  wchar_t name[512];
  wstrcpy(path,elfpath);
  *(wstrrchr(path,L'/'))=0;
  wstrcpy(name,wstrrchr(elfpath,L'/'));
  wchar_t *name2=name;
  name2++;
  FSTAT fst;
  if (fstat(path,name2,&fst)==0)
  {
    fsize=fst.fsize;
  }
};

/*
*===========================================================================
*            Функция запуска эльфа.
*===========================================================================
* 1) Создание книги
* 2) Чтение файла событий
* 3) Первичное полное обновление времени
* 4) Создание процесса и таймера
*/
int main(wchar_t *elfpath)
{
  trace_init();
  getfilesize(elfpath);
  InitializeFolders();
  wchar_t str[50];
  char *st=lang_get_name(get_system_langID());
  str2wstr(str,st);
  wstrcat(str,L".cfg");
  if (lng_load(extFolder, str))
  {
    if (lng_load(intFolder, str))
    {
      if (strcmp(st,"ru"))
      {
        if (lng_load(extFolder, L"en.cfg"))
        {
          lng_load(intFolder, L"en.cfg");
        }
      }
    }
  }
  if (FindBook(isEVBookByName))
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(lng[LNG_ALREADY_RUNNED],ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 0, 0);
    delete(extFolder);
    delete(intFolder);
    destroy_innative_lng();
    SUBPROC(elf_exit);
    return 0;
  }
  MyBOOK *bk=EC_Create();
  if (bk)
  {
    bk->indpath=0;
    bk->indname=0;
    int getfw();
    getfw();
    if (InitializeEvents(bk)==0)
      mbox_Create(&bk->bk, lng[LNG_NOFILE], 0, false);
    DateAndTime_Update(1);
    proc_ = create_process(OS_PRI_PROC, "EvtCtrl_wrk", worker_entrypoint, 32768, 10, (OSTIME) 0, (PROCESS) 0, NULL, (OSVECTOR) 0, (OSUSER) 0);
    start(proc_);
    EventTimer = Timer_Set(1000, onEventTimer, 0);
    onEventTimer(EventTimer, 0);
  }
  else
  {
    delete(extFolder);
    delete(intFolder);
    destroy_innative_lng();
    SUBPROC(elf_exit);
    return 0;
  }
  return 0;
};
