#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "structs.h"
#include "conf_loader.h"
#include "..\\include\cfg_items.h"

void mbox_Create(BOOK *bk, int type);
int isRealGSIBook(BOOK * struc);


int isGSIBook(BOOK *bk)
{
  if (!strcmp(bk->xbook->name,"GSI")) return 1; 
  return 0; 
};

static int TerminateElf(void *mess ,BOOK *bk)
{
  FreeBook(bk);
  return(1);
}

static int ShowAuthorInfo(void *mess ,BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK*)FindBook(isRealGSIBook);
  mbk->stat=0;
  mbox_Create(&mbk->book, 0);
  BookObj_SetFocus(&mbk->book,0);
  return(1);
}

int GPRS_Terminated(void *mess ,BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK*)bk;
  mbk->stat=1;
  BookObj_GotoPage(bk,&bk_main);
  mbk->stat=0;
  BookObj_SetFocus(bk,0);
  return 0;
}

static int onReconfigEvent(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return(result);
};

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,                              TerminateElf,
  ELF_SHOW_INFO_EVENT,                              ShowAuthorInfo,
  ELF_RECONFIG_EVENT,                               onReconfigEvent,
  UI_CONNECTIONMANAGER_SESSION_TERMINATED_EVENT_TAG,GPRS_Terminated,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"GSI_Base_Page",0,bk_msglst_base};

