#include "..\classlib\cl.h"
#ifndef MYCLASS_H_
#define MYCLASS_H_
enum COLOR
{
  BLACK=0,//0xFF000000;
  BLUE=1,//0xFF0000FF;
  GREEN=2,//0xFF00FF00;
  LIGHTBLUE=3,//0xFF00FFFF;
  RED=4,//0xFFFF0000;
  PURPLE=5,//0xFFFF00FF;
  YELLOW=6,//0xFFFFFF00;
  WHITE=7//0xFFFFFFFF;
};

typedef struct
{
  bool state;
  COLOR col;
}PXL;

typedef struct
{
  int x,y;
}XY;

typedef struct
{
 BOOK* book;
 wchar_t *bcfg_p;
 wchar_t *bcfg_n;
}MSG_BCFG;

#ifdef NDEBUG
#define DEBUG
#else
#define DEBUG CDbgPrint __function((char*)__FILE__,(char*)__FUNCTION__)
#endif
#define MESSAGE(x) MessageBox(EMPTY_TEXTID,TextID_Create(x,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0)
class CDbgPrint
{
  char* fl;
  char* fn;
public:
  CDbgPrint(char* file,char* func){debug_printf("\nfunction enter: %s/%s\n",fl=file,fn=func);}
  ~CDbgPrint(){debug_printf("\nfunction leave: %s/%s\n",fl,fn);}
};
class map
{
  bool**_map;
  int _x,_y;
public:
  map(int x,int y);
  void set(int x,int y);
  bool getstate(int x,int y);
  ~map();
};


class display
{
  PXL**_display;
  int _x,_y;
  int _pixel;
  GC*gc_pixel[8];
  int getColor(COLOR col);
public:
  display(int x,int y,int pixel);
  void clear();
  void set(int x,int y,COLOR col,bool state);
  int getx();
  int gety();
  void out(GC*gc);
  ~display();
};

class snake
{
  LIST*list;
  char _way;
  int _mx,_my;
  int _waych;
public:
  snake(int x,int y,int n,char way,int maxx,int maxy);
  void move();
  bool check();
  XY*getlast();
  XY*getfirst();
  void add(int x,int y);
  void out(display*disp);
  void out2(map*m);
  void setway(char way);
  //
  int getlen();
  ~snake();
};
class food
{
  int _x,_y;
  int timeleft;
  bool state;
  COLOR _col;
public:
  food(int x,int y,COLOR col);
  int getx();
  int gety();
  bool getstate();
  void eated();
  void timedown(int dtime);
  int gettime();
  void out(display*disp);
  void out2(map*m);
  //
  int getcol();
  ~food();
};

class CGame
{
  bool pause;
  int img[10];
  display*disp;
  display*pausedisp;
  snake*sh;
  int score;
  int time;
  bool isgameover;
  bool isfast;
  LIST*foodlist;
  bool GetEmpty(int &x,int &y);
  void DrawNumeral(GC*gc,int &x,int &y,int numeral);
  void DrawNumber(GC*gc,int x,int y,int number);
public:
  CGame(int lvl);
  ~CGame();
  int GetScore();
  void OnKey(int key,int mode);
  void OnDraw(GC*gc);
  void OnMove();
  int GetRefreshTime();
  bool IsGameOver();
};


class CGameDisp:public CDispObjT<CGameDisp>
{
  bool isondraw;
  CGame*game;
  bool go_state;
public:
  static char* getName();
  void onDraw(int a,int b,int c);
  void onKey(int key,int,int repeat,int type);
  void onRefresh();
  void onDestroy();
  int onCreate();
};
class CMyBook:public CBook
{
  static int ShowAuthorInfo(CBook** bm_book,CMyBook* mbk);
  static int TerminateElf(CBook** bm_book,CMyBook* mbk);
  static int OnMainPage(void*,CMyBook* mbk);
  static int OnMyKey(int key,int r,int mode,CMyBook* mybook,DISP_OBJ*);
  ~CMyBook(void);
  CGuiBase*gui;
  DECLARE_PAGE_DESC_MEMBER(base_page);
  DECLARE_PAGE_DESC_MEMBER(main_page);
public:
  CMyBook(void);
};
#endif
