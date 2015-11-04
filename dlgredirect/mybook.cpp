#include "mybook.h"

#define MYBOOKNAME "dlgredirect"
#define MYBOOKBASEPAGE "dlgredirect_BasePage"
#define ABOUTTEXT "dlgredirect"
#include "..\classlib\strutils.hpp"
#include "mydisp.hpp"

int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID, STR(ABOUTTEXT), NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}

void CMyBook::UpdateLanguage()
{
	int strid=0;

	if( textidname2id( _T("OBX_FILE_DELETED_TXT"), -1, &strid ) )
	{
		TextID_GetWString( strid, OBX_FILE_DELETED_TXT, MAXELEMS(OBX_FILE_DELETED_TXT) );
	}
	if( textidname2id( _T("OBX_FOLDER_DELETED_TXT"), -1, &strid ) )
	{
		TextID_GetWString( strid, OBX_FOLDER_DELETED_TXT, MAXELEMS(OBX_FOLDER_DELETED_TXT) );
	}
	if( textidname2id( _T("OBX_FOLDER_CREATED_TXT"), -1, &strid ) )
	{
		TextID_GetWString( strid, OBX_FOLDER_CREATED_TXT, MAXELEMS(OBX_FOLDER_CREATED_TXT) );
	}
}

int CMyBook::OnLangChanged(void* msg, CMyBook* book)
{
	book->UpdateLanguage();
	return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, MYBOOKBASEPAGE)
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_MSG( ACCEPT_EVENT_TAG, CMyBook::Accept )
DECLARE_PAGE_DESC_MSG( MMI_LANGUAGE_CHANGED_EVENT_TAG, CMyBook::OnLangChanged )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook(MYBOOKNAME, &base_page), feedback( NULL )
{
	UpdateLanguage();
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB2, this, PAGE_HOOK_ADD_AFTER);
}


CMyBook::~CMyBook()
{
	if( feedback )
		feedback->Free();

	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB2, this, PAGE_HOOK_REMOVE);
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}


int CMyBook::FindBookExCB_Name( BOOK* book , int* param )
{
	return 0 == strcmp( book->xbook->name, (char*)param );
}

int CMyBook::FindBookExCB_Book( BOOK* book , int* param )
{
	return book == (BOOK*)param;
}


CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*) FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME );
}

int CMyBook::_HookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB2( msg, book, page_desc );
}

int CMyBook::HookCB2(void *msg, CBook* book, PAGE_DESC * page_desc)
{
	bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );

	if( exists )
	{
		if( !strcmp("MessageBox_Base_Page", page_desc->name ) )
		{
			if(book->xguilist->guilist->FirstFree)
			{
				DISP_OBJ* d = GUIObject_GetDispObject( (GUI*) List_Get(book->xguilist->guilist, 0) );

				if(!strcmp("Feedback", DispObject_GetName( d ) ) )
				{
					TEXTID msgboxtextid = Feedback_DispObject_GetText( d );
					wchar_t tmp[64];
					int len = TextID_GetWString( msgboxtextid, tmp, MAXELEMS( tmp ) );

					if( wstrwstr( tmp, OBX_FILE_DELETED_TXT )
					   ||wstrwstr( tmp, OBX_FOLDER_DELETED_TXT )
						   ||wstrwstr( tmp, OBX_FOLDER_CREATED_TXT )
					   )
					{
						for(int i=0; i<len; i++)
							if( tmp[i]<' ' ) tmp[i]=' ';
						ShowText( TextID_Create( tmp, ENC_UCS2, len) );
						book->Free();
					}
				}
			}
		}
	}

	return 0;
}

void CMyBook::ShowText( TEXTID strid )
{
	if( !feedback )
	{
		Show(0);
		WindowSetWantsFocus( 0, 0 );

		feedback = new CGuiT<CLogDisp>(this, 0);

		feedback->SetStyle( UI_OverlayStyle_TrueFullScreen );
		feedback->SetTitleType( UI_TitleMode_None );

		//feedback->GetDISPObj()->WindowSetPosition( 0, 0 );
		feedback->GetDISPObj()->WindowSetSize( Display_GetWidth( 0 ), 24 );

		feedback->Show();
	}
	static_cast<CLogDisp*>(feedback->GetDISPObj())->UpdateText( strid );
}

int CMyBook::Accept(void *msg, CMyBook* book)
{
	book->feedback->Free();
	book->feedback = NULL;
	book->Hide(0);
	return 1;
}

