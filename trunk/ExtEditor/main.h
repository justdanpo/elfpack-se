#ifndef _MAIN_H
#define _MAIN_H

typedef struct
{
  BOOK  book;
  GUI_LIST *main_gui;
  char *ebuf;
  int ebuf_len;
  LIST *astr;
  
  GUI_LIST *edit_list;
  int edit_id; 
  char el_ext[36];
  char el_elf[128];
  char el_smicon[64];
  char el_bicon[64];
  int edit_list_selected;   // 1 -select elf, 2 - small, 3 - big
  
  GUI *si_editext;
  
  GUI *yes_no;
  void (*YesNoFunc)(BOOK * mbk, int); //  1 - yes, 0 - no
  
  int is_changed;
}MyBOOK;

typedef struct
{
  BOOK * book;
}MSG;


GUI_LIST * CreateGuiList(MyBOOK * bk);
#define LGP_NULL 0x6FFFFFFF
#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
#endif
