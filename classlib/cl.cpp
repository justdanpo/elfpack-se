#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"

#include "cl.h"

////////////////////////////////////////////////////////////////////////

void CDispObjBase::GetRect( RECT* retrect )
{
	DispObject_GetRect( this, retrect );
}

void CDispObjBase::InvalidateRect( RECT* rect )
{
	DispObject_InvalidateRect( this, rect );
}

char* CDispObjBase::GetName()
{
	return DispObject_GetName( this );
}

char CDispObjBase::IsVisible()
{
	return DispObject_GetVisible( this );
}

DISP_DESC* CDispObjBase::GetDESC()
{
	return DispObject_GetDESC( this );
}

int CDispObjBase::GetAbsoluteXPos()
{
	return DispObject_GetAbsoluteXPos( this );
}

int CDispObjBase::GetAbsoluteYPos()
{
	return DispObject_GetAbsoluteYPos( this );
}

int CDispObjBase::GetWindowHeight()
{
	return DispObject_GetWindowHeight( this );
}

int CDispObjBase::GetWindowWidth()
{
	return DispObject_GetWindowWidth( this );
}

void CDispObjBase::KillRefreshTimer()
{
	DispObject_KillRefreshTimer( this );
}

void CDispObjBase::SetRefreshTimer( int time )
{
	DispObject_SetRefreshTimer( this, time );
}

void CDispObjBase::SetLayerColor( int color )
{
	DispObject_SetLayerColor( this, color );
}

CGuiBase* CDispObjBase::GetGUI()
{
	return (CGuiBase*)DispObject_GetGUI( this );
}

void CDispObjBase::Show( int mode )
{
	DispObject_Show( this, mode );
}

void CDispObjBase::SetTitleText( TEXTID strid )
{
	DispObject_SetTitleText( this, strid );
}

void CDispObjBase::WindowSetPosition( int x, int y )
{
	DispObject_WindowSetPosition( this, x, y );
}

void CDispObjBase::WindowSetSize( int w, int h )
{
	DispObject_WindowSetSize( this, w, h );
}

void CDispObjBase::SetBackgroundImage( IMAGEID imageID )
{
	DispObject_SetBackgroundImage( this, imageID );
}

void CDispObjBase::SetCursorImage( IMAGEID imageID )
{
	DispObject_SetCursorImage( this, imageID );
}

void CDispObjBase::SetTitleBackgroundImage( IMAGEID imageID )
{
	DispObject_SetTitleBackgroundImage( this, imageID );
}

void CDispObjBase::SetAnimation( int style )
{
	DispObject_SetAnimation( this, style );
}

void CDispObjBase::SoftKeys_ExecuteAction( u16 actionID )
{
	DispObject_SoftKeys_ExecuteAction( this, actionID );
}

////////////////////////////////////////////////////////////////////////

CBookBase* CGuiBase::GetBook()
{
	return (CBookBase*) GUIObject_GetBook( this );
}

CDispObjBase* CGuiBase::GetDISPObj()
{
	return (CDispObjBase*)GUIObject_GetDispObject( this );
}

CGuiBase* CGuiBase::Free()
{
	return (CGuiBase*)GUIObject_Destroy( this );
}

void CGuiBase::SoftKeys_Hide()
{
	GUIObject_SoftKeys_Hide( this );
}

void CGuiBase::SetFocus( int i )
{
	GUIObject_SetFocus( this, i );
}

void CGuiBase::SetTitleText( int StrID )
{
	GUIObject_SetTitleText( this, StrID );
}

void CGuiBase::SetStyle( int style )
{
	GUIObject_SetStyle( this, style );
}

void CGuiBase::SetTitleType( int type )
{
	GUIObject_SetTitleType( this, type );
}

void CGuiBase::Show()
{
	GUIObject_Show( this );
}

void CGuiBase::SoftKeys_Show()
{
	GUIObject_SoftKeys_Show( this );
}

void CGuiBase::SetBackgroundImage( wchar_t imageID )
{
	GUIObject_SetBackgroundImage( this, imageID );
}

void CGuiBase::SetCursorImage( wchar_t imageID )
{
	GUIObject_SetCursorImage( this, imageID );
}

void CGuiBase::SetTitleIcon( wchar_t imageID )
{
	GUIObject_SetTitleIcon( this, imageID );
}

void CGuiBase::SetTitleBackgroundImage( wchar_t imageID )
{
	GUIObject_SetTitleBackgroundImage( this, imageID );
}

