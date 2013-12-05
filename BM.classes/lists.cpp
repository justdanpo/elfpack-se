#include "..\deleaker\mem2.h"
#include "lists.hpp"
#include "daemon.hpp"
#include "config_data.h"
#include "..\classlib\strutils.hpp"

int CGuiCommonList::OnMessage( CGUIMessage* msg )
{
	switch( msg->GetMsg() )
	{
	case LISTMSG_GetItem:
		msg->SetItemText( tostrid( listdata[ msg->GetCreatedItem() ].name ) );
		msg->SetItemDisabled( ! listdata[ msg->GetCreatedItem() ].isenabled );
		break;
	}
	return 1;
}

void CGuiCommonList::__OnBack( CBookBase* book, CGuiBase* gui )
{
	((CGuiCommonList*)gui)->OnBack( book );
}

void CGuiCommonList::__OnLongBack( CBookBase* book, CGuiBase* gui )
{
	((CGuiCommonList*)gui)->OnLongBack( book );
}

void CGuiCommonList::__OnDelete( CBookBase* book, CGuiBase* gui )
{
	((CGuiCommonList*)gui)->OnDelete( book );
}

void CGuiCommonList::__OnSelect( CBookBase* book, CGuiBase* gui )
{
	((CGuiCommonList*)gui)->OnSelect( book );
}

void CGuiCommonList::OnBack( CBookBase* book )
{
	book->UI_Event( CANCEL_EVENT );
}

void CGuiCommonList::OnLongBack( CBookBase* book )
{
	book->UI_Event( CANCEL_EVENT );
}

void CGuiCommonList::OnDelete( CBookBase* book )
{
	book->UI_Event( CANCEL_EVENT );
}

void CGuiCommonList::OnSelect( CBookBase* book )
{
	listdata[ GetSelectedItem() ].book->SetFocus( 0 );
	book->UI_Event( CANCEL_EVENT );
}

void CGuiCommonList::OnShortcut( CBookBase* book, CGuiBase* gui )
{
	book->UI_Event( CANCEL_EVENT );
}

void CGuiCommonList::OnRename( CBookBase* book, CGuiBase* gui )
{
	book->UI_Event( CANCEL_EVENT );
}

CGuiCommonList::CGuiCommonList( CBook* book, int display )
: CGuiListMenu( book, display )
{
	SoftKeys_SetAction( ACTION_BACK, __OnBack );
	SoftKeys_SetAction( ACTION_LONG_BACK, __OnLongBack );
	SoftKeys_SetAction( ACTION_DELETE, __OnDelete );
	SoftKeys_SetAction( ACTION_SELECT1, __OnSelect );
	SoftKeys_SetAction( 0, OnShortcut, STR("Shortcuts") );
	SoftKeys_SetAction( 1, OnRename, STR("Rename") );
}

CGuiCommonList::~CGuiCommonList()
{
}

int CGuiCommonList::isElfBook( BOOK * book )
{
	return ( ( (int)book->onClose ) & 0xF8000000 ) != ( ( (int)LastExtDB() ) & 0xF8000000 );
}

int isRSSTickerBook( BOOK* book )
{
	return 0 == strcmp( book->xbook->name, "RSSTicker_Book" );
}

int CGuiCommonList::isPermittedBook( BOOK * book )
{
	return  ( !CDaemonBook::isBookManager( book ) && !IsVolumeControllerBook( book ) && !IsRightNowBook( book ) && !isRSSTickerBook( book ));
}

int CGuiCommonList::isGuiBook( BOOK * book )
{
	return book->xguilist->guilist->FirstFree;
}

//------------------------------------------------------------------------------

CGuiBookList::CGuiBookList( CBook* book, int display )
: CGuiCommonList( book, display )
{
	List_Fill();
	if( !listdata.empty())
		SetFocused(0);
}

void CGuiBookList::List_Fill()
{
	int sessions_count = root_list_get_session_count();
	for ( int i = 0; i < sessions_count; i++ )
	{
		UI_APP_SESSION * session = root_list_get_session( i );
		int books_count = session->listbook->FirstFree;

		for (int j = 0; j < books_count; j++ )
		{
			CBook * book = (CBook*) List_Get( session->listbook, j );
			if ( ( !ElfInBookListEnabled && isElfBook( book ) ) || !isGuiBook( book ) || !isPermittedBook( book ) )
			{
				continue;
			}

			string sessionname = tostring( book->GetSession()->name );
			string bookname;

			if ( strcmp( book->xbook->name, "CUIDisplayableBook" ) )
			{
				bookname = book->xbook->name;
			}
			else
			{
				bookname = sessionname;
			}

			if( 0==sessionname.find("Foreign app") || sessionname == "Java" )
			{
				TEXTID name = JavaSession_GetName();
				bookname = tostring( name );
				TextID_Destroy( name );
			}

			LISTDATA xdata;

			xdata.isenabled = true;
			xdata.book = book;
			xdata.name = bookname;

			listdata.insert( listdata.begin(), xdata );
		}
	}
	SetItemCount( listdata.size() );
}

//------------------------------------------------------------------------------

CGuiElfList::CGuiElfList( CBook* book, int display )
: CGuiCommonList( book, display )
{
	List_Fill();
	if( !listdata.empty() )
		SetFocused(0);
}

void CGuiElfList::List_Fill()
{
	int sessions_count = root_list_get_session_count();
	for ( int i = 0; i < sessions_count; i++ )
	{
		UI_APP_SESSION * session = root_list_get_session( i );
		int books_count = session->listbook->FirstFree;

		for (int j = 0; j < books_count; j++ )
		{
			CBook * book = (CBook*) List_Get( session->listbook, j );
			if ( isElfBook( book ) && !CDaemonBook::isBookManager( book ) )
			{
				LISTDATA xdata;

				xdata.book = book;
				xdata.name = book->xbook->name;
				xdata.isenabled = isGuiBook( book );

				listdata.insert( listdata.begin(), xdata );
			}
		}
	}
	SetItemCount( listdata.size() );
}
