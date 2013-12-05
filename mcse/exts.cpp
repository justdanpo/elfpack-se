#include "inc\exts.h"
#include "inc\mc.h"

wchar_t GetIcon(wchar_t *name, int fattr)
{
  wchar_t icon;
  unsigned short d;
  if (fattr&FA_DIRECTORY) return STD_ICONS[ICN_FOLDER];
  SUB_EXECUTE *data;
  FILEITEM *fi=FILEITEM_Create();
  FILEITEM_SetFname(fi,name);
  data=DataBrowser_CreateSubExecute(-1, fi);
  DataBrowser_ExecuteSubroutine(data,DB_CMD_SETSMALLICON,&d);
  DataBrowser_ExecuteSubroutine(data,DB_CMD_DESTROY_SUBROUTINE,0);
  FILEITEM_Destroy(fi);
  icon=d;
  return icon;
}

