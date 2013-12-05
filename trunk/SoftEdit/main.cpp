#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"

int readcustomcfg(wchar_t *path, wchar_t *name);
MyBOOK *CreateOurBook();
void elf_exit(void);

extern const PAGE_DESC bk_main;

int isSoftEditBook(BOOK *bk);

int main()
{
  MyBOOK *myBook=CreateOurBook();
  if (myBook)
  {
    readcustomcfg(GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG), L"AdvSoftkeys.cfg");
    BookObj_GotoPage(&myBook->book,&bk_main);
  }
  else
  {
    SUBPROC(elf_exit);
    return 0;
  }
  return 0;
};
