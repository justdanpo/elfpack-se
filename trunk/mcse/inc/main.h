#ifndef _MAIN_H
#define _MAIN_H

typedef struct
{
  BOOK  book;
  GUI *main_gui;
  GUI *stop_progr;
  GUI *yes_no;
  GUI *textinp;
  int tstname;
  IFN_RET_PROC retproc;
  void (*YesNoFunc)(int); //  1 - yes, 0 - no
  LIST *lst_send;
  
  FILEITEM *fitem;
  SUB_EXECUTE *subr; 
}MyBOOK;

typedef struct {
  DISP_OBJ dsp_obj;
} DISP_OBJ_MAIN;

typedef GUI MAIN_GUI;

extern int is_db2010;
#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
#endif
