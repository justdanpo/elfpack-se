#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "language.h"
#include "nums.h"

#pragma diag_suppress=Pe177

// --------------------[ Control settings ]---------------------------------- //
__root const CFG_HDR cfghdr_setctrl={CFG_LEVEL,LG_GFGSETCTRL,1,0};
  __root const CFG_HDR cfghdr_ctrlmode={CFG_CBOX,LG_GFGCTRLMODE,0,2};
  __root const int cfg_ctrlmode = 0;
  __root const CFG_CBOX_ITEM cfgcbox_ctrlmode[2]={LG_GFGMENUONLY,LG_GFGSETBUTTONSONLY};

  __root const CFG_HDR cfghdr_menukey={CFG_KEYCODE,LG_GFGMENUKEY,0,0};
  __root const int cfg_menukey = 0;
  __root const int cfg_menukeymode = 0;

  __root const CFG_HDR cfghdr_buttons={CFG_LEVEL,LG_GFGSETBUTTONS,1,0};

    __root const CFG_HDR cfghdr_key={CFG_KEYCODE,LG_GFGSHOWKEY,0,0};
    __root const int cfg_showkey = 0;
    __root const int cfg_showkeymode = 0;
  
    __root const CFG_HDR cfghdr_show={CFG_KEYCODE,LG_GFGCELLIDNAMEKEY,0,0};
    __root const int cfg_showcellid = 0;
    __root const int cfg_chowcellidmode = 0;
  
    __root const CFG_HDR cfghdr_auto_1={CFG_KEYCODE,LG_GFGAUTOLOCATIONKEY,0,0};
    __root const int cfg_autolocation = 0;
    __root const int cfg_autolocationmode = 0;

  __root const CFG_HDR cfghdr_buttons_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_lock={CFG_CHECKBOX,LG_GFGIGNOREKEYLOCK,0,0};
  __root const int cfg_ignore_keylock = 0;

