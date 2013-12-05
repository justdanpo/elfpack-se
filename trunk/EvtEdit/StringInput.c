#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "..\include\var_arg.h"
#include "header\pages.h"
#include "header\externals.h"
#include "header\lng.h"
#include <errno.h>
extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);


void OnBackCreateTextInputGui(BOOK * bk, u16 *string, int len)
{
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

void kickbadsymbols(wchar_t *string, int len)
{
  char *str=(char*)string;
  if (len>0)
  {
    int x;
    for (x=0;x<(len*2);x++)
    {
      if (str[x]==0xFE || str[x]==0xFF)
      {
        str[x]=' ';
      }
    }
  }
};
void kickbadsymbols2(wchar_t *string, int len)
{
  if (len>0)
  {
    int x;
    for (x=0;x<len;x++)
    {
      if (string[x]==L':')
      {
        string[x]=L'-';
      }
    }
  }
};

char *unicode2win1251(char *s, wchar_t *ws, int len)
{
  char *d=s;
  int c;
  while((c=*ws++)&&((len--)>0))
  {
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    *s++=c;
  }
  *s=0;
  return(d);
};

void OnOkCreateWinOrPassGui(BOOK * bk, wchar_t *string, int len)
{
  MyBOOK * mbk=(MyBOOK *)bk;
  kickbadsymbols(string, len); 
  if (mbk->TextInputType==0)
  {
    if (mbk->minint==-1)
    {
      DELETE(mbk->selev->param);
      if (len>0)
      {
        mbk->selev->param=new wchar_t[len+1];
        wstrcpy(mbk->selev->param,string);
        mbk->selev->param[len]=L'\0';
      }
      else
      {
        mbk->selev->param=0;
      }
    }
    else
    {
      DELETE(mbk->selev->param);
      unsigned int ui;
      *_Geterrno()=0;
      ui=wcstoul(string,0,10);
      if (ui<mbk->minint || ui>mbk->maxint || *_Geterrno()==ERANGE)
      {
        wchar_t ustr[64];
        snwprintf(ustr,MAXELEMS(ustr)-1,L"min: %u\nmax: %u",mbk->minint , mbk->maxint );
        MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, bk);
        return;
      }
      else
      {
         mbk->selev->param=new wchar_t[len+1];
         wstrcpy(mbk->selev->param,string);
         mbk->selev->param[len]=L'\0';
      }
    }
  }
  else if (mbk->TextInputType==1)
  {
    DELETE(mbk->selev->txt);
    if (len>0)
    {
      mbk->selev->txt=new wchar_t[len+1];
      wstrcpy(mbk->selev->txt,string);
      mbk->selev->txt[len]=L'\0';
    }
  }
  else if (mbk->TextInputType==2)
  {
    int res=wcstoul(string,0,10);
    mbk->selev->AskBefore=res;
  }
   else if (mbk->TextInputType==3)
  {
    int res=wcstoul(string,0,10);
    mbk->selev->AskAfter=res;
  }
  else if (mbk->TextInputType==4)
  {
    kickbadsymbols2(string, len);
    if (len>0)
    {
      mbk->selev->isGps=true;
      DELETE(mbk->selev->gps);
      mbk->selev->gps=new char[wstrlen(string)+1];
      unicode2win1251(mbk->selev->gps,string,wstrlen(string));
    }
    else
    {
      mbk->selev->isGps=false;
      DELETE(mbk->selev->gps);
      mbk->selev->dtype=0;
      mbk->selev->ttype=1;
    }
  }
  else if (mbk->TextInputType==5)
  {
    int res=wcstoul(string,0,10);
    mbk->tempvol=res;
  }
  else if (mbk->TextInputType==6)
  {
    TIME *time=mbk->curtime;
    *_Geterrno()=0;
    unsigned int ui=wcstoul(string,0,10);
    if (ui<mbk->minint || ui>mbk->maxint || *_Geterrno()==ERANGE)
    {
      wchar_t ustr[64];
      snwprintf(ustr,MAXELEMS(ustr)-1,L"min: %u\nmax: %u",mbk->minint , mbk->maxint );
      MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, bk);
      return;
    }
    else
    {
      time->sec=ui;
    }
  }
  else if (mbk->TextInputType==7)
  {
    int step=mbk->cur_clone_time;
    int x=mbk->selev->time.hour * 3600 + mbk->selev->time.min * 60 + mbk->selev->time.sec;
    x=x+step;
    int i=0,i_max=wcstoul(string,0,10);
    *_Geterrno()=0;
    if (i_max<mbk->minint || i_max>mbk->maxint || *_Geterrno()==ERANGE)
    {
      wchar_t ustr[64];
      snwprintf(ustr,MAXELEMS(ustr)-1,L"min: %u\nmax: %u",mbk->minint , mbk->maxint );
      MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, bk);
      return;
    }
    for (x=x;x<86400;x+=step)
    {
      if (i<i_max)
      {
        EVENT *ev=new EVENT;
        memcpy(ev,mbk->selev, sizeof(EVENT));
        ev->time.hour=x/3600;
        ev->time.min=(x%3600)/60;
        ev->time.sec=((x%3600)%60)/60;
        if (mbk->selev->txt)
        {
          ev->txt=new wchar_t[wstrlen(mbk->selev->txt)+1];
          wstrcpy(ev->txt, mbk->selev->txt);
        }
        if (mbk->selev->param)
        {
          ev->param=new wchar_t[wstrlen(mbk->selev->param)+1];
          wstrcpy(ev->param, mbk->selev->param);
        }
        if (mbk->selev->gps)
        {
          ev->gps=new char[strlen(mbk->selev->gps)+1];
          strcpy(ev->gps, mbk->selev->gps);
        }
        extern LIST *events;
        List_InsertLast(events,ev);
        i++;
      }
      else break;
    }
  }
  else if (mbk->TextInputType==8)
  {
    int step=wcstoul(string,0,10);
    *_Geterrno()=0;
    if (step<mbk->minint || step>mbk->maxint || *_Geterrno()==ERANGE)
    {
      wchar_t ustr[64];
      snwprintf(ustr,MAXELEMS(ustr)-1,L"min: %u\nmax: %u",mbk->minint , mbk->maxint );
      MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, bk);
      return;
    }
    else
    {
      step*=60;
    }
    mbk->cur_clone_time=step;
  }
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
  if (mbk->TextInputType==7)
  {
    extern const PAGE_DESC bk_StringInput;
    BookObj_CallPage(bk,&bk_main);
  }
  else if (mbk->TextInputType==8)
  {
    extern const PAGE_DESC bk_StringInput;
    mbk->TextInputType=7;
    mbk->inputType=IT_UNSIGNED_DIGIT;
    BookObj_CallPage(bk,&bk_StringInput);
    return;
  }
  else if (mbk->TextInputType==2 || mbk->TextInputType==3)
  {
    BookObj_CallPage(bk,&bk_main);
    BookObj_CallPage(bk,&bk_days);
  }
  else if (mbk->TextInputType==5)
  {
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else
  {
    BookObj_CallPage(bk,&bk_main);
  }
};

