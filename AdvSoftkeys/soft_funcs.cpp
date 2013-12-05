#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
#include "header\structs.h"
#include "header\presets.h"

//------------------------------------------------------------------
//определяем, находится ли число между двумя другими числами:
bool isbetween(int num, int min, int max)
{
  if (num >= min && num <=max)
  {
    return true;
  }
  return false;
};
//------------------------------------------------------------------
//случайно не патч ли от IronMaster'а?
bool isInIronPatch(int act)
{
  return isbetween(act, 0xFFC, 0xFFF);
};
//------------------------------------------------------------------
int getelemindex(LIST *lst, int act)
{
  int x;
  for (x=0;x<lst->FirstFree;x++)
  {
    SOFTKEY *sk=(SOFTKEY*)List_Get(lst,x);
    if (sk->action==act)
    {
      return x;
    }
  }
  return -1;
};

int checkelement(SOFTKEY *sk, int check)
{
  if (sk->action==ACTION_INFO)
  {
    return 1;
  }
  else
  {
    if (sk->book)return 1;
    if (check)return 1;
    return 0;
  }
};
//------------------------------------------------------------------
int isvisible(LIST *lst, int index)
{
  SOFTKEY *sk=(SOFTKEY*)List_Get(lst,index);
  if (sk->parent_action==0xFFFF)
  {
    return 1;
  }
  else
  {
    int index=getelemindex(lst,sk->parent_action);
    if (index!=-1)
    {
      SOFTKEY *sk2=(SOFTKEY*)List_Get(lst,index);
      if (sk2->visible && sk2->text!=EMPTY_TEXTID)
      {
        return 1;
      }
    }
  }
  return 0;
};

int getcount(LIST *lst, int check)
{
  int res=0;
  int x;
  for (x=0;x<lst->FirstFree;x++)
  {
    SOFTKEY *sk=(SOFTKEY*)List_Get(lst,x);
    if (sk->visible && sk->action!=ACTION_BACK && sk->action!=ACTION_LONG_BACK && isvisible(lst,x) && isInIronPatch(sk->action)!=true && sk->text!=EMPTY_TEXTID && sk->parent_action==0xFFFF && checkelement(sk, check))
    {
      res++;
    }
  }
  return res;
};
//------------------------------------------------------------------
