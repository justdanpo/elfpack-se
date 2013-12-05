#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177


CFGVAR CFG_HDR cfghdr1={CFG_LEVEL,"Fonts",1,0};
  CFGVAR CFG_HDR cfghdr1_1={CFG_FONT,"Clock Font",0,0};
  CFGVAR int font1=100;

  CFGVAR CFG_HDR cfghdr1_2={CFG_FONT,"Date Font",0,0};
  CFGVAR int font2=30;

  CFGVAR CFG_HDR cfghdr1_3={CFG_FONT,"WeekDay Font",0,0};
  CFGVAR int font3=35;
CFGVAR CFG_HDR cfghdr2={CFG_LEVEL,"",0,0};



CFGVAR CFG_HDR cfghdr3={CFG_LEVEL,"Windows",1,0};
  CFGVAR CFG_HDR cfghdr3_1={CFG_COORDINATES,"Clock",0,0};
  CFGVAR int x0=1;
  CFGVAR int y0=75;

  CFGVAR CFG_HDR cfghdr3_2={CFG_COORDINATES,"Date",0,0};
  CFGVAR int x1=1;
  CFGVAR int y1=175;

  CFGVAR CFG_HDR cfghdr3_3={CFG_COORDINATES,"WeekDay",0,0};
  CFGVAR int x2=1;
  CFGVAR int y2=215;

  CFGVAR CFG_HDR cfghdr3_4={CFG_COORDINATES,"Icons",0,0};
  CFGVAR int x3=1;
  CFGVAR int y3=260;
CFGVAR CFG_HDR cfghdr4={CFG_LEVEL,"",0,0};



CFGVAR CFG_HDR cfghdr5={CFG_INT,"Time correction",0,300};
CFGVAR int OFF_TIME_CORRECTION=10;



CFGVAR CFG_HDR cfghdr6={CFG_CHECKBOX,"Show seconds",0,0};
CFGVAR int ShowSeconds=0;
