#include "..\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

CFGVAR CFG_HDR cfghdr0={CFG_KEYCODE,"Start Button",0,0};
CFGVAR int KeyActiv=0xFFFFFFFF;
CFGVAR int KeyPressMode=0;


CFGVAR CFG_HDR cfghdr1={CFG_LEVEL,"Tabs images",1,0};
  CFGVAR CFG_HDR cfghdr1_1={CFG_UTF16_STRING,"1 tab active",0,99};
  CFGVAR wchar_t tab1_act[100]=L"";

  CFGVAR CFG_HDR cfghdr1_2={CFG_UTF16_STRING,"1 tab inactive",0,99};
  CFGVAR wchar_t tab1_inact[100]=L"";

  CFGVAR CFG_HDR cfghdr1_3={CFG_UTF16_STRING,"2 tab active",0,99};
  CFGVAR wchar_t tab2_act[100]=L"";

  CFGVAR CFG_HDR cfghdr1_4={CFG_UTF16_STRING,"2 tab inactive",0,99};
  CFGVAR wchar_t tab2_inact[100]=L"";
CFGVAR CFG_HDR cfghdr2={CFG_LEVEL,"",0,0};


CFGVAR CFG_HDR cfghdr3={CFG_LEVEL,"Digits images",1,0};
  CFGVAR CFG_HDR cfghdr3_1={CFG_UTF16_STRING,"Digit 0",0,99};
  CFGVAR wchar_t digit0[100]=L"";

  CFGVAR CFG_HDR cfghdr3_2={CFG_UTF16_STRING,"Digit 1",0,99};
  CFGVAR wchar_t digit1[100]=L"";

  CFGVAR CFG_HDR cfghdr3_3={CFG_UTF16_STRING,"Digit 2",0,99};
  CFGVAR wchar_t digit2[100]=L"";

  CFGVAR CFG_HDR cfghdr3_4={CFG_UTF16_STRING,"Digit 3",0,99};
  CFGVAR wchar_t digit3[100]=L"";

  CFGVAR CFG_HDR cfghdr3_5={CFG_UTF16_STRING,"Digit 4",0,99};
  CFGVAR wchar_t digit4[100]=L"";

  CFGVAR CFG_HDR cfghdr3_6={CFG_UTF16_STRING,"Digit 5",0,99};
  CFGVAR wchar_t digit5[100]=L"";

  CFGVAR CFG_HDR cfghdr3_7={CFG_UTF16_STRING,"Digit 6",0,99};
  CFGVAR wchar_t digit6[100]=L"";

  CFGVAR CFG_HDR cfghdr3_8={CFG_UTF16_STRING,"Digit 7",0,99};
  CFGVAR wchar_t digit7[100]=L"";

  CFGVAR CFG_HDR cfghdr3_9={CFG_UTF16_STRING,"Digit 8",0,99};
  CFGVAR wchar_t digit8[100]=L"";

  CFGVAR CFG_HDR cfghdr3_10={CFG_UTF16_STRING,"Digit 9",0,99};
  CFGVAR wchar_t digit9[100]=L"";
CFGVAR CFG_HDR cfghdr4={CFG_LEVEL,"",0,0};


CFGVAR CFG_HDR cfghdr5={CFG_CHECKBOX,"Ignore Keylock",0,0};
CFGVAR int Ignore_KeyLock=0;


CFGVAR CFG_HDR cfghdr6={CFG_CHECKBOX,"Display elfs on 1 tab",0,0};
CFGVAR int ElfInBookListEnabled=1;


CFGVAR CFG_HDR cfghdr7={CFG_CHECKBOX,"Close when inactive",0,0};
CFGVAR int UserInactivityEventEnabled=1;


CFGVAR CFG_HDR cfghdr8={CFG_CBOX,"Show after shortcut",0,2};
CFGVAR int StandbyOnTop=0;
CFGVAR CFG_CBOX_ITEM cfgcbox2[2]={"Current book","Standby"};


CFGVAR CFG_HDR cfghdr9={CFG_CBOX,"Active Tab",0,3};
CFGVAR int FirstTab=0;
CFGVAR CFG_CBOX_ITEM cfgcbox3[3]={"Last Active","Books","Elfs"};
