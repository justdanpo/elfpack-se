#include "..\\include\cfg_items.h"
#include "inc\config_data.h"
#include "inc\lng.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr_m00 = { CFG_LEVEL, psz_sm_common, 1, 0 };

  __root const CFG_HDR cfghdr0 = { CFG_CBOX, psz_savestate, 0, 2 };
  __root const int CONFIG_SAVE_PATH = 1;
  __root const CFG_CBOX_ITEM cfgcbox0[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr1 = { CFG_CBOX, psz_backexit, 0, 2 };
  __root const int CONFIG_BACK_EXIT = 0;
  __root const CFG_CBOX_ITEM cfgcbox1[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr2 = { CFG_CBOX, psz_sconchk, 0, 2 };
  __root const int CONFIG_CUR_DOWN_ON_CHECK = 1;
  __root const CFG_CBOX_ITEM cfgcbox2[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr3 = { CFG_CBOX, psz_loopnav, 0, 2 };
  __root const int CONFIG_LOOP_NAVIGATION_ENABLE = 1;
  __root const CFG_CBOX_ITEM cfgcbox3[] = { psz_no, psz_yes };
  
  __root const CFG_HDR cfghdr34 = { CFG_CBOX, "Show root folder", 0, 2 };
  __root const int SHOW_ROOT_FOLDER = 1;
  __root const CFG_CBOX_ITEM cfgcbox34[] = { psz_no, psz_yes }; 
__root const CFG_HDR cfghdr_m01 = { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr_m05 = { CFG_LEVEL, "Font setup", 1, 0 };

  __root const CFG_HDR cfghdr050 = { CFG_FONT, "Font files", 0, 2 };
  __root const int CONFIG_FONT_FILES = 0x35;

  __root const CFG_HDR cfghdr051 = { CFG_FONT, "Font other", 0, 2 };
  __root const int CONFIG_FONT_HEADER = 0x35;
        
  __root const CFG_HDR cfghdr052 = { CFG_FONT, "Font attributes", 0, 2 };
  __root const int CONFIG_FONT_ATTR = 0x35;
        
__root const CFG_HDR cfghdr_m06 = { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr_m10 = { CFG_LEVEL, psz_sm_gui, 1, 0 };

  __root const CFG_HDR cfghdr4 = { CFG_UINT, psz_scspeed, 0, 2000 };
  __root const int CONFIG_SCROLL_TEXT_SPEED = 50;

  __root const CFG_HDR cfghdr5 = { CFG_UINT, psz_scwait, 0, 200 };
  __root const int CONFIG_SCROLL_TEXT_WAIT = 400;

  __root const CFG_HDR cfghdr6 = { CFG_CBOX, psz_useboldf, 0, 2 };
  __root const int CONFIG_USE_BOLD_FONT = 1;
  __root const CFG_CBOX_ITEM cfgcbox6[] = { psz_no, psz_yes };

__root const CFG_HDR cfghdr_m11 = { CFG_LEVEL, "", 0, 0 };

__root const CFG_HDR cfghdr_m30 = { CFG_LEVEL, psz_sm_configs, 1, 0 };

  __root const CFG_HDR cfghdr11 = { CFG_CBOX, psz_loadicons, 0, 2 };
  __root const int CONFIG_LOAD_ICONS = 1;
  __root const CFG_CBOX_ITEM cfgcbox11[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr12 = { CFG_CBOX, psz_loadkeys, 0, 2 };
  __root const int CONFIG_LOAD_KEYS = 1;
  __root const CFG_CBOX_ITEM cfgcbox12[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr13 = { CFG_CBOX, psz_loadmui, 0, 2 };
  __root const int CONFIG_LOAD_MUI = 1;
  __root const CFG_CBOX_ITEM cfgcbox13[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr14 = { CFG_CBOX, psz_loadcs, 0, 2 };
  __root const int CONFIG_LOAD_CS = 1;
  __root const CFG_CBOX_ITEM cfgcbox14[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr15 = { CFG_UTF16_STRING, psz_configpath, 0, 63 };
  __root const wchar_t CONFIG_MCCONFIG_PATH[64] = L"";
        
__root const CFG_HDR cfghdr_m31 = { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr_m40 = { CFG_LEVEL, psz_sm_confirms, 1, 0 };

  __root const CFG_HDR cfghdr16 = { CFG_CBOX, psz_confirmexit, 0, 2 };
  __root const int CONFIG_CONFIRM_EXIT = 1;
  __root const CFG_CBOX_ITEM cfgcbox16[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr17 = { CFG_CBOX, psz_confirmdel, 0, 2 };
  __root const int CONFIG_CONFIRM_DELETE = 1;
  __root const CFG_CBOX_ITEM cfgcbox17[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr18 = { CFG_CBOX, psz_confirmdelr, 0, 2 };
  __root const int CONFIG_CONFIRM_DELETERO = 1;
  __root const CFG_CBOX_ITEM cfgcbox18[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr19 = { CFG_CBOX, psz_confirmcopy, 0, 2 };
  __root const int CONFIG_CONFIRM_COPY = 1;
  __root const CFG_CBOX_ITEM cfgcbox19[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr20 = { CFG_CBOX, psz_confirmmove, 0, 2 };
  __root const int CONFIG_CONFIRM_MOVE = 1;
  __root const CFG_CBOX_ITEM cfgcbox20[] = { psz_no, psz_yes };

  __root const CFG_HDR cfghdr21 = { CFG_CBOX, psz_confirmrepl, 0, 2 };
  __root const int CONFIG_CONFIRM_REPLACE = 1;
  __root const CFG_CBOX_ITEM cfgcbox21[] = { psz_no, psz_yes };
__root const CFG_HDR cfghdr_m41 = { CFG_LEVEL, "", 0, 0 };


__root const CFG_HDR cfghdr_m60 = { CFG_LEVEL, psz_sm_archive, 1, 0 };

  __root const CFG_HDR cfghdr_m62 = { CFG_LEVEL, psz_sm_zip, 1, 0 };
  
    __root const CFG_HDR cfghdr24 = { CFG_CBOX, psz_zip_enable, 0, 2 };
    __root const int CONFIG_ZIP_ENABLE = 1;
    __root const CFG_CBOX_ITEM cfgcbox24[] = { psz_no, psz_yes };

    __root const CFG_HDR cfghdr25 = { CFG_CBOX, psz_zip_detect_by, 0, 2 };
    __root const int CONFIG_ZIP_DETECT_BY = 0;
    __root const CFG_CBOX_ITEM cfgcbox25[] = { psz_content, psz_extension };
   __root const CFG_HDR cfghdr_m63 = { CFG_LEVEL, "", 0, 0 };
  __root const CFG_HDR cfghdr_m64 = { CFG_LEVEL, psz_sm_7z, 1, 0 };
  
    __root const CFG_HDR cfghdr724 = { CFG_CBOX, psz_7z_enable, 0, 2 };
    __root const int CONFIG_7Z_ENABLE = 1;
    __root const CFG_CBOX_ITEM cfgcbox724[] = { psz_no, psz_yes };

    __root const CFG_HDR cfghdr725 = { CFG_CBOX, psz_7z_detect_by, 0, 2 };
    __root const int CONFIG_7Z_DETECT_BY = 0;
    __root const CFG_CBOX_ITEM cfgcbox725[] = { psz_content, psz_extension };

   __root const CFG_HDR cfghdr_m65 = { CFG_LEVEL, "", 0, 0 };
 
  __root const CFG_HDR cfghdr26 = { CFG_UTF16_STRING, psz_temppath, 0, 63 };
  __root const wchar_t CONFIG_TEMP_PATH[64] = L"";
  
  __root const CFG_HDR cfghdr27 = { CFG_CBOX, psz_cleartemp, 0, 2 };
  __root const int CONFIG_DELETE_TEMP_FILES_ON_EXIT = 0;
  __root const CFG_CBOX_ITEM cfgcbox27[] = { psz_no, psz_yes };
__root const CFG_HDR cfghdr_m61 = { CFG_LEVEL, "", 0, 0 };