void CGuiBase::SoftKeys_AddErrorStr( u16 actionID, TEXTID strid )
{
	GUIObject_SoftKeys_AddErrorStr( this, actionID, strid );
}

void CGuiBase::SoftKeys_ExecuteAction( u16 actionID )
{
	GUIObject_SoftKeys_ExecuteAction( this, actionID );
}

void CGuiBase::SoftKeys_RemoveItem( u16 actionID )
{
	GUIObject_SoftKeys_RemoveItem( this, actionID );
}

void CGuiBase::SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*) )
{
	GUIObject_SoftKeys_SetAction( this, actionID, (void(*)(BOOK*, GUI*))proc );
}

void CGuiBase::SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID strid )
{
	GUIObject_SoftKeys_SetAction( this, actionID, (void(*)(BOOK*, GUI*))proc );
	GUIObject_SoftKeys_SetText( this, actionID, strid );
}

void CGuiBase::SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID short_text, TEXTID full_text )
{
	GUIObject_SoftKeys_SetAction( this, actionID, (void(*)(BOOK*, GUI*))proc );
	GUIObject_SoftKeys_SetTexts( this, actionID, short_text, full_text );
}

void CGuiBase::SoftKeys_SetEnable( u16 actionID, BOOL enabled )
{
	GUIObject_SoftKeys_SetEnable( this, actionID, enabled );
}

void CGuiBase::SoftKeys_SetInfoText( u16 actionID, TEXTID strid )
{
	GUIObject_SoftKeys_SetInfoText( this, actionID, strid );
}

void CGuiBase::SoftKeys_SetItemAsSubItem( u16 actionID, int subitem )
{
	GUIObject_SoftKeys_SetItemAsSubItem( this, actionID, subitem );
}

void CGuiBase::SoftKeys_SetText( u16 actionID, TEXTID strid )
{
	GUIObject_SoftKeys_SetText( this, actionID, strid );
}

void CGuiBase::SoftKeys_SetTexts( u16 actionID, TEXTID short_text, TEXTID full_text )
{
	GUIObject_SoftKeys_SetTexts( this, actionID, short_text, full_text );
}

void CGuiBase::SoftKeys_SetVisible( u16 actionID, BOOL visible )
{
	GUIObject_SoftKeys_SetVisible( this, actionID, visible );
}

void CGuiBase::SoftKeys_SuppressDefaultAction( u16 actionID )
{
	GUIObject_SoftKeys_SuppressDefaultAction( this, actionID );
}

void CGuiBase::SoftKeys_RemoveBackground()
{
	GUIObject_SoftKeys_RemoveBackground( this );
}

void CGuiBase::SoftKeys_RestoreBackground()
{
	GUIObject_SoftKeys_RestoreBackground( this );
}

void CGuiBase::SetBacklightTimeout( int time )
{
	GUIObject_SetBacklightTimeout( this, time );
}

void CGuiBase::SetZOrder( char ordertype, CGuiBase* anothergui )
{
	GUIObject_SetZOrder( this, ordertype, anothergui );
}
////////////////////////////////////////////////////////////////////////

void CBookBase::AddGUIObject( CGuiBase* gui )
{
	BookObj_AddGUIObject( this, gui );
}

void CBookBase::Free()
{
	FreeBook( this );
}

void CBookBase::Hide( int display )
{
	BookObj_Hide( this, display );
}

void CBookBase::Show( int display )
{
	BookObj_Show( this, display );
}

int CBookBase::GetBookID()
{
	return BookObj_GetBookID( this );
}

UI_APP_SESSION* CBookBase::GetSession()
{
	return BookObj_GetSession( this );
}

int CBookBase::GetSessionID()
{
	return BookObj_GetSessionID( this );
}

int CBookBase::GetDisplayOrientation()
{
	return BookObj_GetDisplayOrientation( this );
}

void CBookBase::SetDisplayOrientation( int orientation )
{
	BookObj_SetDisplayOrientation( this, orientation );
}

void CBookBase::UI_Event( int event )
{
	UI_Event_toBookID( event, GetBookID() );
}
void CBookBase::UI_Event_wData( int event, void* message, void (*mfree)(void*) )
{
	UI_Event_toBookIDwData( event, GetBookID(), message, mfree );
}

void CBookBase::GotoPage( const PAGE_DESC* page )
{
	BookObj_GotoPage( this, page );
}

void CBookBase::ReturnPage( int event )
{
	BookObj_ReturnPage( this, event );
}

