#include "main.h"


LIST * DbExtData;
FILESUBROUTINE * FileSub;
wchar_t * CfgFile;
int MEM_EXT=0;
//from epd
LIST * epd_DBExtList;
DB_EXT * (*epd_CreateDbExt)(void);

//------------------------------------------ DbExtData
DB_EXT_DATA * CreateDbExtData(wchar_t * ext)
{
  DB_EXT_DATA * db_ext_data = new DB_EXT_DATA;
  memset(db_ext_data, 0x0, sizeof(DB_EXT_DATA));
  db_ext_data->ext = new wchar_t[wstrlen(ext)+1];
  wstrcpy(db_ext_data->ext, ext);
  wstrnlwr(db_ext_data->ext, TEXTID_ANY_LEN);
  InitIcon(&db_ext_data->icon[small]);
  InitIcon(&db_ext_data->icon[big]);
  InitIcon(&db_ext_data->icon[saved]);
  wchar_t * e = GetFileExtension(ext);
  if (e)
  {
    db_ext_data->isfile = 1;
    db_ext_data->path_int = 0;
    db_ext_data->path_ext = 0;
  }
  else
  {
    SetWstr(&db_ext_data->path_int, GetDir(DIR_OTHER|MEM_INTERNAL));
    if (MEM_EXT) SetWstr(&db_ext_data->path_ext, GetDir(DIR_OTHER|MEM_EXTERNAL));
  }
  return db_ext_data;
}

int DeleteDbExtData(DB_EXT_DATA * db_ext_data)
{
  if (db_ext_data->ext) delete db_ext_data->ext;
  if (db_ext_data->elf_open) delete db_ext_data->elf_open;
  if (db_ext_data->elf_edit) delete db_ext_data->elf_edit;
  FreeIcon(&db_ext_data->icon[small]);
  FreeIcon(&db_ext_data->icon[big]);
  FreeIcon(&db_ext_data->icon[saved]);
  if (db_ext_data->path_int) delete db_ext_data->path_int;
  if (db_ext_data->path_ext) delete db_ext_data->path_ext;
  DB_EXT * db_ext = db_ext_data->db_ext;
  if (db_ext)
  {
    int index = List_IndexOf(epd_DBExtList, db_ext_data->db_ext);
    List_RemoveAt(epd_DBExtList, index);
    if (db_ext->content_type)
    {
      delete db_ext->content_type[0];
      delete db_ext->content_type;
    }
    if (db_ext->ext_list) delete db_ext->ext_list;
    if (db_ext->dbf) delete db_ext->dbf;
    delete db_ext;
  }
  return 1;
}
//------------------------------------------ Create Folders
DB_EXT_FOLDERS * CreateFolders(DB_EXT_DATA * db_ext_data)
{
  DB_EXT_FOLDERS * dbfolders;
  int next;
  if (MEM_EXT && db_ext_data->path_ext)
  {
    dbfolders= new DB_EXT_FOLDERS[3];
    textidname2id(IDN_FOLDERNAME, -1, &dbfolders[0].StrID_FolderName);
    textidname2id(IDN_SAVED_ON_MS, -1, &dbfolders[0].StrID_SavedTo);
    dbfolders[0].Path = db_ext_data->path_ext;
    dbfolders[0].isInternal = 0;
    next = 1;
  }
  else
  {
    dbfolders = new DB_EXT_FOLDERS[2];
    next = 0;
  }
  textidname2id(IDN_FOLDERNAME, -1, &dbfolders[next].StrID_FolderName);
  textidname2id(IDN_SAVED_ON_PH, -1, &dbfolders[next].StrID_SavedTo);
  dbfolders[next].Path = db_ext_data->path_int;
  dbfolders[next].isInternal = 1;
  next++;
  dbfolders[next].StrID_FolderName = EMPTY_TEXTID;
  dbfolders[next].StrID_SavedTo = EMPTY_TEXTID;
  dbfolders[next].Path = NULL;
  dbfolders[next].isInternal = NULL;
  return dbfolders;
}

