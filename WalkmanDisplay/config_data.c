#include "..\deleaker\mem2.h"
#include "..\include\cfg_items.h"
#include "header\config_data.h"
#include "..\include\Colors.h"

#pragma diag_suppress=Pe177

#include "header\eng.h"
#include "header\rus.h"


__root const CFG_HDR cfghdr319={CFG_CHECKBOX,lng_chkUsePauseIcon,0,0};
__root const int use_pauseicn=true;

__root const CFG_HDR cfghdr320={CFG_KEYCODE,lng_keyRotation,0,3};
__root const int key_rotation=0;
__root const int key_rotation_mode=0;


__root const CFG_HDR cfghdr321_={CFG_CBOX,lng_chkAccelerometer,0,3};
__root const int use_accel=0;
__root const CFG_CBOX_ITEM cfgcbox324_[3]={lng_chkAccelOn,lng_chkAccelRotate,lng_chkAccelOff};

__root const CFG_HDR cfghdr322={CFG_CHECKBOX,lng_chkAlbumPng,0,0};
__root const int use_albumpng=true;

__root const CFG_HDR cfghdr323={CFG_CBOX,lng_chkImagePriority,0,3};
__root const int img_priority=0;
__root const CFG_CBOX_ITEM cfgcbox324[3]={lng_chkFirstInternal,lng_chkOnlyExternal,lng_chkOnlyInternal};

__root const CFG_HDR cfghdr325={CFG_CHECKBOX,lng_chkUseVbr,0,0};
__root const int use_vbr=true;

__root const CFG_HDR cfghdr326={CFG_CBOX,lng_chkUnicalSkinData,0,2};
__root const int skin_cfg=0;
__root const CFG_CBOX_ITEM cfgcbox3277[2]={lng_chkSkinAndCommonData,lng_chkOnlyCommonData};



__root const CFG_HDR confighdr_1={CFG_LEVEL,lng_lvlRestrictions,1,0};

__root const CFG_HDR cfghdr__1={CFG_CHECKBOX,lng_chkRestriction_Cover,0,0};
__root const int restr_CoverAlbum=false;

__root const CFG_HDR confighdr_1_close={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr3261={CFG_CBOX,lng_chkPhone,0,3};
__root const int phone=0;
__root const CFG_CBOX_ITEM cfgcbox32771[3]={lng_chkNotSelected,"W580/W850","W610/W660/W880"};


__root const CFG_HDR cfghdr3262={CFG_CBOX,lng_chkHideSoftkeys,0,4};
__root const int hidesoftkeys=0;
__root const CFG_CBOX_ITEM cfgcbox327712[4]={lng_chkNo,lng_chkInHorizontalMode,lng_chkInVerticalMode,lng_chkEverywhere};

__root const CFG_HDR cfghdr___1={CFG_CBOX,lng_chkInitOrientation,0,2};
__root const int cfg_orient=false;
__root const CFG_CBOX_ITEM cfgcbox327711[2]={lng_chkVertical, lng_chkHorizontal};
