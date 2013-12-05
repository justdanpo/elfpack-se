#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"


typedef struct
{
  char CallState;
  char CallID;
  int * PNUM;
}CALLMANAGER_DATA;
  

typedef struct
{
  char pos1;
  char cale_base;
  short pos3;
}CALE_SET;

typedef struct
{
  char dummy[0x2C];
  int unkID;
}ses_est;

typedef struct
{
  char dummy[0x34];
  int unkID;
}ses_term;

typedef struct
{
  wchar_t * w_PNUM;
  wchar_t * Name;
  int first_callstate;
  int last_callstate;
  DATETIME * startdatetime;
  DATETIME * begindatetime;
  char line;
}myList_elem;

typedef struct
{
  int ID;
  DATETIME * SesEst;
}myList_gprs_elem;

typedef struct
{
  BOOK * book;
}MSG;

BOOK * CLBook;
LIST * myList;
LIST * myList_gprs;
DATETIME * cur_date;

int incomleted_connection=0;
wchar_t * Money=0;

//Название говорящее...
void win12512unicode(wchar_t *ws, char *s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    *ws++=c;
  }
  *ws=0;
}

// берем значения из ини-файла
void get_bcfg_key(void)
{
  InitConfig();
  if (Money) delete(Money);
  Money=new wchar_t[strlen((char*)strMoney)+1];
  win12512unicode(Money,(char*)strMoney,strlen((char*)strMoney));

}



//Функция поиска элемента в листе звонков
int myList_Find(void *r0,void *r1)
{
  if (((myList_elem*)r0)->line==((myList_elem*)r1)->line) return(0);
  return(1);
}

//Функция поиска элемента в листе жпрс
int myList_gprs_Find(void *r0,void *r1)
{
  if (((myList_gprs_elem*)r0)->ID==(int)r1) return(0);
  return(1);
}

//Убийство моего элемента
void myList_elem_Free(void * r)
{
  myList_elem * r0=(myList_elem *)r;
  if (r0)
  {
    if (r0->w_PNUM)
    {
      delete(r0->w_PNUM);
    }
    if (r0->Name)
    {
      delete(r0->Name);
    }
    if (r0->startdatetime)
    {
      delete(r0->startdatetime);
    }
    if (r0->begindatetime)
    {
      delete(r0->begindatetime);
    }    
    delete(r0);
  }
}


//Убийство моего элемента жпрс
void myList_gprs_elem_Free(void * r)
{
  myList_gprs_elem * r0=(myList_gprs_elem *)r;
  delete(r0->SesEst);
  delete(r0);
}

//Фильтр для убийства элементов при убийстве листа (всех ненулевых)
int myList_elem_Filter(void * r0)
{
  if (r0) return(1);
  return(0);
}

//Убийство эльфа
int TerminateElf(void * ,BOOK * book)
{
  FreeBook(book);
  return(1);
}

