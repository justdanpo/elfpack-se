#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "..\\include\Colors.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_LEVEL,lngLABELS,1,0};
__root const CFG_HDR cfghdr1={CFG_LEVEL,lngLABEL1,2,0};
  
__root const CFG_HDR cfghdr2={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL1_SHOW=1;
        
__root const CFG_HDR cfghdr3={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL1_X_POS=0;
        __root const int LABEL1_Y_POS=0;

__root const CFG_HDR cfghdr4={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL1_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr5={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL1_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr6={CFG_FONT,lngFont,0,0};
        __root const int LABEL1_FONT=0x11;
        
__root const CFG_HDR cfghdr7={CFG_CBOX,lngCT,0,3};
        __root const int LABEL1_CT=0;
__root const CFG_CBOX_ITEM cfgcbox8[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr9={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL1_NAME[256]="$t";
        
__root const CFG_HDR cfghdr10={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr11={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL1_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr12={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL1_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox13[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr14={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL1_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr15={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL1_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr16={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL1_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr17={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL1_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr18={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL1_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr19={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr20={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr21={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL1_LINE_ON=0;
        
__root const CFG_HDR cfghdr22={CFG_INT,lngDist,0,1000};
        __root const int LABEL1_LINE_DIST=0;
        
__root const CFG_HDR cfghdr23={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL1_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr24={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL1_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr25={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr26={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL1_NEED_REDRAW=0;
        
__root const CFG_HDR cfghdr27={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr28={CFG_LEVEL,lngLABEL2,2,0};
  
__root const CFG_HDR cfghdr29={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL2_SHOW=1;
        
__root const CFG_HDR cfghdr30={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL2_X_POS=0;
        __root const int LABEL2_Y_POS=0;

__root const CFG_HDR cfghdr31={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL2_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr32={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL2_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr33={CFG_FONT,lngFont,0,0};
        __root const int LABEL2_FONT=0x11;
        
__root const CFG_HDR cfghdr34={CFG_CBOX,lngCT,0,3};
        __root const int LABEL2_CT=0;
__root const CFG_CBOX_ITEM cfgcbox35[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr36={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL2_NAME[256]="$a";
        
__root const CFG_HDR cfghdr37={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr38={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL2_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr39={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL2_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox40[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr41={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL2_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr42={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL2_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr43={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL2_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr44={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL2_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr45={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL2_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr46={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr47={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL2_NEED_REDRAW=0;
        
__root const CFG_HDR cfghdr48={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr49={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL2_LINE_ON=0;
        
__root const CFG_HDR cfghdr50={CFG_INT,lngDist,0,1000};
        __root const int LABEL2_LINE_DIST=0;
        
__root const CFG_HDR cfghdr51={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL2_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr52={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL2_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr53={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr54={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr55={CFG_LEVEL,lngLABEL3,3,0};
  
__root const CFG_HDR cfghdr56={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL3_SHOW=1;
        
__root const CFG_HDR cfghdr57={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL3_X_POS=0;
        __root const int LABEL3_Y_POS=0;

__root const CFG_HDR cfghdr58={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL3_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr59={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL3_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr60={CFG_FONT,lngFont,0,0};
        __root const int LABEL3_FONT=0x11;
        
__root const CFG_HDR cfghdr61={CFG_CBOX,lngCT,0,3};
        __root const int LABEL3_CT=0;
__root const CFG_CBOX_ITEM cfgcbox62[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr63={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL3_NAME[256]="$c";
        
__root const CFG_HDR cfghdr64={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr65={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL3_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr66={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL3_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox67[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr68={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL3_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr69={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL3_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr70={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL3_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr71={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL3_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr72={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL3_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr73={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr74={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr75={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL3_LINE_ON=0;
        
__root const CFG_HDR cfghdr76={CFG_INT,lngDist,0,1000};
        __root const int LABEL3_LINE_DIST=0;
        
__root const CFG_HDR cfghdr77={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL3_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr78={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL3_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr79={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr80={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL3_NEED_REDRAW=0;
        
__root const CFG_HDR cfghdr81={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr82={CFG_LEVEL,lngLABEL4,3,0};
  
__root const CFG_HDR cfghdr83={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL4_SHOW=1;
        
__root const CFG_HDR cfghdr84={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL4_X_POS=170;
        __root const int LABEL4_Y_POS=50;

__root const CFG_HDR cfghdr85={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL4_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr86={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL4_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr87={CFG_FONT,lngFont,0,0};
        __root const int LABEL4_FONT=0x2C;
        
__root const CFG_HDR cfghdr88={CFG_CBOX,lngCT,0,3};
        __root const int LABEL4_CT=1;
__root const CFG_CBOX_ITEM cfgcbox89[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr90={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL4_NAME[256]="$i/";
        
__root const CFG_HDR cfghdr91={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr92={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL4_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr93={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL4_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox94[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr95={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL4_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr96={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL4_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr97={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL4_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr98={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL4_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr99={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL4_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr100={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr101={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr102={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL4_LINE_ON=0;
        
__root const CFG_HDR cfghdr103={CFG_INT,lngDist,0,1000};
        __root const int LABEL4_LINE_DIST=0;
        
__root const CFG_HDR cfghdr104={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL4_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr105={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL4_LINE_MINUS=0;
__root const CFG_HDR cfghdr106={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr107={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL4_NEED_REDRAW=1;
__root const CFG_HDR cfghdr108={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr109={CFG_LEVEL,lngLABEL5,3,0};
  
__root const CFG_HDR cfghdr110={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL5_SHOW=1;
        
__root const CFG_HDR cfghdr111={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL5_X_POS=170;
        __root const int LABEL5_Y_POS=53;

__root const CFG_HDR cfghdr112={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL5_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr113={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL5_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr114={CFG_FONT,lngFont,0,0};
        __root const int LABEL5_FONT=0x19;
        
__root const CFG_HDR cfghdr115={CFG_CBOX,lngCT,0,3};
        __root const int LABEL5_CT=0;
__root const CFG_CBOX_ITEM cfgcbox116[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr117={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL5_NAME[256]="$p";
        
__root const CFG_HDR cfghdr118={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr119={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL5_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr120={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL5_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox121[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr122={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL5_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr123={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL5_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr124={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL5_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr125={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL5_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr126={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL5_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr127={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr128={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr129={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL5_LINE_ON=0;
        
__root const CFG_HDR cfghdr130={CFG_INT,lngDist,0,1000};
        __root const int LABEL5_LINE_DIST=0;
        
__root const CFG_HDR cfghdr131={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL5_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr132={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL5_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr133={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr134={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL5_NEED_REDRAW=0;
__root const CFG_HDR cfghdr135={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr136={CFG_LEVEL,lngLABEL6,3,0};
  
__root const CFG_HDR cfghdr137={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL6_SHOW=1;
        
__root const CFG_HDR cfghdr138={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL6_X_POS=200;
        __root const int LABEL6_Y_POS=110;

__root const CFG_HDR cfghdr139={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL6_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr140={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL6_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr141={CFG_FONT,lngFont,0,0};
        __root const int LABEL6_FONT=0x11;
        
__root const CFG_HDR cfghdr142={CFG_CBOX,lngCT,0,3};
        __root const int LABEL6_CT=1;
__root const CFG_CBOX_ITEM cfgcbox143[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr144={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL6_NAME[256]="$b kb/s";
        
__root const CFG_HDR cfghdr145={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr146={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL6_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr147={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL6_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox148[4]={lngScrollToLeft,lngScrollToRight lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr149={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL6_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr150={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL6_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr151={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL6_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr152={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL6_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr153={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL6_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr154={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr155={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr156={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL6_LINE_ON=0;
        
__root const CFG_HDR cfghdr157={CFG_INT,lngDist,0,1000};
        __root const int LABEL6_LINE_DIST=0;
        
__root const CFG_HDR cfghdr158={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL6_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr159={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL6_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr160={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr161={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL6_NEED_REDRAW=0;
__root const CFG_HDR cfghdr162={CFG_LEVEL,"",0,0}; 
   
   
__root const CFG_HDR cfghdr163={CFG_LEVEL,lngLABEL7,3,0};
  
__root const CFG_HDR cfghdr164={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL7_SHOW=1;
        
__root const CFG_HDR cfghdr165={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL7_X_POS=0;
        __root const int LABEL7_Y_POS=110;

__root const CFG_HDR cfghdr166={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL7_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr167={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL7_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr168={CFG_FONT,lngFont,0,0};
        __root const int LABEL7_FONT=0x12;
        
__root const CFG_HDR cfghdr169={CFG_CBOX,lngCT,0,3};
        __root const int LABEL7_CT=0;
__root const CFG_CBOX_ITEM cfgcbox170[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr171={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL7_NAME[256]="$s";
        
__root const CFG_HDR cfghdr172={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr173={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL7_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr174={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL7_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox175[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr176={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL7_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr177={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL7_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr178={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL7_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr179={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL7_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr180={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL7_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr181={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr182={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr183={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL7_LINE_ON=0;
        
__root const CFG_HDR cfghdr184={CFG_INT,lngDist,0,1000};
        __root const int LABEL7_LINE_DIST=0;
        
__root const CFG_HDR cfghdr185={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL7_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr186={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL7_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr187={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr188={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL7_NEED_REDRAW=1;
__root const CFG_HDR cfghdr189={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr190={CFG_LEVEL,lngLABEL8,3,0};
   
__root const CFG_HDR cfghdr191={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL8_SHOW=1;
        
__root const CFG_HDR cfghdr192={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL8_X_POS=240;
        __root const int LABEL8_Y_POS=110;

__root const CFG_HDR cfghdr193={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL8_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr194={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL8_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr195={CFG_FONT,lngFont,0,0};
        __root const int LABEL8_FONT=0x12;
        
__root const CFG_HDR cfghdr196={CFG_CBOX,lngCT,0,3};
        __root const int LABEL8_CT=1;
__root const CFG_CBOX_ITEM cfgcbox197[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr198={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL8_NAME[256]="$l";
        
__root const CFG_HDR cfghdr199={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr200={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL8_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr201={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL8_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox202[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr203={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL8_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr204={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL8_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr205={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL8_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr206={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL8_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr207={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL8_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr208={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr209={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr210={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL8_LINE_ON=0;
        
__root const CFG_HDR cfghdr211={CFG_INT,lngDist,0,1000};
        __root const int LABEL8_LINE_DIST=0;
        
__root const CFG_HDR cfghdr212={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL8_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr213={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL8_LINE_MINUS=0;
        
        
__root const CFG_HDR cfghdr214={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr215={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL8_NEED_REDRAW=0;
        
__root const CFG_HDR cfghdr216={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr217={CFG_LEVEL,lngLABEL9,3,0};
   
__root const CFG_HDR cfghdr218={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL9_SHOW=0;
        
__root const CFG_HDR cfghdr219={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL9_X_POS=0;
        __root const int LABEL9_Y_POS=0;

__root const CFG_HDR cfghdr220={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL9_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr221={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL9_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr222={CFG_FONT,lngFont,0,0};
        __root const int LABEL9_FONT=0x11;
        
__root const CFG_HDR cfghdr223={CFG_CBOX,lngCT,0,3};
        __root const int LABEL9_CT=0;
__root const CFG_CBOX_ITEM cfgcbox224[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr225={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL9_NAME[256]="$l";
        
__root const CFG_HDR cfghdr226={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr227={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL9_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr228={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL9_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox229[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr230={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL9_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr231={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL9_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr232={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL9_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr233={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL9_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr234={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL9_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr235={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr236={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr237={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL9_LINE_ON=0;
        
__root const CFG_HDR cfghdr238={CFG_INT,lngDist,0,1000};
        __root const int LABEL9_LINE_DIST=0;
        
__root const CFG_HDR cfghdr239={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL9_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr240={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL9_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr241={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr242={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL9_NEED_REDRAW=0;
__root const CFG_HDR cfghdr243={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr244={CFG_LEVEL,lngLABEL10,3,0};
   
__root const CFG_HDR cfghdr245={CFG_CHECKBOX,lngShow,0,0};
        __root const int LABEL10_SHOW=0;
        
__root const CFG_HDR cfghdr246={CFG_COORDINATES,lngPos,0,0};
        __root const int LABEL10_X_POS=0;
        __root const int LABEL10_Y_POS=0;
        
__root const CFG_HDR cfghdr247={CFG_COLOR_INT,lngColor,0,0};
         __root const unsigned int LABEL10_COLOR1=clWhite;
      
__root const CFG_HDR cfghdr248={CFG_COLOR_INT,lngColor2,0,0};
        __root const unsigned int LABEL10_COLOR2=clBlack;
      
__root const CFG_HDR cfghdr249={CFG_FONT,lngFont,0,0};
        __root const int LABEL10_FONT=0x11;
        
__root const CFG_HDR cfghdr250={CFG_CBOX,lngCT,0,3};
        __root const int LABEL10_CT=0;
__root const CFG_CBOX_ITEM cfgcbox251[3]={lngLeft,lngRight,lngCenter};
        
__root const CFG_HDR cfghdr252={CFG_STR_WIN1251,lngMask,0,255};
        __root const char LABEL10_NAME[256]="$n";
        
__root const CFG_HDR cfghdr253={CFG_LEVEL,lngScrolling,4,0};
        
__root const CFG_HDR cfghdr254={CFG_CHECKBOX,lngScrollYes,0,0};
              __root const int LABEL10_SCROLL_YES=0;
              
__root const CFG_HDR cfghdr255={CFG_CBOX,lngScrollWhere,0,4};
              __root const int LABEL10_SCROLL_LR=0;
__root const CFG_CBOX_ITEM cfgcbox256[4]={lngScrollToLeft,lngScrollToRight, lngScrollFromLeftToRight, lngScrollFromLeftToRight2};
              
__root const CFG_HDR cfghdr257={CFG_INT,lngScrollLeftPos,0,5000};
              __root const int LABEL10_SCROLL_LEFT=0;
        
__root const CFG_HDR cfghdr258={CFG_INT,lngScrollRightPos,0,5000};
              __root const int LABEL10_SCROLL_RIGHT=0;
              
__root const CFG_HDR cfghdr259={CFG_INT,lngScrollSymLen,0,5000};
              __root const int LABEL10_SCROLL_LENGTH=15;
        
__root const CFG_HDR cfghdr260={CFG_INT,lngScrollStrLen,0,5000};
              __root const int LABEL10_SCROLL_STRLEN=10;
              
__root const CFG_HDR cfghdr261={CFG_INT,lngScrollMP,0,5000};
              __root const int LABEL10_SCROLL_MP=20;
              
__root const CFG_HDR cfghdr262={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr263={CFG_LEVEL,lngCrLf,4,0};

__root const CFG_HDR cfghdr264={CFG_CHECKBOX,lngCrLfEnable,0,0};
        __root const int LABEL10_LINE_ON=0;
        
__root const CFG_HDR cfghdr265={CFG_INT,lngDist,0,1000};
        __root const int LABEL10_LINE_DIST=0;
        
__root const CFG_HDR cfghdr266={CFG_INT,lngStrLength,0,1000};
        __root const int LABEL10_LINE_SIZE=100;
        
__root const CFG_HDR cfghdr267={CFG_INT,lngCrMinus,0,1000};
        __root const int LABEL10_LINE_MINUS=0;
        
__root const CFG_HDR cfghdr268={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr269={CFG_CHECKBOX,lngRedrawEverySecond,0,0};
        __root const int LABEL10_NEED_REDRAW=0;
        
__root const CFG_HDR cfghdr270={CFG_LEVEL,"",0,0}; 
   
__root const CFG_HDR cfghdr271={CFG_LEVEL,"",0,0};
   
__root const CFG_HDR cfghdr272={CFG_LEVEL,lngCover,1,0};

__root const CFG_HDR cfghdr273={CFG_CHECKBOX,lngShow,0,0};
        __root const int COVER_SHOW=1;
        
__root const CFG_HDR cfghdr274={CFG_RECT,lngPosAndSize,0,0};
        __root const RECT CoverRC={0,150,26,176};

__root const CFG_HDR cfghdr275={CFG_CHECKBOX,lngCoverUsePng,0,0};
        __root const int COVER_GETPNG=1;
        
__root const CFG_HDR cfghdr276={CFG_CBOX,lngDraw,0,3};
        __root const int COVER_DRAW=1;
__root const CFG_CBOX_ITEM cfgcbox277[3]={lngDrawEve,lngDrawSta,lngDrawOth};
        
__root const CFG_HDR cfghdr278={CFG_CHECKBOX,lngDrawOnStandby,0,0};
        __root const int Cover_DrawStandby=1;
        
__root const CFG_HDR cfghdr279={CFG_CHECKBOX,lngDrawOnSoft,0,0};
        __root const int Cover_DrawSoft=0;
        
__root const CFG_HDR cfghdr280={CFG_CHECKBOX,lngDrawOnStatus,0,0};
        __root const int Cover_DrawStatus=0;
        
__root const CFG_HDR cfghdr281={CFG_CBOX,lngCoverFunc,0,3};
__root const int GetCover_SWI=0;
__root const CFG_CBOX_ITEM cfgcbox282[3]={lngCoverFuncInt,lngCoverFuncLib, lngCoverFuncIntIndirect};

__root const CFG_HDR cfghdr283={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr284={CFG_LEVEL,lngCustomImage,1,0};

__root const CFG_HDR cfghdr285={CFG_CHECKBOX,lngShow,0,0};
        __root const int CustomImage_Show=1;
        
__root const CFG_HDR cfghdr286={CFG_RECT,lngPosAndSize,0,0};
        __root const RECT CustomRC={0,0,0,0};

__root const CFG_HDR cfghdr287={CFG_STR_WIN1251,lngFilename,0,255};
        __root const char CustomImage[256]="Image.png";
        
__root const CFG_HDR cfghdr288={CFG_CBOX,lngDraw,0,3};
        __root const int CustomImage_Draw=1;
__root const CFG_CBOX_ITEM cfgcbox289[3]={lngDrawEve,lngDrawSta,lngDrawOth};
        
__root const CFG_HDR cfghdr290={CFG_CHECKBOX,lngDrawAfterCover,2,0};
        __root const int CustomImage_DrawAfter=1;
        
__root const CFG_HDR cfghdr291={CFG_CHECKBOX,lngDrawOnStandby,0,0};
        __root const int Custom_DrawStandby=1;
        
__root const CFG_HDR cfghdr292={CFG_CHECKBOX,lngDrawOnSoft,0,0};
        __root const int Custom_DrawSoft=0;
        
__root const CFG_HDR cfghdr293={CFG_CHECKBOX,lngDrawOnStatus,0,0};
        __root const int Custom_DrawStatus=0;
        
        
__root const CFG_HDR cfghdr294={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr295={CFG_LEVEL,lngProgressBar,1,0};

__root const CFG_HDR cfghdr296={CFG_CHECKBOX,lngShow,0,0};
        __root const int ProgressBar_Show=1;

__root const CFG_HDR cfghdr297={CFG_CHECKBOX,lngProgressBarFromTheme,0,0};
__root const int ProgressBar_ColorFromTheme=0;
        
__root const CFG_HDR cfghdr298={CFG_RECT,lngPosAndSize,0,0};
        __root const RECT pbrect={0,240,130,140};
__root const CFG_HDR cfghdr299={CFG_LEVEL,lngColors,1,0};
           
__root const CFG_HDR cfghdr300={CFG_COLOR_INT,lngNowTime,0,0};
           __root const unsigned int ProgressBar_cColor=0x9F000000;
           
__root const CFG_HDR cfghdr301={CFG_COLOR_INT,lngLeftTime,0,0};
           __root const unsigned int ProgressBar_oColor=0xFFFFFF;
           
__root const CFG_HDR cfghdr302={CFG_COLOR_INT,lngHighlight,0,0};
           __root const unsigned int ProgressBar_bColor=0x6FFFFFFF;
           
__root const CFG_HDR cfghdr303={CFG_LEVEL,"",0,0}; 
         
__root const CFG_HDR cfghdr304={CFG_CBOX,lngStyle,0,2};
        __root const int ProgressBar_Style=0;
__root const CFG_CBOX_ITEM cfgcbox305[2]={lngStyleDefault,lngStyleRounded};

__root const CFG_HDR cfghdr306={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr307={CFG_LEVEL,lngAnim,2,0};
       
__root const CFG_HDR cfghdr308={CFG_CHECKBOX,lngAnim,0,2};
        __root const int AllowAnim=1;
        
__root const CFG_HDR cfghdr309={CFG_STR_WIN1251,lngAnimfname,0,255};
        __root const char ANIM_NAME[256]="standart";
__root const CFG_HDR cfghdr310={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr311={CFG_LEVEL,lngOptions,1,0};

__root const CFG_HDR cfghdr312={CFG_LEVEL,lngConfiguration,1,0};
__root const CFG_HDR cfghdr313={CFG_LEVEL,lngWhenInStandby,2,0};
__root const CFG_HDR cfghdr314={CFG_CBOX,lngZoneStandby,0,4};
        __root const int DrawStandby=2;
__root const CFG_CBOX_ITEM cfgcbox315[4]={lngOnlyStandart,lngStandartCover,lngCoverStandart, lngOnlyCover};
         
__root const CFG_HDR cfghdr316={CFG_CBOX,lngZoneSoft,0,4};
        __root const int DrawSoftkey=1;
__root const CFG_CBOX_ITEM cfgcbox317[4]={lngOnlyStandart,lngStandartCover,lngCoverStandart, lngOnlyCover};
        
__root const CFG_HDR cfghdr318={CFG_CBOX,lngZoneStatus,0,4};
        __root const int DrawStatus=1;
__root const CFG_CBOX_ITEM cfgcbox319[4]={lngOnlyStandart,lngStandartCover,lngCoverStandart, lngOnlyCover};

__root const CFG_HDR cfghdr320={CFG_LEVEL,"",0,0}; 
__root const CFG_HDR cfghdr321={CFG_LEVEL,lngWhenNotInStandby,2,0};
        
__root const CFG_HDR cfghdr322={CFG_CBOX,lngZoneSoft,0,4};
            __root const int DrawSoft2=1;
__root const CFG_CBOX_ITEM cfgcbox323[4]={lngOnlyStandart,lngStandartCover,lngCoverStandart, lngOnlyCover};
        
__root const CFG_HDR cfghdr324={CFG_CBOX,lngZoneStatus,0,4};
            __root const int DrawStatus2=1;
__root const CFG_CBOX_ITEM cfgcbox325[4]={lngOnlyStandart,lngStandartCover,lngCoverStandart, lngOnlyCover};
          
__root const CFG_HDR cfghdr326={CFG_LEVEL,"",0,0}; 

__root const CFG_HDR cfghdr327={CFG_LEVEL,lngUsedZones,2,0};
                
__root const CFG_HDR cfghdr328={CFG_CHECKBOX,lngZoneStatus,0,2};
            __root const int AffectStatus=1;
            
__root const CFG_HDR cfghdr329={CFG_CHECKBOX,lngZoneSoft,2,0};
            __root const int AffectSoft=0;
            
__root const CFG_HDR cfghdr330={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr331={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr332={CFG_CHECKBOX,lngAverage,0,0};
        __root const int AverageVBR=0;
        
__root const CFG_HDR cfghdr333={CFG_LEVEL,lngScrolling,2,0};
__root const CFG_HDR cfghdr334={CFG_CHECKBOX,lngScrollOn,0,0};
            __root const int ScrollWhenNot=0;
            
__root const CFG_HDR cfghdr335={CFG_CHECKBOX,lngNSM,0,0};
            __root const int ScrollingMode=0;
            
__root const CFG_HDR cfghdr336={CFG_LEVEL,"",0,0}; 
        
__root const CFG_HDR cfghdr337={CFG_CBOX,lngStartupMode,0,3};
        __root const int StartupMode=0;
__root const CFG_CBOX_ITEM cfgcbox338[3]={lngStartupNoWait,lngStartupWait, lngStartupFirstLaunch};
        
__root const CFG_HDR cfghdr339={CFG_INT,lngUpdateTime,1,5000};
__root const int DefUpdateTime=1000;
        
__root const CFG_HDR cfghdr340={CFG_LEVEL,"",0,0}; 