void CBookBase::CallPage( const PAGE_DESC* page )
{
	BookObj_CallPage( this, page );
}

void CBookBase::SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*) )
{
	BookObj_SoftKeys_SetAction( this, actionID, (void(*)(BOOK*, GUI*))proc );
}

void CBookBase::SoftKeys_SetAction( u16 actionID, void (*proc)(CBookBase*, CGuiBase*), TEXTID strid )
{
	BookObj_SoftKeys_SetAction( this, actionID, (void(*)(BOOK*, GUI*))proc );
	BookObj_SoftKeys_SetText( this, actionID, strid );
}

void CBookBase::SoftKeys_SetText( u16 actionID, TEXTID strid )
{
	BookObj_SoftKeys_SetText( this, actionID, strid );
}

void CBookBase::SetFocus( int display )
{
	BookObj_SetFocus( this, display );
}

void CBookBase::WindowSetWantsFocus( int display, BOOL WantsFocus )
{
	BookObj_WindowSetWantsFocus( this, display, WantsFocus );
}

void CBookBase::StayOnTop( int mode )
{
	BookObj_StayOnTop( this, mode );
}

void CBookBase::Kill()
{
	BookObj_KillBook( this );
}
////////////////////////////////////////////////////////////////////////

CGCBase::CGCBase()
{
}

CGCBase::~CGCBase()
{
}

int CGCBase::DrawRawBitmap( int x1, int y1, int x2, int y2, int* bmp )
{
	return GC_WritePixels( gc, x1, y1, x2, y2, bmp );
}

int CGCBase::GetBrushColor()
{
	return GC_GetBrushColor( gc );
}

int CGCBase::GetPenColor()
{
	return GC_GetPenColor( gc );
}

int CGCBase::GetXX()
{
	return GC_GetXX( gc );
}

void CGCBase::SetXX( int arg )
{
	GC_SetXX( gc, arg );
}

void CGCBase::DrawFRect( int color, int x1, int y1, int x2, int y2 )
{
	GC_DrawFRect( gc, color, x1, y1, x2, y2 );
}

void CGCBase::DrawLine( int x1, int y1, int x2, int y2 )
{
	GC_DrawLine( gc, x1, y1, x2, y2 );
}

void CGCBase::DrawRoundRect( RECT* rect, int arcWidth, int arcHeight, int border_flag, int fill_flag )
{
	GC_DrawRoundRect( gc, rect, arcWidth, arcHeight, border_flag, fill_flag );
}

void CGCBase::SetBrushColor( int brush_color )
{
	GC_SetBrushColor( gc, brush_color );
}

void CGCBase::SetPenColor( int pen_color )
{
	GC_SetPenColor( gc, pen_color );
}

void CGCBase::SetPixel( int x1, int y1, int color )
{
	GC_SetPixel( gc, x1, y1, color );
}

void CGCBase::ValidateRect( RECT* rect )
{
	GC_ValidateRect( gc, rect );
}

void CGCBase::GetRect( RECT* rect )
{
	GC_GetRect( gc, rect );
}

////////////////////////////////////////////////////////////////////////

CBook::CBook( char* pBookName, const PAGE_DESC* bp )
{
	CreateBook( this, (void(*)(BOOK*)) &__onClose, bp, pBookName, -1, 0 );
}

CBook::~CBook()
{
}

void CBook::__onClose( CBook* bk )
{
	bk->~CBook();
}

/////////////////////////////////////////////////////////////////////////

CGuiBase* CGUIMessage::GetGui()
{
	return (CGuiBase*)GUIonMessage_GetGui( this );
}

CBook* CGUIMessage::GetBook()
{
	return (CBook*)GUIonMessage_GetBook( this );
}

int CGUIMessage::GetMsg()
{
	return GUIonMessage_GetMsg( this );
}

int CGUIMessage::GetCreatedItem()
{
	return GUIonMessage_GetCreatedItemIndex( this );
}

int CGUIMessage::GetSelectedItem()
{
	return GUIonMessage_GetSelectedItem( this );
}

int CGUIMessage::GetPrevSelectedItem()
{
	return GUIonMessage_GetPrevSelectedItem( this );
}

char CGUIMessage::SetItemText( TEXTID strid )
{
	return GUIonMessage_SetMenuItemText( this, strid );
}

char CGUIMessage::SetItemSecondLineText( TEXTID strid )
{
	return GUIonMessage_SetMenuItemSecondLineText( this, strid );
}

