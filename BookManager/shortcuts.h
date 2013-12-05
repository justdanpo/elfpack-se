#ifndef _SHORTCUTS_H_
#define _SHORTCUTS_H_

#define IDN_DIGIT_0_ICON _T( "FM_RADIO_MHZ_DIGIT_0_ICN" )
#define IDN_DIGIT_1_ICON _T( "FM_RADIO_MHZ_DIGIT_1_ICN" )
#define IDN_DIGIT_2_ICON _T( "FM_RADIO_MHZ_DIGIT_2_ICN" )
#define IDN_DIGIT_3_ICON _T( "FM_RADIO_MHZ_DIGIT_3_ICN" )
#define IDN_DIGIT_4_ICON _T( "FM_RADIO_MHZ_DIGIT_4_ICN" )
#define IDN_DIGIT_5_ICON _T( "FM_RADIO_MHZ_DIGIT_5_ICN" )
#define IDN_DIGIT_6_ICON _T( "FM_RADIO_MHZ_DIGIT_6_ICN" )
#define IDN_DIGIT_7_ICON _T( "FM_RADIO_MHZ_DIGIT_7_ICN" )
#define IDN_DIGIT_8_ICON _T( "FM_RADIO_MHZ_DIGIT_8_ICN" )
#define IDN_DIGIT_9_ICON _T( "FM_RADIO_MHZ_DIGIT_9_ICN" )

typedef enum
{
	SHORTCUTS_MAINMENU_SOFTKEY = 0,
	SHORTCUTS_JAVA_SOFTKEY,
	SHORTCUTS_ELFS_SOFTKEY
}SHORTCUTS_SOFTKEYS;

#define SHORTCUTS_MAINMENU_NAME_SOFTKEY L"SHC_SET_MAINMENU_TXT"

typedef struct
{
	int str_id;
	int icon_id;
}SC_DATA;


typedef struct
{
	wchar_t* name;
	wchar_t* fullpath;
	wchar_t* hash_name;
	IMAGEID imageID;
}java_list_elem;


int CreateModeList( void* data, BOOK* book );
int ModeList_CancelEvent_Action( void* data, BOOK* bk );
int ExitShortcutsSet( void* data, BOOK* bk );
int CreateButtonList( void* data, BOOK* book );
int ButtonList_Cancel_Event_Action( void* data, BOOK* book );
int ExitButtonList( void* data, BOOK* book );
int CreateJavaList( void* data, BOOK* book );
int onExit_JavaList( void* data, BOOK* book );
int CreateDB( void* data, BOOK* book );
int onAccept_DB( void* data, BOOK* book );
int onPrevious_MainMenu_DB( void* data, BOOK* book );
int onCancel_MainMenu_DB( void* data, BOOK* book );
int CreateMainMenu( void* data, BOOK* book );

void Shortcuts( BOOK* book, GUI* );
int elem_filter(void* elem);
void elem_free(void* elem);

#endif
