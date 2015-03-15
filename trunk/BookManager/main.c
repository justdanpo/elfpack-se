#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "shortcuts.h"
#include "book_names.h"
#include "main.h"


wchar_t* id_digits[DIGITS_COUNT] =
{
	IDN_DIGIT_0_ICON,
	IDN_DIGIT_1_ICON,
	IDN_DIGIT_2_ICON,
	IDN_DIGIT_3_ICON,
	IDN_DIGIT_4_ICON,
	IDN_DIGIT_5_ICON,
	IDN_DIGIT_6_ICON,
	IDN_DIGIT_7_ICON,
	IDN_DIGIT_8_ICON,
	IDN_DIGIT_9_ICON,
};


wchar_t* id_names[ICONS_COUNT] =
{
	IDN_BOOKS_DEACT_ICON,
	IDN_BOOKS_ACT_ICON,
	IDN_ELFS_DEACT_ICON,
	IDN_ELFS_ACT_ICON
};


const wchar_t* img_digits[DIGITS_COUNT] =
{
	digit0,
	digit1,
	digit2,
	digit3,
	digit4,
	digit5,
	digit6,
	digit7,
	digit8,
	digit9,
};


const wchar_t* img_names[ICONS_COUNT] =
{
	tab1_inact,
	tab1_act,
	tab2_inact,
	tab2_act
};


//============= pages start ======================

const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, CreateBookList,
	BOOK_DESTROYED_EVENT_TAG, onRootListChanged,
	USER_INACTIVITY_EVENT_TAG, onUserInactivity,
	ACCEPT_EVENT_TAG, RecreateBookList,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Main_Page = { "BookManager_Main_Page", 0, bk_main_msglst };


const PAGE_MSG bk_idle_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, IdlePage_EnterAction,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Idle_Page = { "BookManager_Idle_Page", 0, bk_idle_msglst };


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
	ELF_RECONFIG_EVENT, ReconfigElf,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BookManager_Base_Page = { "BookManager_Base_Page", 0, bk_base_msglst };

//============= pages end ======================


void elf_exit( void )
{
	trace_done();
	kill_data( &ELF_BEGIN, ( void(*)(void*) )mfree_adr() );
}

// ======================== всякая служебная ерунда start ===========================================

int GetActiveTab( MyBOOK* mbk )
{
	return TabMenuBar_GetFocusedTab( mbk->gui );
}


int isBookManager( BOOK* book )
{
	return 0 == strcmp( book->xbook->name, "BookManager" );
}


// проверка, обрабатывает ли БЕЙЗ_ПЕЙДЖ книги событие
int CheckEv( BOOK* bk, int ev )
{
	if ( bk->base_page )
	{
		const PAGE_MSG* pm = bk->base_page->msg_lst;

		if ( pm )
		{
			while ( pm->msg && pm->proc )
			{
				if ( pm->msg == ev )
				{
					return 1;
				}
				pm++;
			}
		}
	}
	return 0;
}


//Получить пользовательское имя книги
wchar_t * GetUserBookName(wchar_t * ini,wchar_t * orig_name,wchar_t * cur_name)
{
	wchar_t * pos;

	if (ini)
	{
		wchar_t uni_pair[MAX_BOOK_NAME_LEN + sizeof(": ")];
		uni_pair[0]=0;
		wstrcpy(uni_pair,orig_name);
		wstrcat(uni_pair,L": ");

		pos = wstrwstr(ini,uni_pair);

		if (pos)
		{
			swscanf(pos,L"%*[^:]: %l[^\r]",cur_name);
			return pos;
		}
	}

	wstrcpy(cur_name,orig_name);
	return 0;
}


// взять имя книги из ини-файла
TEXTID GetUserBookNameTEXTID( char* name )
{
	wchar_t cur_name[MAX_BOOK_NAME_LEN+1];
	wchar_t orig_name[MAX_BOOK_NAME_LEN+1];

	MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

	str2wstr(orig_name,name);

	GetUserBookName(mbk->booknames_buf, orig_name, cur_name);

	return TextID_Create(cur_name, ENC_UCS2, TEXTID_ANY_LEN);
}


// получить имя жавы
int GetJavaName( BOOK* bk )
{
	wchar_t ws[100];

	TextID_GetWString( BookObj_GetSession( bk )->name, ws, MAXELEMS(ws) );

	if ( !wstrncmp( ws, L"Foreign app", 11 ) )
	{
		return JavaSession_GetName();
	}

	if ( !wstrcmp( ws, L"Java" ) )
	{
		return JavaSession_GetName();
	}

	return EMPTY_TEXTID;
}


//Получить ID жавы для ярлыка
int JavaShortcut_GetID(wchar_t * java_shortcut)
{
	int ret_val = -1;
	wchar_t * hash_name = wstrwstr(java_shortcut,L"//") + 2;

	//Find app
	char sp1;
	wchar_t* sp;
	void* JavaDesc;
	JavaDialog_Open( 0, &sp1, &JavaDesc );
	if ( !JavaAppDesc_GetFirstApp( JavaDesc ) )
	{
		int result = 0;
		while ( !result )
		{
			JavaAppDesc_GetJavaAppInfo( JavaDesc, 4, &sp );
			int cmp_res = wstrcmp(hash_name,sp);
			delete(sp);
			if (cmp_res) result = JavaAppDesc_GetNextApp( JavaDesc );
			else
			{
				ret_val = JavaAppDesc_GetJavaAppID( JavaDesc );
				break;
			}
		}
	}
	JavaDialog_Close( sp1 );
	return(ret_val);
}


BOOK_LIST_ITEM * GetBookListItem( BOOK* bk, int list )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BOOK_LIST_ITEM* elem;

	switch ( list )
	{
	case BOOKLIST:
		if ( mbk->blistcnt )
		{
			if (MINIMIZE_TO_SESSION)
			{
				SESSION_LIST_ITEM * ses_elem = (SESSION_LIST_ITEM*)List_Get( mbk->sessions_list, mbk->blistpos );
				elem = (BOOK_LIST_ITEM*) List_Get( ses_elem->books_list, ses_elem->pos_subitem );
			}
			else
				elem = (BOOK_LIST_ITEM*) List_Get( mbk->books_list, mbk->blistpos );
		}
		else
			return 0;
		break;

	case ELFLIST:
		if ( mbk->elistcnt )
			elem = (BOOK_LIST_ITEM*) List_Get( mbk->elfs_list, mbk->elistpos );
		else
			return 0;
		break;
	}
	return elem;
}


