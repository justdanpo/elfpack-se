#ifndef _STRUCTBOOK_H_
  #define _STRUCTBOOK_H_

static const char NameMyElf[]="Gono4ki";

typedef struct
{
  wchar_t name[10][8];
  int rezultat[10];
}TABL;

typedef struct
{
  TABL*te;
  TABL*tn;
  TABL*th;
}TAB;

typedef struct
{
  wchar_t names[3][128];
  int on;
}IMGN;

typedef struct
{
  wchar_t names[6][256];
  int status[6];
  int sound;
}SOUND;

typedef struct
{
  SOUND*sound;
  IMGN*im;
  TAB*rez;
}NASTROIKI;

typedef struct
{
  IMAGEID ImageID;
  u16 ImageHandle;
  signed int x;
  signed int y;
  signed int mx;
  signed int my;
  bool isImage;
}IMG;

typedef struct
{
  IMG*im[3];
  int fi[3];
}IMAGES;

typedef struct
{
  int x;
  int y;
}COORDINAT;

typedef struct
{
  int x[4];
  int y;
}PREGRADA;

typedef struct
{
  int x;
  int y;
  int cub;
  int FP;
  int FS;
}DISPLAY;

typedef struct
{
  DISP_OBJ deob;
  BOOK bk;
  GUI*guido;
  GUI*gui1;
  GUI_LIST*gui;
  int cub[10][10];
  COORDINAT*tank;
  IMAGES*image;
  int lvl;
  int speed;
  int point;
  NASTROIKI*nast;
  int pause;
  DISPLAY*dis[3];
  DISP_OBJ_ONKEY_METHOD OldKey1;
  GUI*guigame;
  PREGRADA*p[2];
  int megaspeed;
  int palka;
}MyBook;

typedef struct
{
  BOOK * book;
}MSG;



#endif //_STRUCTBOOK_H_
