#ifndef _MENUS_H
#define _MENUS_H

enum MENUS {
  main_ind=0,
  misc_ind,
  view_ind,
  TOTAL_MENUS
};

typedef struct {
  int id;
  void (*proc)(void);
  int enabled;
} MENU_DESC;

extern MENU_DESC opt_menu[];
extern MENU_DESC fl_menu[];
extern MENU_DESC st_menu[];
extern MENU_DESC vw_menu[];
extern MENU_DESC op_menu[];
extern MENU_DESC mis_menu[];
extern MENU_DESC bm_menu[];
void MM_Main(void);

#endif

