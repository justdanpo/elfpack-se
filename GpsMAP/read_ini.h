#ifndef _READ_INI_H_
  #define _READ_INI_H_


typedef struct {
  char *name;
  int map_index;
  float start_x;
  float start_y;
  float end_x;
  float end_y;
  int map_width, map_height;
  int map_arr_x, map_arr_y;
  int *sizes_x,*sizes_y;
  LIST *bs_dat;
} TOWN;

typedef struct {
  LIST *towns;
  TOWN *cur_town;
} MAIN_INI;

void MainIniInit(MAIN_INI *ini);
void MainIniDeInit(MAIN_INI *ini);
void ReadMainIni(MAIN_INI *ini);
float str_tof(char *str, char **end);
#endif
