#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"

#include "..\include\cfg_items.h"
#include "main.h"
#include "datetime.h"

void AcceptTIinput( BOOK* bk, GUI* data )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	unsigned int i = TimeInput_GetTimeInt( data );

	mbk->cur_hp.time->time.hour = TI_GET_HOUR( i );
	mbk->cur_hp.time->time.min = TI_GET_MIN( i );
	mbk->cur_hp.time->time.sec = TI_GET_SEC( i );

	BookObj_ReturnPage( bk, ACCEPT_EVENT );
}

void CloseTIinput( BOOK* bk, GUI*)
{
	BookObj_ReturnPage( bk, PREVIOUS_EVENT );
}

static int TI_OnEnter( void*, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	wchar_t ustr[64];

	int time_format = 4;
	char time_f;
	int icon_id;

	if ( !REQUEST_TIMEFORMAT_GET( SYNC, &time_f ) )
		time_format = time_f;

	win12512unicode( ustr, mbk->cur_hp.time->name, MAXELEMS( ustr )-1 );

	mbk->tinput = CreateTimeInputVA( 0,
									VAR_HEADER_TEXT( TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ) ),
									VAR_TIMEINP_TIMEFORMAT( time_format ),
									VAR_TIMEINP_TIME( &mbk->cur_hp.time->time ),
									VAR_BOOK( mbk ),
									0 );

	GUIObject_SoftKeys_SetAction( mbk->tinput, ACTION_ACCEPT, AcceptTIinput );
	GUIObject_SoftKeys_SetText( mbk->tinput, ACTION_ACCEPT, STR( "OK" ) );
	GUIObject_SoftKeys_SetVisible( mbk->tinput, ACTION_ACCEPT, 1 );
	GUIObject_SoftKeys_SetAction( mbk->tinput, ACTION_BACK, CloseTIinput );

	if ( iconidname2id( IDN_TIME_INPUT_ICON, -1, &icon_id ) )
		GUIInput_SetIcon( mbk->tinput, icon_id );

	return 1;
}

static int TI_OnExit( void*, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	FREE_GUI( mbk->tinput );
	return 1;
}
// ----------------------------------------------------------------
void AcceptDIinput( BOOK* bk, GUI* data )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	unsigned int i = DateInput_GetDateInt( data );

	mbk->cur_hp.date->date.year = DI_GET_YEAR( i );
	mbk->cur_hp.date->date.mon = TI_GET_MONTH( i );
	mbk->cur_hp.date->date.day = TI_GET_DAY( i );

	BookObj_ReturnPage( bk, ACCEPT_EVENT );
}

void CloseDIinput( BOOK* bk, GUI* )
{
	BookObj_ReturnPage( bk, PREVIOUS_EVENT );
}

static int DI_OnEnter( void*, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;

	wchar_t ustr[64];

	int date_format = 4;
	char date_f;
	int icon_id;

	if ( !REQUEST_DATEFORMAT_GET( SYNC, &date_f ) )
		date_format = date_f;

	win12512unicode( ustr, mbk->cur_hp.date->name, MAXELEMS( ustr )-1 );

	mbk->dinput = CreateDateInputVA( 0,
									VAR_HEADER_TEXT( TextID_Create( ustr, ENC_UCS2, TEXTID_ANY_LEN ) ),
									VAR_DATEINP_DATEFORMAT( date_format ),
									VAR_DATEINP_DATE( &mbk->cur_hp.date->date ),
									VAR_BOOK( mbk ),
									0 );

	GUIObject_SoftKeys_SetAction( mbk->dinput, ACTION_ACCEPT, AcceptDIinput );
	GUIObject_SoftKeys_SetText( mbk->dinput, ACTION_ACCEPT, STR( "OK" ) );
	GUIObject_SoftKeys_SetVisible( mbk->dinput, ACTION_ACCEPT, 1 );
	GUIObject_SoftKeys_SetAction( mbk->dinput, ACTION_BACK, CloseDIinput );

	if ( iconidname2id( IDN_DATE_INPUT_ICON, -1, &icon_id ) )
		GUIInput_SetIcon( mbk->dinput, icon_id );

	return 1;
}

static int DI_OnExit( void*, BOOK* bk )
{
	MyBOOK* mbk = (MyBOOK*) bk;
	FREE_GUI( mbk->dinput );
	return 1;
}

const PAGE_MSG bk_msglst_timeinput[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, TI_OnEnter,
	PAGE_EXIT_EVENT_TAG, TI_OnExit,
	NIL_EVENT_TAG, NULL
};

const PAGE_MSG bk_msglst_dateinput[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, DI_OnEnter,
	PAGE_EXIT_EVENT_TAG, DI_OnExit,
	NIL_EVENT_TAG, NULL
};

const PAGE_DESC bk_time_input = { "BcfgEdit_TimeInput_Page", 0, bk_msglst_timeinput };
const PAGE_DESC bk_date_input = { "BcfgEdit_DateInput_Page", 0, bk_msglst_dateinput };
