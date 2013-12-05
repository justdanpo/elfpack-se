typedef struct _LIBRARY_DLL_FUNCTIONINFO
{
    int functionnum;
    void* functionptr;
}LIBRARY_DLL_FUNCTIONINFO;

typedef struct _LIBRARY_DLL_DATA
{
  wchar_t * dll_name; // об€зательное поле.не перемещать!!! необходимо дл€ UnLoadDLL
  
  const LIBRARY_DLL_FUNCTIONINFO* functions;
  
#ifdef DLL_PRIVATE_AREA
#include "LibraryDLL_private.h"
#endif

}LIBRARY_DLL_DATA;

