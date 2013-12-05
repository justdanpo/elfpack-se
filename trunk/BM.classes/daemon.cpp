#include "..\deleaker\mem2.h"
#include "daemon.hpp"
#include "conf_loader.h"
#include "config_data.h"

#define DAEMONBOOKNAME "_BookManagerDaemon"

extern void elf_exit(void);

int CDaemonBook::Terminate( void* mess, CDaemonBook* book )
{
	book->Free();
	return 0;
}

int CDaemonBook::OnCancel( void* mess, CDaemonBook* book )
{
	book->TabsHide();
	return 0;
}

void CDaemonBook::TabsHide()
{
	if(m_tabgui)
	{
		m_focusedtab = m_tabgui->GetFocusedTab();
		m_tabgui->Free();
		m_tabgui = NULL;
	}
	Hide(0);
}

void CDaemonBook::TabsShow()
{
	if(!m_tabgui)
	{
		m_tabgui = new CBookmanTabGui( this );
		m_tabgui->SetFocusedTab( m_focusedtab );
		m_tabgui->Show();
	}
	Show(0);
}

int CDaemonBook::ReconfigElf( RECONFIG_EVENT_DATA* reconf, CDaemonBook* book )
{
	if( !wstrcmpi( reconf->path, successed_config_path ) && !wstrcmpi( reconf->name, successed_config_name ) )
	{
		InitConfig();
		return 1;
	}
	return 0;
}

int CDaemonBook::OnInactivity( void* mess, CDaemonBook* book )
{
	if( UserInactivityEventEnabled )
	{
		book->TabsHide();
	}
	return 0;
}


DECLARE_PAGE_DESC_BEGIN(CDaemonBook::main_page, "Daemon_Main_Page")
////DECLARE_PAGE_DESC_MSG( PAGE_ENTER_EVENT_TAG, CMyBook::CreateBookList )
////DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::RecreateBookList )
DECLARE_PAGE_DESC_MSG( CANCEL_EVENT_TAG, CDaemonBook::OnCancel )
DECLARE_PAGE_DESC_END

DECLARE_PAGE_DESC_BEGIN(CDaemonBook::base_page, "Daemon_Base_Page")
DECLARE_PAGE_DESC_MSG( ELF_RECONFIG_EVENT, CDaemonBook::ReconfigElf )
//DECLARE_PAGE_DESC_MSG( BOOK_DESTROYED_EVENT_TAG, CMyBook::onRootListChanged )
DECLARE_PAGE_DESC_MSG( USER_INACTIVITY_EVENT_TAG, CDaemonBook::OnInactivity )
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CDaemonBook::Terminate )
DECLARE_PAGE_DESC_END


CDaemonBook::CDaemonBook() : CBook( DAEMONBOOKNAME, &base_page)
{
	InitConfig();

	m_focusedtab = CBookmanTabGui::BOOKLIST;
	m_tabgui = NULL;

	GotoPage(&main_page);

	MODIFYKEYHOOK( NewKey, KEY_HOOK_ADD, this );
}


CDaemonBook::~CDaemonBook()
{
	TabsHide();
	MODIFYKEYHOOK( NewKey, KEY_HOOK_REMOVE, this );
	elf_exit();
}

int CDaemonBook::NewKey( int key, int r1, int mode, CDaemonBook* mbk, DISP_OBJ* )
{
	if ( mode == KeyPressMode )
	{
		if ( key == KeyActiv )
		{
			if ( !isKeylocked() || Ignore_KeyLock )
			{
				if( !mbk->m_tabgui )
				{
					mbk->TabsShow();
					return -1;
				}else
				{
					//!!! not mode_list, not YesNoQuestion, not StringInput
					{
						mbk->TabsHide();
						return 0;
					}
					//return -1;
				}
			}
		}
	}
	return 0;
}


int CDaemonBook::FindBookExCallback(BOOK* book ,int * param)
{
	return 0 == strcmp( book->xbook->name, (char*)param);
}

CDaemonBook* CDaemonBook::GetExisting()
{
	return (CDaemonBook*)FindBookEx(FindBookExCallback, (int*)DAEMONBOOKNAME);
}

int CDaemonBook::isBookManager( BOOK* book )
{
	return 0 == strcmp( book->xbook->name, DAEMONBOOKNAME );
}
