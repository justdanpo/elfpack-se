#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr1={CFG_UINT,"On time, ms",0,10000};
__root const unsigned int OnTime=500;

__root const CFG_HDR cfghdr2={CFG_UINT,"Off time, ms",0,10000};
__root const unsigned int OffTime=500;
