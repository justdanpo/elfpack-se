#ifndef STRUCTS_H
  #define STRUCTS_H
  typedef void (*UPDATE_METHOD)(wchar_t *path, wchar_t *name);
  
  typedef struct
  {
    wchar_t *text;
    wchar_t *utext;
    wchar_t *time;
    bool checked;
  }REMIND;
  
  typedef GUI GUI_REMIND;
  
  typedef struct 
  {
    DISP_OBJ DO;
    int num;
    LIST *rems;
  } DISP_OBJ_REMIND;

  typedef struct
  {
    BOOK bk;
    UPDATE_METHOD update;
    wchar_t *path;
    wchar_t *name;
    GUI_REMIND *remind;
    LIST *lst;
    LIST *remlst;
    GUI_LIST *g_test;
    GUI_FEEDBACK *g_mbox;
    int rem_types;
    wchar_t *indpath;
    wchar_t *indname;
  }MyBOOK;

  typedef struct
  {
    bool done;
    signed int tm_start;
    signed int tm_end;
    signed int ask_before;
    signed int ask_after;
    int remdays[7];
    int remdays2[7];
    bool after_done;
    bool before_done;
    //-------------------
    int type;
    int AskBefore;
    bool isMinusInBefore;
    int AskAfter;
    bool isGps;
    bool isMinusInAfter;
    int dtype;
    DATE date;
    DATE date2;
    int days[7];
    int ttype;
    TIME time;
    TIME time2;
    wchar_t *param;
    wchar_t *txt;
    char *gps;
    DATE date_0;
    DATE date_1;
    int d_shift_0;
    int d_shift_1;
    int day_1;
    int day_2;
    int rem;
  }EVENT;

  typedef struct
  {
    bool mon;
    bool tue;
    bool wed;
    bool ths;
    bool fri;
    bool sat;
    bool sun;
    bool workdays;
    bool weekend;
    bool everyday;
  }DAYS;
  
  typedef struct
  {
    BOOK book;
    void (*myled)(void *i,int id,int RED,int GREEN,int BLUE, int delay, int onoff);
    void (*changestate)(bool state);
  }CSBOOK;

  typedef struct
  {
    BOOK * bk;
    wchar_t *text;
    wchar_t *utext;
    wchar_t *time;
    bool vibra;
    bool replace;
    bool need_to_destroy;
  }REMINDER_STRUCT;

#endif
