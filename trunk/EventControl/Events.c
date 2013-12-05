/*
*===========================================================================
*                 Модуль создания/уничтожения списка событий
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\string.h"

extern MyBOOK *ECBook;

void SortList(LIST *lst);
void InitializeFolders();
void date_increase(DATE date, int num, char *day, char *mon, u16 *year);

LIST *events;
wchar_t *extFolder=0;
wchar_t *intFolder=0;

/*
*==============================================================================
* Заполнение глобальных переменных extFolder и intFolder путями к папкам эльфа
*==============================================================================
*/
void InitializeFolders()
{
  if (extFolder==0)
  {
    int size=wstrlen(GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG))+wstrlen(L"/EventControl/");
    extFolder = new wchar_t[size+1];
    snwprintf(extFolder,size,L"%ls/EventControl/",GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG));
  }
  if (intFolder==0)
  {
    int size=wstrlen(GetDir(MEM_INTERNAL+DIR_ELFS_CONFIG))+wstrlen(L"/EventControl/");
    intFolder = new wchar_t[size+1];
    snwprintf(intFolder,size,L"%ls/EventControl/",GetDir(MEM_INTERNAL+DIR_ELFS_CONFIG));
  }
};

/*
*===========================================================================
*             Функция парсинга строки, содержащей дату/дни/GPS
*===========================================================================
*/
void SetDate(char *param, EVENT *ev, int type)
{
  if (type==0) //Указаны дни
  {
    int x=0;
    for (x=0;x<strlen(param);x++)
    {
      ev->days[param[x]-0x30]=true;
    }
  }
  else if (type==1) //Указана даты начала и конца
  {
    int x=0;
    while (param[x]!='-')
    {
      ev->date.day=(ev->date.day*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-')
    {
      ev->date.mon=(ev->date.mon*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='.')
    {
      ev->date.year=(ev->date.year*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-')
    {
      ev->date2.day=(ev->date2.day*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-')
    {
      ev->date2.mon=(ev->date2.mon*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!=':' && param[x]!=0)
    {
      ev->date2.year=(ev->date2.year*10)+(param[x]-0x30);
      x++;
    }
    x++;
  }
  else   
  {
    int x=0;
    ev->gps=getchr(param,&x,'.');
    char *par=getchr(param,&x,0);
    if (par)
    {
      for (x=0;x<strlen(par);x++)
      {
        ev->days[par[x]-0x30]=true; 
      }
      delete(par);
    }
    ev->isGps=true;
  }
};
/*
*===========================================================================
*                 Функция парсинга строки, содержащей время
*===========================================================================
*/

void SetTime(char *param, EVENT *ev, int type)
{
  if (type==0) //Указано время без конечного
  {
    memset(&ev->time,0,sizeof(TIME));
    memset(&ev->time2,0,sizeof(TIME));
    int x=0;
    while (param[x]!='-')
    {
      ev->time.hour=(ev->time.hour*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-' && param[x]!=0)
    {
      ev->time.min=(ev->time.min*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!=':' && param[x]!=0)
    {
      ev->time.sec=(ev->time.sec*10)+(param[x]-0x30);
      x++;
    }
  }
  else if (type==1) //Указаны даты начала и конца
  {
    memset(&ev->time,0,sizeof(TIME));
    memset(&ev->time2,0,sizeof(TIME));
    int x=0;
    while (param[x]!='-')
    {
      ev->time.hour=(ev->time.hour*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-')
    {
      ev->time.min=(ev->time.min*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='.')
    {
      ev->time.sec=(ev->time.sec*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-')
    {
      ev->time2.hour=(ev->time2.hour*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!='-' && param[x]!=0)
    {
      ev->time2.min=(ev->time2.min*10)+(param[x]-0x30);
      x++;
    }
    x++;
    while (param[x]!=':' && param[x]!=0)
    {
      ev->time2.sec=(ev->time2.sec*10)+(param[x]-0x30);
      x++;
    }
  }
};

/*
*===========================================================================
*                      Вычисление дня недели
*===========================================================================
* 1) cur - "базовый" день, ask - на сколько надо сместить день
*/
int calcday(int cur, int ask)
{
  int ret=cur+ask;
  if (ret>6)
  {
    ret=ret%7;
  }
  else if (ret<0)
  {
    while (ret<0)ret=7+ret;
  }
  return ret;
};

/*
*===========================================================================
*   Функция расширения списка дней, при которых выполняется событие
*===========================================================================
* 1) *d - список rem_days, days - указатель на текущие дни, 
* num - на сколько необходимо сместить дни
*/

void shiftdays(int *d, int *days, int num)
{
  for (int x=0;x<7;x++)
  {
    if (days[x])
    {
      d[calcday(x, num)]=1;
    }
  }
};

void shiftdate(DATE date, DATE *dt, int num)
{
  date_increase(date, num, &dt->day, &dt->mon, &dt->year);
};
/*
*===========================================================================
*      Функция парсинга строки, создание EVENT*, занесения его в лист
*===========================================================================
*/
void AddEvent(char *line)
{
  char endstr=0xFE;
  char endline=0xFF;
  EVENT *ev=new EVENT;
  memset(ev,0,sizeof(EVENT));
  int x=0;
  ev->rem=getint(line,&x,':');
  ev->AskBefore=getint(line,&x,':');
  ev->AskAfter=getint(line,&x,':');
  ev->type=getint(line,&x,':');
  ev->dtype=getint(line,&x,':');
  char *date=getchr(line,&x,':');
  if (date)
  {
    SetDate(date,ev,ev->dtype);
    delete(date);
  }
  ev->ttype=getint(line,&x,':');
  char *time=getchr(line,&x,':');
  if (time)
  {
    SetTime(time,ev,ev->ttype);
    delete(time);
  }
  ev->txt=getwchr(line,&x,endstr);
  ev->param=getwchr(line,&x,endline);
  ev->tm_start=(ev->time.hour*3600) + (ev->time.min * 60) + ev->time.sec;
  ev->ask_before=ev->tm_start+(ev->AskBefore*60);
  if (ev->ttype==1)
  {
    ev->tm_end=(ev->time2.hour*3600) + (ev->time2.min * 60) + ev->time2.sec;
  }
  else
  {
    ev->tm_end=(ev->time.hour*3600) + (ev->time.min *60) + ev->time.sec;
  }
  ev->ask_after=ev->tm_end+(ev->AskAfter*60);
  if (!ev->ask_after)ev->after_done=true;
  ev->d_shift_0=0;
  ev->d_shift_1=0;
  if (ev->dtype==0)
  {
    shiftdays(ev->days, ev->days, 0);
    if (ev->ask_before>=86400)
    {
      shiftdays(ev->remdays, ev->days, 1);
      ev->d_shift_0=1;
      ev->ask_before=ev->ask_before-86400;
    }
    if (ev->ask_before<0)
    {
      shiftdays(ev->remdays, ev->days, -1);
      ev->d_shift_0=-1;
      ev->ask_before=ev->ask_before+86400;
    }
    if (ev->ask_after>=86400)
    {
      shiftdays(ev->remdays2, ev->days, 1);
      ev->d_shift_1=1;
      ev->ask_after=ev->ask_after-86400;
    }
    if (ev->ask_after<0)
    {
      shiftdays(ev->remdays2, ev->days, -1);
      ev->d_shift_1=-1;
      ev->ask_after=ev->ask_after+86400;
    }
  }
  else if (ev->dtype==1)
  {
    if (ev->ask_before>86400)
    {
      shiftdate(ev->date, &ev->date_0, 1);
      ev->d_shift_0=1;
      ev->ask_before=ev->ask_before-86400;
    }
    if (ev->ask_before<0)
    {
      shiftdate(ev->date, &ev->date_0, -1);
      ev->d_shift_0=-1;
      ev->ask_before=ev->ask_before+86400;
    }
    if (ev->ask_after>86400)
    {
      shiftdate(ev->date, &ev->date_1, 1);
      ev->d_shift_1=1;
      ev->ask_after=ev->ask_after-86400;
    }
    if (ev->ask_after<0)
    {
      shiftdate(ev->date, &ev->date_1, -1);
      ev->d_shift_1=-1;
      ev->ask_after=ev->ask_after+86400;
    }
  }
  if (!events)
  {
    events=List_Create();
    ECBook->lst=events;
  }
  List_InsertLast(events, ev);
};

/*
*===========================================================================
*   Функция чтение конфиг-файла, занесения событий в лист
*===========================================================================
*/

int readConfig(MyBOOK *mbk, wchar_t *myFolder, wchar_t *fname)
{
  char endline=0xFF;
  int result=-1; //файл не найден
  int file;
  FSTAT fst;
  if (fstat(myFolder,fname,&fst)==0)
  {
    if ((file=_fopen(myFolder,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      char * buf;
      char * param;
      
      buf=new char[fst.fsize+1];
      fread(file,buf,fst.fsize);
      int x=0;
      while (1)
      {
        char pattern[128];
        sprintf(pattern,"%d)",x);
        if (param=getparam(buf,pattern,endline, fst.fsize))
        {
          AddEvent(param);
          mfree(param);
        }
        else
        {
          break;
        }
        param=0;
        x++;
      }
      if (param=getparam(buf,"ind_item)", endline, fst.fsize))
      {
        int x2=0;
        mbk->rem_types=getint(param,&x2,0xFF);
        mfree(param);
      }
      if (mbk->indpath)
        delete(mbk->indpath);
      if (param=getparam(buf,"ind_path)", endline, fst.fsize))
      {
        int x2=0;
        mbk->indpath=getwchr(param,&x2,0xFF);
        mfree(param);
      }
      if (mbk->indname)
        delete(mbk->indname);
      if (param=getparam(buf,"ind_name)", endline, fst.fsize))
      {
        int x2=0;
        mbk->indname=getwchr(param,&x2,0xFF);
        mfree(param);
      }
      delete(buf);
      result=0;
      if (mbk->path)delete(mbk->path);
      mbk->path=new wchar_t[wstrlen(myFolder)+1];
      wstrcpy(mbk->path,myFolder);
      if (mbk->name)delete(mbk->name);
      mbk->name=new wchar_t[wstrlen(fname)+1];
      wstrcpy(mbk->name,fname);

      if (events->FirstFree>1)
      {
        SortList(events);
      }
      fclose(file);
    }
    else
    {
      result=-2;
    }
  }
  return result;
};

/*
*===========================================================================
*                       Чтение дефолтных событий
*===========================================================================
* 1) Возвращает 1 в случае удачного прочтения конфиг-файла, иначе - 0
* 2) Порядок проверки: карта памяти/Config/EventControl/, память телефона/Config/EventControl
*/
int InitializeEvents(MyBOOK *myBook)
{
  events=List_Create();
  myBook->lst=events;
  if (readConfig(myBook, extFolder, L"Events.evt")<0)
  {
    if (readConfig(myBook, intFolder, L"Events.evt")<0)
    {
      return 0;
    }
  }
  return 1;
};

/*
*===========================================================================
*               Функция уничтожения списка событий
*===========================================================================
*/
void DestroyEvents(MyBOOK *myBook)
{
  if (!events)
  {
    myBook->lst=0;
    return;
  }
  while (events->FirstFree)
  {
    EVENT* event=(EVENT*)List_RemoveAt(events,0);
    if (event)
    {
      if (event->param)delete(event->param);
      if (event->gps)delete(event->gps);
      if (event->txt)delete(event->txt);
      delete(event);
    }
  }
  List_Destroy(events);
  events=0;
  myBook->lst=0;
};

