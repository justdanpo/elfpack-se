#include "..\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr1_0={CFG_LEVEL,"Buttons",1,0};
  CFGVAR CFG_HDR cfghdr1_0_1={CFG_KEYCODE,"Start Button",0,0};
  CFGVAR int KeyStart=0xFFFFFFFF;
  CFGVAR int KeyStartPressMode=0;

  CFGVAR CFG_HDR cfghdr1_0_2={CFG_KEYCODE,"Minimize Button",0,0};
  CFGVAR int KeyMinimize=0xFFFFFFFF;
  CFGVAR int KeyMinimizePressMode=0;

  CFGVAR CFG_HDR cfghdr1_0_3={CFG_KEYCODE,"Close all Button",0,0};
  CFGVAR int KeyCloseAll=0xFFFFFFFF;
  CFGVAR int KeyCloseAllPressMode=0;

  CFGVAR CFG_HDR cfghdr1_0_4={CFG_KEYCODE,"Change Tab Button",0,0};
  CFGVAR int KeyChangeTab=0xFFFFFFFF;
  CFGVAR int KeyChangeTabPressMode=0;
CFGVAR CFG_HDR cfghdr1_1={CFG_LEVEL,"",0,0};


CFGVAR CFG_HDR cfghdr2_0={CFG_LEVEL,"Images",1,0};
  CFGVAR CFG_HDR cfghdr2_0_0={CFG_LEVEL,"Tabs images",1,0};
    CFGVAR CFG_HDR cfghdr2_0_0_1={CFG_UTF16_STRING,"1 tab active",0,99};
    CFGVAR wchar_t tab1_act[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_0_2={CFG_UTF16_STRING,"1 tab inactive",0,99};
    CFGVAR wchar_t tab1_inact[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_0_3={CFG_UTF16_STRING,"2 tab active",0,99};
    CFGVAR wchar_t tab2_act[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_0_4={CFG_UTF16_STRING,"2 tab inactive",0,99};
    CFGVAR wchar_t tab2_inact[100]=L"";
  CFGVAR CFG_HDR cfghdr2_0_1={CFG_LEVEL,"",0,0};


  CFGVAR CFG_HDR cfghdr2_0_2={CFG_LEVEL,"Digits images",1,0};
    CFGVAR CFG_HDR cfghdr2_0_2_0={CFG_UTF16_STRING,"Digit 0",0,99};
    CFGVAR wchar_t digit0[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_1={CFG_UTF16_STRING,"Digit 1",0,99};
    CFGVAR wchar_t digit1[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_2={CFG_UTF16_STRING,"Digit 2",0,99};
    CFGVAR wchar_t digit2[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_3={CFG_UTF16_STRING,"Digit 3",0,99};
    CFGVAR wchar_t digit3[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_4={CFG_UTF16_STRING,"Digit 4",0,99};
    CFGVAR wchar_t digit4[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_5={CFG_UTF16_STRING,"Digit 5",0,99};
    CFGVAR wchar_t digit5[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_6={CFG_UTF16_STRING,"Digit 6",0,99};
    CFGVAR wchar_t digit6[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_7={CFG_UTF16_STRING,"Digit 7",0,99};
    CFGVAR wchar_t digit7[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_8={CFG_UTF16_STRING,"Digit 8",0,99};
    CFGVAR wchar_t digit8[100]=L"";

    CFGVAR CFG_HDR cfghdr2_0_2_9={CFG_UTF16_STRING,"Digit 9",0,99};
    CFGVAR wchar_t digit9[100]=L"";
  CFGVAR CFG_HDR cfghdr2_0_3={CFG_LEVEL,"",0,0};
CFGVAR CFG_HDR cfghdr2_1={CFG_LEVEL,"",0,0};


CFGVAR CFG_HDR cfghdr3_0={CFG_LEVEL,"GUI Settings",1,0};
  CFGVAR CFG_HDR cfghdr3_0_0={CFG_LEVEL,"Books Tab Settings",1,0};
    CFGVAR CFG_HDR cfghdr3_0_0_0={CFG_CHECKBOX,"Show hidden",0,0};
    CFGVAR int books_show_hid=0;

    CFGVAR CFG_HDR cfghdr3_0_0_1={CFG_CHECKBOX,"Show NoGUI",0,0};
    CFGVAR int books_show_nogui=0;

    CFGVAR CFG_HDR cfghdr3_0_0_2={CFG_CBOX,"Show books as",0,2};
    CFGVAR int MINIMIZE_TO_SESSION=0;
    CFGVAR CFG_CBOX_ITEM cfgcbox3_0_0_2_0[2]={"Books","Sessions"};
  CFGVAR CFG_HDR cfghdr3_0_1={CFG_LEVEL,"",0,0};

  CFGVAR CFG_HDR cfghdr3_0_2={CFG_LEVEL,"Elfs Tab Settings",1,0};
    CFGVAR CFG_HDR cfghdr3_0_2_0={CFG_CHECKBOX,"Show hidden",0,0};
    CFGVAR int elfs_show_hid=0;

    CFGVAR CFG_HDR cfghdr3_0_2_1={CFG_CHECKBOX,"Show NoGUI",0,0};
    CFGVAR int elfs_show_nogui=0;
  CFGVAR CFG_HDR cfghdr3_0_4={CFG_LEVEL,"",0,0};
CFGVAR CFG_HDR cfghdr3_1={CFG_LEVEL,"",0,0};

CFGVAR CFG_HDR cfghdr4_0={CFG_CHECKBOX,"Ignore Keylock",0,0};
CFGVAR int Ignore_KeyLock=0;


CFGVAR CFG_HDR cfghdr6_0={CFG_CHECKBOX,"Close when inactive",0,0};
CFGVAR int UserInactivityEventEnabled=1;


CFGVAR CFG_HDR cfghdr7_0={CFG_CBOX,"Active Tab",0,3};
CFGVAR int FirstTab=0;
CFGVAR CFG_CBOX_ITEM cfgcbox8_0_0[3]={"Last Active","Books","Elfs"};


CFGVAR CFG_HDR cfghdr8_0={CFG_CBOX,"Shortcuts mode",0,2};
CFGVAR int ShortcutsMode=0;
CFGVAR CFG_CBOX_ITEM cfgcbox10_0_0[2]={"Tab-specific","Unified"};

