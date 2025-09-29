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
  saved,
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
  wchar_t * ext;
  int isfile;
  wchar_t * elf_open;
  wchar_t * elf_edit;
  ICON icon[icon_count];
  wchar_t * path_int;
  wchar_t * path_ext;
  DB_EXT * db_ext;
}DB_EXT_DATA;

typedef struct
{
  wchar_t external;
  wchar_t id;
  wchar_t path[300];
  wchar_t name[100];
}ICONF;

typedef struct
{
  wchar_t ext[56];
  int isfile;
  ICONF icon[icon_count];
  wchar_t elf_open[256];
  wchar_t elf_edit[256];
  wchar_t path_int[256];
  wchar_t path_ext[256];
}EXTF;

#define IDN_FOLDERNAME _T("DB_OTHER_TXT") //old value: _T("DB_APPLICATIONS_TXT")
#define IDN_SAVED_ON_MS _T("DB_SAVED_IN_OTHER_ON_MS_TXT")
#define IDN_SAVED_ON_PH _T("DB_SAVED_IN_OTHER_IN_PH_TXT")
#define IDN_START _T("GUI_SELECT_SK") //old values: _T("OAF_START_SK"), _T("MENU_SYNCH_START_REMOTE_TXT")
#define IDN_EDIT _T("DB_EDIT_TXT")

#define CFG_PATH (GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL))
#define CFG_NAME (L"extman.db")

extern LIST * DbExtData;
extern FILESUBROUTINE * FileSub;
extern wchar_t * CfgFile;
extern int MEM_EXT;
//from epd
extern LIST * epd_DBExtList;
extern DB_EXT * (*epd_CreateDbExt)(void);

typedef struct
{
  int (*Ext_Exist)(wchar_t * ext);
  int (*ExtMemExist)();
  int (*GetExtList)(wchar_t *** ext_list);
  int (*GetFullList)(wchar_t *** ext_list);
  int (*Ext_IsFile)(wchar_t * ext);
  int (*Ext_Add)(wchar_t * ext);
  int (*Ext_Rename)(wchar_t * ext, wchar_t * ext_new);
  int (*Ext_Remove)(wchar_t * ext);
  int (*Ext_SetElfOpen)(wchar_t * ext, wchar_t * elf);
  int (*Ext_GetElfOpen)(wchar_t * ext, wchar_t ** elf);
  int (*Ext_SetElfEdit)(wchar_t * ext, wchar_t * elf);
  int (*Ext_GetElfEdit)(wchar_t * ext, wchar_t ** elf);
  int (*Ext_SetSmallIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetSmallIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetSmallIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetBigIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetBigIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetBigIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetSavedIconId)(wchar_t * ext, wchar_t iconid);
  int (*Ext_SetSavedIconFile)(wchar_t * ext, wchar_t * path, wchar_t * name);
  int (*Ext_GetSavedIcon)(wchar_t * ext, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Ext_SetPathInt)(wchar_t * ext, wchar_t * path);
  int (*Ext_GetPathInt)(wchar_t * ext, wchar_t ** path);
  int (*Ext_SetPathExt)(wchar_t * ext, wchar_t * path);
  int (*Ext_GetPathExt)(wchar_t * ext, wchar_t ** path);
}EXTENSION_METHODS;

wchar_t * GetFileExtension(wchar_t * fname);
DB_EXT_DATA * EXT_GetDB_EXT_DATA(wchar_t * ext);
wchar_t * MakeFullName(wchar_t * path, wchar_t * name);
wchar_t * SetWstr(wchar_t ** dest, wchar_t * source);
void InitIcon(ICON * icon);
void FreeIcon(ICON * icon);
void SetIcon(ICON * icon, wchar_t id);
void SetIcon(ICON * icon, wchar_t * path, wchar_t * name);
wchar_t GetIcon(ICON * icon);
int GetIcon(ICON * icon, wchar_t * id, wchar_t ** path, wchar_t ** name);
DB_EXT_DATA * CreateDbExtData(wchar_t * ext);
int DeleteDbExtData(DB_EXT_DATA * db_ext_data);
DB_EXT_FOLDERS * CreateFolders(DB_EXT_DATA * db_ext_data);
int ExtDataCompare(void * ext1, void * ext2);
int SaveData();
void GetMethods(SUB_EXECUTE * sub_execute);
int ExtMemExist();
int InitMethods();
char ** CreateType(wchar_t * ext);
