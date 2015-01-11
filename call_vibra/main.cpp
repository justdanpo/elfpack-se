#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"


typedef struct
{
  BOOK * book;
}MSG;


typedef struct _MYBOOK : BOOK
{
  int isA2;
  int ready_to_vib;
}CallVibra_Book;


int TerminateElf(void * ,BOOK * book)
{
  FreeBook(book);
  return(1);
}


int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("Call Vibra\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
  }
  return(0);
}


void elf_exit(void)

{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void vibrate(int time)
{
  PAudioControl pAC = AudioControl_Init();
  if (!pAC) pAC = *GetAudioControlPtr();
  
  AudioControl_Vibrate(pAC, time, 100, time);
}


int OnCallConnected(void * data, BOOK * book)
{
  if (vibra_connection) vibrate(dur_connection);
  return 1;
}


int get_from_a1(CALLMANAGER_EVENT_DATA * CallManStruct)
{
  return CallManStruct->CallState;
}


int get_from_a2(CALLMANAGER_EVENT_DATA_A2 * CallManStruct)
{
  return CallManStruct->CallState;
}


int OnCallManager(void * data, BOOK * book)
{
  CallVibra_Book * CallVibraBook = (CallVibra_Book*)book;
  
  int call_state;
  
  if (CallVibraBook->isA2) call_state=get_from_a2((CALLMANAGER_EVENT_DATA_A2*)data);
  else call_state=get_from_a1((CALLMANAGER_EVENT_DATA*)data);

  if (call_state)
  {
    switch(call_state)
    {
    case 2:
      CallVibraBook->ready_to_vib=1;
      break;
    case 1:
      if (CallVibraBook->ready_to_vib)
      {
        if (vibra_start) vibrate(dur_start);
        CallVibraBook->ready_to_vib=0;
      }
      break;
    case 6:
      if (CallVibraBook->ready_to_vib) CallVibraBook->ready_to_vib=0;
      break;
    }
  }
  return 1;
}


int OnCallTime(void * data, BOOK * book)
{
  TIME * time = (TIME*)data;
  if (vibra_conv)
  {
    int min = second_conv/60;
    int sec = second_conv%60;
    if ( ( (!second_conv) || (second_conv==60) ) && (!time->min) ) return 1;
    if ( (time->sec == sec) && ( (!min) || (time->min%min==0) ) ) vibrate(dur_conv);
  }
  return 1;
}


const PAGE_MSG CallVibra_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT, TerminateElf,
  ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
  ELF_RECONFIG_EVENT, ReconfigElf,
  ONGOINGCALL_CALL_CONNECTED_EVENT_TAG, OnCallConnected,
  ONGOINGCALL_SET_CALLTIME_EVENT_TAG, OnCallTime,
  ON_CALLMANAGER_EVENT_TAG, OnCallManager,
  NIL_EVENT_TAG,0
};

PAGE_DESC base_page ={"CallVibra_BasePage",0,CallVibra_PageEvents};


void onCloseCallVibraBook(BOOK * book)
{
  SUBPROC(elf_exit);
}


int myFind(BOOK* book)
{
  if(!strcmp(book->xbook->name,"Call Vibra")) return(1);
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
    CallVibra_Book * CallVibraBook = (CallVibra_Book*)malloc(sizeof(CallVibra_Book));
    CreateBook(CallVibraBook,onCloseCallVibraBook,&base_page,"Call Vibra",-1,0);
    
    CallVibraBook->ready_to_vib=0;
    
    int platform=GetChipID()&CHIPID_MASK;
    if (platform==CHIPID_DB3150||platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350) CallVibraBook->isA2=1;
    else CallVibraBook->isA2=0;
  }
  
  return(0);
}
