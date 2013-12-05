#ifndef _CFG_ITEMS_H_
	#define _CFG_ITEMS_H_
	
#include "Types.h"

#define CFGVAR _Pragma("location=\"CONFIG_C\"") __root const

#define CFG_UINT 1
#define CFG_STR_UTF8 2
#define CFG_INT 3
#define CFG_STR_WIN1251 4
#define CFG_CBOX 5
#define CFG_STR_PASS 6
#define CFG_COORDINATES 7
#define CFG_COLOR 8
//Level
//min<>0 - start tag (name used)
//min=0 - end tag
#define CFG_LEVEL 9

#define CFG_UTF8_STRING 10
#define CFG_CHECKBOX 11
#define CFG_TIME 12
//TTime
#define CFG_DATE 13
//TDate
#define CFG_RECT 14
//RECT
#define CFG_COLOR_INT 15
#define CFG_FONT 16
#define CFG_KEYCODE 17

#define CFG_UTF16_STRING 18

typedef struct
{
	int type;
	char name[32];
	unsigned int min;
	unsigned int max;
}CFG_HDR;

typedef struct
{
	char cbox_text[32];
}CFG_CBOX_ITEM;


typedef struct
{
	wchar_t path[64];
	wchar_t name[64];
} RECONFIG_EVENT_DATA;

typedef struct
{
	CFG_HDR;
	TIME time;
}CFG_HDR_TIME;

typedef struct
{
	CFG_HDR;
	DATE date;
}CFG_HDR_DATE;

typedef struct
{
	CFG_HDR;
	int keycode;
	int keymode;
}CFG_HDR_KEY;

typedef struct
{
	CFG_HDR;
	union
	{
		unsigned int color;
		char rgb[4];
	};
}CFG_HDR_COLOR;

typedef struct
{
	CFG_HDR;
	RECT rect;
}CFG_HDR_RECT;

typedef struct
{
	CFG_HDR;
	POINT point;
}CFG_HDR_POINT;

typedef struct
{
	CFG_HDR;
	int font;
}CFG_HDR_FONT;

typedef struct
{
	CFG_HDR;
	int isChecked;
}CFG_HDR_CHECKBOX;

typedef struct
{
	CFG_HDR;
	int signedint;
}CFG_HDR_SIGNEDINT;

typedef struct
{
	CFG_HDR;
	unsigned int unsignedint;
}CFG_HDR_UNSIGNEDINT;

typedef struct
{
	CFG_HDR;
	int selected;
	CFG_CBOX_ITEM items[1];
}CFG_HDR_CBOX;

typedef struct
{
	CFG_HDR;
	char chars[1];
}CFG_HDR_STR;


typedef struct
{
	CFG_HDR;
	wchar_t chars[1];
}CFG_HDR_WSTR;


#endif
