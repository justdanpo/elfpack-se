#ifndef _MAIN_H_
#define _MAIN_H_

//#define font_step 5
#define max_size 100
#define bold 1
#define italic 2

typedef struct
{
	wchar_t path[128];
	wchar_t name[128];
	unsigned char *cfg;
	int size_cfg;
	unsigned int level;
	CFG_HDR *levelstack[16];
}BCFG_DATA;

enum SELF {
	SFOLDER=0,
	SFILE=1
};

typedef struct _MyBOOK : BOOK
{
	GUI_LIST *bcfg;
	GUI_ONEOFMANY *cbox_gui;
	GUI *text_input;
	GUI *yesno;
	GUI *color;
	GUI *coord;
	GUI *tinput;
	GUI *dinput;
	GUI *font_select;
	GUI_FEEDBACK *key_input;
	GUI_LIST * key_sel_list;
	GUI_ONEOFMANY * keymode_sel_list;
	GUI_LIST *selectf;
	int type;

	LIST *list;

	union
	{
		CFG_HDR* hdr;
		CFG_HDR_DATE* date;
		CFG_HDR_TIME* time;
		CFG_HDR_KEY* key;
		CFG_HDR_COLOR* color;
		CFG_HDR_RECT* rect;
		CFG_HDR_POINT* point;
		CFG_HDR_FONT* font;
		CFG_HDR_CHECKBOX* checkbox;
		CFG_HDR_SIGNEDINT* signedint;
		CFG_HDR_UNSIGNEDINT* unsignedint;
		CFG_HDR_CBOX* cbox;
		CFG_HDR_STR* str;
		CFG_HDR_WSTR* wstr;
	} cur_hp;

	unsigned long old_crc;
	TEXTID check_box_unchecked;
	TEXTID check_box_checked;
	TEXTID changes_have_been_made;
	TEXTID save_before_exit;
	BCFG_DATA bdata;
}MyBOOK;

typedef struct
{
	BOOK * book;
}MSG;

void RefreshEdList(BOOK * bk);
int TerminateElf(void * ,BOOK* book);
int ShowAuthorInfo(void *mess ,BOOK* book);
int MainPageOnCreate(void *, BOOK *bk);
int MainPageOnClose(void *, BOOK *bk);
int isBcfgEditBook(BOOK * struc);
GUI *create_ed(BOOK *book, CFG_HDR *need_to_focus);
void win12512unicode(wchar_t *ws, const char *s, int len);
int SaveCfg();

extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);
extern "C" long wcstol(const wchar_t *, wchar_t **, int);

#define IDN_CHANGES_HAVE_BEEN_MADE L"HPB_SAVE_BEFORE_EXITD_TXT"
#define IDN_SAVE_BEFORE_EXIT L"HPB_SAVE_BEFORE_EXITQ_TXT"

#define IDN_CHECKBOX_UNCHECKED_ICON L"OFF_TXT"
#define IDN_CHECKBOX_CHECKED_ICON L"ON_TXT"

#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
#endif
