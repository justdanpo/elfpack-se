#ifndef __HEADER_PRESETS__
#define __HEADER_PRESETS__
  void preset_standby(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right);
  void preset_default(LABELS *lbl, BOOK *bk, DISP_OBJ *DO, LIST *lst, int cnt, MyBOOK *our, int left, int right);
  extern int presets[];
  extern ITEM *lastitem;
#define T_STANDBY 0
#define T_DEFAULT 1
#define T_YESNO 2
#define T_MESSAGEBOX 3
#define T_DDMENU 4
#define T_3SK_LAYOUT 5


#endif
