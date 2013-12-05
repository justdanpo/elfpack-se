#include "..\\..\\include\Lib_Clara.h"
#include "..\\..\\include\Dir.h"
#include "export\opendialog.h"

#include <stdarg.h>

int isDataBrowser1(BOOK * book, int * id)
{
  if (IsDataBrowserBook(book))
  {
    if (((DB_BOOK*)book)->bookID==*id)
    {
      return(1);
    }
  }
  return(0);
}

int OFD_ShowFilesFilter(const wchar_t *ExtList, const wchar_t *ItemPath, const wchar_t *ItemName)
{
  if (DataBrowser_isFileInListExt(ExtList,ItemPath,ItemName)) return(1);
  return(0);
}

int OFD_ShowFoldersFilter(const wchar_t *ExtList, const wchar_t *ItemPath, const wchar_t *ItemName)
{
  FSTAT fs;
  fstat(ItemPath,ItemName,&fs);
  if ((fs.st_mode&FSX_S_IFDIR)) return(1);
  return(0);
}

int OFD_ShowAllFilter(const wchar_t *ExtList, const wchar_t *ItemPath, const wchar_t *ItemName)
{
  FSTAT fs;
  if (DataBrowser_isFileInListExt(ExtList,ItemPath,ItemName)) return(1);
  fstat(ItemPath,ItemName,&fs);
  if ((fs.st_mode&FSX_S_IFDIR)) return(1);
  return(0);
}

static int ExplorerOnCreate(void *, BOOK *bk)
{
  OFD_BOOK * ofd = (OFD_BOOK *)bk;
  void * DB_Desc=DataBrowserDesc_Create();
  // ИД книги владельца браузера
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(bk));
  
  // Сканируемые папки
  DataBrowserDesc_SetFolders(DB_Desc,(const wchar_t **)ofd->folders_list->listdata);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,ofd->folders_list->FirstFree);
  
  // "Выбрать" на софткее
  DataBrowserDesc_SetSelectAction(DB_Desc,3);
  
  // заголовок окна
  DataBrowserDesc_SetHeaderText(DB_Desc,ofd->htext);
  
  // фильтр файлов
  DataBrowserDesc_SetFileExtList(DB_Desc,ofd->filters);
  
  // битики из flags
  if (ofd->flags & OFD_SELECT_FOLDER)       DataBrowserDesc_SetSelectActionOnFolders(DB_Desc,1);
  if (ofd->flags & OFD_NEW_FOLDERS_ENABLE)   DataBrowserDesc_SetOption(DB_Desc,"semc/dir");
  if (ofd->flags & OFD_EMPTY_FOLDER_ENABLE)  DataBrowserDesc_SetOpenEmptyFolder(DB_Desc,1);
  if (ofd->flags & OFD_INSERT_ON_OK_KEY)  
  {
    TEXTID str_id;
    textidname2id(L"GUI_INSERT_SK",TEXTID_ANY_LEN,&str_id);
    DataBrowserDesc_SetOKSoftKeyText(DB_Desc,str_id);
    DataBrowserDesc_SetSelectActionOnFolders(DB_Desc,1);
  }
  
  
  switch  (ofd->flags & (OFD_SHOW_FILES|OFD_SHOW_FOLDERS))
  {
  case OFD_SHOW_FILES:
    DataBrowserDesc_SetItemFilter(DB_Desc,OFD_ShowFilesFilter);
    break;
  case OFD_SHOW_FOLDERS:
    DataBrowserDesc_SetItemFilter(DB_Desc,OFD_ShowFoldersFilter);
    break;
  case (OFD_SHOW_FILES | OFD_SHOW_FOLDERS):
    DataBrowserDesc_SetItemFilter(DB_Desc,OFD_ShowAllFilter);
    break;
  }
  
  // ACTIONs
  ofd->actions= new char[17];
  
  char * pactions = ofd->actions;
  
  // а вот это наверное лучше в константы......
  if (ofd->flags & OFD_ACT_PREVIEW_ENABLE) *pactions++ = DB_CMD_RUN;
  if (ofd->flags & OFD_ACT_DELETE_ENABLE) *pactions++ = DB_CMD_DELETE;
  if (ofd->flags & OFD_ACT_INFO_ENABLE) *pactions++ = DB_CMD_INFO;
  
  *pactions = DB_CMD_LAST;
  
  DataBrowserDesc_SetActions(DB_Desc,ofd->actions);
  
  DataBrowser_Create(DB_Desc);
  
  int p=BookObj_GetBookID(bk);
  ofd->DataBrowserBook=FindBookEx(isDataBrowser1,&p);
  DataBrowserDesc_Destroy(DB_Desc);
  return(1);
}


