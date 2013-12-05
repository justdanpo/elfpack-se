#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"

typedef struct
{
  BOOK book;
  GUI * TextInput;
  GUI * NameInput;
  GUI_LIST * list;
  GUI_ONEOFMANY * oom_list;
  wchar_t * w_string;
  char * string;
  wchar_t * FileName;
  int length;
  int coding;
}MyBOOK;

typedef struct
{
  BOOK book;
  int bookID;
}DB_BOOK;

typedef struct
{
  BOOK * book;
}MSG;

void CreateNameInput(int editable_strID,u16 curs_pos,char hz);
MyBOOK * CreateFileBook;

char * unicode2win1251(char *s, wchar_t *ws, int len)
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

void ExitAction(BOOK *)
{
  FreeBook((BOOK*)CreateFileBook);
}

int TerminateElf(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("CreateTXTFile, v.1.2\r\n\r\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}

void DestroyList(BOOK * bk, GUI* )
{
  if (CreateFileBook->list)
  {
    GUIObject_Destroy(CreateFileBook->list);
    CreateFileBook->list=0;
  }
}

void DestroyOOM(BOOK * bk, GUI* )
{
  if (CreateFileBook->oom_list)
  {
    GUIObject_Destroy(CreateFileBook->oom_list);
    CreateFileBook->oom_list=0;
  }
}

int DestroyNameInput()
{
  if (CreateFileBook->NameInput)
  {
    GUIObject_Destroy(CreateFileBook->NameInput);
    CreateFileBook->NameInput=0;
  }
  return(0);
}

int DestroyTextInput()
{
  if (CreateFileBook->TextInput)
  {
    GUIObject_Destroy(CreateFileBook->TextInput);
    CreateFileBook->TextInput=0;
  }
  return(0);
}

int onAccept(void * data, BOOK *)
{
  if (fstat( FILEITEM_GetPath((FILEITEM*)data), CreateFileBook->FileName, 0)>=0)
  {
    int str_id;
    textidname2id(L"DB_NAME_EXIST_TXT",TEXTID_ANY_LEN,&str_id);
    MessageBox(EMPTY_TEXTID,str_id, NOIMAGE, 1, 5000,0);
    DestroyList(0,0);
  }
  else
  {
    int f;
    int len;
    if ( (f=_fopen( FILEITEM_GetPath((FILEITEM*)data), CreateFileBook->FileName, FSX_O_WRONLY, FSX_S_IREAD|FSX_S_IWRITE, 0)) >=0 )
    {
      void * str_buf;
      if (CreateFileBook->coding)
      {
        str_buf=CreateFileBook->w_string;
        len=CreateFileBook->length*2+2;
      }
      else
      {
        str_buf=CreateFileBook->string;
        len=CreateFileBook->length;
      }
      if (fwrite(f,str_buf,len)!=len)
      {
        MessageBox(EMPTY_TEXTID,STR("Write File Error!!!"), NOIMAGE, 1, 5000,0);
      }
      fclose(f);
      FreeBook((BOOK*)CreateFileBook);
    }
    else
    {
      MessageBox(EMPTY_TEXTID,STR("Can't open file. Try later."), NOIMAGE, 1, 5000,0);
    }
  }
  return(0);
}

void CreateBrowser(wchar_t const ** folder_list)
{
  void * DB_Desc=DataBrowserDesc_Create();
  char option[20];
  strcpy(option,"semc/dir");
  int str_id;
  textidname2id(L"DB_OTHER_TXT",TEXTID_ANY_LEN,&str_id);
  DataBrowserDesc_SetHeaderText(DB_Desc,str_id);
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID((BOOK*)CreateFileBook));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,1);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowserDesc_SetSelectActionOnFolders(DB_Desc,1);
  DataBrowserDesc_SetOption(DB_Desc,option);
  DataBrowserDesc_SetOpenEmptyFolder(DB_Desc,1);
  textidname2id(L"GUI_INSERT_SK",TEXTID_ANY_LEN,&str_id);
  DataBrowserDesc_SetOKSoftKeyText(DB_Desc,str_id);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
}


void ExitActionList(BOOK * bk, GUI* )
{
  FreeBook((BOOK*)CreateFileBook);
}


void onEnter(BOOK * bk, GUI* )
{
  if (ListMenu_GetSelectedItem(CreateFileBook->list))
  {
    const wchar_t * folder_list[2];
    folder_list[0]=GetDir(DIR_OTHER | MEM_EXTERNAL);
    folder_list[1]=0;
    CreateBrowser(folder_list);
  }
  else
  {
    const wchar_t * folder_list[2];
    folder_list[0]=GetDir(DIR_OTHER | MEM_INTERNAL);
    folder_list[1]=0;
    CreateBrowser(folder_list);
  }
}


