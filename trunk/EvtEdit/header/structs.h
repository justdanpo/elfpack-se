#ifndef STRUCTS_H
  #define STRUCTS_H
/*
  typedef struct
  {
    int type;
    TIME time;
    int day;
    char *param; // ѕараметр может быть разных типов, поэтому оставл€ю в чаре :-)
  }EVENT;
  */

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
    int type;
    int AskBefore;
    bool isMinusInBefore;
    int AskAfter;
    bool isGps;
    bool isMinusInAfter;
    int dtype;
    DATE date;
    DATE date2;
    bool days[7];
    int ttype;
    TIME time;
    TIME time2;
    wchar_t *param;
    wchar_t *txt;
    //char *param;
    //char *txt;
    char *gps;
    int rem;
  }EVENT;
  typedef struct
  {
    BOOK  book;
    LIST *events;
    EVENT *selev;
    //------------
    GUI_LIST *lst;
    int main_lastindex;
    int opt_lastindex;
    //------------
    GUI_LIST *addgui;
    GUI_ONEOFMANY *cbox_gui;
    //------------
    GUI *text_input;
    int minint;
    int maxint;
    int TextInputType;
    int inputType;
    //------------
    GUI *yesno;
    //------------
    GUI *tinput;
    TIME *curtime;
    //------------
    GUI *dinput;
    DATE *curdate;
    //------------
    GUI_LIST *days;
    int daystype;
    bool *curdays;
    //------------
    GUI_LIST *javalst;
    GUI_LIST *profiles;
    GUI_LIST *ind;
    //------------
    int dir1;
    int dir2;
    int fType;
    int selectfiletype;
    //-------------------
    int tempvol;
    //-------------------
    GUI *vol;
    int curvol;
    int maxvol;
    int vol_mod;
    int vol_type;
    //-------------------
    int prof_type;
    unsigned long old_crc;
    u16 check_box_unchecked;
    u16 check_box_checked;
    TEXTID changes_have_been_made;
    TEXTID save_before_exit;
    GUI_LIST *switch_gui;
    LIST *switch_lst;
    int cur_clone_time;
    int rem_types;
    wchar_t *indpath;
    wchar_t *indname;
  } MyBOOK;
  /*
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
  */
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
