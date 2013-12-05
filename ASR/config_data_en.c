#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_LEVEL,"Radio Recorder",1,0};


__root const CFG_HDR cfghdr1={CFG_CBOX,"Type",0,3};
__root const int FmRadioRecorderType=0;
__root const CFG_CBOX_ITEM cfgcbox0[3]={"As MMS","As Advanced","As TrackID"};

__root const CFG_HDR cfghdr2={CFG_CBOX,"Style",0,2};
__root const int FmStyle=0;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"Window Style","Full Screen Style"};

__root const CFG_HDR cfghdr3={CFG_UTF16_STRING,"Title",0,39};
__root const wchar_t _FmTitle[40]=L"Radio Recorder";

__root const CFG_HDR cfghdr4={CFG_CBOX,"Limit",0,3};
__root const int LimitRadioType=0;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"None","As Time","As Size"};

__root const CFG_HDR cfghdr5={CFG_TIME,"Time Limit",0,0};
__root const TIME FmTIME={0,1,0};

__root const CFG_HDR cfghdr6={CFG_UINT,"Size Limit (Kb)",0,0xFFFFFF};
__root const unsigned int FmSize=30;

__root const CFG_HDR cfghdr7={CFG_CHECKBOX,"Recording to file",0,0};
__root const int RecordFmToFile=1;

__root const CFG_HDR cfghdr8={CFG_UTF16_STRING,"Folder",0,199};
__root const wchar_t FmFolder[200]=L"/usb/other";

__root const CFG_HDR cfghdr9={CFG_UTF16_STRING,"Name",0,199};
__root const wchar_t FmName[200]=L"Radio.amr";

__root const CFG_HDR cfghdr10={CFG_CHECKBOX,"Add Record to menu",0,0};
__root const int Hook=1;

__root const CFG_HDR cfghdr11={CFG_UTF16_STRING,"Menu item text",0,39};
__root const wchar_t _ItemText[40]=L"Record";

__root const CFG_HDR cfghdr12={CFG_KEYCODE,"Key",0,0};
__root const int KEY1=0;
__root const int KEYMODE1=0;


__root const CFG_HDR cfghdr13={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr14={CFG_LEVEL,"Sound Recorder",1,0};


__root const CFG_HDR cfghdr15={CFG_CBOX,"Type",0,3};
__root const int UserSoundRecorderType=0;
__root const CFG_CBOX_ITEM cfgcbox3[3]={"As MMS","As Advanced","As TrackID"};

__root const CFG_HDR cfghdr16={CFG_CBOX,"Style",0,2};
__root const int UserStyle=0;
__root const CFG_CBOX_ITEM cfgcbox4[2]={"Window Style","Full Screen Style"};

__root const CFG_HDR cfghdr17={CFG_UTF16_STRING,"Title",0,39};
__root const wchar_t _UserTitle[40]=L"Sound Recorder";

__root const CFG_HDR cfghdr18={CFG_CBOX,"Limit",0,3};
__root const int LimitSoundType=0;
__root const CFG_CBOX_ITEM cfgcbox5[3]={"None","As Time","As Size"};

__root const CFG_HDR cfghdr19={CFG_TIME,"Time Limit",0,0};
__root const TIME UserTIME={0,1,0};

__root const CFG_HDR cfghdr20={CFG_UINT,"Size Limit (Kb)",0,0xFFFFFF};
__root const unsigned int UserSize=30;

__root const CFG_HDR cfghdr21={CFG_CHECKBOX,"Recording to file",0,0};
__root const int RecordUserToFile=1;

__root const CFG_HDR cfghdr22={CFG_UTF16_STRING,"Folder",0,199};
__root const wchar_t UserFolder[200]=L"/usb/other";

__root const CFG_HDR cfghdr23={CFG_UTF16_STRING,"Name",0,199};
__root const wchar_t UserName[200]=L"Sound.amr";

__root const CFG_HDR cfghdr24={CFG_KEYCODE,"Key",0,0};
__root const int KEY2=0;
__root const int KEYMODE2=0;


__root const CFG_HDR cfghdr25={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr26={CFG_LEVEL,"Call record",1,0};

__root const CFG_HDR cfghdr27={CFG_KEYCODE,"Key",0,0};
__root const int KEY3=0;
__root const int KEYMODE3=0;

__root const CFG_HDR cfghdr28={CFG_LEVEL,"",0,0};
