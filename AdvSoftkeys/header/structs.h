#ifndef __HEADER_TYPES__
  #define __HEADER_TYPES__

  typedef int (*DISP_OBJ_ONREDRAW_METHOD2)(DISP_OBJ *,int ,int,int);
  typedef wchar_t* (*UPDATE)();
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
  }MyBOOK;

  typedef struct
  {
    BOOK * book;
  }MSG;
  
  typedef struct
  {
    wchar_t *name;
    int strids[3];
    int style;
    LIST *keys;
    int type;
  }ITEM;
  
  typedef struct
  {
    int oldkey;
    int newkey;
  }KEY;

  typedef struct
  {
     u16 action;
     u16 unk;
     BOOK * book;
     DISP_OBJ * DISP_OBJ1;
     u16 unk2; 
     u16 unk3;
     int text;
     int text_mini;
     int text_disabled;
     int text_help;
     u16 parent_action;
     u16 unk4;
     LIST * list;
     void (*proc)(BOOK *,void*);
     char unk5[12]; 
     char enable;
     char visible;
     char unk6[6];
  }SOFTKEY;
#pragma pack(1)
  typedef struct
  {
    void *unk1;
    wchar_t intFFFF;
    wchar_t visible_softs_count;
    int unk3; 
    char unk4[4]; 
    void *unk5;
    void *action_list;
    void *unk7;
    char smth;
    char visible;
    char background;
    char unk9;
    int color;
  }SOFTKEY_PARAMS;
#pragma pack()
  typedef struct
  {
    int strids[3];
    bool enable[3];
  }LABELS;
  
#endif
