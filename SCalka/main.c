#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "main.h"
#include "calc_menu.h"
#include "calc_object.h"

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,     MainPageOnTerminate,
  NIL_EVENT_TAG,           NULL
};

const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    MainPageOnCreate,
  PAGE_EXIT_EVENT_TAG,     MainPageOnClose,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"SCalka_Base_Page",0,bk_msglst_base};
const PAGE_DESC bk_main = {"SCalka_Main_Page",0,bk_msglst_main};

#pragma segment="ELFBEGIN"
void elf_exit(void){
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}


void win12512unicode(wchar_t *ws, const char *s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    *ws++=c;
  }
  *ws=0;
}

char *unicode2win1251(char *s, wchar_t *ws, int len)
{
  char *d=s;
  int c;
  while((c=*ws++)&&((len--)>0))
  {
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    *s++=c;
  }
  *s=0;
  return(d);
}

// при закрытии книги
static void onMyBookClose(BOOK * book)
{
  SUBPROC(elf_exit);
}

int isScalkaBook(BOOK * struc)
{
  return(struc->onClose==onMyBookClose);
}

void CloseMyBook(BOOK * bk, void *)
{
  FreeBook(bk);
};

static int MainPageOnCreate(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  ReadCalcSettings();
  GUI * si=CreateCalkGUI(bk);
  GUIObject_Show(si);
  mbk->si=si;
  return (1);
}

static int MainPageOnClose(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  WriteCalcSettings();
  FREE_GUI(mbk->si);
  return (1);
}

static int MainPageOnTerminate(void *, BOOK *bk)
{
    FreeBook(bk);
    return 1;
}

int main()
{
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook((BOOK*)myBook,onMyBookClose,&bk_base,"SCalka",-1,0))
  {
    delete myBook;
    SUBPROC(elf_exit);
    return (0);    
  }
  BookObj_GotoPage((BOOK *)myBook,&bk_main);
  return 0;
}
