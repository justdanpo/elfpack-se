#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\include\cfg_items.h"
#include <errno.h>

#include "visual.h"
#include "datetime.h"
#include "keyinput.h"
#include "revision.h"
#include "main.h"

#define MESSAGE( __STR__ ) MessageBox( EMPTY_TEXTID, __STR__, NOIMAGE, 1 , 11000, (BOOK*) FindBook( isBcfgEditBook ) );

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE" =
{
	ELF_TERMINATE_EVENT, TerminateElf,
	ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
	NIL_EVENT_TAG, NULL
};

const PAGE_DESC bk_base = { "BcfgEdit_Base_Page", 0, bk_msglst_base };

enum INPUT_TYPES {
	IT_REAL = 0,
	IT_STRING = 1,
	IT_INTEGER = 2,
	IT_PHONE_NUMBER = 3,
	IT_DIGITAL_PASS = 4,
	IT_DIGITAL_IP = 5,
	IT_URL = 6,
	IT_UNSIGNED_DIGIT = 9,
	IT_EXTRA_DIGIT = 10,
	IT_ABC_OR_DIGIT = 11,
	IT_EXTRA_DIGIT_2 = 12,
	IT_ABC_OR_DIGIT_2 = 13,
};

#pragma segment = "ELFBEGIN"
void elf_exit( void )
{
	kill_data( __segment_begin( "ELFBEGIN" ), (void(*)(void*))mfree_adr() );
}

unsigned long Crc32( unsigned char* buf, unsigned long len )
{
	unsigned long crc_table[256];
	unsigned long crc;
	for ( int i = 0; i < 256; i ++ )
	{
		crc = i;
		for ( int j = 0; j < 8; j ++ )
		{
			crc = crc & 1 ? ( crc >> 1 ) ^ 0xEDB88320UL : crc >> 1;
		}
		crc_table[i] = crc;
	}
	crc = 0xFFFFFFFFUL;

	while ( len -- )
		crc = crc_table[ ( crc ^* buf ++ ) & 0xFF ] ^ ( crc >> 8 );

	return crc ^ 0xFFFFFFFFUL;
};

void win12512unicode( wchar_t* ws, const char* s, int len )
{
	int c;
	while( ( c = *s++ ) && len -- > 0 )
	{
		if( c == 0xA8 )
			c = 0x401;
		if( c == 0xAA )
			c = 0x404;
		if( c == 0xAF )
			c = 0x407;
		if( c == 0xB8 )
			c = 0x451;
		if( c == 0xBA )
			c = 0x454;
		if( c == 0xBF )
			c = 0x457;
		if( c >= 0xC0 && c < 0x100 )
			c += 0x350;
		*ws++ = c;
	}
	* ws = 0;
}

char* unicode2win1251( char* s, wchar_t* ws, int len )
{
	char* d = s;
	int c;
	while( ( c = *ws++ ) && len -- > 0 )
	{
		if( c == 0x401 )
			c = 0xA8;
		if( c == 0x404 )
			c = 0xAA;
		if( c == 0x407 )
			c = 0xAF;
		if( c == 0x451 )
			c = 0xB8;
		if( c == 0x454 )
			c = 0xBA;
		if( c == 0x457 )
			c = 0xBF;
		if( c >= 0x410 && c < 0x450 )
			c -= 0x350;
		*s++ = c;
	}
	*s = 0;
	return d;
}

int getnumwidth( unsigned int num )
{
	int i = 1;
	while( num >= 10 )
	{
		num /= 10;
		i++ ;
	}
	return i;
}

