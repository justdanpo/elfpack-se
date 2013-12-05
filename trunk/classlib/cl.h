#ifndef _CL_H_
#define _CL_H_

#include "..\\include\lib_clara.h"
//#include "..\\include\types.h"

//class CDispObjBase;
class CGuiBase;
class CBookBase;
//class CGCBase;
//class CBook;
//template class CDispObjT;
//template class CGuiT;
//class CGUIMessage;
//class CStdGui;
//class CGuiListMenu;
//class CGuiOneOfMany;
//class CGuiNOfMany;
//class CGuiTabMenuBar;
//class CPaintGC;
//class CMemoryGC;




class CDispObjBase : public DISP_OBJ
{
public:
	CGuiBase* GetGUI(); //DispObject_GetGUI
	char IsVisible(); //DispObject_GetVisible
	char* GetName (); //DispObject_GetName
	DISP_DESC* GetDESC(); //DispObject_GetDESC
	int GetAbsoluteXPos(); //DispObject_GetAbsoluteXPos
	int GetAbsoluteYPos(); //DispObject_GetAbsoluteYPos
	int GetWindowHeight(); //DispObject_GetWindowHeight
	int GetWindowWidth(); //DispObject_GetWindowWidth
	void InvalidateRect( RECT* rect ); //DispObject_InvalidateRect
	void KillRefreshTimer(); //DispObject_KillRefreshTimer
	void SetLayerColor( int color ); //DispObject_SetLayerColor
	void SetRefreshTimer( int time ); //DispObject_SetRefreshTimer
	void SetTitleText( int StrID ); //DispObject_SetTitleText
	void Show( int mode ); //DispObject_Show
	void GetRect( RECT* retrect ); //DispObject_GetRect
	void WindowSetPosition( int x, int y ); //DispObject_WindowSetPosition
	void WindowSetSize( int w, int h ); //DispObject_WindowSetSize
	void SetBackgroundImage( IMAGEID imageID ); //DispObject_SetBackgroundImage
	void SetCursorImage( IMAGEID imageID ); //DispObject_SetCursorImage
	void SetTitleBackgroundImage( IMAGEID imageID ); //DispObject_SetTitleBackgroundImage
	void SetAnimation( int style ); //DispObject_SetAnimation
	void SoftKeys_ExecuteAction( u16 actionID ); //DispObject_SoftKeys_ExecuteAction
};

class CGuiBase: public GUI
{
public:
	CBookBase* GetBook(); //GUIObject_GetBook
	CDispObjBase* GetDISPObj(); //GUIObject_GetDispObject
	CGuiBase* Free(); //GUIObject_Destroy
	void SetBackgroundImage( wchar_t imageID ); //GUIObject_SetBackgroundImage
	void SetBacklightTimeout( int time ); //GUIObject_SetBacklightTimeout
	void SetCursorImage( wchar_t imageID ); //GUIObject_SetCursorImage
	void SetFocus( int i ); //GUIObject_SetFocus
	void SetStyle( int style ); //GUIObject_SetStyle
	void SetTitleIcon( wchar_t imageID ); //GUIObject_SetTitleIcon
	void SetTitleBackgroundImage( wchar_t imageID ); //GUIObject_SetTitleBackgroundImage
	void SetTitleText( TEXTID strid ); //GUIObject_SetTitleText
	void SetTitleType( int type ); //GUIObject_SetTitleType
	void Show(); //GUIObject_Show
	void SoftKeys_AddErrorStr( u16 actionID, TEXTID ); //GUIObject_SoftKeys_AddErrorStr
	void SoftKeys_ExecuteAction( u16 actionID ); //GUIObject_SoftKeys_ExecuteAction
	void SoftKeys_Hide(); //GUIObject_SoftKeys_Hide
	void SoftKeys_RemoveBackground(); //GUIObject_SoftKeys_RemoveBackground
	void SoftKeys_RemoveItem( u16 actionID ); //GUIObject_SoftKeys_RemoveItem
	void SoftKeys_RestoreBackground(); //GUIObject_SoftKeys_RestoreBackground
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*) ); //GUIObject_SoftKeys_SetAction
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID short_text, TEXTID full_text );
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID strid );
	void SoftKeys_SetEnable( u16 actionID, BOOL enabled ); //GUIObject_SoftKeys_SetEnable
	void SoftKeys_SetInfoText( u16 actionID, TEXTID ); //GUIObject_SoftKeys_SetInfoText
	void SoftKeys_SetItemAsSubItem( u16 actionID, int subitem ); //GUIObject_SoftKeys_SetItemAsSubItem
	void SoftKeys_SetText( u16 actionID, TEXTID ); //GUIObject_SoftKeys_SetText
	void SoftKeys_SetTexts( u16 actionID, TEXTID short_text, TEXTID full_text ); //GUIObject_SoftKeys_SetTexts
	void SoftKeys_SetVisible( u16 actionID, BOOL visible ); //GUIObject_SoftKeys_SetVisible
	void SoftKeys_Show(); //GUIObject_SoftKeys_Show
	void SoftKeys_SuppressDefaultAction( u16 actionID ); //GUIObject_SoftKeys_SuppressDefaultAction
	void SetZOrder( char, CGuiBase* ); //GUIObject_SetZOrder
};