// взять выбранную книгу из листа книг/эльфов
BOOK* GetBook( BOOK* bk, int list )
{
	BOOK_LIST_ITEM * elem = GetBookListItem(bk,list);
	if (!elem) return 0;
	else return elem->book;
}


int BOOK_LIST_ITEM_Filter(void * elem)
{
	if (elem) return(1);
	return(0);
}


void BOOK_LIST_ITEM_Destroy(void * data)
{
	BOOK_LIST_ITEM * elem = (BOOK_LIST_ITEM*)data;

	if (elem)
	{
		delete(elem->book_name);
		delete(elem);
	}
}


int SESSION_LIST_ITEM_Filter(void * elem)
{
	if (elem) return(1);
	return(0);
}


void SESSION_LIST_ITEM_Destroy(void * data)
{
	SESSION_LIST_ITEM * elem = (SESSION_LIST_ITEM*)data;

	if (elem)
	{
		List_DestroyElements(elem->books_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
		List_Destroy(elem->books_list);
		delete(elem);
	}
}


void DestroyBookAndElfsLists( MyBOOK* mbk )
{
	if ( mbk->sessions_list )
	{
		List_DestroyElements(mbk->sessions_list,SESSION_LIST_ITEM_Filter,SESSION_LIST_ITEM_Destroy);
		List_Destroy(mbk->sessions_list);
		mbk->sessions_list=0;
	}

	if ( mbk->books_list )
	{
		List_DestroyElements(mbk->books_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
		List_Destroy(mbk->books_list);
		mbk->books_list=0;
	}

	if ( mbk->elfs_list )
	{
		List_DestroyElements(mbk->elfs_list,BOOK_LIST_ITEM_Filter,BOOK_LIST_ITEM_Destroy);
		List_Destroy(mbk->elfs_list);
		mbk->elfs_list=0;
	}
}


int GetBooksFromSession(MyBOOK * mbk,UI_APP_SESSION * session,LIST * books_list,LIST * elfs_list)
{
	int k;
	int res=0;
	int mask = ( (int)LastExtDB() )&FLASH_MASK;

	for ( k = 0; k < session->listbook->FirstFree; k++ )
	{
		BOOK * book = (BOOK*) List_Get( session->listbook, k );

		if ( !isBookManager( book ) )
		{
			char s[MAX_BOOK_NAME_LEN+1];
			BOOK_LIST_ITEM * elem = new BOOK_LIST_ITEM;
			elem->book=book;
			elem->gui_count = book->xguilist->guilist->FirstFree;
			elem->isJava=FALSE;
			elem->isJava_2010=FALSE;

			if ( strcmp( book->xbook->name, JAVA_BOOK_NAME ) )
			{
				char * bn =new char[strlen(book->xbook->name)+1];
				strcpy(bn,book->xbook->name);
				elem->book_name = bn;
			}
			else
			{
				TextID_GetString( BookObj_GetSession(book)->name, s, MAXELEMS(s) );
				char * bn =new char[strlen(s)+1];
				strcpy(bn,s);
				elem->book_name = bn;
				elem->isJava=TRUE;
			}

			if ( mbk->hidden_buf_size && strstr(mbk->hidden_buf,elem->book_name) ) elem->isHidden=TRUE;
			else elem->isHidden=FALSE;

			TEXTID tmp = GetJavaName( book );

			if ( tmp != EMPTY_TEXTID )
			{
				delete(elem->book_name);
				TextID_GetString( tmp, s, MAXELEMS(s) );
				char * java_name = new char[strlen(s)+1];
				strcpy(java_name,s);
				elem->book_name = java_name;
				elem->isJava_2010=TRUE;
				TextID_Destroy( tmp );
			}

			else if ( ( ((int)book->onClose)&FLASH_MASK ) == mask )
			{
				if (mbk->books_show_hid==FALSE && elem->isHidden==TRUE)
					BOOK_LIST_ITEM_Destroy(elem);
				else if (mbk->books_show_nogui==FALSE && !elem->gui_count)
					BOOK_LIST_ITEM_Destroy(elem);
				else
					List_InsertFirst( books_list, elem );
			}
			else
			{
				if (mbk->elfs_show_hid==FALSE && elem->isHidden==TRUE)
					BOOK_LIST_ITEM_Destroy(elem);
				else if (mbk->elfs_show_nogui==FALSE && !elem->gui_count)
					BOOK_LIST_ITEM_Destroy(elem);
				else
				{
					List_InsertFirst( elfs_list, elem );
					res=1;
				}
			}
		}
	}
	return res;
}


// создаём список книг
void CreateBookAndElfsLists( MyBOOK* mbk )
{
	int i, j, isElf;
	UI_APP_SESSION* session;

	if (MINIMIZE_TO_SESSION)
		mbk->sessions_list = List_Create();
	else
		mbk->books_list = List_Create();

	mbk->elfs_list = List_Create();

	i = root_list_get_session_count();

	for ( j = 0; j < i; j++ )
	{
		session = root_list_get_session( j );

		if (MINIMIZE_TO_SESSION)
		{
			SESSION_LIST_ITEM * elem = new SESSION_LIST_ITEM;
			elem->session=session;
			elem->books_list=List_Create();
			elem->pos_subitem=0;

			isElf = GetBooksFromSession(mbk,session,elem->books_list,mbk->elfs_list);

			if ( !elem->books_list->FirstFree || isElf )
				SESSION_LIST_ITEM_Destroy(elem);
			else
				List_InsertFirst( mbk->sessions_list, elem );
		}
		else
			GetBooksFromSession(mbk,session,mbk->books_list,mbk->elfs_list);
	}

	if (MINIMIZE_TO_SESSION)
		mbk->blistcnt = mbk->sessions_list->FirstFree;
	else
		mbk->blistcnt = mbk->books_list->FirstFree;

	mbk->elistcnt = mbk->elfs_list->FirstFree;
}


wchar_t* get_path()
{
	wchar_t* path = new wchar_t[wstrlen( GetDir( DIR_INI ) ) + sizeof("/bookman")];
	wstrcpy( path, GetDir( DIR_INI ) );
	wstrcat( path, L"/bookman" );
	return path;
}


int get_file( wchar_t* name, void** buf_set )
{
	int size = 0;
	int file;
	void* buf = 0;
	FSTAT _fstat;

	wchar_t* path = get_path();

	if ( fstat( path, name, &_fstat ) == 0 )
	{
		if ( ( file = _fopen( path, name, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
		{
			buf = malloc( _fstat.fsize+1 );
			fread( file, buf, _fstat.fsize );
			fclose( file );
			size = _fstat.fsize;
		}
	}

	*buf_set = buf;
	delete( path );
	return size;
}


void get_iconsID( MyBOOK* mbk )
{
	int i;
	IMAGEID imgID;
	int _imgID;
	wchar_t wstr_path[100];

	for ( i = 0;i < ICONS_COUNT;i++ )
	{
		iconidname2id(id_names[i],TEXTID_ANY_LEN,&_imgID);
		mbk->tabs_image[i].ImageID = _imgID;
	}
	for ( i = 0;i < DIGITS_COUNT;i++ )
	{
		iconidname2id(id_digits[i],TEXTID_ANY_LEN,&_imgID);
		mbk->digs_image[i].ImageID = _imgID;
	}
	for ( i = 0;i < ICONS_COUNT;i++ )
	{
		wchar_t * wstr_name = wstrrchr(img_names[i],L'/');
		if ( wstr_name++ )
		{
			int path_len = wstr_name-img_names[i]-1;
			wstrncpy(wstr_path,img_names[i],path_len);
			wstr_path[path_len]=0;
			if (ImageID_Get( wstr_path, wstr_name, &imgID ) >= 0 )
			{
				mbk->tabs_image[i].ImageID=imgID;
			}
		}
	}
	for ( i = 0;i < DIGITS_COUNT;i++ )
	{
		wchar_t * wstr_name = wstrrchr(img_digits[i],L'/');
		if ( wstr_name++ )
		{
			int path_len = wstr_name-img_digits[i]-1;
			wstrncpy(wstr_path,img_digits[i],path_len);
			wstr_path[path_len]=0;
			if (ImageID_Get( wstr_path, wstr_name, &imgID ) >= 0 )
			{
				mbk->digs_image[i].ImageID=imgID;
			}
		}
	}
}


//Загрузить ини файлы
void LoadBookNames(MyBOOK * mbk)
{
	if ( mbk->booknames_buf )
	{
		delete (mbk->booknames_buf);
		mbk->booknames_buf=0;
	}
	void* sp;
	mbk->booknames_buf_size = get_file( INI_BOOK_NAMES, &sp );
	mbk->booknames_buf = (wchar_t*)sp;
}


void LoadShortcuts(MyBOOK * mbk)
{
	if ( mbk->shortcuts_buf )
	{
		delete (mbk->shortcuts_buf);
		mbk->shortcuts_buf=0;
	}
	void* sp;
	mbk->shortcuts_buf_size = get_file( INI_SHORTCUTS, &sp );
	mbk->shortcuts_buf = (char*)sp;
}


void LoadHidden(MyBOOK * mbk)
{
	if ( mbk->hidden_buf )
	{
		delete (mbk->hidden_buf);
		mbk->hidden_buf=0;
	}
	void* sp;
	mbk->hidden_buf_size = get_file( INI_HIDDEN, &sp );
	mbk->hidden_buf = (char*)sp;
}

// ======================== всякая служебная ерунда end ===========================================


// при убийстве какой либо книги
int onRootListChanged( void* r0, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	if ( !mbk->mode_list && !mbk->but_list )
	{
		// создали меню
		mbk->ActiveTAB=GetActiveTab(mbk);
		CreateBookManagerGUI( mbk );
	}
	return 0;
}


// если юзер заснул...
int onUserInactivity( void* r0, BOOK* bk )
{
	if ( UserInactivityEventEnabled )
		CloseMyBook( bk, 0 );
	return 0;
}


TEXTID onCallback_Books_GetItemName(BOOK_LIST_ITEM * elem)
{
	TEXTID name;

	name = GetUserBookNameTEXTID(elem->book_name);
	if (elem->isHidden)
	{
		TEXTID tmp[2];
		tmp[0]=TextID_Create("[H] ",ENC_LAT1,TEXTID_ANY_LEN);
		tmp[1]=name;
		name=TextID_Create(tmp,ENC_TEXTID,2);
	}
	return name;
}


// устанавливаем тексты в пунктах меню
int onCallback_Books( GUI_MESSAGE* msg )
{
	MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );
	int item;
	int parent_item;
	SESSION_LIST_ITEM * ses_elem;
	BOOK_LIST_ITEM * elem;

	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		if ( mbk->blistcnt )
		{
			item = GUIonMessage_GetCreatedItemIndex( msg );

			if (MINIMIZE_TO_SESSION)
			{
				ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, item );

				GUIonMessage_SetNumberOfSubItems(msg,0,ses_elem->books_list->FirstFree);
				GUIonMessage_SetMenuItemText( msg, TextID_Copy(ses_elem->session->name) );
			}
			else
			{
				elem = (BOOK_LIST_ITEM*) List_Get( mbk->books_list, item );

				GUIonMessage_SetMenuItemText( msg, onCallback_Books_GetItemName(elem) );

				if ( !elem->gui_count )
				{
					GUIonMessage_SetItemDisabled( msg, TRUE );
					GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to book without GUI..." ) );
				}
			}
		}
		break;

	case LISTMSG_HighlightChanged:
		mbk->blistpos = GUIonMessage_GetSelectedItem( msg );
		RefreshBookSoftkeys( mbk );
		break;

	case LISTMSG_GetSubItem:
		parent_item = GUIonMessage_SubItem_GetCreatedParentIndex(msg);
		item = GUIonMessage_SubItem_GetCreatedIndex(msg);
		ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, parent_item );
		elem = (BOOK_LIST_ITEM*)List_Get(ses_elem->books_list,item);

		GUIonMessage_SubItem_SetText(msg,onCallback_Books_GetItemName(elem));

		if ( !elem->gui_count )
		{
			GUIonMessage_SubItem_SetDisabled( msg, TRUE );
			//GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to book without GUI..." ) );
		}
		break;

	case LISTMSG_SubItemHighlightChanged:
		ses_elem = (SESSION_LIST_ITEM*) List_Get( mbk->sessions_list, mbk->blistpos );
		ses_elem->pos_subitem = GUIonMessage_SubItem_GetSelectedIndex(msg);

		elem=(BOOK_LIST_ITEM*)List_Get(ses_elem->books_list,ses_elem->pos_subitem);
		if ( !elem->gui_count ) GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to book without GUI..." ) );

		RefreshBookSoftkeys( mbk );
		break;
	}

	return 1;
}


int onCallback_Elfs( GUI_MESSAGE* msg )
{
	MyBOOK* mbk = (MyBOOK*) GUIonMessage_GetBook( msg );
	int item;
	BOOK_LIST_ITEM* elem;
	TEXTID name;

	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		if ( mbk->elistcnt )
		{
			item = GUIonMessage_GetCreatedItemIndex( msg );
			elem = (BOOK_LIST_ITEM*)List_Get( mbk->elfs_list, item );
			name = GetUserBookNameTEXTID(elem->book_name);
			if (elem->isHidden)
			{
				TEXTID tmp[2];
				tmp[0]=TextID_Create("[H] ",ENC_LAT1,TEXTID_ANY_LEN);
				tmp[1]=name;
				name=TextID_Create(tmp,ENC_TEXTID,2);
			}
			GUIonMessage_SetMenuItemText( msg, name );

			if ( !elem->gui_count )
			{
				GUIonMessage_SetItemDisabled( msg, TRUE );
				GUIonMessage_SetMenuItemUnavailableText( msg, STR( "Can't set focus to elf without GUI..." ) );
			}
		}
		break;
	case LISTMSG_HighlightChanged:
		mbk->elistpos = GUIonMessage_GetSelectedItem( msg );
		RefreshElfSoftkeys( mbk, mbk->elistpos );
		break;
	}
	return 1;
}