int SaveCfg( BCFG_DATA* bdata )
{
	int f;
	int result = 0;
	if( ( f = _fopen( bdata->path, bdata->name, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
	{
		if( fwrite( f, bdata->cfg, bdata->size_cfg ) == bdata->size_cfg )
			result = 1;

		fclose( f );
	}
	return result;
}

int LoadCfg( BCFG_DATA* bdata )
{
	int f;
	FSTAT _fstat;
	int result = 0;

	if( bdata->cfg )
		delete bdata->cfg;

	if( bdata->path )
	{
		if( fstat( bdata->path, bdata->name, &_fstat ) != - 1 )
		{
			if( ( f = _fopen( bdata->path, bdata->name, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
			{
				bdata->size_cfg = _fstat.fsize;
				if( bdata->size_cfg <= 0 )
				{
					MESSAGE( STR( "Zero lenght of .bcfg file ! " ) );
				}
				else
				{
					bdata->cfg = new unsigned char[( bdata->size_cfg + 3 )&~3];
					if( fread( f, bdata->cfg, bdata->size_cfg ) != bdata->size_cfg )
					{
						MESSAGE( STR( "Can't read .bcfg file ! " ) );
						delete bdata->cfg;
						bdata->cfg = NULL;
					}
					else
						result = 1;
				}
				fclose( f );
			}
		}
	}
	return result;
}

void SendReconfigEvent( BCFG_DATA* bdata )
{
	RECONFIG_EVENT_DATA* reconf;
	if( wstrlen( bdata->path ) < MAXELEMS( reconf->path ) && wstrlen( bdata->name ) < MAXELEMS( reconf->name ) )
	{
		reconf = (RECONFIG_EVENT_DATA*) malloc( sizeof(RECONFIG_EVENT_DATA) );
		wstrcpy( reconf->path, bdata->path );
		wstrcpy( reconf->name, bdata->name );
		UI_Event_wData( ELF_RECONFIG_EVENT, reconf, (void(*)(void*)) mfree_adr() );
	}
}


void OnYesExitGui( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BCFG_DATA* bdata = &mbk->bdata;
	SaveCfg( bdata );
	SendReconfigEvent( bdata );
	FreeBook( bk );
}

void OnNoExitGui( BOOK* bk, GUI* )
{
	FreeBook( bk );
}

void OnBackExitGui( BOOK* bk, GUI* )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	FREE_GUI( myBook->yesno );
}

void OnBackBcfgGui( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BCFG_DATA* bdata = &mbk->bdata;
	CFG_HDR* hp;
	if( bdata->level )
	{
		hp = bdata->levelstack[ bdata->level ];
		bdata->levelstack[ bdata->level ] = NULL;
		bdata->level -- ;
		FREE_GUI( mbk->bcfg );
		create_ed( bk, hp );
	}
	else
	{
		if( mbk->old_crc == Crc32( bdata->cfg, bdata->size_cfg ) )
		{
			FreeBook( bk );
		}
		else
		{
			mbk->yesno = CreateYesNoQuestionVA( 0,
											   VAR_BOOK( mbk ),
											   VAR_YESNO_PRE_QUESTION( mbk->changes_have_been_made ),
											   VAR_YESNO_QUESTION( mbk->save_before_exit ),
											   0 );
			GUIObject_SoftKeys_SetAction( mbk->yesno, ACTION_YES, OnYesExitGui );
			GUIObject_SoftKeys_SetAction( mbk->yesno, ACTION_NO, OnNoExitGui );
			GUIObject_SoftKeys_SetAction( mbk->yesno, ACTION_BACK, OnBackExitGui );
		}
	}
}

void OnCloseCBoxGui( BOOK* bk, GUI* )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	FREE_GUI( myBook->cbox_gui );
}

void OnSelectCBoxGui( BOOK* bk, GUI* )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	int item = OneOfMany_GetSelected( myBook->cbox_gui );

	if( item < myBook->cur_hp.cbox->max )
		myBook->cur_hp.cbox->selected = item;

	FREE_GUI( myBook->cbox_gui );
	RefreshEdList(bk);
}

void CreateCBoxGui( MyBOOK* myBook )
{
	GUI_ONEOFMANY* om = CreateOneOfMany( myBook );

	wchar_t ustr[64];
	myBook->cbox_gui = om;
	win12512unicode( ustr, myBook->cur_hp.cbox->name, MAXELEMS( ustr ) - 1 );
	GUIObject_SetTitleText( om, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ) );
	TEXTID* strid = new TEXTID[ myBook->cur_hp.cbox->max ];

	for ( int i = 0; i < myBook->cur_hp.cbox->max; i ++ )
	{
		win12512unicode( ustr, myBook->cur_hp.cbox->items[i].cbox_text, MAXELEMS( ustr ) - 1 );
		strid[i] = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	}

	OneOfMany_SetTexts( om, strid, myBook->cur_hp.cbox->max );
	delete strid;
	OneOfMany_SetChecked( om, myBook->cur_hp.cbox->selected );
	GUIObject_SoftKeys_SetAction( om, ACTION_BACK, OnCloseCBoxGui );
	GUIObject_SoftKeys_SetAction( om, ACTION_SELECT1, OnSelectCBoxGui );
	GUIObject_Show( om );
}


void OnBackCreateTextInputGui( BOOK* bk, u16* string, int len )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	FREE_GUI( myBook->text_input );
}

void OnOkCreateUnsignedNumberGui( BOOK* bk, wchar_t* string, int len )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	wchar_t ustr[64];
	unsigned int ui;
	*_Geterrno() = 0;
	ui = wcstoul( string, 0, 10 );
	if( ui < myBook->cur_hp.unsignedint->min || ui > myBook->cur_hp.unsignedint->max || *_Geterrno() == ERANGE )
	{
		snwprintf( ustr, MAXELEMS( ustr ) - 1, L"min: %u\nmax: %u", myBook->cur_hp.unsignedint->min, myBook->cur_hp.unsignedint->max );
		MessageBox( EMPTY_TEXTID, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ), NOIMAGE, 1 , 5000, bk );
	}
	else
	{
		myBook->cur_hp.unsignedint->unsignedint = ui;
		FREE_GUI( myBook->text_input );
		RefreshEdList(bk);
	}
}


