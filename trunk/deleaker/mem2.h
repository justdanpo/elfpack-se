#ifndef __INCLUDE_MEM__
#define __INCLUDE_MEM__

#include "..\include\Lib_Clara.h"
#include "Lib_Clara_original.h"

#ifdef NDEBUG

#define trace_init(f)
#define trace_done()

void* operator new(size_t sz);
void* operator new[](size_t sz);
void operator delete(void * p);
void operator delete[](void * p);

void* operator new(size_t size, void* p);
void* operator new[](size_t size, void* p);
void operator delete (void*, void*);
void operator delete[] (void*, void*);

#else

#define DELEAKER

enum trace_types
{
	trace_memory,
	trace_strid,
	trace_iconid,
	trace_timer,
	trace_file,
	trace_hook,
	trace_dll,
	trace_gc,
	trace_gvi,
	trace_gui,
	trace_book,
	trace_process,
	trace_osebuff,
	trace_metadatadesc,
	trace_fileitemstruct,
	trace_w_dir,

	trace_unallocated,

	trace_typescount
};

#ifndef __cplusplus
void trace_init(wchar_t*);
#else
void trace_init(wchar_t* logname=NULL);
#endif
void trace_done();
void* trace_alloc_ret(int mt, void* ptr, void* badvalue, const char* file, int line);
void __deleaker_pushfileline( const char* __file__, int __line__ );

void operator delete(void* p);
void operator delete[](void* p);

void* operator new(size_t sz);
void* operator new[](size_t sz);

void operator delete (void*, void*);
void operator delete[] (void*, void*);

void* operator new(size_t size, void* p);
void* operator new[](size_t size, void* p);

#define new (__deleaker_pushfileline(__FILE__, __LINE__),false) ? NULL : new
#define delete __deleaker_pushfileline(__FILE__, __LINE__), delete




//!!! обязательно проверять аргументы
#define CreateStringInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateStringInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateDateInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateDateInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreatePercentInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreatePercentInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateStringInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateStringInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateTimeInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateTimeInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateYesNoQuestionVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateYesNoQuestionVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )

