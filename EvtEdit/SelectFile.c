#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\externals.h"

#define MUSIC 1
#define ELF 2
#define IMAGE 3
#define EVT 4
#define THM 5
int onPrevious_MainMenu_DB(void *data, BOOK * book);
int onCancel_MainMenu_DB(void *data, BOOK * book);
int isEvtEditBook(BOOK * struc);
void kickbadsymbols(wchar_t *string, int len);

int isSupportable(wchar_t const *fpath, wchar_t const *fname, int type)
{
  if (type==MUSIC)
  {
    if (wstrcmpni(L"m4a",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"wma",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"mp3",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
  }
  else if (type==ELF)
  {
    if (wstrcmpni(L"elf",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
  }
  else if (type==IMAGE)
  {
    if (wstrcmpni(L"png",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"gif",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"bmp",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"jpg",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
    else if (wstrcmpni(L"jpeg",&fname[wstrlen(fname)-4],4)==0)
    {
      return 1;
    }
  }
  else if (type==EVT)
  {
    if (wstrcmpni(L"evt",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
  }
  else if (type==THM)
  {
    if (wstrcmpni(L"thm",&fname[wstrlen(fname)-3],3)==0)
    {
      return 1;
    }
  }
  return 0;
};

int DB_Filter(const wchar_t * ext_table,const wchar_t * path ,const wchar_t * name)
{
  MyBOOK *mbk=(MyBOOK*)FindBook(isEvtEditBook);
  if (isSupportable(path,name,mbk->fType)) return 1;
  FSTAT _fstat;
  fstat(path,name,&_fstat);
  if ((_fstat.st_mode&FSX_S_IFDIR)) return(1);
  return(0);
};

int onAccept_DB(void * data, BOOK * book)
{
  wchar_t *full=new wchar_t[wstrlen( FILEITEM_GetPath((FILEITEM*)data) )+1+wstrlen( FILEITEM_GetFname((FILEITEM*)data) )+1];
  wstrcpy(full, FILEITEM_GetPath((FILEITEM*)data) );
  wstrcat(full,L"/");
  wstrcat(full, FILEITEM_GetFname((FILEITEM*)data) );
  MyBOOK *mbk=(MyBOOK*)book;
  kickbadsymbols(full,wstrlen(full));
  if (mbk->selectfiletype==2)
  {
    DELETE(mbk->indpath);
    DELETE(mbk->indname);
    mbk->indpath=new wchar_t[wstrlen( FILEITEM_GetPath((FILEITEM*)data) )+1];
    wstrcpy(mbk->indpath, FILEITEM_GetPath((FILEITEM*)data) );
    mbk->indname=new wchar_t[wstrlen( FILEITEM_GetFname((FILEITEM*)data) )+1];
    wstrcpy(mbk->indname, FILEITEM_GetFname((FILEITEM*)data) );
    BookObj_ReturnPage(book,ACCEPT_EVENT);
  }
  else if (mbk->selectfiletype==1)
  {
    DELETE(mbk->selev->param);
    mbk->selev->param=new wchar_t[wstrlen(full)+4+1];
    snwprintf(mbk->selev->param,wstrlen(full)+4+1,L"%03d.%ls", mbk->tempvol, full);
    BookObj_ReturnPage(book,ACCEPT_EVENT);
    BookObj_CallPage(book,&bk_main);
  }
  else
  {
    DELETE(mbk->selev->param);
    mbk->selev->param=new wchar_t[wstrlen(full)+1];
    wstrcpy(mbk->selev->param,full);
    BookObj_ReturnPage(book,ACCEPT_EVENT);
    BookObj_CallPage(book,&bk_main);
  }
  delete(full);
  return(0);
};

int CreateDB(void *data, BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  folder_list[0]=GetDir(mbk->dir1);
  folder_list[1]=GetDir(mbk->dir2);
  folder_list[2]=0;
  DataBrowserDesc_SetHeaderText(DB_Desc,GetStrID(L"DB_BYNAME_TXT"));
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(FindBook(isEvtEditBook)));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetItemFilter(DB_Desc,DB_Filter);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);

  return(0);
};

const PAGE_MSG SelectFile_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateDB,
  ACCEPT_EVENT_TAG,onAccept_DB,
  PREVIOUS_EVENT_TAG,onPrevious_MainMenu_DB,
  CANCEL_EVENT_TAG,onCancel_MainMenu_DB,
  NIL_EVENT_TAG,0
};

const PAGE_DESC SelectFile_page ={"EvtEdit_SelectFile_Page",0,SelectFile_PageEvents};