void CreateUnsignedNumberInput( MyBOOK* myBook )
{
	wchar_t ustr[64];
	TEXTID text, header_name;

	snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%u", myBook->cur_hp.unsignedint->unsignedint );
	text = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	win12512unicode( ustr, myBook->cur_hp.unsignedint->name, MAXELEMS( ustr ) - 1 );
	header_name = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	myBook->text_input = CreateStringInputVA( 0,
											 VAR_HEADER_TEXT( header_name ),
											 VAR_STRINP_MAX_LEN( getnumwidth( myBook->cur_hp.unsignedint->max ) ),
											 VAR_STRINP_MODE( IT_UNSIGNED_DIGIT ),
											 VAR_BOOK( myBook ),
											 VAR_STRINP_ENABLE_EMPTY_STR( 0 ),
											 VAR_STRINP_TEXT( text ),
											 VAR_PREV_ACTION_PROC( OnBackCreateTextInputGui ),
											 VAR_OK_PROC( OnOkCreateUnsignedNumberGui ),
											 0 );
}

void OnOkCreateSignedNumberGui( BOOK* bk, wchar_t* string, int len )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	wchar_t ustr[64];
	int i;
	*_Geterrno() = 0;
	i = wcstol( string, 0, 10 );
	if( i < (int) myBook->cur_hp.signedint->min || i > (int) myBook->cur_hp.signedint->max || *_Geterrno() == ERANGE )
	{
		snwprintf( ustr, MAXELEMS( ustr ) - 1, L"min: %d\nmax: %d", myBook->cur_hp.signedint->min, myBook->cur_hp.signedint->max );
		MessageBox( EMPTY_TEXTID, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ), NOIMAGE, 1 , 5000, bk );
	}
	else
	{
		myBook->cur_hp.signedint->signedint = i;
		FREE_GUI( myBook->text_input );
		RefreshEdList(bk);
	}
}

void CreateSignedNumberInput( MyBOOK* myBook )
{
	wchar_t ustr[64];
	TEXTID header_name;
	win12512unicode( ustr, myBook->cur_hp.signedint->name, MAXELEMS( ustr ) - 1 );
	header_name = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	myBook->text_input = CreateStringInputVA( 0,
											 VAR_HEADER_TEXT( header_name ),
											 VAR_STRINP_MODE( IT_INTEGER ),
											 VAR_BOOK( myBook ),
											 VAR_STRINP_MIN_INT_VAL( myBook->cur_hp.signedint->min ),
											 VAR_STRINP_MAX_INT_VAL( myBook->cur_hp.signedint->max ),
											 VAR_STRINP_SET_INT_VAL( myBook->cur_hp.signedint->signedint ),
											 VAR_PREV_ACTION_PROC( OnBackCreateTextInputGui ),
											 VAR_OK_PROC( OnOkCreateSignedNumberGui ),
											 0 );
}


void OnOkCreateWinOrPassGui( BOOK* bk, wchar_t* string, int len )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	wchar_t ustr[64];
	if( len < myBook->cur_hp.str->min || len > myBook->cur_hp.str->max )
	{
		snwprintf( ustr, MAXELEMS( ustr ) - 1, L"min_string_len: %d\nmax_string_len: %d", myBook->cur_hp.str->min, myBook->cur_hp.str->max );
		MessageBox( EMPTY_TEXTID, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ), NOIMAGE, 1 , 5000, bk );
	}
	else
	{
		unicode2win1251( &myBook->cur_hp.str->chars[0], string, myBook->cur_hp.str->max );
		FREE_GUI( myBook->text_input );
		RefreshEdList(bk);
	}
}

void CreateWinOrPassSI( MyBOOK* myBook, int is_pass )
{
	wchar_t* ustr;
	int len;
	TEXTID text, header_name;
	len = myBook->cur_hp.str->max;

	if( len < 63 )
		len = 63;

	ustr = new wchar_t[len + 1];
	win12512unicode( ustr, myBook->cur_hp.str->name, len );
	header_name = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	win12512unicode( ustr, &myBook->cur_hp.str->chars[0], len );
	text = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	myBook->text_input = CreateStringInputVA( 0,
											 VAR_HEADER_TEXT( header_name ),
											 VAR_STRINP_MIN_LEN( myBook->cur_hp.str->min ),
											 VAR_STRINP_MAX_LEN( myBook->cur_hp.str->max ),
											 VAR_STRINP_MODE( IT_STRING ),
											 VAR_BOOK( myBook ),
											 VAR_STRINP_TEXT( text ),
											 VAR_PREV_ACTION_PROC( OnBackCreateTextInputGui ),
											 VAR_OK_PROC( OnOkCreateWinOrPassGui ),
											 VAR_STRINP_IS_PASS_MODE( is_pass ),
											 0 );
	delete ustr;
}

void OnOkCreateUnicodeGui( BOOK* bk, wchar_t* string, int len )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	wchar_t ustr[64];
	if( len < myBook->cur_hp.wstr->min || len > myBook->cur_hp.wstr->max )
	{
		snwprintf( ustr, MAXELEMS( ustr ) - 1, L"min_string_len: %d\nmax_string_len: %d", myBook->cur_hp.wstr->min, myBook->cur_hp.wstr->max );
		MessageBox( EMPTY_TEXTID, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ), NOIMAGE, 1 , 5000, bk );
	}
	else
	{
		// unicode2win1251( ( char*)hp + sizeof( CFG_HDR ), string, hp->max );
		wstrncpy( &myBook->cur_hp.wstr->chars[0], string, myBook->cur_hp.wstr->max - 1 );
		FREE_GUI( myBook->text_input );
		RefreshEdList(bk);
	}
}

