#include "main.h"

//------------------------------------------ Get extension list
int GetExtList(wchar_t *** ext_list)
{
  int cnt=0;
  int count = DbExtData->FirstFree;
  wchar_t ** w = new wchar_t * [count+1];
  for (int i=0; i<count; i++)
  {
    DB_EXT_DATA * db_ext_data = (DB_EXT_DATA*)List_Get(DbExtData, i);
    if (!db_ext_data->isfile)
    {
      wchar_t * e = db_ext_data->ext;
      w[cnt] = new wchar_t[wstrlen(e)+1];
      wstrcpy(w[cnt++], e);
    }
  }
  //w[count] = 0;
  wchar_t ** list = new wchar_t*[cnt+1];
  memcpy(list, w, cnt*sizeof(wchar_t*));
  list[cnt] = 0;
  *ext_list = list;
  delete w;
  return cnt;
}

//------------------------------------------ Get extension & file list
int GetFullList(wchar_t *** ext_list)
{
  int count = DbExtData->FirstFree;
  wchar_t ** w = new wchar_t * [count+1];
  for (int i=0; i<count; i++)
  {
    DB_EXT_DATA * db_ext_data = (DB_EXT_DATA*)List_Get(DbExtData, i);
    wchar_t * e = db_ext_data->ext;
    w[i] = new wchar_t[wstrlen(e)+1];
    wstrcpy(w[i], e);
  }
  w[count] = 0;
  *ext_list = w;
  return count;
}

//------------------------------------------ Is file name
int Ext_IsFile(wchar_t * ext)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data) return db_ext_data->isfile;
  return 0;
}

//------------------------------------------ Is extension exist
int Ext_Exist(wchar_t * ext)
{FUNCTION
  if (EXT_GetDB_EXT_DATA(ext)) return 1;
  return 0;
}

//------------------------------------------ Add
int Ext_Add(wchar_t * ext)
{FUNCTION
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data) return 1;
  wchar_t * e = GetFileExtension(ext);
  if(e)
  {
    if(EXT_GetDB_EXT_DATA(e))
    {
      db_ext_data = CreateDbExtData(ext);
      List_InsertSorted(DbExtData, db_ext_data, ExtDataCompare);
      SaveData();
      return 1;
    }
  }
  else
  {
    db_ext_data = CreateDbExtData(ext);
    List_InsertSorted(DbExtData, db_ext_data, ExtDataCompare);
    db_ext_data->db_ext= epd_CreateDbExt();
    db_ext_data->db_ext->content_type = CreateType(db_ext_data->ext);
    wchar_t ** w = new wchar_t*[2];
    w[0] = db_ext_data->ext;
    w[1] = 0;
    db_ext_data->db_ext->ext_list = w;
    db_ext_data->db_ext->GetMethods = GetMethods;
    db_ext_data->db_ext->dbf = CreateFolders(db_ext_data);
    List_InsertFirst(epd_DBExtList, db_ext_data->db_ext);
    SaveData();
    return 1;
  }
  return 0;
}

//------------------------------------------ Rename
int Ext_Rename(wchar_t * ext, wchar_t * ext_new)
{FUNCTION
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    delete db_ext_data->ext;
    db_ext_data->ext = new wchar_t[wstrlen(ext_new)+1];
    wstrcpy(db_ext_data->ext, ext_new);
    wstrnlwr(db_ext_data->ext, TEXTID_ANY_LEN);
    if (!wstrrchr(db_ext_data->ext,'.')) db_ext_data->db_ext->ext_list[0] = db_ext_data->ext;
    int index = List_IndexOf(DbExtData, db_ext_data);
    List_RemoveAt(DbExtData, index);
    List_InsertSorted(DbExtData, db_ext_data, ExtDataCompare);
    SaveData();
    return 1;
  }
  return 0;
}

//------------------------------------------ Remove
int Ext_Remove(wchar_t * ext)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    if (!GetFileExtension(ext))
    {
      for (int i=0; i<DbExtData->FirstFree;)
      {
        DB_EXT_DATA * db_file_data = (DB_EXT_DATA*)List_Get(DbExtData, i);
        wchar_t * e;
        if (e = GetFileExtension(db_file_data->ext))
        {
          if (!wstrcmp(e, ext))
          {
            List_RemoveAt(DbExtData, i);
            DeleteDbExtData(db_file_data);
            continue;
          }
        }
        i++;
      }
    }
    int index = List_IndexOf(DbExtData, db_ext_data);
    List_RemoveAt(DbExtData, index);
    DeleteDbExtData(db_ext_data);
    SaveData();
    return 1;
  }
  return 0;
}

//------------------------------------------ Open elf
int Ext_SetElfOpen(wchar_t * ext, wchar_t * elf)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    delete db_ext_data->elf_open;
    if (elf)
    {
      db_ext_data->elf_open = new wchar_t[wstrlen(elf)+1];
      wstrcpy(db_ext_data->elf_open, elf);
    }
    else
    {
      db_ext_data->elf_open = 0;
    }
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetElfOpen(wchar_t * ext, wchar_t ** elf)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    *elf = db_ext_data->elf_open;
    return 1;
  }
  return 0;
}

//------------------------------------------ Edit elf
int Ext_SetElfEdit(wchar_t * ext, wchar_t * elf)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    delete db_ext_data->elf_edit;
    if (elf)
    {
      db_ext_data->elf_edit = new wchar_t[wstrlen(elf)+1];
      wstrcpy(db_ext_data->elf_edit, elf);
    }
    else
    {
      db_ext_data->elf_edit = 0;
    }
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetElfEdit(wchar_t * ext, wchar_t ** elf)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    *elf = db_ext_data->elf_edit;
    return 1;
  }
  return 0;
}

