#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

typedef struct
{
  BOOK * book;
}MSG;

extern DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw;
extern DISP_OBJ * GUI_display;
extern DISP_DESC * Display_desc;

extern void Display_ReDraw(DISP_OBJ * db,int r1, int r2,int r3);

extern u16 timer;

void elf_exit(void);

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE *sbm = new REDRAW_RELEASE_MESSAGE;

  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;

  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}

int TerminateSBDraw(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
};

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("EvtDesk\n(C) UltraShot"), NOIMAGE, 1, 5000,msg->book);
  return(1);
};


int SB_ELF_Killed(void *mess, BOOK *book)
{
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Display_oldReDraw = sbm->SB_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);

    ms->SB_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(BLOCK_EVENT_GLOBALLY);
  }
  return(0);
}

static int OnReconfig(void *mess ,BOOK *book)
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

const PAGE_MSG SBDraw_PageEvents[]@ "DYN_PAGE" ={
  SBY_REDRAW_RELEASE_EVENT  , SB_ELF_Killed,
  ELF_TERMINATE_EVENT       , TerminateSBDraw,
  ELF_SHOW_INFO_EVENT       , ShowAuthorInfo,
  ELF_RECONFIG_EVENT        , OnReconfig,
  NIL_EVENT_TAG             , NULL

};

PAGE_DESC base_page ={"SBDraw_BasePage",0,SBDraw_PageEvents};

void onCloseMyBook(BOOK * book)
{
  if (book)
  {
    Timer_Kill(&timer);
    if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
    SUBPROC(elf_exit);
  }
};

BOOK * CreateEvtDeskBook()
{
  BOOK * EvtDeskBook= new BOOK;
  CreateBook(EvtDeskBook,onCloseMyBook,&base_page,"EvtDesk",-1,0);
  return(EvtDeskBook);
};
