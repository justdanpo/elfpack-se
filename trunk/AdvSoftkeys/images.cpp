#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\images.h"
IMAGEID images[10]={NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
u16 handles[10]={NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};

wchar_t ImageHandler_Get(wchar_t * path, wchar_t * fname, int x)
{
  char error_code;
  images[x]=NOIMAGE;
  handles[x]=NOIMAGE;
  if (!fstat(path,fname,0))
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(SYNC,&handles[x],&error_code))
    {
      if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(SYNC,handles[x],path,fname,0,&images[x],&error_code))
      {
        if (error_code)
        {
          handles[x]=NOIMAGE;
        }
      }
    }
  }
  return(images[x]);
};

wchar_t ImageHandler_Free(int x)
{
  if (images[x]!=NOIMAGE)
  {
    char error_code;
    REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER(SYNC,handles[x],0,0,images[x],1,&error_code);
    images[x]=NOIMAGE;
    handles[x]=NOIMAGE;
  }
  return 0;
};

void images_add(wchar_t *path, wchar_t *name, int index)
{
  ImageHandler_Get(path, name, index);
};

void destroy_all_images()
{
  int x;
  for (x=0;x<7;x++)
  {
    if (images[x]!=NOIMAGE)
    {
      ImageHandler_Free(x);
    }
    images[x]=NOIMAGE;
  }
};

