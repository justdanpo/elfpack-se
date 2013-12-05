#ifndef _MAP_OBJECT_H_
  #define _MAP_OBJECT_H_

#include "pngread.h"
#include "read_ini.h"

typedef GUI GUI_MAP;


typedef struct {
  DISP_OBJ dsp_obj;
  int x1,x2,y1,y2;
  int disp_x, disp_y;
  int total_x_pos, total_y_pos;
  MAIN_INI ini;
  int *scr_buf;
  LIST *png_cache;
  float Xres, Yres;
} DISP_OBJ_MAP;

GUI_MAP *CreateMapGUI(BOOK *bk);
#endif
