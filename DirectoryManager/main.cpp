#include "main.h"


LIST * DirData;
FILESUBROUTINE * DirSub;
wchar_t * CfgFile;

int (*DeleteCheck)(struct SUB_EXECUTE *, void * );
int (*RenameCheck)(struct SUB_EXECUTE *, void * );
int (*SetItemText)(struct SUB_EXECUTE *, int * );
int (*SetSmallIcon)(struct SUB_EXECUTE *, wchar_t * );
int (*SetThumbnailIcon)(struct SUB_EXECUTE *, wchar_t * );

//------------------------------------------ cmp proc
int DirDataCompare(void * dir_data1, void * dir_data2)
{FUNCTION
  return (wstrcmpi(((DIR_DATA*)dir_data1)->dir,((DIR_DATA*)dir_data2)->dir));
}

//------------------------------------------ Dir subroutine
DIR_DATA * SUB_EXECUTE_GetDIR_DATA(SUB_EXECUTE * sub_execute)
{
  DIR_DATA * dir_data = 0;
  wchar_t * path = FILEITEM_GetPath(sub_execute->file_item);
  wchar_t * folder = FILEITEM_GetFname(sub_execute->file_item);
  wchar_t * fullpath = MakeFullName(path, folder);
  wchar_t * shortpath = GetShortPath(fullpath);
  delete fullpath;
  int count = DirData->FirstFree;
  for (int i = 0; i<count; i++)
  {
    DIR_DATA * data = (DIR_DATA*)List_Get(DirData, i);
    if (!wstrcmpi(shortpath, data->dir))
    {
      dir_data = data;
      break;
    }
  }
  delete shortpath;
  return dir_data;
}

int DirSub_DB_CMD_DELETE_Check(SUB_EXECUTE * sub_execute, void * v)
{
  DIR_DATA * dir_data = SUB_EXECUTE_GetDIR_DATA(sub_execute);
  if (dir_data)
  {
    return dir_data->delete_status;
  }
  return DeleteCheck(sub_execute, v);
}

int DirSub_DB_CMD_RENAME_Check(SUB_EXECUTE * sub_execute, void * v)
{
  DIR_DATA * dir_data = SUB_EXECUTE_GetDIR_DATA(sub_execute);
  if (dir_data)
  {
    return dir_data->rename_status;
  }
  return RenameCheck(sub_execute, v);
}

int DirSub_DB_CMD_SETITEMTEXT(SUB_EXECUTE * sub_execute, int * strid)
{
  DIR_DATA * dir_data = SUB_EXECUTE_GetDIR_DATA(sub_execute);
  if (dir_data)
  {
    if (dir_data->name)
    {
      *strid = TextID_Create(dir_data->name, ENC_UCS2, TEXTID_ANY_LEN);
      return 0;
    }
  }
  return SetItemText(sub_execute, strid);
}

int DirSub_DB_CMD_SETSMALLICON(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
  DIR_DATA * dir_data = SUB_EXECUTE_GetDIR_DATA(sub_execute);
  if (dir_data)
  {
    *iconid = GetIcon(&dir_data->icon[small]);
    return 0;
  }
  return SetSmallIcon(sub_execute, iconid);
}

int DirSub_DB_CMD_SETTHUMBNAILICON(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
  DIR_DATA * dir_data = SUB_EXECUTE_GetDIR_DATA(sub_execute);
  if (dir_data)
  {
    *iconid = GetIcon(&dir_data->icon[big]);
    return 0;
  }
  return SetThumbnailIcon(sub_execute, iconid);
}
__root void GetDirMethods(SUB_EXECUTE * sub_execute)
{
  sub_execute->filesub = DirSub;
}

//------------------------------------------ Create Dir Subroutine
void CreateDirSubroutine(DB_EXT * db_dir)
{
  int ds_count = 0;
  FILESUBROUTINE * OldDirSub;
  SUB_EXECUTE se;
  db_dir->GetMethods(&se);
  OldDirSub = se.filesub;
  while (OldDirSub[ds_count++].cmd != DB_CMD_LAST);
  DirSub = (FILESUBROUTINE*)malloc((ds_count)*sizeof(FILESUBROUTINE));
  memcpy(DirSub, OldDirSub, ds_count*sizeof(FILESUBROUTINE));
  db_dir->GetMethods = GetDirMethods;

  for (int i = 0; i< ds_count; i++)
  {
    FILESUBROUTINE * ds = DirSub + i;
    int cmd = ds->cmd;
    if (cmd == DB_CMD_DELETE)
    {
      DeleteCheck = ds->ON_CHECK;
      ds->ON_CHECK = DirSub_DB_CMD_DELETE_Check;
    }
    if (cmd == DB_CMD_RENAME)
    {
      RenameCheck = ds->ON_CHECK;
      ds->ON_CHECK = DirSub_DB_CMD_RENAME_Check;
    }
    else if (cmd == DB_CMD_SETITEMTEXT)
    {
      SetItemText = ds->ON_CMD_TEXTID;
      ds->ON_CMD_TEXTID = DirSub_DB_CMD_SETITEMTEXT;
    }
    else if (cmd == DB_CMD_SETSMALLICON)
    {
      SetSmallIcon = ds->ON_CMD_ICON;
      ds->ON_CMD_ICON = DirSub_DB_CMD_SETSMALLICON;
    }
    else if(cmd == DB_CMD_SETTHUMBNAILICON)
    {
      SetThumbnailIcon = ds->ON_CMD_ICON;
      ds->ON_CMD_ICON = DirSub_DB_CMD_SETTHUMBNAILICON;
    }
  }
}

