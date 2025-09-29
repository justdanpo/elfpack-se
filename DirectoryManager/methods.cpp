#include "main.h"

//------------------------------------------ Is dir exist
int Dir_Registered(wchar_t * dir)
{FUNCTION
  if (DIR_GetDATA(dir)) return 1;
  return 0;
}

//------------------------------------------ Add
int Dir_Add(wchar_t * dir)
{FUNCTION
  if (!dir) return 0;
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data) return 1;
  dir_data = new DIR_DATA;
  memset(dir_data, 0x0, sizeof(DIR_DATA));
  dir_data->dir = GetShortPath(dir);
  wstrnlwr(dir_data->dir, TEXTID_ANY_LEN);
  for (int i = 0; i < icon_count; i++)
  {
    InitIcon(&dir_data->icon[i]);
  }
  dir_data->delete_status = 0;
  dir_data->rename_status = 0;
  List_InsertSorted(DirData, dir_data, DirDataCompare);
  SaveData();
  return 1;
}

//------------------------------------------ Rename
int Dir_Rename(wchar_t * dir, wchar_t * new_dir)
{FUNCTION
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    delete dir_data->dir;
    dir_data->dir = GetShortPath(new_dir);
    wstrnlwr(dir_data->dir, TEXTID_ANY_LEN);
    int index = List_IndexOf(DirData, dir_data);
    List_RemoveAt(DirData, index);
    List_InsertSorted(DirData, dir_data, DirDataCompare);
    SaveData();
    return 1;
  }
  return 0;
}

//------------------------------------------ Remove
int Dir_Remove(wchar_t * dir)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    int index = List_IndexOf(DirData, dir_data);
    List_RemoveAt(DirData, index);
    delete dir_data->dir;
    if (dir_data->name) delete dir_data->name;
    for (int i = 0; i < icon_count; i++)
    {
      FreeIcon(&dir_data->icon[i]);
    }
    SaveData();
    return 1;
  }
  return 0;
}
//------------------------------------------ Name
int Dir_SetName(wchar_t * dir, wchar_t * name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    if (!name)
    {
      if (dir_data->name) delete dir_data->name;
      dir_data->name = 0;
    }
    else
    {
      SetWstr(&dir_data->name, name);
    }
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_GetName(wchar_t * dir, wchar_t ** name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    *name = dir_data->name;
    return 1;
  }
  return 0;
}

//------------------------------------------ Delete state
int Dir_SetDeleteState(wchar_t * dir, char state)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    dir_data->delete_status = state;
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_GetDeleteState(wchar_t * dir, char * state)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    *state = dir_data->delete_status;
    return 1;
  }
  return 0;
}

//------------------------------------------ Rename state
int Dir_SetRenameState(wchar_t * dir, char state)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    dir_data->rename_status = state;
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_GetRenameState(wchar_t * dir, char * state)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    *state = dir_data->rename_status;
    return 1;
  }
  return 0;
}

//------------------------------------------ Small icon
int Dir_SetSmallIconId(wchar_t * dir, wchar_t iconid)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    SetIcon(&dir_data->icon[small], iconid);
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_SetSmallIconFile(wchar_t * dir, wchar_t * path, wchar_t * name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    SetIcon(&dir_data->icon[small], path, name);
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_GetSmallIcon(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    return GetIcon(&dir_data->icon[small], iconid, path, name);
  }
  *iconid = NOIMAGE;
  return 0;
}

//------------------------------------------ Big icon
int Dir_SetBigIconId(wchar_t * dir, wchar_t iconid)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    SetIcon(&dir_data->icon[big], iconid);
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_SetBigIconFile(wchar_t * dir, wchar_t * path, wchar_t * name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    SetIcon(&dir_data->icon[big], path, name);
    SaveData();
    return 1;
  }
  return 0;
}

int Dir_GetBigIcon(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name)
{
  DIR_DATA * dir_data = DIR_GetDATA(dir);
  if (dir_data)
  {
    return GetIcon(&dir_data->icon[big], iconid, path, name);
  }
  *iconid = NOIMAGE;
  return 0;
}

//------------------------------------------ Get short name
wchar_t * Dir_GetShortPath(wchar_t * dir)
{FUNCTION
  return GetShortPath(dir);
}

//------------------------------------------ Get directorys count
int Dir_GetCount(void)
{FUNCTION
  return DirData->FirstFree;
}

//------------------------------------------ Create directorys list
LIST * Dir_ListCreate(void)
{FUNCTION
  int count = DirData->FirstFree;
  LIST * list = List_Create();
  for (int i=0; i<count; i++)
  {
    DIR_DATA * dir_data = (DIR_DATA*)List_Get(DirData, i);
    wchar_t * dir = new wchar_t[wstrlen(dir_data->dir)+1];
    List_InsertLast(list, wstrcpy(dir, dir_data->dir));
  }
  return list;
}

//------------------------------------------ Destroy directorys list
int Dir_ListDestroy(LIST * list)
{FUNCTION
  if (!list) return 0;
  int count = DirData->FirstFree;
  for (int i=0; i<count; i++)
  {
    delete List_Get(list, i);
  }
  List_Destroy(list);
  return 1;
}

int InitMethods()
{
  DIRECTORY_METHODS * methods = new DIRECTORY_METHODS;
  methods->Dir_Registered = Dir_Registered;
  methods->Dir_Add = Dir_Add;
  methods->Dir_Rename = Dir_Rename;
  methods->Dir_Remove = Dir_Remove;
  methods->Dir_SetName = Dir_SetName;
  methods->Dir_GetName = Dir_GetName;
  methods->Dir_SetDeleteState = Dir_SetDeleteState;
  methods->Dir_GetDeleteState = Dir_GetDeleteState;
  methods->Dir_SetRenameState = Dir_SetRenameState;
  methods->Dir_GetRenameState = Dir_GetRenameState;
  methods->Dir_SetSmallIconId = Dir_SetSmallIconId;
  methods->Dir_SetSmallIconFile = Dir_SetSmallIconFile;
  methods->Dir_GetSmallIcon = Dir_GetSmallIcon;
  methods->Dir_SetBigIconId = Dir_SetBigIconId;
  methods->Dir_SetBigIconFile = Dir_SetBigIconFile;
  methods->Dir_GetBigIcon = Dir_GetBigIcon;

  methods->Dir_GetShortPath = Dir_GetShortPath;
  methods->Dir_GetCount = Dir_GetCount;
  methods->Dir_ListCreate = Dir_ListCreate;
  methods->Dir_ListDestroy = Dir_ListDestroy;

  set_envp(get_bid(current_process()), "directory_methods", (OSADDRESS)methods);
  return 0;
}
