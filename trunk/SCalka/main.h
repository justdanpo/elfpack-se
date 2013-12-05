#ifndef _MAIN_H_
  #define _MAIN_H_

typedef struct
{
  BOOK  book;
  GUI *si; 
  GUI_LIST *calc_menu;
  GUI_LIST *calc_settings;
  GUI_ONEOFMANY *angle_sel_list;
  GUI_ONEOFMANY *autocalc_sel_list;
  GUI_LIST *calc_insvar;
  GUI *edit_fmtstr;
}MyBOOK;

int MainPageOnCreate(void *, BOOK *bk);
int MainPageOnClose(void *, BOOK *bk);
int MainPageOnTerminate(void *, BOOK *bk);
int isScalkaBook(BOOK * struc);
void win12512unicode(wchar_t *ws, const char *s, int len);
char *unicode2win1251(char *s, wchar_t *ws, int len);

#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)

#endif
