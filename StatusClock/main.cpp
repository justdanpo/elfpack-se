#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "conf_loader.h"

void redraw_init();
BOOK * MyBook_Create();

u16 timer;

void onTimer (u16 tmr, LPARAM lparam)
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  if (dt.time.sec==0)
  {
    DispObject_InvalidateRect(*StatusRow_p(),0);
  }
  Timer_ReSet(&timer,1000,onTimer,0);
}

int main()
{
  MyBook_Create();
  timer=Timer_Set(1000,onTimer,0);
  InitConfig();
  redraw_init();
  return 0;
}
