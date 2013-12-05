#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_CHECKBOX,"Divide Traf",0,0};
__root const int TrafDiv=1;


__root const CFG_HDR cfghdr1={CFG_CBOX,"Measure Unit",0,2};
__root const int TrafKb=0;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"Bytes","Kbytes"};


__root const CFG_HDR cfghdr12={CFG_CBOX,"Sort Order",0,2};
__root const int sort_order=0;
__root const CFG_CBOX_ITEM cfgcbox2[2]={"FIFO","LIFO"};


__root const CFG_HDR cfghdr2={CFG_UINT,"Traf Cost",0,1000000};
__root const unsigned int TrafCost=0;


__root const CFG_HDR cfghdr3={CFG_UINT,"Traf Round",0,1024};
__root const unsigned int TrafRound=1;


__root const CFG_HDR cfghdr4={CFG_UINT,"Day Time Start",0,24};
__root const unsigned int TimeDay=0;


__root const CFG_HDR cfghdr5={CFG_UINT,"Night Time Start",0,24};
__root const unsigned int TimeNight=0;


__root const CFG_HDR cfghdr6={CFG_UINT,"Day Traf Cost",0,1000000};
__root const unsigned int TrafCostDay=0;


__root const CFG_HDR cfghdr7={CFG_UINT,"Night Traf Cost",0,1000000};
__root const unsigned int TrafCostNight=0;


__root const CFG_HDR cfghdr8={CFG_UINT,"Holiday #1",0,7};
__root const unsigned int Hol1=0;


__root const CFG_HDR cfghdr9={CFG_UINT,"Holiday #2",0,7};
__root const unsigned int Hol2=0;


__root const CFG_HDR cfghdr10={CFG_UINT,"Holiday Traf Cost",0,1000000};
__root const unsigned int TrafCostHol=0;


__root const CFG_HDR cfghdr11={CFG_STR_WIN1251,"Money Unit",0,15};
__root const char strMoney[16]="rub";

