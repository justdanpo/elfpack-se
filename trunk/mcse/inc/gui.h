#ifndef _GUI_H
#define _GUI_H

#define FONT_N   FONT_SMALL
#ifdef ELKA
#define FONT_B   FONT_SMALL
#else
#define FONT_B   (FONT_SMALL+1)
#endif


#define MAIN_B	2

#define TOP_B	2
#define TOP_Y   (MAIN_B)  // y1 заголовка
#define TOP_H   (TOP_B+head_h+TOP_B)  // Высота заголвка
#define DRV_O   2
#define DRV_X   MAIN_B
#define DRV_W   (head_h*7/2)
#define TAB_X   (scr_w-MAIN_B-TAB_W)
#define TAB_W   (TOP_H-2)
#define SRT_X   (DRV_X+DRV_O*MAX_DRV+DRV_W)
#define SRT_W   (TOP_H-2)
#define IND_X1  (SRT_X+SRT_W)
#define IND_X2  (TAB_X-1)

#define FLS_B   1          // fl border
#define FLS_Y   (TOP_Y+TOP_H+FLS_B)   // y1 списка файлов
#define FLS_H   (scr_h-FLS_B-FLS_Y)

#define ITM_B	1  // Item border
#define ITM_S	1
#define ITM_FH  (ITM_H+ITM_S)
#define ITM_X1  MAIN_B
#define ITM_X2  SB_X
#define ITM_H   (ITM_B+itm_ch+ITM_B) // Item height
#define ITM_W   MAIN_B
#define ICO_X	(ITM_X1+ITM_B)
#define ICO_DY  ((ITM_H-ico_h) / 2)
#define TXT_X	(ICO_X+ico_h)

#define ATTR_B  ITM_B
#define ATTR_S  ITM_S
#define ATTR_FH (ATTR_H+ATTR_S)
#define ATTR_H  (ITM_B+attr_h+ITM_B) // Item height

#define SB_HS   ((scr_w*3)>>7)
#define SB_VS   (FLS_H>>4)
#define SB_B	1
#define SB_X	(scr_w-MAIN_B-SB_HS)
#define SB_H	(FLS_H-1-SB_B)

#define PRGB_X1  (5)   // x1 прогрессбара 
#define PRGB_X2  (scr_w-5-1)   // x2 прогрессбара 
#define PRGB_Y   (0+50) // y1 прогрессбара 
#define PRGB_H   (MAIN_B+txt_h+MAIN_B+txt_h+MAIN_B+PRGP_H+MAIN_B+MAIN_B+PRGP_H+MAIN_B)

#define PRG_AT_Y (PRGB_Y+MAIN_B)
#define PRG_PT_Y (PRG_AT_Y+MAIN_B+txt_h)
#define PRG_B1_Y (PRG_PT_Y+MAIN_B+txt_h)
#define PRG_B2_Y (PRG_B1_Y+MAIN_B+PRGP_H)

#define PRGP_X1  (PRGB_X1+4)  
#define PRGP_X2  (PRGB_X2-4)  
#define PRGP_H   5


extern int scr_h;
extern int scr_w;
extern int ico_h;
extern int ico_w;
extern int itms_max;

extern int NormalFont;
extern int BoldFont;

extern FILEINF *scfile;


void InitScr();
void DrwName();
void ShowFiles(GC *gc, RECT *rc);
void ShowProgr(GC *gc, RECT *rc);
void ScrollTimerProc (u16 tmr , void *);
void DisableScroll ();
#endif
