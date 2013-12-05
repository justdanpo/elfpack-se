#ifndef __STRUCTS_H
#define __STRUCTS_H

  
  typedef struct
  {
    wchar_t *name;
    wchar_t *lsi;
    wchar_t *msi;
    wchar_t *rsi;
    int style;
    LIST *keys;
  }ITEM;
  
  typedef struct
  {
    int oldkey;
    int newkey;
  }KEY;
  
  typedef int (*DISP_OBJ_ONREDRAW_METHOD2)(DISP_OBJ *,int ,int,int);
  typedef wchar_t (*UPDATE)();
    typedef struct
  {
    int x;
    int y;
    int ct;
    int font;
    int hx;
    int hy;
    int hct;
    int hfont;
  }UI_SOFTKEY;
  typedef void (*SK_CHANGE)(UI_SOFTKEY *sk, int x);
  typedef void (*SET_STATE)(bool state);
  typedef struct
  {
    BOOK bk;
    int (*onkey)(int key);
    DISP_OBJ_ONREDRAW_METHOD2 redraw;
    int funcstrid;
    int okstrid;
    int backstrid;
    UPDATE upd;
    SK_CHANGE change;
    SET_STATE SetWorkState;
    int version;
  }SKBOOK;
  typedef GUI GUI_SKIN;
  
  typedef struct 
  {
    DISP_OBJ DO;
    int curkey;
    int setting;
    int total_fonts;
    u16 *fonts;
    int cur;
    int step;
  } DISP_OBJ_SKIN;
  typedef struct
  {
    BOOK book;
    GUI *StringInput;
    GUI_LIST *lst;
    GUI_FEEDBACK *keyselect;
    GUI *skin;
    ITEM *curit;
    KEY *curkey;
    int main_lastindex;
    int opt_lastindex;
    int StringInputType;
    int inputType;
    int maxint;
    int key_stage;
  }MyBOOK;

  typedef struct
  {
    BOOK * book;
  }MSG;

  enum INPUT_TYPES {
  IT_REAL=0,
  IT_STRING=1,
  IT_INTEGER=2,
  IT_PHONE_NUMBER=3,
  IT_DIGITAL_PASS=4,
  IT_DIGITAL_IP=5,
  IT_URL=6,
  IT_UNSIGNED_DIGIT=9,
  IT_EXTRA_DIGIT=10,
  IT_ABC_OR_DIGIT=11,
  IT_EXTRA_DIGIT_2=12,
  IT_ABC_OR_DIGIT_2=13,
  };
#endif
