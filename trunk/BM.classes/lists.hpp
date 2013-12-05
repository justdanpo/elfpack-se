#pragma once

#include "..\classlib\cl.h"

#include <string>
#include <vector>

struct LISTDATA
{
	CBook* book;
	string name;
	//      basic_string<wchar_t> label;
	bool isenabled;
};

class CGuiCommonList : public CGuiListMenu
{
protected:
	vector<LISTDATA> listdata;

	int OnMessage( CGUIMessage* msg );
	static void __OnBack( CBookBase* book, CGuiBase* gui );
	static void __OnLongBack( CBookBase* book, CGuiBase* gui );
	static void __OnDelete( CBookBase* book, CGuiBase* gui );
	static void __OnSelect( CBookBase* book, CGuiBase* gui );
	static void OnShortcut( CBookBase* book, CGuiBase* gui );
	static void OnRename( CBookBase* book, CGuiBase* gui );
public:
	virtual void OnBack( CBookBase* book );
	virtual void OnLongBack( CBookBase* book );
	virtual void OnDelete( CBookBase* book );
	virtual void OnSelect( CBookBase* book );

	CGuiCommonList( CBook* book, int display=0 );
	~CGuiCommonList();
	int isElfBook( BOOK * book );
	int isPermittedBook( BOOK * book );
	int isGuiBook( BOOK * book );
};

class CGuiBookList : public CGuiCommonList
{
protected:
	void List_Fill();
public:
	CGuiBookList( CBook* book, int display=0 );
	~CGuiBookList(){};
};


class CGuiElfList : public CGuiCommonList
{
protected:
	void List_Fill();
public:
	CGuiElfList( CBook* book, int display=0 );
	~CGuiElfList(){};
};
