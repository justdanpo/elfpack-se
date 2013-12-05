#ifndef _CALC_MENU_H_
  #define _CALC_MENU_H_

typedef enum {
  DEGREES=0,
  RADIANS,
  GRADS
}DRG;
  
typedef struct {
  double x;
  double y;
  char fmt[16];
  char drg;
  char auto_recalc;
  int n_vars;  
} CALC_SETTINGS;

void CreateCalcMenu(void);

extern CALC_SETTINGS calc_set;

void WriteCalcSettings(void);
void ReadCalcSettings(void);
#endif