#define malloc(size) __deleaker_malloc( __FILE__,  __LINE__, size )
void* __deleaker_malloc( const char* __file__, int __line__, int size );
#define mfree_adr() __deleaker_mfree_adr( __FILE__,  __LINE__ )
void* __deleaker_mfree_adr( const char* __file__, int __line__ );
#define mfree(p) __deleaker_mfree( __FILE__,  __LINE__, p )
void __deleaker_mfree( const char* __file__, int __line__, void* p );
#define CreateBook(pbook, onClose, bp, name, ParentBookID, appdesc) __deleaker_CreateBook( __FILE__,  __LINE__, pbook, onClose, bp, name, ParentBookID, appdesc )
int __deleaker_CreateBook( const char* __file__, int __line__, BOOK* pbook, void (*onClose)( BOOK* ), const PAGE_DESC* bp, const char* name, int ParentBookID, const APP_DESC* appdesc );
#define FreeBook(book) __deleaker_FreeBook( __FILE__,  __LINE__, book )
void __deleaker_FreeBook( const char* __file__, int __line__, BOOK* book );
#define BookObj_KillBook(book) __deleaker_BookObj_KillBook( __FILE__,  __LINE__, book )
void __deleaker_BookObj_KillBook( const char* __file__, int __line__, BOOK* book );
#define List_Create() __deleaker_List_Create( __FILE__,  __LINE__ )
LIST* __deleaker_List_Create( const char* __file__, int __line__ );
#define List_Destroy(lst) __deleaker_List_Destroy( __FILE__,  __LINE__, lst )
void __deleaker_List_Destroy( const char* __file__, int __line__, LIST* lst );
#define manifest_GetParam(buf, param_name, unk) __deleaker_manifest_GetParam( __FILE__,  __LINE__, buf, param_name, unk )
char* __deleaker_manifest_GetParam( const char* __file__, int __line__, const char* buf, const char* param_name, int unk );
#define AllocDirHandle(path) __deleaker_AllocDirHandle( __FILE__,  __LINE__, path )
DIR_HANDLE* __deleaker_AllocDirHandle( const char* __file__, int __line__, const wchar_t* path );
#define DestroyDirHandle(handle) __deleaker_DestroyDirHandle( __FILE__,  __LINE__, handle )
void __deleaker_DestroyDirHandle( const char* __file__, int __line__, DIR_HANDLE* handle );
#define GUIObject_Create(__unknwnargname1, GuiDestroy, DispDescCreate, __unknwnargname4, DispObjCallBack, display, size_of_gui) __deleaker_GUIObject_Create( __FILE__,  __LINE__, __unknwnargname1, GuiDestroy, DispDescCreate, __unknwnargname4, DispObjCallBack, display, size_of_gui )
int __deleaker_GUIObject_Create( const char* __file__, int __line__, GUI* __unknwnargname1, void (*GuiDestroy)( GUI* ), void (*DispDescCreate)( DISP_DESC* ), BOOK* __unknwnargname4, void (*DispObjCallBack)( DISP_OBJ*, void* msg, GUI* ), int display, int size_of_gui );
#define GUIObject_Destroy(__unknwnargname1) __deleaker_GUIObject_Destroy( __FILE__,  __LINE__, __unknwnargname1 )
GUI* __deleaker_GUIObject_Destroy( const char* __file__, int __line__, GUI* __unknwnargname1 );
#define _fopen(filpath, filname, mode, rights, destfname) __deleaker__fopen( __FILE__,  __LINE__, filpath, filname, mode, rights, destfname )
int __deleaker__fopen( const char* __file__, int __line__, const wchar_t* filpath, const wchar_t* filname, unsigned int mode, unsigned int rights, FILELISTITEM* destfname );
#define fopen(fname, mode, rights) __deleaker_fopen( __FILE__,  __LINE__, fname, mode, rights )
int __deleaker_fopen( const char* __file__, int __line__, const wchar_t* fname, int mode, int rights );
#define fclose(file) __deleaker_fclose( __FILE__,  __LINE__, file )
int __deleaker_fclose( const char* __file__, int __line__, int file );
#define w_fopen(name, attr, rights, err) __deleaker_w_fopen( __FILE__,  __LINE__, name, attr, rights, err )
int __deleaker_w_fopen( const char* __file__, int __line__, const wchar_t* name, int attr, int rights, int err );
#define w_fclose(f) __deleaker_w_fclose( __FILE__,  __LINE__, f )
int __deleaker_w_fclose( const char* __file__, int __line__, int f );
#define CreateListMenu(__unknwnargname1, display) __deleaker_CreateListMenu( __FILE__,  __LINE__, __unknwnargname1, display )
GUI_LIST* __deleaker_CreateListMenu( const char* __file__, int __line__, BOOK* __unknwnargname1, int display );
#define CreateNOfMany(book) __deleaker_CreateNOfMany( __FILE__,  __LINE__, book )
GUI_NOFMANY* __deleaker_CreateNOfMany( const char* __file__, int __line__, BOOK* book );
#define CreateOneOfMany(book) __deleaker_CreateOneOfMany( __FILE__,  __LINE__, book )
GUI_ONEOFMANY* __deleaker_CreateOneOfMany( const char* __file__, int __line__, BOOK* book );
#define CreateTabMenuBar(book) __deleaker_CreateTabMenuBar( __FILE__,  __LINE__, book )
GUI_TABMENUBAR* __deleaker_CreateTabMenuBar( const char* __file__, int __line__, BOOK* book );
#define CreateProgressBar(book, display) __deleaker_CreateProgressBar( __FILE__,  __LINE__, book, display )
GUI* __deleaker_CreateProgressBar( const char* __file__, int __line__, BOOK* book, int display );
#define TextID_Create(wstr, flag, len) __deleaker_TextID_Create( __FILE__,  __LINE__, wstr, flag, len )
TEXTID __deleaker_TextID_Create( const char* __file__, int __line__, const void* wstr, TEXT_ENCODING flag, int len );
#define TextID_Copy(__unknwnargname1) __deleaker_TextID_Copy( __FILE__,  __LINE__, __unknwnargname1 )
TEXTID __deleaker_TextID_Copy( const char* __file__, int __line__, TEXTID __unknwnargname1 );
#define TextID_CreateIntegerID(num) __deleaker_TextID_CreateIntegerID( __FILE__,  __LINE__, num )
TEXTID __deleaker_TextID_CreateIntegerID( const char* __file__, int __line__, int num );
#define TextID_Destroy(__unknwnargname1) __deleaker_TextID_Destroy( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_TextID_Destroy( const char* __file__, int __line__, TEXTID __unknwnargname1 );
#define GUIObject_SetTitleText(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetTitleText( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetTitleText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID __unknwnargname2 );
#define GUIObject_SoftKeys_SetText(__unknwnargname1, actionID, __unknwnargname3) __deleaker_GUIObject_SoftKeys_SetText( __FILE__,  __LINE__, __unknwnargname1, actionID, __unknwnargname3 )
void __deleaker_GUIObject_SoftKeys_SetText( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 );
#define GUIonMessage_SetMenuItemText(msg, __unknwnargname2) __deleaker_GUIonMessage_SetMenuItemText( __FILE__,  __LINE__, msg, __unknwnargname2 )
char __deleaker_GUIonMessage_SetMenuItemText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 );
#define GUIonMessage_SetMenuItemSecondLineText(msg, __unknwnargname2) __deleaker_GUIonMessage_SetMenuItemSecondLineText( __FILE__,  __LINE__, msg, __unknwnargname2 )
char __deleaker_GUIonMessage_SetMenuItemSecondLineText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 );
#define GUIonMessage_SetMenuItemInfoText(msg, __unknwnargname2) __deleaker_GUIonMessage_SetMenuItemInfoText( __FILE__,  __LINE__, msg, __unknwnargname2 )
char __deleaker_GUIonMessage_SetMenuItemInfoText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 );
#define GUIonMessage_SetMenuItemUnavailableText(msg, __unknwnargname2) __deleaker_GUIonMessage_SetMenuItemUnavailableText( __FILE__,  __LINE__, msg, __unknwnargname2 )
char __deleaker_GUIonMessage_SetMenuItemUnavailableText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 );
#define MessageBox(header_text, message_text, __unknwnargname3, style, time, book) __deleaker_MessageBox( __FILE__,  __LINE__, header_text, message_text, __unknwnargname3, style, time, book )
int __deleaker_MessageBox( const char* __file__, int __line__, TEXTID header_text, TEXTID message_text, IMAGEID __unknwnargname3, int style, int time, BOOK* book );
#define BookObj_SoftKeys_SetText(book, actionID, __unknwnargname3) __deleaker_BookObj_SoftKeys_SetText( __FILE__,  __LINE__, book, actionID, __unknwnargname3 )
void __deleaker_BookObj_SoftKeys_SetText( const char* __file__, int __line__, BOOK* book, int actionID, TEXTID __unknwnargname3 );
#define DataBrowserDesc_SetHeaderText(DataBrowserDesc, __unknwnargname2) __deleaker_DataBrowserDesc_SetHeaderText( __FILE__,  __LINE__, DataBrowserDesc, __unknwnargname2 )
void __deleaker_DataBrowserDesc_SetHeaderText( const char* __file__, int __line__, void* DataBrowserDesc, TEXTID __unknwnargname2 );
#define DataBrowserDesc_SetOKSoftKeyText(DataBrowserDesc, __unknwnargname2) __deleaker_DataBrowserDesc_SetOKSoftKeyText( __FILE__,  __LINE__, DataBrowserDesc, __unknwnargname2 )
void __deleaker_DataBrowserDesc_SetOKSoftKeyText( const char* __file__, int __line__, void* DataBrowserDesc, TEXTID __unknwnargname2 );
#define DispObject_SetTitleText(__unknwnargname1, __unknwnargname2) __deleaker_DispObject_SetTitleText( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_DispObject_SetTitleText( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, TEXTID __unknwnargname2 );
#define GUIObject_SoftKeys_AddErrorStr(__unknwnargname1, actionID, __unknwnargname3) __deleaker_GUIObject_SoftKeys_AddErrorStr( __FILE__,  __LINE__, __unknwnargname1, actionID, __unknwnargname3 )
void __deleaker_GUIObject_SoftKeys_AddErrorStr( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 );
#define ProgressBar_SetText(__unknwnargname1, text) __deleaker_ProgressBar_SetText( __FILE__,  __LINE__, __unknwnargname1, text )
void __deleaker_ProgressBar_SetText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID text );
#define StatusIndication_Item8_SetText(__unknwnargname1) __deleaker_StatusIndication_Item8_SetText( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_StatusIndication_Item8_SetText( const char* __file__, int __line__, TEXTID __unknwnargname1 );
#define StatusIndication_SetItemText(__unknwnargname1, item, __unknwnargname3) __deleaker_StatusIndication_SetItemText( __FILE__,  __LINE__, __unknwnargname1, item, __unknwnargname3 )
void __deleaker_StatusIndication_SetItemText( const char* __file__, int __line__, GUI* __unknwnargname1, int item, TEXTID __unknwnargname3 );
#define StatusIndication_ShowNotes(__unknwnargname1) __deleaker_StatusIndication_ShowNotes( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_StatusIndication_ShowNotes( const char* __file__, int __line__, TEXTID __unknwnargname1 );
#define StringInput_DispObject_SetText(__unknwnargname1, __unknwnargname2) __deleaker_StringInput_DispObject_SetText( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_StringInput_DispObject_SetText( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, TEXTID __unknwnargname2 );
#define GUIObject_SoftKeys_SetInfoText(__unknwnargname1, actionID, __unknwnargname3) __deleaker_GUIObject_SoftKeys_SetInfoText( __FILE__,  __LINE__, __unknwnargname1, actionID, __unknwnargname3 )
void __deleaker_GUIObject_SoftKeys_SetInfoText( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 );
#define ListMenu_SetItemSecondLineText(__unknwnargname1, elem_num, __unknwnargname3) __deleaker_ListMenu_SetItemSecondLineText( __FILE__,  __LINE__, __unknwnargname1, elem_num, __unknwnargname3 )
void __deleaker_ListMenu_SetItemSecondLineText( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, int elem_num, TEXTID __unknwnargname3 );
#define GUIonMessage_SubItem_SetText(msg, __unknwnargname2) __deleaker_GUIonMessage_SubItem_SetText( __FILE__,  __LINE__, msg, __unknwnargname2 )
void __deleaker_GUIonMessage_SubItem_SetText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 );
#define Feedback_SetTextExtended(__unknwnargname1, text, where) __deleaker_Feedback_SetTextExtended( __FILE__,  __LINE__, __unknwnargname1, text, where )
void __deleaker_Feedback_SetTextExtended( const char* __file__, int __line__, GUI_FEEDBACK* __unknwnargname1, TEXTID text, int where );
#define GUIObject_SoftKeys_SetTexts(__unknwnargname1, actionID, short_text, full_text) __deleaker_GUIObject_SoftKeys_SetTexts( __FILE__,  __LINE__, __unknwnargname1, actionID, short_text, full_text )
void __deleaker_GUIObject_SoftKeys_SetTexts( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID short_text, TEXTID full_text );
#define PNUM2Name(pnum, isIconNeeded, only_full_number_flag) __deleaker_PNUM2Name( __FILE__,  __LINE__, pnum, isIconNeeded, only_full_number_flag )
TEXTID __deleaker_PNUM2Name( const char* __file__, int __line__, BCD_TEXT pnum, int isIconNeeded, int only_full_number_flag );
#define Date2ID(__unknwnargname1, DateFormat, __unknwnargname3) __deleaker_Date2ID( __FILE__,  __LINE__, __unknwnargname1, DateFormat, __unknwnargname3 )
TEXTID __deleaker_Date2ID( const char* __file__, int __line__, DATE* __unknwnargname1, int DateFormat, int __unknwnargname3 );
#define Time2ID(__unknwnargname1, TimeFormat, isSec) __deleaker_Time2ID( __FILE__,  __LINE__, __unknwnargname1, TimeFormat, isSec )
TEXTID __deleaker_Time2ID( const char* __file__, int __line__, TIME* __unknwnargname1, char TimeFormat, int isSec );
#define KeyCode2Name(key_code) __deleaker_KeyCode2Name( __FILE__,  __LINE__, key_code )
TEXTID __deleaker_KeyCode2Name( const char* __file__, int __line__, int key_code );
#define NOfMany_SetTexts(__unknwnargname1, strids, items_count) __deleaker_NOfMany_SetTexts( __FILE__,  __LINE__, __unknwnargname1, strids, items_count )
void __deleaker_NOfMany_SetTexts( const char* __file__, int __line__, GUI_NOFMANY* __unknwnargname1, TEXTID* strids, int items_count );
#define OneOfMany_SetTexts(__unknwnargname1, strids, Count) __deleaker_OneOfMany_SetTexts( __FILE__,  __LINE__, __unknwnargname1, strids, Count )
void __deleaker_OneOfMany_SetTexts( const char* __file__, int __line__, GUI_ONEOFMANY* __unknwnargname1, TEXTID* strids, int Count );
#define Feedback_SetText(__unknwnargname1, __unknwnargname2) __deleaker_Feedback_SetText( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_Feedback_SetText( const char* __file__, int __line__, GUI_FEEDBACK* __unknwnargname1, TEXTID __unknwnargname2 );
#define Gif2ID(IMAGEHANDLE, path, fname, __unknwnargname4) __deleaker_Gif2ID( __FILE__,  __LINE__, IMAGEHANDLE, path, fname, __unknwnargname4 )
int __deleaker_Gif2ID( const char* __file__, int __line__, u16 IMAGEHANDLE, const wchar_t* path, const wchar_t* fname, IMAGEID* __unknwnargname4 );
#define GUIObject_SetTitleIcon(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetTitleIcon( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetTitleIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define GUIInput_SetIcon(__unknwnargname1, __unknwnargname2) __deleaker_GUIInput_SetIcon( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIInput_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define ProgressBar_SetIcon(__unknwnargname1, __unknwnargname2) __deleaker_ProgressBar_SetIcon( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_ProgressBar_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define GUIonMessage_SetMenuItemIcon(msg, align, __unknwnargname3) __deleaker_GUIonMessage_SetMenuItemIcon( __FILE__,  __LINE__, msg, align, __unknwnargname3 )
void __deleaker_GUIonMessage_SetMenuItemIcon( const char* __file__, int __line__, GUI_MESSAGE* msg, int align, IMAGEID __unknwnargname3 );
#define TabMenuBar_SetTabIcon(__unknwnargname1, tab, __unknwnargname3, for_state) __deleaker_TabMenuBar_SetTabIcon( __FILE__,  __LINE__, __unknwnargname1, tab, __unknwnargname3, for_state )
void __deleaker_TabMenuBar_SetTabIcon( const char* __file__, int __line__, GUI_TABMENUBAR* __unknwnargname1, int tab, IMAGEID __unknwnargname3, int for_state );
#define VCALL_SetNameIcon(vc, __unknwnargname2) __deleaker_VCALL_SetNameIcon( __FILE__,  __LINE__, vc, __unknwnargname2 )
void __deleaker_VCALL_SetNameIcon( const char* __file__, int __line__, void* vc, IMAGEID __unknwnargname2 );
#define ModifyKeyHook(proc, mode, lparam) __deleaker_ModifyKeyHook( __FILE__,  __LINE__, proc, mode, lparam )
int __deleaker_ModifyKeyHook( const char* __file__, int __line__, KEYHOOKPROC proc, int mode, LPARAM lparam );
#define ModifyUIPageHook(event, proc, ClientData, mode) __deleaker_ModifyUIPageHook( __FILE__,  __LINE__, event, proc, ClientData, mode )
int __deleaker_ModifyUIPageHook( const char* __file__, int __line__, int event, PAGEHOOKPROC proc, LPARAM ClientData, int mode );
#define ImageID_Get(fpath, fname, __unknwnargname3) __deleaker_ImageID_Get( __FILE__,  __LINE__, fpath, fname, __unknwnargname3 )
int __deleaker_ImageID_Get( const char* __file__, int __line__, const wchar_t* fpath, const wchar_t* fname, IMAGEID* __unknwnargname3 );
#define ImageID_GetIndirect(buf_image, size, __NULL, image_type, __unknwnargname5) __deleaker_ImageID_GetIndirect( __FILE__,  __LINE__, buf_image, size, __NULL, image_type, __unknwnargname5 )
int __deleaker_ImageID_GetIndirect( const char* __file__, int __line__, void* buf_image, int size, int __NULL, wchar_t* image_type, IMAGEID* __unknwnargname5 );
#define ImageID_Free(__unknwnargname1) __deleaker_ImageID_Free( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_ImageID_Free( const char* __file__, int __line__, IMAGEID __unknwnargname1 );
#define GC_CreateMemoryGC(xsize, ysize, bpp, unk, somefn, unk2) __deleaker_GC_CreateMemoryGC( __FILE__,  __LINE__, xsize, ysize, bpp, unk, somefn, unk2 )
GC* __deleaker_GC_CreateMemoryGC( const char* __file__, int __line__, int xsize, int ysize, int bpp, int unk, void* somefn, int unk2 );
#define GC_FreeGC(gc) __deleaker_GC_FreeGC( __FILE__,  __LINE__, gc )
void __deleaker_GC_FreeGC( const char* __file__, int __line__, GC* gc );
#define GVI_CreateDashedPen(thikness, color, bitmask, step, offset) __deleaker_GVI_CreateDashedPen( __FILE__,  __LINE__, thikness, color, bitmask, step, offset )
GVI_PEN __deleaker_GVI_CreateDashedPen( const char* __file__, int __line__, char thikness, int color, int bitmask, int step, int offset );
#define GVI_CreatePen(thikness, color) __deleaker_GVI_CreatePen( __FILE__,  __LINE__, thikness, color )
GVI_PEN __deleaker_GVI_CreatePen( const char* __file__, int __line__, char thikness, int color );
#define GVI_CreateSolidBrush(color) __deleaker_GVI_CreateSolidBrush( __FILE__,  __LINE__, color )
GVI_BRUSH __deleaker_GVI_CreateSolidBrush( const char* __file__, int __line__, int color );
#define GVI_CreateBitmap(xsize, ysize, bpp) __deleaker_GVI_CreateBitmap( __FILE__,  __LINE__, xsize, ysize, bpp )
GVI_BMP __deleaker_GVI_CreateBitmap( const char* __file__, int __line__, int xsize, int ysize, int bpp );
#define GVI_CreateMemoryGC(bitmap) __deleaker_GVI_CreateMemoryGC( __FILE__,  __LINE__, bitmap )
GVI_GC __deleaker_GVI_CreateMemoryGC( const char* __file__, int __line__, GVI_BMP bitmap );
#define GVI_Delete_GVI_Object(__unknwnargname1) __deleaker_GVI_Delete_GVI_Object( __FILE__,  __LINE__, __unknwnargname1 )
BOOL __deleaker_GVI_Delete_GVI_Object( const char* __file__, int __line__, GVI_OBJ* __unknwnargname1 );
#define GVI_DeleteMemoryGC(gc) __deleaker_GVI_DeleteMemoryGC( __FILE__,  __LINE__, gc )
void __deleaker_GVI_DeleteMemoryGC( const char* __file__, int __line__, GVI_GC gc );
#define TextFeedbackWindow(book, zero) __deleaker_TextFeedbackWindow( __FILE__,  __LINE__, book, zero )
GUI_FEEDBACK* __deleaker_TextFeedbackWindow( const char* __file__, int __line__, BOOK* book, int zero );
#define DataBrowserDesc_Create() __deleaker_DataBrowserDesc_Create( __FILE__,  __LINE__ )
void* __deleaker_DataBrowserDesc_Create( const char* __file__, int __line__ );
#define DataBrowserDesc_Destroy(DataBrowserDesc) __deleaker_DataBrowserDesc_Destroy( __FILE__,  __LINE__, DataBrowserDesc )
void __deleaker_DataBrowserDesc_Destroy( const char* __file__, int __line__, void* DataBrowserDesc );
#define CreateMonitorFeedback(__unknwnargname1, __unknwnargname2, onbusy, onedit, ondelete) __deleaker_CreateMonitorFeedback( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2, onbusy, onedit, ondelete )
GUI_FEEDBACK* __deleaker_CreateMonitorFeedback( const char* __file__, int __line__, TEXTID __unknwnargname1, BOOK* __unknwnargname2, void (*onbusy)(BOOK*), void (*onedit)(BOOK*), void (*ondelete)(BOOK*) );
#define LoadDLL(DllName) __deleaker_LoadDLL( __FILE__,  __LINE__, DllName )
void* __deleaker_LoadDLL( const char* __file__, int __line__, wchar_t* DllName );
#define UnLoadDLL(DllData) __deleaker_UnLoadDLL( __FILE__,  __LINE__, DllData )
int __deleaker_UnLoadDLL( const char* __file__, int __line__, void* DllData );
#define GUIObject_SetSecondRowTitleText(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetSecondRowTitleText( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetSecondRowTitleText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID __unknwnargname2 );
#define ListMenu_SetNoItemText(__unknwnargname1, str) __deleaker_ListMenu_SetNoItemText( __FILE__,  __LINE__, __unknwnargname1, str )
void __deleaker_ListMenu_SetNoItemText( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, TEXTID str );
#define TabMenuBar_SetTabTitle(__unknwnargname1, tab_num, __unknwnargname3) __deleaker_TabMenuBar_SetTabTitle( __FILE__,  __LINE__, __unknwnargname1, tab_num, __unknwnargname3 )
void __deleaker_TabMenuBar_SetTabTitle( const char* __file__, int __line__, GUI_TABMENUBAR* __unknwnargname1, int tab_num, TEXTID __unknwnargname3 );
#define DispObject_SetBackgroundImage(__unknwnargname1, __unknwnargname2) __deleaker_DispObject_SetBackgroundImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_DispObject_SetBackgroundImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 );
#define DispObject_SetCursorImage(__unknwnargname1, __unknwnargname2) __deleaker_DispObject_SetCursorImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_DispObject_SetCursorImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 );
#define DispObject_SetTitleBackgroundImage(__unknwnargname1, __unknwnargname2) __deleaker_DispObject_SetTitleBackgroundImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_DispObject_SetTitleBackgroundImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 );
#define GUIObject_SetBackgroundImage(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetBackgroundImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetBackgroundImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define GUIObject_SetCursorImage(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetCursorImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetCursorImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define GUIObject_SetTitleBackgroundImage(__unknwnargname1, __unknwnargname2) __deleaker_GUIObject_SetTitleBackgroundImage( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_GUIObject_SetTitleBackgroundImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define Shortcut_Get_MenuItemName(__unknwnargname1) __deleaker_Shortcut_Get_MenuItemName( __FILE__,  __LINE__, __unknwnargname1 )
TEXTID __deleaker_Shortcut_Get_MenuItemName( const char* __file__, int __line__, void* __unknwnargname1 );
#define YesNoQuestion_SetIcon(__unknwnargname1, __unknwnargname2) __deleaker_YesNoQuestion_SetIcon( __FILE__,  __LINE__, __unknwnargname1, __unknwnargname2 )
void __deleaker_YesNoQuestion_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 );
#define ListMenu_SetItemIcon(__unknwnargname1, Item, unk_FFFF, mode, __unknwnargname5) __deleaker_ListMenu_SetItemIcon( __FILE__,  __LINE__, __unknwnargname1, Item, unk_FFFF, mode, __unknwnargname5 )
int __deleaker_ListMenu_SetItemIcon( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, int Item, wchar_t unk_FFFF, int mode, IMAGEID __unknwnargname5 );
#define Shortcut_Get_MenuItemIconID(__unknwnargname1) __deleaker_Shortcut_Get_MenuItemIconID( __FILE__,  __LINE__, __unknwnargname1 )
IMAGEID __deleaker_Shortcut_Get_MenuItemIconID( const char* __file__, int __line__, void* __unknwnargname1 );
#define create_process(proc_type, name, entrypoint, stack_size, priority, timeslice, pid_block, redir_table, vector, user) __deleaker_create_process( __FILE__,  __LINE__, proc_type, name, entrypoint, stack_size, priority, timeslice, pid_block, redir_table, vector, user )
PROCESS __deleaker_create_process( const char* __file__, int __line__, PROCESS_TYPE proc_type, char* name, OSENTRYPOINT* entrypoint, OSADDRESS stack_size, OSPRIORITY priority, OSTIME timeslice, PROCESS pid_block, void* redir_table, OSVECTOR vector, OSUSER user );
#define kill_proc(pid) __deleaker_kill_proc( __FILE__,  __LINE__, pid )
void __deleaker_kill_proc( const char* __file__, int __line__, PROCESS pid );
#define MediaPlayer_SoftKeys_AddHelpStr(player_gui, item, __unknwnargname3) __deleaker_MediaPlayer_SoftKeys_AddHelpStr( __FILE__,  __LINE__, player_gui, item, __unknwnargname3 )
void __deleaker_MediaPlayer_SoftKeys_AddHelpStr( const char* __file__, int __line__, GUI* player_gui, int item, TEXTID __unknwnargname3 );
#define MediaPlayer_SoftKeys_SetText(player_gui, actionID, __unknwnargname3) __deleaker_MediaPlayer_SoftKeys_SetText( __FILE__,  __LINE__, player_gui, actionID, __unknwnargname3 )
void __deleaker_MediaPlayer_SoftKeys_SetText( const char* __file__, int __line__, GUI* player_gui, int actionID, TEXTID __unknwnargname3 );
#define MenuBook_Desktop_GetSelectedItemID(MenuBook_Desktop) __deleaker_MenuBook_Desktop_GetSelectedItemID( __FILE__,  __LINE__, MenuBook_Desktop )
wchar_t* __deleaker_MenuBook_Desktop_GetSelectedItemID( const char* __file__, int __line__, BOOK* MenuBook_Desktop );
#define CreateURI(fpath, fname, URIScheme) __deleaker_CreateURI( __FILE__,  __LINE__, fpath, fname, URIScheme )
char* __deleaker_CreateURI( const char* __file__, int __line__, wchar_t* fpath, wchar_t* fname, char* URIScheme );
#define Timer_Kill(timerID) __deleaker_Timer_Kill( __FILE__,  __LINE__, timerID )
void __deleaker_Timer_Kill( const char* __file__, int __line__, u16* timerID );
#define Timer_Set(time, onTimer, lparam) __deleaker_Timer_Set( __FILE__,  __LINE__, time, onTimer, lparam )
u16 __deleaker_Timer_Set( const char* __file__, int __line__, int time, TIMERPROC onTimer, LPARAM lparam );
#define Timer_ReSet(timer, time, onTimer, lparam) __deleaker_Timer_ReSet( __FILE__,  __LINE__, timer, time, onTimer, lparam )
void __deleaker_Timer_ReSet( const char* __file__, int __line__, u16* timer, int time, TIMERPROC onTimer, LPARAM lparam );
#define alloc(size, signo) __deleaker_alloc( __FILE__,  __LINE__, size, signo )
union SIGNAL* __deleaker_alloc( const char* __file__, int __line__, OSBUFSIZE size, SIGSELECT signo );
#define receive(sigsel) __deleaker_receive( __FILE__,  __LINE__, sigsel )
union SIGNAL* __deleaker_receive( const char* __file__, int __line__, const SIGSELECT* sigsel );
#define receive_w_tmo(timeout, sel) __deleaker_receive_w_tmo( __FILE__,  __LINE__, timeout, sel )
union SIGNAL* __deleaker_receive_w_tmo( const char* __file__, int __line__, OSTIME timeout, SIGSELECT* sel );
#define free_buf(sig) __deleaker_free_buf( __FILE__,  __LINE__, sig )
void __deleaker_free_buf( const char* __file__, int __line__, union SIGNAL** sig );
#define send(sig, to) __deleaker_send( __FILE__,  __LINE__, sig, to )
void __deleaker_send( const char* __file__, int __line__, union SIGNAL** sig, PROCESS to );
#define hunt(name, user, name_, hunt_sig) __deleaker_hunt( __FILE__,  __LINE__, name, user, name_, hunt_sig )
OSBOOLEAN __deleaker_hunt( const char* __file__, int __line__, const char* name, OSUSER user, PROCESS* name_, union SIGNAL** hunt_sig );
#define JavaApp_LogoImageID_Get(fullpath, __unknwnargname2) __deleaker_JavaApp_LogoImageID_Get( __FILE__,  __LINE__, fullpath, __unknwnargname2 )
int __deleaker_JavaApp_LogoImageID_Get( const char* __file__, int __line__, wchar_t* fullpath, IMAGEID* __unknwnargname2 );
#define ObexSendFile(__unknwnargname1) __deleaker_ObexSendFile( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_ObexSendFile( const char* __file__, int __line__, SEND_OBEX_STRUCT* __unknwnargname1 );
#define JavaSession_GetName() __deleaker_JavaSession_GetName( __FILE__,  __LINE__ )
TEXTID __deleaker_JavaSession_GetName( const char* __file__, int __line__ );
#define MetaData_Desc_Create(path, name) __deleaker_MetaData_Desc_Create( __FILE__,  __LINE__, path, name )
void* __deleaker_MetaData_Desc_Create( const char* __file__, int __line__, wchar_t* path, wchar_t* name );
#define MetaData_Desc_Destroy(MetaData_Desc) __deleaker_MetaData_Desc_Destroy( __FILE__,  __LINE__, MetaData_Desc )
void __deleaker_MetaData_Desc_Destroy( const char* __file__, int __line__, void* MetaData_Desc );
#define FILEITEM_Create() __deleaker_FILEITEM_Create( __FILE__,  __LINE__ )
FILEITEM* __deleaker_FILEITEM_Create( const char* __file__, int __line__ );
#define FILEITEM_CreateCopy(__unknwnargname1) __deleaker_FILEITEM_CreateCopy( __FILE__,  __LINE__, __unknwnargname1 )
FILEITEM* __deleaker_FILEITEM_CreateCopy( const char* __file__, int __line__, FILEITEM* __unknwnargname1 );
#define FILEITEM_Destroy(__unknwnargname1) __deleaker_FILEITEM_Destroy( __FILE__,  __LINE__, __unknwnargname1 )
void __deleaker_FILEITEM_Destroy( const char* __file__, int __line__, FILEITEM* __unknwnargname1 );
#define w_dirclose(__unknwnargname1) __deleaker_w_dirclose( __FILE__,  __LINE__, __unknwnargname1 )
int __deleaker_w_dirclose( const char* __file__, int __line__, void* __unknwnargname1 );
#define w_diropen(dir) __deleaker_w_diropen( __FILE__,  __LINE__, dir )
void* __deleaker_w_diropen( const char* __file__, int __line__, const wchar_t* dir );
#define SoundRecorderDesc_Create() __deleaker_SoundRecorderDesc_Create( __FILE__,  __LINE__ )
void* __deleaker_SoundRecorderDesc_Create( const char* __file__, int __line__ );
#define SoundRecorderDesc_Destroy(desc) __deleaker_SoundRecorderDesc_Destroy( __FILE__,  __LINE__, desc )
void __deleaker_SoundRecorderDesc_Destroy( const char* __file__, int __line__, void* desc );
#define AB_NAME_ITEM2TextID(ab_name) __deleaker_AB_NAME_ITEM2TextID( __FILE__,  __LINE__, ab_name )
TEXTID __deleaker_AB_NAME_ITEM2TextID( const char* __file__, int __line__, AB_STR_ITEM* ab_name );
#define AB_NUM_ITEM2TextID(ab_num_item) __deleaker_AB_NUM_ITEM2TextID( __FILE__,  __LINE__, ab_num_item )
TEXTID __deleaker_AB_NUM_ITEM2TextID( const char* __file__, int __line__, AB_NUM_ITEM* ab_num_item );

#endif
#endif
