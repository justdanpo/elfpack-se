#include "mybook.h"

#define MYBOOKNAME "SMSViewRewritten"
#define MYBOOKBASEPAGE "SMSViewRewritten_BOOK_BasePage"
#define ABOUTTEXT "SMSView rewritten.\n" __DATE__ "\nGreetz to DuMOHsmol"

#include "../classlib/softkeyutil.hpp"

void elfexit();


int CMyBook::TerminateElf( CBook** pbookmanbook, CMyBook* book )
{
	book->Free();
	return 1;
}


int CMyBook::ShowAuthorInfo( CBook** pbookmanbook, CMyBook* book )
{
	MessageBox( EMPTY_TEXTID, TextID_Create( ABOUTTEXT, ENC_LAT1, TEXTID_ANY_LEN ), NOIMAGE, 1 , 5000, *pbookmanbook );
	return 1;
}


DECLARE_PAGE_DESC_BEGIN( CMyBook::base_page, MYBOOKBASEPAGE )
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook( MYBOOKNAME, &base_page )
{
	//софт в списке сообщений
	textidname2id( _T("MSG_UI_VIEW_MESSAGE_SK"), TEXTID_ANY_LEN, &strid_viewmsg );
	textidname2id( _T("GUI_VIEW_SK"), TEXTID_ANY_LEN, &strid_view );
	//софт в просмотре входящих
	textidname2id( _T("MSG_UI_REPLY_MESSAGE_SK"), TEXTID_ANY_LEN, &strid_reply );
	//софт в просмотре переданных
	textidname2id( _T("MSG_UI_FORWARD_MESSAGE_SK"), TEXTID_ANY_LEN, &strid_forward );
	//софт при проигрывании ммс
	textidname2id( _T("SMIL_VIEWER_STOP_SK"), TEXTID_ANY_LEN, &strid_stop );

	MODIFYKEYHOOK( _HookCBKey, KEY_HOOK_ADD, this );
}


CMyBook::~CMyBook()
{
	MODIFYKEYHOOK( _HookCBKey, KEY_HOOK_REMOVE, NULL );

	if( !blockbooks.empty() )
		MODIFYUIPAGEHOOK( CANCEL_EVENT, _HookAction, this, PAGE_HOOK_REMOVE );

	elfexit();
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
	return static_cast<CMyBook*>( FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME ) );
}


int CMyBook::_HookCBKey( int key, int repeat_count, int mode, CMyBook* book, DISP_OBJ* hooked_disp )
{
	return book->HookCBKey( key, repeat_count, mode, hooked_disp );
}