// при выборе пункта
void onEnterPressed_Books( BOOK* book, GUI* lt )
{
	BookObj_SetFocus( GetBook( book, BOOKLIST ), 0 );
	CloseMyBook( book, 0 );
}


// при выборе эльфа
void onEnterPressed_Elfs( BOOK* book, GUI* lt )
{
	BOOK* bk = GetBook( book, ELFLIST );
	if ( bk )
	{
		BookObj_SetFocus( bk, 0 );
		CloseMyBook( book, 0 );
	}
}


int List_Find_Back_Action( SOFTKEY_DESC * elem, int act )
{
  if ( elem->action == act ) return(0);
  return(1);
}


int List_Find_Back_Action_A2( SOFTKEY_DESC_A2 * elem, int act )
{
  if ( elem->action == act ) return(0);
  return(1);
}


// при нажатии "С"
void onDelPressed_Books( BOOK* book )
{
        MyBOOK* mbk = (MyBOOK*) book;
        
        BOOK * sby_book=Find_StandbyBook();
	BOOK_LIST_ITEM * elem = GetBookListItem( mbk, BOOKLIST );

	if (elem )
	{
		BOOK* bk = elem->book;

                if (bk != sby_book )
                {
		      if (elem->isJava_2010==TRUE) JavaSession_Manager( 0x0E );
		      else if (elem->isJava==TRUE) UI_Event_toBookID( TERMINATE_SESSION_EVENT, BookObj_GetBookID( bk ) );
		      else
      		      {
			      if ( CheckEv( bk, TERMINATE_SESSION_EVENT ) )
	      			      UI_Event_toBookID( TERMINATE_SESSION_EVENT, BookObj_GetBookID( (BOOK*)List_Get( bk->xbook->app_session->listbook, bk->xbook->app_session->listbook->FirstFree - 1 ) ) );
      			      else if ( CheckEv( bk, RETURN_TO_STANDBY_EVENT ) )
		      		      UI_Event_toBookID( RETURN_TO_STANDBY_EVENT, BookObj_GetBookID( (BOOK*)List_Get( bk->xbook->app_session->listbook, bk->xbook->app_session->listbook->FirstFree - 1 ) ) );
                              else
                              {
                                      if ( elem->gui_count )
                                      {
                                              BookObj_SetFocus(bk,0);
                                              GUI * gui = (GUI*)List_Get( bk->xguilist->guilist, elem->gui_count - 1 );
                                              GUIObject_SoftKeys_ExecuteAction( gui, ACTION_LONG_BACK );
                                              BookObj_SetFocus(mbk,0);
                                      }
                              }
		      }
                }
	}
}


