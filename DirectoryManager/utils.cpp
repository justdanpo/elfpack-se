#include "main.h"

wchar_t * GetShortPath(wchar_t * dir)
{
  wchar_t * res = 0;
  wchar_t * dir_user = GetDir(DIR_USER|MEM_INTERNAL);
  int len = wstrlen(dir_user);
  if (!wstrcmpni(dir_user, dir, len))
  {
    dir += len;
    res = new wchar_t[wstrlen(dir)+1];
    return wstrcpy(res, dir);
  }
  dir_user = GetDir(DIR_USER|MEM_EXTERNAL);
  len = wstrlen(dir_user);
  if (!wstrcmpni(dir_user, dir, len))
  {
    dir += len;
    res = new wchar_t[wstrlen(dir)+1];
    return wstrcpy(res, dir);
  }
  res = new wchar_t[wstrlen(dir)+1];
  return wstrcpy(res, dir);
}

DIR_DATA * DIR_GetDATA(wchar_t * dir)
{
  dir = GetShortPath(dir);
  int count = DirData->FirstFree;
  for (int i=0; i<count; i++)
  {
    DIR_DATA * dir_data = (DIR_DATA*)List_Get(DirData, i);
    if (!wstrcmpi(dir_data->dir, dir))
    {
      delete dir;
      return dir_data;
    }
  }
  delete dir;
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