int ExtDataCompare(void * ext1, void * ext2)
{FUNCTION
  wchar_t * E1 = ((DB_EXT_DATA*)ext1)->ext;
  wchar_t * E2 = ((DB_EXT_DATA*)ext2)->ext;
  wchar_t * e1 = GetFileExtension(E1);
  wchar_t * e2 = GetFileExtension(E2);
  if (!e1)//если вставляем расширение
  {
    if (!e2)//если сравниваем с расширением
    {
      return wstrcmpi(E1, E2);//то сравниваем их
    }
    else return 1;//иначе сюда нам точно вставлять не надо
  }
  else//если вставляем файл
  {
    if (!e2)//сравниваем с расширением
    {
      if (wstrcmpi(e1, E2)>=0)//сравнили расширение файла и расширение
      {
        return 1;
      }
      else
      {
        return -1;
      }
    }
    else
    {
      if (!wstrcmpi(e1, e2))//если расширения файлов равны
      {
        return wstrcmpi(E1, E2);//то сравниваем файлы
      }
      else return 1;
    }
  }
}


void epd_processing(void)
{
  EP_DATA * epd = (EP_DATA *)get_envp(get_bid(current_process()), "elfpackdata");
  epd_DBExtList = epd->DBExtList;
  epd_CreateDbExt = epd->CreateDbExt;
}

DB_EXT_DATA * SUB_EXECUTE_GetDB_EXT_DATA(SUB_EXECUTE * sub_execute)
{
  wchar_t * w = FILEITEM_GetFname(sub_execute->file_item);
  wchar_t * e = wstrrchr(w, '.') + 1;
  int count = DbExtData->FirstFree;
  DB_EXT_DATA * res = 0;
  for (int i = 0; i<count; i++)
  {
    DB_EXT_DATA * db_ext_data = (DB_EXT_DATA*)List_Get(DbExtData, i);
    if (!wstrcmpi(w, db_ext_data->ext))
    {
      return db_ext_data;
    }
    else if (!wstrcmpi(e, db_ext_data->ext))
    {
      res = db_ext_data;
    }
  }
  return res;
}

__root void GetMethods(SUB_EXECUTE * sub_execute)
{
  sub_execute->filesub = FileSub;
}

//------------------------------------------ page run & edit
int FileOpen_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{
  SUB_EXECUTE * sub_execute = BrowserItem_Get_SUB_EXECUTE(book);
  DB_EXT_DATA * ext = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  elfload(ext->elf_open,FILEITEM_GetPath(sub_execute->file_item),FILEITEM_GetFname(sub_execute->file_item),0);
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return(1);
}
int FileEdit_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{
  SUB_EXECUTE * sub_execute = BrowserItem_Get_SUB_EXECUTE(book);
  DB_EXT_DATA * ext = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  elfload(ext->elf_edit,FILEITEM_GetPath(sub_execute->file_item),FILEITEM_GetFname(sub_execute->file_item),0);
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return(1);
}

int on_PAGE_ACCEPT_EVENT(void * r0, BOOK * book)
{
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return(1);
}

int on_PAGE_PREVIOUS_EVENT(void * r0, BOOK * book)
{
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return(1);
}

int on_PAGE_CANCEL_EVENT(void * r0, BOOK * book)
{
  BookObj_ReturnPage(book,CANCEL_EVENT);
  return(1);
}
int on_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{
  return(1);
}

const PAGE_MSG FileOpen_EventList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  FileOpen_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      on_PAGE_ACCEPT_EVENT,
  PREVIOUS_EVENT_TAG,    on_PAGE_PREVIOUS_EVENT,
  CANCEL_EVENT_TAG,      on_PAGE_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   on_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

PAGE_DESC FileOpen_Page={"FileOpen_Page",0,FileOpen_EventList};
const char book_sub_open_name[]={"FileOpen_Subroutine"};
BOOK_SUBROUTINE book_sub_open = {book_sub_open_name, &FileOpen_Page, 0, 0};

const PAGE_MSG FileEdit_EventList[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG,  FileEdit_PAGE_ENTER_EVENT,
  ACCEPT_EVENT_TAG,      on_PAGE_ACCEPT_EVENT,
  PREVIOUS_EVENT_TAG,    on_PAGE_PREVIOUS_EVENT,
  CANCEL_EVENT_TAG,      on_PAGE_CANCEL_EVENT,
  PAGE_EXIT_EVENT_TAG,   on_PAGE_EXIT_EVENT,
  NIL_EVENT_TAG,0
};

PAGE_DESC FileEdit_Page={"FileEdit_Page",0,FileEdit_EventList};
const char book_sub_edit_name[]={"FileEdit_Subroutine"};
BOOK_SUBROUTINE book_sub_edit = {book_sub_edit_name, &FileEdit_Page, 0, 0};


