#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "..\\include\Colors.h"
#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr1={CFG_COORDINATES,"Position",0,0};
__root const int cfg_x=240;
__root const int cfg_y=0;

__root const CFG_HDR cfghdr2={CFG_FONT,"Font",0,0};
__root const int cfg_font=0x18;

__root const CFG_HDR cfghdr3={CFG_CBOX,"Centering",0,3};
__root const int cfg_ct=1;
__root const CFG_CBOX_ITEM cfgcbox5[3]={"Left","Right","Center"};

__root const CFG_HDR cfghdr4={CFG_COLOR_INT,"Color",0,0};
__root const unsigned int cfg_color=clWhite;

__root const CFG_HDR cfghdr5={CFG_COLOR_INT,"Shadow Color",0,0};
__root const unsigned int cfg_shadow=clBlack;

__root const CFG_HDR cfghdr6={CFG_COLOR_INT,"Border Color",0,0};
__root const unsigned int cfg_border=0;

__root const CFG_HDR cfghdr7={CFG_INT,"Shadow offset",0,10};
__root const int cfg_shad=1;

__root const CFG_HDR cfghdr8={CFG_INT,"Border size",0,10};
__root const int cfg_bordsize=1;

__root const CFG_HDR cfghdr9={CFG_COLOR_INT,"BG color",0,0};
__root const unsigned int cfg_bg=0x9C000000;

__root const CFG_HDR cfghdr10={CFG_RECT,"BG rect",0,0};
__root const RECT cfg_rect={200,240,1,22};
