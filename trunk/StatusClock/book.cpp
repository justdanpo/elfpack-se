#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#define AuthorInfo L"StatusClock v1.0\n© UltraShot\n"

extern DISP_OBJ_ONREDRAW_METHOD DREDRAW;
extern u16 timer;
void DispDraw(DISP_OBJ* DO,int a,int b,int c);
void Send_REDRAW_RELEASE();

typedef struct
{
  BOOK * book;
}MSG;

void elf_exit()
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

int MyBook_OnTerminate(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
};

int MyBook_OnShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,TextID_Create(AuthorInfo,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1 ,5000,msg->book);
  return(1);
};

//--------------------------------------------------------------
static int OnReconfig(void *mess ,BOOK *book)
{
  
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    DispObject_InvalidateRect(*StatusRow_p(),0);
    return 1;
  }
  return 0;
};
//--------------------------------------------------------------
void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
  memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
  sbm->SI_OldOnRedraw=DREDRAW;
  sbm->SI_NewOnRedraw=DispDraw;
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
};

int onElfKilled(void *mess ,BOOK* book)
{
  REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
  if (sbm->SK_NewOnRedraw==DREDRAW)
  {
    REDRAW_RELEASE_MESSAGE * ms= new REDRAW_RELEASE_MESSAGE;
    memcpy(ms,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) DREDRAW=sbm->SK_OldOnRedraw;

    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*StatusRow_p()),DispDraw);

    ms->SI_OldOnRedraw=EMPTY_REDRAW_METHOD;
    ms->SI_NewOnRedraw=DispDraw;

    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return BLOCK_EVENT_GLOBALLY;
  }
  return 0;
};

void MyBook_OnClose(BOOK * book)
{
  if (book)
  {
    Timer_Kill(&timer);
    DISP_OBJ *si=*StatusRow_p();
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(si), DREDRAW);
    SUBPROC(elf_exit);
  }
};

const PAGE_MSG HW_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , MyBook_OnTerminate,
  ELF_SHOW_INFO_EVENT  , MyBook_OnShowAuthorInfo,
  ELF_RECONFIG_EVENT,     OnReconfig,
  NIL_EVENT_TAG,            NULL
};

PAGE_DESC base_page ={"StatusClockPage",0,HW_PageEvents};

BOOK * MyBook_Create()
{
  BOOK * MyBook= new BOOK;
  CreateBook(MyBook,MyBook_OnClose,&base_page,"StatusClock",-1,0);
  return(MyBook);
};

