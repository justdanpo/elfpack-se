#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
#include "header\structs.h"
#include "header\presets.h"

ITEM *lastitem=0;
extern LIST *csofts;

int isSoftkeysBook(BOOK *bk);
int getcount(LIST *lst, int check);
//------------------------------------------------------------------
ITEM *getbyname(wchar_t *name)
{
  if (!name)return 0;
  if (csofts)
  {
    int x=0;
    while(x<csofts->FirstFree)
    {
      ITEM *it=(ITEM*)csofts->listdata[x++];
      if (!it)break;
      if (!it->name)break;
      if (wstrcmp(it->name,name)==0)
      {
        return it;
      }
    }
  }
  return 0;
};

void getitem(BOOK *bk)
{
  wchar_t ws[100];
  char *bkname=bk->xbook->name;
  if (bkname[0]=='C')
  {
    if (strcmp(bkname,"CUIDisplayableBook")==0)
    {
      TextID_GetWString(bk->xbook->app_session->name,ws,99);
    }
    else goto L_NOT_JAVA;
  }
  else
  {
  L_NOT_JAVA:
    str2wstr(ws,bk->xbook->name);
  }
  if (lastitem)
  {
    if (wstrcmp(ws,lastitem->name)==0)
    {
      return;
    }
  }
  if (lastitem=getbyname(ws))
  {
    return;
  }
  else
  {
    lastitem=getbyname(L"DEFAULT");
  }
};
//------------------------------------------------------------------
bool get_enable(LIST *lst, int strid)
{
  int x=0;
  while(x<lst->FirstFree)
  {
    SOFTKEY *sk=(SOFTKEY*)lst->listdata[x++];
    if (sk->text==strid)
    {
      return sk->enable;
    }
  }
  return 1;
};

wchar_t get_action(LIST *lst, int strid)
{
  int x=0;
  while(x<lst->FirstFree)
  {
    SOFTKEY *sk=(SOFTKEY*)lst->listdata[x++];
    if (sk->text==strid)
    {
      return sk->action;
    }
  }
  return 0xF00;
};

void preset_calculate(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int left, int right)
{
  MyBOOK *our=(MyBOOK*)FindBook(isSoftkeysBook);
  int cnt=getcount(lst,1);
  if (GUIObject_GetDispObject(SBY_GetMainInput(Find_StandbyBook()))==DO)
  {
    lastitem->type=T_STANDBY;
    preset_standby(lbl,bk,DO, lst, cnt, our, left, right);
  }
  else
  {
    if (!isKeylocked())
    {
      if (our)
      {
        lbl->strids[2]=our->backstrid;
        lbl->enable[2]=true;
      }
    }
    bool isv=false;
    int x;
    for (x=0;x<3;x++)
    {
      if (presets[x*2])
      {
        bool (*isValid)(LIST *lst, int cnt, int left, int right)=(bool(*)(LIST *lst, int cnt, int left, int right))(presets[x*2]);
        void (*preset)(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)=(void(*)(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right))(presets[x*2+1]);
        if (isValid(lst,cnt,left,right))
        {
          lastitem->type=2+x;
          preset(lbl, bk, DO, lst, cnt, our, left, right);
          isv=true;
          break;
        }
      }
      else break;
    }
    if (isv==false)
    {
      lastitem->type=T_DEFAULT;
      preset_default(lbl, bk, DO, lst, cnt, our, left, right);
    }
  }
};
//------------------------------------------------------------------
LABELS *get_labels(DISP_OBJ *sk, DISP_OBJ *DO, BOOK *bk, int vis)
{
  if (DO==0 || bk==0 || sk==0)return 0;
  LABELS *ret=new LABELS;
  memset(ret,0,sizeof(LABELS));
  ret->strids[0]=EMPTY_TEXTID;
  ret->strids[1]=EMPTY_TEXTID;
  ret->strids[2]=EMPTY_TEXTID;
  
  SKLABEL left;
  SoftKeys_GetLabel(sk,&left, 0);
  SKLABEL right;
  right.text=0xFFFFFFFF;
  if (vis==2)
  {
    SoftKeys_GetLabel(sk,&right, 1);
  }
  if (lastitem && csofts)
  {
    LIST *lst = DispObject_SoftKeys_GetList(DO,bk, 0);
    if (lst)
    {
      if (lastitem->style==0)
      {
        ret->strids[2]=right.text;
        ret->enable[2]=get_enable(lst,right.text);
        ret->strids[0]=left.text;
        ret->enable[0]=get_enable(lst,left.text);
        ret->strids[1]=EMPTY_TEXTID;
        ret->enable[1]=1;
        if (get_action(lst, right.text)==ACTION_DONE && strcmp(bk->xbook->name,"Installer")==0) //поправка для окна ElfInstaller
        {
          ret->strids[0]=right.text;
          ret->strids[2]=left.text;
          int temp=ret->enable[0];
          ret->enable[0]=ret->enable[0];
          ret->enable[2]=temp;
        }
        return ret;
      }
      preset_calculate(ret, bk, DO, lst, left.text, right.text);
    }
  }
  return ret;
};
//------------------------------------------------------------------
