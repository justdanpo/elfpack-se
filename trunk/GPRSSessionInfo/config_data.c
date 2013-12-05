#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "..\\include\Colors.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_STR_WIN1251,"Currency name",0,10};
__root const char cfgCurrency[11]="рублей";

__root const CFG_HDR cfghdr1={CFG_INT,"Min package size (KB)",0,0xFFFFF};
__root const int cfgMinLength=100;

__root const CFG_HDR cfghdr2={CFG_INT,"Not counted traf (KB)",0,0xFFFFF};
__root const int cfgNotCountedTraf=5;

__root const CFG_HDR cfghdr3={CFG_LEVEL,"Cost Settings",1,0};

  __root const CFG_HDR cfghdr4={CFG_TIME,"Day start time",0,0};
      __root const TIME DAY_TIME={8,0,0};
      
  __root const CFG_HDR cfghdr5={CFG_INT,"(D)Cost for out-package",0,0xFFFFF};
      __root const int cfgDayCostOut=68;
      
  __root const CFG_HDR cfghdr6={CFG_INT,"(D)Cost for in-package",0,0xFFFFF};
      __root const int cfgDayCostIn=68;
      
  __root const CFG_HDR cfghdr7={CFG_INT,"(D)Cost for package",0,0xFFFFF};
      __root const int cfgDayCost=68;
      
  __root const CFG_HDR cfghdr8={CFG_INT,"(D)Package size (KB)",0,0xFFFFF};
      __root const int cfgDayLength=10000;
  
  __root const CFG_HDR cfghdr9={CFG_TIME,"Night start time",0,0};
      __root const TIME NIGHT_TIME ={20,0,0};
  
  __root const CFG_HDR cfghdr10={CFG_INT,"(N)Cost for out-package",0,0xFFFFF};
      __root const int cfgNightCostOut=68;
      
  __root const CFG_HDR cfghdr11={CFG_INT,"(N)Cost for in-package",0,0xFFFFF};
      __root const int cfgNightCostIn=68;
      
  __root const CFG_HDR cfghdr12={CFG_INT,"(N)Cost for package",0,0xFFFFF};
      __root const int cfgNightCost=68;
      
  __root const CFG_HDR cfghdr13={CFG_INT,"(N)Package size (KB)",0,0xFFFFF};
      __root const int cfgNightLength=10000;
    
__root const CFG_HDR cfghdr14={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr15={CFG_INT,"Show timeout (sec)",0,0xFFFFF};
__root const int cfgTimeout=10;

__root const CFG_HDR cfghdr16={CFG_CHECKBOX,"Divide Traf",0,0};
__root const int cfgDivide=0;
