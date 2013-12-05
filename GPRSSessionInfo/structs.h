#ifndef STRUCTS_H
  #define STRUCTS_H
  extern const PAGE_DESC bk_base;
  extern const PAGE_DESC bk_main;
  typedef struct
  {
    BOOK  book;
    GUI_FEEDBACK *mbox;
    int ShouldClose;
    int stat;
  } MyBOOK;

  typedef struct
  {
    BOOK * book;
  }MSG;
  
  typedef struct
  {
    int rtraf;
    int traf;
    float cost;
    float in;
    float out;
  } TRAF;
  
  #define SID_NULL 0x6FFFFFFF
  void elf_exit(void);
  void InitConfig(void);
  int isGSIBook(BOOK *bk);
#endif
