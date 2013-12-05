#include "softkeyutil.hpp"
#include <stddef.h>

static bool isallocatedstrid(TEXTID strid)
{
	return (strid!=EMPTY_TEXTID) && ((strid&0xFFFF0000)!=0);
}

enum
{
	STRUCTTYPE_NONE,
	STRUCTTYPE_OLD,
	STRUCTTYPE_NEW
};

int CSoftKeyDesc::structtype = STRUCTTYPE_NONE;

int CSoftKeyDesc::getstructtype()
{
	if( structtype == STRUCTTYPE_NONE )
	{
		switch( GetChipID() & CHIPID_MASK )
		{
		case CHIPID_DB2000:
		case CHIPID_DB2010:
		case CHIPID_DB2020:
			structtype = STRUCTTYPE_OLD;
			break;
		default:
			structtype = STRUCTTYPE_NEW;
		}
	}
	return structtype;
}

#define RETURNSOFTKEYPARAM( member ) \
	if( offsetof( SOFTKEY_DESC, member ) == offsetof( SOFTKEY_DESC_A2, member ) ) \
		return reinterpret_cast<SOFTKEY_DESC*>( this )->member; \
	else if( getstructtype() == STRUCTTYPE_OLD ) \
		return reinterpret_cast<SOFTKEY_DESC*>( this )->member; \
	else return reinterpret_cast<SOFTKEY_DESC_A2*>( this )->member;


char& CSoftKeyDesc::GetVisible()
{
	RETURNSOFTKEYPARAM( visible );
}

char& CSoftKeyDesc::GetEnabled()
{
	RETURNSOFTKEYPARAM( enable );
}

void (*&CSoftKeyDesc::GetProc())(BOOK*,GUI*)
{
	RETURNSOFTKEYPARAM( proc );
}

void (*&CSoftKeyDesc::GetDisabledAction())(BOOK*,GUI*)
{
	RETURNSOFTKEYPARAM( DisabledAction );
}

u16& CSoftKeyDesc::GetAction()
{
	RETURNSOFTKEYPARAM( action );
}

TEXTID& CSoftKeyDesc::GetButtonText()
{
	RETURNSOFTKEYPARAM( ButtonText );
}

TEXTID& CSoftKeyDesc::GetMenuText()
{
	RETURNSOFTKEYPARAM( MenuText );
}

TEXTID& CSoftKeyDesc::GetHelpText()
{
	RETURNSOFTKEYPARAM( HelpText );
}

LIST*& CSoftKeyDesc::GetSubitems()
{
	RETURNSOFTKEYPARAM( subitems_list );
}

CSoftKeyDesc* FindSoftkey( DISP_OBJ* disp, BOOK* book, TEXTID strid, int sktype )
{
	wchar_t temp[32];
	temp[0]=0;

	LIST* softkeys = DispObject_SoftKeys_GetList( disp, book, sktype );
	if( softkeys )
	{
		for( int i=0; i< softkeys->FirstFree; i++ )
		{
			CSoftKeyDesc* sk = reinterpret_cast<CSoftKeyDesc*>( List_Get( softkeys, i ) );
			if( sk->GetVisible() )
			{
				if( isallocatedstrid(sk->GetButtonText()) || isallocatedstrid(strid) )
				{
					if( !temp[0] )
						TextID_GetWString( strid, temp, MAXELEMS( temp ) );

					wchar_t temp2[32];
					TextID_GetWString( sk->GetButtonText(), temp2, MAXELEMS( temp2 ) );

					if( !wstrcmp( temp, temp2 ) )
						return sk;
				}else
				{
					if( sk->GetButtonText() == strid )
						return sk;
				}
			}
		}
	}
	return NULL;
}


CSoftKeyDesc* FindActionSoftkey( DISP_OBJ* disp, BOOK* book, u16 action, int sktype )
{
	LIST* softkeys = DispObject_SoftKeys_GetList( disp, book, sktype );
	if( softkeys )
	{
		for( int i=0; i< softkeys->FirstFree; i++ )
		{
			CSoftKeyDesc* sk = reinterpret_cast<CSoftKeyDesc*>( List_Get( softkeys, i ) );
			if( sk->GetAction() == action )
			{
				return sk;
			}
		}
	}
	return NULL;
}

