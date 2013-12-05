#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"


typedef enum
{
  Start=0,
  SafeCharge,
  Charge,
  Await,
  AwaitExtTemp,
  AwaiSafeTime,
  SOSCharg,
  Handheld,
  Completed,
  ConstCurrent,
  ConstVoltage,
  CompSafeTime,
  CompLowCurr,
  CompDVDT,
  CompDTDT,
  CompD2VDT2,
  CompFlatV
}ChargingState;


typedef struct _MYBOOK : BOOK
{
  u16 timer;
}RL_Book;


void onTimer(u16 unk,RL_Book * RLBook);
void BeginTimer(u16 unk,RL_Book * RLBook);


typedef struct
{
  BOOK * book;
}MSG;


int TerminateElf(void * ,BOOK * book)
{
  FreeBook(book);
  return(1);
}


int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("RedLED Blinked v1.2\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


int BeginTimer_event(void *,BOOK * book)
{
  RL_Book * RLBook = (RL_Book*)book;
  
  BeginTimer(RLBook->timer, RLBook);
  return(1);
}


int KillTimer_event(void *,BOOK * book)
{
  RL_Book * RLBook = (RL_Book*)book;
  
  if (RLBook->timer)
  {
    Timer_Kill(&RLBook->timer);
    RLBook->timer=0;
    RedLED_Off(0);
  }
  return(1);
}


void BeginTimer(u16 timerID, RL_Book * RLBook)
{
  BATT bat_struct;
  GetBatteryState(SYNC,&bat_struct);
  
  if ((bat_struct.ChargingState!=Handheld)&&(bat_struct.ChargingState!=Completed))
  {
    RedLED_On(0);
    Timer_ReSet(&RLBook->timer,OnTime,MKTIMERPROC(onTimer),RLBook);
  }
  else
  {
    RLBook->timer=0;
  }
}


void onTimer (u16 unk, RL_Book * RLBook)
{ 
  RedLED_Off(0);
  Timer_ReSet(&RLBook->timer,OffTime,MKTIMERPROC(BeginTimer),RLBook);
}


static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return(result);
}


const PAGE_MSG RL_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  BATTERY_CHARGER_CONNECTED_EVENT_TAG, BeginTimer_event,
  BATTERY_CHARGER_DISCONNECTED_EVENT_TAG, KillTimer_event,
  ELF_RECONFIG_EVENT,ReconfigElf,
  NIL_EVENT_TAG,0
};

PAGE_DESC base_page ={"RL_BasePage",0,RL_PageEvents};


void elf_exit(void)

{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void onCloseRLBook(BOOK * book)
{
  RL_Book * RLBook = (RL_Book*)book;
  
  if (RLBook->timer)
  {
    Timer_Kill(&RLBook->timer);
    RLBook->timer=0;
    RedLED_Off(0);
  }
  SUBPROC(elf_exit);
}


void CreateRLBook()
{
  RL_Book * RLBook = (RL_Book*)malloc(sizeof(RL_Book));
  CreateBook(RLBook,onCloseRLBook,&base_page,"Blinked Red LED",-1,0);
  RLBook->timer=0;
  BeginTimer(RLBook->timer,RLBook);
}


int myFind(BOOK* book)
{
   if(!strcmp(book->xbook->name,"Blinked Red LED")) return(1);
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
    InitConfig();
    CreateRLBook();
    return(0);
  }
}
