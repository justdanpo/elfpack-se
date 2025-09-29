#include "main.h"

char ** CreateType(wchar_t * ext)
{
  char * mime_type = new char[6+wstrlen(ext)];
  strcpy(mime_type, "file/");
  wstr2strn(mime_type+5, ext, wstrlen(ext));
  char ** content_type = new char*[2];
  content_type[0] = mime_type;
  content_type[1] = 0;
  return content_type;
}

wchar_t * GetFileExtension(wchar_t * fname)
{FUNCTION
  if(fname)
  {
    wchar_t * e;
    if(e = wstrrchr(fname, '.'))
    {
      return e+1;
    }
  }
  return 0;
}

DB_EXT_DATA * EXT_GetDB_EXT_DATA(wchar_t * ext)
{
  int count = DbExtData->FirstFree;
  for (int i = 0; i<count; i++)
  {
    DB_EXT_DATA * db_ext_data = (DB_EXT_DATA*)List_Get(DbExtData, i);
    if (!wstrcmpi(ext, db_ext_data->ext))
    {
      return db_ext_data;
    }
  }
  return 0;
}

wchar_t * MakeFullName(wchar_t * path, wchar_t * name)
{FUNCTION
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t * fullname = new wchar_t[len+1];
  wstrcpy(fullname, path);
  wstrcat(fullname, L"/");
  wstrcat(fullname, name);
  return fullname;
}


//------------------------------------------ wstr util
wchar_t * SetWstr(wchar_t ** dest, wchar_t * source)
{
  wchar_t * wstr = *dest;
  if (!wstr) delete wstr;
  if (source)
  {
    wstr = new wchar_t[wstrlen(source)+1];
    wstrcpy(wstr, source);
  }
  else
  {
    wstr = 0;
  }
  return *dest = wstr;
}



//------------------------------------------ icon util
void InitIcon(ICON * icon)
{
  icon->id = NOIMAGE;
  icon->path = 0;
  icon->name = 0;
}
void FreeIcon(ICON * icon)
{
  ImageID_Free(icon->id);
  icon->id = NOIMAGE;
  if (icon->path) delete icon->path;
  icon->path = 0;
  if (icon->name) delete icon->name;
  icon->name = 0;
}

void SetIcon(ICON * icon, wchar_t id)
{
  FreeIcon(icon);
  icon->id = id;
}
void SetIcon(ICON * icon, wchar_t * path, wchar_t * name)
{
  FreeIcon(icon);
  icon->path = new wchar_t[wstrlen(path)+1];
  wstrcpy(icon->path, path);
  icon->name = new wchar_t[wstrlen(name)+1];
  wstrcpy(icon->name, name);
  ImageID_Get(path, name, &icon->id);
}

wchar_t GetIcon(ICON * icon)
{
  if (icon->path)
  {
    FSTAT _fstat;
    if (fstat(icon->path, icon->name, &_fstat)<0)
    {
      return NOIMAGE;
    }
    else
    {
      if (icon->id == NOIMAGE)
      {
        ImageID_Get(icon->path, icon->name, &icon->id);
      }
    }
  }
  return icon->id;
}

int GetIcon(ICON * icon, wchar_t * id, wchar_t ** path, wchar_t ** name)
{
  if (!icon) return 0;
  if (icon->path)
  {
    *path = icon->path;
    *name = icon->name;
    *id = GetIcon(icon);
    return 2;
  }
  *path = 0;
  *name = 0;
  *id = icon->id;
  return 1;
}

int ExtMemExist()
{
  if (!wstrncmp(GetDir(DIR_OTHER|MEM_EXTERNAL), L"/card", 5))
  {
    MEM_EXT = 1;
    return 1;
  }
  else
  {
    MEM_EXT = 0;
    return 0;
  }
}