int get_headerstrid(int type)
{
  switch (type)
  {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    return GetStrID(L"WAP_ENTER_SK");
  case 6:
    return TextID_Create(lng[SECONDS],ENC_UCS2,TEXTID_ANY_LEN);
  case 7:
    return TextID_Create(lng[MAXCOUNT],ENC_UCS2,TEXTID_ANY_LEN);
  case 8:
    return TextID_Create(lng[INTERVAL],ENC_UCS2,TEXTID_ANY_LEN);
  default:
    return EMPTY_TEXTID;
  }
};
int get_curstrid(MyBOOK *mbk)
{
  switch (mbk->TextInputType)
  {
  case 0:
    if (mbk->selev->param)
    {
      int ret_strid=TextID_Create(mbk->selev->param,ENC_UCS2,TEXTID_ANY_LEN);
      return ret_strid;
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 1:
    if (mbk->selev->txt)
    {
      int ret_strid=TextID_Create(mbk->selev->txt,ENC_UCS2,TEXTID_ANY_LEN);
      return ret_strid;
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 2:
    return TextID_CreateIntegerID(mbk->selev->AskBefore);
  case 3:
    return TextID_CreateIntegerID(mbk->selev->AskAfter);
  case 4:
    if (mbk->selev->gps)
    {
      wchar_t *ret=new wchar_t[strlen(mbk->selev->gps)+1];
      str2wstr(ret,mbk->selev->gps);
      int ret_strid=TextID_Create(ret,ENC_UCS2,TEXTID_ANY_LEN);
      delete(ret);
      return ret_strid;
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 6:
    return TextID_CreateIntegerID(mbk->curtime->sec);
  case 8:
    return TextID_CreateIntegerID(0);
  }
  return EMPTY_TEXTID;
};

int getnumwidth(unsigned int num)
{
  int i=1;
  while(num>=10)
  {
    num/=10;
    i++;
  }
  return (i);
};

void CreateTextInput(MyBOOK *myBook)
{
  int text=EMPTY_TEXTID;
  int type=myBook->inputType;
  int size=0;
  text=get_curstrid(myBook);
  if (myBook->inputType==IT_UNSIGNED_DIGIT)
  {
    size=getnumwidth(myBook->maxint);
  }
  else
  {
    size=myBook->maxint;
  }
  int en=0;
  if (myBook->TextInputType==4 || myBook->TextInputType==1)
  {
    en=1;
  }
  myBook->text_input = CreateStringInputVA(0,
                                              VAR_HEADER_TEXT(get_headerstrid(myBook->TextInputType)),
                                              VAR_STRINP_FIXED_TEXT(GetStrID(L"EC_TEXTP_TXT")),
                                              VAR_STRINP_ENABLE_EMPTY_STR(en),
                                              VAR_STRINP_MAX_LEN(size),
                                              VAR_STRINP_MODE(type),
                                              VAR_STRINP_TEXT(text),
                                              VAR_BOOK(myBook),
                                              VAR_PREV_ACTION_PROC(OnBackCreateTextInputGui),
                                              VAR_OK_PROC(OnOkCreateWinOrPassGui),
                                              0);
};


static int StringInputPage_OnEnter(void *,BOOK * bk)
{
  CreateTextInput((MyBOOK*)bk);
  return (1);
};

static int StringInputPage_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->text_input);
  return (1);  
};

const PAGE_MSG bk_msglst_StringInput[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    StringInputPage_OnEnter,
  PAGE_EXIT_EVENT_TAG,     StringInputPage_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_StringInput = {"EvtEdit_StringInput_Page",0,bk_msglst_StringInput};