//------------------------------------------ DB_CMD Methods
int NewFileSub_DB_CMD_SETSMALLICON(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
  DB_EXT_DATA * db_ext_data = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  *iconid = GetIcon(&db_ext_data->icon[small]);
  return 0;
}

int NewFileSub_DB_CMD_SETTHUMBNAILICON(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
  DB_EXT_DATA * db_ext_data = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  *iconid = GetIcon(&db_ext_data->icon[big]);
  return 0;
}

int NewFileSub_DB_CMD_SETSAVEDICON(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
  DB_EXT_DATA * db_ext_data = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  *iconid = GetIcon(&db_ext_data->icon[saved]);
  return 0;
}

int NewFileSub_DB_CMD_RUN(SUB_EXECUTE * sub_execute)
{
  BookObj_CallSubroutine(sub_execute->BrowserItemBook,&book_sub_open);
  return(1);
};

int NewFileSub_DB_CMD_EDIT(SUB_EXECUTE * sub_execute)
{
  BookObj_CallSubroutine(sub_execute->BrowserItemBook,&book_sub_edit);
  return(1);
};

//------------------------------------------ DB_CMD Methods check
int isOpenElfExist(SUB_EXECUTE * sub_execute)
{
  DB_EXT_DATA * ext = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  W_FSTAT wfstat;
  if (!ext->elf_open) return 0;
  if (w_fstat(ext->elf_open, &wfstat)<0) return 1;
  return 2;
}

int isEditElfExist(SUB_EXECUTE * sub_execute)
{
  DB_EXT_DATA * ext = SUB_EXECUTE_GetDB_EXT_DATA(sub_execute);
  W_FSTAT wfstat;
  if (!ext->elf_edit) return 0;
  if (w_fstat(ext->elf_edit, &wfstat)<0) return 1;
  return 2;
}

//------------------------------------------ Create FileSub (only once)
void CreateFileSubroutine()
{FUNCTION
  int ofs_count = 0;
  FILESUBROUTINE * OtherFileSub;
  FILESUBROUTINE * NewFileSub;

  GetOtherExtMethods(&OtherFileSub);
  while (OtherFileSub[ofs_count++].ON_CMD);
  FileSub = NewFileSub = new FILESUBROUTINE[ofs_count+2];
  memcpy(NewFileSub+2, OtherFileSub, ofs_count*sizeof(FILESUBROUTINE));

  NewFileSub->cmd = DB_CMD_RUN;
  NewFileSub->ON_CMD_RUN = NewFileSub_DB_CMD_RUN;
  NewFileSub->ON_CMD_RUN_CHECK = isOpenElfExist;
  textidname2id(IDN_START,-1,&NewFileSub->StrID);
#ifdef DB3350
  NewFileSub->StrID_1 = NewFileSub->StrID;
#endif

  NewFileSub++;
  NewFileSub->cmd = DB_CMD_EDIT;
  NewFileSub->ON_CMD_RUN = NewFileSub_DB_CMD_EDIT;
  NewFileSub->ON_CMD_RUN_CHECK = isEditElfExist;
  textidname2id(IDN_EDIT,-1,&NewFileSub->StrID);
#ifdef DB3350
  NewFileSub->StrID_1 = NewFileSub->StrID;
#endif

  while (NewFileSub->ON_CMD)
  {
    int cmd = NewFileSub->cmd;
    if (cmd == DB_CMD_SETSMALLICON)
    {
      NewFileSub->ON_CMD_ICON = NewFileSub_DB_CMD_SETSMALLICON;
    }
    else if(cmd == DB_CMD_SETTHUMBNAILICON)
    {
      NewFileSub->ON_CMD_ICON = NewFileSub_DB_CMD_SETTHUMBNAILICON;
    }
    else if(cmd == DB_CMD_SETSAVEDICON)
    {
      NewFileSub->ON_CMD_ICON = NewFileSub_DB_CMD_SETSAVEDICON;
    }
    NewFileSub++;
  }
}

