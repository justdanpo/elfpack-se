#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\presets.h"
extern int DB;
void MyBook_OnClose(BOOK * book);

int isSoftkeysBook_ByName(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"Softkeys")==0)return 1;
  return 0;
};

int isMMBrowserBook(BOOK *bk)
{
  if (strstr(bk->xbook->name,"MM_Browser")) return 1;
  return 0;
};

int isMenuBook(BOOK *bk)
{
  if (!strcmp(bk->xbook->name,"MenuBook"))return 1;
  if (!strcmp(bk->xbook->name,"Menu_Globals"))return 1;
  return 0;
};

int isImageViewerBook(BOOK *bk)
{
  if (!strcmp(bk->xbook->name,"ImageViewerBook"))return 1;
  return 0;
};

int isSoftkeysBook(BOOK *bk)
{
  if (bk->onClose == MyBook_OnClose)return 1;
  return 0;
};


int isImageViewer(BOOK *bk)
{
  if (bk->xbook->name[0]=='I')
  {
    if (bk->xbook->name[1]=='m')
    {
      if (strcmp(bk->xbook->name,"ImageViewerBook")==0)
      {
        if (DB==2010)return 1;
      }
    }
  }
  return 0;
};
