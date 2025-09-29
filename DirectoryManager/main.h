#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\dll.h"
#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\var_arg.h"

class CDbgPrint
{
  char* fl;
  char* fn;
public:
  CDbgPrint(char* file,char* func){debug_printf("\nfunction enter: %s/%s\n",fl=file,fn=func);}
  ~CDbgPrint(){debug_printf("\nfunction leave: %s/%s\n",fl,fn);}
};
#ifdef NDEBUG
#define FUNCTION
#else
#define FUNCTION CDbgPrint __function((char*)__FILE__,(char*)__FUNCTION__);
#endif


enum
{
  small = 0,
  big,
  icon_count
};

typedef struct
{
  IMAGEID id;
  wchar_t * path;
  wchar_t * name;
}ICON;

typedef struct
{
  wchar_t * dir;
  wchar_t * name;
  ICON icon[icon_count];
  char delete_status;
  char rename_status;
}DIR_DATA;

typedef struct
{
  wchar_t external;
  wchar_t id;
  wchar_t path[300];
  wchar_t name[100];
}ICONF;

typedef struct
{
  wchar_t dir[300];
  wchar_t name[100];
  ICONF icon[icon_count];
  char delete_status;
  char rename_status;
}DIR_DATA_FILE;

#define CFG_PATH (GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL))
#define CFG_NAME (L"dirman.db")

extern LIST * DirData;
extern FILESUBROUTINE * DirSub;
extern wchar_t * CfgFile;

typedef struct
{
  int (*Dir_Registered)(wchar_t * dir);
  int (*Dir_Add)(wchar_t * dir);
  int (*Dir_Rename)(wchar_t * dir, wchar_t * new_dir);
  int (*Dir_Remove)(wchar_t * dir);
  int (*Dir_SetName)(wchar_t * dir, wchar_t * name);
  int (*Dir_GetName)(wchar_t * dir, wchar_t ** name);
  int (*Dir_SetDeleteState)(wchar_t * dir, char state);
  int (*Dir_GetDeleteState)(wchar_t * dir, char * state);
  int (*Dir_SetRenameState)(wchar_t * dir, char state);
  int (*Dir_GetRenameState)(wchar_t * dir, char * state);
  int (*Dir_SetSmallIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetSmallIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetSmallIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Dir_SetBigIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetBigIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetBigIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);

  wchar_t * (*Dir_GetShortPath)(wchar_t * dir);
  int (*Dir_GetCount)();
  LIST * (*Dir_ListCreate)();
  int (*Dir_ListDestroy)(LIST * list);
}DIRECTORY_METHODS;

wchar_t * MakeFullName(wchar_t * path, wchar_t * name);
wchar_t * SetWstr(wchar_t ** dest, wchar_t * source);

void InitIcon(ICON * icon);
void FreeIcon(ICON * icon);
void SetIcon(ICON * icon, wchar_t id);
void SetIcon(ICON * icon, wchar_t * path, wchar_t * name);
wchar_t GetIcon(ICON * icon);
int GetIcon(ICON * icon, wchar_t * id, wchar_t ** path, wchar_t ** name);

wchar_t * GetShortPath(wchar_t * dir);
DIR_DATA * DIR_GetDATA(wchar_t * dir);
int DirDataCompare(void * dir_data1, void * dir_data2);
int SaveData();
int InitMethods();
