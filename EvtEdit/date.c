#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "..\include\var_arg.h"
#include "header\pages.h"
#include "header\lng.h"
#include "header\externals.h"

#define IDN_TIME_INPUT_ICON L"FR_INPUT_GUI_SET_TIME_ICN"
#define IDN_DATE_INPUT_ICON L"FR_INPUT_GUI_SET_DATE_ICN"

#define DI_GET_YEAR(x) ((unsigned int)x&0xFFFF)
#define DI_GET_MONTH(x) (((unsigned int)x>>16)&0xFF)
#define DI_GET_DAY(x) ((unsigned int)x>>24)

void AcceptDIinput(BOOK *bk, GUI* data)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  unsigned int i=DateInput_GetDateInt(data);
  DATE *date=mbk->curdate;
  date->year=DI_GET_YEAR(i);
  date->mon=DI_GET_MONTH(i);
  date->day=DI_GET_DAY(i);
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

void CloseDIinput(BOOK * bk, GUI* )
{
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

static int DI_OnEnter(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  DATE d;
  DATE *date=mbk->curdate;
  int date_format=4;
  char date_f;
  int icon_id;
  if (date->year==0)
  {
    d.year=2000;
  }
  else
  {
    d.year=date->year;
  }
  if (date->mon==0)
  {
    d.mon=1;
  }
  else
  {
    d.mon=date->mon;
  }
  if (date->day==0)
  {
    d.day=1;
  }
  else
  {
    d.day=date->day;
  }
  if (!REQUEST_DATEFORMAT_GET(SYNC,&date_f)) date_format=date_f;
  mbk->dinput=CreateDateInputVA(0,
                                VAR_HEADER_TEXT(TextID_Create(lng[LNG_DATE],ENC_UCS2,TEXTID_ANY_LEN)),
                                VAR_DATEINP_DATEFORMAT(date_format),
                                VAR_DATEINP_DATE(&d),
                                VAR_BOOK(&mbk->book),
                                0);
  GUIObject_SoftKeys_SetAction(mbk->dinput,ACTION_ACCEPT, AcceptDIinput);
  GUIObject_SoftKeys_SetText(mbk->dinput,ACTION_ACCEPT,STR("OK"));
  GUIObject_SoftKeys_SetVisible(mbk->dinput,ACTION_ACCEPT,1);
  GUIObject_SoftKeys_SetAction(mbk->dinput,ACTION_BACK, CloseDIinput);
  if (iconidname2id(IDN_DATE_INPUT_ICON,-1,&icon_id))
    GUIInput_SetIcon(mbk->dinput,icon_id);
  return (1);
};

static int DI_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->dinput);
  return (1);  
};

const PAGE_MSG bk_msglst_dateinput[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    DI_OnEnter,
  PAGE_EXIT_EVENT_TAG,     DI_OnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_date_input = {"EvtEdit_DateInput_Page",0,bk_msglst_dateinput};
