#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

DISP_OBJ * StatusIndication;
int Height,Width;
u16 ftimer;

int _x=100;
int _y=100;
int _dx=2;
int _dy=3;

void (*SIonRedraw)(DISP_OBJ *,int r1,int r2,int r3);
void Draw(DISP_OBJ * db,int r1, int r2,int r3);

typedef struct
{
  BOOK * book;
}MSG;

void Send_REDRAW_RELEASE()
{
  // посылаем всем  мессагу со старым и своим методами Redraw
  REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
  memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
  sbm->SB_OldOnRedraw=SIonRedraw;
  sbm->SB_NewOnRedraw=Draw;
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
}

int TerminateSBDraw(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("StandBy Draw test..."), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


int SB_ELF_Killed(void *mess ,BOOK* book)
{
  // если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
  REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
  // его ли метод мы используем в качестве oldRedraw?
  if (sbm->SB_NewOnRedraw==SIonRedraw)
  {
    REDRAW_RELEASE_MESSAGE *res=new REDRAW_RELEASE_MESSAGE;
    
    // скопируем существующие методы в нашу мессагу
    memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    
    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) SIonRedraw=sbm->SB_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication),Draw);

    // и шлём мессагу снова, чтоб следующие эльфы сделали тоже самое
    res->SB_OldOnRedraw=EMPTY_REDRAW_METHOD;
    res->SB_NewOnRedraw=Draw;
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,res,(void (*)(void*))mfree_adr());
    return BLOCK_EVENT_GLOBALLY;
  }
  return 0;
}



const PAGE_MSG SBDraw_PageEvents[]@ "DYN_PAGE" ={
  SBY_REDRAW_RELEASE_EVENT  , SB_ELF_Killed,
  ELF_TERMINATE_EVENT       , TerminateSBDraw,
  ELF_SHOW_INFO_EVENT       , ShowAuthorInfo,
  NIL_EVENT_TAG             , NULL

};

PAGE_DESC base_page ={"SBDraw_BasePage",0,SBDraw_PageEvents};

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onfTimer (u16 tmr , LPARAM)
{
  DispObject_InvalidateRect(StatusIndication,0);
  Timer_ReSet(&ftimer,20,onfTimer,0);
}

void Draw(DISP_OBJ * db,int r1, int r2,int r3)
{
  // вызываем родной onRedraw.
  SIonRedraw(db,r1,r2,r3);

  DrawRect(_x,_y,_x+10,_y+10,clRed,clRed);
  if ((_x>Width-11) && (_dx>0)) _dx=0-_dx;
  if ((_x<9) && (_dx<0)) _dx=0-_dx;
  if ((_y>Height-50) && (_dy>0)) _dy=0-_dy;
  if ((_y<20) && (_dy<0)) _dy=0-_dy;
  _x+=_dx;
  _y+=_dy;
}

void onCloseMyBook(BOOK * book)
{
  if (book)
  {
    Timer_Kill(&ftimer);
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication),SIonRedraw);
    SUBPROC(elf_exit);
  }
}

BOOK * CreateSBDBook()
{
  BOOK * SBDBook= new BOOK;
  CreateBook(SBDBook,onCloseMyBook,&base_page,"SBDraw_Test",-1,0);
  return(SBDBook);
}

//
// Исключительно пример рисования на StandBy.
// запускаем несколько копий эльфа. после запуска каждой необходимо выйти на StandBy.
//
// для рисования заменяем OnRedraw у StatusIndication на свой. старый вызываем в своём методе.
// при завершении работы посылаем сообщение(см.Send_REDRAW_RELEASE())
// так же сами обязательно обрабатываем SBY_REDRAW_RELEASE_EVENT
//
int main (void)
{
  Height=Display_GetHeight(0);
  Width=Display_GetWidth(0);;
  CreateSBDBook();

  StatusIndication=GUIObject_GetDispObject ( SBY_GetStatusIndication(Find_StandbyBook()) );
  SIonRedraw=DispObject_GetOnRedraw(StatusIndication);
  DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication),Draw);
  ftimer=Timer_Set(20,onfTimer,0);
  return(0);
}