__root const CFG_HDR cfghdr_setctrl_x={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// --------------------[ Main settings ]------------------------------------- //
__root const CFG_HDR cfghdr_setmain={CFG_LEVEL,LG_GFGSETMAIN,1,0};

  __root const CFG_HDR cfghdr_show_type={CFG_CBOX,LG_GFGSHOW,0,4};
  __root const int cfg_show_type = 1;
  __root const CFG_CBOX_ITEM cfgcbox_show_type[4]={LG_CFGNOTHING,LG_CFGNAMEONLY,LG_CFGIMAGEONLY,LG_CFGNAMEIMAGE};

  __root const CFG_HDR cfghdr_cellidmode={CFG_CBOX,LG_GFGCELLIDMODE,0,2};
  __root const int cfg_cellidmode = 0;
  __root const CFG_CBOX_ITEM cfgcbox_cellidmode[2]={LG_GFGHEX,LG_GFGDEC};

  __root const CFG_HDR cfghdr_location={CFG_CBOX,LG_GFGLOCATION,0,10};
  __root const int cfg_location = 9;
  __root const CFG_CBOX_ITEM cfgcbox_location[10]={LG_GFGFIRSTOPERATORLINE,
                                                   LG_GFGSECONDOPERATORLINE,
                                                   LG_GFGTHIRDOPERATORLINE,
                                                   LG_GFGNOTESBOX,
                                                   LG_GFGDATE,
                                                   LG_GFGBIGCLOCK,
                                                   LG_GFGCLOCK,
                                                   LG_GFGBOTTOMLEFT,
                                                   LG_GFGABOVEBOTTOMLEFT,
                                                   LG_GFGCUSTOM};

  __root const CFG_HDR cfghdr_customlocation={CFG_LEVEL,LG_GFGCUSTOMLOCATION,1,0};
    __root const CFG_HDR cfghdr_cell_1={CFG_FONT,LG_GFGFONT,0,0};
    __root const unsigned int cfg_cell_font=0x18;
    __root const CFG_HDR cfghdr_cell_2={CFG_COLOR_INT,LG_GFGCOLOR,0,0};
    __root const unsigned int cfg_cell_color=0xFFFFFFFF;
    __root const CFG_HDR cfghdr_cell_3={CFG_COLOR_INT,LG_GFGBORDER,0,0};
    __root const unsigned int cfg_cell_border=0xFF000000;
    __root const CFG_HDR cfghdr_cell_4={CFG_CBOX,LG_GFGALIGN,0,3};
    __root const int cfg_cell_align = 2;
    __root const CFG_CBOX_ITEM cfgcbox_cell_5[3]={LG_GFGLEFT,LG_GFGRIGHT,LG_GFGCENTER};
    __root const CFG_HDR cfghdr_highlight={CFG_CHECKBOX,LG_GFGHIGHLIGHT,0,0};
    __root const int cfg_highlight = 1;
    __root const CFG_HDR cfghdr_cell_6={CFG_COORDINATES,LG_GFGPOSITION,0,0};
    __root const unsigned int cfg_cell_x = 0;
    __root const unsigned int cfg_cell_y = 40;
  __root const CFG_HDR cfghdr_customlocation_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_vibrate={CFG_CBOX,LG_GFGVIBRATEONCHANGE,0,4};
  __root const int cfg_vibrate = 0;
  __root const CFG_CBOX_ITEM cfgcbox_vibrate[4]={LG_GFGNEVERVIBRATE,
                                                 LG_GFGVIBRATE3X70,
                                                 LG_GFGVIBRATE1X250,
                                                 LG_GFGVIBRATE1X500};

  __root const CFG_HDR cfghdr_new_action={CFG_UINT,LG_GFGNEWACTIONTIMEOUT,1,10000};
  __root const unsigned int cfg_new_action_timeout = 10;

  __root const CFG_HDR cfghdr_auto={CFG_LEVEL,LG_GFGAUTOLOCATION,1,0};

    __root const CFG_HDR cfghdr_auto_image_1={CFG_CHECKBOX,LG_GFGSHOW,0,0};
    __root const int cfg_auto_image_show = 0;
    __root const CFG_HDR cfghdr_auto_image_2={CFG_COORDINATES,LG_GFGPOSITION,0,0};
    __root const unsigned int cfg_auto_image_x = 0;
    __root const unsigned int cfg_auto_image_y = 0;
    __root const CFG_HDR cfghdr_auto_file={CFG_UTF16_STRING,LG_GFGFILE,0,255};
    __root const wchar_t cfg_auto_file[256]=L"%ELFS_INT%/MiniGPS/Images/auto.png";

  __root const CFG_HDR cfghdr_auto_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_images={CFG_LEVEL,LG_GFGIMGAGES,1,0};

    __root const CFG_HDR cfghdr_location_image={CFG_RECT,LG_GFGPOSITIONSIZE,0,0};
    __root const RECT cfg_location_image = {0,60,0,60};
    __root const CFG_HDR cfghdr_images_path ={CFG_UTF16_STRING,LG_GFGPATHIMG,0,255};
    __root const wchar_t cfg_images_path[256]=L"%ELFS_INT%/MiniGPS/Images";

  __root const CFG_HDR cfghdr_images_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_location_file ={CFG_UTF16_STRING,LG_GFGLOCATIONFILE,0,255};
  __root const wchar_t cfg_location_file[256]=L"%ELFS_INT%/MiniGPS/db/MiniGPS.db";

__root const CFG_HDR cfghdr_setmain_x={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// --------------------[ Action settings ]----------------------------------- //
__root const CFG_HDR cfghdr_setaction={CFG_LEVEL,LG_GFGSETACTIONS,1,0};

  __root const CFG_HDR cfghdr_default_profile={CFG_CBOX,LG_CFGDEFAULTPROFILE,0,8};
  __root const int cfg_default_profile = 0;
  __root const CFG_CBOX_ITEM cfgcbox_default_profile[8]={LG_GFGNONE,
                                                         LG_GFGNORMAL,
                                                         LG_GFGMEETING,
                                                         LG_GFGINCAR,
                                                         LG_GFGOUTDOORS,
                                                         LG_GFGHANDSFREE,
                                                         LG_GFGHOME,
                                                         LG_GFGOFFICE};

  __root const CFG_HDR cfghdr_manual_profile={CFG_LEVEL,LG_CFGMANUALPROFILES,1,0};

#define MANUAL_PROFILE(num) \
  __root const CFG_HDR cfghdr_manual_profile##num={CFG_CBOX,LG_CFGMANUALPROFILE NUM##num,0,8}; \
  __root const int cfg_manual_profile##num = 0; \
  __root const CFG_CBOX_ITEM cfgcbox_manual_profile##num[8]={LG_GFGNONE, \
                                                             LG_GFGNORMAL, \
                                                             LG_GFGMEETING, \
                                                             LG_GFGINCAR, \
                                                             LG_GFGOUTDOORS, \
                                                             LG_GFGHANDSFREE, \
                                                             LG_GFGHOME, \
                                                             LG_GFGOFFICE}
    MANUAL_PROFILE(1);
    MANUAL_PROFILE(2);
    MANUAL_PROFILE(3);
    MANUAL_PROFILE(4);
    MANUAL_PROFILE(5);
    MANUAL_PROFILE(6);
    MANUAL_PROFILE(7);

  __root const CFG_HDR cfghdr_manual_profile_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_places={CFG_LEVEL,LG_CFGPLACES,1,0};


#define PLACE(num) \
    __root const CFG_HDR cfghdr_place##num={CFG_LEVEL,LG_CFGPLACE NUM##num,1,0}; \
      __root const CFG_HDR cfghdr_place_enable##num={CFG_CHECKBOX,LG_CFGPLACEENABLE,0,0}; \
      __root const int cfg_place_enable##num = 0; \
      __root const CFG_HDR cfghdr_place_name##num={CFG_UTF16_STRING,LG_CFGPLACENAME,0,127}; \
      __root const wchar_t cfg_place_name##num[128]=L"Place" LNUM##num; \
      __root const CFG_HDR cfghdr_place_profile##num={CFG_CBOX,LG_CFGPROFILE,0,8}; \
      __root const int cfg_place_profile##num = 0; \
      __root const CFG_CBOX_ITEM cfgcbox_place_profile##num[8]={LG_GFGNONE, \
                                                                LG_GFGNORMAL, \
                                                                LG_GFGMEETING, \
                                                                LG_GFGINCAR, \
                                                                LG_GFGOUTDOORS, \
                                                                LG_GFGHANDSFREE, \
                                                                LG_GFGHOME, \
                                                                LG_GFGOFFICE}; \
      __root const CFG_HDR cfghdr_place_vibrate##num={CFG_CBOX,LG_CFGVIBRATE,0,4}; \
      __root const int cfg_place_vibrate##num = 0; \
      __root const CFG_CBOX_ITEM cfgcbox_place_vibrate##num[4]={LG_GFGNEVERVIBRATE, \
                                                                LG_GFGVIBRATE3X70, \
                                                                LG_GFGVIBRATE1X250, \
                                                                LG_GFGVIBRATE1X500}; \
      __root const CFG_HDR cfghdr_place_sound##num={CFG_LEVEL,LG_CFGSOUND,1,0}; \
        __root const CFG_HDR cfghdr_place_sound_enable##num={CFG_CHECKBOX,LG_CFGENABLESOUND,0,0}; \
        __root const int cfg_place_sound_enable##num = 0; \
        __root const CFG_HDR cfghdr_sound_file##num ={CFG_UTF16_STRING,LG_CFGFILE,0,255}; \
        __root const wchar_t cfg_sound_file##num[256]=L"%ELFS_INT%/MiniGPS/Sound/sound.mp3"; \
      __root const CFG_HDR cfghdr_place_sound_x##num ={CFG_LEVEL,"",0,0}; \
    __root const CFG_HDR cfghdr_place_x##num ={CFG_LEVEL,"",0,0}

    PLACE(1);
    PLACE(2);
    PLACE(3);
    PLACE(4);
    PLACE(5);
    PLACE(6);
    PLACE(7);
    PLACE(8);
    PLACE(9);
    PLACE(10);
    PLACE(11);
    PLACE(12);
    PLACE(13);
    PLACE(14);
    PLACE(15);
    PLACE(16);

  __root const CFG_HDR cfghdr_places_x={CFG_LEVEL,"",0,0};