//Показать информацию об авторе
int ShowAuthorInfo(void *mess ,BOOK * book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("Call Logger, v2.3.2\n\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


void sum_data(wchar_t * buf,wchar_t * substring,int * res_whole,int * res_fract)
{
  wchar_t * pos=buf;
  wchar_t temp_buf[50];
  wchar_t separator=' ';
  int fract_mode=0;
  int sum_whole=0;
  int sum_fract=0;
  int hex;
  int len;
  if (res_fract)
  {
    fract_mode=1;
    separator='.';
  }
  while (pos=wstrwstr(pos,substring))
  {
    pos=pos+wstrlen(substring);
    len=(wstrchr(pos,separator)-pos);
    wstrncpy(temp_buf,pos,len);
    wtoi(temp_buf,len,&hex);
    sum_whole=sum_whole+hex;
    hex=0;
    if (fract_mode)
    {
      pos=wstrchr(pos,separator)+1;
      wstrncpy(temp_buf,pos,2);
      wtoi(temp_buf,2,&hex);
      sum_fract=sum_fract+hex;
      hex=0;
    }
  }
  if (fract_mode)
  {
    sum_whole=sum_whole+sum_fract/100;
    sum_fract=sum_fract%100;
    *res_fract=sum_fract;
  }
  *res_whole=sum_whole;
}


void sum_traf_cost(DATETIME * cur_date)
{
  FSTAT _fstat;
  wchar_t fpath[200];
  wchar_t folder[20];
  wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
  wstrcat(fpath,L"/Call Logger/");
  snwprintf(folder,20,L"%04d-%02d-%02d",cur_date->date.year,cur_date->date.mon,cur_date->date.day);
  wstrcat(fpath,folder);
  if (fstat(fpath,L"gprs.txt",&_fstat)>=0)
  {
    char * buffer_gprs=new char[_fstat.fsize+2];
    wchar_t * w_buffer_gprs=(wchar_t*)buffer_gprs;
    memset(buffer_gprs,0,_fstat.fsize+2);
    int f=_fopen(fpath,L"gprs.txt",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
    fread(f,buffer_gprs,_fstat.fsize);
    fclose(f);
    wchar_t buffer_str[200];
    if (!wstrwstr((wchar_t*)buffer_gprs,L"Total cost"))
    {
      int rub=0;
      int kop=0;
      sum_data(w_buffer_gprs,L"cost - ",&rub,&kop);
      
      if (TrafDiv)
      {
        int traf_whole_sent=0;
        int traf_fract_sent=0;
        int traf_whole_rec=0;
        int traf_fract_rec=0;          
        if (TrafKb)
        {
          sum_data(w_buffer_gprs,L"sent - ",&traf_whole_sent,&traf_fract_sent);
          sum_data(w_buffer_gprs,L"received - ",&traf_whole_rec,&traf_fract_rec);

          snwprintf(buffer_str,400,L"\r\nTotal cost - %d.%02d %ls\r\nTotal sent - %d.%02d Kb\r\nTotal received - %d.%02d Kb\r\n",rub,kop,Money,traf_whole_sent,traf_fract_sent,traf_whole_rec,traf_fract_rec);
        }
        else
        {
          sum_data(w_buffer_gprs,L"sent - ",&traf_whole_sent,0);
          sum_data(w_buffer_gprs,L"received - ",&traf_whole_rec,0);

          snwprintf(buffer_str,400,L"\r\nTotal cost - %d.%02d %ls\r\nTotal sent - %d b\r\nTotal received - %d b\r\n",rub,kop,Money,traf_whole_sent,traf_whole_rec);
        }
      }
      else
      {
        int traf_whole=0;
        int traf_fract=0;
        if (TrafKb)
        {
          sum_data(w_buffer_gprs,L"traffic - ",&traf_whole,&traf_fract);

          snwprintf(buffer_str,400,L"\r\nTotal cost - %d.%02d %ls\r\nTotal traffic - %d.%02d Kb\r\n",rub,kop,Money,traf_whole,traf_fract);
        }
        else
        {
          sum_data(w_buffer_gprs,L"traffic - ",&traf_whole,0);

          snwprintf(buffer_str,400,L"\r\nTotal cost - %d.%02d %ls\r\nTotal traffic - %d b\r\n",rub,kop,Money,traf_whole);
        }
      }
      f=_fopen(fpath,L"gprs.txt",FSX_O_CREAT|FSX_O_APPEND,FSX_S_IREAD|FSX_S_IWRITE,0);
      fwrite(f,buffer_str,wstrlen(buffer_str)*2);
      fclose(f);
    }
    delete(buffer_gprs);
  }      
}


int onClockChange(void * r0, BOOK *)
{
  DATETIME datetime;
  REQUEST_DATEANDTIME_GET(SYNC,&datetime);
  if (cur_date->date.day!=datetime.date.day)
  {
    if (!incomleted_connection)
    {
      if (ConnectionManager_Connection_GetState())
      {
        incomleted_connection=1;
      }
      else
      {
        sum_traf_cost(cur_date);
      }
    }
    memcpy(cur_date,&datetime,sizeof(DATETIME));
  }
  return(0);
}

//Ловим создание содинения
int onSessionEstablished(void * r0, BOOK *)
{
  if (r0)
  {
    if (ListElement_Find(myList_gprs,(void*)((ses_est*)r0)->unkID,myList_gprs_Find)==LIST_ERROR)
    {
      myList_gprs_elem * elem_gprs=new(myList_gprs_elem);
      elem_gprs->ID=((ses_est*)r0)->unkID;
      elem_gprs->SesEst=new(DATETIME);
      REQUEST_DATEANDTIME_GET(SYNC,elem_gprs->SesEst);
      ListElement_AddtoTop(myList_gprs,elem_gprs);
    }
  }
  return(0);
}

void b2kb(int traffic,int * Kb_out,int * b1_out,int * b2_out)
{
  int Kb=traffic/1024;
  int b1=traffic%1024*10/1024;
  int b2=traffic%1024*10%1024*10/1024;
  if (traffic%1024*10%1024*10%1024*10/1024>=5) b2=b2+1;
  if (b2==10)
  {
    b1=b1+1;
    b2=0;
  }
  if (b1==10)
  {
    Kb=Kb+1;
    b1=0;
  }
  *Kb_out=Kb;
  *b1_out=b1;
  *b2_out=b2;
}


void GetTrafCost(int traffic,int cost,int * rub,int * kop1,int * kop2)
{
  int traf_cost=traffic*cost/1024;
  if (traffic*cost%1024*10/1024>=5) traf_cost=traf_cost+1;
  int cost_rub=traf_cost/100;
  int cost_kop1=traf_cost%100*10/100;
  int cost_kop2=traf_cost%100*10%100*10/100;
  *rub=cost_rub;
  *kop1=cost_kop1;
  *kop2=cost_kop2;
}

void delayed_save_gprs_info(u16 unk,LPARAM term_ID)
{
  int f;
  wchar_t fpath_gprs[200];
  wchar_t folder_gprs[20];
  wchar_t buffer_gprs_ptr[400];
  wchar_t * buffer_gprs=buffer_gprs_ptr;
  memset(buffer_gprs,0,800);
  GPRS_SESSION_INFO * buf_ses=new(GPRS_SESSION_INFO);
  GPRS_GetLastSessionInfo(0,buf_ses);
  int pos=ListElement_Find(myList_gprs,(void*)term_ID,myList_gprs_Find);
  if (pos!=LIST_ERROR)
  {
    myList_gprs_elem * elem_gprs=(myList_gprs_elem*)ListElement_Remove(myList_gprs,pos);
    wstrcpy(fpath_gprs,GetDir(DIR_OTHER | MEM_EXTERNAL));
    wstrcat(fpath_gprs,L"/Call Logger/");
    snwprintf(folder_gprs,20,L"%04d-%02d-%02d",elem_gprs->SesEst->date.year,elem_gprs->SesEst->date.mon,elem_gprs->SesEst->date.day);
    wstrcat(fpath_gprs,folder_gprs);
    if (fstat(fpath_gprs,L"gprs.txt",0)<0)
    {
      buffer_gprs[0]=0xFEFF;
      buffer_gprs=buffer_gprs+1;
    }
    int traf=buf_ses->SentBytes+buf_ses->RcvBytes;
    if (traf/1024<=TrafRound) traf=TrafRound;
    else
    {
      if (traf/1024%TrafRound>0) traf=traf/1024/TrafRound*TrafRound+TrafRound;
      else traf=traf/1024;
    }
    int cost_rub;
    int cost_kop1;
    int cost_kop2;
    if (TimeDay==TimeNight) GetTrafCost(traf,TrafCost,&cost_rub,&cost_kop1,&cost_kop2);
    else
    {
      int cur_time_base;
      int night_time_base;
      cur_time_base=elem_gprs->SesEst->time.hour;
      
      //TimeDay берем за базу
      if (TimeDay<TimeNight) night_time_base=TimeNight-TimeDay;
      else night_time_base=TimeNight-TimeDay+24;
      
      if (TimeDay<cur_time_base) cur_time_base=cur_time_base-TimeDay;
      else cur_time_base=cur_time_base-TimeDay+24;
      
      if (cur_time_base<night_time_base) GetTrafCost(traf,TrafCostDay,&cost_rub,&cost_kop1,&cost_kop2);
      else GetTrafCost(traf,TrafCostNight,&cost_rub,&cost_kop1,&cost_kop2);
    }
    if ((Hol1)||(Hol2))
    {
      int cur_day;
      char weekday;
      CALE_SET * baseday=new(CALE_SET);
      DATE_GetWeekDay((DATE*)elem_gprs->SesEst,&weekday);
      Cale_GetSettings(3,baseday);
      if (weekday>=baseday->cale_base)
      {
        cur_day=weekday-baseday->cale_base+1;
      }
      else
      {
        cur_day=7+weekday-baseday->cale_base+1;
      }
      delete(baseday);
      if ((Hol1==cur_day)||(Hol2==cur_day)) GetTrafCost(traf,TrafCostHol,&cost_rub,&cost_kop1,&cost_kop2);
    }
    
    if (TrafDiv)
    {
      if (TrafKb)
      {
        int Kb_s;
        int b_s1;
        int b_s2;
        int Kb_r;
        int b_r1;
        int b_r2;
        b2kb(buf_ses->SentBytes,&Kb_s,&b_s1,&b_s2);
        b2kb(buf_ses->RcvBytes,&Kb_r,&b_r1,&b_r2);
        
        snwprintf(buffer_gprs,390,L"Start time - %02d:%02d:%02d, sent - %d.%d%d Kb, received - %d.%d%d Kb, cost - %d.%d%d %ls\r\n",elem_gprs->SesEst->time.hour,elem_gprs->SesEst->time.min,elem_gprs->SesEst->time.sec,Kb_s,b_s1,b_s2,Kb_r,b_r1,b_r2,cost_rub,cost_kop1,cost_kop2,Money);
      }
      else
      {
        snwprintf(buffer_gprs,390,L"Start time - %02d:%02d:%02d, sent - %d b, received - %d b, cost - %d.%d%d %ls\r\n",elem_gprs->SesEst->time.hour,elem_gprs->SesEst->time.min,elem_gprs->SesEst->time.sec,buf_ses->SentBytes,buf_ses->RcvBytes,cost_rub,cost_kop1,cost_kop2,Money);
      }
    }
    else
    {
      traf=buf_ses->SentBytes+buf_ses->RcvBytes;
      if (TrafKb)
      {
        int Kb=0;
        int b1=0;
        int b2=0;
        b2kb(buf_ses->SentBytes+buf_ses->RcvBytes,&Kb,&b1,&b2);
        
        snwprintf(buffer_gprs,390,L"Start time - %02d:%02d:%02d, traffic - %d.%d%d Kb, cost - %d.%d%d %ls\r\n",elem_gprs->SesEst->time.hour,elem_gprs->SesEst->time.min,elem_gprs->SesEst->time.sec,Kb,b1,b2,cost_rub,cost_kop1,cost_kop2,Money);
      }
      else
      {
        snwprintf(buffer_gprs,390,L"Start time - %02d:%02d:%02d, traffic - %d b, cost - %d.%d%d %ls\r\n",elem_gprs->SesEst->time.hour,elem_gprs->SesEst->time.min,elem_gprs->SesEst->time.sec,traf,cost_rub,cost_kop1,cost_kop2,Money);
      }
    }
    if (fstat(fpath_gprs,L"gprs.txt",0)<0)
    {
      buffer_gprs=buffer_gprs-1;
    }
    int str_len=(wstrlen(buffer_gprs))*2;
    FSTAT _fstat;
    if ((fstat(fpath_gprs,L"gprs.txt",&_fstat)<0)||(!sort_order))
    {
      f=_fopen(fpath_gprs,L"gprs.txt",FSX_O_CREAT|FSX_O_APPEND,FSX_S_IREAD|FSX_S_IWRITE,0);
      fwrite(f,buffer_gprs,str_len);
    }
    else
    {
      char * new_buff=new char[_fstat.fsize+str_len];
      f=_fopen(fpath_gprs,L"gprs.txt",FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0);
      fread(f,new_buff,2);
      lseek(f,2,0);
      memcpy(new_buff+2,buffer_gprs,str_len);
      fread(f,new_buff+2+str_len,_fstat.fsize-2);
      lseek(f,0,0);
      fwrite(f,new_buff,_fstat.fsize+str_len);
      delete(new_buff);
    }
    fclose(f);
    if (incomleted_connection)
    {
      if (!ConnectionManager_Connection_GetState())
      {
        sum_traf_cost(elem_gprs->SesEst);
        incomleted_connection=0;
      }
    }
    myList_gprs_elem_Free(elem_gprs);
  }
  delete(buf_ses);
}

//Ловим окончание содинения
int onSessionTerminated(void * r0, BOOK *)
{
  if (r0)
  {
    Timer_Set(1000,delayed_save_gprs_info,((ses_term*)r0)->unkID);
  }
  return(0);
}

void get_from_a1(CALLMANAGER_DATA * cms,CALLMANAGER_EVENT_DATA * CallManStruct)
{
  cms->CallState=CallManStruct->CallState;
  cms->PNUM=CallManStruct->PNUM;
  cms->CallID=CallManStruct->CallID;
}

void get_from_a2(CALLMANAGER_DATA * cms,CALLMANAGER_EVENT_DATA_A2 * CallManStruct)
{
  cms->CallState=CallManStruct->CallState;
  cms->PNUM=CallManStruct->PNUM;
  cms->CallID=CallManStruct->CallID;
}


char* strrchr(char* str,char c)
{
  char* retpos=NULL;
  for(;*str;str++)if(*str==c)retpos=str;
  return retpos;
}


char * get_region(wchar_t * w_PNUM)
{
#define BLOCK_SIZE 2000
  wchar_t fpath_ini[200];
  char * region=0;
  FSTAT _fstat;
  wstrcpy(fpath_ini,GetDir(DIR_INI));
  wstrcat(fpath_ini,L"/Call Logger");
  if (fstat(fpath_ini,L"regions.ini",&_fstat)>=0)
  {
    int readen_size=0;
    int size_to_read;
    int ptr=0;
    char * buffer_ini=new char[BLOCK_SIZE+1];
    memset(buffer_ini,0x0,BLOCK_SIZE+1);
    int w_PNUM_len=wstrlen(w_PNUM);
    char * PNUM_str=new char[w_PNUM_len+1];
    
    int f=_fopen(fpath_ini,L"regions.ini",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
    
    while (readen_size<_fstat.fsize)
    {
      if ((_fstat.fsize-readen_size)<BLOCK_SIZE) size_to_read=_fstat.fsize-readen_size;
      else size_to_read=BLOCK_SIZE;
      lseek(f,readen_size,0);
      fread(f,buffer_ini,size_to_read);
      buffer_ini[size_to_read]=0;
      char * end_line=strrchr(buffer_ini,0xA);
      if (end_line[1]==0xD) end_line=end_line+2;
      else end_line=end_line+1;
      end_line[0]=0;
      readen_size=readen_size+(end_line-buffer_ini);
      int count=w_PNUM_len/2;
      wstr2strn(PNUM_str,w_PNUM,count);
      if (strstr(buffer_ini,PNUM_str))
      {
        ptr=1;
        while (count<=w_PNUM_len)
        {
          count=count+1;
          wstr2strn(PNUM_str,w_PNUM,count);
          if (!strstr(buffer_ini,PNUM_str)) break;
        }
        count=count-1;
      }
      else
      {
        while (count>0)
        {
          count=count-1;
          wstr2strn(PNUM_str,w_PNUM,count);
          if (strstr(buffer_ini,PNUM_str))
          {
            ptr=1;
            break;
          }
        }
      }
      if (ptr)
      {
        wstr2strn(PNUM_str,w_PNUM,count);
        region=manifest_GetParam(buffer_ini,PNUM_str,0);
        if (region) break;
      }
    }
    fclose(f);
    delete(buffer_ini);
    delete(PNUM_str);
  }
  return(region);
}
  
//Оновное действо.... Все в куче....
int OnCallManager(void * CallManStruct, BOOK *)
{
  int platform=GetChipID()&CHIPID_MASK;
  CALLMANAGER_DATA cms;
  if (platform==CHIPID_DB3150||platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350)
    get_from_a2(&cms,(CALLMANAGER_EVENT_DATA_A2*)CallManStruct);
  else
    get_from_a1(&cms,(CALLMANAGER_EVENT_DATA*)CallManStruct);
  //Ловим всех кроме idle
  if (cms.CallState)
  {
    //Создаем структурку
    myList_elem* temp_elem=new(myList_elem);
    int length=PNUM_len(cms.PNUM);
    //Проверяем наличие элемента в листе
    if (cms.CallID==0xFF)
    {
      if (myList->FirstFree)
      {
        temp_elem->line=1;
      }
      else
      {
        temp_elem->line=0;
      }
    }
    else
    {
      temp_elem->line=cms.CallID;
    }
    temp_elem->w_PNUM=new wchar_t[length+1];
    char * sp=new char[length+1];
    memset(sp,0,length+1);
    PNUM2str(sp,cms.PNUM,length,length+1);
    str2wstr(temp_elem->w_PNUM,sp);
    delete(sp);
    if (ListElement_Find(myList,temp_elem,myList_Find)==LIST_ERROR)
    {
      temp_elem->startdatetime=new DATETIME;
      REQUEST_DATEANDTIME_GET(SYNC,temp_elem->startdatetime);
      temp_elem->first_callstate=cms.CallState;
      temp_elem->last_callstate=0;
      wchar_t * CallName=CallStatusDesc_GetName(CallID_GetCallStatusDesc(temp_elem->line));
      if (CallName)
      {
        if (CallName[0])
        {
          temp_elem->Name=new wchar_t[wstrlen(CallName+1)];
          wstrcpy(temp_elem->Name,CallName+2);
        }
        else
        {
          temp_elem->Name=new wchar_t[14];
          temp_elem->Name[0]=0;
        }
      }
      else
      {
        temp_elem->Name=new wchar_t[20];
        temp_elem->Name[0]=0;
        wstrcpy(temp_elem->Name,L"Emergency");
      }
        
      temp_elem->begindatetime=new DATETIME;
      temp_elem->begindatetime->time.hour=0;
      temp_elem->begindatetime->time.min=0;
      temp_elem->begindatetime->time.sec=0;
      ListElement_AddtoTop(myList,temp_elem);
    }
    else
    {
      if (cms.CallState==6)
      {
        DATETIME enddatetime;
        REQUEST_DATEANDTIME_GET(SYNC,&enddatetime);
        myList_elem * elem=(myList_elem*)ListElement_Remove(myList,ListElement_Find(myList,temp_elem,myList_Find));
        int f;
        int sub;
        int m;
        int s;
        wchar_t fpath[200];
        wchar_t fname[20];
        wchar_t folder[20];
        wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
        wstrcat(fpath,L"/Call Logger/");
        wchar_t buffer_ptr[400];
        wchar_t * buffer=buffer_ptr;
        memset(buffer,0,800);
        if (!elem->Name[0])
        {
          snwprintf(elem->Name,8,L"NoName");
        }
        if (elem->last_callstate==0)
        {
          if (elem->first_callstate==2)
          {
            //Исходящий вызов
            wstrcpy(fname,L"out.txt");
          }
          if (elem->first_callstate==4)
          {
            //Пропущенный вызов
            wstrcpy(fname,L"missed.txt");
          }
          m=0;
          s=0;
        }
        else
        {
          if (elem->first_callstate==2)
          {
            //Исходящий вызов
            wstrcpy(fname,L"out.txt");
          }
          if (elem->first_callstate==4)
          {
            //Входящий вызов
            wstrcpy(fname,L"in.txt");
          }
          //Если вызов начался в 23, а закончился в 00, меняем 00 на 24
          if ((elem->begindatetime->time.hour==23)&&(enddatetime.time.hour==0)) enddatetime.time.hour=24;
          //Получаем время разговора в секундах
          sub=(enddatetime.time.hour-elem->begindatetime->time.hour)*3600+(enddatetime.time.min-elem->begindatetime->time.min)*60+enddatetime.time.sec-elem->begindatetime->time.sec;
          //Выделяем минуты и секунды
          m=sub/60;
          s=sub%60;
        }
        //Получаем название папки
        snwprintf(folder,20,L"%04d-%02d-%02d",elem->startdatetime->date.year,elem->startdatetime->date.mon,elem->startdatetime->date.day);
        //Получаем полный путь
        wstrcat(fpath,folder);
        //Проверяем существование файла, если нету то добавляем в буфер хидер юникодного файла
        if (fstat(fpath,fname,0)<0)
        {
          buffer[0]=0xFEFF;
          buffer=buffer+1;
        }
        
        char * region=get_region(elem->w_PNUM);
        if (region)
        {
          int reg_len=strlen(region);
          wchar_t * s2ws = new wchar_t[reg_len+1];
          win12512unicode(s2ws,region,reg_len);
          if ((elem->last_callstate==0)&&(elem->first_callstate==4))
          {
            snwprintf(buffer,390,L"%ls, %ls (%ls) - %02d:%02d:%02d\r\n",elem->Name,s2ws,elem->w_PNUM,elem->startdatetime->time.hour,elem->startdatetime->time.min,elem->startdatetime->time.sec);
          }
          else
          {
            snwprintf(buffer,390,L"%ls, %ls (%ls) - %02d:%02d:%02d, %02d:%02d\r\n",elem->Name,s2ws,elem->w_PNUM,elem->startdatetime->time.hour,elem->startdatetime->time.min,elem->startdatetime->time.sec,m,s);
          }
          delete(s2ws);
          delete(region);
        }
        else
        {
          if ((elem->last_callstate==0)&&(elem->first_callstate==4))
          {
            
            //Печатаем "отчет о звонке" в буфер без региона
            snwprintf(buffer,390,L"%ls (%ls) - %02d:%02d:%02d\r\n",elem->Name,elem->w_PNUM,elem->startdatetime->time.hour,elem->startdatetime->time.min,elem->startdatetime->time.sec);
          }
          else
          {
            snwprintf(buffer,390,L"%ls (%ls) - %02d:%02d:%02d, %02d:%02d\r\n",elem->Name,elem->w_PNUM,elem->startdatetime->time.hour,elem->startdatetime->time.min,elem->startdatetime->time.sec,m,s);
          }
        }
        if (fstat(fpath,fname,0)<0)
        {
          buffer=buffer-1;
        }
        int str_len=(wstrlen(buffer))*2;
        FSTAT _fstat;
        if ((fstat(fpath,fname,&_fstat)<0)||(!sort_order))
        {
          f=_fopen(fpath,fname,FSX_O_CREAT|FSX_O_APPEND,FSX_S_IREAD|FSX_S_IWRITE,0);
          fwrite(f,buffer,str_len);
        }
        else
        {
          char * new_buff=new char[_fstat.fsize+str_len];
          f=_fopen(fpath,fname,FSX_O_CREAT|FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0);
          fread(f,new_buff,2);
          lseek(f,2,0);
          memcpy(new_buff+2,buffer,str_len);
          fread(f,new_buff+2+str_len,_fstat.fsize-2);
          lseek(f,0,0);
          fwrite(f,new_buff,_fstat.fsize+str_len);
          delete(new_buff);
        }
        fclose(f);
        myList_elem_Free(elem);
      }
      if (cms.CallState==1)
      {
        myList_elem * elem=(myList_elem*)ListElement_GetByIndex(myList,ListElement_Find(myList,temp_elem,myList_Find));
        if (elem->last_callstate!=1)
        {
          elem->last_callstate=1;
          elem->begindatetime=new DATETIME;
          REQUEST_DATEANDTIME_GET(SYNC,elem->begindatetime);
        }
      }
      //Убиваем временный элемент если не засунули в лист
      delete(temp_elem->w_PNUM);
      delete(temp_elem);
    }
  }
  return(1);
}


static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    get_bcfg_key();
  }
  return(result);
}

const PAGE_MSG CL_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  ON_CALLMANAGER_EVENT_TAG, OnCallManager,
  UI_CONNECTIONMANAGER_SESSION_ESTABLISHED_EVENT_TAG, onSessionEstablished,
  UI_CONNECTIONMANAGER_SESSION_TERMINATED_EVENT_TAG, onSessionTerminated,
  CLOCK_DATEANDTIME_IND_EVENT_TAG,onClockChange,
  ELF_RECONFIG_EVENT,ReconfigElf,
  NIL_EVENT_TAG,0
};

PAGE_DESC base_page ={"CL_BasePage",0,CL_PageEvents};


void elf_exit(void)

{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void onCloseCLBook(BOOK * CLBook)
{
  List_FreeElements(myList,myList_elem_Filter,myList_elem_Free);
  List_Free(myList);
  List_FreeElements(myList_gprs,myList_elem_Filter,myList_gprs_elem_Free);
  List_Free(myList_gprs);
  if (Money) delete(Money);
  delete(cur_date);
  SUBPROC(elf_exit);
}

BOOK * CreateCLBook()
{
  CLBook=new(BOOK);
  CreateBook(CLBook,onCloseCLBook,&base_page,"Call Logger",-1,0);
  myList=List_New();
  myList_gprs=List_New();
  cur_date=new(DATETIME);
  REQUEST_DATEANDTIME_GET(SYNC,cur_date);
  get_bcfg_key();
  return(CLBook);
}


int myFind(BOOK* book)
{
  if(!strcmp(book->xbook->name,"Call Logger")) return(1);
  return(0);
}


int main (void)
{
  if (FindBook(myFind))
  {
    MessageBox(EMPTY_TEXTID,STR("Already runed"), NOIMAGE, 1, 5000,0);
    SUBPROC(elf_exit);
  }
  else
  {
    CreateCLBook();
    return(0);
  }
}
