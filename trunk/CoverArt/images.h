#ifndef _IMAGES_H_
  #include "structs.h"

  #define _IMAGES_H_

  wchar_t ImageHandler_Get(UI_IMG * i , wchar_t * path, wchar_t * fname);
  wchar_t ImageHandler_Get(METADATA * i , wchar_t * path, wchar_t * fname);
  wchar_t ImageHandler_Free(UI_IMG *i);
  wchar_t ImageHandler_Free(METADATA *i);
  
  wchar_t ImageHandler_Get(IMG_DESC * i , wchar_t * path, wchar_t * fname);

#endif