void UnicodeOnSelFile( BOOK* bk, GUI* )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	myBook->type = SFILE;
	wchar_t* str;
	u16 len;
	StringInput_GetStringAndLen( myBook->text_input, &str, &len );
	myBook->selectf = CreateFileFolderSelect( myBook, str );
}

void UnicodeOnSelFolder( BOOK* bk, GUI* )
{
	MyBOOK* myBook = (MyBOOK*) bk;
	myBook->type = SFOLDER;
	wchar_t* str;
	u16 len;
	StringInput_GetStringAndLen( myBook->text_input, &str, &len );
	myBook->selectf = CreateFileFolderSelect( myBook, str );
}

void CreateUnicodeSI( MyBOOK* myBook, int is_pass )
{
	wchar_t* ustr;
	int len;
	TEXTID text, header_name, sel_file, sel_folder;
	int tmp=EMPTY_TEXTID;

	len = myBook->cur_hp.wstr->max;

	if( len < 63 )
		len = 63;

	ustr = new wchar_t[len + 1];
	win12512unicode( ustr, myBook->cur_hp.wstr->name, len );
	header_name = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	// win12512unicode( ustr, ( char*)hp + sizeof( CFG_HDR ), len );
	wstrncpy( ustr, &myBook->cur_hp.wstr->chars[0], len );
	text = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
	textidname2id( L"MSG_UI_MOVE_MESSAGE_SELECT_FOLDER_TXT", - 1, &tmp );
	if (tmp==EMPTY_TEXTID) tmp = TextID_Create(L"Select folder",ENC_UCS2,TEXTID_ANY_LEN);
	sel_folder = tmp;
	textidname2id( L"WAP_SELECT_FILE_TXT", - 1, &tmp );
	sel_file = tmp;
	myBook->text_input = CreateStringInputVA( 0,
											 VAR_HEADER_TEXT( header_name ),
											 VAR_STRINP_MIN_LEN( myBook->cur_hp.wstr->min ),
											 VAR_STRINP_MAX_LEN( myBook->cur_hp.wstr->max ),
											 VAR_STRINP_MODE( IT_STRING ),
											 VAR_BOOK( myBook ),
											 VAR_STRINP_TEXT( text ),
											 VAR_PREV_ACTION_PROC( OnBackCreateTextInputGui ),
											 VAR_OK_PROC( OnOkCreateUnicodeGui ),
											 VAR_STRINP_IS_PASS_MODE( is_pass ),
											 0 );
	GUIObject_SoftKeys_SetAction( myBook->text_input, 1, UnicodeOnSelFolder );
	GUIObject_SoftKeys_SetText( myBook->text_input, 1, sel_folder );
	StringInput_MenuItem_SetPriority( myBook->text_input, 0, 1 );
	GUIObject_SoftKeys_SetAction( myBook->text_input, 2, UnicodeOnSelFile );
	GUIObject_SoftKeys_SetText( myBook->text_input, 2, sel_file );
	StringInput_MenuItem_SetPriority( myBook->text_input, 0, 2 );
	delete ustr;
}

void OnSelect1GuiBcfg( BOOK* bk, GUI* )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BCFG_DATA* bdata = &mbk->bdata;
	int item = ListMenu_GetSelectedItem( mbk->bcfg );
	mbk->cur_hp.hdr = (CFG_HDR*) List_Get( mbk->list, item );
	switch( mbk->cur_hp.hdr->type )
	{
	case CFG_UINT:
		CreateUnsignedNumberInput( mbk );
		break;
	case CFG_INT:
		CreateSignedNumberInput( mbk );
		break;
	case CFG_STR_WIN1251:
		CreateWinOrPassSI( mbk, 0 );
		break;
	case CFG_CBOX:
		CreateCBoxGui( mbk );
		break;
	case CFG_STR_PASS:
		CreateWinOrPassSI( mbk, 1 );
		break;
	case CFG_COORDINATES:
		CreateEditCoordinatesGUI( mbk, 0 );
		break;
	case CFG_COLOR:
		CreateEditColorGUI( mbk, 0 );
		break;
	case CFG_LEVEL:
		bdata->level ++ ;
		bdata->levelstack[bdata->level] = mbk->cur_hp.hdr;
		GUIObject_Destroy( mbk->bcfg );
		mbk->bcfg = NULL;
		create_ed( bk, NULL );
		return;
	case CFG_CHECKBOX:
		mbk->cur_hp.checkbox->isChecked = !mbk->cur_hp.checkbox->isChecked;
		ListMenu_SetItemSecondLineText( mbk->bcfg, item, (mbk->cur_hp.checkbox->isChecked ? mbk->check_box_checked : mbk->check_box_unchecked) );
		break;
	case CFG_TIME:
		BookObj_CallPage( mbk, &bk_time_input );
		break;
	case CFG_DATE:
		BookObj_CallPage( mbk, &bk_date_input );
		break;
	case CFG_RECT:
		CreateEditCoordinatesGUI( mbk, 1 );
		break;
	case CFG_COLOR_INT:
		CreateEditColorGUI( mbk, 1 );
		break;
	case CFG_FONT:
		CreateFontSelectGUI( mbk, 5 );
		break;
	case CFG_KEYCODE:
		BookObj_CallPage( mbk, &bk_keycode_select );
		break;
	case CFG_UTF16_STRING:
		CreateUnicodeSI( mbk, 0 );
		break;
	default:
		return;
	}
}