//  __root const CFG_HDR cfghdr_sound_path={CFG_UTF16_STRING,LG_GFGPATHSOUND,0,255};
//  __root const wchar_t cfg_sound_path[256]=L"%ELFS_INT%/MiniGPS/Sound";

//  __root const CFG_HDR cfghdr_images_path={CFG_UTF16_STRING,LG_GFGPATHIMG,0,255};
//  __root const wchar_t cfg_images_path[256]=L"%ELFS_INT%/MiniGPS/Images";

__root const CFG_HDR cfghdr_setaction_x={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// --------------------[ Log settings ]-------------------------------------- //
__root const CFG_HDR cfghdr_setlog={CFG_LEVEL,LG_GFGSETLOG,1,0};

  __root const CFG_HDR cfghdr_logmode={CFG_CBOX,LG_GFGLOGMODE,0,5};
  __root const int cfg_log_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox_logmode[5]={LG_GFGNEVERLOG,
                                                 LG_GFGEVERYCHANGELOG,
                                                 LG_GFGNAMECHANGELOG,
                                                 LG_GFGKNOWNLOG,
                                                 LG_GFGUNKNOWNLOG};

  __root const CFG_HDR cfghdr_date_format={CFG_CBOX,LG_GFGDATEFORMAT,0,2};
  __root const int cfg_date_format = 0;
  __root const CFG_CBOX_ITEM cfgcbox_date_format[2]={"YYYY-MM-DD","DD-MM-YYYY"};

  __root const CFG_HDR cfghdr_logext={CFG_CBOX,LG_GFGLOGEXT,0,2};
  __root const int cfg_log_ext = 0;
  __root const CFG_CBOX_ITEM cfgcbox3[2]={LG_GFGTXT,LG_GFGINI};

  __root const CFG_HDR cfghdr_logs_path={CFG_UTF16_STRING,LG_GFGPATHLOGS,0,255};
  __root const wchar_t cfg_logs_path[256]=L"%ELFS_INT%/MiniGPS/Logs";

__root const CFG_HDR cfghdr_setlog_x={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// --------------------[ Other settings ]------------------------------------ //
__root const CFG_HDR cfghdr_setother={CFG_LEVEL,LG_GFGSETOTHER,1,0};

  __root const CFG_HDR cfghdr_refresh={CFG_UINT,LG_GFGREFRESH,500,100000};
  __root const unsigned int cfg_update_interval = 2000;

  __root const CFG_HDR cfghdr_standby_only_0={CFG_CHECKBOX,LG_GFGSTANDBAYONLY,0,0};
  __root const int cfg_standby_only = 1;
  
  __root const CFG_HDR cfghdr_gdfs={CFG_CHECKBOX,LG_GFGSETINGDFS,0,0};
  __root const int cfg_set_in_gdfs = 1;

  __root const CFG_HDR cfghdr_silent={CFG_CHECKBOX,LG_GFGSILENT,0,0};
  __root const int cfg_silent = 0;

  __root const CFG_HDR cfghdr_setencode={CFG_LEVEL,LG_GFGSETENCODING,1,0};

    __root const CFG_HDR cfghdr_encodetype={CFG_CBOX,LG_GFGENCODETYPE,0,2};
    __root const int cfg_encode_type=0;
    __root const CFG_CBOX_ITEM cfgcbox_encodetype[2]={LG_GFGENCODEFILE, LG_GFGENCODEMAP};

    __root const CFG_HDR cfghdr_encode_file={CFG_UTF16_STRING,LG_GFGPATHENCODE,0,255};
    __root const wchar_t cfg_encode_file[256]=L"%ELFS_INT%/MiniGPS/Encode/cyrillic.bin";

    __root const CFG_HDR cfghdr_codemap_addr={CFG_UTF16_STRING,LG_GFGCODEMAPADDR,10,15};
    __root const wchar_t cfg_codemap_addr[16]=L"0x";

  __root const CFG_HDR cfghdr_setencode_x={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_bcfgedit={CFG_UTF16_STRING,LG_GFGPATHBCFGEDIT,0,255};
  __root const wchar_t cfg_bcfgedit[256]=L"%ELFS_INT%/BcfgEdit.elf";

__root const CFG_HDR cfghdr_setother_x={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