//------------------------------------------ Small icon
int Ext_SetSmallIconId(wchar_t * ext, wchar_t iconid)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[small], iconid);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_SetSmallIconFile(wchar_t * ext, wchar_t * path, wchar_t * name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[small], path, name);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetSmallIcon(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    return GetIcon(&db_ext_data->icon[small], iconid, path, name);
  }
  *iconid = NOIMAGE;
  return 0;
}

//------------------------------------------ Big icon
int Ext_SetBigIconId(wchar_t * ext, wchar_t iconid)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[big], iconid);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_SetBigIconFile(wchar_t * ext, wchar_t * path, wchar_t * name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[big], path, name);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetBigIcon(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    return GetIcon(&db_ext_data->icon[big], iconid, path, name);
  }
  *iconid = NOIMAGE;
  return 0;
}

//------------------------------------------ Saved icon
int Ext_SetSavedIconId(wchar_t * ext, wchar_t iconid)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[saved], iconid);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_SetSavedIconFile(wchar_t * ext, wchar_t * path, wchar_t * name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    SetIcon(&db_ext_data->icon[saved], path, name);
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetSavedIcon(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name)
{
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    return GetIcon(&db_ext_data->icon[saved], iconid, path, name);
  }
  *iconid = NOIMAGE;
  return 0;
}

//------------------------------------------ Internal path
int Ext_SetPathInt(wchar_t * ext, wchar_t * path)
{
  if (!path) return 0;
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    if (db_ext_data->isfile) return 0;
    SetWstr(&db_ext_data->path_int, path);
    DB_EXT_FOLDERS * dbf = (DB_EXT_FOLDERS *)db_ext_data->db_ext->dbf;
    dbf[(MEM_EXT && db_ext_data->path_ext)?1:0].Path = db_ext_data->path_int;
    SaveData();
    return 1;
  }
  return 0;
}

int Ext_GetPathInt(wchar_t * ext, wchar_t ** path)
{
  wchar_t * e = GetFileExtension(ext);
  if (e) ext = e;
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    *path = db_ext_data->path_int;
    return 1;
  }
  return 0;
}

//------------------------------------------ External path
int Ext_SetPathExt(wchar_t * ext, wchar_t * path)
{
  if (MEM_EXT)
  {
    DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
    if (db_ext_data)
    {
      if (db_ext_data->isfile) return 0;
      if (path)
      {
        int state = (int)db_ext_data->path_ext;
        SetWstr(&db_ext_data->path_ext, path);
        DB_EXT_FOLDERS * dbf = (DB_EXT_FOLDERS *)db_ext_data->db_ext->dbf;
        if (state)
        {
          dbf[0].Path = db_ext_data->path_ext;
        }
        else
        {
          delete dbf;
          db_ext_data->db_ext->dbf = CreateFolders(db_ext_data);
        }
      }
      else
      {
        if (db_ext_data->path_ext)
        {
          delete db_ext_data->path_ext;
          db_ext_data->path_ext = 0;
          delete db_ext_data->db_ext->dbf;
          db_ext_data->db_ext->dbf = CreateFolders(db_ext_data);
        }
      }
      SaveData();
      return 1;
    }
  }
  return 0;
}

int Ext_GetPathExt(wchar_t * ext, wchar_t ** path)
{
  wchar_t * e = GetFileExtension(ext);
  if (e) ext = e;
  DB_EXT_DATA * db_ext_data = EXT_GetDB_EXT_DATA(ext);
  if (db_ext_data)
  {
    *path = db_ext_data->path_ext;
    return 1;
  }
  return 0;
}

int InitMethods()
{
  EXTENSION_METHODS * methods = new EXTENSION_METHODS;
  methods->Ext_Exist = Ext_Exist;
  methods->ExtMemExist = ExtMemExist;
  methods->GetExtList = GetExtList;
  methods->GetFullList = GetFullList;
  methods->Ext_IsFile = Ext_IsFile;
  methods->Ext_Add = Ext_Add;
  methods->Ext_Rename = Ext_Rename;
  methods->Ext_Remove = Ext_Remove;
  methods->Ext_SetElfOpen = Ext_SetElfOpen;
  methods->Ext_GetElfOpen = Ext_GetElfOpen;
  methods->Ext_SetElfEdit = Ext_SetElfEdit;
  methods->Ext_GetElfEdit = Ext_GetElfEdit;
  methods->Ext_SetSmallIconId = Ext_SetSmallIconId;
  methods->Ext_SetSmallIconFile = Ext_SetSmallIconFile;
  methods->Ext_GetSmallIcon = Ext_GetSmallIcon;
  methods->Ext_SetBigIconId = Ext_SetBigIconId;
  methods->Ext_SetBigIconFile = Ext_SetBigIconFile;
  methods->Ext_GetBigIcon = Ext_GetBigIcon;
  methods->Ext_SetSavedIconId = Ext_SetSavedIconId;
  methods->Ext_SetSavedIconFile = Ext_SetSavedIconFile;
  methods->Ext_GetSavedIcon = Ext_GetSavedIcon;
  methods->Ext_SetPathInt = Ext_SetPathInt;
  methods->Ext_GetPathInt = Ext_GetPathInt;
  methods->Ext_SetPathExt = Ext_SetPathExt;
  methods->Ext_GetPathExt = Ext_GetPathExt;
  set_envp(get_bid(current_process()), "extension_methods", (OSADDRESS)methods);
  return 0;
}
