#pragma once

#include "..\include\cfg_items.h"
#include "..\classlib\cl.h"
#include "tabgui.hpp"


class CDaemonBook : public CBook
{
protected:
	~CDaemonBook();
	void TabsHide();
	void TabsShow();

	int m_focusedtab;
	CBookmanTabGui* m_tabgui;

	static int FindBookExCallback(BOOK* book ,int * param);
	static int NewKey( int key, int r1, int mode, CDaemonBook* book, DISP_OBJ* );

	DECLARE_PAGE_DESC_MEMBER(base_page);
	DECLARE_PAGE_DESC_MEMBER(main_page);

public:
	CDaemonBook();

	static int ReconfigElf( RECONFIG_EVENT_DATA* mess, CDaemonBook* book );
	static int OnInactivity( void* mess, CDaemonBook* book );
	static int OnCancel( void* mess, CDaemonBook* book );
	static int Terminate( void* mess, CDaemonBook* book );

	static CDaemonBook* GetExisting();
	static int isBookManager( BOOK* book );
};
