#ifndef _CALC_OBJECT_H_
  #define _CALC_OBJECT_H_

enum MATH_OP
{
  DIG_1=0,
  DIG_2,
  DIG_3,
  DIG_4,
  DIG_5,
  DIG_6,
  DIG_7,
  DIG_8,
  DIG_9,
  DIG_POINT,
  DIG_0,
  DIEZ_1,
  
  OP_SIN,
  OP_MULT,
  OP_DIV,
  OP_COS,
  OP_PLUS,
  OP_MINUS,
  OP_TAN,
  LEFTBRACKET,
  RIGHTBRACKET,
  OP_SQUARE,
  OP_SQRT,
  DIEZ_2,  
  
  OP_ASIN,
  OP_LN,
  OP_LOG,
  OP_ACOS,
  OP_EPOW,
  OP_NEG,
  OP_ATAN,
  OP_TENPOW,
  NUM_PI,
  OP_POW,
  SPACE,
  DIEZ_3,  
  
  OP_SH,
  OP_ASH,
  NUM_ANS,
  OP_CH,
  OP_ACH,
  OP_ABS,
  OP_TH,
  OP_ATH,
  OP_FAC,
  NUM_A,
  NUM_B,
  DIEZ_4,
 
  VAR_A,
  VAR_B,
  VAR_C,
  VAR_D,
  VAR_E,
  VAR_F,
  VAR_G,
  VAR_H,
  VAR_I,
  VAR_J,
  VAR_K,
  VAR_L,
  VAR_M,
  VAR_N,
  VAR_O,
  VAR_P,
  VAR_Q,
  VAR_R,
  VAR_S,
  VAR_T,
  VAR_U,
  VAR_V,
  VAR_W,
  VAR_X,
  VAR_Y,
  VAR_Z,
  
  TOTAL_OPS
};

typedef GUI GUI_CALC;

typedef struct {
  DISP_OBJ dsp_obj;
  int current_tab;
  int cursor_pos;
  int curx_pos;
  TEXTID answer_sid;
  int x1, x2, y1, y2;
  int cur_base;
  int maxintab;
  TEXTID yx[TOTAL_OPS];
  int names_len[TOTAL_OPS];
} DISP_OBJ_CALC;

void insert_operation(int op);
GUI_CALC *CreateCalkGUI(BOOK *bk);

extern double d_answer;
#define LGP_NULL 0x6FFFFFFF


#endif
