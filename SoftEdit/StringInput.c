#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"
#include "..\\include\var_arg.h"
#include <errno.h>

extern const PAGE_DESC bk_StringInput;
extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);

int GetStrID(wchar_t *txt);

void OnBackCreateTextInputGui(BOOK * bk, u16 *string, int len)
{
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
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

void OnOkCreateWinOrPassGui(BOOK * bk, wchar_t *string, int len)
{
  MyBOOK * mbk=(MyBOOK *)bk;
  kickbadsymbols(string,len);
  if (mbk->StringInputType==0)
  {
    DELETE(mbk->curit->name);
    if (len>0)
    {
      mbk->curit->name=new wchar_t[len+1];
      wstrncpy(mbk->curit->name,string, len);
      mbk->curit->name[len]=L'\0';
    }
    else
    {
      mbk->curit->name=0;
    }
  }
  if (mbk->StringInputType==1)
  {
    DELETE(mbk->curit->lsi);
    if (len>0)
    {
      mbk->curit->lsi=new wchar_t[len+1];
      wstrncpy(mbk->curit->lsi,string, len);
      mbk->curit->lsi[len]=L'\0';
    }
    else
    {
      mbk->curit->lsi=0;
    }
  }
  if (mbk->StringInputType==2)
  {
    DELETE(mbk->curit->msi);
    if (len>0)
    {
      mbk->curit->msi=new wchar_t[len+1];
      wstrncpy(mbk->curit->msi,string, len);
      mbk->curit->msi[len]=L'\0';
    }
    else
    {
      mbk->curit->msi=0;
    }
  }
  if (mbk->StringInputType==3)
  {
    DELETE(mbk->curit->rsi);
    if (len>0)
    {
      mbk->curit->rsi=new wchar_t[len+1];
      wstrncpy(mbk->curit->rsi,string,len);
      mbk->curit->rsi[len]=L'\0';
    }
    else
    {
      mbk->curit->rsi=0;
    }
  }
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

int get_curstrid(MyBOOK *mbk)
{
  switch (mbk->StringInputType)
  {
  case 0:
    if (mbk->curit->name)
    {
      return TextID_Create(mbk->curit->name,ENC_UCS2,TEXTID_ANY_LEN);
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 1:
    if (mbk->curit->lsi)
    {
      return TextID_Create(mbk->curit->lsi,ENC_UCS2,TEXTID_ANY_LEN);
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 2:
    if (mbk->curit->msi)
    {
      return TextID_Create(mbk->curit->msi,ENC_UCS2,TEXTID_ANY_LEN);
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
  case 3:
    if (mbk->curit->rsi)
    {
      return TextID_Create(mbk->curit->rsi,ENC_UCS2,TEXTID_ANY_LEN);
    }
    else
    {
      return TextID_Create(L"",ENC_UCS2,TEXTID_ANY_LEN);
    }
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
  if (myBook->StringInputType>0)
  {
    en=1;
  }
  myBook->StringInput = CreateStringInputVA(0,
                                              VAR_HEADER_TEXT(GetStrID(L"WAP_ENTER_SK")),
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


//____________________
static int StringInputPage_OnEnter(void *,BOOK * bk)
{
  CreateTextInput((MyBOOK*)bk);
  return (1);
};

static int StringInputPage_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->StringInput);
  return (1);  
};

const PAGE_MSG bk_msglst_StringInput[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    StringInputPage_OnEnter,
  PAGE_EXIT_EVENT_TAG,     StringInputPage_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_StringInput = {"SoftEdit_StringInput_Page",0,bk_msglst_StringInput};
//____________________
