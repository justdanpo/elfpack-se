#include "..\\include\cfg_items.h"
#include "Tamagochi.h"
#include "language.h"

#pragma diag_suppress=Pe177

//Конфигурация
__root const CFG_HDR cfghdr0={CFG_CBOX,LG_GFGHELLO,0,2};
__root const int ENA_HELLO_MSG=1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={LG_GFGNO,LG_GFGYES};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,LG_GFGSETSCREEN,1,0};

  __root const CFG_HDR cfghdr1={CFG_UINT,LG_GFGUPDATE,1,600};
  __root const unsigned int REFRESH=20;

//  __root const CFG_HDR cfghdr1_1 = {CFG_CBOX, LG_GFGVIEW, 0, 3};
//  __root const int cfgShowIn = 2;
//  __root const CFG_CBOX_ITEM cfgcbox1_0[3] = {LG_GFGLOCK, LG_GFGUNLOCK, LG_GFGBOTH};

  __root const CFG_HDR cfghdr1_2={CFG_COORDINATES,LG_GFGPOSITION,0,0};
  __root const unsigned int POS_X=10;
  __root const unsigned int POS_Y=100;

//  __root const CFG_HDR cfghdr1_3_1 = {CFG_CHECKBOX, "Enable Effects", 0, 2};
//  __root const int Effects_Ena = 1;

  __root const CFG_HDR cfghdr1_3 = {CFG_UINT, LG_GFGMINSIZE, 0, 500};
  __root const unsigned int MINSIZE = 25;

  __root const CFG_HDR cfghdr1_4 = {CFG_UINT, LG_GFGMAXSIZE, 0, 500};
  __root const unsigned int MAXSIZE = 100;

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m21={CFG_LEVEL,LG_GFGSETPATH,1,0};

  __root const CFG_HDR cfghdr2_1={CFG_STR_WIN1251,LG_GFGPATHPET,0,63};
  __root const char PET_PATH[64]="%ELFS_INT%/Tamagochi";

  __root const CFG_HDR cfghdr2_2={CFG_STR_WIN1251,LG_GFGPATHROOM,0,63};
  __root const char ROOM_PATH[64]="%ELFS_INT%/Tamagochi/Room";

  __root const CFG_HDR cfghdr2_3={CFG_STR_WIN1251,LG_GFGPATHIMG,0,63};
  __root const char PIC_PATH[64]="%ELFS_INT%/Tamagochi/img";

  __root const CFG_HDR cfghdr2_4={CFG_STR_WIN1251,LG_GFGPATHSOUND,0,63};
  __root const char SOUND_PATH[64]="%ELFS_INT%/Tamagochi/Sound";

  __root const CFG_HDR cfghdr3_2={CFG_STR_WIN1251,LG_GFGPATHGAMELIST,0,63};
  __root const char GAMELIST_PATH[64]="%ELFS_INT%/Tamagochi/Game.list";

  __root const CFG_HDR cfghdr3_3={CFG_STR_WIN1251,LG_GFGPATHPLAYERLIST,0,63};
  __root const char PLAYERLIST_PATH[64]="%ELFS_INT%/Tamagochi/Player.list";

  __root const CFG_HDR cfghdr3_4={CFG_STR_WIN1251,LG_GFGPATHBCFGEDIT,0,63};
  __root const char BCFGEDIT_PATH[64]="%ELFS_INT%/BcfgEdit.elf";

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m31={CFG_LEVEL,LG_GFGSETPET,1,0};

  __root const CFG_HDR cfghdr3_1 = {CFG_CBOX, LG_GFGSPEED, 0, 3};
  __root const int SpeedLife = 1;
  __root const CFG_CBOX_ITEM cfgcbox3_0[3] = {LG_GFGFAST, LG_GFGMEDIUM, LG_GFGSLOW};

  __root const CFG_HDR cfghdr3_1_5={CFG_UINT,LG_GFGPARENT,1,600};
  __root const unsigned int PARENT_AGE=19;

  __root const CFG_HDR cfghdr3_1_6={CFG_UINT,LG_GFGPETCNT,MIN_PET_COUNT_GLB,MAX_PET_COUNT_GLB};
  __root const unsigned int MAX_PET_COUNT=8;

  __root const CFG_HDR cfghdr3_1_4 = {CFG_CHECKBOX, LG_GFGMOTION, 0, 2};
  __root const int Motion_Ena = 1;

  __root const CFG_HDR cfghdr3_1_3 = {CFG_CHECKBOX, LG_MUSIC, 0, 2};
  __root const int Music_Ena = 1;

  __root const CFG_HDR cfghdr3_1_2 = {CFG_CHECKBOX, LG_MALWARE, 0, 2};
  __root const int Malware_Ena = 1;

  __root const CFG_HDR cfghdr_m311={CFG_CHECKBOX,LG_SCANGAME,0,2};
  __root const int Scan_Game_Ena = 1;

  __root const CFG_HDR cfghdr_m321={CFG_LEVEL,LG_GFGNIGHT,1,0};

      __root const CFG_HDR cfghdr3_31 = {CFG_CHECKBOX, LG_GFGNIGHTENA, 0, 2};
      __root const int Night_Ena = 1;

      __root const CFG_HDR cfghdr3_32 = {CFG_UINT, LG_GFGNIGHTBEGIN, 0, 23};
      __root const unsigned int Night_begin = 22;

      __root const CFG_HDR cfghdr3_33 = {CFG_UINT, LG_GFGNIGHTEND, 0, 23};
      __root const unsigned int Night_end = 7;

  __root const CFG_HDR cfghdr_m320={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m41={CFG_LEVEL,LG_GFGSETALARM,1,0};

  __root const CFG_HDR cfghdr_m4_5={CFG_LEVEL,LG_GFGMSGSETUP,1,0};

    __root const CFG_HDR cfghdr4_51 = {CFG_CBOX, LG_GFGMSGTIME, 0, 3};
    __root const int msgShowTime = 1;
    __root const CFG_CBOX_ITEM cfgcbox4_0[3] = {LG_GFGMSGUNRESTRICTED, LG_GFGMSG5SEC, LG_GFGMSG10SEC};

  __root const CFG_HDR cfghdr_m4_50={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_m4_11={CFG_LEVEL,LG_GFGSNDSETUP,1,0};

    __root const CFG_HDR cfghdr4_1 = {CFG_CHECKBOX, LG_GFGSNDENABLE, 0, 2};
    __root const int sndEnable = 1;

    __root const CFG_HDR cfghdr4_12 = {CFG_CHECKBOX, LG_CFGSILENT, 0, 2};
    __root const int sndOnSilent = 1;

    __root const CFG_HDR cfghdr4_13 = {CFG_CHECKBOX, LG_GFGSNDNIGHT, 0, 2};
    __root const int sndOnNight = 1;

    __root const CFG_HDR cfghdr4_14={CFG_UINT,LG_GFGVOLUME,0,99};
    __root const unsigned int sndVolume=3;

  __root const CFG_HDR cfghdr_m4_10={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_m4_21={CFG_LEVEL,LG_GFGVBRSETUP,1,0};

    __root const CFG_HDR cfghdr4_2 = {CFG_CHECKBOX, LG_GFGVBRENABLE, 0, 2};
    __root const int Is_Vibra_Enabled = 1;

    __root const CFG_HDR cfghdr4_21={CFG_UINT,LG_GFGVBRTIME,100,10000};
    __root const unsigned int vibraTime=1000;

    __root const CFG_HDR cfghdr4_22={CFG_CBOX,LG_GFGVBRTYPE,0,2};
    __root const int VIBR_TYPE = 0;
    __root const CFG_CBOX_ITEM cfgcbox18_2[2]={LG_GFGVBRTYPE1,LG_GFGVBRTYPE2};

  __root const CFG_HDR cfghdr_m4_20={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_m4_31={CFG_LEVEL,LG_GFGSCRSETUP,1,0};

    __root const CFG_HDR cfghdr4_32 = {CFG_CHECKBOX, LG_GFGSCRENABLE, 0, 2};
    __root const int Is_Screen_Enabled = 1;

    __root const CFG_HDR cfghdr_screen_2={CFG_UINT,LG_GFGSCRTIME,1,1000};
    __root const unsigned int cfg_screen_time=3;

    __root const CFG_HDR cfghdr_screen_blink_speed={CFG_UINT,LG_GFGSCRFREQ,20,1000};
    __root const int cfg_screen_blink_speed = 500;

    __root const CFG_HDR cfghdr_screen_level={CFG_UINT,LG_GFGSCRLEVEL,0,100};
    __root const int cfg_screen_level = 30;

  __root const CFG_HDR cfghdr_m4_30={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m51={CFG_LEVEL,LG_GFGSETKEY,1,0};
  __root const CFG_HDR cfghdr_button={CFG_KEYCODE,LG_GFGMNUENA,0,0};
  __root const int ACTIVE_KEY=0x20;
  __root const int ACTIVE_KEY_STYLE=3;
__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};


