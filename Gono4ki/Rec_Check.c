#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "MyFUNC.h"
#include "StructBook.h"

wchar_t ImageHandler_Free(IMG*i)
{
  char error_code;
  REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER(SYNC,i->ImageHandle,0,0,i->ImageID,1,&error_code);
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  i->isImage=false;
  return 0;
};

void ImagesDel(BOOK*bk)
{
  for(int i=0; i<3; i++)
  {
    if(((MyBook*)bk)->image->fi[i]) ImageHandler_Free(((MyBook*)bk)->image->im[i]);
  }
}

u16 RegisterImage(IMG * i , wchar_t * path, wchar_t fname[256]) 
{
  char error_code;
  i->ImageID=NOIMAGE;
  i->ImageHandle=NOIMAGE;
  if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(SYNC,&i->ImageHandle,&error_code))
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(SYNC,i->ImageHandle,path, fname,0,&i->ImageID,&error_code))
    {
      if (error_code)
      {
        i->ImageHandle=NOIMAGE;
      }
    }
  }
  return(i->ImageID);
}

void OpenRes(IMGN*i, MyBook*bk)
{
  for(int j=0; j<3; j++)
  {
    if(!FindFile(GetConfigPath(), i->names[j]))
    {
      RegisterImage(bk->image->im[j], GetConfigPath(), i->names[j]);
      bk->image->fi[j]=1;
    }
  }
}

int CheckResurs(MyBook*bk)
{
  OpenRes(bk->nast->im, bk);
  return 1;
}
