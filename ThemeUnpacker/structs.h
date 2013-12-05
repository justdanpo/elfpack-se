#ifndef STRUCTS_H
  #define STRUCTS_H

  typedef struct
  {
    BOOK  book;
    GUI_LIST *lst;
    wchar_t *path;
    wchar_t *name;
  } MyBOOK;

#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
  
#endif
