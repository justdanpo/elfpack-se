#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_STR_WIN1251,"Number for Cash Request",0,31};
__root const char CASHREQNUM[32]="*#";

__root const CFG_HDR cfghdr000={CFG_UINT,"Auto cash check, hours",0,24};
__root const unsigned int CHECK_HOURS=1;

__root const CFG_HDR cfghdr01={CFG_CHECKBOX,"Enable Cash Trace",0,0};
__root const int ENA_CASHTRACE = 1;

__root const CFG_HDR cfghdr08={CFG_CHECKBOX,"Enable Cash Trace logging",0,0};
__root const int ENA_CASHTRACELOG = 0;

__root const CFG_HDR cfghdr02={CFG_STR_WIN1251,"Cash pattern 0",0,31};
__root const char CashPat0[32]="alans #hrn";

__root const CFG_HDR cfghdr03={CFG_STR_WIN1251,"Cash pattern 1",0,31};
__root const char CashPat1[32]="onus #hrn";

__root const CFG_HDR cfghdr04={CFG_STR_WIN1251,"Cash pattern 2",0,31};
__root const char CashPat2[32]="# hv";

__root const CFG_HDR cfghdr05={CFG_STR_WIN1251,"Cash pattern 3",0,31};
__root const char CashPat3[32]="# Kb GPRS";

__root const CFG_HDR cfghdr19={CFG_COORDINATES,"Idle icon position",0,0};
__root const unsigned int IDLE_X=0;
__root const unsigned int IDLE_Y=95;

__root const CFG_HDR cfghdr_m12={CFG_LEVEL,"Setup Progressbar",1,0};
  __root const CFG_HDR cfghdr10={CFG_COLOR_INT,"Cash Progressbar 1",0,0};
  __root const int COLOR_CASHPB1=0x80FF0000;

  __root const CFG_HDR cfghdr11={CFG_COLOR_INT,"Cash Progressbar 2",0,0};
  __root const int COLOR_CASHPB2=0x8000FF00;

  __root const CFG_HDR cfghdr12={CFG_COLOR_INT,"Cash Progressbar 3",0,0};
  __root const int COLOR_CASHPB3=0x8000FFFF;

  __root const CFG_HDR cfghdr13={CFG_COLOR_INT,"Cash Progressbar 4",0,0};
  __root const int COLOR_CASHPB4=0x80FFFF00;

  __root const CFG_HDR cfghdr14={CFG_COLOR_INT,"Font Color",0,0};
  __root const int COLOR_TEXTPB=0xFFFFFFFF;

  __root const CFG_HDR cfghdr15={CFG_FONT,"Font Size",0,100};
  __root const unsigned int TEXT_FONTSZ=0x18;
__root const CFG_HDR cfghdr_m13={CFG_LEVEL,"",0,0};
