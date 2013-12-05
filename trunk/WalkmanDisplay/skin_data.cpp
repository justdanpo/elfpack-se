#include "..\deleaker\mem2.h"
#include "..\include\cfg_items.h"
#include "header\skin_data.h"
#include "..\include\Colors.h"

#pragma diag_suppress=Pe177

#include "header\eng.h"
#include "header\rus.h"

__root const CFG_HDR cfghdr0={CFG_LEVEL,lng_lvlVerticalView,1,0};

__root const CFG_HDR cfghdr1={CFG_CHECKBOX,lng_chkStandartWindow,0,0};
__root const int standartwnd=false;

__root const CFG_HDR cfghdr2={CFG_LEVEL,lng_lvlTitle,1,0};

__root const CFG_HDR cfghdr3={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int title_show=true;

__root const CFG_HDR cfghdr4={CFG_FONT,lng_itmFont,0,0};
__root const int title_font=0x2B;

__root const CFG_HDR cfghdr5={CFG_RECT,lng_itmPosition,0,0};
__root const RECT title_rc={25,240,30,60};

__root const CFG_HDR cfghdr6={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int title_color=clWhite;

__root const CFG_HDR cfghdr7={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int title_scolor=clBlack;

__root const CFG_HDR cfghdr7__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int title_hl=0;
__root const CFG_CBOX_ITEM cfgcbox9__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr8={CFG_CBOX,lng_itmCentering,0,3};
__root const int title_ct=0;
__root const CFG_CBOX_ITEM cfgcbox9[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr10={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr11={CFG_LEVEL,lng_lvlAlbum,1,0};

__root const CFG_HDR cfghdr12={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int album_show=true;

__root const CFG_HDR cfghdr13={CFG_FONT,lng_itmFont,0,0};
__root const int album_font=0x2B;

__root const CFG_HDR cfghdr14={CFG_RECT,lng_itmPosition,0,0};
__root const RECT album_rc={0,240,120,140};

__root const CFG_HDR cfghdr15={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int album_color=0xFFFFFFFF;

__root const CFG_HDR cfghdr16={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int album_scolor=0xFF000000;

__root const CFG_HDR cfghdr17__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int album_hl=0;
__root const CFG_CBOX_ITEM cfgcbox18__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};


__root const CFG_HDR cfghdr17={CFG_CBOX,lng_itmCentering,0,3};
__root const int album_ct=2;
__root const CFG_CBOX_ITEM cfgcbox18[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr19={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr20={CFG_LEVEL,lng_lvlArtist,1,0};

__root const CFG_HDR cfghdr21={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int artist_show=true;

__root const CFG_HDR cfghdr22={CFG_FONT,lng_itmFont,0,0};
__root const int artist_font=0x2B;

__root const CFG_HDR cfghdr23={CFG_RECT,lng_itmPosition,0,0};
__root const RECT artist_rc={25,240,55,75};

__root const CFG_HDR cfghdr24={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int artist_color=clWhite;

__root const CFG_HDR cfghdr25={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int artist_scolor=clBlack;

__root const CFG_HDR cfghdr25__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int artist_hl=0;
__root const CFG_CBOX_ITEM cfgcbox27__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr26={CFG_CBOX,lng_itmCentering,0,3};
__root const int artist_ct=0;
__root const CFG_CBOX_ITEM cfgcbox27[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr28={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr29={CFG_LEVEL,lng_lvlYear,1,0};

__root const CFG_HDR cfghdr30={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int year_show=true;

__root const CFG_HDR cfghdr31={CFG_FONT,lng_itmFont,0,0};
__root const int year_font=0x18;

__root const CFG_HDR cfghdr32={CFG_RECT,lng_itmPosition,0,0};
__root const RECT year_rc={0,60,104,124};

__root const CFG_HDR cfghdr33={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int year_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr34={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int year_scolor=0;

__root const CFG_HDR cfghdr34__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int year_hl=0;
__root const CFG_CBOX_ITEM cfgcbox36__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr35={CFG_CBOX,lng_itmCentering,0,3};
__root const int year_ct=0;
__root const CFG_CBOX_ITEM cfgcbox36[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr37={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr38={CFG_LEVEL,lng_lvlGenre,1,0};

__root const CFG_HDR cfghdr39={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int genre_show=false;

__root const CFG_HDR cfghdr40={CFG_FONT,lng_itmFont,0,0};
__root const int genre_font=0x11;

__root const CFG_HDR cfghdr41={CFG_RECT,lng_itmPosition,0,0};
__root const RECT genre_rc={0,100,140,160};

__root const CFG_HDR cfghdr42={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int genre_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr43={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int genre_scolor=0;

__root const CFG_HDR cfghdr43__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int genre_hl=0;
__root const CFG_CBOX_ITEM cfgcbox45__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr44={CFG_CBOX,lng_itmCentering,0,3};
__root const int genre_ct=0;
__root const CFG_CBOX_ITEM cfgcbox45[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr46={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr47={CFG_LEVEL,lng_lvlTrackNumber,1,0};

__root const CFG_HDR cfghdr48={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int trackn_show=true;

__root const CFG_HDR cfghdr49={CFG_FONT,lng_itmFont,0,0};
__root const int trackn_font=0x2B;

__root const CFG_HDR cfghdr50={CFG_RECT,lng_itmPosition,0,0};
__root const RECT trackn_rc={80,138,84,104};

__root const CFG_HDR cfghdr51={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int trackn_color=clWhite;

__root const CFG_HDR cfghdr52={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int trackn_scolor=clBlack;

__root const CFG_HDR cfghdr52__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int trackn_hl=0;
__root const CFG_CBOX_ITEM cfgcbox54__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr53={CFG_CBOX,lng_itmCentering,0,3};
__root const int trackn_ct=1;
__root const CFG_CBOX_ITEM cfgcbox54[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr55={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t trackn_mask[49]=L"%d";

__root const CFG_HDR cfghdr56={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr57={CFG_LEVEL,lng_lvlTracksCount,1,0};

__root const CFG_HDR cfghdr58={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int tracks_show=true;

__root const CFG_HDR cfghdr59={CFG_FONT,lng_itmFont,0,0};
__root const int tracks_font=0x12;

__root const CFG_HDR cfghdr60={CFG_RECT,lng_itmPosition,0,0};
__root const RECT tracks_rc={140,240,86,106};

__root const CFG_HDR cfghdr61={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int tracks_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr62={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int tracks_scolor=0;

__root const CFG_HDR cfghdr62__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int tracks_hl=0;
__root const CFG_CBOX_ITEM cfgcbox64__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr63={CFG_CBOX,lng_itmCentering,0,3};
__root const int tracks_ct=0;
__root const CFG_CBOX_ITEM cfgcbox64[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr65={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t tracks_mask[49]=L"/%d";


__root const CFG_HDR cfghdr66={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr67={CFG_LEVEL,lng_lvlCurrentTime,1,0};

__root const CFG_HDR cfghdr68={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ctime_show=true;

__root const CFG_HDR cfghdr69={CFG_FONT,lng_itmFont,0,0};
__root const int ctime_font=0x18;

__root const CFG_HDR cfghdr70={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ctime_rc={180,240,84,104};

__root const CFG_HDR cfghdr71={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ctime_color=clWhite;

__root const CFG_HDR cfghdr72={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ctime_scolor=clBlack;

__root const CFG_HDR cfghdr72__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ctime_hl=0;
__root const CFG_CBOX_ITEM cfgcbox73__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr73={CFG_CBOX,lng_itmCentering,0,3};
__root const int ctime_ct=1;
__root const CFG_CBOX_ITEM cfgcbox74[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr75={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ctime_mask[49]=L"%02d:%02d";


__root const CFG_HDR cfghdr76={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr77={CFG_LEVEL,lng_lvlFullTime,1,0};

__root const CFG_HDR cfghdr78={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ftime_show=true;

__root const CFG_HDR cfghdr79={CFG_FONT,lng_itmFont,0,0};
__root const int ftime_font=0x18;

__root const CFG_HDR cfghdr80={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ftime_rc={180,240,104,124};

__root const CFG_HDR cfghdr81={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ftime_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr82={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ftime_scolor=0;

__root const CFG_HDR cfghdr82__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ftime_hl=0;
__root const CFG_CBOX_ITEM cfgcbox83__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr83={CFG_CBOX,lng_itmCentering,0,3};
__root const int ftime_ct=1;
__root const CFG_CBOX_ITEM cfgcbox84[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr85={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ftime_mask[49]=L"%02d:%02d";


__root const CFG_HDR cfghdr86={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr87={CFG_LEVEL,lng_lvlTimeLeft,1,0};

__root const CFG_HDR cfghdr88={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ltime_show=false;

__root const CFG_HDR cfghdr89={CFG_FONT,lng_itmFont,0,0};
__root const int ltime_font=0x18;

__root const CFG_HDR cfghdr90={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ltime_rc={180,240,120,140};


__root const CFG_HDR cfghdr91={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ltime_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr92={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ltime_scolor=0;

__root const CFG_HDR cfghdr92__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ltime_hl=0;
__root const CFG_CBOX_ITEM cfgcbox93__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr93={CFG_CBOX,lng_itmCentering,0,3};
__root const int ltime_ct=1;
__root const CFG_CBOX_ITEM cfgcbox94[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr95={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ltime_mask[49]=L"-%02d:%02d";


__root const CFG_HDR cfghdr96={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr97={CFG_LEVEL,lng_lvlBitrate,1,0};

__root const CFG_HDR cfghdr98={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int bitrate_show=true;

__root const CFG_HDR cfghdr99={CFG_FONT,lng_itmFont,0,0};
__root const int bitrate_font=0x18;

__root const CFG_HDR cfghdr100={CFG_RECT,lng_itmPosition,0,0};
__root const RECT bitrate_rc={0,100,122,142};

__root const CFG_HDR cfghdr101={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int bitrate_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr102={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int bitrate_scolor=0;

__root const CFG_HDR cfghdr102__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int bitrate_hl=0;
__root const CFG_CBOX_ITEM cfgcbox103__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr103={CFG_CBOX,lng_itmCentering,0,3};
__root const int bitrate_ct=0;
__root const CFG_CBOX_ITEM cfgcbox104[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr105={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t bitrate_mask[49]=lng_standartBitrateMask;

__root const CFG_HDR cfghdr106={CFG_UTF16_STRING,lng_itmVbrMask,0,48};
__root const wchar_t bitrate_vbr[49]=L"VBR";


__root const CFG_HDR cfghdr107={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr108={CFG_LEVEL,lng_lvlFrequency,1,0};

__root const CFG_HDR cfghdr109={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int freq_show=true;

__root const CFG_HDR cfghdr110={CFG_FONT,lng_itmFont,0,0};
__root const int freq_font=0x11;

__root const CFG_HDR cfghdr111={CFG_RECT,lng_itmPosition,0,0};
__root const RECT freq_rc={180,240,275,295};

__root const CFG_HDR cfghdr112={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int freq_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr113={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int freq_scolor=0;

__root const CFG_HDR cfghdr113__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int freq_hl=0;
__root const CFG_CBOX_ITEM cfgcbox114__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr114={CFG_CBOX,lng_itmCentering,0,3};
__root const int freq_ct=1;
__root const CFG_CBOX_ITEM cfgcbox115[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr116={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t freq_mask[49]=lng_standartFrequencyMask;


__root const CFG_HDR cfghdr117={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr118={CFG_LEVEL,lng_lvlChannels,1,0};

__root const CFG_HDR cfghdr119={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ch_show=true;

__root const CFG_HDR cfghdr120={CFG_FONT,lng_itmFont,0,0};
__root const int ch_font=0x11;

__root const CFG_HDR cfghdr121={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ch_rc={0,100,275,295};

__root const CFG_HDR cfghdr122={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ch_color=0xC0FFFFFF;

__root const CFG_HDR cfghdr123={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ch_scolor=0;

__root const CFG_HDR cfghdr123__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ch_hl=0;
__root const CFG_CBOX_ITEM cfgcbox124__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr124={CFG_CBOX,lng_itmCentering,0,3};
__root const int ch_ct=0;
__root const CFG_CBOX_ITEM cfgcbox125[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr126={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr127={CFG_LEVEL,lng_lvlProgressBar,1,0};

__root const CFG_HDR cfghdr128={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ProgressBar_Show=1;

__root const CFG_HDR cfghdr129={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT pbrect={0,240,79,81};

__root const CFG_HDR cfghdr130={CFG_COLOR_INT,lng_lvlCurrentTime,0,0};
__root const unsigned int ProgressBar_cColor=clWhite;

__root const CFG_HDR cfghdr131={CFG_COLOR_INT,lng_lvlTimeLeft,0,0};
__root const unsigned int ProgressBar_oColor=0;

__root const CFG_HDR cfghdr132={CFG_COLOR_INT, lng_itmHighlightColor,0,0};
__root const unsigned int ProgressBar_bColor=clBlack;

__root const CFG_HDR cfghdr133={CFG_CHECKBOX,lng_chkHighlight,0,0};
__root const int ProgressBar_highlight=0;

__root const CFG_HDR cfghdr134={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr135={CFG_LEVEL,lng_lvlCover,1,0};

__root const CFG_HDR cfghdr136={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int cover_show=true;

__root const CFG_HDR cfghdr137={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT cover_rect={45,195,120,270};

__root const CFG_HDR cfghdr138={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr_tabs1={CFG_LEVEL,lng_lvlIcons,1,0};



__root const CFG_HDR cfghdr139={CFG_LEVEL,lng_lvlTitleIcon,1,0};

__root const CFG_HDR cfghdr140={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ticn_show=true;

__root const CFG_HDR cfghdr141={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT ticn_rect={0,0,28,28};


__root const CFG_HDR cfghdr142={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr143={CFG_LEVEL,lng_lvlAlbumIcon,1,0};

__root const CFG_HDR cfghdr144={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int alicn_show=false;

__root const CFG_HDR cfghdr145={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT alicn_rect={0,150,26,176};

__root const CFG_HDR cfghdr146={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr147={CFG_LEVEL,lng_lvlArtistIcon,1,0};

__root const CFG_HDR cfghdr148={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int aricn_show=true;

__root const CFG_HDR cfghdr149={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT aricn_rect={0,0,52,52};

__root const CFG_HDR cfghdr150={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr151={CFG_LEVEL,lng_lvlPlayStopIcon,1,0};

__root const CFG_HDR cfghdr152={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int pls_show=true;

__root const CFG_HDR cfghdr153={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT pls_rect={175,175,82,82};

__root const CFG_HDR cfghdr154={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr155={CFG_LEVEL,lng_lvlEqualizerIcon,1,0};

__root const CFG_HDR cfghdr156={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int eq_preset_show=true;

__root const CFG_HDR cfghdr157={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT eq_preset_rect={5,5,84,84};

__root const CFG_HDR cfghdr158={CFG_LEVEL,"",0,0}; 






__root const CFG_HDR cfghdr155_11211a={CFG_LEVEL,lng_lvlShuffleMode,1,0};

__root const CFG_HDR cfghdr156111211a={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int shuffle_show=true;

__root const CFG_HDR cfghdr157111211a={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT shuffle_rect={40,40,84,84};
        
__root const CFG_HDR cfghdr158111211a={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_11211a1={CFG_LEVEL,lng_lvlRepeatMode,1,0};

__root const CFG_HDR cfghdr156111211a1={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int repeat_show=true;

__root const CFG_HDR cfghdr157111211a1={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT repeat_rect={65,65,84,84};
        
__root const CFG_HDR cfghdr158111211a1={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr155_11211a1h1={CFG_LEVEL,lng_lvlStereoWidening,1,0};

__root const CFG_HDR cfghdr156111211a1h1={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int stereo_show=true;

__root const CFG_HDR cfghdr157111211a1h1={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT stereo_rect={85,85,84,84};
        
__root const CFG_HDR cfghdr158111211a1h1={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr_tabs1_close={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr_tabs2={CFG_LEVEL,lng_lvlNavigation,1,0};


__root const CFG_HDR cfghdr155_11211={CFG_LEVEL,lng_lvlBackground,1,0};

__root const CFG_HDR cfghdr156111211={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int navigimg_show=true;

__root const CFG_HDR cfghdr157111211={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT navigimg_rect={85,85,220,220};
        
__root const CFG_HDR cfghdr158111211={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_={CFG_LEVEL,lng_lvlLeftIcon,1,0};

__root const CFG_HDR cfghdr1561={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int leftimg_show=true;

__root const CFG_HDR cfghdr1571={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT leftimg_rect={70,70,245,245};
        
__root const CFG_HDR cfghdr1581={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr155_1={CFG_LEVEL,lng_lvlRightIcon,1,0};

__root const CFG_HDR cfghdr15611={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int rightimg_show=true;

__root const CFG_HDR cfghdr15711={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT rightimg_rect={140,140,245,245};
        
__root const CFG_HDR cfghdr15811={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr155_11={CFG_LEVEL,lng_lvlCenterIcon,1,0};

__root const CFG_HDR cfghdr156111={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int centerimg_show=true;

__root const CFG_HDR cfghdr157111={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT centerimg_rect={110,110,245,245};
        
__root const CFG_HDR cfghdr158111={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr155_111={CFG_LEVEL,lng_lvlUpIcon,1,0};

__root const CFG_HDR cfghdr1561111={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int upimg_show=true;

__root const CFG_HDR cfghdr1571111={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT upimg_rect={110,110,210,210};
        
__root const CFG_HDR cfghdr1581111={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr155_112={CFG_LEVEL,lng_lvlDownIcon,1,0};

__root const CFG_HDR cfghdr1561112={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int downimg_show=true;

__root const CFG_HDR cfghdr1571112={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT downimg_rect={110,110,278,278};
        
__root const CFG_HDR cfghdr1581112={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr_tabs2_close={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr159={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr160={CFG_LEVEL,lng_lvlHorizontalView,1,0};


__root const CFG_HDR cfghdr161={CFG_LEVEL,lng_lvlTitle,1,0};

__root const CFG_HDR cfghdr162={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int title_show_h=true;

__root const CFG_HDR cfghdr163={CFG_FONT,lng_itmFont,0,0};
__root const int title_font_h=0x2B;

__root const CFG_HDR cfghdr164={CFG_RECT,lng_itmPosition,0,0};
__root const RECT title_rc_h={25,320,10,30};

__root const CFG_HDR cfghdr165={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int title_color_h=clWhite;

__root const CFG_HDR cfghdr166={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int title_scolor_h=clBlack;

__root const CFG_HDR cfghdr166__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int title_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox167__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr167={CFG_CBOX,lng_itmCentering,0,3};
__root const int title_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox168[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr169={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr170={CFG_LEVEL,lng_lvlAlbum,1,0};

__root const CFG_HDR cfghdr171={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int album_show_h=true;

__root const CFG_HDR cfghdr172={CFG_FONT,lng_itmFont,0,0};
__root const int album_font_h=0x2B;

__root const CFG_HDR cfghdr173={CFG_RECT,lng_itmPosition,0,0};
__root const RECT album_rc_h={0,320,85,105};

__root const CFG_HDR cfghdr174={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int album_color_h=0xF0FFFFFF;

__root const CFG_HDR cfghdr175={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int album_scolor_h=0xFF000000;

__root const CFG_HDR cfghdr175__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int album_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox176__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr176={CFG_CBOX,lng_itmCentering,0,3};
__root const int album_ct_h=2;
__root const CFG_CBOX_ITEM cfgcbox177[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr178={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr179={CFG_LEVEL,lng_lvlArtist,1,0};

__root const CFG_HDR cfghdr180={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int artist_show_h=true;

__root const CFG_HDR cfghdr181={CFG_FONT,lng_itmFont,0,0};
__root const int artist_font_h=0x2B;

__root const CFG_HDR cfghdr182={CFG_RECT,lng_itmPosition,0,0};
__root const RECT artist_rc_h={25,320,33,53};

__root const CFG_HDR cfghdr183={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int artist_color_h=clWhite;

__root const CFG_HDR cfghdr184={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int artist_scolor_h=clBlack;

__root const CFG_HDR cfghdr184__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int artist_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox185__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr185={CFG_CBOX,lng_itmCentering,0,3};
__root const int artist_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox186[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr187={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr188={CFG_LEVEL,lng_lvlYear,1,0};

__root const CFG_HDR cfghdr189={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int year_show_h=true;

__root const CFG_HDR cfghdr190={CFG_FONT,lng_itmFont,0,0};
__root const int year_font_h=0x18;

__root const CFG_HDR cfghdr191={CFG_RECT,lng_itmPosition,0,0};
__root const RECT year_rc_h={0,60,80,100};

__root const CFG_HDR cfghdr192={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int year_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr193={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int year_scolor_h=0;

__root const CFG_HDR cfghdr193__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int year_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox194__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr194={CFG_CBOX,lng_itmCentering,0,3};
__root const int year_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox195[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr196={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr197={CFG_LEVEL,lng_lvlGenre,1,0};

__root const CFG_HDR cfghdr198={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int genre_show_h=true;

__root const CFG_HDR cfghdr199={CFG_FONT,lng_itmFont,0,0};
__root const int genre_font_h=0x11;

__root const CFG_HDR cfghdr200={CFG_RECT,lng_itmPosition,0,0};
__root const RECT genre_rc_h={0,100,120,140};

__root const CFG_HDR cfghdr201={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int genre_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr202={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int genre_scolor_h=0;

__root const CFG_HDR cfghdr202__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int genre_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox203__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr203={CFG_CBOX,lng_itmCentering,0,3};
__root const int genre_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox204[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr205={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr206={CFG_LEVEL,lng_lvlTrackNumber,1,0};

__root const CFG_HDR cfghdr207={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int trackn_show_h=true;

__root const CFG_HDR cfghdr208={CFG_FONT,lng_itmFont,0,0};
__root const int trackn_font_h=0x2B;

__root const CFG_HDR cfghdr209={CFG_RECT,lng_itmPosition,0,0};
__root const RECT trackn_rc_h={150,218,60,80};

__root const CFG_HDR cfghdr210={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int trackn_color_h=clWhite;

__root const CFG_HDR cfghdr211={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int trackn_scolor_h=clBlack;

__root const CFG_HDR cfghdr211__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int trackn_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox212__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr212={CFG_CBOX,lng_itmCentering,0,3};
__root const int trackn_ct_h=1;
__root const CFG_CBOX_ITEM cfgcbox213[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr214={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t trackn_mask_h[49]=L"%d";


__root const CFG_HDR cfghdr215={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr216={CFG_LEVEL,lng_lvlTracksCount,1,0};

__root const CFG_HDR cfghdr217={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int tracks_show_h=true;

__root const CFG_HDR cfghdr218={CFG_FONT,lng_itmFont,0,0};
__root const int tracks_font_h=0x18;

__root const CFG_HDR cfghdr219={CFG_RECT,lng_itmPosition,0,0};
__root const RECT tracks_rc_h={220,320,62,82};

__root const CFG_HDR cfghdr220={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int tracks_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr221={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int tracks_scolor_h=0;

__root const CFG_HDR cfghdr221__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int tracks_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox222__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr222={CFG_CBOX,lng_itmCentering,0,3};
__root const int tracks_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox223[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr224={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t tracks_mask_h[49]=L"/%d";

__root const CFG_HDR cfghdr225={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr226={CFG_LEVEL,lng_lvlCurrentTime,1,0};

__root const CFG_HDR cfghdr227={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ctime_show_h=true;

__root const CFG_HDR cfghdr228={CFG_FONT,lng_itmFont,0,0};
__root const int ctime_font_h=0x18;

__root const CFG_HDR cfghdr229={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ctime_rc_h={260,320,60,80};

__root const CFG_HDR cfghdr230={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ctime_color_h=clWhite;

__root const CFG_HDR cfghdr231={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ctime_scolor_h=clBlack;

__root const CFG_HDR cfghdr231__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ctime_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox232__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr232={CFG_CBOX,lng_itmCentering,0,3};
__root const int ctime_ct_h=1;
__root const CFG_CBOX_ITEM cfgcbox233[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr234={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ctime_mask_h[49]=L"%02d:%02d";

__root const CFG_HDR cfghdr235={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr236={CFG_LEVEL,lng_lvlFullTime,1,0};

__root const CFG_HDR cfghdr237={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ftime_show_h=true;

__root const CFG_HDR cfghdr238={CFG_FONT,lng_itmFont,0,0};
__root const int ftime_font_h=0x18;

__root const CFG_HDR cfghdr239={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ftime_rc_h={260,320,80,100};

__root const CFG_HDR cfghdr240={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ftime_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr241={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ftime_scolor_h=0;

__root const CFG_HDR cfghdr241__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ftime_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox242__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr242={CFG_CBOX,lng_itmCentering,0,3};
__root const int ftime_ct_h=1;
__root const CFG_CBOX_ITEM cfgcbox243[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr244={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ftime_mask_h[49]=L"%02d:%02d";

__root const CFG_HDR cfghdr245={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr246={CFG_LEVEL,lng_lvlTimeLeft,1,0};

__root const CFG_HDR cfghdr247={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ltime_show_h=false;

__root const CFG_HDR cfghdr248={CFG_FONT,lng_itmFont,0,0};
__root const int ltime_font_h=0x18;

__root const CFG_HDR cfghdr249={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ltime_rc_h={260,320,100,120};

__root const CFG_HDR cfghdr250={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ltime_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr251={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ltime_scolor_h=0;

__root const CFG_HDR cfghdr251__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ltime_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox252__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr252={CFG_CBOX,lng_itmCentering,0,3};
__root const int ltime_ct_h=1;
__root const CFG_CBOX_ITEM cfgcbox253[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr254={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t ltime_mask_h[49]=L"-%02d:%02d";

__root const CFG_HDR cfghdr255={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr256={CFG_LEVEL,lng_lvlBitrate,1,0};

__root const CFG_HDR cfghdr257={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int bitrate_show_h=true;

__root const CFG_HDR cfghdr258={CFG_FONT,lng_itmFont,0,0};
__root const int bitrate_font_h=0x18;

__root const CFG_HDR cfghdr259={CFG_RECT,lng_itmPosition,0,0};
__root const RECT bitrate_rc_h={0,100,100,120};

__root const CFG_HDR cfghdr260={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int bitrate_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr261={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int bitrate_scolor_h=0;

__root const CFG_HDR cfghdr261__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int bitrate_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox262__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr262={CFG_CBOX,lng_itmCentering,0,3};
__root const int bitrate_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox263[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr264={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t bitrate_mask_h[49]=lng_standartBitrateMask;

__root const CFG_HDR cfghdr265={CFG_UTF16_STRING,lng_itmVbrMask,0,48};
__root const wchar_t bitrate_vbr_h[49]=L"VBR";

__root const CFG_HDR cfghdr266={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr267={CFG_LEVEL,lng_lvlFrequency,1,0};

__root const CFG_HDR cfghdr268={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int freq_show_h=true;

__root const CFG_HDR cfghdr269={CFG_FONT,lng_itmFont,0,0};
__root const int freq_font_h=0x11;

__root const CFG_HDR cfghdr270={CFG_RECT,lng_itmPosition,0,0};
__root const RECT freq_rc_h={200,320,190,210};


__root const CFG_HDR cfghdr271={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int freq_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr272={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int freq_scolor_h=0;

__root const CFG_HDR cfghdr272__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int freq_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox273__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr273={CFG_CBOX,lng_itmCentering,0,3};
__root const int freq_ct_h=1;
__root const CFG_CBOX_ITEM cfgcbox274[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};

__root const CFG_HDR cfghdr275={CFG_UTF16_STRING,lng_itmMask,0,48};
__root const wchar_t freq_mask_h[49]=L"%0d Hz";

__root const CFG_HDR cfghdr276={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr277={CFG_LEVEL,lng_lvlChannels,1,0};

__root const CFG_HDR cfghdr278={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ch_show_h=true;

__root const CFG_HDR cfghdr279={CFG_FONT,lng_itmFont,0,0};
__root const int ch_font_h=0x11;

__root const CFG_HDR cfghdr280={CFG_RECT,lng_itmPosition,0,0};
__root const RECT ch_rc_h={0,100,190,210};


__root const CFG_HDR cfghdr281={CFG_COLOR_INT,lng_itmColor,0,0};
__root const unsigned int ch_color_h=0xC0FFFFFF;

__root const CFG_HDR cfghdr282={CFG_COLOR_INT,lng_itmSecondColor,0,0};
__root const unsigned int ch_scolor_h=0;

__root const CFG_HDR cfghdr282__={CFG_CBOX,lng_chkHighlightType,0,3};
__root const int ch_hl_h=0;
__root const CFG_CBOX_ITEM cfgcbox283__[3]={lng_chkDefault, lng_chkDefault2, lng_chkShadow};

__root const CFG_HDR cfghdr283={CFG_CBOX,lng_itmCentering,0,3};
__root const int ch_ct_h=0;
__root const CFG_CBOX_ITEM cfgcbox284[3]={lng_chkLeft,lng_chkRight,lng_chkCenter};


__root const CFG_HDR cfghdr285={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr286={CFG_LEVEL,lng_lvlProgressBar,1,0};

__root const CFG_HDR cfghdr287={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ProgressBar_Show_h=1;

__root const CFG_HDR cfghdr288={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT pbrect_h={0,320,55,57};

__root const CFG_HDR cfghdr289={CFG_COLOR_INT,lng_lvlCurrentTime,0,0};
__root const unsigned int ProgressBar_cColor_h=clWhite;

__root const CFG_HDR cfghdr290={CFG_COLOR_INT,lng_lvlTimeLeft,0,0};
__root const unsigned int ProgressBar_oColor_h=0;


__root const CFG_HDR cfghdr291={CFG_COLOR_INT, lng_itmHighlightColor,0,0};
__root const unsigned int ProgressBar_bColor_h=0;

__root const CFG_HDR cfghdr292={CFG_CHECKBOX,lng_chkHighlight,0,0};
__root const int ProgressBar_highlight_h=0;

__root const CFG_HDR cfghdr293={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr294={CFG_LEVEL,lng_lvlCover,1,0};

__root const CFG_HDR cfghdr295={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int cover_show_h=true;

__root const CFG_HDR cfghdr296={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT cover_rect_h={80,230,85,235};

__root const CFG_HDR cfghdr297={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr_tabs3={CFG_LEVEL,lng_lvlIcons,1,0};


__root const CFG_HDR cfghdr298={CFG_LEVEL,lng_lvlTitleIcon,1,0};

__root const CFG_HDR cfghdr299={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int ticn_show_h=true;

__root const CFG_HDR cfghdr300={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT ticn_rect_h={0,0,8,8};

__root const CFG_HDR cfghdr301={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr302={CFG_LEVEL,lng_lvlAlbumIcon,1,0};

__root const CFG_HDR cfghdr303={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int alicn_show_h=false;

__root const CFG_HDR cfghdr304={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT alicn_rect_h={80,80,215,215};

__root const CFG_HDR cfghdr305={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr306={CFG_LEVEL,lng_lvlArtistIcon,1,0};

__root const CFG_HDR cfghdr307={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int aricn_show_h=true;

__root const CFG_HDR cfghdr308={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT aricn_rect_h={0,0,33,33};

__root const CFG_HDR cfghdr309={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr310={CFG_LEVEL,lng_lvlPlayStopIcon,1,0};

__root const CFG_HDR cfghdr311={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int pls_show_h=true;

__root const CFG_HDR cfghdr312={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT pls_rect_h={255,255,58,58};
        

__root const CFG_HDR cfghdr313={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr314={CFG_LEVEL,lng_lvlEqualizerIcon,1,0};

__root const CFG_HDR cfghdr315={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int eq_preset_show_h=true;

__root const CFG_HDR cfghdr316={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT eq_preset_rect_h={5,5,60,60};
        
__root const CFG_HDR cfghdr317={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_11211ah={CFG_LEVEL,lng_lvlShuffleMode,1,0};

__root const CFG_HDR cfghdr156111211ah={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int shuffle_show_h=true;

__root const CFG_HDR cfghdr157111211ah={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT shuffle_rect_h={40,40,60,60};
        
__root const CFG_HDR cfghdr158111211ah={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_11211a1h={CFG_LEVEL,lng_lvlRepeatMode,1,0};

__root const CFG_HDR cfghdr156111211a1h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int repeat_show_h=true;

__root const CFG_HDR cfghdr157111211a1h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT repeat_rect_h={65,65,60,60};
        
__root const CFG_HDR cfghdr158111211a1h={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr155_11211a1h1h={CFG_LEVEL,lng_lvlStereoWidening,1,0};

__root const CFG_HDR cfghdr156111211a1h1h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int stereo_show_h=true;

__root const CFG_HDR cfghdr157111211a1h1h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT stereo_rect_h={85,85,60,60};
        
__root const CFG_HDR cfghdr158111211a1h1h={CFG_LEVEL,"",0,0}; 



__root const CFG_HDR cfghdr_tabs3_close={CFG_LEVEL,"",0,0}; 









__root const CFG_HDR cfghdr_tabs4={CFG_LEVEL,lng_lvlNavigation,1,0};



__root const CFG_HDR cfghdr155_112111={CFG_LEVEL,lng_lvlBackground,1,0};

__root const CFG_HDR cfghdr1561112111={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int navigimg_show_h=true;

__root const CFG_HDR cfghdr1571112111={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT navigimg_rect_h={233,233,85,85};
        
__root const CFG_HDR cfghdr1581112111={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155__h={CFG_LEVEL,lng_lvlLeftIcon,1,0};

__root const CFG_HDR cfghdr1561_h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int leftimg_show_h=true;

__root const CFG_HDR cfghdr1571_h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT leftimg_rect_h={220,220,110,110};
        
__root const CFG_HDR cfghdr1581_h={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_1_h={CFG_LEVEL,lng_lvlRightIcon,1,0};

__root const CFG_HDR cfghdr15611_h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int rightimg_show_h=true;

__root const CFG_HDR cfghdr15711_h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT rightimg_rect_h={288,288,110,110};
        
__root const CFG_HDR cfghdr15811_h={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr155_11_h={CFG_LEVEL,lng_lvlCenterIcon,1,0};

__root const CFG_HDR cfghdr156111_h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int centerimg_show_h=true;

__root const CFG_HDR cfghdr157111_h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT centerimg_rect_h={259,259,110,110};
        
__root const CFG_HDR cfghdr158111_h={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr155_111_h={CFG_LEVEL,lng_lvlUpIcon,1,0};

__root const CFG_HDR cfghdr1561111_h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int upimg_show_h=true;

__root const CFG_HDR cfghdr1571111_h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT upimg_rect_h={258,258,74,74};
        
__root const CFG_HDR cfghdr1581111_h={CFG_LEVEL,"",0,0}; 





__root const CFG_HDR cfghdr155_112_h={CFG_LEVEL,lng_lvlDownIcon,1,0};

__root const CFG_HDR cfghdr1561112_h={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int downimg_show_h=true;

__root const CFG_HDR cfghdr1571112_h={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT downimg_rect_h={259,259,143,143};
        
__root const CFG_HDR cfghdr1581112_h={CFG_LEVEL,"",0,0}; 




__root const CFG_HDR cfghdr_tabs4_close={CFG_LEVEL,"",0,0}; 


__root const CFG_HDR cfghdr318h={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr127__={CFG_LEVEL,lng_lvlVolumeBar,1,0};

__root const CFG_HDR cfghdr128__={CFG_CHECKBOX,lng_itmShow,0,0};
__root const int VolumeBar_Show=1;

__root const CFG_HDR cfghdr129__={CFG_RECT,lng_itmPositionAndSize,0,0};
__root const RECT volrect={0,320,0,5};

__root const CFG_HDR cfghdr130__={CFG_COLOR_INT,lng_itmCurVolume,0,0};
__root const unsigned int VolumeBar_cColor=0xFF007700;

__root const CFG_HDR cfghdr131__={CFG_COLOR_INT,lng_itmLeftVolume,0,0};
__root const unsigned int VolumeBar_oColor=clBlack;

__root const CFG_HDR cfghdr132__={CFG_COLOR_INT, lng_itmHighlightColor,0,0};
__root const unsigned int VolumeBar_bColor=clBlack;

__root const CFG_HDR cfghdr133__={CFG_CHECKBOX,lng_chkHighlight,0,0};
__root const int VolumeBar_highlight=1;

__root const CFG_HDR cfghdr134__={CFG_LEVEL,"",0,0}; 