// при нажатии "С" на эльфе
void onDelPressed_Elfs( BOOK* book )
{
	BOOK* bk = GetBook( book, ELFLIST );
	if (bk)
	{
		if ( CheckEv( bk, RETURN_TO_STANDBY_EVENT ) )
		{
			UI_Event_toBookID( RETURN_TO_STANDBY_EVENT, BookObj_GetBookID( bk ) );
		}
		else
		{
			if ( CheckEv( bk, ELF_TERMINATE_EVENT ) )
			{
				UI_Event_toBookID( ELF_TERMINATE_EVENT, BookObj_GetBookID( bk )/* bk->BookID*/ );
			}
			else
			{
				MessageBox(EMPTY_TEXTID,STR("TerminateEvent not supported by elf..."),NOIMAGE,1,3000,book);
			}
		}
	}
}


void onDelPressed()
{
	MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

	if (GetActiveTab(mbk)==BOOKLIST)
		onDelPressed_Books(mbk);
	else
		onDelPressed_Elfs(mbk);
}


// при нажатии "Автора!" на эльфе
void Author( BOOK* book, GUI* lt )
{
	BOOK* bk = GetBook( book, ELFLIST );
	if ( bk )
	{
		if ( CheckEv( bk, ELF_SHOW_INFO_EVENT ) )
		{
			MSG* msg = new MSG;
			msg->book = book;
			UI_Event_toBookIDwData( ELF_SHOW_INFO_EVENT, BookObj_GetBookID( bk ), msg, ( void(*)( void*) )mfree_adr() );
		}
		else
		{
			MessageBox(EMPTY_TEXTID,STR("Author unknown"),NOIMAGE,1,3000,book);
		}
	}
}


void Copyright( BOOK* book, GUI* lt )
{
	MessageBox(EMPTY_TEXTID,COPYRIGHT_STRING,NOIMAGE,1,3000,book);
}


