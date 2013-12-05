#include "..\deleaker\mem2.h"
#include "config_data.h"
#include "tabgui.hpp"

CBookmanTabGui::CBookmanTabGui( CBook* book, int display )
: CGuiTabMenuBar( book, display )
{
	InitIcons();

	SetTabCount( TABSCOUNT );

	m_pguitabs[ BOOKLIST ] = new CGuiBookList( book );
	SetTabGui( BOOKLIST, m_pguitabs[0] );
	SetTabIcon( BOOKLIST, icon[ICON_BOOK_DEACTIVE], 0 );
	SetTabIcon( BOOKLIST, icon[ICON_BOOK_ACTIVE], 1 );
	SetTabTitle( BOOKLIST, STR("tab1") );

	m_pguitabs[ ELFLIST ] = new CGuiElfList( book );
	SetTabGui( ELFLIST, m_pguitabs[1] );
	SetTabIcon( ELFLIST, icon[ICON_ELF_DEACTIVE], 0 );
	SetTabIcon( ELFLIST, icon[ICON_ELF_ACTIVE], 1 );
	SetTabTitle( ELFLIST, STR("tab2") );
}

CBookmanTabGui::~CBookmanTabGui()
{
	m_pguitabs[ BOOKLIST ]->Free();
	m_pguitabs[ ELFLIST ]->Free();
	for( int i=0; i<ICONS_COUNT; i++)
	{
		ImageID_Free( icon[i] );
	}
}

#define IDN_BOOKS_DEACT_ICON _T( "RN_TAB_SESSION_MANAGER_DESELECTED_ICN" )
#define IDN_BOOKS_ACT_ICON _T( "RN_TAB_SESSION_MANAGER_SELECTED_ICN" )
#define IDN_ELFS_DEACT_ICON _T( "RN_TAB_BOOKMARKS_DESELECTED_ICN" )
#define IDN_ELFS_ACT_ICON _T( "RN_TAB_BOOKMARKS_SELECTED_ICN" )

wchar_t* id_names[CBookmanTabGui::ICONS_COUNT] =
{
	IDN_BOOKS_DEACT_ICON,
	IDN_BOOKS_ACT_ICON,
	IDN_ELFS_DEACT_ICON,
	IDN_ELFS_ACT_ICON
};

const wchar_t* img_names[CBookmanTabGui::ICONS_COUNT] =
{
	tab1_inact,
	tab1_act,
	tab2_inact,
	tab2_act
};

void CBookmanTabGui::InitIcons()
{
	wchar_t wstr_path[100];
	for( int i=0; i<ICONS_COUNT; i++)
	{
		wchar_t * wstr_name = wstrrchr(img_names[i],L'/');
		if ( wstr_name )
		{
			int path_len = wstr_name - img_names[i];
			wstrncpy( wstr_path, img_names[i], path_len );
			wstr_path[path_len] = 0;
			wstr_name++;
			if (ImageID_Get( wstr_path, wstr_name, &icon[i] ) >= 0 )
			{
				continue;
			}
		}
		int iconid;
		iconidname2id( id_names[i], TEXTID_ANY_LEN, &iconid );
		icon[i] = iconid;
	}
}