TEXTID GetSubItemText( MyBOOK* myBook, CFG_HDR* hp )
{
	wchar_t ustr[64];
	TEXTID str_id = EMPTY_TEXTID;
	if( hp )
	{
		switch( hp->type )
		{
		case CFG_UINT:
			snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%u", ((CFG_HDR_UNSIGNEDINT*)hp)->unsignedint );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_INT:
			snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%d", ((CFG_HDR_SIGNEDINT*)hp)->signedint );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_STR_WIN1251:
			win12512unicode( ustr, &((CFG_HDR_STR*)hp)->chars[0], MAXELEMS( ustr ) - 1 );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_CBOX:
			win12512unicode( ustr, &((CFG_HDR_CBOX*)hp)->items[ ((CFG_HDR_CBOX*)hp)->selected ].cbox_text[0], MAXELEMS( ustr ) - 1 );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_STR_PASS:
			win12512unicode( ustr, "********", MAXELEMS( ustr ) - 1 );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_COORDINATES:
			snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%03d, %03d", ((CFG_HDR_POINT*)hp)->point.x, ((CFG_HDR_POINT*)hp)->point.y );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_COLOR:
			{
				snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%02X, %02X, %02X, %02X", ((CFG_HDR_COLOR*)hp)->rgb[0], ((CFG_HDR_COLOR*)hp)->rgb[1], ((CFG_HDR_COLOR*)hp)->rgb[2], ((CFG_HDR_COLOR*)hp)->rgb[3] );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_LEVEL:
			{
				win12512unicode( ustr, "[Enter]", MAXELEMS( ustr ) - 1 );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_CHECKBOX:
			str_id = ( ((CFG_HDR_CHECKBOX*)hp)->isChecked ? myBook->check_box_checked : myBook->check_box_unchecked );
			break;
		case CFG_TIME:
			{
				snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%02d:%02d", ((CFG_HDR_TIME*)hp)->time.hour, ((CFG_HDR_TIME*)hp)->time.min );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_DATE:
			{
				snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%02d.%02d.%04d", ((CFG_HDR_DATE*)hp)->date.day, ((CFG_HDR_DATE*)hp)->date.mon, ((CFG_HDR_DATE*)hp)->date.year );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_RECT:
			{
				snwprintf( ustr, MAXELEMS( ustr ) - 1, L"RC:%03d, %03d, %03d, %03d", ((CFG_HDR_RECT*)hp)->rect.x1, ((CFG_HDR_RECT*)hp)->rect.y1, ((CFG_HDR_RECT*)hp)->rect.x2, ((CFG_HDR_RECT*)hp)->rect.y2 );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_COLOR_INT:
			{
				unsigned int color = ((CFG_HDR_COLOR*)hp)->color;
				snwprintf( ustr, MAXELEMS( ustr ) - 1, L"%02X, %02X, %02X, %02X", COLOR_GET_R( color ), COLOR_GET_G( color ), COLOR_GET_B( color ), COLOR_GET_A( color ) );
				str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_FONT:
			{
				int platform=GetChipID()&CHIPID_MASK;
				if (platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350)
				{
					int n=1;
					int sp[5];
					sp[0] = TextID_CreateIntegerID (((CFG_HDR_FONT*)hp)->font&0xFF);
					int style_flags = ((CFG_HDR_FONT*)hp)->font>>8;
					if (style_flags&bold)
					{
						sp[n] = 0x78000000 + '_';
						sp[n+1] = 0x78000000 + 'B';
						n = n+2;
					}
					if (style_flags&italic)
					{
						sp[n] = 0x78000000 + '_';
						sp[n+1] = 0x78000000 + 'I';
						n = n+2;
					}
					str_id = TextID_Create(sp,ENC_TEXTID,n);
				}
				else str_id = TextID_Create( Font_GetNameByFontId( ((CFG_HDR_FONT*)hp)->font ), ENC_UCS2, TEXTID_ANY_LEN );
			}
			break;
		case CFG_KEYCODE:
			{
				TEXTID s_ids[3];
				s_ids[0] = KeyCode2Name( ((CFG_HDR_KEY*)hp)->keycode );
				s_ids[1] = ',' | 0x78000000;
				s_ids[2] = GetKeyModeName( ((CFG_HDR_KEY*)hp)->keymode );
				str_id = TextID_Create( s_ids, ENC_TEXTID, 3 );
			}
			break;
		case CFG_UTF16_STRING:
			str_id = TextID_Create( &((CFG_HDR_WSTR*)hp)->chars[0], ENC_UCS2, TEXTID_ANY_LEN );
			break;
		case CFG_STR_UTF8:
		case CFG_UTF8_STRING:
		default:
			snwprintf( ustr, MAXELEMS( ustr ) - 1, L"Type %d is not supporting yet", hp->type );
			str_id = TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN );
			break;
		}
	}
	return str_id;
}

// устанавливаем тексты в пунктах меню
int onLBMessage( GUI_MESSAGE* msg )
{
	MyBOOK* myBook = (MyBOOK*) GUIonMessage_GetBook( msg );

	switch( GUIonMessage_GetMsg( msg ) )
	{
		// onCreateListItem
	case LISTMSG_GetItem:
		int item = GUIonMessage_GetCreatedItemIndex( msg );
		CFG_HDR* hp = (CFG_HDR*) List_Get( myBook->list, item );
		wchar_t ustr[32];
		win12512unicode( ustr, hp->name, MAXELEMS( ustr ) - 1 );
		GUIonMessage_SetMenuItemText( msg, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ) );
		GUIonMessage_SetMenuItemInfoText( msg, TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ) );
		int str_id = GetSubItemText( myBook, hp );

		if( str_id == EMPTY_TEXTID )
			str_id = TextID_Create ( L"’рень", ENC_UCS2, TEXTID_ANY_LEN );

		GUIonMessage_SetMenuItemSecondLineText( msg, str_id );
	}
	return 1;
};


TEXTID GetParentName( BCFG_DATA* bdata )
{
	wchar_t ustr[32];
	if( bdata->level )
	{
		CFG_HDR* hp = bdata->levelstack[bdata->level];
		win12512unicode( ustr, hp->name, MAXELEMS( ustr ) - 1 );
	}
	else
	{
		wchar_t* ext = wstrrchr( bdata->name, '.' );
		int len = ext ? ext-bdata->name : MAXELEMS(ustr)-1;
		wstrncpy( ustr, bdata->name, len );
		ustr[len] = 0;
	}
	return TextID_Create( ustr, ENC_UCS2, MAXELEMS( ustr ) - 1 );
}

// создание меню
GUI_LIST* CreateGuiList( MyBOOK* bk, int set_focus )
{
	GUI_LIST* lo;
	lo = CreateListMenu( bk, 0 );
	bk->bcfg = lo;
	GUIObject_SetTitleText( lo, GetParentName( &bk->bdata ) );
	ListMenu_SetItemCount( lo, bk->list->FirstFree );
	ListMenu_SetOnMessage( lo, onLBMessage );
	ListMenu_SetCursorToItem( lo, set_focus );
	ListMenu_SetItemStyle( lo, 3 );
	ListMenu_SetItemTextScroll( lo, 1 );
	GUIObject_SoftKeys_SetAction( lo, ACTION_BACK, OnBackBcfgGui );
	GUIObject_SoftKeys_SetAction( lo, ACTION_LONG_BACK, OnBackBcfgGui );
	GUIObject_SoftKeys_SetAction( lo, ACTION_SELECT1, OnSelect1GuiBcfg );
	return lo;
};


void RefreshEdList(BOOK * bk)
{
	MyBOOK* mbk = (MyBOOK*) bk;
	int pos = ListMenu_GetSelectedItem(mbk->bcfg);
	ListMenu_DestroyItems(mbk->bcfg);
	ListMenu_SetItemCount( mbk->bcfg, mbk->list->FirstFree );
	ListMenu_SetCursorToItem( mbk->bcfg, pos );
	GUIObject_SoftKeys_SetVisible( mbk->bcfg, ACTION_SELECT1, 1 );
}


GUI* create_ed( BOOK* book, CFG_HDR* need_to_focus )
{
	MyBOOK* mbk = (MyBOOK*) book;
	BCFG_DATA* bdata = &mbk->bdata;
	unsigned char* p = bdata->cfg;
	int n = bdata->size_cfg;
	CFG_HDR* hp;
	int need_to_jump = 0;
	LIST* list = mbk->list;

	while( list->FirstFree )
		List_RemoveAt( list, 0 );

	GUI* gui = NULL;

	int i;
	unsigned int curlev = 0;
	CFG_HDR* parent = NULL;
	CFG_HDR* parents[16];

	bool error = false;

	while( !error && n >= sizeof( CFG_HDR ) )
	{
		hp = (CFG_HDR*) p;
		if( hp->type == CFG_LEVEL )
		{
			if( hp->min )
			{
				if( curlev == bdata->level && parent == bdata->levelstack[bdata->level] )
				{
					List_InsertLast( list, hp );
				}
			}
			else if( curlev )
			{
				parent = parents[curlev -- ];
			}
		}
		else
		{
			if( curlev == bdata->level && parent == bdata->levelstack[bdata->level] )
			{
				List_InsertLast( list, hp );
			}
		}
		n -= sizeof( CFG_HDR );
		p += sizeof( CFG_HDR );

		switch( hp->type )
		{
		case CFG_UINT:
			n -= sizeof( unsigned int );
			if( n < 0 )
				error = true;
			else
				p += sizeof( unsigned int );
			break;
		case CFG_INT:
			n -= sizeof(int) ;
			if( n < 0 )
				error = true;
			else
				p += sizeof(int) ;
			break;
		case CFG_STR_UTF8:
			n -= ( hp->max + 1 + 3 )&( ~3 );
			if( n < 0 )
				error = true;
			else
				p += ( hp->max + 1 + 3 )&( ~3 );
			break;
		case CFG_STR_WIN1251:
			n -= ( hp->max + 1 + 3 )&( ~3 );
			if( n < 0 )
				error = true;
			else
				p += ( hp->max + 1 + 3 )&( ~3 );
			break;

		case CFG_UTF8_STRING:
			n -= ( hp->max + 1 + 3 )&( ~3 );
			if( n < 0 )
				error = true;
			else
				p += ( hp->max + 1 + 3 )&( ~3 );
			break;

		case CFG_CBOX:
			n -= hp->max * sizeof( CFG_CBOX_ITEM ) + 4;
			if( n < 0 )
				error = true;
			else
			{
				i =* ( ( int*)p );
				if( i >= hp->max )
					error = true;
				else
					p += hp->max * sizeof( CFG_CBOX_ITEM ) + 4;
			}
			break;
		case CFG_STR_PASS:
			n -= ( hp->max + 1 + 3 )&( ~3 );
			if( n < 0 )
				error = true;
			else
				p += ( hp->max + 1 + 3 )&( ~3 );
			break;
		case CFG_COORDINATES:
			n -= 8;
			if( n < 0 )
				error = true;
			else
				p += 8;
			break;
		case CFG_COLOR:
			n -= 4;
			if( n < 0 )
				error = true;
			else
				p += 4;
			break;
		case CFG_LEVEL:
			if( n < 0 )
			{
				error = true;
				break;
			}
			if( hp->min )
			{
				if( curlev == bdata->level && parent == bdata->levelstack[bdata->level] )
				{
					if( need_to_focus )
					{
						if( need_to_focus == hp )
							need_to_jump = list->FirstFree - 1;
					}
				}
				curlev ++ ;
				parents[curlev] = parent;
				parent = hp;
			}
			continue;
		case CFG_CHECKBOX:
			n -= sizeof(int) ;
			if( n < 0 )
				error = true;
			else
				p += sizeof(int) ;
			break;

		case CFG_TIME:
			n -= sizeof( TIME );
			if( n < 0 )
				error = true;
			else
				p += sizeof( TIME );
			break;

		case CFG_DATE:
			n -= sizeof( DATE );
			if( n < 0 )
				error = true;
			else
				p += sizeof( DATE );
			break;

		case CFG_RECT:
			n -= sizeof( RECT );
			if( n < 0 )
				error = true;
			else
				p += sizeof( RECT );
			break;

		case CFG_COLOR_INT:
			n -= sizeof(int) ;
			if( n < 0 )
				error = true;
			else
				p += sizeof(int) ;
			break;
		case CFG_FONT:
			n -= sizeof(int) ;
			if( n < 0 )
				error = true;
			else
				p += sizeof(int) ;
			break;
		case CFG_KEYCODE:
			n -= 2* sizeof(int) ;
			if( n < 0 )
				error = true;
			else
				p += 2* sizeof(int) ;
			break;
		case CFG_UTF16_STRING:
			n -= ( ( ( hp->max + 1 )* 2 ) + 3 )&( ~3 );
			if( n < 0 )
				error = true;
			else
				p += ( ( ( hp->max + 1 )* 2 ) + 3 )&( ~3 );
			break;
		default:
			error = true;
			break;
		}
	}

	gui = CreateGuiList( mbk, need_to_jump );
	GUIObject_Show( gui );
	return gui;
}

static int RecreateEdPage( void* mess , BOOK* bk )
{
	RefreshEdList(bk);
	return 1;
}


static int CreateEdPageOnCreate( void* mess , BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BCFG_DATA* bdata = &mbk->bdata;
	mbk->old_crc = Crc32( bdata->cfg, bdata->size_cfg );
	create_ed( mbk, 0 );
	return 1;
}

const PAGE_MSG bk_msglst_editor[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, CreateEdPageOnCreate,
	ACCEPT_EVENT_TAG, RecreateEdPage,
	NIL_EVENT_TAG, NULL
};

const PAGE_DESC bk_editor = { "BcfgEdit_CreateEditor_Page", 0, bk_msglst_editor };



static int TerminateElf( void* , BOOK* book )
{
	UI_Event_toSID( TERMINATE_SESSION_EVENT, BookObj_GetSessionID( book ) );
	FreeBook( book );
	return 1;
}


static int ShowAuthorInfo( void* mess , BOOK* book )
{
	MSG* msg = ( MSG*)mess;
	wchar_t ustr[64];
	snwprintf( ustr, MAXELEMS( ustr ) - 1, L"\nBcfgEdit v1.0\nRevision %d\n( c ) Rst7, KreN, Hussein", __SVN_REVISION__ );
	MessageBox( EMPTY_TEXTID, TextID_Create( ustr, ENC_UCS2, MAXELEMS( ustr ) - 1 ), NOIMAGE, 1 , 5000, msg->book );
	return 1;
}

int SelBcfg_BcfgFilter( const wchar_t* ExtList, const wchar_t* ItemPath, const wchar_t* ItemName )
{
	return DataBrowser_isFileInListExt( ExtList, ItemPath, ItemName );
}

static int SelBcfgPageOnCreate( void* , BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	static char actions[4];
	void* DB_Desc = DataBrowserDesc_Create();

	const wchar_t* folder_list[2];
	folder_list[0] = GetDir( DIR_ELFS_CONFIG|MEM_INTERNAL );
	folder_list[1] = GetDir( DIR_ELFS_CONFIG|MEM_EXTERNAL );
	DataBrowserDesc_SetHeaderText( DB_Desc, TextID_Create( L"Config", ENC_UCS2, TEXTID_ANY_LEN ) );
	DataBrowserDesc_SetBookID( DB_Desc, BookObj_GetBookID( mbk ) );
	DataBrowserDesc_SetFolders( DB_Desc, folder_list );
	DataBrowserDesc_SetFoldersNumber( DB_Desc, 2 );

	DataBrowserDesc_SetSelectAction( DB_Desc, 1 );
	DataBrowserDesc_SetFileExtList( DB_Desc, L"*.bcfg" );
	DataBrowserDesc_SetItemFilter( DB_Desc, SelBcfg_BcfgFilter );

	actions[0] = DB_CMD_DELETE;
	actions[1] = DB_CMD_LAST;
	DataBrowserDesc_SetActions( DB_Desc, actions );

	DataBrowser_Create( DB_Desc );
	DataBrowserDesc_Destroy( DB_Desc );
	return 1;
}

static int SelBcfgPageOnAccept( void* data, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	FILEITEM* file = ( FILEITEM*)data;
	BCFG_DATA* bdata = &mbk->bdata;
	wstrncpy( bdata->path, FILEITEM_GetPath( file ), MAXELEMS( bdata->path ) );
	wstrncpy( bdata->name, FILEITEM_GetFname( file ), MAXELEMS( bdata->name ) );
	if( LoadCfg( bdata ) )
	{
		BookObj_GotoPage( mbk, &bk_editor );
	}
	else
	{
		FreeBook( mbk );
	}
	return 1;
}

static int SelBcfgPageOnCancel( void* data, BOOK* bk )
{
	FreeBook( bk );
	return 1;
}

const PAGE_MSG bk_msglst_selbcfg[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, SelBcfgPageOnCreate,
	ACCEPT_EVENT_TAG, SelBcfgPageOnAccept,
	CANCEL_EVENT_TAG, SelBcfgPageOnCancel,
	PREVIOUS_EVENT_TAG, SelBcfgPageOnCancel,
	NIL_EVENT_TAG, NULL
};

const PAGE_DESC bk_selbcfg = { "BcfgEdit_SelectBcfg_Page", 0, bk_msglst_selbcfg };

static int MainPageOnCreate( void* , BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	BCFG_DATA* bdata = &mbk->bdata;
	mbk->list = List_Create();

	textidname2id( IDN_CHANGES_HAVE_BEEN_MADE, - 1, &mbk->changes_have_been_made );
	textidname2id( IDN_SAVE_BEFORE_EXIT, - 1, &mbk->save_before_exit );
	textidname2id( IDN_CHECKBOX_UNCHECKED_ICON, - 1, &mbk->check_box_unchecked );
	textidname2id( IDN_CHECKBOX_CHECKED_ICON, - 1, &mbk->check_box_checked );
	if( *bdata->path == '/' ) // ¬еро€тно передали указатель на файл
	{
		if( LoadCfg( bdata ) )
		{
			BookObj_GotoPage( mbk, &bk_editor );
		}
		else
		{
			FreeBook( mbk );
		}
	}
	else
	{
		BookObj_GotoPage( mbk, &bk_selbcfg );
	}
	return 1;
}

const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, MainPageOnCreate,
	NIL_EVENT_TAG, NULL
};

const PAGE_DESC bk_main = { "BcfgEdit_Main_Page", 0, bk_msglst_main };


// при закрытии книги
static void onMyBookClose( BOOK* book )
{
	MyBOOK* mbk = (MyBOOK*) book;
	Free_FLIST();
	delete mbk->bdata.cfg;
	List_Destroy( mbk->list );
	SUBPROC( elf_exit );
}

int isBcfgEditBook( BOOK* struc )
{
	return struc->onClose == onMyBookClose;
}


int main( wchar_t* elfname, wchar_t* path, wchar_t* fname )
{
	MyBOOK* myBook = new MyBOOK;
	BCFG_DATA* bdata = &myBook->bdata;
	memset( myBook, 0, sizeof( MyBOOK ) );
	if( path && fname )
	{
		wstrncpy( bdata->path, path, MAXELEMS( bdata->path ) );
		wstrncpy( bdata->name, fname, MAXELEMS( bdata->name ) );
	}
	if( ! CreateBook( myBook, onMyBookClose, &bk_base, "BcfgEdit", - 1, 0 ) )
	{
		delete myBook;
		SUBPROC( elf_exit );
		return 0;
	}
	BookObj_GotoPage( (BOOK*) myBook, &bk_main );
	return 0;
}
