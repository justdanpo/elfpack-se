#include "inc\mc.h"
#include "inc\file_op.h"
#include "inc\fn_list.h"

static wchar_t *st_pathbuf_fn=NULL;

wchar_t *GetPathBuf(void)
{
  return (st_pathbuf_fn?st_pathbuf_fn:(st_pathbuf_fn=new wchar_t[MAX_PATH]));
}

void DestroyPathBuf(void)
{
  if (st_pathbuf_fn) {delete st_pathbuf_fn; st_pathbuf_fn=NULL;};
}


void fn_zero(FN_LIST *list)
{
  memset(list, 0, sizeof(FN_LIST));
}

void fn_free(FN_LIST *list)
{
  if (list)
  {
    if (list->count)
    {
      while(list->items)
      {
        FN_ITM *itm = list->items;
        list->items = (FN_ITM *)itm->next;
      
        if (itm->full) delete(itm->full);
        if (itm->zipPath) delete(itm->zipPath);
        delete(itm);
        list->count--;
      }
    }
  }
}


void fn_rev(FN_LIST *list)
{
  if (list==NULL) return;
  
  if (list->count)
  {
    FN_ITM *itm = list->items;
    FN_ITM *next = (FN_ITM *)itm->next;
    while(next)
    {
      itm->next=next->next;
      next->next=list->items;
      list->items = next;
      next = (FN_ITM *)itm->next;
    }
  }
}


int fn_inlist(FN_LIST *list, wchar_t *full)
{
  if (list==NULL || !list->count) return 0;
  
  FN_ITM *itm = list->items;
  while(itm)
  {
    if (!wstrcmp(itm->full, full)) return 1;  
    itm=(FN_ITM *)itm->next;
  }
  return 0;
}

void fn_add(FN_LIST* list, int type, int ftype, int pname, wchar_t* full, wchar_t* zipPath)
{
  if (list==NULL) return;
  
  if (type!=list->type)
  {
    if (list->count) fn_free(list);
    list->type = type;
  }
  
  if (!fn_inlist(list, full))
  {
    FN_ITM *itm = new FN_ITM;
    
    itm->ftype = ftype;
    itm->pname = pname;
    itm->full = new wchar_t[wstrlen(full)+1];
    wstrcpy(itm->full, full);
    if (zipPath != NULL)
    {
      itm->zipPath = new wchar_t[wstrlen(zipPath)+1];
      wstrcpy(itm->zipPath, zipPath);
    }
    else
    {
      itm->zipPath = NULL;
    }
    itm->next  = list->items;
    list->items = itm;
    list->count++;
  }
}

int _fn_fill(wchar_t *path, wchar_t *name, W_FSTAT *fs, int param)
{
  wchar_t *p=GetPathBuf();
  if (p)
  {
    int ftype = fs->attr & FA_DIRECTORY ? TYPE_COMMON_DIR : TYPE_COMMON_FILE;
    FN_LIST *list = (FN_LIST *) param;
    snwprintf(p, MAX_PATH-1, _ls_ls, path, name);
    fn_add(list, FNT_NONE, ftype, 0, p, NULL);
    return 1;
  }
  return 0;
}

void fn_fill(FN_LIST *list, wchar_t *path)
{
  if (list==NULL) return;
  
  int ftype = isdir(path) ? TYPE_COMMON_DIR : TYPE_COMMON_FILE;
  if (ftype == TYPE_COMMON_DIR)
    EnumFiles(path, _fn_fill, (int)list);
  fn_add(list, FNT_NONE, ftype, 0, path, NULL);
}