class CBookBase : public BOOK
{
public:
	int GetBookID(); //BookObj_GetBookID
	int GetDisplayOrientation(); //BookObj_GetDisplayOrientation
	int GetSessionID(); //BookObj_GetSessionID
	UI_APP_SESSION* GetSession(); //BookObj_GetSession
	void AddGUIObject( CGuiBase* gui ); //BookObj_AddGUIObject
	void CallPage( const PAGE_DESC* page ); //BookObj_CallPage
	void Free(); //FreeBook
	void GotoPage( const PAGE_DESC* page ); //BookObj_GotoPage
	void Hide( int display ); //BookObj_Hide
	void ReturnPage( int event ); //BookObj_ReturnPage
	void SetDisplayOrientation( int orientation ); //BookObj_SetDisplayOrientation
	void Show( int display ); //BookObj_Show
	void UI_Event( int event ); //UI_Event_toBookID
	void UI_Event_wData( int event, void* message, void (*mfree)(void*) ); //UI_Event_toBookIDwData
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*) ); //BookObj_SoftKeys_SetAction
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID short_text, TEXTID full_text );
	void SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID strid );
	void SoftKeys_SetText( u16 actionID, TEXTID ); //BookObj_SoftKeys_SetText
	void SetFocus( int display ); //BookObj_SetFocus
	void WindowSetWantsFocus( int display, BOOL WantsFocus ); //BookObj_WindowSetWantsFocus
	void StayOnTop( int mode ); //BookObj_StayOnTop
	void Kill(); //BookObj_KillBook
};

/////////////////////////////////////////////////////////////////////////

class CGCBase
{
protected:
	GC* gc;
	int gc_xx_old;
	CGCBase();
	~CGCBase();
public:
	int DrawRawBitmap( int x1, int y1, int x2, int y2, int* bmp ); //GC_WritePixels
	int GetBrushColor(); //GC_GetBrushColor
	int GetPenColor(); //GC_GetPenColor
	int GetXX(); //GC_GetXX
	void DrawFRect ( int color, int x1, int y1, int x2, int y2 ); //GC_DrawFRect
	void DrawLine( int x1, int y1, int x2, int y2 ); //GC_DrawLine
	void DrawRoundRect( RECT*, int arcWidth, int arcHeight, int border_flag, int fill_flag ); //GC_DrawRoundRect
	void GetRect( RECT* ); //GC_GetRect
	void SetBrushColor( int brush_color ); //GC_SetBrushColor
	void SetPenColor( int pen_color ); //GC_SetPenColor
	void SetPixel ( int x1, int y1, int color ); //GC_SetPixel
	void SetXX( int ); //GC_SetXX
	void ValidateRect( RECT* ); //GC_ValidateRect
};


/////////////////////////////////////////////////////////////////////////

class CBook : public CBookBase
{
	static void __onClose( CBook* bk );
protected:
	virtual ~CBook(); //не удаляем объект напрямую
public:
	//CreateBook
	CBook( char* pBookName, const PAGE_DESC* bp );
};

/////////////////////////////////////////////////////////////////////////

