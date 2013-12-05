#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "structs.h"
#include "untar.h"
#include "revision.h"

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

static void onMyBookClose(BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  if (mbk->path)delete(mbk->path);
  if (mbk->name)delete(mbk->name);
  SUBPROC(elf_exit);
};

typedef struct
{
  BOOK * book;
}MSG;

int isThmUnpackerBook(BOOK * struc)
{
  return(struc->onClose == onMyBookClose);
};

static int TerminateElf(void *mess ,BOOK *book)
{
  BookObj_ReturnPage(book,CANCEL_EVENT);
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
  FreeBook(book);
  return(1);
};

#define SID_NULL 0x6FFFFFFF

static int ShowAuthorInfo(void *mess ,BOOK *book)
{
  MSG * msg = (MSG *)mess;
  wchar_t ustr[64];
  snwprintf(ustr,MAXELEMS(ustr)-1,L"\nThemeUnpacker\n© UltraShot\n\nrevision %d", __SVN_REVISION__ );
  MessageBox(SID_NULL,TextID_Create(ustr,ENC_UCS2,MAXELEMS(ustr)-1), NOIMAGE, 1, 5000,msg->book);
  return(1);
};

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,     TerminateElf,
  ELF_SHOW_INFO_EVENT,     ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"ThmUnpacker_base",0,bk_msglst_base};  wchar_t ustr[64];

int isSupportable(wchar_t const *fpath, wchar_t const *fname)
{
  if (wstrcmpni(L"thm",&fname[wstrlen(fname)-3],3)==0)
  {
    return 1;
  }
  return 0;
}

int DB_Filter(const wchar_t * ext_table,const wchar_t * path ,const wchar_t * name)
{
  MyBOOK *mbk=(MyBOOK*)FindBook(isThmUnpackerBook);
  if (isSupportable(path,name)) return 1;
  FSTAT _fstat;
  fstat(path,name,&_fstat);
  if ((_fstat.st_mode&FSX_S_IFDIR)) return(1);
  
  return(0);
}
extern const PAGE_DESC bk_main;

int onAccept_DB(void * data, BOOK * book)
{
  int res=tar_uncompress( FILEITEM_GetPath((FILEITEM*)data), FILEITEM_GetFname((FILEITEM*)data) );
  wchar_t ustr[64];
  if (res>0)
  {
    snwprintf(ustr,MAXELEMS(ustr)-1,L"Unpacked\nTotal files: %d", res);
  }
  else
  {
    snwprintf(ustr,MAXELEMS(ustr)-1,L"Unpacking failed\nTotal files: %d", res);
  }
  MessageBox(SID_NULL,TextID_Create(ustr,ENC_UCS2,MAXELEMS(ustr)-1), NOIMAGE, 1, 5000,0);
  FreeBook(book);
  return(0);
};

static int CreateDB(void *data, BOOK * book)
{
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  folder_list[0]=GetDir(MEM_INTERNAL+DIR_THEME);
  folder_list[1]=GetDir(MEM_EXTERNAL+DIR_THEME);
  folder_list[2]=0;
  DataBrowserDesc_SetHeaderText(DB_Desc,STR("Select theme"));
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetItemFilter(DB_Desc,DB_Filter);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);

  return(0);
};

static int onCancelDB(void *data, BOOK * book)
{
  FreeBook(book);
  return(1);
};

const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,CreateDB,
  ACCEPT_EVENT_TAG,onAccept_DB,
  PREVIOUS_EVENT_TAG,onCancelDB,
  CANCEL_EVENT_TAG,onCancelDB,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_main = {"ThmUnpacker_main",0,bk_msglst_main};
extern const PAGE_DESC bk_gui;

int main(wchar_t *epath, wchar_t *path, wchar_t *name)
{
  MyBOOK * myBook=new MyBOOK;
  memset(myBook,0,sizeof(MyBOOK));
  if (!CreateBook(&myBook->book,onMyBookClose,&bk_base,"ThemeUnpacker",-1,0))
  {
    delete myBook;
    SUBPROC(elf_exit);
    return (0);
  }
  if (path && name)
  {
    myBook->path=new wchar_t[wstrlen(path)+1];
    wstrcpy(myBook->path,path);
    myBook->name=new wchar_t[wstrlen(name)+1];
    wstrcpy(myBook->name,name);
    BookObj_GotoPage(&myBook->book,&bk_gui);
  }
  else
  {
    BookObj_GotoPage(&myBook->book,&bk_main);
  }
  return 0;
};