void EXTF_2_DB_EXT_DATA(DB_EXT_DATA * ext, EXTF * extf)
{FUNCTION
  memset(ext, 0x0, sizeof(DB_EXT_DATA));
  ext->ext = new wchar_t[wstrlen(extf->ext)+1];
  wstrcpy(ext->ext, extf->ext);
  ext->isfile = extf->isfile;
  //icons
  for (int i=0; i<icon_count; i++)
  {
    if (extf->icon[i].external)
    {
      SetIcon(&ext->icon[i], extf->icon[i].path, extf->icon[i].name);
    }else
    {
      SetIcon(&ext->icon[i], extf->icon[i].id);
    }
  }
  //open
  if (extf->elf_open[0]) SetWstr(&ext->elf_open, extf->elf_open);
  if (extf->elf_edit[0]) SetWstr(&ext->elf_edit, extf->elf_edit);
  if (extf->path_int[0]) SetWstr(&ext->path_int, extf->path_int);
  if (extf->path_ext[0]) SetWstr(&ext->path_ext, extf->path_ext);
}

//------------------------------------------ Load data from database (only once)
void LoadData()
{FUNCTION
  W_FSTAT wfstat;
  int res = w_fstat(CfgFile, &wfstat);
  if (res>=0)
  {
    int count = wfstat.st_size/sizeof(EXTF);
    int f = w_fopen(CfgFile, WA_Read, 0x1FF, 0);
    if (f>=0)
    {
      EXTF * extf = new EXTF;
      for (int i=0; i<count; i++)
      {
        w_fread(f, extf, sizeof(EXTF));
        DB_EXT_DATA * db_ext_data = new DB_EXT_DATA;
        EXTF_2_DB_EXT_DATA(db_ext_data, extf);
        List_InsertSorted(DbExtData, db_ext_data, ExtDataCompare);
        if (!db_ext_data->isfile)
        {
          db_ext_data->db_ext= epd_CreateDbExt();

          db_ext_data->db_ext->content_type = CreateType(db_ext_data->ext);
          wchar_t ** w = new wchar_t*[2];
          w[0] = db_ext_data->ext;
          w[1] = 0;
          db_ext_data->db_ext->ext_list = w;
          db_ext_data->db_ext->GetMethods = GetMethods;
          db_ext_data->db_ext->dbf = CreateFolders(db_ext_data);
          List_InsertFirst(epd_DBExtList, db_ext_data->db_ext);
        }
      }
      delete extf;
      w_fclose(f);
    }
  }
}


void DB_EXT_DATA_2_EXTF(EXTF * extf, DB_EXT_DATA * ext)
{
  wchar_t *ipath, *iname;
  memset(extf, 0x0, sizeof(EXTF));
  wstrcpy(extf->ext, ext->ext);
  extf->isfile = ext->isfile;
  //icons
  for (int i=0; i<icon_count; i++)
  {
    int state = GetIcon(&ext->icon[i], &extf->icon[i].id, &ipath,  &iname);
    switch (state)
    {
    case 0: //ошибка при чтении
      extf->icon[i].external = 0;
      extf->icon[i].id = NOIMAGE;
      break;
    case 1: //в теле
      extf->icon[i].external = 0;
      break;
    case 2: //файлом
      extf->icon[i].external = 1;
      wstrcpy(extf->icon[i].path, ipath);
      wstrcpy(extf->icon[i].name, iname);
      break;
    }
  }
  if (ext->elf_open) wstrcpy(extf->elf_open, ext->elf_open);
  if (ext->elf_edit) wstrcpy(extf->elf_edit, ext->elf_edit);
  if (ext->path_int) wstrcpy(extf->path_int, ext->path_int);
  if (ext->path_ext) wstrcpy(extf->path_ext, ext->path_ext);
}

//------------------------------------------ Save data to database
int SaveData()
{FUNCTION
  int f = w_fopen(CfgFile, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
  if (f>=0)
  {
    int count = DbExtData->FirstFree;
    EXTF * extf = new EXTF;
    for (int i=0; i<count; i++)
    {
      DB_EXT_DATA * ext = (DB_EXT_DATA*)List_Get(DbExtData, i);
      DB_EXT_DATA_2_EXTF(extf, ext);
      w_fwrite(f, extf, sizeof(EXTF));
    }
    delete extf;
    w_fclose(f);
  }
  return 0;
}

int main (void)
{
  if (get_envp(get_bid(current_process()), "extension_methods")) return 0;
  ExtMemExist();
  CfgFile = MakeFullName(CFG_PATH, CFG_NAME);
  epd_processing();
  CreateFileSubroutine();
  DbExtData = List_Create();
  LoadData();
  InitMethods();
  return 0;
}
