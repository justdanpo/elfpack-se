#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr0={CFG_CHECKBOX,"Is Enabled",0,0};
CFGVAR int IS_ENABLED=1;

CFGVAR CFG_HDR cfghdr1={CFG_COORDINATES,"XY Position",0,0};
CFGVAR int X_POS=1;
CFGVAR int Y_POS=1;

CFGVAR CFG_HDR cfghdr_2={CFG_LEVEL,"Setup Colors",1,0};
  CFGVAR CFG_HDR cfghdr3={CFG_COLOR_INT,"Monitor Pen Color",0,0};
  CFGVAR unsigned int PEN_COLOR=0xFF000000;

  CFGVAR CFG_HDR cfghdr4={CFG_COLOR_INT,"Monitor Brush Color",0,0};
  CFGVAR unsigned int BRUSH_COLOR=0xFFFFFFFF;
CFGVAR CFG_HDR cfghdr_5={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr6={CFG_CBOX,"Show",0,3};
CFGVAR int SHOW_IF_LOCKED=0;
CFGVAR CFG_CBOX_ITEM cfgcbox1[3]={"If Unlocked","If Locked","Always"};

CFGVAR CFG_HDR cfghdr7={CFG_UINT,"Refresh Time",0,10000};
CFGVAR unsigned int REFRESH_TIME=500;

CFGVAR CFG_HDR cfghdr81={CFG_UTF16_STRING,"UTF16",0,511};
CFGVAR wchar_t  UTF16_TEST[512]=L"Превед16";

CFGVAR CFG_HDR cfghdr8={CFG_INT,"Signed int test",-100,100};
CFGVAR int INT_TEST=0;

CFGVAR CFG_HDR cfghdr9={CFG_STR_WIN1251,"Русские символы",0,63};
CFGVAR char WIN_TEST[64]="Превед";

CFGVAR CFG_HDR cfghdr10={CFG_STR_PASS,"PassStr test",0,63};
CFGVAR char PASS_TEST[64]="Парол";

CFGVAR CFG_HDR cfghdr11={CFG_TIME,"Time test",0,0};
CFGVAR TIME MY_TIME={23,11,3};

CFGVAR CFG_HDR cfghdr12={CFG_DATE,"Date test",0,0};
CFGVAR DATE MY_DATE={1988,10,4};

CFGVAR CFG_HDR cfghdr13={CFG_FONT,"Font test",0,0};
CFGVAR int FONT=0;

CFGVAR CFG_HDR cfghdr14={CFG_KEYCODE,"Тест клавиши",0,0};
CFGVAR int KEY=0;
CFGVAR int KEYMODE=0;

CFGVAR CFG_HDR cfghdr134={CFG_RECT,"Rect Test",0,0};
CFGVAR RECT rc={80,120,50,100};
