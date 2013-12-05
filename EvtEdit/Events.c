#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\externals.h"
#include "header\string.h"
#include "header\lng.h"

extern MyBOOK *myBook;
extern wchar_t *defPath;
extern wchar_t *defName;

LIST *events=0;

wchar_t *extFolder=0;
wchar_t *intFolder=0;

void InitializeFolders();

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

int cmp(char *param1,char *param2)
{
  if (strcmp(param1,param2)==0)
  {
    return 1;
  }
  return 0;
};

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
    char *par=getchr(param,&x,':');
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

void SetTime(char *param, EVENT *ev, int type)
{
  if (type==0) //Указано время без конечного
  {
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
  else if (type==1) //Указана даты начала и конца
  {
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

void AddEvent2(char *line)
{
  EVENT *ev=new EVENT;
  memset(ev,0,sizeof(EVENT));
  int x=0;
  ev->rem=getint(line,&x,':');
  ev->AskBefore=getint(line,&x,':');
  if (ev->AskBefore<0)
  {
    ev->isMinusInBefore=true;
    ev->AskBefore=-ev->AskBefore;
  }
  ev->AskAfter=getint(line,&x,':');
  if (ev->AskAfter<0)
  {
    ev->isMinusInAfter=true;
    ev->AskAfter=-ev->AskAfter;
  }
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
  ev->txt=getwchr(line,&x,0xFE);
  ev->param=getwchr(line,&x,0xFF);
  List_InsertLast(events, ev);
}
void *memmem(const unsigned char *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
  const unsigned char *cursor;
  const unsigned char *last_possible_needle_location = (unsigned char *)haystack + haystacklen - needlelen;
  if (needlelen > haystacklen) return(NULL);
  if (needle == NULL) return(NULL);
  if (haystack == NULL) return(NULL);
  if (needlelen == 0) return(NULL);
  if (haystacklen == 0) return(NULL);
  
  for (cursor = haystack; cursor <= last_possible_needle_location; cursor++) 
  {
    if (memcmp(needle, cursor, needlelen) == 0) 
    {
      return((void *) cursor);
    }
  }
  return(NULL);
};

char *getparam(char *buf, char *mask, char end, int fsize)
{
  char *str=0;
  if (str=(char*)memmem((unsigned char const*)buf,fsize,mask,strlen(mask)))
  {
    char *ret=0;;
    int addr=(int)str;
    str=(char*)(addr+strlen(mask)+2);
    int x=0;
    int len=0;
    while (str[x]!=end)
    {
      x++;
    }
    len=x+1;
    if (len>1)
    {
      ret=new char[len+1];
      memcpy(ret,str,len);
      ret[len]='\0';
      return ret;
    }
    return 0;
  }
  return 0;
};

int readConfig(BOOK *myBook, wchar_t *myFolder, wchar_t *name)
{
  char endline=0xFF;
  int result=-1; //файл не найден
  int file;
  FSTAT fst;
  if (fstat(myFolder,name,&fst)==0)
  {
    if ((file=_fopen(myFolder,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
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
          AddEvent2(param);
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
        MyBOOK *mbk=(MyBOOK*)myBook;
        int x2=0;
        mbk->rem_types=getint(param,&x2,0xFF);
        mfree(param);
      }
      MyBOOK *mbk=(MyBOOK*)myBook;
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
      if (defPath)delete(defPath);
      if (defName)delete(defName);
      defPath=new wchar_t[wstrlen(myFolder)+1];
      wstrcpy(defPath,myFolder);
      defName=new wchar_t[wstrlen(name)+1];
      wstrcpy(defName,name);
      delete(buf);
      result=0; //Успешно прочитали, занесли данные в лист
    }
    fclose(file);
  }
  return result;
};

int InitializeEvents(BOOK *myBook, wchar_t *path, wchar_t *fname)
{
  events=List_Create();
  if (path && fname)
  {
    readConfig(myBook, path, fname);
  }
  else
  {
    if (readConfig(myBook, extFolder, L"Events.evt")<0)
    {
      if (readConfig(myBook, intFolder, L"Events.evt")<0)
      {
        return 0;
      }
    }
  }
  return 1;
};

char* getevents_days(EVENT *ev)
{
  char *ret=new char[10];
  ret[0]='\0';
  int x;
  for (x=0;x<10;x++)
  {
    if (ev->days[x])
    {
      char hz[2];
      hz[0]=x+0x30;
      hz[1]='\0';
      strcat(ret,hz);
    }
  }
  return ret;
};

void saveevents(wchar_t *folder, wchar_t *name)
{
    int file;
    FSTAT fst;
    //перевод на новую строку:
    char cr=0x0D;
    char lf=0x0A;
    char endline=0xFF;
    char endstr=0xFE;
    char idle2=':';
    if ((file=_fopen(folder,name,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      if (fstat(folder,name,&fst)==0)
      {
        int x=0;
        for (x=0;x<events->FirstFree;x++)
        {
          //Запишем в файл объекты в юникодной кодировке
          EVENT *ev=(EVENT*)List_Get(events,x);
          char pattern[1024];
          if (ev->isMinusInBefore==true)
          {
            ev->AskBefore=-ev->AskBefore;
          }
          if (ev->isMinusInAfter==true)
          {
            ev->AskAfter=-ev->AskAfter;
          }
          if (ev->isGps)
          {
            ev->dtype=2;
          }
          sprintf(pattern, "%d): %d:%d:%d:%d:%d:", x, ev->rem, ev->AskBefore,ev->AskAfter,ev->type,ev->dtype);
          fwrite(file,pattern,strlen(pattern));
          if (ev->dtype==0)
          {
            char *t=getevents_days(ev);
            fwrite(file,t,strlen(t));
            fwrite(file,&idle2,1);
            delete(t);
          }
          else if (ev->dtype==1)
          {
            sprintf(pattern,"%02d-%02d-%02d.%02d-%02d-%02d:", ev->date.day, ev->date.mon,ev->date.year, ev->date2.day, ev->date2.mon, ev->date2.year);
            fwrite(file,pattern,strlen(pattern));
          }
          else
          {
            char *t=getevents_days(ev);
            sprintf(pattern,"%s.%s:", ev->gps,t);
            delete(t);
            fwrite(file,pattern,strlen(pattern));
          }
          sprintf(pattern,"%d:",ev->ttype);
          fwrite(file,pattern,strlen(pattern));
          if (ev->ttype==0)
          {
            sprintf(pattern,"%02d-%02d-%02d:", ev->time.hour, ev->time.min, ev->time.sec);
            fwrite(file,pattern,strlen(pattern));
          }
          else
          {
            sprintf(pattern,"%02d-%02d-%02d.%02d-%02d-%02d:", ev->time.hour, ev->time.min, ev->time.sec, ev->time2.hour, ev->time2.min, ev->time2.sec);
            fwrite(file,pattern,strlen(pattern));
          }
          if (ev->txt)
          {
            fwrite(file,ev->txt,wstrlen(ev->txt)*2);
          }
          fwrite(file,&endstr,1);
          if (ev->param)
          {
            fwrite(file,ev->param,wstrlen(ev->param)*2);
          }
          fwrite(file,&endline,1);
          fwrite(file,&cr,1);
          fwrite(file,&lf,1);
        }
        fwrite(file,&cr,1);
        fwrite(file,&lf,1);
        char str[50];
        sprintf(str,"ind_item): %d",myBook->rem_types);
        fwrite(file,str,strlen(str));
        fwrite(file,&endline,1);
        fwrite(file,&cr,1);
        fwrite(file,&lf,1);
        if (myBook->indpath)
        {
          if (myBook->indpath[0])
          {
            fwrite(file,"ind_path): ",strlen("ind_path): "));
            fwrite(file,myBook->indpath,wstrlen(myBook->indpath)*2);
            fwrite(file,&endline,1);
            fwrite(file,&cr,1);
            fwrite(file,&lf,1);
          }
        }
        if (myBook->indname)
        {
          if (myBook->indname[0])
          {
            fwrite(file,"ind_name): ",strlen("ind_name): "));
            fwrite(file,myBook->indname,wstrlen(myBook->indname)*2);
            fwrite(file,&endline,1);
            fwrite(file,&cr,1);
            fwrite(file,&lf,1);
          }
        }
      }
    }
    fclose(file);
};

GUI_LIST *g=0;
int isEvtControl(BOOK *bk);
typedef void (*UPDATE_METHOD)(wchar_t *path, wchar_t *name);

typedef struct
{
  BOOK bk;
  UPDATE_METHOD update;
}EVTBOOK;

void Saver_OnSelect(BOOK*bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)bk;
  int item=ListMenu_GetSelectedItem(g);
  if (item==0)
    saveevents(extFolder, L"Events.evt");
  else
    saveevents(intFolder, L"Events.evt");
  EVTBOOK *ek=(EVTBOOK*)FindBook(isEvtControl);
  if (ek)
  {
    ek->update(defPath,defName);
  }
  FREE_GUI(mbk->lst);
  if (events)
  {
    while (events->FirstFree)
    {
      EVENT* event=(EVENT*)List_RemoveAt(events,0);
      if (event)
      {
        DELETE(event->txt);
        DELETE(event->param);
        DELETE(event);
        delete(event);
      }
    }
    List_Destroy(events);
    events=0;
  }
  FreeBook(bk);
};
void Saver_OnBack(BOOK *bk, GUI* )
{
  GUIObject_Destroy(g);
};
//wchar_t *str[]={L"На карту",L"На телефон"};
int Saver_onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    EVENT* evt=(EVENT *)List_Get(events,item);
    GUIonMessage_SetMenuItemText(msg,TextID_Create(lng[LNG_ONCARD+item],ENC_UCS2,TEXTID_ANY_LEN));
    break;
  }
  return(1);
};

GUI_LIST * CreateSaverList(BOOK * book)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    ListMenu_SetItemCount(lo,2);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,Saver_onLBMessage);
    ListMenu_SetItemStyle(lo,0);
    GUIObject_SetStyle(lo,9);
    GUIObject_SetTitleText(lo,TextID_Create(lng[LNG_SAVE],ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, Saver_OnBack);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,Saver_OnSelect);
  }
  return(lo);
};

GUI_LIST *create_saver(BOOK *book)
{
  g=(GUI_LIST*)CreateSaverList(book);
  GUIObject_Show(g);
  return(g);
};


int DestroyEvents(MyBOOK *mbk, bool save)
{  
  if (events)
  {
    if (save)
    {
      if (defPath && defName)
      {
        saveevents(defPath, defName);
      }
      else
      {
        create_saver(&mbk->book);
        return 1;
      }
    }
    while (events->FirstFree)
    {
      EVENT* event=(EVENT*)List_RemoveAt(events,0);
      if (event)
      {
        DELETE(event->txt);
        DELETE(event->param);
        DELETE(event);
        delete(event);
      }
    }
    List_Destroy(events);
    events=0;
  }
  return 0;
};