template <class T>
class CDispObjT : public CDispObjBase
{
private:
	CDispObjT();
	~CDispObjT();
	static void __onKey( T* db, int key, int a3, int repeat, int type )
	{
		db->onKey( key, a3, repeat, type );
	}
	static void __onDraw( T* db, int a, int b, int c )
	{
		db->onDraw( a, b, c );
	}
	static int __onCreate( T* db )
	{
		return db->onCreate();
	}
	static void __onDestroy( T* db )
	{
		db->onDestroy();
	}
	static void __onRefresh( T* db )
	{
		db->onRefresh();
	}
	static void __onLayout( T* db, void* layoutstruct )
	{
		db->onLayout( layoutstruct );
	}
	static void __onConfig( T* db, int unk, int DisplayWidth, int DisplayHeight )
	{
		db->onConfig( unk, DisplayWidth, DisplayHeight );
	}
public:
	static void DispDescCreate( DISP_DESC* desc )
	{
		DISP_DESC_SetSize( desc, sizeof(T) );
		DISP_DESC_SetName( desc, T::getName() );
		DISP_DESC_SetOnCreate( desc, (DISP_OBJ_ONCREATE_METHOD)__onCreate );
		DISP_DESC_SetOnClose( desc, (DISP_OBJ_ONCLOSE_METHOD)__onDestroy );
		DISP_DESC_SetOnRedraw( desc, (DISP_OBJ_ONREDRAW_METHOD)__onDraw );
		DISP_DESC_SetOnKey( desc, (DISP_OBJ_ONKEY_METHOD)__onKey );
		DISP_DESC_SetOnRefresh( desc, (DISP_OBJ_METHOD)__onRefresh );
		DISP_DESC_SetOnLayout( desc, (DISP_OBJ_ONLAYOUT_METHOD)__onLayout );
		DISP_DESC_SetOnConfig( desc, (DISP_OBJ_ONCONFIG_METHOD)__onConfig );
	}

	static char* getName();
	int onCreate()
	{
		return 1;
	}
	void onKey( int key, int, int repeat, int type ){}
	void onDraw( int a, int b, int c ){}
	void onDestroy(){}
	void onRefresh(){}
	void onLayout( void* layoutstruct ){}
	void onConfig( int unk, int DisplayWidth, int DisplayHeight ){}
};

/////////////////////////////////////////////////////////////////////////

template<class T>
class CGuiT: public CGuiBase
{
	explicit CGuiT();
	static void __onclose( CGuiT<T>* gui )
	{
		gui-> ~CGuiT();
	}
protected:
	virtual ~CGuiT()
	{
	}
public:
	//GUIObject_Create
	CGuiT( CBook* book, int display )
	{
		GUIObject_Create( this, (void(*)(GUI*))__onclose, (void(*)(DISP_DESC*))T::DispDescCreate, book,
						 0, display, sizeof(CGuiBase) );

		if( book )
			book->AddGUIObject( this );
	}
};

/////////////////////////////////////////////////////////////////////////

class CGUIMessage : public GUI_MESSAGE
{
public:
	CBook* GetBook(); //GUIonMessage_GetBook
	CGuiBase* GetGui(); //GUIonMessage_GetGui
	int GetMsg(); //GUIonMessage_GetMsg
	int GetCreatedItem(); //GUIonMessage_GetCreatedItemIndex
	int GetSelectedItem(); //GUIonMessage_GetSelectedItem
	int GetPrevSelectedItem(); //GUIonMessage_GetPrevSelectedItem
	char SetItemText( TEXTID strid ); //GUIonMessage_SetMenuItemText
	char SetItemSecondLineText( TEXTID strid ); //GUIonMessage_SetMenuItemSecondLineText
	char SetItemInfoText( TEXTID strid ); //GUIonMessage_SetMenuItemInfoText
	char SetItemUnavailableText( TEXTID strid ); //GUIonMessage_SetMenuItemUnavailableText
	void SubItem_SetText( TEXTID strid ); //GUIonMessage_SubItem_SetText
	int SubItem_GetCreatedParentIndex(); //GUIonMessage_SubItem_GetCreatedParentIndex
	int SubItem_GetCreatedIndex(); //GUIonMessage_SubItem_GetCreatedIndex
	int SubItem_GetSelectedParentIndex(); //GUIonMessage_SubItem_GetSelectedParentIndex
	void SetItemIcon( int align, wchar_t iconID ); //GUIonMessage_SetMenuItemIcon
	void SetNumberOfSubItems( int unk, int n_sub_items ); //GUIonMessage_SetNumberOfSubItems
	void SetItemDisabled( BOOL disabled ); //GUIonMessage_SetItemDisabled
	void SetLineSeparator( int _unk1 ); //GUIonMessage_SetLineSeparator
	wchar_t* GetSearchString(); //GUIonMessage_GetSearchString
	int GetSearchStringLength(); //GUIonMessage_GetSearchStringLength
	int SubItem_GetSelectedIndex(); //GUIonMessage_SubItem_GetSelectedIndex
	void SubItem_SetDisabled( BOOL state ); //GUIonMessage_SubItem_SetDisabled
};

/////////////////////////////////////////////////////////////////////////

