#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"


typedef struct
{
  BOOK  book;
}MyBOOK;

DISP_OBJ * StatusRow;
TEXTID SIDheap=EMPTY_TEXTID;
void (*SIonRedraw)(DISP_OBJ *,int r1,int r2,int r3);

u16 timer; // ID таймера

#pragma segment="ELFBEGIN"
void elf_exit(void){
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}


void Draw(DISP_OBJ * db,int r1, int r2,int r3)
{
  // вызываем родной onRedraw.
  SIonRedraw(db,r1,r2,r3);
  {
    if (IS_ENABLED)
    {
      SetFont(FONT);
      //очистили прямоугольник
      DrawRect(X_POS,Y_POS,X_POS+40,Y_POS+9,BRUSH_COLOR,BRUSH_COLOR) ;
      
      //печатаем хип
      DrawString(SIDheap,2,X_POS,Y_POS,X_POS+40,Y_POS+9,01,0x01,PEN_COLOR,BRUSH_COLOR);//2
    }
  }
}

void onTimer(u16 r0, LPARAM)
{
  if (SIDheap!=EMPTY_TEXTID){TextID_Destroy(SIDheap);SIDheap=EMPTY_TEXTID;}
  SIDheap=TextID_CreateIntegerID(GetFreeBytesOnHeap());
  Timer_ReSet(&timer,REFRESH_TIME,onTimer,0);
  DispObject_InvalidateRect(StatusRow,0);
}




static int TerminateElf(void * ,BOOK *book)
{
  FreeBook(book);
  return(1);
}

typedef struct
{
  BOOK * book;
}MSG;

static int ShowAuthorInfo(void *mess ,BOOK *book)
{
  MSG * msg = (MSG *)mess;
  MessageBox(EMPTY_TEXTID,STR("BcfgExample"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}

static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    Timer_ReSet(&timer,REFRESH_TIME,onTimer,0);
    result=1;
  }
  return(result);
}

static void onMyBookClose(BOOK * book)
{
  DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusRow),SIonRedraw);
  Timer_Kill(&timer);
  SUBPROC(elf_exit);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,     TerminateElf,
  ELF_SHOW_INFO_EVENT,     ShowAuthorInfo,
  ELF_RECONFIG_EVENT,      ReconfigElf,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"BcfgExample_Base_Page",0,bk_msglst_base};


int main()
{ 
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook((BOOK*)myBook,onMyBookClose,&bk_base,"BcfgExample",-1,0))
  {
    delete myBook;
    SUBPROC(elf_exit);
    return (0);    
  }
  InitConfig();
  StatusRow=*StatusRow_p();
  SIonRedraw=DispObject_GetOnRedraw(StatusRow);
  DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusRow),Draw);
  MMIPROC((void (*)(void))onTimer);
  return 0;
}