void HideBook( BOOK* bk, GUI* gui )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	int f;
	wchar_t* path = get_path();

	mbk->ActiveTAB=GetActiveTab(mbk);
	BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);

	if ( ( f = _fopen( path, INI_HIDDEN, FSX_O_CREAT|FSX_O_APPEND, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
	{
		char temp_buf[MAX_BOOK_NAME_LEN+10];
		strcpy( temp_buf, elem->book_name );
		strcat( temp_buf, "\r\n" );
		fwrite( f, temp_buf, strlen( temp_buf ) );
		fclose( f );
	}

	delete path;

	LoadHidden(mbk);

	CreateBookManagerGUI( mbk );
}


void ShowBook( BOOK* bk, GUI* gui )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	int f;
	char* pos;
	char* end_pos;
	wchar_t* path = get_path();

	mbk->ActiveTAB=GetActiveTab(mbk);
	BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);

	if ( mbk->hidden_buf )
	{
		pos = strstr( mbk->hidden_buf, elem->book_name );
		end_pos = pos + strlen(elem->book_name) + sizeof("\r\n") - sizeof("");
		f = _fopen( path, INI_HIDDEN, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 );
		fwrite( f, mbk->hidden_buf, pos - mbk->hidden_buf );
		fwrite( f, end_pos, mbk->hidden_buf_size - (end_pos - mbk->hidden_buf) );
		fclose( f );
	}

	elem->isHidden=FALSE;

	delete path;

	LoadHidden(mbk);

	mbk->ActiveTAB=GetActiveTab(mbk);
	CreateBookManagerGUI( mbk );
}


void HiddenMode( BOOK* bk, GUI* gui )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	mbk->ActiveTAB=GetActiveTab(mbk);

	if (mbk->ActiveTAB==BOOKLIST)
	{
		if (mbk->books_show_hid==FALSE) mbk->books_show_hid=TRUE;
		else mbk->books_show_hid=FALSE;
	}
	else
	{
		if (mbk->elfs_show_hid==FALSE) mbk->elfs_show_hid=TRUE;
		else mbk->elfs_show_hid=FALSE;
	}

	CreateBookManagerGUI( mbk );
}


void NoGUIMode( BOOK* bk, GUI* gui )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	mbk->ActiveTAB=GetActiveTab(mbk);

	if (mbk->ActiveTAB==BOOKLIST)
	{
		if (mbk->books_show_nogui==FALSE) mbk->books_show_nogui=TRUE;
		else mbk->books_show_nogui=FALSE;
	}
	else
	{
		if (mbk->elfs_show_nogui==FALSE) mbk->elfs_show_nogui=TRUE;
		else mbk->elfs_show_nogui=FALSE;
	}

	CreateBookManagerGUI( mbk );
}


int StartElf( wchar_t* path, char* name )
{
	int res = -1;
	wchar_t* elfname = new wchar_t[wstrlen( path )+strlen( name )+1];
	wchar_t* wname = new wchar_t[strlen( name )+1];
	str2wstr( wname, name );
	wstrcpy( elfname, path );
	wstrcat( elfname, wname );

	{
		wchar_t* path;
		wchar_t* name;
		int len;
		name = wstrrchr( elfname, '/' );
		if ( name )
		{
			path = new wchar_t[ ( len = name-elfname ) + 1 ];
			memcpy( path, elfname, len * sizeof(wchar_t) );
			path[len] = 0;
			name++;
			if ( !fstat( path, name, 0 ) )
			{
				res = elfload( elfname, 0, 0, 0 );
			}
			delete( path );
		}
	}
	delete( elfname );
	delete( wname );
	return res;
}


void StartShortcut_Internal(char * param)
{
	wchar_t par[256];

	str2wstr( par, param );

	if ( strstr( param, "java:" ) )
	{
		char sp_flag;
		REQUEST_UI_OAF_START_APPLICATION( ASYNC, JavaShortcut_GetID(par)+0x10000, &sp_flag );
	}
	else
	{
		Shortcut_Run( par );
	}
}


void StartShortcut_External(char * param)
{
	if( StartElf( GetDir( DIR_ELFS | MEM_INTERNAL ), param ) < 0 )
		if ( StartElf( GetDir( DIR_ELFS | MEM_EXTERNAL ), param ) < 0 )
		{
			int ms[3];
			ms[0] = STR( "ZBin" );
			ms[1] = TextID_Create( param, ENC_LAT1, strlen( param ) );
			ms[2] = STR( "\n\nnot found.." );
			MessageBox( EMPTY_TEXTID,TextID_Create(ms,ENC_TEXTID,3), NOIMAGE, 1, 3000, 0);
		}
}


// патченый onKey от своей менюхи...; )
void myOnKey_Unified( DISP_OBJ* p, int keyID, int i2, int i3, int press_mode )
{
	MyBOOK* mbk = (MyBOOK*) FindBook( isBookManager );

	mbk->oldOnKey( p, keyID, i2, i3, press_mode );

	if ( keyID == KEY_DEL && press_mode == KBD_SHORT_RELEASE )
		onDelPressed();

	if ( keyID == KeyMinimize && press_mode == KeyMinimizePressMode )
	{
		BOOK* stby_bk = Find_StandbyBook();
		if ( stby_bk ) BookObj_SetFocus( stby_bk, 0 );
		CloseMyBook( mbk, 0 );
	}

	else if ( keyID == KeyCloseAll && press_mode == KeyCloseAllPressMode )
	{
		UI_Event( RETURN_TO_STANDBY_EVENT );
		CloseMyBook( mbk, 0 );
	}

	else if ( press_mode == KBD_SHORT_RELEASE || press_mode == KBD_LONG_PRESS )
	{
		if ( mbk->shortcuts_buf )
		{
			char key[20];

			if ( GetActiveTab(mbk)==BOOKLIST || ShortcutsMode )
				sprintf( key, press_mode == 3?"[S_KEY%d]":"[L_KEY%d]", keyID-KEY_DIGITAL_0 );
			else
				sprintf( key, press_mode == 3 ? "[ES_KEY%d]" : "[EL_KEY%d]", keyID - KEY_DIGITAL_0 );

			char* param = manifest_GetParam( mbk->shortcuts_buf, key, 0 );
			if ( param )
			{
				if (param[0]=='/')
					StartShortcut_External(param);
				else
					StartShortcut_Internal(param);

				mfree( param );
				CloseMyBook( mbk, 0 );
			}
		}
	}
}


