#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "..\\include\Colors.h"
#pragma diag_suppress=Pe177


__root const CFG_HDR cfghdr0={CFG_RECT,"Events rect",0,0};
__root const RECT rc={0,240,30,50};

__root const CFG_HDR cfghdr1={CFG_COLOR_INT,"Upper color",0,0};
__root const unsigned int upp_color=0xFF000055;

__root const CFG_HDR cfghdr2={CFG_COLOR_INT,"Lower color",0,0};
__root const unsigned int low_color=0x22000000;

__root const CFG_HDR cfghdr8={CFG_FONT,"Events font",0,0};
__root const int ev_font=0x18;

__root const CFG_HDR cfghdr9={CFG_COLOR_INT,"Events text color",0,0};
__root const unsigned int ev_color=0xBBFFFFFF;

__root const CFG_HDR cfghdr3={CFG_RECT,"Default rect",0,0};
__root const RECT rc2={0,240,0,20};

__root const CFG_HDR cfghdr4={CFG_FONT,"Text font",0,0};
__root const int text_font=0x17;

__root const CFG_HDR cfghdr5={CFG_COLOR_INT,"Text color",0,0};
__root const unsigned int text_color=clWhite;

__root const CFG_HDR cfghdr6={CFG_FONT,"Time font",0,0};
__root const int time_font=0x18;

__root const CFG_HDR cfghdr7={CFG_COLOR_INT,"Time color",0,0};
__root const unsigned int time_color=clWhite;

__root const CFG_HDR cfghdr10={CFG_COLOR_INT,"Missed bg color",0,0};
__root const unsigned int miss_color=0x22772222;


__root const CFG_HDR cfghdr11={CFG_UTF16_STRING,"String - Turned off",0,49};
__root const wchar_t str_off[50]=L"EventControl turned off";

__root const CFG_HDR cfghdr12={CFG_UTF16_STRING,"String - Events",0,49};
__root const wchar_t str_events[50]=L"Events";

__root const CFG_HDR cfghdr13={CFG_UTF16_STRING,"String - No events",0,49};
__root const wchar_t str_noevents[50]=L"No events today";
