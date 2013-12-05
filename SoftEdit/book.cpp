#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"
#include "revision.h"
int isSBook(BOOK *);
void destroylists();
#define SID_NULL 0x6FFFFFFF

int isSBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"Softkeys")==0)return 1;
 return 0;
}

int isSoftEditBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"SoftEdit")==0)return 1;
  return 0;
};

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

static void onMyBookClose(BOOK * book)
{
  SKBOOK *sk=(SKBOOK*)FindBook(isSBook);
  if (sk)
  {
    if (sk->version==VERSION_)
    {
      sk->SetWorkState(true);
    }
  }
  destroylists();
  SUBPROC(elf_exit);
};

static int TerminateElf(void *mess ,BOOK *book)
{
  FreeBook(book);
  return(1);
};

static int ShowAuthorInfo(void *mess ,BOOK *book)
{
  MSG * msg = (MSG *)mess;
  wchar_t text[100];
  snwprintf(text,99,L"SoftEdit\n© UltraShot\n\nrevision %d", __SVN_REVISION__ );
  MessageBox(SID_NULL,TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000,msg->book);
  return(1);
};

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,     TerminateElf,
  ELF_SHOW_INFO_EVENT,     ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"SoftEdit_Base_Page",0,bk_msglst_base};

MyBOOK *CreateOurBook()
{
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook(&myBook->book,onMyBookClose,&bk_base,"SoftEdit",-1,0))
  {
    delete(myBook);
    return 0;
  }
  return myBook;
};