void RefreshElfSoftkeys( MyBOOK* mbk, int item )
{
	if ( mbk->elistcnt )
	{
		BOOK_LIST_ITEM* elem = (BOOK_LIST_ITEM*) List_Get( mbk->elfs_list, item );
		if ( !elem->gui_count )
			GUIObject_SoftKeys_SetVisible( mbk->elist, ACTION_SELECT1, FALSE );
		else
			GUIObject_SoftKeys_SetVisible( mbk->elist, ACTION_SELECT1, TRUE );

		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, 1 - elem->isHidden );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, elem->isHidden );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, TRUE );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, TRUE );
	}
	else
	{
		ListMenu_SetNoItemText( mbk->elist, STR( "No elfs in memory" ) );
		GUIObject_SoftKeys_SetVisible( mbk->elist, ACTION_SELECT1, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, FALSE );
		mbk->elistpos = 0;
	}

	GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, 1 - mbk->elfs_show_hid );
	GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, mbk->elfs_show_hid );

	GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, 1 - mbk->elfs_show_nogui );
	GUIObject_SoftKeys_SetVisible( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, mbk->elfs_show_nogui );
}


void RefreshBookSoftkeys( MyBOOK* mbk )
{
	if ( mbk->blistcnt )
	{
		BOOK_LIST_ITEM * elem = GetBookListItem(mbk,BOOKLIST);

		if ( elem->gui_count )
			GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, TRUE );
		else
			GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, FALSE );

		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, 1 - elem->isHidden );
		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, elem->isHidden );
		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, TRUE );
	}
	else
	{
		ListMenu_SetNoItemText( mbk->blist, STR( "No books in memory" ) );
		GUIObject_SoftKeys_SetVisible( mbk->blist, ACTION_SELECT1, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, FALSE );
		GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, FALSE );
		mbk->blistpos = 0;
	}

	GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, 1 - mbk->books_show_hid );
	GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, mbk->books_show_hid );

	GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, 1 - mbk->books_show_nogui );
	GUIObject_SoftKeys_SetVisible( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, mbk->books_show_nogui );
}


// создание меню
void CreateBookManagerGUI( MyBOOK* mbk )
{
	int str_id;

	DestroyBookAndElfsLists( mbk );
	CreateBookAndElfsLists( mbk );

	if ( !mbk->gui )
	{
		mbk->gui = CreateTabMenuBar( mbk );
		TabMenuBar_SetTabCount( mbk->gui, 2 );

		TabMenuBar_SetTabIcon( mbk->gui, 0, mbk->tabs_image[0].ImageID, 0 );
		TabMenuBar_SetTabIcon( mbk->gui, 0, mbk->tabs_image[1].ImageID, 1 );

		TabMenuBar_SetTabIcon( mbk->gui, 1, mbk->tabs_image[2].ImageID, 0 );
		TabMenuBar_SetTabIcon( mbk->gui, 1, mbk->tabs_image[3].ImageID, 1 );
	}

	if ( mbk->blistpos > mbk->blistcnt-1 ) mbk->blistpos = mbk->blistcnt-1;

	if ( mbk->blist )
	{
		ListMenu_DestroyItems(mbk->blist);
		ListMenu_SetItemCount( mbk->blist, mbk->blistcnt );
		ListMenu_SetCursorToItem( mbk->blist, mbk->blistpos );
		RefreshBookSoftkeys( mbk );
	}
	else
	{
		mbk->blist = CreateListMenu( mbk, 0 );
		ListMenu_SetOnMessage( mbk->blist, onCallback_Books );
		ListMenu_SetItemCount( mbk->blist, mbk->blistcnt );
		ListMenu_SetCursorToItem( mbk->blist, mbk->blistpos );
		if (MINIMIZE_TO_SESSION)
			ListMenu_SetItemStyle( mbk->blist, 3 );

		GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_BACK, CloseMyBook );
		GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_LONG_BACK, PreTerminateManager );
		GUIObject_SoftKeys_SetAction( mbk->blist, ACTION_SELECT1, onEnterPressed_Books );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHORTCUTS_SOFTKEY, Shortcuts );
		textidname2id( TAB_BOOKS_SHORTCUTS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHORTCUTS_SOFTKEY, str_id );
		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, BookNames );
		textidname2id( TAB_BOOKS_RENAME_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_RENAME_SOFTKEY, str_id );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, HideBook );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDEBOOK_SOFTKEY, STR( "Hide book" ) );
		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, ShowBook );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWBOOK_SOFTKEY, STR( "Show book" ) );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, HiddenMode );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWHIDDEN_SOFTKEY, STR( "Show hidden" ) );
		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, HiddenMode );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDEHIDDEN_SOFTKEY, STR( "Hide hidden" ) );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, NoGUIMode );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SHOWNOGUI_SOFTKEY, STR( "Show NoGUI" ) );
		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, NoGUIMode );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_HIDENOGUI_SOFTKEY, STR( "Hide NoGUI" ) );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, 0 );
		textidname2id( TAB_BOOKS_SETTINGS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, str_id );

		GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_SHOWHIDDEN_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_HIDEHIDDEN_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_SHOWNOGUI_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->blist, TAB_BOOKS_SETTINGS_SOFTKEY, TAB_BOOKS_HIDENOGUI_SOFTKEY );

		GUIObject_SoftKeys_SetAction( mbk->blist, TAB_BOOKS_COPYRIGHT_SOFTKEY, Copyright );
		GUIObject_SoftKeys_SetText( mbk->blist, TAB_BOOKS_COPYRIGHT_SOFTKEY, STR( "About" ) );

		mbk->oldOnKey = DispObject_GetOnKey( GUIObject_GetDispObject( mbk->blist ) );

		DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject( mbk->blist ) ), myOnKey_Unified );

		RefreshBookSoftkeys( mbk );

		TabMenuBar_SetTabGui( mbk->gui, 0, mbk->blist );
	}

	TEXTID p[2] = { TextID_Create ( L"Heap : ", ENC_UCS2, 7 ), TextID_CreateIntegerID ( GetFreeBytesOnHeap() ) };
	TabMenuBar_SetTabTitle( mbk->gui, 0, TextID_Create( p, ENC_TEXTID, 2 ) );

	//---------------

	if ( mbk->elistpos > mbk->elistcnt-1 ) mbk->elistpos = mbk->elistcnt-1;

	if ( mbk->elist )
	{
		ListMenu_DestroyItems(mbk->elist);
		ListMenu_SetItemCount( mbk->elist, mbk->elistcnt );
		ListMenu_SetCursorToItem( mbk->elist, mbk->elistpos );
		RefreshElfSoftkeys( mbk, mbk->elistpos );
	}
	else
	{
		mbk->elist = CreateListMenu( mbk, 0 );
		ListMenu_SetCursorToItem( mbk->elist, 0 );
		ListMenu_SetOnMessage( mbk->elist, onCallback_Elfs );
		ListMenu_SetItemCount( mbk->elist, mbk->elistcnt );
		ListMenu_SetCursorToItem( mbk->elist, mbk->elistpos );

		GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_BACK, CloseMyBook );
		GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_LONG_BACK, PreTerminateManager );
		GUIObject_SoftKeys_SetAction( mbk->elist, ACTION_SELECT1, onEnterPressed_Elfs );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHORTCUTS_SOFTKEY, Shortcuts );
		textidname2id( TAB_ELFS_SHORTCUTS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHORTCUTS_SOFTKEY, str_id );
		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, BookNames );
		textidname2id( TAB_ELFS_RENAME_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_RENAME_SOFTKEY, str_id );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, HideBook );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDEBOOK_SOFTKEY, STR( "Hide elf" ) );
		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, ShowBook );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWBOOK_SOFTKEY, STR( "Show elf" ) );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, HiddenMode );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWHIDDEN_SOFTKEY, STR( "Show hidden" ) );
		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, HiddenMode );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDEHIDDEN_SOFTKEY, STR( "Hide hidden" ) );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, NoGUIMode );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SHOWNOGUI_SOFTKEY, STR( "Show NoGUI" ) );
		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, NoGUIMode );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_HIDENOGUI_SOFTKEY, STR( "Hide NoGUI" ) );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, 0 );
		textidname2id( TAB_ELFS_SETTINGS_NAME_SOFTKEY, TEXTID_ANY_LEN, &str_id );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, str_id );

		GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_SHOWHIDDEN_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_HIDEHIDDEN_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_SHOWNOGUI_SOFTKEY );
		GUIObject_SoftKeys_SetItemAsSubItem( mbk->elist, TAB_ELFS_SETTINGS_SOFTKEY, TAB_ELFS_HIDENOGUI_SOFTKEY );

		GUIObject_SoftKeys_SetAction( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, Author );
		GUIObject_SoftKeys_SetText( mbk->elist, TAB_ELFS_AUTHOR_SOFTKEY, STR( "Author" ) );

		//онкеи все равно одинаковые, так что уберем пока
		//    mbk->oldOnKey1 = DispObject_GetOnKey( GUIObject_GetDispObject( mbk->elist ) );

		DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject( mbk->elist ) ), myOnKey_Unified );

		RefreshElfSoftkeys( mbk, mbk->elistpos );

		TabMenuBar_SetTabGui( mbk->gui, 1, mbk->elist );
		TabMenuBar_SetTabTitle( mbk->gui, 1, STR( "Elfs" ) );
	}

	TabMenuBar_SetFocusedTab( mbk->gui, mbk->ActiveTAB );

	GUIObject_Show(mbk->gui);
}


