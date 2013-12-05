#ifndef __FILEDIALOGDLL_H__
#define __FILEDIALOGDLL_H__

#include <stdarg.h>
#include "opendialog.h"

typedef struct FILE_DIALOG
{
  wchar_t * dll_name; // об€зательное поле.не перемещать!!! необходимо дл€ UnLoadDLL
  wchar_t * (*CreateFD) (FILE_DIALOG* fd,
                                 BOOK * book,
                          int flags,
                          wchar_t * header,
                          wchar_t * filters,
                          ...  // folders list
                          );


#ifdef DLL_PRIVATE_AREA
#include "FileDialogDll_private.h"
#endif

}_FILE_DIALOG;


#define FILE_DIALOG_Create(__this__,...)  __this__->CreateFD(__this__,__VA_ARGS__)

#endif
