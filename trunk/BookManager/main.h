#ifndef _MAIN_H_
#define _MAIN_H_

#define COPYRIGHT_STRING STR( "\nBookManager v3.42\nbuild 170112\nCopyright (c) 2007-2008\nHussein\n\nRespect\nIronMaster, KreN\n\n" )

#define BOOKLIST 0
#define ELFLIST 1

#define ICONS_COUNT 4
#define DIGITS_COUNT 10


#define IDN_BOOKS_DEACT_ICON _T( "RN_TAB_SESSION_MANAGER_DESELECTED_ICN" )
#define IDN_BOOKS_ACT_ICON _T( "RN_TAB_SESSION_MANAGER_SELECTED_ICN" )
#define IDN_ELFS_DEACT_ICON _T( "RN_TAB_BOOKMARKS_DESELECTED_ICN" )
#define IDN_ELFS_ACT_ICON _T( "RN_TAB_BOOKMARKS_SELECTED_ICN" )

#define MAX_BOOK_NAME_LEN 50

#define JAVA_BOOK_NAME "CUIDisplayableBook"

#define INI_BOOK_NAMES L"booknames.ini"
#define INI_SHORTCUTS L"shortcuts.ini"
#define INI_HIDDEN L"hidden.ini"

typedef enum
{
	TAB_BOOKS_SHORTCUTS_SOFTKEY = 0,
	TAB_BOOKS_RENAME_SOFTKEY,
	TAB_BOOKS_COPYRIGHT_SOFTKEY,
	TAB_BOOKS_SETTINGS_SOFTKEY,
	TAB_BOOKS_HIDEBOOK_SOFTKEY,
	TAB_BOOKS_SHOWBOOK_SOFTKEY,
	TAB_BOOKS_SHOWHIDDEN_SOFTKEY,
	TAB_BOOKS_HIDEHIDDEN_SOFTKEY,
	TAB_BOOKS_SHOWNOGUI_SOFTKEY,
	TAB_BOOKS_HIDENOGUI_SOFTKEY
}TAB_BOOKS_SOFTKEYS;

typedef enum
{
	TAB_ELFS_SHORTCUTS_SOFTKEY = 0,
	TAB_ELFS_RENAME_SOFTKEY,
	TAB_ELFS_AUTHOR_SOFTKEY,
	TAB_ELFS_SETTINGS_SOFTKEY,
	TAB_ELFS_HIDEBOOK_SOFTKEY,
	TAB_ELFS_SHOWBOOK_SOFTKEY,
	TAB_ELFS_SHOWHIDDEN_SOFTKEY,
	TAB_ELFS_HIDEHIDDEN_SOFTKEY,
	TAB_ELFS_SHOWNOGUI_SOFTKEY,
	TAB_ELFS_HIDENOGUI_SOFTKEY
}TAB_ELFS_SOFTKEYS;

#define TAB_BOOKS_SHORTCUTS_NAME_SOFTKEY L"SHC_EDIT_SHORTCUT_TXT"
#define TAB_BOOKS_RENAME_NAME_SOFTKEY L"DB_RENAME_TXT"
#define TAB_BOOKS_SETTINGS_NAME_SOFTKEY L"CALE_ADVANCED_TXT"

#define TAB_ELFS_SHORTCUTS_NAME_SOFTKEY L"SHC_EDIT_SHORTCUT_TXT"
#define TAB_ELFS_RENAME_NAME_SOFTKEY L"DB_RENAME_TXT"
#define TAB_ELFS_SETTINGS_NAME_SOFTKEY L"CALE_ADVANCED_TXT"

#define FLASH_MASK 0xF8000000


typedef struct
{
	wchar_t ImageID;
}IMG;

typedef struct _MYBOOK : BOOK
{
	GUI_TABMENUBAR* gui;
	wchar_t* booknames_buf;
	int booknames_buf_size;
	char* shortcuts_buf;
	int shortcuts_buf_size;
	char* hidden_buf;
	int hidden_buf_size;
	DISP_OBJ_ONKEY_METHOD oldOnKey;
	//  DISP_OBJ_ONKEY_METHOD oldOnKey1;
	LIST* sessions_list;
	LIST* books_list;
	LIST* elfs_list;
	LIST* java_list;
	GUI_LIST* java_list_menu;
	GUI_LIST* blist;
	GUI_LIST* elist;
	GUI_LIST* mode_list;
	GUI_LIST* but_list;
	GUI* YesNoQuestion;
	GUI* StringInput;
	IMG tabs_image[ICONS_COUNT];
	IMG digs_image[DIGITS_COUNT];
	int MainMenuID;
	wchar_t blistpos;
	wchar_t elistpos;
	wchar_t blistcnt;
	wchar_t elistcnt;
	wchar_t ActiveTAB;
	BOOL isA2;
	BOOL books_show_hid;
	BOOL books_show_nogui;
	BOOL elfs_show_hid;
	BOOL elfs_show_nogui;
}MyBOOK;


typedef struct
{
	BOOK * book;
	char * book_name;
	int gui_count;
	BOOL isJava;
	BOOL isJava_2010;
	BOOL isHidden;
}BOOK_LIST_ITEM;


typedef struct
{
	UI_APP_SESSION * session;
	LIST * books_list;
	wchar_t pos_subitem;
}SESSION_LIST_ITEM;


typedef struct
{
	int BookID;
}DESTROYBOOK_DATA;


typedef struct
{
	BOOK* book;
}MSG;


BOOK_LIST_ITEM * GetBookListItem( BOOK* bk, int list );
void RefreshBookSoftkeys( MyBOOK* mbk );
int IdlePage_EnterAction( void* r0, BOOK* bk );
int ReconfigElf( void* mess, BOOK* book );
void LoadBookNames(MyBOOK * mbk);
void LoadShortcuts(MyBOOK * mbk);
int CreateBookList( void* r0, BOOK* bk );
int RecreateBookList( void* r0, BOOK* bk );
int isBookManager( BOOK* struc );
int onUserInactivity( void* r0, BOOK* bk );
int onRootListChanged( void* r0, BOOK* bk );
void CloseMyBook( BOOK* Book, GUI* );
void CreateBookManagerGUI( MyBOOK* bk );
void PreTerminateManager( BOOK* Book, GUI* );
int NewKey( int key, int rep_count, int mode, MyBOOK * mbk, DISP_OBJ* );
int get_file( wchar_t* fname, void** buf_set );
void RefreshElfSoftkeys( MyBOOK* mbk, int item );
int GetActiveTab(MyBOOK*);
int GetUserBookNameTEXTID( char* name);
wchar_t * GetUserBookName(wchar_t * ini,wchar_t * orig_name,wchar_t * cur_name);
wchar_t* get_path();


#endif