char CGUIMessage::SetItemInfoText( TEXTID strid )
{
	return GUIonMessage_SetMenuItemInfoText( this, strid );
}

char CGUIMessage::SetItemUnavailableText( TEXTID strid )
{
	return GUIonMessage_SetMenuItemUnavailableText( this, strid );
}

void CGUIMessage::SubItem_SetText( TEXTID strid )
{
	GUIonMessage_SubItem_SetText( this, strid );
}

int CGUIMessage::SubItem_GetCreatedParentIndex()
{
	return GUIonMessage_SubItem_GetCreatedParentIndex( this );
}

int CGUIMessage::SubItem_GetCreatedIndex()
{
	return GUIonMessage_SubItem_GetCreatedIndex( this );
}

int CGUIMessage::SubItem_GetSelectedParentIndex()
{
	return GUIonMessage_SubItem_GetSelectedParentIndex( this );
}

void CGUIMessage::SetItemIcon( int align, wchar_t iconID )
{
	GUIonMessage_SetMenuItemIcon( this, align, iconID );
}

void CGUIMessage::SetNumberOfSubItems( int unk, int n_sub_items )
{
	GUIonMessage_SetNumberOfSubItems( this, unk, n_sub_items );
}

void CGUIMessage::SetItemDisabled( BOOL disabled )
{
	GUIonMessage_SetItemDisabled( this, disabled );
}

void CGUIMessage::SetLineSeparator( int _unk1 )
{
	GUIonMessage_SetLineSeparator( this, _unk1 );
}

wchar_t* CGUIMessage::GetSearchString()
{
	return GUIonMessage_GetSearchString( this );
}

int CGUIMessage::GetSearchStringLength()
{
	return GUIonMessage_GetSearchStringLength( this );
}

int CGUIMessage::SubItem_GetSelectedIndex()
{
	return GUIonMessage_SubItem_GetSelectedIndex( this );
}

void CGUIMessage::SubItem_SetDisabled( BOOL state )
{
	GUIonMessage_SubItem_SetDisabled( this, state );
}

////////////////////////////////////////////////////////////////////////

CStdGui::CStdGui( CBook* book, int display, void(*onguidestroy)(GUI*), void(*ondesccreate)(DISP_DESC*),
				 void (*ondispmessage)(DISP_OBJ*, void*, GUI*) )
{
	GUIObject_Create( this, (void(*)(GUI*))__onclose, ondesccreate, book,
					 ondispmessage, display, sizeof(CGuiBase) );
	oldonclose = onguidestroy;
	if( book )
		book->AddGUIObject( this );
}

void CStdGui::__onclose( CStdGui* gui )
{
	gui->~CStdGui();
	gui->oldonclose( gui );
}

CStdGui::~CStdGui()
{
}


////////////////////////////////////////////////////////////////////////

int CGuiListMenu::OnMessage( CGUIMessage* msg )
{
	return 0;
}

int CGuiListMenu::__onmessage( CGUIMessage* msg )
{
	return static_cast<CGuiListMenu*>( msg->GetGui() ) -> OnMessage( msg );
}

CGuiListMenu::CGuiListMenu( CBook* book, int display )
: CStdGui( book, display, getListMenuOnGuiDestroy(), getListMenuOnDescCreate(), getListMenuOnDispMessage() )
{
	ListMenu_SetOnMessage( this, (int(*)(GUI_MESSAGE*))__onmessage );
}

void CGuiListMenu::SetItemCount( int count )
{
	ListMenu_SetItemCount( this, count );
}

int CGuiListMenu::GetSelectedItem()
{
	return ListMenu_GetSelectedItem( this );
}

void CGuiListMenu::SetFocused( int item )
{
	ListMenu_SetCursorToItem( this, item );
}

void CGuiListMenu::SetNoItemText( TEXTID strid )
{
	ListMenu_SetNoItemText( this, strid );
}

void CGuiListMenu::DestroyItems()
{
	ListMenu_DestroyItems( this );
}

int CGuiListMenu::GetItemCount()
{
	return ListMenu_GetItemCount( this );
}

void CGuiListMenu::SetItemStyle( int style )
{
	ListMenu_SetItemStyle( this, style );
}

////////////////////////////////////////////////////////////////////////

int CGuiOneOfMany::OnMessage( CGUIMessage* msg )
{
	return 0;
}