int CMyBook::HookCBKey( int key, int repeat_count, int mode, DISP_OBJ* hooked_disp )
{
	if( mode != KBD_SHORT_PRESS )
		return 0;

	if( (key != KEY_LEFT) && (key != KEY_RIGHT) && (key != KEY_STAR) && (key != KEY_DIEZ)
       && (key != KEY_UP) && (key != KEY_DOWN) )
		return 0;

	//topbook - текущая книга (должен быть вьюер смс/ммс)
	BOOK* topbook = Display_GetTopBook( 0 );
	if( !topbook )
		return 0;

	bool ismms = 0==strcmp( topbook->xbook->name, "SmilViewer Book" );

	//для ммс не меняем кнопки лево/право
    //надо ли игнорировать также вверх/вниз?
	if( ismms )
		if( key == KEY_LEFT || key == KEY_RIGHT )
			return 0;

	//смотрим текущую книгу
	if( strcmp( topbook->xbook->name, "SMSViewer Book" ) && !ismms )
		return 0;

	//если есть "ответить" или "переслать" или "стоп" (ммс) - наш гуй
	if( !FindSoftkey( hooked_disp, topbook, strid_reply )
		&& !FindSoftkey( hooked_disp, topbook, strid_forward )
		&& !FindSoftkey( hooked_disp, topbook, strid_stop ) )
		return 0;

	//смотрим промежуточную книгу
	BOOK* msguibook = FindBookByID( topbook->xbook->parent_BookID );
	if( !msguibook || 0!=strcmp(msguibook->xbook->name, "MSG UI Book") )
		return 0;

	//смотрим книгу списка сообщений
	BOOK* msguilistbook = FindBookByID( msguibook->xbook->parent_BookID );
	if( !msguilistbook )
		return 0;
	if( 0!=strcmp(msguilistbook->xbook->name, "MSG UI List Book") //a2
		&& 0!=strcmp(msguilistbook->xbook->name, "MSG UI Book") ) //db2020
		return 0;

	//гуй списка сообщений
	if( 0 == msguilistbook->xguilist->guilist->FirstFree )
		return 0;
	GUI_LIST* guimsglist = NULL;
	DISP_OBJ* dispmsglist;

	for( int i = 0; i < msguilistbook->xguilist->guilist->FirstFree; i++ )
	{
		GUI_LIST* tmpguimsglist = reinterpret_cast<GUI_LIST*>( List_Get( msguilistbook->xguilist->guilist, i ) );
		dispmsglist = GUIObject_GetDispObject( tmpguimsglist );
		if( !strcmp( "ListObject", DispObject_GetName( dispmsglist ) ) )
		{
			guimsglist = tmpguimsglist;
        }
	}
	//нет у книги листа - выход
	if( !guimsglist )
		return 0;

	//не уверен, нужна ли эта проверка, для всех ли элементов в списке есть "показать"
	//смотрим софты списка. если есть "Показать", значит наш лист
	if( !FindSoftkey( dispmsglist, msguilistbook, strid_viewmsg )
		&& !FindSoftkey( dispmsglist, msguilistbook, strid_view ) )
		return 0;

	if(key==KEY_UP)
    {
        DispObject_GetOnKey(hooked_disp) ( hooked_disp, KEY_LEFT, 0, repeat_count, mode);
        return -1;
    }
	if(key==KEY_DOWN)
    {
        DispObject_GetOnKey(hooked_disp) ( hooked_disp, KEY_RIGHT, 0, repeat_count, mode);
        return -1;
    }

	int cnt = ListMenu_GetItemCount( guimsglist );
	int cur = ListMenu_GetSelectedItem( guimsglist );

	cur += key == KEY_LEFT || key == KEY_STAR ? -1 : 1;

	if( cur < 0 )
		cur = cnt - 1;

	if( cur >= cnt )
		cur = 0;

	//сохраняем книгу в списке, в хуке для одной страницы этой книги запретим эвент
	if( blockbooks.empty() )
		MODIFYUIPAGEHOOK(CANCEL_EVENT, _HookAction, this, PAGE_HOOK_ADD_BEFORE);
	blockbooks.insert( msguilistbook );

	BookObj_ReturnPage( topbook, CANCEL_EVENT );

	ListMenu_SetCursorToItem( guimsglist, cur );

	//вызываем экшн "view" нового выбранного элемента
	CSoftKeyDesc* sk = FindSoftkey( dispmsglist, msguilistbook, strid_viewmsg );
	if( !sk )
		sk = FindSoftkey( dispmsglist, msguilistbook, strid_view );
	DispObject_SoftKeys_ExecuteAction( dispmsglist, sk->GetAction() );

	return -1;
}


int CMyBook::_HookAction(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookAction(msg, book, page_desc, event);
}

int CMyBook::HookAction( void *msg, BOOK* book, PAGE_DESC * page_desc, u16 event)
{
	set<BOOK*>::iterator i = blockbooks.find( book );
	if( i != blockbooks.end() )
	{
		if( !strcmp("MSG_UI_ExtendedMessageList_Page", page_desc->name)
		   ||!strcmp("MSG_UI_MessageList_Page", page_desc->name))
		{
			blockbooks.erase( i );
			if( blockbooks.empty() )
				MODIFYUIPAGEHOOK(CANCEL_EVENT, _HookAction, this, PAGE_HOOK_REMOVE);

			//блокируем CANCEL_EVENT для одной страницы, чтоб не пересоздавался лист
			return BLOCK_EVENT_IN_THIS_SESSION;
		}
	}
	return 0;
}
