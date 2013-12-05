#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\var_arg.h"
#include "book_names.h"
#include "main.h"


//============= pages start ======================

const PAGE_MSG ChangeName_PageEvents[]@ "DYN_PAGE" = {
	PAGE_ENTER_EVENT_TAG, CreateSI,
	PAGE_EXIT_EVENT_TAG, ExitChangeName,
	NIL_EVENT_TAG, 0
};

PAGE_DESC ChangeName_page = { "BookManager_ChangeName_Page", 0, ChangeName_PageEvents };

//============= pages end ======================


char * GetOriginalBookName(MyBOOK * mbk)
{
	BOOK_LIST_ITEM* elem = GetBookListItem(mbk,mbk->ActiveTAB);
	return(elem->book_name);
}


void onCancel_SI(BOOK* book, void* )
{
	BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void onPrevious_SI(BOOK* book, void* )
{
	BookObj_ReturnPage( book, ACCEPT_EVENT );
}


void onAccept_SI( BOOK * book, wchar_t * new_name, int len )
{
	MyBOOK* mbk = (MyBOOK*) book;
	int f;
	wchar_t * pos;
	wchar_t * pos_uni_pair;
	wchar_t cur_name[MAX_BOOK_NAME_LEN+1];
	wchar_t orig_name[MAX_BOOK_NAME_LEN+1];

	wchar_t * path = get_path();

	str2wstr(orig_name,GetOriginalBookName(mbk));

	pos_uni_pair = GetUserBookName(mbk->booknames_buf, orig_name, cur_name);

	int new_name_len = wstrlen(new_name);
	int cur_name_len = wstrlen(cur_name);
	int orig_name_len = wstrlen(orig_name);

	if ( (!len) || ( (!wstrcmp(orig_name,new_name)) && (new_name_len==orig_name_len) ) )
	{
		if ( pos_uni_pair )
		{
			if ( ( f = _fopen( path, INI_BOOK_NAMES, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0) ) >= 0 )
			{
				//Delete
				pos = wstrwstr(pos_uni_pair, L"\r\n") + sizeof("\r\n") - sizeof("");

				fwrite( f, mbk->booknames_buf, (pos_uni_pair - mbk->booknames_buf) * sizeof(wchar_t) );     //пишем начало файла
				fwrite( f, pos, mbk->booknames_buf_size - (pos - mbk->booknames_buf) * sizeof(wchar_t) );     //пишем остаток файла
				fclose( f );
			}
		}
	}
	else
	{
		if ( (wstrcmp(cur_name,new_name)) || ( (!wstrcmp(cur_name,new_name)) && (new_name_len!=cur_name_len) ) )
		{
			if ( pos_uni_pair )
			{
				if ( ( f = _fopen( path, INI_BOOK_NAMES, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
				{
					//ReWrite
					pos = pos_uni_pair + wstrlen(orig_name) + sizeof(": ") - sizeof("");
					int len_minus = wstrlen( cur_name );    //длина старого названия

					fwrite( f, mbk->booknames_buf, (pos - mbk->booknames_buf) * sizeof(wchar_t) );     //пишем начало файла
					fwrite( f, new_name, new_name_len * sizeof(wchar_t) );      //пишем новое название
					fwrite( f, pos + len_minus, mbk->booknames_buf_size - (( pos - mbk->booknames_buf ) + len_minus ) * sizeof(wchar_t));      //пишем остаток файла
					fclose( f );
				}
			}
			else
			{
				if ( ( f = _fopen( path, INI_BOOK_NAMES, FSX_O_RDWR|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 ) ) >= 0 )
				{
					//Append
					fwrite(f, mbk->booknames_buf, mbk->booknames_buf_size);     //пишем старый файл
					fwrite(f, orig_name, wstrlen(orig_name) * sizeof(wchar_t));
					fwrite(f, L": ", (sizeof(L": ") - sizeof(wchar_t)));
					fwrite(f, new_name, wstrlen(new_name) * sizeof(wchar_t));
					fwrite(f, L"\r\n", (sizeof(L"\r\n") - sizeof(wchar_t)));
					fclose(f);
				}
			}
		}
	}

	delete path;

	LoadBookNames(mbk);

	BookObj_ReturnPage( mbk, ACCEPT_EVENT );
}



int CreateSI( void* data, BOOK* book )
{
	MyBOOK* mbk = (MyBOOK*) book;
	TEXTID editable_strID = GetUserBookNameTEXTID(GetOriginalBookName(mbk));

	mbk->StringInput = CreateStringInputVA( 0,
										   VAR_PREV_ACTION_PROC( onPrevious_SI ),
										   VAR_LONG_BACK_PROC( onCancel_SI ),
										   VAR_BOOK( book ),
										   VAR_OK_PROC( onAccept_SI ),
										   VAR_STRINP_TEXT( editable_strID ),
										   VAR_STRINP_MIN_LEN( 0 ),
										   VAR_STRINP_MAX_LEN( MAX_BOOK_NAME_LEN ),
										   0 );
	return 0;
}


int ExitChangeName( void* data, BOOK* book )
{
	MyBOOK* mbk = (MyBOOK*) book;
	if (mbk->StringInput)
	{
		GUIObject_Destroy(mbk->StringInput);
		mbk->StringInput=0;
	}
	return 0;
}


void BookNames( BOOK* book, GUI* )
{
	MyBOOK * mbk = (MyBOOK*)book;
	mbk->ActiveTAB=GetActiveTab(mbk);
	BookObj_CallPage( book, &ChangeName_page );
}