int CGuiOneOfMany::__onmessage( CGUIMessage* msg )
{
	return static_cast<CGuiOneOfMany*>(msg->GetGui()) -> OnMessage( msg );
}

CGuiOneOfMany::CGuiOneOfMany( CBook* book, int display )
: CStdGui( book, display, getOneOfManyOnGuiDestroy(), getOneOfManyOnDescCreate(), getOneOfManyOnDispMessage() )
{
	OneOfMany_SetOnMessage( this, (int(*)(GUI_MESSAGE*))__onmessage );
}

void CGuiOneOfMany::SetItemCount( int count )
{
	OneOfMany_SetItemCount( this, count );
}

int CGuiOneOfMany::GetSelectedItem()
{
	return OneOfMany_GetSelected( this );
}

void CGuiOneOfMany::SetFocused( int item )
{
	OneOfMany_SetFocused( this, item );
}

void CGuiOneOfMany::SetChecked( int checked )
{
	OneOfMany_SetChecked( this, checked );
}

void CGuiOneOfMany::SetTexts( TEXTID* strids, int count )
{
	OneOfMany_SetTexts( this, strids, count );
}

////////////////////////////////////////////////////////////////////////

int CGuiNOfMany::OnMessage( CGUIMessage* msg )
{
	return 0;
}

int CGuiNOfMany::__onmessage( CGUIMessage* msg )
{
	return static_cast<CGuiNOfMany*>(msg->GetGui()) -> OnMessage( msg );
}

CGuiNOfMany::CGuiNOfMany( CBook* book, int display )
: CStdGui( book, display, getNOfManyOnGuiDestroy(), getNOfManyOnDescCreate(), getNOfManyOnDispMessage() )
{
	NOfMany_SetOnMessage( this, (int(*)(GUI_MESSAGE*))__onmessage );
}

void CGuiNOfMany::SetItemCount( int count )
{
	ListMenu_SetItemCount( this, count );
}

int CGuiNOfMany::GetChecked( u16* buffer )
{
	return NOfMany_GetChecked( this, buffer );
}

int CGuiNOfMany::GetCheckedCount()
{
	return NOfMany_GetCheckedCount( this );
}

void CGuiNOfMany::SetFocused( int item )
{
	NOfMany_SetCursor( this, item );
}

void CGuiNOfMany::SetChecked( u16* checked_table, int items_count )
{
	NOfMany_SetChecked( this, checked_table, items_count );
}

void CGuiNOfMany::SetTexts( TEXTID* strids, int count )
{
	NOfMany_SetTexts( this, strids, count );
}

////////////////////////////////////////////////////////////////////////

CGuiTabMenuBar::CGuiTabMenuBar( CBook* book, int display )
: CStdGui( book, display, getTabMenuBarOnGuiDestroy(), getTabMenuBarOnDescCreate(), getTabMenuBarOnDispMessage() )
{
}

int CGuiTabMenuBar::GetFocusedTab()
{
	return TabMenuBar_GetFocusedTab( this );
}

void CGuiTabMenuBar::SetTabCount( int count )
{
	TabMenuBar_SetTabCount( this, count );
}

void CGuiTabMenuBar::SetFocusedTab( int tab )
{
	TabMenuBar_SetFocusedTab( this, tab );
}

void CGuiTabMenuBar::SetTabGui( int tab, CGuiBase* gui )
{
	TabMenuBar_SetTabGui( this, tab, gui );
}

void CGuiTabMenuBar::SetTabIcon( int tab, int icon, int state )
{
	TabMenuBar_SetTabIcon( this, tab, icon, state );
}

void CGuiTabMenuBar::SetTabTitle( int tab, TEXTID strid )
{
	TabMenuBar_SetTabTitle( this, tab, strid );
}

////////////////////////////////////////////////////////////////////////

CPaintGC::CPaintGC()
{
	gc = get_DisplayGC();
	gc_xx_old = GetXX();
	SetXX( 1 );
}

CPaintGC::~CPaintGC()
{
	SetXX( gc_xx_old );
}

/////////////////////////////////////////////////////////////////////////

CMemoryGC::CMemoryGC( int xsize, int ysize, int bpp )
{
	gc = GC_CreateMemoryGC( xsize, ysize, bpp, 0, 0, 0 );
	gc_xx_old = GetXX();
	SetXX( 1 );
}

CMemoryGC::~CMemoryGC()
{
	SetXX( gc_xx_old );
	GC_FreeGC( gc );
}

/////////////////////////////////////////////////////////////////////////
