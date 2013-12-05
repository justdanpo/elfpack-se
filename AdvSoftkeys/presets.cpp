#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
#include "header\structs.h"
#include "header\presets.h"
wchar_t get_action(LIST *lst, int strid);
bool get_enable(LIST *lst, int strid);
#define T_DEFAULT 1
#define T_YESNO 2
#define T_MESSAGEBOX 3
#define T_DDMENU 4
//------------------------------------------------------------------
//пресет для выпадающего меню:
bool lookslike_DDMenu(LIST *lst, int cnt, int left, int right)
{
  if (cnt==1)
  {
    wchar_t act=get_action(lst, left);
    if (act==ACTION_SELECT || act==ACTION_INFO)
    {
      return true;
    }
  }
  else if (cnt==2)
  {
    wchar_t act=get_action(lst, left);
    wchar_t act2=get_action(lst, right);
    if (act==ACTION_SELECT1 && act2==ACTION_INFO)
    {
      return true;
    }
  }
  return false;
};
void preset_DDMenu(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)
{
  lbl->strids[1]=left;
  lbl->enable[1]=get_enable(lst, left);
  //lbl->enable[1]=true;
  lbl->strids[0]=right;
  lbl->enable[0]=get_enable(lst, right);
};
//------------------------------------------------------------------
//пресет для Да/Нет окна:
bool lookslike_YesNo(LIST *lst, int cnt, int left, int right)
{
  if (cnt==2)
  {
    wchar_t act = get_action(lst, left);
    wchar_t act2 = get_action(lst,right);
    if (((act==ACTION_YES || act==ACTION_ACCEPT) && (act2==ACTION_NO || act2==ACTION_CANCEL))||((act2==ACTION_YES || act2==ACTION_ACCEPT) && (act==ACTION_NO || act==ACTION_CANCEL)))
    {
      return true;
    }
  }
  else if (cnt==1)
  {
    wchar_t act = get_action(lst, left);
    if (act==ACTION_CANCEL || act==ACTION_NO)
    {
      return true;
    }
  }
  return false;
};

void preset_YesNo(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)
{
  if (strcmp(bk->xbook->name,"ScreensaverBook")!=0)
  {
    wchar_t act = get_action(lst, left);
    wchar_t act2 = get_action(lst,right);
    int ct=0;
    if (left!=EMPTY_TEXTID)ct++;
    if (right!=EMPTY_TEXTID)ct++;
    if (ct==2)
    {
      if (act==ACTION_YES || act==ACTION_ACCEPT)
      {
        lbl->strids[1]=left;
        lbl->enable[1]=get_enable(lst, left);
        lbl->strids[2]=right;
        lbl->enable[2]=get_enable(lst, right);
      }
      else if (act2==ACTION_YES || act2==ACTION_ACCEPT)
      {
        lbl->strids[1]=right;
        lbl->enable[1]=get_enable(lst, right);
        lbl->strids[2]=left;
        lbl->enable[2]=get_enable(lst, left);
      }
    }
    else if (ct==1)
    {
      lbl->strids[2]=left;
      lbl->enable[2]=get_enable(lst, left);
    }
  }
  else
  {
    lbl->strids[0]=left;
    lbl->enable[0]=get_enable(lst,left);
    lbl->strids[2]=right;
    lbl->enable[2]=get_enable(lst,right);
  }
};
//------------------------------------------------------------------
//пресет для МессагБокса:
bool lookslike_OK(LIST *lst, int cnt, int left, int right)
{
  if (cnt==1 && right==EMPTY_TEXTID)
  {
    wchar_t act=get_action(lst, left);
    if (act==ACTION_OK)
    {
      return true;
    }
  }
  return false;
};
void preset_OK(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)
{
  if (strcmp(bk->xbook->name,"ScreensaverBook")!=0)
  {
    lbl->strids[1]=left;
    lbl->enable[1]=get_enable(lst,left);
    lbl->enable[2]=false;
    lbl->strids[2]=EMPTY_TEXTID;
  }
  else
  {
    lbl->strids[0]=left;
    lbl->enable[0]=get_enable(lst,left);
  }
};
//------------------------------------------------------------------
//Стандартный пресет:
void preset_default(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)
{
  if (lastitem)lastitem->type=T_DEFAULT;
  if (cnt==1 && right!=EMPTY_TEXTID)cnt++;
  wchar_t act1=get_action(lst, left);
  wchar_t act2=get_action(lst, right);
  lbl->strids[1]=left;
  lbl->enable[1]=get_enable(lst, left);
  lbl->strids[0]=right;
  lbl->enable[0]=get_enable(lst,right);
  if (right==EMPTY_TEXTID)
  {
    if (act1==ACTION_BACK || act1==ACTION_CANCEL || act1==ACTION_NO)
    {
      lbl->strids[2]=left;
      lbl->enable[2]=get_enable(lst, left);
      lbl->strids[1]=EMPTY_TEXTID;
      lbl->enable[1]=false;
    }
  }
  else
  {
    if (act2==ACTION_BACK || act2==ACTION_CANCEL || act2==ACTION_NO)
    {
      lbl->strids[2]=right;
      lbl->enable[2]=get_enable(lst, right);
      lbl->strids[0]=EMPTY_TEXTID;
      lbl->enable[0]=false;
    }
  }
  if (get_action(lst, right)==ACTION_DONE)
  {
    lbl->strids[1]=right;
    lbl->strids[0]=left;
    int temp=lbl->enable[1];
    lbl->enable[1]=lbl->enable[0];
    lbl->enable[0]=temp;
  }
};
//------------------------------------------------------------------
//Пресет для главного экрана:
void preset_standby(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right)
{
  if (lastitem)lastitem->type=0;
  int vis=MainInput_getVisible(SBY_GetMainInput(bk));
  if (vis==0)
  {
    if (cnt>0)
    {
      lbl->enable[1]=true;
      lbl->strids[0]=left;
      lbl->enable[0]=get_enable(lst, left);
      lbl->strids[2]=right;
      lbl->enable[2]=get_enable(lst, right);
    }
  }
  else
  {
    lbl->strids[2]=our->backstrid;
    lbl->enable[2]=true;
    preset_default(lbl,bk,DO,lst, cnt, our, left, right);
  }
};
//------------------------------------------------------------------
int presets[]={  (int)lookslike_YesNo,  (int)preset_YesNo,
                 (int)lookslike_OK,     (int)preset_OK,
                 (int)lookslike_DDMenu, (int)preset_DDMenu,
                 0,                0};
//------------------------------------------------------------------
