#pragma once


/*   Accelerometer class for ElfPack
     Author: UltraShot
     Supported phones: W580 R8BE001, W580 R8BA024, W580 R8BE001 Orange, W580 R6CA001 Orange, W710 R1JC002
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"


//Possible accelerometer statuses
#define ACC_INACTIVE 0
#define ACC_ACTIVE 1
#define ACC_NOT_SUPPORTED 2

//Possible display orientations
#define HOR 1
#define VERT 2

//Possible accelerometer vectors
#define GET_UP(a) a&1
#define GET_DOWN(a) (a>>1)&1
#define GET_LEFT(a) (a>>2)&1
#define GET_RIGHT(a) (a>>3)&1


#define V_UP 1
#define V_DOWN 2
#define V_LEFT 4
#define V_RIGHT 8

class CAccelCtrl
{
  LIST *hooks;
  u16 timer;
  int status;
  int identify_phone();
  void onTimer(u16 timerID, LPARAM lparam);
  bool isbetween(char num, int min, int max);
public:
  CAccelCtrl();
  ~CAccelCtrl();
  void AddHook(void (*proc)(void*, int, int), char on);
  void (*GetCoords)(char * x,char * y);
  int getstatus();
  LIST *gethooks();
  u16 *gettimer();
  int getvector(int x, int y);
  int getorientation(int x, int y);
};