class CStdGui : public CGuiBase
{
	void (*oldonclose)(GUI*);
	explicit CStdGui();
	static void __onclose( CStdGui* disp );
protected:
	virtual ~CStdGui();
public:
	CStdGui( CBook* book, int display, void(*onguidestroy)(GUI*), void(*ondesccreate)(DISP_DESC*),
			void (*ondispmessage)(DISP_OBJ*, void*, GUI*) );
};

/////////////////////////////////////////////////////////////////////////

class CGuiListMenu : public CStdGui
{
protected:
	static int __onmessage( CGUIMessage* msg );
public:
	//ListMenu_SetOnMessage
	CGuiListMenu( CBook* book, int display = 0 );
	virtual int OnMessage( CGUIMessage* msg );

	void SetItemCount( int count ); //ListMenu_SetItemCount
	int GetSelectedItem(); //ListMenu_GetSelectedItem
	void SetFocused( int item ); //ListMenu_SetCursorToItem
	void SetNoItemText( TEXTID ); //ListMenu_SetNoItemText
	void DestroyItems(); //ListMenu_DestroyItems
	int GetItemCount(); //ListMenu_GetItemCount
	void SetItemStyle( int style ); //ListMenu_SetItemStyle
};

/////////////////////////////////////////////////////////////////////////

class CGuiOneOfMany : public CStdGui
{
protected:
	static int __onmessage( CGUIMessage* msg );
public:
	//OneOfMany_SetOnMessage
	CGuiOneOfMany( CBook* book, int display = 0 );
	virtual int OnMessage( CGUIMessage* msg );

	void SetItemCount( int count ); //OneOfMany_SetItemCount
	int GetSelectedItem(); //OneOfMany_GetSelected
	void SetFocused( int item ); //OneOfMany_SetFocused
	void SetChecked( int checked ); //OneOfMany_SetChecked
	void SetTexts( TEXTID* strids, int count ); //OneOfMany_SetTexts
};

/////////////////////////////////////////////////////////////////////////

class CGuiNOfMany : public CStdGui
{
protected:
	static int __onmessage( CGUIMessage* msg );
public:
	//NOfMany_SetOnMessage
	CGuiNOfMany( CBook* book, int display = 0 );
	virtual int OnMessage( CGUIMessage* msg );

	void SetItemCount( int count ); //ListMenu_SetItemCount
	int GetChecked( u16* buffer ); //NOfMany_GetChecked
	int GetCheckedCount(); //NOfMany_GetCheckedCount
	void SetFocused( int item ); //NOfMany_SetCursor
	void SetChecked( u16* checked_table, int items_count ); //NOfMany_SetChecked
	void SetTexts( TEXTID* strids, int count ); //NOfMany_SetTexts
};

/////////////////////////////////////////////////////////////////////////

class CGuiTabMenuBar : public CStdGui
{
public:
	CGuiTabMenuBar( CBook* book, int display = 0 );
	int GetFocusedTab(); //TabMenuBar_GetFocusedTab
	void SetTabCount( int count ); //TabMenuBar_SetTabCount
	void SetFocusedTab( int tab ); //TabMenuBar_SetFocusedTab
	void SetTabGui( int tab, CGuiBase* gui ); //TabMenuBar_SetTabGui
	void SetTabIcon( int tab, int icon, int state ); //TabMenuBar_SetTabIcon
	void SetTabTitle( int tab, TEXTID strid ); //TabMenuBar_SetTabTitle
};

/////////////////////////////////////////////////////////////////////////

class CPaintGC : public CGCBase
{
public:
	CPaintGC();
	~CPaintGC();
};

class CMemoryGC : public CGCBase
{
public:
	CMemoryGC( int xsize, int ysize, int bpp );
	~CMemoryGC();
};

/////////////////////////////////////////////////////////////////////////

#define DECLARE_PAGE_DESC_MEMBER( name ) static const PAGE_DESC name; static const PAGE_MSG name##__PAGE_MSG__[]

#define DECLARE_PAGE_DESC_BEGIN( objname, name ) \
	const PAGE_DESC objname = {name, 0, objname##__PAGE_MSG__}; \
		const PAGE_MSG objname##__PAGE_MSG__[]@ "DYN_PAGE" = {

#define DECLARE_PAGE_DESC_END NIL_EVENT_TAG, 0 };

#define DECLARE_PAGE_DESC_MSG( evt, func ) evt, (int(*)(void*, BOOK*))func,

/////////////////////////////////////////////////////////////////////////

#endif
