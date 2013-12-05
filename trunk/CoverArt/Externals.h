#ifndef _EXTERNALS_H_
  #include "structs.h"

  #define _EXTERNALS_H_

  #define SID_NULL 0x6FFFFFFF
  extern TRACK_DESC *currenttrack;
  //Глобальные
  wchar_t *checkfile(wchar_t *name);
  extern wchar_t extFolder[512];
  extern wchar_t intFolder[512];
  //extern wchar_t *mp3data;
  
  //Объекты дисплея
  extern DISP_DESC *Standby_DD;
  extern DISP_OBJ *Standby_DO;
  extern DISP_OBJ_ONREDRAW_METHOD Standby_DefaultRedraw;
  
  extern DISP_DESC *Softkey_DD;
  extern DISP_OBJ *Softkey_DO;
  extern DISP_OBJ_ONREDRAW_METHOD Softkey_DefaultRedraw;
  
  extern DISP_DESC *StatusIndication_DD;
  extern DISP_OBJ *StatusIndication_DO;
  extern DISP_OBJ_ONREDRAW_METHOD StatusIndication_DefaultRedraw;
  //Прочие переменные
  extern int locTime;
  extern int AnimUpdateTime;
  #define NUM 10
  extern LABEL labels[NUM];
  extern MUSIC nowmusic;
  extern UI_IMG img;
  extern int CurrentTime;
  //Координаты/размер обложки
  extern int locCover_X;
  extern int locCover_Y;
  extern int locCover_MaxX;
  extern int locCover_MaxY;
  
  extern int AnimYPlus;
  extern int AnimUpdateTime;
  extern int YSize;
  
  extern int StatusSize;
  extern int SoftSize;
  
  //Функции
  void RemoveAnimation();
  void Send_REDRAW_RELEASE();
  void Standby_CARedraw(DISP_OBJ* DO,int a,int b,int c);
  void Softkey_CARedraw(DISP_OBJ *DO, int a, int b, int c);
  void StatusIndication_CARedraw(DISP_OBJ *DO, int a, int b, int c);

  void CA_Kill();
  
  void optimized_InitLbls();
  void CustomLbls();
  void InitLbls();
  
  void Images_Unload();
  void Images_Load();
  
  void Anim_Init();
  void Anim_Free();
  int Anim_NextFrame();
  void NextScrollFrame(int mode);
  
  void char2unicode(wchar_t *ws, char const *s, int len);
  void Lang_Initialize();
  void Lang_Remove();
  
  void TrackDesc_Free(TRACK_DESC *track_desc);
  TRACK_DESC * TrackDesc_Get(BOOK *bk);
  bool TrackDesc_Compare(TRACK_DESC *t1, TRACK_DESC *t2);
  
  int Font_GetHeight(int FONT);
#endif