int list_callback(GUI_MESSAGE * msg)
{
  int str_id;
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    if (GUIonMessage_GetCreatedItemIndex(msg))
    {
      textidname2id(L"CAMI_HZ_SAVE_TO_MS_TXT",TEXTID_ANY_LEN,&str_id);
      GUIonMessage_SetMenuItemText(msg,str_id);
      iconidname2id(L"DB_2ROW_MS_ICN",TEXTID_ANY_LEN,&str_id);
      GUIonMessage_SetMenuItemIcon(msg,0,str_id);
    }
    else
    {
      textidname2id(L"CAMI_HZ_SAVE_TO_PHONE_TXT",TEXTID_ANY_LEN,&str_id);
      GUIonMessage_SetMenuItemText(msg,str_id);
      iconidname2id(L"DB_2ROW_PHONE_ICN",TEXTID_ANY_LEN,&str_id);
      GUIonMessage_SetMenuItemIcon(msg,0,str_id);
    }

  }
  return(1);
};


void CreateList(void)
{
  GUI_LIST * list_menu=CreateListMenu((BOOK*)CreateFileBook,0);
  CreateFileBook->list=list_menu;
  int str_id=EMPTY_TEXTID;
  textidname2id(L"MSG_UI_MOVE_MESSAGE_SELECT_FOLDER_TXT",TEXTID_ANY_LEN,&str_id);
  if (str_id==EMPTY_TEXTID) str_id=TextID_Create("Select folder",ENC_LAT1,TEXTID_ANY_LEN);
  GUIObject_SetTitleText(list_menu,str_id);
  ListMenu_SetItemCount(list_menu,2);
  OneOfMany_SetOnMessage(list_menu,list_callback);
  ListMenu_SetCursorToItem(list_menu,0);
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_BACK,DestroyList);
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_LONG_BACK,ExitActionList);
  GUIObject_SoftKeys_SetAction(list_menu,ACTION_SELECT1,onEnter);
  GUIObject_Show(list_menu);
};

void oom_onEnter(BOOK * bk, GUI* )
{
  CreateFileBook->coding=OneOfMany_GetSelected(CreateFileBook->oom_list);
  CreateList();
}

int oom_callback(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    if (GUIonMessage_GetCreatedItemIndex(msg))
    {
      GUIonMessage_SetMenuItemText(msg,STR("Unicode"));
    }
    else
    {
      GUIonMessage_SetMenuItemText(msg,STR("ASCII"));
    }
  }
  return(1);
};

void CreateOOMList(void)
{
  GUI_ONEOFMANY * oom_menu=CreateOneOfMany((BOOK*)CreateFileBook);
  CreateFileBook->oom_list=oom_menu;
  int str_id=EMPTY_TEXTID;
  textidname2id(L"ES_TEXT_FORMAT_TXT",TEXTID_ANY_LEN,&str_id);
  if (str_id==EMPTY_TEXTID) str_id=TextID_Create("Text format",ENC_LAT1,TEXTID_ANY_LEN);
  GUIObject_SetTitleText(oom_menu,str_id);
  OneOfMany_SetItemCount(oom_menu,2);
  OneOfMany_SetOnMessage(oom_menu,oom_callback);
  OneOfMany_SetChecked(oom_menu,0);
  GUIObject_SoftKeys_SetAction(oom_menu,ACTION_BACK,DestroyOOM);
  GUIObject_SoftKeys_SetAction(oom_menu,ACTION_LONG_BACK,ExitActionList);
  GUIObject_SoftKeys_SetAction(oom_menu,ACTION_SELECT1,oom_onEnter);
  GUIObject_SoftKeys_SetAction(oom_menu,0xBC3,oom_onEnter);
  GUIObject_Show(oom_menu);
};

const PAGE_MSG CreateFile_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  CANCEL_EVENT_TAG, TerminateElf,
  ACCEPT_EVENT_TAG, onAccept,
  NIL_EVENT_TAG,0
};


PAGE_DESC base_page ={"CreateFile_BasePage",0,CreateFile_PageEvents};


