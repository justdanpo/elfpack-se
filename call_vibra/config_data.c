#include "..\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177


__root const CFG_HDR cfghdr1={CFG_LEVEL,"Connection",1,0};
  __root const CFG_HDR cfghdr1_1={CFG_CHECKBOX,"Vibra",0,0};
  __root const int vibra_connection=0;

  __root const CFG_HDR cfghdr1_2={CFG_UINT,"Duration",100,2000};
  __root const unsigned int dur_connection=500;
__root const CFG_HDR cfghdr2={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr3={CFG_LEVEL,"Start conversation",1,0};
  __root const CFG_HDR cfghdr3_1={CFG_CHECKBOX,"Vibra",0,0};
  __root const int vibra_start=0;

  __root const CFG_HDR cfghdr3_2={CFG_UINT,"Duration",100,2000};
  __root const unsigned int dur_start=500;
__root const CFG_HDR cfghdr4={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr5={CFG_LEVEL,"Minute signal",1,0};
  __root const CFG_HDR cfghdr5_1={CFG_CHECKBOX,"Vibra",0,0};
  __root const int vibra_conv=0;

  __root const CFG_HDR cfghdr5_2={CFG_UINT,"Duration",100,2000};
  __root const unsigned int dur_conv=500;

  __root const CFG_HDR cfghdr5_3={CFG_UINT,"Second",0,1800};
  __root const unsigned int second_conv=55;
__root const CFG_HDR cfghdr6={CFG_LEVEL,"",0,0};



