#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "config_data.h"
#include "structs.h"
#include "Externals.h"

UI_IMG img;

wchar_t ImageHandler_Get(UI_IMG * i , wchar_t * path, wchar_t * fname)
{
  char error_code;
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  if (!fstat(path,fname,0))
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(SYNC,&i->ImageHandle,&error_code))
    {
      if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(SYNC,i->ImageHandle,path,fname,0,&i->ImageID,&error_code))
      {
        if (error_code)
        {
          i->ImageHandle=NOIMAGE;
        }
      }
    }
  }
  return(i->ImageID);
};

wchar_t ImageHandler_Get(METADATA * i , wchar_t * path, wchar_t * fname)
{
  char error_code;
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  i->Handle=true;
  if (!fstat(path,fname,0))
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(SYNC,&i->ImageHandle,&error_code))
    {
      if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(SYNC,i->ImageHandle,path,fname,0,&i->ImageID,&error_code))
      {
        if (error_code)
        {
          i->Handle=false;
          i->ImageHandle=NOIMAGE;
        }
      }
    }
  }
  return(i->ImageID);
};

wchar_t ImageHandler_Get(IMG_DESC * i , wchar_t * path, wchar_t * fname)
{
  char error_code;
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  i->Handle=true;
  i->HasImage=true;
  if (!fstat(path,fname,0))
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(SYNC,&i->ImageHandle,&error_code))
    {
      if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(SYNC,i->ImageHandle,path,fname,0,&i->ImageID,&error_code))
      {
        if (error_code)
        {
          i->Handle=false;
          i->HasImage=false;
          i->ImageHandle=NOIMAGE;
        }
      }
    }
  }
  return(i->ImageID);
};


wchar_t ImageHandler_Free(UI_IMG *i)
{
  char error_code;
  REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER(SYNC,i->ImageHandle,0,0,i->ImageID,1,&error_code);
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  i->isImage=false;
  return 0;
};

wchar_t ImageHandler_Free(METADATA *i)
{
  // Если у изображения нету хэндла, чистим ИД функцией ImageID_Free, а если есть - то анрегистером
  if (!i->Handle)
  {
    ImageID_Free(i->ImageID);
    i->ImageID=NOIMAGE;
    i->ImageHandle=NOIMAGE;
    i->HasImage=false;
    i->Handle=false;
  }
  else
  {
    char error_code;
    REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER(SYNC,i->ImageHandle,0,0,i->ImageID,1,&error_code);
    i->ImageID=NOIMAGE;
    i->ImageHandle=NOIMAGE;
    i->HasImage=false;
    i->Handle=false;
  }
  return 0;
};



void Images_Load()
{
  if (CustomImage_Show==true)
  {
    img.isImage=true;
    wchar_t *name = new wchar_t[strlen(CustomImage)+1];
    char2unicode(name,CustomImage,strlen(CustomImage));
    ImageHandler_Get(&img, checkfile(name), name);
    delete(name);
  }
};

void Images_Unload()
{
  if (img.isImage==true)
  {
    ImageHandler_Free(&img);
    img.isImage=false;
  }
};