void AcceptAction_FileName(BOOK *,wchar_t * string,int len)
{
  if (FSX_GetInvalidChar(string))
  {
    int str_id;
    textidname2id(L"DB_ILLEGAL_CHAR_TXT",TEXTID_ANY_LEN,&str_id);
    MessageBox(EMPTY_TEXTID,str_id, NOIMAGE, 0, 2000,0);
    FILEITEM * item_desc=FILEITEM_Create();
    wchar_t fname[40];
    memset(fname,0,sizeof(fname));
    wchar_t * extpos = wstrrchr(string,L'.');
    wstrncpy(fname, string, extpos-string);
    FILEITEM_SetFname(item_desc, fname);
    FSX_RemoveIllegalSymbolsName(item_desc);
    wchar_t sp[50];
    sp[0]=0;
    wstrcpy(sp, FILEITEM_GetFname(item_desc));
    wstrcat(sp, extpos);
    StringInput_DispObject_SetText(GUIObject_GetDispObject(CreateFileBook->NameInput),TextID_Create(sp,ENC_UCS2,TEXTID_ANY_LEN));
    FILEITEM_Destroy(item_desc);
  }
  else
  {
    if (CreateFileBook->FileName) delete(CreateFileBook->FileName);
    CreateFileBook->FileName=new wchar_t[len+1];
    wstrcpy(CreateFileBook->FileName,string);
    CreateOOMList();
  }
}

void CreateNameInput(int editable_strID,u16 cursor_pos,char hz)
{
  int str_id;
  textidname2id(L"MSG_UI_FILENAMEP_TXT",TEXTID_ANY_LEN,&str_id);
  DestroyNameInput();
  CreateFileBook->NameInput=CreateStringInputVA(0,
                                                         VAR_PREV_ACTION_PROC(DestroyNameInput),
                                                         VAR_LONG_BACK_PROC(ExitAction),
                                                         VAR_BOOK(CreateFileBook),
                                                         VAR_OK_PROC(AcceptAction_FileName),
                                                         VAR_STRINP_FIXED_TEXT(str_id),
                                                         VAR_STRINP_TEXT(editable_strID),
                                                         VAR_STRINP_MIN_LEN(1),
                                                         VAR_STRINP_MAX_LEN(39),
                                                         0);
  StringInput_SetCursorPosition(CreateFileBook->NameInput,cursor_pos,hz);
}

void AcceptAction_String(BOOK *,wchar_t * string,int len)
{
  CreateFileBook->length=len;
  if (CreateFileBook->w_string) delete(CreateFileBook->w_string);
  CreateFileBook->w_string=new wchar_t[len+2];
  CreateFileBook->w_string[0]=0xFEFF;
  wstrcpy(CreateFileBook->w_string+1,string);
  if (CreateFileBook->string) delete(CreateFileBook->string);
  CreateFileBook->string=new char[len+1];
  unicode2win1251(CreateFileBook->string,string,len);
  CreateNameInput(STR(".txt"),0,1);
}

void elf_exit(void)

{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int isMyDataBrowser(BOOK * book)
{
  if (IsDataBrowserBook(book))
  {
    if (((DB_BOOK*)book)->bookID==BookObj_GetBookID((BOOK*)CreateFileBook))
    {
      return(1);
    }
  }
  return(0);
}

void onCloseCreateFileBook(BOOK *)
{
  if (CreateFileBook)
  {
    BOOK * bk=FindBook(isMyDataBrowser);
    if (bk) FreeBook(bk);
    if (CreateFileBook->w_string) delete(CreateFileBook->w_string);
    if (CreateFileBook->string) delete(CreateFileBook->string);
    if (CreateFileBook->FileName) delete(CreateFileBook->FileName);
    DestroyList(0,0);
    DestroyOOM(0,0);
    DestroyNameInput();
    DestroyTextInput();
    SUBPROC(elf_exit);
  }
}


int CreateCreateFileBook()
{
  CreateFileBook = new MyBOOK;
  CreateBook((BOOK*)CreateFileBook,onCloseCreateFileBook,&base_page,"CreateTXTFile",-1,0);
  CreateFileBook->NameInput=0;
  CreateFileBook->w_string=0;
  CreateFileBook->string=0;
  CreateFileBook->FileName=0;
  CreateFileBook->list=0;
  CreateFileBook->oom_list=0;
  int str_id;
  textidname2id(L"BL_BLOG_TEXTP_TXT",TEXTID_ANY_LEN,&str_id);
  CreateFileBook->TextInput=CreateStringInputVA(0,
                                              VAR_PREV_ACTION_PROC(ExitAction),
                                              VAR_LONG_BACK_PROC(ExitAction),
                                              VAR_BOOK(CreateFileBook),
                                              VAR_STRINP_FIXED_TEXT(str_id),
                                              VAR_STRINP_MIN_LEN(1),
                                              VAR_STRINP_MAX_LEN(1024),
                                              VAR_OK_PROC(AcceptAction_String),
                                              0);
  return(0);
}


int isCreateFileBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"CreateTXTFile")) return(1);
  return(0);
}


int main (void)
{
  if (!FindBook(isCreateFileBook))
  {
    CreateCreateFileBook();
    return(0);
  }
  else
  {
    MessageBox(EMPTY_TEXTID,STR("Already runed"), NOIMAGE, 1, 5000,0);
    SUBPROC(elf_exit);
  }
}
