#ifndef _VISUAL_H_
#define _VISUAL_H_

#include "main.h"


#define COLOR_GET_A(x) ((unsigned int)x>>24)
#define COLOR_GET_R(x) (((unsigned int)x>>16)&0xFF)
#define COLOR_GET_G(x) (((unsigned int)x>>8)&0xFF)
#define COLOR_GET_B(x) ((unsigned int)x&0xFF)

#define COLOR_RGBA(r,g,b,a) (((unsigned int)r<<16)|((unsigned int)g<<8)|((unsigned int)b)|((unsigned int)a<<24))

typedef struct GUI GUI_COLOR;

typedef struct _DISP_OBJ_COLOR : DISP_OBJ
{
	int r, g, b, a;
	int type;
	int current_column;
	TEXTID str_id;
	int need_str;
	int cstep;
} DISP_OBJ_COLOR;

typedef union {
	unsigned int int_color;
	char *char_color;
} COLOR_TYPE;

typedef struct GUI GUI_COORDINATES;

typedef struct _DISP_OBJ_COORD : DISP_OBJ
{
	int x, y, x2, y2;
	int type;
	int is_first_set;
	RECT old_rect;
	TEXTID str_id;
	int need_str;
	int cstep;
	MyBOOK * mb;
} DISP_OBJ_COORD;

typedef union {
	int xy[2];
	RECT rc;
} COORD_TYPE;


typedef struct GUI GUI_FONT_SEL;

typedef struct _DISP_OBJ_FONT_SEL : DISP_OBJ
{
	int cur_font;
	int cur_offs, cur_pos;
	int total_fonts;
	TEXTID test_str_id;
	u16 *font_heights;
	int req_check_vis;
	int platform_flag;
	int style_bold;
	int style_italic;
} DISP_OBJ_FONT_SEL;

GUI_COLOR *CreateEditColorGUI(MyBOOK * myBook, int type);
GUI_COORDINATES *CreateEditCoordinatesGUI(MyBOOK * myBook, int type);
//GUI_FONT_SEL *CreateFontSelectGUI(MyBOOK * myBook);
GUI_FONT_SEL* CreateFontSelectGUI( MyBOOK* myBook, int step );
wchar_t *Font_GetNameByFontId(int id);
GUI_LIST *CreateFileFolderSelect(MyBOOK * myBook, wchar_t *str);
void Free_FLIST(void);
#endif
