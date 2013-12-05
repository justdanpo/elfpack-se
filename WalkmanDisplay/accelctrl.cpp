#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "header\accelctrl.h"

/*   Accelerometer class for ElfPack
     Author: UltraShot
     Supported phones: W580 R8BE001, W580 R8BA024, W580 R8BE001 Orange, W580 R6CA001 Orange, W710 R1JC002
*/

#pragma optimize = z 9
void onAccTimer(u16 timerID, CAccelCtrl *cac)
{
  if (cac->getstatus()==ACC_ACTIVE)
  {
    LIST *acc_lst=cac->gethooks();
    if (acc_lst)
    {
      int x;
      for (x=0;x<acc_lst->FirstFree;x++)
      {
        void (*proc)(CAccelCtrl*,int,int)=(void (*)(CAccelCtrl*,int,int))(List_Get(acc_lst,x));
        char x,y;
        cac->GetCoords(&x,&y);
        if (x>100 && y>100)
          proc(cac,x,y);
      }
    }
  }
  Timer_ReSet(cac->gettimer(),1000,MKTIMERPROC(onAccTimer),cac);
};

CAccelCtrl::CAccelCtrl()
{
  hooks=List_Create();
  if (identify_phone())
  {
    timer=Timer_Set(1000, MKTIMERPROC(onAccTimer), this);
    status=ACC_ACTIVE;
  }
  else
  {
    status=ACC_NOT_SUPPORTED;
  }
};

CAccelCtrl::~CAccelCtrl(void)
{
  if (timer)
  {
    Timer_Kill(gettimer());
  }
  if (hooks)
  {
    while (hooks->FirstFree)
    {
      List_RemoveAt(hooks,0);
    }
    List_Destroy(hooks);
  }
  timer=0;
  hooks=0;
  status=ACC_INACTIVE;
};

int CAccelCtrl::identify_phone()
{
  char *hz=(char*)(0x453FEFD4);
  if (hz[0]==0x70 && hz[1]==0xB5)
  {
    //W580 R8BE001
    GetCoords=(void(*)(char * x,char * y))0x453FEFD5;
  }
  else if (hz[0]==0xA1 && hz[1]==0x6A)
  {
    //W580 R8BE001 Orange
    GetCoords=(void(*)(char * x,char * y))0x4543AD01;
  }
  else if (hz[0]==0x01 && hz[1]==0x28)
  {
    //W580 R8BA024
    GetCoords=(void(*)(char * x,char * y))0x453FD1B1;
  }
  else if (hz[0]==0xAC && hz[1]==0x33)
  {
    //W710 R1JC002
    GetCoords=(void(*)(char * x,char * y))0x453D80A9;
  }
  else if (hz[0]==0x01 && hz[1]==0x26)
  {
    //W580 R6CA001 Orange
    GetCoords=(void(*)(char * x,char * y))0x4542BE31;
  }
  else
  {
    return 0;
  }
  return 1;
};

int CAccelCtrl::getstatus()
{
  return status;
};

void CAccelCtrl::AddHook(void (*proc)(void*, int, int), char on)
{
  if (status!=ACC_ACTIVE)
    return;
  int i=ListElement_Prt2NumElement(hooks,(void*)proc);
  if (on)
  {
    if (i==LIST_ERROR)
    {
      List_InsertLast(hooks, (void*)proc);
    }
  }
  else
  {
    if (i!=LIST_ERROR)
    {
      List_RemoveAt(hooks, i);
    }
  }
};

bool CAccelCtrl::isbetween(char num, int min, int max)
{
  if (num >= min && num <=max)
  {
    return true;
  }
  return false;
};

LIST * CAccelCtrl::gethooks()
{
  return hooks;
};

u16 *CAccelCtrl::gettimer()
{
  return &timer;
};

int CAccelCtrl::getvector(int x, int y)
{
  int res=0;
  if (isbetween(x,120,130) && y<115)
    res=res|V_UP;
  else if (x>133 && isbetween(y,120,130))
    res=res|V_RIGHT;
  else if (isbetween(x,120,130) && y>133)
    res=res|V_DOWN;
  else if (x<115 && isbetween(y,120,130))
    res=res|V_LEFT;
  return res;
};

int CAccelCtrl::getorientation(int x, int y)
{
  if (isbetween(x, 116, 136) && isbetween(y, 116, 136))
    return 0;
  if (isbetween(x,112,144))
  {
    if (!isbetween(y, 132, 144))
    {
      return HOR;
    }
    return VERT;
  }
  else
  {
    return HOR;
  }
};
