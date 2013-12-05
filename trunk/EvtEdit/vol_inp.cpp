#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "..\include\var_arg.h"
#include "header\pages.h"
#include "header\externals.h"
#include "header\lng.h"


GUI *CreateVolInput(BOOK *bk, int vol, int maxvol)
{
  GUI *g=CreatePercentInputVA(0, 
                             VAR_BOOK(bk), 
                             VAR_PERINP_ROWNUM(maxvol),
                             VAR_PERINP_CURPERC(vol), 
                             VAR_HEADER_TEXT(TextID_Create(lng[VOLUME],ENC_UCS2,TEXTID_ANY_LEN)));
  GUIInput_SetIcon(g, 0xEB42);
  return g;
};

void Vol_OnSave(BOOK *bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)bk;
  int v=PercentInput_GetPercent(mbk->vol);
  if (mbk->vol_type==0)
  {
    mbk->tempvol= v * mbk->vol_mod;
    BookObj_CallPage(bk,&SelectFile_page);
  }
  else
  {
    DELETE(mbk->selev->param);
    mbk->selev->param=new wchar_t[2];
    snwprintf(mbk->selev->param,1,L"%d", v);
    BookObj_CallPage(bk,&bk_main);
  }
};

void Vol_OnBack(BOOK *bk, GUI* )
{
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

static int VolInputPage_OnEnter(void *,BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK*)bk;
  mbk->vol=CreateVolInput(bk, mbk->curvol, mbk->maxvol);
  GUIObject_SoftKeys_SetAction(mbk->vol,ACTION_SAVE,Vol_OnSave);
  GUIObject_SoftKeys_SetAction(mbk->vol,ACTION_BACK,Vol_OnBack);
  GUIObject_Show(mbk->vol);
  return (1);
};

static int VolInputPage_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->vol);
  return (1);  
};

const PAGE_MSG bk_msglst_VolInput[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    VolInputPage_OnEnter,
  PAGE_EXIT_EVENT_TAG,     VolInputPage_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_VolInput = {"EvtEdit_VolumeControl_Page",0,bk_msglst_VolInput};