void DestroyIniBuffers(MyBOOK* mbk)
{
	if ( mbk->booknames_buf )
	{
		delete mbk->booknames_buf;
		mbk->booknames_buf=0;
	}
	if ( mbk->shortcuts_buf )
	{
		delete mbk->shortcuts_buf;
		mbk->shortcuts_buf=0;
	}
	if ( mbk->hidden_buf )
	{
		delete mbk->hidden_buf;
		mbk->hidden_buf=0;
	}
}


void DestroyAllGUIs(MyBOOK* mbk)
{
	if ( mbk->java_list_menu )
	{
		GUIObject_Destroy( mbk->java_list_menu );
		mbk->java_list_menu=0;
		List_DestroyElements( mbk->java_list, elem_filter, elem_free );
		List_Destroy( mbk->java_list );
		mbk->java_list = 0;
	}

	if ( mbk->mode_list )
	{
		GUIObject_Destroy( mbk->mode_list );
		mbk->mode_list=0;
	}

	if ( mbk->but_list )
	{
		GUIObject_Destroy( mbk->but_list );
		mbk->but_list=0;
	}

	if ( mbk->YesNoQuestion )
	{
		GUIObject_Destroy( mbk->YesNoQuestion );
		mbk->YesNoQuestion=0;
	}

	if ( mbk->blist )
	{
		GUIObject_Destroy( mbk->blist );
		mbk->blist=0;
		mbk->blistpos=0;
	}

	if ( mbk->elist )
	{
		GUIObject_Destroy( mbk->elist );
		mbk->elist=0;
		mbk->elistpos=0;
	}

	if ( mbk->gui )
	{
		GUIObject_Destroy( mbk->gui );
		mbk->gui=0;
	}

	if ( mbk->MainMenuID != -1 )
	{
		BOOK* MainMenu = FindBookByID( mbk->MainMenuID );

		if ( MainMenu )
			FreeBook( MainMenu );

		mbk->MainMenuID = -1;
	}

	mbk->oldOnKey=0;
}


void DestroyIcons(MyBOOK* mbk)
{
	int i;

	for ( i = 0;i < ICONS_COUNT;i++ )
	{
		ImageID_Free( mbk->tabs_image[i].ImageID );
	}

	for ( i = 0;i < DIGITS_COUNT;i++ )
	{
		ImageID_Free( mbk->digs_image[i].ImageID );
	}
}


// при закрытии книги
void onMyBookClose( BOOK* book )
{
	MyBOOK* mbk = (MyBOOK*) book;

	DestroyIniBuffers(mbk);

	DestroyAllGUIs(mbk);

	DestroyBookAndElfsLists( mbk );

	DestroyIcons(mbk);
}


void ReturnMyBook( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	GUIObject_Destroy( mbk->YesNoQuestion );
	mbk->YesNoQuestion = 0;
	mbk->ActiveTAB=GetActiveTab(mbk);
	CreateBookManagerGUI( mbk );
}


void CloseMyBook( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	mbk->ActiveTAB=GetActiveTab(mbk);
	BookObj_GotoPage( mbk, &BookManager_Idle_Page);
}


