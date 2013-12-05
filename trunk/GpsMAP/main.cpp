#define USEPNG
#include "..\include\Lib_Clara.h"
#include "pngread.h"
#include "map_object.h"
#include "main.h"


#pragma segment="ELFBEGIN"
void elf_exit(void){
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}


void CloseMyBook( BOOK* bk, GUI* )
{
  FreeBook(bk);
}

//extern "C" void GUIObject_SetTitleType(GUI *gui, int type);
static int MainPageOnCreate(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  GUI * map_gui = CreateMapGUI(bk);
  GUIObject_SoftKeys_SetAction(map_gui,ACTION_BACK, CloseMyBook);
  GUIObject_SetTitleType(map_gui, 1);
  GUIObject_Show(map_gui);
  mbk->map_gui=map_gui;
  return (1);
}



const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  NIL_EVENT_TAG,           NULL
};

const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    MainPageOnCreate,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"GpsMAP_Base_Page",0,bk_msglst_base};
const PAGE_DESC bk_main = {"GpsMAP_Main_Page",0,bk_msglst_main};



// при закрытии книги
static void onMyBookClose(BOOK * book)
{
  SUBPROC(elf_exit);
}

int main()
{
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook((BOOK*)myBook,onMyBookClose,&bk_base,"GpsMAP",-1,0))
  {
    delete myBook;
    SUBPROC(elf_exit);
    return (0);    
  }
  BookObj_GotoPage((BOOK *)myBook,&bk_main);
  return 0;
}
