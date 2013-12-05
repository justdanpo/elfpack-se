#ifndef EXTERNALS_H
  #define EXTERNALS_H
  #define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
#define DELETE(a) if (a) { delete(a);a=0; }

int GetStrID(wchar_t *txt);
#endif