//------------------------------------------ dir data replacement
DB_EXT * MoveDirData(void)
{
  DB_EXT * db_dir = new DB_EXT;
  EP_DATA * epd = (EP_DATA *)get_envp(get_bid(current_process()), "elfpackdata");
  LIST * epd_DBExtList = epd->DBExtList;
  for (int i=0; i<epd_DBExtList->FirstFree; i++)
  {
    DB_EXT * db_ext = (DB_EXT*)List_Get(epd_DBExtList, i);
    if (!strcmp(db_ext->content_type[0], "semc/dir"))
    {
      memcpy(db_dir, db_ext, sizeof(DB_EXT));
      DB_EXT ** db = (DB_EXT**)epd_DBExtList->listdata;
      db[i] = db_dir;
      break;
    }
  }
  return db_dir;
}

//------------------------------------------ Load data from database (only once)
void FILE_2_DATA(DIR_DATA * dir_data, DIR_DATA_FILE * dir_data_file)
{FUNCTION
  memset(dir_data, 0x0, sizeof(DIR_DATA));
  SetWstr(&dir_data->dir, dir_data_file->dir);
  if (dir_data_file->name[0]) SetWstr(&dir_data->name, dir_data_file->name);
  //icons
  for (int i=0; i<icon_count; i++)
  {
    if (dir_data_file->icon[i].external)
    {
      SetIcon(&dir_data->icon[i], dir_data_file->icon[i].path, dir_data_file->icon[i].name);
    }else
    {
      SetIcon(&dir_data->icon[i], dir_data_file->icon[i].id);
    }
  }
  dir_data->delete_status = dir_data_file->delete_status;
  dir_data->rename_status = dir_data_file->rename_status;
}

void LoadData()
{FUNCTION
  W_FSTAT wfstat;
  int res = w_fstat(CfgFile, &wfstat);
  if (res>=0)
  {
    int count = wfstat.st_size/sizeof(DIR_DATA_FILE);
    int f = w_fopen(CfgFile, WA_Read, 0x1FF, 0);
    if (f>=0)
    {
      DIR_DATA_FILE * dir_data_file = new DIR_DATA_FILE;
      for (int i=0; i<count; i++)
      {
        w_fread(f, dir_data_file, sizeof(DIR_DATA_FILE));
        DIR_DATA * dir_data = new DIR_DATA;
        FILE_2_DATA(dir_data, dir_data_file);
        List_InsertSorted(DirData, dir_data, DirDataCompare);
      }
      delete dir_data_file;
      w_fclose(f);
    }
  }
}

//------------------------------------------ Save data to database
void DATA_2_FILE(DIR_DATA_FILE * dir_data_file, DIR_DATA * dir_data)
{
  wchar_t *ipath, *iname;
  memset(dir_data_file, 0x0, sizeof(DIR_DATA_FILE));
  wstrcpy(dir_data_file->dir, dir_data->dir);
  if (dir_data->name) wstrcpy(dir_data_file->name, dir_data->name);
  //icons
  for (int i=0; i<icon_count; i++)
  {
    int state = GetIcon(&dir_data->icon[i], &dir_data_file->icon[i].id, &ipath,  &iname);
    switch (state)
    {
    case 0: //ошибка при чтении
      dir_data_file->icon[i].external = 0;
      dir_data_file->icon[i].id = NOIMAGE;
      break;
    case 1: //в теле
      dir_data_file->icon[i].external = 0;
      break;
    case 2: //файлом
      dir_data_file->icon[i].external = 1;
      wstrcpy(dir_data_file->icon[i].path, ipath);
      wstrcpy(dir_data_file->icon[i].name, iname);
      break;
    }
  }
  dir_data_file->delete_status = dir_data->delete_status;
  dir_data_file->rename_status = dir_data->rename_status;
}

int SaveData()
{FUNCTION
  int f = w_fopen(CfgFile, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
  if (f>=0)
  {
    int count = DirData->FirstFree;
    DIR_DATA_FILE * dir_data_file = new DIR_DATA_FILE;
    for (int i=0; i<count; i++)
    {
      DIR_DATA * ext = (DIR_DATA*)List_Get(DirData, i);
      DATA_2_FILE(dir_data_file, ext);
      w_fwrite(f, dir_data_file, sizeof(DIR_DATA_FILE));
    }
    delete dir_data_file;
    w_fclose(f);
  }
  return 0;
}

int main (void)
{
  if (get_envp(get_bid(current_process()), "directory_methods")) return 0;
  DirData = List_Create();
  CfgFile = MakeFullName(CFG_PATH, CFG_NAME);
  DB_EXT * db_dir = MoveDirData();
  CreateDirSubroutine(db_dir);
  LoadData();
  InitMethods();
  return 0;
}