static int ExplorerOnAccept(void *data, BOOK *bk)
{
  FILEITEM * fitem = (FILEITEM *)data;
  OFD_BOOK * ofd = (OFD_BOOK *)bk;
  UI_Event_toBookIDwData(ACCEPT_EVENT,ofd->parrent_book->BookID,FILEITEM_CreateCopy(fitem),(void(*)(void*))FILEITEM_Destroy_adr());
  FreeBook(bk);
  return(1);
}

static int ExplorerOnCancel(void *data, BOOK *bk)
{
  UI_Event_toBookID(CANCEL_EVENT,((OFD_BOOK *)bk)->parrent_book->BookID);
  FreeBook(bk);
  return(1);
}

const PAGE_MSG file_dialog_msglist[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    ExplorerOnCreate,
  ACCEPT_EVENT_TAG,        ExplorerOnAccept,
  CANCEL_EVENT_TAG,        ExplorerOnCancel,
  PREVIOUS_EVENT_TAG,      ExplorerOnCancel,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC ofd_file_dialog_msglist = {"Explorer_SelectFile_Page",0,file_dialog_msglist};



void onCloseOFDBook(BOOK * book)
{
  OFD_BOOK * ofd = (OFD_BOOK *)book;
  delete(ofd->filters);
  delete(ofd->actions);
  TextID_Destroy(ofd->htext);
  while (ofd->folders_list->FirstFree) 
  {
    delete(List_RemoveAt(ofd->folders_list,ofd->folders_list->FirstFree-1));
  }
  List_Destroy(ofd->folders_list);
}

// сообщение от БМ
int TerminateOFD(void * ,BOOK * book)
{
  OFD_BOOK * ofd = (OFD_BOOK *)book;
  
  UI_Event_toBookID(CANCEL_EVENT,ofd->parrent_bookID);
  
  FreeBook (ofd->DataBrowserBook);
  FreeBook(book);
  return(1);
}

// если убита родительская книга, то шабаш..
int PossibleDestroyParrentBook(void * book_id,BOOK * book)
{
  OFD_BOOK * ofd = (OFD_BOOK *)book;
  int * bookid = (int*)book_id;
  if (*bookid==ofd->parrent_bookID) 
  {
    FreeBook(book);
    FreeBook (ofd->DataBrowserBook);
  }
  return(1);
}

const PAGE_MSG ofd_base_msglst[] @ "DYN_PAGE"  =
{
  BOOK_DESTROYED_EVENT_TAG,      PossibleDestroyParrentBook,
  ELF_TERMINATE_EVENT ,          TerminateOFD,
  NIL_EVENT_TAG,                 NULL
};

const PAGE_DESC odf_base = {"Explorer_Base_Page",0,ofd_base_msglst};

wchar_t * CreateFileDialog (
                            BOOK * book,
                            int flags,
                            wchar_t * header,
                            wchar_t * filters,
                            va_list ap
                              )
{
  
  
  int args;
  wchar_t * ws;
  
  OFD_BOOK  * ofd = new OFD_BOOK;
  CreateBook((BOOK*)ofd,onCloseOFDBook,&odf_base,"Fantom_ExplorerBook",-1,0);
  ofd->parrent_book=book;
  ofd->parrent_bookID=BookObj_GetBookID(book);
  ofd->filters=new wchar_t[wstrlen(filters)+1];
  wstrcpy(ofd->filters,filters);
  
  // проверяем, не TEXTID ли?
  if (((int)header>0x70000000) || ((int)header<0x10000) )
  {
    ofd->htext=(int)header;
  }
  else
  {
    ofd->htext=TextID_Create(header,ENC_UCS2,TEXTID_ANY_LEN);
  }
  ofd->flags=flags;
  ofd->DataBrowserBook=0;
  ofd->folders_list=List_Create();
  
  ofd->folders=0;
  
  // создаём список папок
  args = va_arg(ap, int);
  
  while (args>=0)
  {
    if (args>0x1000)
    {
      ws = (wchar_t *)args;
    }
    else
    {
      ws = GetDir(args);
    }
    wchar_t * wsnew= new wchar_t[wstrlen(ws)+1];
    wstrncpy(wsnew,ws,wstrlen(ws)+1);
    List_InsertLast(ofd->folders_list,wsnew);
    args = va_arg(ap, int);
    ofd->folders++;
  }
 
  BookObj_GotoPage((BOOK*)ofd,&ofd_file_dialog_msglist);
  return(0);
}

