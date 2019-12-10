#include "..\..\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

//------------weather.co.ua----------------------
CFGVAR CFG_HDR cfghdr_0000b={CFG_LEVEL, "weather.co.ua", 1, 0};

   CFGVAR CFG_HDR cfghdr_city={CFG_UTF16_STRING, "City", 0, 255};
   CFGVAR wchar_t CITY[255+1] = L"Челябинск";

   CFGVAR CFG_HDR cfghdr1={CFG_COORDINATES, "XY Position", 0, 0};
   CFGVAR int X_POS = 10;
   CFGVAR int Y_POS = 90;

   CFGVAR CFG_HDR cfghdr13={CFG_FONT, "Font" ,0,0};
   CFGVAR int FONT = 20;

   CFGVAR CFG_HDR cfghdr3={CFG_COLOR_INT, "Font color", 0, 0};
   CFGVAR unsigned int COLOR = 0xFFFFFFFF;
   
   CFGVAR CFG_HDR cfghdr_ts4={CFG_COLOR_INT,"Stroke text color",0,0};
   CFGVAR int H_COLOR = 0xFF000000;
   
   CFGVAR CFG_HDR cfghdr_bord_tipe={CFG_CBOX,"Stroke text type",0,4};
   CFGVAR int H_TYPE = 0;
   CFGVAR CFG_CBOX_ITEM cfgcbox_bord_tipe_x[4] = {"No","v.1","v.2","Shadow"};

   CFGVAR CFG_HDR cfghdr_2={CFG_LEVEL, "Info", 1, 0};
   
      CFGVAR CFG_HDR cfghdr_01={CFG_CHECKBOX,"Temperature",0,0};
      CFGVAR int IS_TEMP = 1;
      CFGVAR CFG_HDR cfghdr_03={CFG_CHECKBOX,"Wind",0,0};
      CFGVAR int IS_W = 1;
      CFGVAR CFG_HDR cfghdr_05={CFG_CHECKBOX,"Wind direction",0,0};
      CFGVAR int IS_WD = 1;
      CFGVAR CFG_HDR cfghdr_02={CFG_CHECKBOX,"Pressure",0,0};
      CFGVAR int IS_P = 1;
      CFGVAR CFG_HDR cfghdr_04={CFG_CHECKBOX,"Humidity",0,0};
      CFGVAR int IS_H = 1;
      CFGVAR CFG_HDR cfghdr_06={CFG_CHECKBOX,"Use mask",0,0};
      CFGVAR int IS_MASK = 1;
      CFGVAR CFG_HDR cfghdr_mask={CFG_UTF16_STRING, "Mask", 0, 255};
      CFGVAR wchar_t MASK[255+1] = L"<t>°C\n\nВетер <d>, <w> м/с";
      //<t>°C\n<w> м/с\n<d>\n<p> мм рт.ст.\n<h>%%
   
   CFGVAR CFG_HDR cfghdr_5={CFG_LEVEL, "", 0, 0};
   
   CFGVAR CFG_HDR cfghdr_000b={CFG_LEVEL, "Clipart weather", 1, 0};
   
      CFGVAR CFG_HDR cfghdr_001={CFG_CHECKBOX, "Show clipart", 0, 0};
      CFGVAR int IS_SHOW_CLIPART = 1;
      CFGVAR CFG_HDR cfghdr_002={CFG_UTF16_STRING, "Clipart dir", 0, 255};
      CFGVAR wchar_t CLIPART_DIR[255+1] = L"/usb/other/ZBin/Config/XWeather/clipart";
      CFGVAR CFG_HDR cfghdr_003={CFG_COORDINATES, "XY Position", 0, 0};
      CFGVAR int CLIPART_X_POS = 100;
      CFGVAR int CLIPART_Y_POS = 90;
      CFGVAR CFG_HDR cfghdr_004={CFG_INT, "Scale, %", 0, 1000};
      CFGVAR int SCALE = 100;
   
   CFGVAR CFG_HDR cfghdr_000e={CFG_LEVEL, "", 0, 0};
   
   CFGVAR CFG_HDR cfghdr_001b={CFG_LEVEL, "Substrate", 1, 0};
   
      CFGVAR CFG_HDR cfghdr_0012={CFG_CHECKBOX, "Use substrate", 0, 0};
      CFGVAR int IS_SUBSTRATE = 1;
      CFGVAR CFG_HDR cfghdr_0010={CFG_RECT, "Size", 0, 0};
      CFGVAR RECT S_RECT = {1, 141, 83, 147};
      CFGVAR CFG_HDR cfghdr_0011={CFG_COLOR_INT, "Color", 0, 0};
      CFGVAR unsigned int S_COLOR = 0xA0483CA3;
      CFGVAR CFG_HDR cfghdr_0014={CFG_CHECKBOX, "Stroke", 0, 0};
      CFGVAR int IS_STROKE = 1;
      CFGVAR CFG_HDR cfghdr_0013={CFG_COLOR_INT, "Stroke color", 0, 0};
      CFGVAR unsigned int STROKE_COLOR = 0xFF000000;
      CFGVAR CFG_HDR cfghdr_0015={CFG_CHECKBOX, "Use image", 0, 0};
      CFGVAR int IS_SUBSTRATE_IMAGE = 1;
      CFGVAR CFG_HDR cfghdr_0016={CFG_COORDINATES, "XY Position image", 0, 0};
      CFGVAR int BG_X_POS = 75;
      CFGVAR int BG_Y_POS = 118;
      CFGVAR CFG_HDR cfghdr_0017={CFG_INT, "Scale image, %", 0, 1000};
      CFGVAR int BG_SCALE = 65;
   
   CFGVAR CFG_HDR cfghdr_001e={CFG_LEVEL, "", 0, 0};
   
   CFGVAR CFG_HDR cfghdr8={CFG_UINT, "Days forecast", 1, 5};
   CFGVAR unsigned int DAYS = 1;
   
   CFGVAR CFG_HDR cfghdr7={CFG_TIME, "Update interval", 0, 0};
   CFGVAR TIME UP_TIME = {6,0,0};

   CFGVAR CFG_HDR cfghdr_6={CFG_CHECKBOX, "Get weather after reconf.", 0, 0};
   CFGVAR int IS_RECONF = 0;
   
   CFGVAR CFG_HDR cfghdr_88={CFG_CHECKBOX, "Consolidate current position", 0, 0};
   CFGVAR int IS_CONS = 1;
   CFGVAR CFG_HDR cfghdr876={CFG_INT, "X", -400, 400};
   CFGVAR int CONS_X = 90;
   CFGVAR CFG_HDR cfghdr843={CFG_INT, "Y", -400, 400};
   CFGVAR int CONS_Y = -30;
   
CFGVAR CFG_HDR cfghdr_0000e={CFG_LEVEL, "", 0, 0};
//------------------------------------------------
