#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "config_data.h"
#include "structs.h"

void get_nd_cost(int *costOut, int *costIn,int *length, int *cost)
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC, &dt);
  char hour=dt.time.hour;
  if (hour > DAY_TIME.hour && hour < NIGHT_TIME.hour)
  {
    costOut[0]=cfgDayCostOut;
    costIn[0]=cfgDayCostIn;
    length[0]=cfgDayLength;
    cost[0]=cfgDayCost;
  }
  else
  {
    costOut[0]=cfgNightCostOut;
    costIn[0]=cfgNightCostIn;
    length[0]=cfgNightLength;
    cost[0]=cfgNightCost;
  }
};

TRAF TrafCost_Get(int rcv, int sent)
{
  int cfgLength, cfgCostOut, cfgCostIn, cfgCost;
  get_nd_cost(&cfgCostOut, &cfgCostIn, &cfgLength, &cfgCost);
  int full = rcv+sent;
  TRAF traf;
  traf.rtraf=full;
  if (full<(cfgNotCountedTraf*1024))
  {
    traf.in=0;
    traf.out=0;
    traf.cost=0;
  }
  else
  {
    int cfgMinLength2=cfgMinLength*1024;
    if (full<=cfgMinLength2)
    {
      full=cfgMinLength2;
    }
    else
    {
      if ((full%cfgMinLength2)>0)
      {
        full=full-(full%cfgMinLength2)+cfgMinLength2;
      }
    }
    float f_out, f_in, f_pkglen, f_costOut, f_costIn, f_length, f_full, f_cost;
    f_out=(float)sent;
    f_in=(float)rcv;
    f_length=(float)cfgLength;
    f_pkglen=f_length*1024.0;
    f_costOut=(float)cfgCostOut;
    f_costIn=(float)cfgCostIn;
    f_cost=(float)cfgCost;
    f_full=(float)full;
    traf.out=f_out/f_pkglen * f_costOut;
    traf.in=f_in/f_pkglen * f_costIn;
    if (cfgDivide)
    {
      traf.cost=traf.out+traf.in;
    }
    else
    {
      traf.cost=f_full/f_pkglen * f_cost;
    }
  }
  traf.traf=full;
  return traf;
};

wchar_t *get_gprstxt(int stat)
{
  if (stat) return L"disconnected";
  return L"last session";
}

//____________________
void Feedback_OnClose(BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (mbk->mbox)GUIObject_Destroy(mbk->mbox);
  mbk->mbox=0;
  if (!mbk->ShouldClose)
  {
    BookObj_Hide(bk,0);
  }
  else
  {
    FreeBook(bk);
  }
}

void Feedback_OnOK( BOOK* bk, GUI* )
{
  Feedback_OnClose(bk);
}

void mbox_Create(BOOK *bk, int type)
{
  MyBOOK *mbk=(MyBOOK*)bk;
  GPRS_SESSION_INFO gprs;
  GPRS_GetLastSessionInfo(0,&gprs);
  wchar_t str[512];
  TRAF traf=TrafCost_Get(gprs.RcvBytes,gprs.SentBytes);
  if (type)
  {
    snwprintf(str,512,L"GPRS %ls\n\nTime: %d:%02d\nSent: %d KB(%0.2f)\nRcvd: %d KB(%0.2f)\nTraf: %d (%d)KB\nCost: %0.2f %s", 
              get_gprstxt(mbk->stat),
              gprs.SesTime/60,
              gprs.SesTime%60, 
              gprs.SentBytes/1024,
              traf.out,
              gprs.RcvBytes/1024, 
              traf.in,
              traf.rtraf /1024, 
              traf.traf /1024,
              traf.cost, 
              cfgCurrency);
  }
  else
  {
    snwprintf(str,512,L"GPRS %ls\n\nTime: %d:%02d\nSent: %d KB(%0.2f)\nRcvd: %d KB(%0.2f)\nTraf: %d (%d)KB\nCost: %0.2f %s\n© UltraShot", 
              get_gprstxt(mbk->stat),
              gprs.SesTime/60,
              gprs.SesTime%60, 
              gprs.SentBytes/1024,
              traf.out,
              gprs.RcvBytes/1024, 
              traf.in,
              traf.rtraf /1024, 
              traf.traf /1024,
              traf.cost, 
              cfgCurrency);
  }
  mbk->mbox=TextFeedbackWindow(&mbk->book,0);
  Feedback_SetTextExtended(mbk->mbox,TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN),0);
  GUIObject_SetStyle(mbk->mbox,1);
  GUIObject_SoftKeys_SetAction(mbk->mbox,ACTION_SELECT1,Feedback_OnOK);
  GUIObject_SoftKeys_SetAction(mbk->mbox,ACTION_BACK,Feedback_OnOK);
  Feedback_SetOnClose(mbk->mbox,Feedback_OnClose);
  Feedback_SetTimeout(mbk->mbox,cfgTimeout * 1000);
  GUIObject_Show(mbk->mbox);
  BookObj_SetFocus(bk,0);
  mbk->stat=0;
}
//______________
static int MainPageOnCreate(void *, BOOK *bk)
{
  mbox_Create(bk, 1);
  return (1);
}

const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    MainPageOnCreate,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_main = {"GSI_Main_Page",0,bk_msglst_main};

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

static void onMyBookClose(BOOK * book)
{
  SUBPROC(elf_exit);
}
int isRealGSIBook(BOOK * struc)
{
  return(struc->onClose == onMyBookClose);
}
//_____________________
int main()
{
  int ShouldClose=false;
  if (FindBook(isGSIBook))ShouldClose=true;
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook(&myBook->book,onMyBookClose,&bk_base,"GSI",-1,0))
  {
    delete myBook;
    SUBPROC(elf_exit);
    return (0);
  }
  InitConfig();
  if (ShouldClose)
  {
    myBook->ShouldClose=true;
    BookObj_GotoPage(&myBook->book,&bk_main);
  }
  return 0;
}
