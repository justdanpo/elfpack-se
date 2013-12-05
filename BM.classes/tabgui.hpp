#pragma once

#include "..\classlib\cl.h"
#include "lists.hpp"

class CBookmanTabGui : public CGuiTabMenuBar
{
public:
	enum
	{
		BOOKLIST,
		ELFLIST,

		TABSCOUNT
	};
	enum
	{
		ICON_BOOK_DEACTIVE,
		ICON_BOOK_ACTIVE,
		ICON_ELF_DEACTIVE,
		ICON_ELF_ACTIVE,

		ICONS_COUNT
	};
private:
	CGuiListMenu* m_pguitabs[ TABSCOUNT ];
	IMAGEID icon[ ICONS_COUNT ];
	void InitIcons();

protected:
	~CBookmanTabGui();

public:
	CBookmanTabGui( CBook* book, int display=0 );
};
