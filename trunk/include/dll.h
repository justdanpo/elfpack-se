#ifndef __DLL_H__
#define __DLL_H__

typedef struct
{
  wchar_t * name;
}DLL_DATA;

typedef struct
{
  wchar_t * name;
  int (*EntryPoint)(int Action, DLL_DATA * data);
}DLL_LIST_ELEMENT;

#define INVALID(__param__)  ((int)__param__<=0)


#define DLL_INIT 0
#define DLL_LOAD 1
#define DLL_UNLOAD 2

#define DLL_ERROR(__error__) (void*)__error__

#define DLL_ERROR_INIT              DLL_ERROR(-1111)
#define DLL_ERROR_LOAD              DLL_ERROR(-2222)
#define DLL_ERROR_DLL_NOT_LOADED    -3333
#define DLL_ERROR_NO_DATA_P         -4444
#define DLL_ERROR_FILE_NOT_LOADED   DLL_ERROR(-5555)
#define DLL_ERROR_FILE_NOT_FOUND    DLL_ERROR(-6666)

#endif
