#ifndef _LIB_CLARA_H
#define _LIB_CLARA_H

extern void *ELF_BEGIN;

#ifdef LIBCLARANS
namespace libclara{
#endif

#include "..\\include\Types.h"

#ifdef USEPNG
#include "png.h"
#endif

#ifdef USEZLIB
#include "zlib.h"
#endif

#include "..\\include\DYN_ext.h"
#include "..\\include\DYN_tag.h"
#include "..\\include\Colors.h"

#include "..\include\obsolete.h"

extern int *SYNC;
extern int *ASYNC;

#ifdef __cplusplus
extern "C" void kill_data( void* p, void (*func_p)( void* ) );
#else
extern void kill_data( void* p, void (*func_p)( void* ) );
#endif


#pragma diag_suppress=Ta035
#pragma diag_suppress=Ta036
#pragma diag_suppress=Ta031

#pragma swi_number=0x100
__swi __arm void IMB ( void );


#pragma swi_number=0x103
__swi __arm void* malloc( int size );

#pragma swi_number=0x104
__swi __arm void mfree( void* p );

#pragma swi_number=0x8104
__swi __arm void* mfree_adr( void );

#pragma swi_number=0x105
__swi __arm wchar_t* GetDir( int DirIndex );

#pragma swi_number=0x106
__swi __arm int fopen( const wchar_t* fname, int mode, int rights );

#pragma swi_number=0x107
__swi __arm int ModifyKeyHook( KEYHOOKPROC proc, int mode, LPARAM lparam );

#pragma swi_number=0x108
__swi __arm void SUBPROC( void*, ... );


#ifdef __cplusplus
#pragma swi_number=0x108
__swi __arm void SUBPROC( void (*PROC)( void ) );

#pragma swi_number=0x108
__swi __arm void SUBPROC( void (*PROC)( int ), int p1 );

#pragma swi_number=0x108
__swi __arm void SUBPROC( void (*PROC)( int, void* ), int p1, void* p2 );
#endif

#pragma swi_number=0x109
__swi __arm void MMIPROC( void (*PROC)( void ) );

#ifdef __cplusplus
#pragma swi_number=0x109
__swi __arm void MMIPROC( void (*PROC)( int ), int p1 );
#pragma swi_number=0x109
__swi __arm void MMIPROC( void (*PROC)( int, void* ), int p1, void* p2 );
#endif

#pragma swi_number=0x10D
__swi __arm int elfload( const wchar_t* filename, void* param1, void* param2, void* param3 );

#pragma swi_number=0x10E
__swi __arm void* LoadDLL( wchar_t* DllName );

#pragma swi_number=0x10F
__swi __arm int UnLoadDLL( void* DllData );

#pragma swi_number=0x110
__swi __arm int ModifyUIPageHook( int event, PAGEHOOKPROC proc, LPARAM ClientData, int mode );
//-------------------------------------------------------------------------------------------

#pragma swi_number=0x112
__swi __arm void* memset( void* mem, char chr, int size );

#pragma swi_number=0x113
__swi __arm int memcpy( void* dest, const void* source, int cnt );

#pragma swi_number=0x114
__swi __arm int sprintf( char* buf, const char* fmt, ... );

#pragma swi_number=0x115
__swi __arm int snwprintf( wchar_t* buffer, int size, const wchar_t* fmt, ... );

#pragma swi_number=0x116
__swi __arm int _fopen( const wchar_t* filpath, const wchar_t* filname, unsigned int mode, unsigned int rights, FILELISTITEM* destfname);

#pragma swi_number=0x117
__swi __arm int fclose( int file );

#pragma swi_number=0x118
__swi __arm int fread( int file, void* ptr, int size );

#pragma swi_number=0x119
__swi __arm int fwrite( int file, const void* ptr, int size );


#pragma swi_number=0x11C
__swi __arm int fstat( const wchar_t* path, const wchar_t* fname, FSTAT* fstat_stuct );

#pragma swi_number=0x11D
__swi __arm DIR_HANDLE* AllocDirHandle( const wchar_t* path );

#pragma swi_number=0x11E
__swi __arm FILELISTITEM* GetFname( DIR_HANDLE*, FILELISTITEM* );

#pragma swi_number=0x11F
__swi __arm void* DataBrowserDesc_Create( void );

#pragma swi_number=0x120
__swi __arm void DataBrowserDesc_SetItemStyle( void* DataBrowserDesc, int style );

#pragma swi_number=0x121
__swi __arm void DataBrowserDesc_SetHeaderText( void* DataBrowserDesc, TEXTID );

#pragma swi_number=0x122
__swi __arm void DataBrowserDesc_SetFolders( void* DataBrowserDesc, const wchar_t** FolderList );

#pragma swi_number=0x123
__swi __arm void DataBrowserDesc_SetOKSoftKeyText( void* DataBrowserDesc, TEXTID );

#pragma swi_number=0x124
__swi __arm void DataBrowser_Create( void* DataBrowserDesc );

#pragma swi_number=0x125
__swi __arm void DataBrowserDesc_Destroy( void* DataBrowserDesc );

#pragma swi_number=0x126
__swi __arm wchar_t* getFileExtention( wchar_t* fname );

#pragma swi_number=0x127
__swi __arm int DataBrowser_isFileInListExt( const wchar_t* ext_table, const wchar_t* path, const wchar_t* fname );
#pragma swi_number=0x8127
__swi __arm DB_FILE_FILTER DataBrowser_isFileInListExt_adr( void );

#pragma swi_number=0x128
__swi __arm void Timer_ReSet( u16* timer, int time, TIMERPROC onTimer, LPARAM lparam );
#pragma swi_number=0x129
__swi __arm u16 Timer_Set( int time, TIMERPROC onTimer, LPARAM lparam );
#pragma swi_number=0x12A
__swi __arm void Timer_Kill( u16* timerID );

#pragma swi_number=0x12B
__swi __arm int CreateBook( BOOK* pbook, void (*onClose)( BOOK* ), const PAGE_DESC* bp, const char* name, int ParentBookID, const APP_DESC* appdesc );

#pragma swi_number=0x12C
__swi __arm void BookObj_KillBook( BOOK* book );

#pragma swi_number=0x12D
__swi __arm void BookObj_GotoPage( BOOK* book, const PAGE_DESC* page );

#pragma swi_number=0x12E
__swi __arm void BookObj_ReturnPage( BOOK* book, int );

#pragma swi_number=0x12F
__swi __arm void FreeBook( BOOK* book );


#pragma swi_number=0x130
__swi __arm BOOK* FindBook( IS_NEEDED_BOOK );

#pragma swi_number=0x131
__swi __arm BOOK* Find_StandbyBook( void );

#pragma swi_number=0x132
__swi __arm void BookObj_SetFocus( BOOK* book, int display );

#pragma swi_number=0x133
__swi __arm void GUIObject_SetTitleText( GUI*, TEXTID );

#pragma swi_number=0x134
__swi __arm void GUIObject_Show( GUI* );

#pragma swi_number=0x135
__swi __arm void ListMenu_SetCursorToItem( GUI_LIST*, int item );

#pragma swi_number=0x136
__swi __arm void GUIObject_SetStyle( GUI*, int style );

#pragma swi_number=0x137
__swi __arm int GUIonMessage_GetCreatedItemIndex( GUI_MESSAGE* msg );

#pragma swi_number=0x138
__swi __arm char GUIonMessage_SetMenuItemText( GUI_MESSAGE* msg, TEXTID );

#pragma swi_number=0x139
__swi __arm char GUIonMessage_SetMenuItemSecondLineText( GUI_MESSAGE* msg, TEXTID );

#pragma swi_number=0x13A
__swi __arm char GUIonMessage_SetMenuItemInfoText( GUI_MESSAGE* msg, TEXTID );

#pragma swi_number=0x13B
__swi __arm char GUIonMessage_SetMenuItemUnavailableText( GUI_MESSAGE* msg, TEXTID );

#pragma swi_number=0x13C
__swi __arm void GUIonMessage_SetMenuItemIcon( GUI_MESSAGE* msg, int align, IMAGEID );

#pragma swi_number=0x13D
__swi __arm void DispObject_SetTitleText( DISP_OBJ*, TEXTID );

#pragma swi_number=0x13E
__swi __arm int ListMenu_GetSelectedItem( GUI_LIST* );

#pragma swi_number=0x13F
__swi __arm void ListMenu_SetItemStyle( GUI_LIST*, int style );

#pragma swi_number=0x140
__swi __arm void GC_PutChar( GC* gc, int x, int y, int _zero, int zero1, wchar_t wchar );

#pragma swi_number=0x141
__swi __arm int IsScreenSaverBook( BOOK* bk );
#pragma swi_number=0x8141
__swi __arm IS_NEEDED_BOOK get_IsScreenSaverBook( void );

#pragma swi_number=0x142
__swi __arm void StatusIndication_SetItemText( GUI*, int item, TEXTID );

#pragma swi_number=0x143
__swi __arm void StatusIndication_ShowNotes( TEXTID );

#pragma swi_number=0x144
__swi __arm GUI* CreateStringInputVA( int, ... );

#pragma swi_number=0x145
__swi __arm GUI_ONEOFMANY* CreateOneOfMany( BOOK* book );

#pragma swi_number=0x146
__swi __arm void OneOfMany_SetItemCount( GUI_ONEOFMANY*, int count );

#pragma swi_number=0x147
__swi __arm void OneOfMany_SetChecked( GUI_ONEOFMANY*, int checked );

#pragma swi_number=0x148
__swi __arm void OneOfMany_SetTexts( GUI_ONEOFMANY*, TEXTID* strids, int Count );

#pragma swi_number=0x149
__swi __arm int OneOfMany_GetSelected( GUI_ONEOFMANY* );

#pragma swi_number=0x14A
__swi __arm void StatusIndication_Item8_SetText( TEXTID );

#pragma swi_number=0x14B
__swi __arm void GUIObject_SoftKeys_SetAction( GUI*, u16 actionID, SKACTIONPROC proc );

#pragma swi_number=0x14C
__swi __arm void GUIObject_SoftKeys_SetText( GUI*, u16 actionID, TEXTID );

#pragma swi_number=0x14D
__swi __arm void GUIObject_SoftKeys_SetEnable( GUI*, u16 actionID, BOOL );

#pragma swi_number=0x14E
__swi __arm void GUIObject_SoftKeys_AddErrorStr( GUI*, u16 actionID, TEXTID );

#pragma swi_number=0x14F
__swi __arm void GUIObject_SoftKeys_RemoveItem( GUI*, u16 actionID );

#pragma swi_number=0x150
__swi __arm void GUIObject_SoftKeys_SetVisible( GUI*, u16 actionID, BOOL );

#pragma swi_number=0x151
__swi __arm void GUIObject_SoftKeys_SuppressDefaultAction( GUI*, u16 actionID );

#pragma swi_number=0x152
__swi __arm wchar_t* wstrcpy( wchar_t* dest, const wchar_t* source );

#pragma swi_number=0x153
__swi __arm wchar_t* wstrncpy( wchar_t* dest, const wchar_t* source, int maxlen );

#pragma swi_number=0x154
__swi __arm wchar_t* wstrcat( wchar_t* wstr, const wchar_t* subwstr );

#pragma swi_number=0x155
__swi __arm wchar_t* wstrncat( wchar_t* wstr, const wchar_t* subwstr, int maxlen );

#pragma swi_number=0x156
__swi __arm int wstrcmp( const wchar_t* wstr1, const wchar_t* wstr2 );

#pragma swi_number=0x157
__swi __arm int wstrlen( const wchar_t* wstr );

#pragma swi_number=0x158
__swi __arm wchar_t* str2wstr( wchar_t* wstr, const char* str );

#pragma swi_number=0x159
__swi __arm int strcmp( const char* str1, const char* str2 );

#pragma swi_number=0x15A
__swi __arm int strlen( const char* str );

#pragma swi_number=0x15B
__swi __arm char* wstr2strn( char* str, const wchar_t* wstr, int maxlen );

#pragma swi_number=0x15C
__swi __arm TEXTID TextID_CreateIntegerID( int num );

#pragma swi_number=0x15D
__swi __arm TEXTID TextID_Create( const void* wstr, TEXT_ENCODING flag, int len );

#pragma swi_number=0x15E
__swi __arm void TextID_GetString( TEXTID, char* str, int maxlen );

#pragma swi_number=0x15F
__swi __arm int TextID_GetWString( TEXTID, wchar_t* dest, int maxlen );

#pragma swi_number=0x160
__swi __arm int TextID_GetLength( TEXTID );

#pragma swi_number=0x161
__swi __arm void TextID_Destroy( TEXTID );

#pragma swi_number=0x162
__swi __arm int AB_DEFAULTNBR_GET( int rec_num, void* mem_0x30, int* unk );

#pragma swi_number=0x163
__swi __arm int AB_READSTRING( AB_STR_ITEM*, int rec_num, int field_ID );

#pragma swi_number=0x164
__swi __arm int AB_READPHONENBR( AB_NUM_ITEM*, int rec_num, int field_ID );

#pragma swi_number=0x165
__swi __arm int AB_GETNBROFITEMS( int get_from, int sub_id );

#pragma swi_number=0x166
__swi __arm int PNUM_len( BCD_TEXT pnum );

#pragma swi_number=0x167
__swi __arm void PNUM2str( char* str, BCD_TEXT pnum, int pnum_len_to_convert, int str_buf_size );

#pragma swi_number=0x168
__swi __arm DISP_OBJ* GUIObject_GetDispObject( GUI* );

#pragma swi_number=0x169
__swi __arm LIST* List_Create( void );

#pragma swi_number=0x16A
__swi __arm void List_Destroy( LIST* lst );

#pragma swi_number=0x16B
__swi __arm int List_InsertFirst( LIST* lst, void* item );

#pragma swi_number=0x16C
__swi __arm int Gif2ID( u16 IMAGEHANDLE, const wchar_t* path, const wchar_t* fname, IMAGEID* );

#pragma swi_number=0x16D
__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE( const int* sync, u16* ImageHandler, char* unk );

#pragma swi_number=0x16E
__swi __arm void REQUEST_DATEANDTIME_GET( const int* sync, DATETIME* dt );

#pragma swi_number=0x16F
__swi __arm void IndicationDevice_Backlight_FadeToLevel( int unk_zero, int bl_level );

#pragma swi_number=0x170
__swi __arm int GetFreeBytesOnHeap( void );

#pragma swi_number=0x171
__swi __arm void BookObj_Hide( BOOK* book, int display );

#pragma swi_number=0x172
__swi __arm void BookObj_Show( BOOK* book, int display );

#pragma swi_number=0x173
__swi __arm void StartAPP( const wchar_t* appname );

#pragma swi_number=0x174
__swi __arm void ListMenu_SetOnMessage( GUI_LIST*, int (*proc)( GUI_MESSAGE* ) );

#pragma swi_number=0x175
__swi __arm char* manifest_GetParam( const char* buf, const char* param_name, int unk );

#pragma swi_number=0x176
__swi __arm int lseek( int file, int offset, int mode );

#pragma swi_number=0x8177
__swi __arm int get_VBUFFER( void );

#pragma swi_number=0x178
__swi __arm void SetLampLevel( int level );

#pragma swi_number=0x179
__swi __arm void* List_RemoveAt( LIST* lst, int index );

#pragma swi_number=0x17A
__swi __arm PROCESS GetCurrentPID( void );

#pragma swi_number=0x17B
__swi __arm int List_IndexOf( LIST* lst, void* item );


#pragma swi_number=0x17C
__swi __arm union SIGNAL* alloc( OSBUFSIZE size, SIGSELECT signo );
#pragma swi_number=0x17D
__swi __arm union SIGNAL* receive( const SIGSELECT* sigsel );
#pragma swi_number=0x17E
__swi __arm void send( union SIGNAL** sig, PROCESS to );
#pragma swi_number=0x17F
__swi __arm PROCESS sender( union SIGNAL** sig );
#pragma swi_number=0x180
__swi __arm void free_buf( union SIGNAL** sig );
#pragma swi_number=0x181
__swi __arm PROCESS create_process( PROCESS_TYPE proc_type, char* name, OSENTRYPOINT* entrypoint, OSADDRESS stack_size, OSPRIORITY priority, OSTIME timeslice, PROCESS pid_block, void* redir_table, OSVECTOR vector, OSUSER user );
#pragma swi_number=0x182
__swi __arm void start( PROCESS pid );
#pragma swi_number=0x183
__swi __arm int get_ptype( PROCESS pid );
#pragma swi_number=0x184
__swi __arm PROCESS current_process( void );
#pragma swi_number=0x185
__swi __arm void delay( OSTIME timeout );
#pragma swi_number=0x186
__swi __arm OSBOOLEAN hunt( const char* name, OSUSER user, PROCESS* name_, union SIGNAL** hunt_sig );
#pragma swi_number=0x187
__swi __arm void kill_proc( PROCESS pid );

#pragma swi_number=0x189
__swi __arm union SIGNAL* receive_w_tmo( OSTIME timeout, SIGSELECT* sel );

#pragma swi_number=0x18B
__swi __arm void stop( PROCESS pid );
#pragma swi_number=0x18C
__swi __arm OSBOOLEAN get_mem( PROCESS pid, OSADDRESS from, void* to, OSADDRESS size );
#pragma swi_number=0x18D
__swi __arm OSADDRESS get_envp( PROCESS pid, char* name );
#pragma swi_number=0x18E
__swi __arm OSBOOLEAN set_envp( PROCESS pid, char* name, OSADDRESS value );
#pragma swi_number=0x18F
__swi __arm PROCESS get_bid( PROCESS pid );

#pragma swi_number=0x190
__swi __arm int datetime2unixtime( DATETIME* dt );

#pragma swi_number=0x1A0
__swi __arm char* strcpy( char* dest, const char* source );


#pragma swi_number=0x1A6
__swi __arm char* UIEventName( int event );

#pragma swi_number=0x81A7
__swi __arm char* MissedEvents( void );
#pragma swi_number=0x1A8
__swi __arm void UI_Event( int event );
#pragma swi_number=0x1A9
__swi __arm void UI_Event_wData( int event, void* message, void (*free_proc)( void* ) );
#pragma swi_number=0x1AA
__swi __arm void UI_Event_toBookID( int event, int BookID );
#pragma swi_number=0x1AB
__swi __arm void UI_Event_toBookIDwData( int event, int BookID, void* message, void (*free_proc)( void* ) );

#pragma swi_number=0x1AC
__swi __arm int List_Find( LIST* lst, void* itemtofind, LISTFINDCALLBACK cmp_proc );

#pragma swi_number=0x1AD
__swi __arm void* List_Get( LIST* lst, int index );
#pragma swi_number=0x1AE
__swi __arm wchar_t* wstrrchr( const wchar_t* wstr, wchar_t wchar );
#pragma swi_number=0x1AF
__swi __arm void BookObj_CallSubroutine( BOOK* book, BOOK_SUBROUTINE* subr );
#pragma swi_number=0x1B0
__swi __arm int List_InsertLast( LIST* lst, void* item );

#pragma swi_number=0x1B1
__swi __arm void debug_printf( const char* fmt, ... );

#pragma swi_number=0x1B2
__swi __arm int PlayFile( const wchar_t* path, const wchar_t* fname );

#pragma swi_number=0x1B4
__swi __arm int REQUEST_PROFILE_GETACTIVEPROFILE( const int* sync, int* level );

#pragma swi_number=0x1B5
__swi __arm int Profile_SetActive( int NUMprof, int set_in_gdfs_flag );

#pragma swi_number=0x1B7
__swi __arm int isKeylocked( void );

#pragma swi_number=0x1B8
__swi __arm int IsMediaPlayerVideoBook( BOOK* bk );
#pragma swi_number=0x81B8
__swi __arm IS_NEEDED_BOOK get_IsMediaPlayerVideoBook( void );

#pragma swi_number=0x1BC
__swi __arm int REQUEST_DATEFORMAT_GET( const int* sync, char* DateFormat );

#pragma swi_number=0x1BD
__swi __arm int REQUEST_TIMEFORMAT_GET( const int* sync, char* TimeFormat );

#pragma swi_number=0x1BE
__swi __arm TEXTID Date2ID( DATE*, int DateFormat, int );

#pragma swi_number=0x1BF
__swi __arm TEXTID Time2ID( TIME*, char TimeFormat, int isSec );

#pragma swi_number=0x1C0
__swi __arm GUI_LIST* CreateListMenu( BOOK*, int display );
#pragma swi_number=0x1C1
__swi __arm void ListMenu_SetItemCount( GUI_LIST*, int item_count );

#pragma swi_number=0x81C2
__swi __arm LIST** ROOT_APP( void );
#pragma swi_number=0x1C3
__swi __arm void GUIObject_SoftKeys_SetInfoText( GUI*, u16 actionID, TEXTID );
#pragma swi_number=0x1C4
__swi __arm void GUIObject_SoftKeys_SetItemAsSubItem( GUI*, u16 actionID, int subitem );
#pragma swi_number=0x1C5
__swi __arm int REQUEST_SYSTEM_SHUTDOWN( void );
#pragma swi_number=0x1C6
__swi __arm int REQUEST_SYSTEM_RESTART( void );
#pragma swi_number=0x1C7
__swi __arm void GUIObject_SoftKeys_SetTexts( GUI*, u16 actionID, TEXTID short_text, TEXTID full_text );

#pragma swi_number=0x1C8
__swi __arm int IsRightNowBook( BOOK* book );
#pragma swi_number=0x81C8
__swi __arm IS_NEEDED_BOOK get_IsRightNowBook( void );

#pragma swi_number=0x1C9
__swi __arm int IsVolumeControllerBook( BOOK* book );
#pragma swi_number=0x81C9
__swi __arm IS_NEEDED_BOOK get_IsVolumeControllerBook( void );

#pragma swi_number=0x1CA
__swi __arm GUI_TABMENUBAR* CreateTabMenuBar( BOOK* book );
#pragma swi_number=0x1CB
__swi __arm void TabMenuBar_SetTabCount( GUI_TABMENUBAR*, int count );
#pragma swi_number=0x1CC
__swi __arm void TabMenuBar_SetTabGui( GUI_TABMENUBAR*, int tab, GUI* );
#pragma swi_number=0x1CD
__swi __arm void TabMenuBar_SetTabIcon( GUI_TABMENUBAR*, int tab, IMAGEID, int for_state );
#pragma swi_number=0x1CE
__swi __arm GUI* GUIObject_Destroy( GUI* );
#pragma swi_number=0x1CF
__swi __arm void GUIInput_SetIcon( GUI*, IMAGEID );

#pragma swi_number=0x1D0
__swi __arm int StringInput_GetStringAndLen( GUI*, wchar_t**, u16* );

#pragma swi_number=0x81D1
__swi __arm PAudioControl* GetAudioControlPtr( void );
#pragma swi_number=0x1D2
__swi __arm int AudioControl_Vibrate( PAudioControl pIAudioControl, int vibratime, int pausetime, int totaltime );

#pragma swi_number=0x1D3
__swi __arm int GetVolumeSize( wchar_t* root_folder, VOLUMESIZE* );

#pragma swi_number=0x1D4
__swi __arm GUI_NOFMANY* CreateNOfMany( BOOK* book );

#pragma swi_number=0x1D5
__swi __arm void NOfMany_SetTexts( GUI_NOFMANY*, TEXTID* strids, int items_count );

#pragma swi_number=0x1D6
__swi __arm void NOfMany_SetChecked( GUI_NOFMANY*, u16* checked_table, int items_count );

#pragma swi_number=0x1D7
__swi __arm void NOfMany_SetCursor( GUI_NOFMANY*, int item );

#pragma swi_number=0x1D8
__swi __arm int NOfMany_GetCheckedCount( GUI_NOFMANY* );

#pragma swi_number=0x1D9
__swi __arm int NOfMany_GetChecked( GUI_NOFMANY*, u16* buffer );

#pragma swi_number=0x1DA
__swi __arm void NOfMany_SetOnMessage( GUI_NOFMANY*, int (*proc)( GUI_MESSAGE* ) );

#pragma swi_number=0x1DF
__swi __arm PAudioControl AudioControl_Init( void );

#pragma swi_number=0x1E0
__swi __arm void OneOfMany_SetFocused( GUI_ONEOFMANY*, int item );
#pragma swi_number=0x1E1
__swi __arm void OneOfMany_SetOnMessage( GUI_ONEOFMANY*, int (*proc)( GUI_MESSAGE* ) );



#pragma swi_number=0x1E2
__swi __arm GUI_FEEDBACK* CreateMonitorFeedback( TEXTID, BOOK*, void (*onbusy)(BOOK*), void (*onedit)(BOOK*), void (*ondelete)(BOOK*) );
#pragma swi_number=0x1E3
__swi __arm void Feedback_SetText( GUI_FEEDBACK*, TEXTID );
#pragma swi_number=0x1E4
__swi __arm int GetBatteryState( void* unused, BATT* );
#pragma swi_number=0x1E5
__swi __arm GC* get_DisplayGC( void );
#pragma swi_number=0x1E6
__swi __arm void DispObject_GetRect( DISP_OBJ*, RECT* );
#pragma swi_number=0x1E7
__swi __arm void GC_GetRect( GC* gc, RECT* );
#pragma swi_number=0x1E8
__swi __arm int GC_GetXX( GC* gc );
#pragma swi_number=0x1E9
__swi __arm void GC_SetXX( GC* gc, int );
#pragma swi_number=0x1EA
__swi __arm void GC_ValidateRect( GC* gc, RECT* );
#pragma swi_number=0x1EB
__swi __arm void BookObj_AddGUIObject( BOOK* book, GUI* );
#pragma swi_number=0x1EC
__swi __arm void DrawRect( int x1, int y1, int x2, int y2, int pen_color, int brush_color );
#pragma swi_number=0x1ED
__swi __arm void DrawString( TEXTID, int align, int x1, int y1, int x2, int y2, int unk, int unk1, int pen_color, int brush_color );
#pragma swi_number=0x1EE
__swi __arm void XGUIList_AddGUIObject( XGUILIST* xguilist, GUI* );
#pragma swi_number=0x1EF
__swi __arm void DispObject_InvalidateRect( DISP_OBJ*, RECT* );
#pragma swi_number=0x1F0
__swi __arm int GUIObject_Create( GUI*, void (*GuiDestroy)( GUI* ), void (*DispDescCreate)( DISP_DESC* ), BOOK*, void (*DispObjCallBack)( DISP_OBJ*, void* msg, GUI* ), int display, int size_of_gui );
#pragma swi_number=0x1F1
__swi __arm int SetFont( int );
#pragma swi_number=0x81F2
__swi __arm DISP_OBJ** StatusRow_p( void );

#pragma swi_number=0x1F3
__swi __arm int root_list_get_session_count( void );
#pragma swi_number=0x1F4
__swi __arm UI_APP_SESSION* root_list_get_session( int num_session );
#pragma swi_number=0x1F5
__swi __arm BOOK* SESSION_GetTopBook( UI_APP_SESSION* );

#pragma swi_number=0x1F6
__swi __arm int MainInput_getVisible( GUI* );
#pragma swi_number=0x1F7
__swi __arm int MainInput_strlen( GUI* );
#pragma swi_number=0x1F8
__swi __arm BCD_TEXT MainInput_getPNUM( GUI* );
#pragma swi_number=0x1F9
__swi __arm int MainInput_isPlus( GUI* );

#pragma swi_number=0x1FA
__swi __arm int BookObj_GetBookID( BOOK* book );
#pragma swi_number=0x1FB
__swi __arm int wstrncmp( const wchar_t*, const wchar_t*, int );
#pragma swi_number=0x1FC
__swi __arm int DispObject_GetAbsoluteXPos( DISP_OBJ* );
#pragma swi_number=0x1FD
__swi __arm int DispObject_GetAbsoluteYPos( DISP_OBJ* );

#pragma swi_number=0x1FF
__swi __arm int GetThemeColor( int, int );
#pragma swi_number=0x200
__swi __arm int REQUEST_SETTING_SILENCE_SET( const int* sync, u16 profile, u16 value );
#pragma swi_number=0x201
__swi __arm int REQUEST_SETTING_SILENCE_GET( const int* sync, u16 profile, char* silence_mode );

#pragma swi_number=0x202
__swi __arm void VCALL_Init( void* vc );
#pragma swi_number=0x203
__swi __arm void VCALL_SetName( void* vc, wchar_t* name, unsigned short name_len );
#pragma swi_number=0x204
__swi __arm void VCALL_SetNameIcon( void* vc, IMAGEID );
#pragma swi_number=0x205
__swi __arm void VCALL_SetNumber( void* vc, wchar_t* number, unsigned short num_len );
#pragma swi_number=0x206
__swi __arm void VCALL_SetHZ1( void* vc, int, u16 );
#pragma swi_number=0x207
__swi __arm void VCALL_SetHZ2( void* vc, u16 );
#pragma swi_number=0x208
__swi __arm void MakeVoiceCall( int SessioID, void* vc, int flag );

#pragma swi_number=0x20A
__swi __arm void FileDelete( wchar_t* path, wchar_t* filename, int* error );
#pragma swi_number=0x20B
__swi __arm void GUIObject_SetFocus( GUI*, int );

#pragma swi_number=0x20C
__swi __arm int MSG_SendMessage_CreateMessage( int, void* );
#pragma swi_number=0x20D
__swi __arm int MSG_SendMessage_DestroyMessage( void* );
#pragma swi_number=0x20E
__swi __arm int MSG_SendMessage_AddRecipient( void*, wchar_t*, wchar_t*, int, int );
#pragma swi_number=0x20F
__swi __arm int MSG_SendMessage_Start( int, void*, int );
#pragma swi_number=0x8210
__swi __arm void* get_APP_DESC_TABLE( void );

#pragma swi_number=0x212
__swi __arm int AB_POSITIONNBR_GET( void* ab_name, char*, u16*, void* ab_num );
#pragma swi_number=0x213
__swi __arm TEXTID AB_NAME_ITEM2TextID( AB_STR_ITEM* ab_name );
#pragma swi_number=0x214
__swi __arm TEXTID AB_NUM_ITEM2TextID( AB_NUM_ITEM* ab_num_item );
#pragma swi_number=0x215
__swi __arm int GetABRecNum( AB_GET_STRUCT* );
#pragma swi_number=0x8216
__swi __arm AB_ITEM_DESC* get_AB_ITEMS_DESC( void );


#pragma swi_number=0x217
__swi __arm GUI* SBY_GetMainInput( BOOK* StandBy ); // !!!!! не настоящий !!!!!!
#pragma swi_number=0x218
__swi __arm GUI* SBY_GetStatusIndication( BOOK* StandBy ); // !!!!! не настоящий !!!!!!

#pragma swi_number=0x219
__swi __arm int REQUEST_PHONEBOOK_ACCESSSTATUS_TOTAL_GET( const int* sync, int* );

#pragma swi_number=0x821A
__swi __arm PROCESS* PID_MMI( void );

#pragma swi_number=0x21C
__swi __arm GUI* DispObject_GetGUI( DISP_OBJ* );

#pragma swi_number=0x21D
__swi __arm DISP_OBJ* Display_GetFocusedDispObject( int display );
#pragma swi_number=0x21E
__swi __arm char DispObject_GetVisible( DISP_OBJ* );

#pragma swi_number=0x21F
__swi __arm DISP_DESC* DispObject_GetDESC( DISP_OBJ* ); // !!!!! не настоящий !!!!!!

#pragma swi_number=0x220
__swi __arm void DISP_DESC_SetName( DISP_DESC*, const char* name );
#pragma swi_number=0x221
__swi __arm char* DispObject_GetName( DISP_OBJ* );
#pragma swi_number=0x222
__swi __arm void DISP_DESC_SetSize( DISP_DESC*, u16 size );
#pragma swi_number=0x223
__swi __arm u16 DISP_DESC_GetSize( DISP_OBJ* );
#pragma swi_number=0x224
__swi __arm void DISP_DESC_SetOnCreate( DISP_DESC*, DISP_OBJ_ONCREATE_METHOD );
#pragma swi_number=0x225
__swi __arm DISP_OBJ_ONCREATE_METHOD DispObject_GetOnCreate( DISP_OBJ* );
#pragma swi_number=0x226
__swi __arm void DISP_DESC_SetOnClose( DISP_DESC*, DISP_OBJ_ONCLOSE_METHOD );
#pragma swi_number=0x227
__swi __arm DISP_OBJ_ONCLOSE_METHOD DispObject_GetOnClose( DISP_OBJ* );
#pragma swi_number=0x228
__swi __arm void DISP_DESC_SetOnRedraw( DISP_DESC*, DISP_OBJ_ONREDRAW_METHOD );
#pragma swi_number=0x229
__swi __arm DISP_OBJ_ONREDRAW_METHOD DispObject_GetOnRedraw( DISP_OBJ* );
#pragma swi_number=0x22A
__swi __arm void DISP_DESC_SetOnKey( DISP_DESC*, DISP_OBJ_ONKEY_METHOD );
#pragma swi_number=0x22B
__swi __arm DISP_OBJ_ONKEY_METHOD DispObject_GetOnKey( DISP_OBJ* );



#pragma swi_number=0x22C
__swi __arm void DISP_DESC_SetOnRefresh( DISP_DESC*, DISP_OBJ_METHOD );
#pragma swi_number=0x22D
__swi __arm DISP_OBJ_METHOD DispObject_GetonRefresh( DISP_OBJ* );
#pragma swi_number=0x22E
__swi __arm void DISP_DESC_SetMethod04( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x22F
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod04( DISP_OBJ* );
#pragma swi_number=0x230
__swi __arm void DISP_DESC_SetMethod05( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x231
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod05( DISP_OBJ* );
#pragma swi_number=0x232
__swi __arm void DISP_DESC_SetMethod06( DISP_DESC*, DISP_OBJ_METHOD );
#pragma swi_number=0x233
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod06( DISP_OBJ* );
#pragma swi_number=0x234
__swi __arm void DISP_DESC_SetMethod07( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x235
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod07( DISP_OBJ* );
#pragma swi_number=0x236
__swi __arm void DISP_DESC_SetOnLayout( DISP_DESC*, DISP_OBJ_ONLAYOUT_METHOD );
#pragma swi_number=0x237
__swi __arm DISP_OBJ_ONLAYOUT_METHOD DispObject_GetOnLayout( DISP_OBJ* );
#pragma swi_number=0x238
__swi __arm void DISP_DESC_SetOnConfig( DISP_DESC*, DISP_OBJ_ONCONFIG_METHOD );
#pragma swi_number=0x239
__swi __arm DISP_OBJ_ONCONFIG_METHOD DispObject_GetOnConfig( DISP_OBJ* );
#pragma swi_number=0x23A
__swi __arm void DISP_DESC_SetMethod0A( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x23B
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod0A( DISP_OBJ* );
#pragma swi_number=0x23C
__swi __arm void DISP_DESC_SetMethod0B( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x23D
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod0B( DISP_OBJ* );
#pragma swi_number=0x23E
__swi __arm void DISP_DESC_SetMethod0C( DISP_DESC*, void (*)( void* ) );
#pragma swi_number=0x23F
__swi __arm DISP_OBJ_METHOD DispObject_GetMethod0C( DISP_OBJ* );

#pragma swi_number=0x240
__swi __arm void JavaSession_Manager( int cmd );
#pragma swi_number=0x241
__swi __arm TEXTID JavaSession_GetName( void );
#pragma swi_number=0x242
__swi __arm TEXTID TextID_Copy( TEXTID );
#pragma swi_number=0x243
__swi __arm UI_APP_SESSION* BookObj_GetSession( BOOK* book );
#pragma swi_number=0x244
__swi __arm void GUIonMessage_SetNumberOfSubItems( GUI_MESSAGE* msg, int unk, int num_sub_items );
#pragma swi_number=0x245
__swi __arm void GUIonMessage_SubItem_SetText( GUI_MESSAGE* msg, TEXTID );
#pragma swi_number=0x246
__swi __arm int GUIonMessage_SubItem_GetCreatedParentIndex( GUI_MESSAGE* msg );
#pragma swi_number=0x247
__swi __arm void GoMusic( void );
#pragma swi_number=0x248
__swi __arm int GUIonMessage_SubItem_GetSelectedIndex( GUI_MESSAGE* msg );
#pragma swi_number=0x249
__swi __arm void GUIonMessage_SubItem_SetDisabled( GUI_MESSAGE* msg, BOOL state );

#pragma swi_number=0x24A
__swi __arm int PlayFileV( const wchar_t* path, const wchar_t* fname, int vol );
#pragma swi_number=0x24B
__swi __arm int GetSilent( void );
#pragma swi_number=0x24C
__swi __arm int GetVibrator( int ignorevibrator, int ignoresilent );
#pragma swi_number=0x24D
__swi __arm int GetAudioSettings( int what, char* retvalue );

#pragma swi_number=0x24E
__swi __arm int GetChipID( void );

#pragma swi_number=0x252
__swi __arm int GUIonMessage_SubItem_GetCreatedIndex( GUI_MESSAGE* msg );
#pragma swi_number=0x253
__swi __arm int GUIonMessage_SubItem_GetSelectedParentIndex( GUI_MESSAGE* msg );
#pragma swi_number=0x254
__swi __arm void DestroyDirHandle( DIR_HANDLE* handle );
#pragma swi_number=0x8255
__swi __arm DB_EXT* LastExtDB( void );
#pragma swi_number=0x256
__swi __arm void GetOtherExtMethods( FILESUBROUTINE** );
#pragma swi_number=0x257
__swi __arm int MessageBox( TEXTID header_text, TEXTID message_text, IMAGEID, int style, int time, BOOK* book );
#pragma swi_number=0x258
__swi __arm int Display_GetHeight( int display );
#pragma swi_number=0x259
__swi __arm int Display_GetWidth( int display );
#pragma swi_number=0x25A
__swi __arm void PlaySystemSound( int SndNumber );


#pragma swi_number=0x25D
__swi __arm int TabMenuBar_GetFocusedTab( GUI_TABMENUBAR* );
#pragma swi_number=0x25E
__swi __arm void TabMenuBar_SetFocusedTab( GUI_TABMENUBAR*, int tab_num );
#pragma swi_number=0x25F
__swi __arm int GC_GetPenColor( GC* gc );
#pragma swi_number=0x260
__swi __arm void GC_SetPenColor( GC* gc, int pen_color );

#pragma swi_number=0x261
__swi __arm void GC_SetPixel( GC* gc, int x1, int y1, int color );
#pragma swi_number=0x262
__swi __arm void GC_DrawLine( GC* gc, int x1, int y1, int x2, int y2 );
#pragma swi_number=0x263
__swi __arm void GC_DrawFRect( GC* gc, int color, int x1, int y1, int x2, int y2 );
#pragma swi_number=0x264
__swi __arm int GC_WritePixels( GC* gc, int x1, int y1, int x2, int y2, int* bmp );

#ifdef USEPNG
#pragma swi_number=0x265
__swi __arm png_structp png_create_read_struct_2( png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn );
#pragma swi_number=0x266
__swi __arm void png_destroy_read_struct( png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr );
#pragma swi_number=0x267
__swi __arm png_infop png_create_info_struct( png_structp png_ptr );
#pragma swi_number=0x268
__swi __arm void png_destroy_info_struct( png_structp png_ptr, png_infopp info_ptr_ptr );
#pragma swi_number=0x269
__swi __arm void png_set_progressive_read_fn( png_structp png_ptr, png_voidp progressive_ptr, png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn, png_progressive_end_ptr end_fn );
#pragma swi_number=0x26A
__swi __arm void png_process_data( png_structp png_ptr, png_infop info_ptr, png_bytep buffer, png_size_t buffer_size );
#pragma swi_number=0x26B
__swi __arm void png_read_update_info( png_structp png_ptr, png_infop info_ptr );
#pragma swi_number=0x26C
__swi __arm png_voidp png_get_progressive_ptr( png_structp png_ptr );
#pragma swi_number=0x26D
__swi __arm png_uint_32 png_get_IHDR( png_structp png_ptr, png_infop info_ptr, png_uint_32* width, png_uint_32* height, int* bit_depth, int* color_type, int* interlace_method, int* compression_method, int* filter_method );
#pragma swi_number=0x26E
__swi __arm void png_set_strip_16( png_structp png_ptr );
#pragma swi_number=0x26F
__swi __arm void png_set_gray_1_2_4_to_8( png_structp png_ptr );
#pragma swi_number=0x270
__swi __arm void png_set_gray_to_rgb( png_structp png_ptr );
#pragma swi_number=0x271
__swi __arm png_uint_32 png_get_valid( png_structp png_ptr, png_infop info_ptr, png_uint_32 flag );
#pragma swi_number=0x272
__swi __arm void png_set_tRNS_to_alpha( png_structp png_ptr );
#pragma swi_number=0x273
__swi __arm void png_set_filler( png_structp png_ptr, png_uint_32 filler, int flags );
#pragma swi_number=0x274
__swi __arm void png_set_palette_to_rgb( png_structp png_ptr );
#pragma swi_number=0x275
__swi __arm png_uint_32 png_get_rowbytes( png_structp png_ptr, png_infop info_ptr );
#pragma swi_number=0x276
__swi __arm void png_progressive_combine_row( png_structp png_ptr, png_bytep old_row, png_bytep new_row );
#pragma swi_number=0x277
__swi __arm int png_sig_cmp( png_bytep sig, png_size_t start, png_size_t num_to_check );
#endif

#pragma swi_number=0x278
__swi __arm int setjmp( jmp_buf jmpbuf );
#pragma swi_number=0x279
__swi __arm void longjmp( jmp_buf, int );

#ifdef USEZLIB
#pragma swi_number=0x27A
__swi __arm int inflateInit2_( z_streamp strm, int windowBits, const char* version, int stream_size );
#pragma swi_number=0x27B
__swi __arm int inflate( z_streamp strm, int flush );
#pragma swi_number=0x27C
__swi __arm int inflateEnd( z_streamp strm );
#endif

#pragma swi_number=0x27D
__swi __arm int memcmp( const void* m1, const void* m2, int n );
#pragma swi_number=0x27E
__swi __arm int strncmp( const char* s1, const char* s2, int len );
#pragma swi_number=0x27F
__swi __arm char* strncpy( char* dest, const char* source, int maxlen );
#pragma swi_number=0x280
__swi __arm char* strcat( char* dest, const char* src );
#pragma swi_number=0x281
__swi __arm void GUIObject_SetTitleType( GUI*, int type );

#pragma swi_number=0x282
__swi __arm void GUIonMessage_SetItemDisabled( GUI_MESSAGE* msg, BOOL );
#pragma swi_number=0x283
__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_REGISTER( const int* sync, u16 ImageHandler, wchar_t* path, wchar_t* fname, int unk, IMAGEID*, char* error );
#pragma swi_number=0x284
__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER( const int* sync, u16 ImageHandler, u16*, u16*, IMAGEID, int unk_1, char* error );
#pragma swi_number=0x8285
__swi __arm SURFACE** get_Surfaces( void );

#pragma swi_number=0x286
__swi __arm int iconidname2id( const wchar_t* idname, int maxnamelen, int* id );
#pragma swi_number=0x287
__swi __arm int textidname2id( const wchar_t* idname, int maxnamelen, TEXTID* id );

#pragma swi_number=0x288
__swi __arm void ListMenu_SetNoItemText( GUI_LIST*, TEXTID str );

#pragma swi_number=0x28A
__swi __arm int IsFmRadioBook( BOOK* bk );
#pragma swi_number=0x828A
__swi __arm IS_NEEDED_BOOK get_IsFmRadioBook( void );

#pragma swi_number=0x28B
__swi __arm int IsAudioPlayerBook( BOOK* bk);
#pragma swi_number=0x828B
__swi __arm IS_NEEDED_BOOK get_IsAudioPlayerBook( void );

#pragma swi_number=0x28C
__swi __arm void PlayerControl( BOOK* AudioPlayerBook, int );
#pragma swi_number=0x28D
__swi __arm void SwitchRadioStationFromList( BOOK* FmRadioBook, int );

#pragma swi_number=0x28E
__swi __arm void Shortcut_Run( wchar_t* shortcut_name );

#pragma swi_number=0x28F
__swi __arm int mkdir( const wchar_t* path );
#pragma swi_number=0x290
__swi __arm int rmdir( const wchar_t* path, int zero );
#pragma swi_number=0x291
__swi __arm int chmod( const wchar_t* path, const wchar_t* fname, int amode );
#pragma swi_number=0x292
__swi __arm int rename( const wchar_t* oldpath, const wchar_t* oldfname, const wchar_t* newpath, const wchar_t* newfname, int zero );

#pragma swi_number=0x293
__swi __arm int GetImageWidth( IMAGEID );
#pragma swi_number=0x294
__swi __arm int GetImageHeight( IMAGEID );

#pragma swi_number=0x295
__swi __arm void* CallID_GetCallStatusDesc( int CallID );
#pragma swi_number=0x296
__swi __arm wchar_t* CallStatusDesc_GetName( void* CallStatusDesc );
#pragma swi_number=0x297
__swi __arm void List_DestroyElements( LIST* lst, int (*cmp_proc)( void* item ), void (*free_proc)( void* item ) );

#pragma swi_number=0x298
__swi __arm char* strstr( const char* str1, const char* str2 );

#pragma swi_number=0x299
__swi __arm int GPRS_GetLastSessionInfo( int, GPRS_SESSION_INFO* );

#pragma swi_number=0x29A
__swi __arm int wstrcmpni( const wchar_t* wstr1, const wchar_t* wstr2, int len );

#pragma swi_number=0x29B
__swi __arm int wtoi( const wchar_t* wstr, int len, int* dest );

#pragma swi_number=0x29C
__swi __arm void DATE_GetWeekDay( DATE*, char* dest );

#pragma swi_number=0x29D
__swi __arm void Cale_GetSettings( int setID, void* );

#pragma swi_number=0x29E
__swi __arm wchar_t* wstrwstr( const wchar_t* wstr1, const wchar_t* wstr2 );

#pragma swi_number=0x29F
__swi __arm int wstrcmpi( const wchar_t* ws1, const wchar_t* ws2 );

#pragma swi_number=0x2A0
__swi __arm wchar_t* wstrchr( const wchar_t* source, wchar_t chr );

#pragma swi_number=0x2A1
__swi __arm void GUIObject_SoftKeys_Hide( GUI* );

#pragma swi_number=0x2A2
__swi __arm void GUIObject_SoftKeys_Show( GUI* );

#pragma swi_number=0x2A3
__swi __arm DISP_OBJ* DispObject_SoftKeys_Get( void );
#pragma swi_number=0x2A4
__swi __arm int ThemeImage_SetImage( int theme_image_type, int _zero, int __zero, const wchar_t* path, const wchar_t* fname, int wallp_scaling);

#pragma swi_number=0x2A5
__swi __arm GUI* CreateYesNoQuestionVA( int zero, ... );
#pragma swi_number=0x2A6
__swi __arm void ListMenu_SetItemSecondLineText( GUI_LIST*, int elem_num, TEXTID );

#pragma swi_number=0x2A7
__swi __arm void _REQUEST_OAF_START_APPLICATION( const int* sync, int __zero, MIDP_DESC*, int ___zero );

#pragma swi_number=0x2A8
__swi __arm wchar_t* FSX_GetInvalidChar( wchar_t* name );

#pragma swi_number=0x2A9
__swi __arm void DataBrowserDesc_SetFoldersNumber( void* DataBrowserDesc, int state );
#pragma swi_number=0x2AA
__swi __arm void DataBrowserDesc_SetActions( void* DataBrowserDesc, char* actions );
#pragma swi_number=0x2AB
__swi __arm void DataBrowserDesc_SetSelectAction( void* DataBrowserDesc, int state );
#pragma swi_number=0x2AC
__swi __arm void DataBrowserDesc_SetSelectActionOnFolders( void* DataBrowserDesc, int state );
#pragma swi_number=0x2AD
__swi __arm void DataBrowserDesc_SetBookID( void* DataBrowserDesc, int BookID );
#pragma swi_number=0x2AE
__swi __arm void DataBrowserDesc_SetItemFilter( void* DataBrowserDesc, DB_FILE_FILTER );
#pragma swi_number=0x2AF
__swi __arm void DataBrowserDesc_SetOption( void* DataBrowserDesc, char* option );
#pragma swi_number=0x2B0
__swi __arm void DataBrowserDesc_SetOpenEmptyFolder( void* DataBrowserDesc, int state );
#pragma swi_number=0x2B1
__swi __arm void DataBrowserDesc_SetFileExtList( void* DataBrowserDesc, const wchar_t* ExtList );

#pragma swi_number=0x2B2
__swi __arm void FSX_RemoveIllegalSymbolsName( FILEITEM* );
#pragma swi_number=0x2B3
__swi __arm FILEITEM* FILEITEM_Create( void );
#pragma swi_number=0x2B4
__swi __arm void FILEITEM_Destroy( FILEITEM* );
#pragma swi_number=0x82B4
__swi __arm void* FILEITEM_Destroy_adr( void );

#pragma swi_number=0x2B5
__swi __arm GUI* CreateDateInputVA( int zero, ... );
#pragma swi_number=0x2B6
__swi __arm GUI* CreateTimeInputVA( int zero, ... );

#pragma swi_number=0x2B7
__swi __arm void StringInput_SetCursorPosition( GUI*, u16 curs_pos, char unk );

#pragma swi_number=0x2B8
__swi __arm void BookObj_CallPage( BOOK* book, const PAGE_DESC* page );

#pragma swi_number=0x2B9
__swi __arm unsigned int DateInput_GetDateInt( GUI* );

#pragma swi_number=0x2BA
__swi __arm unsigned int TimeInput_GetTimeInt( GUI* );

#pragma swi_number=0x82BB
__swi __arm FONT_DESC* GetFontDesc( void );

#pragma swi_number=0x82BC
__swi __arm int* GetFontCount( void );

#pragma swi_number=0x2BD
__swi __arm int GC_GetBrushColor( GC* gc );
#pragma swi_number=0x2BE
__swi __arm void GC_SetBrushColor( GC* gc, int brush_color );
#pragma swi_number=0x2BF
__swi __arm void GC_DrawRoundRect( GC* gc, RECT*, int arcWidth, int arcHeight, int border_flag, int fill_flag );

#pragma swi_number=0x2C0
__swi __arm BOOK* MenuBook_Desktop( int mode, int BookID );
#pragma swi_number=0x2C1
__swi __arm wchar_t* MenuBook_Desktop_GetSelectedItemID( BOOK* MenuBook_Desktop );
#pragma swi_number=0x2C2
__swi __arm void BookObj_SoftKeys_SetAction( BOOK* book, int actionID, SKACTIONPROC proc );
#pragma swi_number=0x2C3
__swi __arm void BookObj_SoftKeys_SetText( BOOK* book, int actionID, TEXTID );
#pragma swi_number=0x2C4
__swi __arm BOOK* FindBookByID( int BookID );

#pragma swi_number=0x2C5
__swi __arm GUI_FEEDBACK* TextFeedbackWindow( BOOK* book, int zero );
#pragma swi_number=0x2C6
__swi __arm void Feedback_SetKeyHook( GUI_FEEDBACK*, void (*hook)( BOOK* book, int key, int unk, int unk2 ) );
#pragma swi_number=0x2C7
__swi __arm void Feedback_SetOnClose( GUI_FEEDBACK*, void (*OnClose)( BOOK* book ) );
#pragma swi_number=0x2C8
__swi __arm void Feedback_SetTimeout( GUI_FEEDBACK*, int time_ms );
#pragma swi_number=0x2C9
__swi __arm void Feedback_SetTextExtended( GUI_FEEDBACK*, TEXTID text, int where );

#pragma swi_number=0x2CA
__swi __arm void TabMenuBar_SetOnTabSwitch( GUI_TABMENUBAR*, void (*proc)( BOOK*, int act_tab ) );

#pragma swi_number=0x2CB
__swi __arm TEXTID KeyCode2Name( int key_code );

#pragma swi_number=0x2CC
__swi __arm int ImageID_Get( const wchar_t* fpath, const wchar_t* fname, IMAGEID* );
#pragma swi_number=0x2CD
__swi __arm void ImageID_Free( IMAGEID );

#pragma swi_number=0x2CE
__swi __arm SUB_EXECUTE* DataBrowser_CreateSubExecute( int BookID, FILEITEM* );
#pragma swi_number=0x2CF
__swi __arm int DataBrowser_ExecuteSubroutine( SUB_EXECUTE* sub, int action, u16* unk );
#pragma swi_number=0x2D0
__swi __arm int FILEITEM_SetFname( FILEITEM*, const wchar_t* fname );
#pragma swi_number=0x2D1
__swi __arm int FILEITEM_SetPath( FILEITEM*, const wchar_t* fpath );
#pragma swi_number=0x2D2
__swi __arm wchar_t* FILEITEM_GetFname( FILEITEM* );
#pragma swi_number=0x2D3
__swi __arm wchar_t* FILEITEM_GetPath( FILEITEM* );
#pragma swi_number=0x2D4
__swi __arm int FILEITEM_SetFnameAndContentType( FILEITEM*, const wchar_t* fname );
#pragma swi_number=0x2D5
__swi __arm int FILEITEM_SetPathAndContentType( FILEITEM*, const wchar_t* fpath );

#pragma swi_number=0x2D6
__swi __arm int JavaDialog_Open( int unk1, char* unk2, void** JavaDesc );
#pragma swi_number=0x2D7
__swi __arm void JavaDialog_Close( int unk1 );
#pragma swi_number=0x2D8
__swi __arm int JavaAppDesc_Get( int unk1, void** JavaDesc );
#pragma swi_number=0x2D9
__swi __arm int JavaAppDesc_GetFirstApp( void* JavaDesc );
#pragma swi_number=0x2DA
__swi __arm int JavaAppDesc_GetNextApp( void* JavaDesc );
#pragma swi_number=0x2DB
__swi __arm int JavaAppDesc_GetJavaAppInfo( void* JavaDesc, int ID, wchar_t** wstr );

#pragma swi_number=0x2DC
__swi __arm void OrangeLED_Control( int _free_val, int or_LED_ID, int level, int fade_time );

#pragma swi_number=0x2DD
__swi __arm void DispObject_SetRefreshTimer( DISP_OBJ*, int time );
#pragma swi_number=0x2DE
__swi __arm void DispObject_KillRefreshTimer( DISP_OBJ* );

#pragma swi_number=0x2DF
__swi __arm int IsDataBrowserBook( BOOK* book );
#pragma swi_number=0x82DF
__swi __arm IS_NEEDED_BOOK get_IsDataBrowserBook( void );

#pragma swi_number=0x2E0
__swi __arm SUB_EXECUTE* BrowserItem_Get_SUB_EXECUTE( BOOK* BrowserItemBook );

#pragma swi_number=0x2E1
__swi __arm TEXTID Shortcut_Get_MenuItemName( void* );
#pragma swi_number=0x2E2
__swi __arm IMAGEID Shortcut_Get_MenuItemIconID( void* );

#pragma swi_number=0x2E3
__swi __arm BOOK* FindBookEx( int (*cmp_proc)( BOOK* book_from_list, int* param ), int* param );
#pragma swi_number=0x2E4
__swi __arm FILEITEM* FILEITEM_CreateCopy( FILEITEM* );

#pragma swi_number=0x2E5
__swi __arm IMAGEID ImageID_Copy( IMAGEID );

#pragma swi_number=0x2E6
__swi __arm int BookObj_GetSessionID( BOOK* book );
#pragma swi_number=0x2E7
__swi __arm void UI_Event_toSID( int event, int sess_id );
#pragma swi_number=0x2E8
__swi __arm char* GetIMSI( void );
#pragma swi_number=0x2E9
__swi __arm void UI_CONTROLLED_SHUTDOWN_RESPONSE( int book_id );

#pragma swi_number=0x2EA
__swi __arm int MainInput_getCurPos( GUI* );
#pragma swi_number=0x2EB
__swi __arm void MainInput_Hide( GUI* );
#pragma swi_number=0x2EC
__swi __arm void DispObject_SoftKeys_RestoreDefaultAction( DISP_OBJ*, int action );
#pragma swi_number=0x2ED
__swi __arm TEXTID PNUM2Name( BCD_TEXT pnum, int isIconNeeded, int only_full_number_flag );
#pragma swi_number=0x2EE
__swi __arm int SpeedDial_GetPNUM( int _zero, char charter0__9, BCD_TEXT PNUM );

#pragma swi_number=0x2EF
__swi __arm void* MetaData_Desc_Create( wchar_t* path, wchar_t* name );
#pragma swi_number=0x2F0
__swi __arm void MetaData_Desc_Destroy( void* MetaData_Desc );
#pragma swi_number=0x2F1
__swi __arm wchar_t* MetaData_Desc_GetTags( void* MetaData_Desc, int tagID );
#pragma swi_number=0x2F2
__swi __arm int MetaData_Desc_GetTrackNum( void* MetaData_Desc, int __NULL );

#pragma swi_number=0x2F3
__swi __arm int DispObject_GetWindowWidth( DISP_OBJ* );
#pragma swi_number=0x2F4
__swi __arm int DispObject_GetWindowHeight( DISP_OBJ* );
#pragma swi_number=0x2F5
__swi __arm int get_system_langID( void );
#pragma swi_number=0x2F6
__swi __arm char* lang_get_name( int langID );

#pragma swi_number=0x2F7
__swi __arm int w_chdir( const wchar_t* path );
#pragma swi_number=0x2F8
__swi __arm int w_fopen( const wchar_t* name, int attr, int rights, int err );
#pragma swi_number=0x2F9
__swi __arm int w_fstat( const wchar_t* name, W_FSTAT* );
#pragma swi_number=0x2FA
__swi __arm int w_fwrite( int f, void* buf, int size );
#pragma swi_number=0x2FB
__swi __arm int w_fread( int f, void* buf, int size );
#pragma swi_number=0x2FC
__swi __arm int w_lseek( int f, int offs, int mode );
#pragma swi_number=0x2FD
__swi __arm int w_fclose( int f );
#pragma swi_number=0x2FE
__swi __arm int w_mkdir( const wchar_t* path, int mode );
#pragma swi_number=0x2FF
__swi __arm int DirHandle_SetFilterStr( DIR_HANDLE*, const wchar_t* filter );
#pragma swi_number=0x300
__swi __arm int Disp_GetTextIDWidth( TEXTID, int len );

#pragma swi_number=0x301
__swi __arm int MetaData_Desc_GetCoverInfo( void* MetaData_Desc, char* cover_type, int* size, int* cover_offset );
#pragma swi_number=0x302
__swi __arm int ImageID_GetIndirect( void* buf_image, int size, int __NULL, wchar_t* image_type, IMAGEID* );

#pragma swi_number=0x303
__swi __arm void unixtime2datetime( int, DATETIME* );
#pragma swi_number=0x304
__swi __arm int List_Insert( LIST* lst, int i, void* item );
#pragma swi_number=0x305
__swi __arm int FileCopy( wchar_t* src_path, wchar_t* src_name, wchar_t* dest_path, wchar_t* dest_name, int );
#pragma swi_number=0x306
__swi __arm int FileMove( wchar_t* src_path, wchar_t* src_name, wchar_t* dest_path, wchar_t* dest_name, int );

#pragma swi_number=0x307
__swi __arm void RedLED_On( int __NULL );
#pragma swi_number=0x308
__swi __arm void RedLED_Off( int __NULL );

#pragma swi_number=0x309
__swi __arm void DispObject_SetLayerColor( DISP_OBJ*, int color );

#pragma swi_number=0x30A
__swi __arm int JavaAppDesc_GetJavaAppID( void* JavaDesc );
#pragma swi_number=0x30B
__swi __arm void REQUEST_UI_OAF_START_APPLICATION( const int* sync, int appID, char* flag );

#pragma swi_number=0x30C
__swi __arm int JavaApp_LogoImageID_Get( wchar_t* fullpath, IMAGEID* );

#pragma swi_number=0x30D
__swi __arm GUI* CreateProgressBar( BOOK* book, int display );
#pragma swi_number=0x30E
__swi __arm void ProgressBar_SetText( GUI*, TEXTID text );
#pragma swi_number=0x30F
__swi __arm void ProgressBar_SetIcon( GUI*, IMAGEID );
#pragma swi_number=0x310
__swi __arm void ProgressBar_SetPercentValue( GUI*, int value );
#pragma swi_number=0x311
__swi __arm void ProgressBar_SetBarDisabled( GUI*, BOOL );

#pragma swi_number=0x312
__swi __arm void StringInput_DispObject_SetText( DISP_OBJ*, TEXTID );

#pragma swi_number=0x313
__swi __arm void GUIObject_SetTitleBackgroundImage( GUI*, IMAGEID );
#pragma swi_number=0x314
__swi __arm void GUIObject_SetTitleIcon( GUI*, IMAGEID );
#pragma swi_number=0x315
__swi __arm void GUIObject_SetCursorImage( GUI*, IMAGEID );
#pragma swi_number=0x316
__swi __arm void GUIObject_SetBackgroundImage( GUI*, IMAGEID );

#pragma swi_number=0x317
__swi __arm void* DispObject_SoftKeys_GetParams( DISP_OBJ* );
#pragma swi_number=0x318
__swi __arm LIST* DispObject_SoftKeys_GetList( DISP_OBJ*, BOOK* book, char __zero );

#pragma swi_number=0x319
__swi __arm int BookObj_GetDisplayOrientation( BOOK* book );
#pragma swi_number=0x31A
__swi __arm void BookObj_SetDisplayOrientation( BOOK* book, int orientation );

#pragma swi_number=0x31B
__swi __arm BOOK* Display_GetTopBook( int display );
#pragma swi_number=0x31C
__swi __arm void Display_SetBrightness( int display, int brightness );
#pragma swi_number=0x31D
__swi __arm int Display_GetBrightness( int display );

#pragma swi_number=0x31E
__swi __arm void DataBrowserDesc_Menu_AddFSFunctions( void* DataBrowserDesc, int );
#pragma swi_number=0x31F
__swi __arm void DataBrowserDesc_Menu_AddNewFolder( void* DataBrowserDesc, int );
#pragma swi_number=0x320
__swi __arm void DataBrowserDesc_Menu_AddMarkFiles( void* DataBrowserDesc, int );

#pragma swi_number=0x321
__swi __arm void SetTheme( wchar_t* path, wchar_t* name, int bookid, int unk_1 );

#pragma swi_number=0x322
__swi __arm void SoftKeys_GetLabel( DISP_OBJ* softkeys, SKLABEL* lbl, int id );
#pragma swi_number=0x323
__swi __arm void SoftKeys_Update( DISP_OBJ* softkeys );

#pragma swi_number=0x324
__swi __arm int get_CellData( PLMN_LAC_DESC* desc1, RAT_CI_DESC* desc2, char* CSReg );

#pragma swi_number=0x325
__swi __arm void* IncommingCall_Reject( BOOK* book );
#pragma swi_number=0x326
__swi __arm void* IncommingCall_Mute( BOOK* book );

#pragma swi_number=0x327
__swi __arm int GetRadioState( char* level, char* flag );

#pragma swi_number=0x328
__swi __arm int Bluetooth_GetState( void );
#pragma swi_number=0x329
__swi __arm int Bluetooth_GetPhoneVisibility( void );
#pragma swi_number=0x32A
__swi __arm int Bluetooth_isBusy( void );

#pragma swi_number=0x32B
__swi __arm int USB_isConnected( void );

#pragma swi_number=0x32C
__swi __arm void* w_diropen( const wchar_t* dir );
#pragma swi_number=0x32D
__swi __arm wchar_t* w_dirread( void* );
#pragma swi_number=0x32E
__swi __arm int w_dirclose( void* );
#pragma swi_number=0x32F
__swi __arm int w_rename( const wchar_t* dst, const wchar_t* src );
#pragma swi_number=0x330
__swi __arm int w_remove( const wchar_t* dir );

#pragma swi_number=0x331
__swi __arm int ConnectionManager_Connection_GetState( void );

#pragma swi_number=0x332
__swi __arm int GetSignalQuality( char* rssi, char* ber );

#pragma swi_number=0x333
__swi __arm int IsCameraBook( BOOK* bk );
#pragma swi_number=0x8333
__swi __arm IS_NEEDED_BOOK get_IsCameraBook( void );

#pragma swi_number=0x334
__swi __arm int IsSoundRecorderBook( BOOK* bk );
#pragma swi_number=0x8334
__swi __arm IS_NEEDED_BOOK get_IsSoundRecorderBook( void );

#pragma swi_number=0x335
__swi __arm void StringInput_MenuItem_SetPriority( GUI* strinp, int prio, int actionID );

#pragma swi_number=0x336
__swi __arm void VideoPlayerControl( BOOK* VideoPlayerBook, int );
#pragma swi_number=0x337
__swi __arm void Video_ActionBack( BOOK* VideoPlayerBook, void* );
#pragma swi_number=0x338
__swi __arm void Video_ExtractFrame( BOOK* VideoPlayerBook, void* );
#pragma swi_number=0x339
__swi __arm void Video_ZoomOn( BOOK* VideoPlayerBook, void* );

#pragma swi_number=0x33A
__swi __arm void RightNow_SetActive( int flag );

#pragma swi_number=0x33B
__swi __arm void VideoResize_AutomateMode( BOOK* VideoPlayerBook, u16 VideoWidth, u16 VideoHeight, u16* NewWidth, u16* NewHeight );
#pragma swi_number=0x33C
__swi __arm void VideoResize_AllScreenMode( BOOK* VideoPlayerBook, u16 VideoWidth, u16 VideoHeight, u16* NewWidth, u16* NewHeight );

#pragma swi_number=0x33D
__swi __arm int Video_GetCurrentSkinID( BOOK* VideoPlayerBook );
#pragma swi_number=0x33E
__swi __arm void Video_SetSkin( GUI* VideoPlayerGUI, int skinID );
#pragma swi_number=0x33F
__swi __arm int Video_SetPermit( BOOK* VideoPlayerBook, u16 NewWidth, u16 NewHeight );
#pragma swi_number=0x340
__swi __arm int Video_Refresh( BOOK* VideoPlayerBook, GUI* VideoPlayerGUI );

#pragma swi_number=0x341
__swi __arm void Video_Play( BOOK* VideoPlayerBook, void* );
#pragma swi_number=0x342
__swi __arm void Video_Pause( BOOK* VideoPlayerBook, void* );
#pragma swi_number=0x343
__swi __arm void Video_Stop( BOOK* VideoPlayerBook, void* );

#pragma swi_number=0x344
__swi __arm void Audio_Pause( BOOK* );
#pragma swi_number=0x345
__swi __arm void Audio_Play( BOOK* );

#pragma swi_number=0x346
__swi __arm GUI* CreatePercentInputVA( int zero, ... );
#pragma swi_number=0x347
__swi __arm int PercentInput_GetPercent( GUI* PercentInput );

#pragma swi_number=0x348
__swi __arm void GUIonMessage_SetLineSeparator( GUI_MESSAGE* msg, int _unk1 );

#pragma swi_number=0x349
__swi __arm int PHF_GetState( void );
#pragma swi_number=0x34A
__swi __arm int CHF_GetState( void );

#pragma swi_number=0x34B
__swi __arm void GUIObject_SoftKeys_RemoveBackground( GUI* );
#pragma swi_number=0x34C
__swi __arm void GUIObject_SoftKeys_RestoreBackground( GUI* );

#pragma swi_number=0x34D
__swi __arm void SetTrayIcon( IMAGEID, char mode );

#pragma swi_number=0x34E
__swi __arm int REQUEST_SETTING_RINGVOLUME_SET( const int* sync, int unk, int null, int vol );
#pragma swi_number=0x34F
__swi __arm int REQUEST_SETTING_VIBRATOR_SET( const int* sync, int unk, int mode );
#pragma swi_number=0x350
__swi __arm int REQUEST_SETTING_RINGTYPESOUNDFILE_SET( const int* sync, int null, wchar_t* path, wchar_t* name );
#pragma swi_number=0x351
__swi __arm int REQUEST_SETTING_ANSWERINGMODE_SET( const int* sync, int unk, int mode );
#pragma swi_number=0x352
__swi __arm int REQUEST_SETTING_INCREASINGRING_SET( const int* sync, int unk, int unk2, int mode );
#pragma swi_number=0x353
__swi __arm int REQUEST_SETTING_ALLOWEDCALLERS_SET( const int* sync, int unk, int mode );

#pragma swi_number=0x354
__swi __arm int JavaAppDesc_GetJavaAppFullpath( void* JavaDesc, JavaAppFullpath* );

#pragma swi_number=0x355
__swi __arm void* SoundRecorderDesc_Create( void );
#pragma swi_number=0x356
__swi __arm void SoundRecorderDesc_Destroy( void* desc );
#pragma swi_number=0x357
__swi __arm int SoundRecorderDesc_SetBookID( void* desc, int BookID );
#pragma swi_number=0x358
__swi __arm int SoundRecorderDesc_SetType( void* desc, const int type );
#pragma swi_number=0x359
__swi __arm int SoundRecorderDesc_SetFolder( void* desc, const wchar_t* fpath );
#pragma swi_number=0x35A
__swi __arm int SoundRecorderDesc_SetFname( void* desc, const wchar_t* fname );
#pragma swi_number=0x35B
__swi __arm int SoundRecorderDesc_SetRecordSize( void* desc, const int size );
#pragma swi_number=0x35C
__swi __arm int SoundRecorder_Create( void* desc );

#pragma swi_number=0x35D
__swi __arm int SoundRecorder_RecordCall( BOOK* OngoingCallBook );

#pragma swi_number=0x35E
__swi __arm int Browser_OpenURI( BOOK* book, char* URI, int mode );
#pragma swi_number=0x35F
__swi __arm char* GetURIScheme( int schemeID );
#pragma swi_number=0x360
__swi __arm char* CreateURI( wchar_t* fpath, wchar_t* fname, char* URIScheme );

#pragma swi_number=0x361
__swi __arm void DataBrowserDesc_SetViewModeAndSortOrder( void* DataBrowserDesc, int view_sort_ID );

#pragma swi_number=0x362
__swi __arm int Alarm_GetCurrentTypeAndAlarmID( char* type, wchar_t* AlarmID );
#pragma swi_number=0x363
__swi __arm int Alarm_GetState( char* state, int AlarmID );
#pragma swi_number=0x364
__swi __arm int Alarm_GetTIME( TIME* AlarmTime, int AlarmID );
#pragma swi_number=0x365
__swi __arm int Alarm_GetWeekSettings( void* week, int AlarmID );
#pragma swi_number=0x366
__swi __arm int IrDa_GetState( char* state );
#pragma swi_number=0x367
__swi __arm void REQUEST_SETTING_ALLOWEDCALLERS_GET( const int* sync, int ProfileNum, char* state );
#pragma swi_number=0x368
__swi __arm void* IncommingCall_Accept( BOOK* book );

#pragma swi_number=0x369
__swi __arm void MediaPlayer_SoftKeys_SetText( GUI* player_gui, int actionID, TEXTID );
#pragma swi_number=0x36A
__swi __arm void MediaPlayer_SoftKeys_SetAction( GUI* player_gui, int actionID, SKACTIONPROC proc );
#pragma swi_number=0x36B
__swi __arm void MediaPlayer_SoftKeys_SetItemAsSubItem( GUI* player_gui, int item, int subitem );
#pragma swi_number=0x36C
__swi __arm void MediaPlayer_SoftKeys_AddHelpStr( GUI* player_gui, int item, TEXTID );
#pragma swi_number=0x36D
__swi __arm void MediaPlayer_ShowNowPlaying( DISP_OBJ*, int );

#pragma swi_number=0x36E
__swi __arm void DispObject_Show( DISP_OBJ*, int mode );
#pragma swi_number=0x36F
__swi __arm int Request_EventChannel_Subscribe( const int* sync, int mode, int event );

#pragma swi_number=0x370
__swi __arm void GUIObject_SoftKeys_ExecuteAction( GUI*, u16 actionID );

#pragma swi_number=0x371
__swi __arm int Request_ICA_ShutdownAllConnections( const int* sync );

#pragma swi_number=0x372
__swi __arm int REQUEST_PROFILE_GETPROFILENAME( const int* sync, int unk, TEXTID_DATA*, char* error );

#pragma swi_number=0x373
__swi __arm int DataBrowserDesc_SetFocusToFILEITEM( void* DataBrowserDesc, FILEITEM* );

#pragma swi_number=0x374
__swi __arm int GUIonMessage_GetMsg( GUI_MESSAGE* msg );
#pragma swi_number=0x375
__swi __arm BOOK* GUIonMessage_GetBook( GUI_MESSAGE* msg );
#pragma swi_number=0x376
__swi __arm GUI* GUIonMessage_GetGui( GUI_MESSAGE* msg );
#pragma swi_number=0x377
__swi __arm int GUIonMessage_GetSelectedItem( GUI_MESSAGE* msg );
#pragma swi_number=0x378
__swi __arm int GUIonMessage_GetPrevSelectedItem( GUI_MESSAGE* msg );

#pragma swi_number=0x379
__swi __arm void ObexSendFile( SEND_OBEX_STRUCT* );


#pragma swi_number=0x37A
__swi __arm void Feedback_SetManualScrollingText( DISP_OBJ* feedback_disp_obj, int state );

#pragma swi_number=0x37B
__swi __arm int EqualizerGain_Set( int AudioSessionID, int Band, int Level );

#pragma swi_number=0x37C
__swi __arm int CANVAS_Get_GviGC( PCANVAS pcanvas, GVI_GC* dest );
#pragma swi_number=0x37D
__swi __arm void GVI_BitBlt( GVI_GC destgc, int destx, int desty, int destxsize, int destysize, GVI_GC srcgc, int srcx, int srcy, int _204, int _0, int _0_2, int _0_3 );
#pragma swi_number=0x37E
__swi __arm void GVI_StretchBlt( GVI_GC destgc, int destx, int desty, int destxsize, int destysize, GVI_GC srcgc, int srcx, int srcy, int srcxsize, int srcysize, int _204, int _0, int _0_2, int _0_3 );
#pragma swi_number=0x37F
__swi __arm void GVI_TransformBlt( GVI_GC destgc, int destx, int desty, GVI_GC srcgc, int srcx, int srcy, int srcxsize, int srcysize, int orientation, int _0 );

#pragma swi_number=0x380
__swi __arm GVI_PEN GVI_CreatePen( char thikness, int color );
#pragma swi_number=0x381
__swi __arm GVI_PEN GVI_CreateDashedPen( char thikness, int color, int bitmask, int step, int offset );
#pragma swi_number=0x382
__swi __arm GVI_BRUSH GVI_CreateSolidBrush( int color );
#pragma swi_number=0x383
__swi __arm BOOL GVI_Delete_GVI_Object( GVI_OBJ* );

#pragma swi_number=0x384
__swi __arm void GVI_DrawEllipse( GVI_GC gc, int x, int y, int w, int h, GVI_PEN pen );
#pragma swi_number=0x385
__swi __arm void GVI_DrawLine( GVI_GC gc, int x1, int y1, int x2, int y2, GVI_PEN pen );
#pragma swi_number=0x386
__swi __arm void GVI_DrawLines( GVI_GC gc, POINT* points, int count, GVI_PEN pen );
#pragma swi_number=0x387
__swi __arm void GVI_DrawPolyLine( GVI_GC gc, POINT* points, int count, GVI_PEN pen, int is_closed );
#pragma swi_number=0x388
__swi __arm void GVI_DrawRectangle( GVI_GC gc, int x, int y, int w, int h, GVI_PEN pen );
#pragma swi_number=0x389
__swi __arm void GVI_DrawRoundRect( GVI_GC gc, int x, int y, int w, int h, int arcw, int arch, GVI_PEN pen );

#pragma swi_number=0x38A
__swi __arm void GVI_DrawSolidLines( GVI_GC gc, POINT* points, int count, int color, int width );
#pragma swi_number=0x38B
__swi __arm void GVI_DrawSolidPolyLine( GVI_GC gc, POINT* points, int count, int is_closed, int color, int width );
#pragma swi_number=0x38C
__swi __arm void GVI_DrawSolidRectangle( GVI_GC gc, int x, int y, int w, int h, int color, int width );

#pragma swi_number=0x38D
__swi __arm void GVI_FillEllipse( GVI_GC gc, int x, int y, int w, int h, GVI_BRUSH brush );
#pragma swi_number=0x38E
__swi __arm void GVI_FillPolygon( GVI_GC gc, POINT* pp, int count, GVI_BRUSH brush );
#pragma swi_number=0x38F
__swi __arm void GVI_FillRectangle( GVI_GC gc, int x, int y, int w, int h, GVI_BRUSH brush );
#pragma swi_number=0x390
__swi __arm void GVI_FillRectangles( GVI_GC gc, GVI_XYWH* rcs, int count, GVI_BRUSH brush );

#pragma swi_number=0x391
__swi __arm void GVI_FillSolidEllipse( GVI_GC gc, int x, int y, int w, int h, int color );
#pragma swi_number=0x392
__swi __arm void GVI_FillSolidRectangle( GVI_GC gc, int x, int y, int w, int h, int color );
#pragma swi_number=0x393
__swi __arm int GVI_FillSolidRoundRect( GVI_GC gc, int x, int y, int w, int h, int arcx, int arcy, int color );
#pragma swi_number=0x394
__swi __arm void GVI_FillSolidTriangle( GVI_GC gc, int x1, int y1, int x2, int y2, int x3, int y3, int color );

#pragma swi_number=0x395
__swi __arm GC* GC_CreateMemoryGC( int xsize, int ysize, int bpp, int unk, void* somefn, int unk2 );
#pragma swi_number=0x396
__swi __arm void GC_FreeGC( GC* gc );

#pragma swi_number=0x397
__swi __arm void REQUEST_ALARMCLOCKSTATUS_GET( const int* sync, char* alarm_status );
#pragma swi_number=0x398
__swi __arm void REQUEST_ALARMCLOCKTIME_GET( const int* sync, TIME* t );
#pragma swi_number=0x399
__swi __arm void REQUEST_ALARMDATEANDTIMESTATUS_GET( const int* sync, DATETIME* );

#pragma swi_number=0x39A
__swi __arm int Settings_ShowNumber_Get( char* state );

#pragma swi_number=0x39B
__swi __arm GVI_BMP GVI_CreateBitmap( int xsize, int ysize, int bpp );
#pragma swi_number=0x39C
__swi __arm GVI_GC GVI_CreateMemoryGC( GVI_BMP bitmap );
#pragma swi_number=0x39D
__swi __arm void GVI_DeleteMemoryGC( GVI_GC gc );

#pragma swi_number=0x39E
__swi __arm int StringInput_DispObject_SetLanguage( DISP_OBJ_STRING_INPUT*, int langID, int flag );

#pragma swi_number=0x39F
__swi __arm int List_InsertSorted( LIST* list, void* item, int (*cmp_proc)( void*, void* ) );

#pragma swi_number=0x3A0
__swi __arm int ListMenu_SetItemIcon( GUI_LIST*, int Item, wchar_t unk_FFFF, int mode, IMAGEID );

#pragma swi_number=0x3A1
__swi __arm void wstrnupr( wchar_t* wstr, int maxlen );
#pragma swi_number=0x3A2
__swi __arm void wstrnlwr( wchar_t* wstr, int maxlen );

#pragma swi_number=0x3A3
__swi __arm int ListMenu_SetItemTextScroll( GUI_LIST*, int scroll );

#pragma swi_number=0x3A4
__swi __arm void GUIObject_SetTitleTextColor( GUI*, int color );
#pragma swi_number=0x3A5
__swi __arm void GUIObject_SetListTextColor( GUI*, int unk1, int list_color, int unk2, int unk3, int list_select_color, int unk4, int _zerro );
#pragma swi_number=0x3A6
__swi __arm void DispObject_SetTitleTextColor( DISP_OBJ*, int color );
#pragma swi_number=0x3A7
__swi __arm void DispObject_SetListTextColor( DISP_OBJ*, int unk1, int list_color, int unk2, int unk3, int list_select_color, int unk4, int _zerro );
#pragma swi_number=0x3A8
__swi __arm void DispObject_SetCursorImage( DISP_OBJ*, IMAGEID );
#pragma swi_number=0x3A9
__swi __arm void DispObject_SetBackgroundImage( DISP_OBJ*, IMAGEID );
#pragma swi_number=0x3AA
__swi __arm void DispObject_SetTitleBackgroundImage( DISP_OBJ*, IMAGEID );

#pragma swi_number=0x3AB
__swi __arm void TabMenuBar_SetTabTitle( GUI_TABMENUBAR*, int tab_num, TEXTID );

#pragma swi_number=0x3AC
__swi __arm BOOK* GUIObject_GetBook( GUI* );

#pragma swi_number=0x3AD
__swi __arm u16 SoftKeys_GetSelectedAction( void );

#pragma swi_number=0x3AE
__swi __arm void MonitorFeedback_SetTimer( GUI_FEEDBACK*, int time, void (*callback)(BOOK*) );


#pragma swi_number=0x83AF
__swi __arm void (*getListMenuOnGuiDestroy())( GUI* );
#pragma swi_number=0x83B0
__swi __arm void (*getListMenuOnDescCreate())( DISP_DESC* );
#pragma swi_number=0x83B1
__swi __arm void (*getListMenuOnDispMessage())( DISP_OBJ*, void*, GUI* );

#pragma swi_number=0x83B2
__swi __arm void (*getTabMenuBarOnGuiDestroy())( GUI* );
#pragma swi_number=0x83B3
__swi __arm void (*getTabMenuBarOnDescCreate())( DISP_DESC* );
#pragma swi_number=0x83B4
__swi __arm void (*getTabMenuBarOnDispMessage())( DISP_OBJ*, void*, GUI* );

#pragma swi_number=0x83B5
__swi __arm void (*getOneOfManyOnGuiDestroy())( GUI* );
#pragma swi_number=0x83B6
__swi __arm void (*getOneOfManyOnDescCreate())( DISP_DESC* );
#pragma swi_number=0x83B7
__swi __arm void (*getOneOfManyOnDispMessage())( DISP_OBJ*, void*, GUI* );

#pragma swi_number=0x83B8
__swi __arm void (*getNOfManyOnGuiDestroy())( GUI* );
#pragma swi_number=0x83B9
__swi __arm void (*getNOfManyOnDescCreate())( DISP_DESC* );
#pragma swi_number=0x83BA
__swi __arm void (*getNOfManyOnDispMessage())( DISP_OBJ*, void*, GUI* );

#pragma swi_number=0x3BB
__swi __arm void ListMenu_DestroyItems( GUI_LIST* );

#pragma swi_number=0x3BC
__swi __arm void DispObject_SetScrollBarBackgroundColor( DISP_OBJ*, int color );
#pragma swi_number=0x3BD
__swi __arm void DispObject_SetScrollBarColor( DISP_OBJ*, int color );
#pragma swi_number=0x3BE
__swi __arm void DispObject_SetSoftkeysColor( DISP_OBJ*, int color );

#pragma swi_number=0x3BF
__swi __arm void YesNoQuestion_SetIcon( GUI*, IMAGEID );
#pragma swi_number=0x3C0
__swi __arm void GUIObject_SetBacklightTimeout( GUI*, int time );

#pragma swi_number=0x3C1
__swi __arm void ListMenu_SetHotkeyMode( GUI_LIST*, LISTMENU_HOTKEY_MODE );

#pragma swi_number=0x3C2
__swi __arm void ListMenu_SetSearchLanguageMode( GUI_LIST*, int mode );
#pragma swi_number=0x3C3
__swi __arm void ListMenu_EnableSearchHeader( GUI_LIST*, BOOL );
#pragma swi_number=0x3C4
__swi __arm wchar_t* GUIonMessage_GetSearchString( GUI_MESSAGE* msg );
#pragma swi_number=0x3C5
__swi __arm int GUIonMessage_GetSearchStringLength( GUI_MESSAGE* msg );

#pragma swi_number=0x3C6
__swi __arm int EqualizerGain_Get( int AudioSessionID, int Band, int* Level );

#pragma swi_number=0x3C7
__swi __arm int Theme_DestroyMenuIcons( void );

#pragma swi_number=0x3C8
__swi __arm void CoCreateInstance( PUUID cid, PUUID iid, void** pInterface );

#pragma swi_number=0x3C9
__swi __arm void GUIObject_SetSecondRowTitleText( GUI*, TEXTID );
#pragma swi_number=0x3CA
__swi __arm int ListMenu_GetItemCount( GUI_LIST* );

#pragma swi_number=0x3CB
__swi __arm void OSE_GetShell( void** pInterface );

#pragma swi_number=0x3CC
__swi __arm int swscanf( const wchar_t* buffer, const wchar_t* format, ... );

#pragma swi_number=0x3CD
__swi __arm int sscanf( const char* buffer, const char* format, ... );

#pragma swi_number=0x3CE
__swi __arm void BookObj_WindowSetWantsFocus( BOOK* book, int display, BOOL WantsFocus );

#pragma swi_number=0x3CF
__swi __arm void BookObj_StayOnTop( BOOK* book, int mode );

#pragma swi_number=0x3D0
__swi __arm void DispObject_WindowSetPosition( DISP_OBJ*, int x, int y );

#pragma swi_number=0x3D1
__swi __arm void DispObject_WindowSetSize( DISP_OBJ*, int w, int h );

#pragma swi_number=0x3D2
__swi __arm TEXTID Feedback_DispObject_GetText( DISP_OBJ* );

#pragma swi_number=0x3D3
__swi __arm void DispObject_SoftKeys_ExecuteAction( DISP_OBJ*, u16 actionID );

#pragma swi_number=0x3D4
__swi __arm LIST* DataBrowserBook_GetCurrentFoldersList( BOOK* DataBrowserBook );

#pragma swi_number=0x3D5
__swi __arm void DispObject_SetAnimation( DISP_OBJ*, int style );

#pragma swi_number=0x3D6
__swi __arm void GUIObject_SetZOrder( GUI*, char, GUI* );

#pragma swi_number=0x3D7
__swi __arm int StringInput_GetCursorPosition( GUI*, u16* pos, char* _zero );

#pragma swi_number=0x3D8
__swi __arm void str2PNUM( BCD_TEXT pnum, char* str, int len_to_convert, int pnum_buf_len );
#pragma swi_number=0x3D9
__swi __arm void pb_ui_search_bk_create_search_menu( int BookID, int mode );

#pragma swi_number=0x3DA
__swi __arm int ThemeImage_Delete( int theme_image_type );


#pragma swi_number=0x3DD
__swi __arm void GUIObject_SoftKeys_SubActionPressedNotification( GUI*, u16 actionID, BOOL );

//#pragma swi_number=0x3DE

//#pragma swi_number=0x3DF

#pragma swi_number=0x3E0
__swi __arm int fs_demand_cache_page( int virtAddr, int mode, int intrMask );

#pragma swi_number=0x3E1
__swi __arm int fs_demand_pagePool_alloc_mem( pagePool* pagePool_p, int intrMask );

#pragma swi_number=0x3E2
__swi __arm wchar_t fs_demand_get_page_i_from_queue( wchar_t* queue, char only_allocked_pool, int page_count );

#pragma swi_number=0x3E3
__swi __arm void fs_demand_remove_from_queue( pageCache* page_p_to_remove, wchar_t page_i_to_insert );

#pragma swi_number=0x3E4
__swi __arm int fs_demand_kick_out_page( wchar_t page_i, int intrMask );

#pragma swi_number=0x83E5
__swi __arm char* getSWAP_DATA_BASE( void );

#ifdef LIBCLARANS
}
#endif

#endif