void TerminateManager( BOOK* Book, GUI* )
{
	MessageBox( EMPTY_TEXTID,STR("BookManager\n\nterminated"),NOIMAGE,1,3000,0);
	FreeBook( Book );
	MODIFYKEYHOOK( NewKey, KEY_HOOK_REMOVE, NULL );
	SUBPROC( elf_exit );
}


void PreTerminateManager( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	mbk->YesNoQuestion = CreateYesNoQuestionVA( 0,
											   VAR_BOOK( mbk ),
											   VAR_YESNO_PRE_QUESTION( TextID_Create( "Exit command selected", ENC_LAT1, TEXTID_ANY_LEN ) ),
											   VAR_YESNO_QUESTION( TextID_Create( "Unload?", ENC_LAT1, TEXTID_ANY_LEN ) ),
											   0 );
	GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_YES, TerminateManager );
	GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_NO, CloseMyBook );
	GUIObject_SoftKeys_SetAction( mbk->YesNoQuestion, ACTION_BACK, ReturnMyBook );
}


int RecreateBookList( void* r0, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	mbk->ActiveTAB=GetActiveTab(mbk);
	CreateBookManagerGUI( mbk );
	return(0);
}


// собственно старт
__root int CreateBookList( void* r0, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	get_iconsID( mbk );

	if ( FirstTab )
		mbk->ActiveTAB = FirstTab-1;

	CreateBookManagerGUI( mbk );
	return 0;
}


int IdlePage_EnterAction( void* r0, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	DestroyAllGUIs(mbk);
	DestroyBookAndElfsLists(mbk);
	DestroyIcons(mbk);

	BookObj_WindowSetWantsFocus(mbk,0,FALSE);
	BookObj_Hide(mbk,0);
	return 0;
}


int ReconfigElf( void* mess, BOOK* book )
{
	RECONFIG_EVENT_DATA* reconf = ( RECONFIG_EVENT_DATA* )mess;

	if ( !wstrcmpi( reconf->path, successed_config_path ) && !wstrcmpi( reconf->name, successed_config_name ) )
	{
		InitConfig();
		return 1;
	}
	return 0;
}


int NewKey( int key, int rep_count, int mode, MyBOOK * mbk, DISP_OBJ* disp_obj )
{
	DISP_OBJ * disp;

	BOOK * top_book = Display_GetTopBook(0);

	if ( mode == KeyStartPressMode )
	{
		if ( key == KeyStart )
		{
			if ( isKeylocked() && !Ignore_KeyLock )
				return 0;

			if ( top_book!=mbk )
			{
				BookObj_WindowSetWantsFocus(mbk,0,TRUE);
				BookObj_SetFocus(mbk,0);
				BookObj_GotoPage( mbk, &BookManager_Main_Page );
				return -1;
			}
			else
			{
				if ( !mbk->mode_list && !mbk->YesNoQuestion && !mbk->StringInput )
				{
					CloseMyBook( mbk, 0 );
					return 0;
				}
				return -1;
			}
		}
	}

	if ( top_book==mbk )
	{
		if (MINIMIZE_TO_SESSION)
		{
			if ( key == KEY_LEFT || key == KEY_RIGHT )
			{
				if (mbk->oldOnKey)
				{
					if ( GetActiveTab(mbk) == BOOKLIST )
						disp = GUIObject_GetDispObject(mbk->blist);
					else
						disp = GUIObject_GetDispObject(mbk->elist);

					if (disp==disp_obj)
					{
						mbk->oldOnKey( disp, key, 0, rep_count, mode );
						return -1;
					}
				}
			}

			if ( key == KeyChangeTab && mode == KeyChangeTabPressMode )
			{
				if ( GetActiveTab(mbk) == BOOKLIST )
					disp = GUIObject_GetDispObject(mbk->blist);
				else
					disp = GUIObject_GetDispObject(mbk->elist);

				if (disp==disp_obj)
				{
					TabMenuBar_SetFocusedTab( mbk->gui, 1 - GetActiveTab(mbk) );
					return -1;
				}
			}
		}
	}
	return 0;
}


MyBOOK * CreateBookManagerBook()
{
	MyBOOK* mbk = (MyBOOK*) malloc( sizeof( MyBOOK ) );
	memset( mbk, 0, sizeof( MyBOOK ) );
	CreateBook( mbk, onMyBookClose, &BookManager_Base_Page, "BookManager", -1, 0 );
	//Init flags
	mbk->gui = 0;
	mbk->booknames_buf=0;
	mbk->booknames_buf_size=0;
	mbk->shortcuts_buf = 0;
	mbk->shortcuts_buf_size = 0;
	mbk->hidden_buf = 0;
	mbk->hidden_buf_size = 0;
	mbk->oldOnKey=0;
	mbk->sessions_list = 0;
	mbk->books_list = 0;
	mbk->elfs_list = 0;
	mbk->java_list = 0;
	mbk->java_list_menu = 0;
	mbk->blist = 0;
	mbk->elist = 0;
	mbk->mode_list = 0;
	mbk->but_list = 0;
	mbk->YesNoQuestion = 0;
	mbk->StringInput = 0;
	mbk->MainMenuID = -1;
	mbk->blistpos = 0;
	mbk->elistpos = 0;
	mbk->blistcnt = 0;
	mbk->elistcnt = 0;
	mbk->ActiveTAB = 0;
	mbk->isA2 = FALSE;
	mbk->books_show_hid = books_show_hid;
	mbk->books_show_nogui = books_show_nogui;
	mbk->elfs_show_hid = elfs_show_hid;
	mbk->elfs_show_nogui = elfs_show_nogui;
	//End init
	int platform=GetChipID()&CHIPID_MASK;
	if (platform==CHIPID_DB3150||platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350)
		mbk->isA2 = TRUE;

	LoadBookNames(mbk);
	LoadShortcuts(mbk);
	LoadHidden(mbk);

	BookObj_GotoPage( mbk, &BookManager_Idle_Page);

	return mbk;
}


int main ( void )
{
	if ( !FindBook( isBookManager ) )
	{
		trace_init(L"bookmanmem.txt");
		InitConfig();
		MODIFYKEYHOOK( NewKey, KEY_HOOK_ADD, CreateBookManagerBook() );
	}
	else
	{
		MessageBox( EMPTY_TEXTID, STR( "BookManager is already runed" ), NOIMAGE, 1, 5000, 0 );
		SUBPROC( elf_exit );
	}
	return 0;
}
