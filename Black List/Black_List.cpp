#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\include\var_arg.h"
#include "Black_List.h"
#include "Pages.h"

wchar_t * SET_CALLS_TXT;
wchar_t * SET_SMS_DIG_TXT;
wchar_t * SET_SMS_TEXT_TXT;
wchar_t * SET_CONTROL_TXT;
wchar_t * SET_BLOCK_TYPE_TXT;
wchar_t * SET_BLOCK_MODE_TXT;
wchar_t * SET_LOG_TXT;
wchar_t * SET_NOTIFICATION_TXT;
wchar_t * SET_LANGUAGE_TXT;
wchar_t * STATE_ON_TXT;
wchar_t * STATE_OFF_TXT;
wchar_t * BLOCK_TYPE_BUSY_TXT;
wchar_t * BLOCK_TYPE_OUT_OF_SERVICE_TXT;
wchar_t * BLOCK_MODE_ALL_TXT;
wchar_t * BLOCK_MODE_BL_TXT;
wchar_t * BLOCK_MODE_WL_TXT;
wchar_t * SETTINGS_TXT;
wchar_t * EDIT_LISTS_TXT;
wchar_t * NUMBER_EXISTS_TXT;
wchar_t * ILLEGAL_SYMBOLS_ERROR_TXT;
wchar_t * ENTER_NUMBER_TXT;
wchar_t * ENTER_MASK_TXT;
wchar_t * EMPTY_TXT;
wchar_t * FROM_PB_TXT;
wchar_t * MANUAL_TXT;
wchar_t * UNKNOWN_PNUM_TXT;
wchar_t * MASK_TXT;
wchar_t * LANGUAGE_EN_TXT;
wchar_t * LANGUAGE_RU_TXT;
wchar_t * BLOCKED_TXT;
wchar_t * BLOCKED_TYPE_SMS_TXT;
wchar_t * BLOCKED_TYPE_CALL_TXT;
wchar_t * ALL_UNKNOWN_TXT;
wchar_t * ALL_UNKNOWN_SECOND_LINE_TXT;
wchar_t * ALL_KNOWN_TXT;
wchar_t * ALL_KNOWN_SECOND_LINE_TXT;

char * end_of_line="\r\n";
char * accept_digits="0123456789";
char * accept_mask="0123456789*?";


//===========================List func=============================

int ConcatList_Insert_Filter(void * elem1,void * elem2)
{
	sms_concat_part* elem_to_ins = (sms_concat_part*)elem1;
	sms_concat_part* cur_elem = (sms_concat_part*)elem2;
	
	if (cur_elem->part_ID < elem_to_ins->part_ID)
		return 1;
	else
		return -1;
}


int List_Find_sms_part_cmp(void *r0,void *r1)
{
	char partID = (int)r1;
	sms_concat_part* sms_part_elem = (sms_concat_part*)r0;

	if (sms_part_elem->part_ID == partID) return 0;
	
	return 1;
}


int List_Find_sms_concat_cmp(void *r0,void *r1)
{
	wchar_t smsID = (int)r1;
	sms_concat* sms_concat_elem = (sms_concat*)r0;

	if (sms_concat_elem->unique_smsID == smsID) return 0;
	
	return 1;
}


int elem_filter( void* elem )
{
	return elem!= NULL;
}


void sms_concat_part_elem_free( void* elem )
{
	sms_concat_part* elem_sms_part = (sms_concat_part*)elem;
	
	delete( elem_sms_part->text );
	delete( elem_sms_part );
}


void sms_concat_elem_free( void* elem )
{
	sms_concat* elem_sms = (sms_concat*)elem;
	
	List_DestroyElements(elem_sms->parts_list, elem_filter, sms_concat_part_elem_free);
	List_Destroy( elem_sms->parts_list );
	
	delete ( elem_sms );
}


void filter_list_elem_free( void* elem )
{
	filter_list_elem* elem_to_free = (filter_list_elem*)elem;
	delete (elem_to_free->data);
	delete (elem_to_free);
}

//==========================================================

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int TerminateElf(void * ,BOOK * BLBook)
{
	FreeBook(BLBook);
	return 1;
}


int ShowAuthorInfo(void *mess ,BOOK * BLBook)
{
	MSG * msg = (MSG*)mess;
	MessageBox( EMPTY_TEXTID, STR("Black List, v2.01\n\n(c) IronMaster"), NOIMAGE, 1, 5000, msg->book );
	return 1;
}


int gsm7_to_ascii(unsigned char* gsm,unsigned char* ascii,int gsm_size)
{
	int i;
	int gsm_count=0;
	int ascii_count=0;
	
	while (gsm_count < gsm_size)
	{
		for (i=0; i < 7 && gsm_count < gsm_size; i++)
		{
			
			unsigned char x = gsm[gsm_count];
			unsigned char ascii_chr_1 = (x<<(1+i)&0xFF)>>1;//(x<<(25+i))>>25;
			
			if (i == 0)
				ascii[ascii_count]=ascii_chr_1;
			else
			{
				unsigned char pre_x = gsm[gsm_count-1];
				unsigned char ascii_chr_2 = pre_x>>(8-i);//(pre_x<<24)>>(32-i);
				ascii[ascii_count]=ascii_chr_1 | ascii_chr_2;
			}
			
			ascii_count++;
			gsm_count++;
		}

		if (i == 7)
		{
			ascii[ascii_count]=gsm[gsm_count-1]>>1;//(gsm[gsm_count-1]<<24)>>25;
			ascii_count++;
		}
	}
	ascii[ascii_count]=0;
	
	return ascii_count;
}


void ascii_to_gsm7(char* ascii,char* gsm,int size)
{
}


char toupper(char chr)
{
	if (chr >= 'a' & chr <= 'z') return chr-0x20;
	return chr;
}


int strcmpi(char* str1, char* str2)
{
	while (*str1 != 0)
	{
		if (toupper(*str1) != toupper(*str2)) return -1;
		str1++;
		str2++;
	}
	
	if (*str2 != 0) return -1;
	
	return 0;
}


//0 - all chars in accept, -1 - where are symbols not from accept
int my_strspn(char* str1, char* accept)
{
	char* charset_to_cmp;
	char* str_to_cmp = str1;
	
	while (*str_to_cmp != 0)
	{
		charset_to_cmp = accept;
		while (*charset_to_cmp != 0)
		{
			if (*str_to_cmp == *charset_to_cmp) break;
			charset_to_cmp++;
		}
		
		if (*charset_to_cmp == 0) return -1;
		str_to_cmp++;
	}
	
	return 0;
}


int str_mask_cmp(char* str,char* mask)
{
	char* post_star_pos=0;
	
	while (*str != 0)
	{
		if (*mask == 0)	return -1;
		
		if (*mask == '?' || toupper(*str) == toupper(*mask))
		{
			str++;
			mask++;
		}
		else
		{
			if (*mask != '*')
			{
				if (post_star_pos)
				{
					mask = post_star_pos;
					str++;
				}
				else return -1;
			}
			else
			{
				if (*(mask+1) == 0) return 0;
				
				post_star_pos = mask+1;
				mask++;
			}
		}
	}
	
	if (*mask == 0 || (*mask == '*' && *(mask+1) == 0)) return 0;
	
	return -1;
}


void CheckUnk(int BLBook1, void * number1)
{
	BlackListBook * BLBook = (BlackListBook*) BLBook1;
	char* number = (char*)number1;
	
	char pnum[50];
	str2PNUM(pnum,number,50,50);
	TEXTID pnum_name = PNUM2Name(pnum,0,0);
	
	if (pnum_name==EMPTY_TEXTID) BLBook->isUnk = TRUE;
	else
	{
		BLBook->isUnk = FALSE;
		TextID_Destroy(pnum_name);
	}
	
	delete number;
}


void ShowMsgBox(int number1,void* type1)
{
	char * number = (char*)number1;
	int type = (int)type1;
	TEXTID text_buf[7];
	
	if (type == NOTIFIC_TYPE_CALL)
	{
		text_buf[0] = TextID_Create( BLOCKED_TYPE_CALL_TXT, ENC_UCS2, TEXTID_ANY_LEN );
	}
	else
	{
		text_buf[0] = TextID_Create( BLOCKED_TYPE_SMS_TXT, ENC_UCS2, TEXTID_ANY_LEN );
	}
	text_buf[1] = 0x7800000A;
	text_buf[2] = 0x7800000A;
	
	if (type == NOTIFIC_TYPE_SMS_TEXT)
	{
		text_buf[3] = TextID_Create( number, ENC_LAT1, TEXTID_ANY_LEN );
	}
	else
	{
		char pnum[50];
		str2PNUM(pnum,number,50,50);
		TEXTID pnum_name = PNUM2Name( pnum, TRUE, 0 );
		if (pnum_name==EMPTY_TEXTID)
			text_buf[3] = TextID_Create( number, ENC_LAT1, TEXTID_ANY_LEN );
		else
			text_buf[3] = pnum_name;
	}
	
	text_buf[4] = 0x7800000A;
	text_buf[5] = 0x7800000A;
	text_buf[6] = TextID_Create( BLOCKED_TXT, ENC_UCS2, TEXTID_ANY_LEN );
	
	MessageBox( EMPTY_TEXTID, TextID_Create( text_buf, ENC_TEXTID, 7 ), NOIMAGE, 1, 5000, 0 );
	
	delete number;
}


void wstr_remove_null(wchar_t* str, int size)
{
	int i;
	int count=0;
	
	for (i=0; i < size; i++)
	{
		if (str[i] != 0)
		{
			str[count] = str[i];
			count++;
		}
	}
	str[count] = 0;
}


void str_remove_null(char* str, int size)
{
	int i;
	int count=0;
	
	for (i=0; i < size; i++)
	{
		if (str[i] != 0)
		{
			str[count] = str[i];
			count++;
		}
	}
	str[count] = 0;
}


void WriteLog_SMS(int pdu_in, void* book)
{
	BlackListBook * BLBook = (BlackListBook*) book;
	char* pdu = (char*)pdu_in;
	
	int i;
	int j;
	int concat_sms_need_to_log = FALSE;
	int concat_sms_index;
	
	char is_tp_udhi = *(pdu) & 0x40;
	
	char tp_oa_size = *(pdu+1);
	char tp_oa_type = *(pdu+2);
	
	char* tp_oa = pdu + 1/*tp-mti*/ + 1/*tp-oa_size*/ + 1/*tp-oa_type*/;
	char* tp_dcs = tp_oa + (tp_oa_size+1)/2/*tp-oa*/ + 1/*tp-pid*/;
	char* tp_scts = tp_dcs + 1/*tp_dcs*/;
	char* tp_udl = tp_scts + 7/*tp_scts*/;
	char* tp_ud;
	
	int total_buf_size = 0 + 1;		//сразу выделим место по хидер
	
#define WRITELOG_SMS_BUF_SIZE 30
	char tp_oa_buf[WRITELOG_SMS_BUF_SIZE];
	if ( (tp_oa_type & 0x70) != 0x50)
	{
		//digits
		PNUM2str(tp_oa_buf, tp_oa, tp_oa_size, WRITELOG_SMS_BUF_SIZE);
		total_buf_size = total_buf_size + tp_oa_size;
	}
	else
	{
		//text
		int tp_oa_size_text = (tp_oa_size + 1)/2;		//in bytes
		total_buf_size = total_buf_size + gsm7_to_ascii((unsigned char*)tp_oa, (unsigned char*)tp_oa_buf, tp_oa_size_text);
	}
	
	total_buf_size = total_buf_size + 11/*from:	()\r\n*/ + 29/*when....*/ + 4;
	
	//reverse half bytes
	for (i=0; i < 6; i++)
	{
		tp_scts[i] = (tp_scts[i]>>4 | tp_scts[i]<<4) & 0xFF;
	}
	
	if (is_tp_udhi)
	{
		char* tp_udh = tp_udl + 1;
		char tp_udhl = *tp_udh;			//size of TP-UDH
		
		char IEI = *(tp_udh+1);
		
		tp_ud = tp_udh + 1 + tp_udhl;
		
		wchar_t uniqueID;
		char num_of_parts;
		char partID;
		if ( (IEI&0xF) == 0x8 )
		{
			uniqueID = (*(tp_udh+3))<<8 | *(tp_udh+4);
			num_of_parts = *(tp_udh+5);
			partID = *(tp_udh+6);
		}
		else
		{
			uniqueID = (wchar_t)*(tp_udh+3);
			num_of_parts = *(tp_udh+4);
			partID = *(tp_udh+5);
		}
		
		sms_concat_part* sms_part_elem = new sms_concat_part;
		sms_part_elem->part_ID = partID;
		
		if ((*tp_dcs & 0xC) == 0x8)
			sms_part_elem->text_size = *tp_udl;
		else
			sms_part_elem->text_size = *tp_udl-*tp_udl/8;
		
		sms_part_elem->text = new char[sms_part_elem->text_size];
		memset(sms_part_elem->text, 0, sms_part_elem->text_size);
		memcpy(sms_part_elem->text + (tp_udhl+1), tp_ud, sms_part_elem->text_size - (tp_udhl+1));
		
		int sms_index = List_Find(BLBook->concat_sms_list, (void*)uniqueID, List_Find_sms_concat_cmp);
		
		if (sms_index == LIST_ERROR)
		{
			sms_concat* new_sms_elem = new sms_concat;
			new_sms_elem->unique_smsID = uniqueID;
			new_sms_elem->total_parts_num = num_of_parts;
			new_sms_elem->parts_list = List_Create();
			List_InsertFirst(new_sms_elem->parts_list, sms_part_elem);
			List_InsertLast(BLBook->concat_sms_list, new_sms_elem);
		}
		else
		{
			sms_concat* sms_elem = (sms_concat*)List_Get(BLBook->concat_sms_list, sms_index);
			
			if (List_Find(sms_elem->parts_list, (void*)sms_part_elem->part_ID, List_Find_sms_part_cmp) == LIST_ERROR)
				List_InsertSorted(sms_elem->parts_list, sms_part_elem, ConcatList_Insert_Filter);
			else
				sms_concat_part_elem_free(sms_part_elem);
			
			if (sms_elem->total_parts_num == sms_elem->parts_list->FirstFree)
			{
				concat_sms_need_to_log = TRUE;
				concat_sms_index = sms_index;
			}
		}
	}
	
	if (!is_tp_udhi || concat_sms_need_to_log == TRUE)
	{
		sms_concat* sms_concat_to_log = 0;
		tp_ud = tp_udl + 1;
		
		if (concat_sms_need_to_log == TRUE)
		{
			sms_concat_to_log = (sms_concat*)List_Get(BLBook->concat_sms_list, concat_sms_index);
		}
		
		wchar_t* text_buf;
		int text_buf_size=0;
		
		if ((*tp_dcs & 0xC) == 0x8)
		{
			//ucs2
			if (concat_sms_need_to_log == TRUE)
			{
				for (i=0; i < sms_concat_to_log->parts_list->FirstFree; i++)
				{
					sms_concat_part* part_i = (sms_concat_part*)List_Get(sms_concat_to_log->parts_list, i);
					text_buf_size = text_buf_size + part_i->text_size / 2;
				}
			}
			else
				text_buf_size = *tp_udl/2;
			
			text_buf = new wchar_t[text_buf_size+1];
			
			if (concat_sms_need_to_log == TRUE)
			{
				wchar_t* _text_buf = text_buf;
				
				for (i=0; i < sms_concat_to_log->parts_list->FirstFree; i++)
				{
					sms_concat_part* part_i = (sms_concat_part*)List_Get(sms_concat_to_log->parts_list, i);
					
					//big endian to little endian
					for (j=0; j < part_i->text_size / 2; j++)
					{
						((char*)_text_buf)[j*2] = part_i->text[j*2+1];
						((char*)_text_buf)[j*2+1] = part_i->text[j*2];
					}
					_text_buf = _text_buf + j;
				}
				
				wstr_remove_null(text_buf, text_buf_size);
			}
			else
			{
				//big endian to little endian
				for (i=0; i < text_buf_size; i++)
				{
					((char*)text_buf)[i*2] = tp_ud[i*2+1];
					((char*)text_buf)[i*2+1] = tp_ud[i*2];
				}
				text_buf[text_buf_size]=0;
			}
		}
		else
		{
			//gsm7
			if (concat_sms_need_to_log == TRUE)
			{
				for (i=0; i < sms_concat_to_log->parts_list->FirstFree; i++)
				{
					sms_concat_part* part_i = (sms_concat_part*)List_Get(sms_concat_to_log->parts_list, i);
					text_buf_size = text_buf_size + (part_i->text_size + part_i->text_size/7);
				}
			}
			else
				text_buf_size = *tp_udl;
			
			char* text_buf_str = new char[text_buf_size+1];
			text_buf = new wchar_t[text_buf_size+1];
			
			if (concat_sms_need_to_log == TRUE)
			{
				char* _text_buf_str = text_buf_str;
				
				for (i=0; i < sms_concat_to_log->parts_list->FirstFree; i++)
				{
					sms_concat_part* part_i = (sms_concat_part*)List_Get(sms_concat_to_log->parts_list, i);
					
					_text_buf_str = _text_buf_str + gsm7_to_ascii((unsigned char*)part_i->text, (unsigned char*)_text_buf_str, part_i->text_size);
				}
				str_remove_null(text_buf_str, text_buf_size);
			}
			else
			{
				gsm7_to_ascii((unsigned char*)tp_ud, (unsigned char*)text_buf_str, *tp_udl - *tp_udl/8);
			}
			str2wstr(text_buf,text_buf_str);
			delete text_buf_str;
		}
		
#define PNUM_NAME_BUF_SIZE 60
		wchar_t* pnum_name = new wchar_t[PNUM_NAME_BUF_SIZE];
		if ( (tp_oa_type & 0x70) != 0x50)
		{
			TEXTID pnum_name_textid = PNUM2Name(tp_oa,0,0);
			if (pnum_name_textid == EMPTY_TEXTID)
			{
				wstrcpy(pnum_name, L"NoName");
			}
			else
			{
				TextID_GetWString(pnum_name_textid, pnum_name, PNUM_NAME_BUF_SIZE);
				TextID_Destroy(pnum_name_textid);
			}
		}
		else
		{
			wstrcpy(pnum_name, L"NoName");
		}
		
		total_buf_size = total_buf_size + text_buf_size + PNUM_NAME_BUF_SIZE;
		wchar_t* buf_to_write = new wchar_t[total_buf_size+1];
		int used_len=0;
		memset(buf_to_write, 0, (total_buf_size+1)*2);
		
		DATETIME datetime;
		wchar_t folder[11];
		wchar_t fpath[45];
		
		REQUEST_DATEANDTIME_GET(SYNC,&datetime);
		snwprintf(folder,11,L"%04d-%02d-%02d",datetime.date.year,datetime.date.mon,datetime.date.day);
		
		wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
		wstrcat(fpath,L"/Call Logger/");
		wstrcat(fpath,folder);
		
		if (fstat(fpath,LOG_SMS_FNAME,0)<0)
		{
			buf_to_write[0] = 0xFEFF;
			used_len = used_len + 1;
		}
		
		snwprintf(buf_to_write+used_len, total_buf_size-used_len, SMS_LOG_MASK, pnum_name, tp_oa_buf, tp_scts[0], tp_scts[1], tp_scts[2], tp_scts[3], tp_scts[4], tp_scts[5], text_buf);
		
		int fd = _fopen(fpath, LOG_SMS_FNAME, FSX_O_CREAT|FSX_O_APPEND, FSX_S_IREAD|FSX_S_IWRITE, 0);
		fwrite(fd, buf_to_write, wstrlen(buf_to_write)*2);
		fclose(fd);
		
		delete pnum_name;
		delete text_buf;
		delete buf_to_write;
		
		if (concat_sms_need_to_log == TRUE)
		{
			List_RemoveAt(BLBook->concat_sms_list, concat_sms_index);
			sms_concat_elem_free(sms_concat_to_log);
		}
	}
	
	delete pdu;
}


void WriteLog_Call(int number1)
{
	char * number = (char*)number1;
	
	DATETIME datetime;
	wchar_t folder[11];
	wchar_t fpath[45];
	
#define WRITELOG_CALL_BUF_SIZE 100
	wchar_t buffer[WRITELOG_CALL_BUF_SIZE];
	int len=0;
	
	REQUEST_DATEANDTIME_GET(SYNC,&datetime);
	snwprintf(folder,11,L"%04d-%02d-%02d",datetime.date.year,datetime.date.mon,datetime.date.day);
	
	wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
	wstrcat(fpath,L"/Call Logger/");
	wstrcat(fpath,folder);
	//Проверяем существование файла, если нет то добавляем в буфер хидер юникодного файла
	if (fstat(fpath,LOG_CALLS_FNAME,0) < 0)
	{
		buffer[0] = 0xFEFF;
		len = len + 1;
	}
	char pnum[50];
	str2PNUM(pnum,number,50,50);
	TEXTID pnum_name = PNUM2Name(pnum,0,0);
	if (pnum_name==EMPTY_TEXTID)
	{
		wstrcpy(buffer+len,L"NoName");
		len = len + 6;
	}
	else
	{
		len = len + TextID_GetWString(pnum_name,buffer+len,WRITELOG_CALL_BUF_SIZE-len);
		TextID_Destroy(pnum_name);
	}
	
	snwprintf(buffer+len, WRITELOG_CALL_BUF_SIZE-len, L" (%s) - %02d:%02d:%02d\r\n", number, datetime.time.hour, datetime.time.min, datetime.time.sec);
	
	int f=_fopen(fpath, LOG_CALLS_FNAME, FSX_O_CREAT|FSX_O_APPEND, FSX_S_IREAD|FSX_S_IWRITE, 0);
	fwrite(f, buffer, wstrlen(buffer)*2);
	fclose(f);
	
	delete number;
}


void ReadBlockList(BLOCK_LIST_STRUCT * list,wchar_t * fname)
{
	FSTAT fstat_struct;
	wchar_t path[50];
	path[0]=0;
	
	wstrcpy(path,GetDir(MEM_EXTERNAL | DIR_ELFS_CONFIG));
	wstrcat(path,MAIN_FPATH);
	
	if ( fstat( path, fname, &fstat_struct ) >=0 )
	{
		char * buf=new char[fstat_struct.fsize+1];
		
		int f=_fopen( path, fname, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 );
		fread( f, buf, fstat_struct.fsize );
		buf[fstat_struct.fsize]=0;
		fclose( f );
		
		list->unknown = *buf;
		list->known = *(buf+1);
		
		char * cur_pos = buf+2;

		while ( cur_pos < buf+fstat_struct.fsize )
		{
			char * end_pos = strstr(cur_pos,end_of_line);
			int len = end_pos-cur_pos;
			if (len)
			{
				filter_list_elem* elem = new filter_list_elem;
				elem->data = new char[len+1];
				strncpy(elem->data,cur_pos,len);
				elem->data[len]=0;
				
				if (my_strspn(elem->data,accept_digits) == 0) elem->type = FILTERLIST_TYPE_PNUM;
				else elem->type = FILTERLIST_TYPE_MASK;
				
				List_InsertLast(list->block_list,elem);
			}
			else break;
			cur_pos = cur_pos + len + 2;
		}
		delete buf;
	}
}


void WriteBlockList(BLOCK_LIST_STRUCT * list,wchar_t * fname)
{
	wchar_t path[50];
	path[0]=0;
	
	wstrcpy(path,GetDir(MEM_EXTERNAL | DIR_ELFS_CONFIG));
	wstrcat(path,MAIN_FPATH);
	
	int f=_fopen( path, fname, FSX_O_WRONLY|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 );
	
	fwrite( f, &list->unknown, 1 );
	fwrite( f, &list->known, 1 );
	
	int count=0;
	
	while ( count < list->block_list->FirstFree )
	{
		filter_list_elem* elem = (filter_list_elem*)List_Get(list->block_list,count);
		fwrite( f, elem->data, strlen(elem->data) );
		fwrite( f, end_of_line, 2 );
		count++;
	}
	
	fclose( f );
}


void GetSettings(BlackListBook * BLBook,int settingsID)
{
	FSTAT fstat_struct;
	wchar_t* fname;
	int size;
	void* settings_buf;
	wchar_t path[50];
	path[0]=0;
	
	wstrcpy(path,GetDir(MEM_EXTERNAL | DIR_ELFS_CONFIG));
	wstrcat(path,MAIN_FPATH);
	
	switch(settingsID)
	{
	case SETTINGS_ID_COMMON:
		fname=SETTINGS_COMMON_FNAME;
		size=sizeof(SETTINGS_COMMON);
		settings_buf=&BLBook->settings_common;
		break;
	case SETTINGS_ID_CALLS:
		fname=SETTINGS_CALLS_FNAME;
		size=sizeof(SETTINGS_CALLS);
		settings_buf=&BLBook->settings_calls;
		break;
	case SETTINGS_ID_SMS_DIG:
		fname=SETTINGS_SMS_DIG_FNAME;
		size=sizeof(SETTINGS_SMS_DIG);
		settings_buf=&BLBook->settings_sms_dig;
		break;
	case SETTINGS_ID_SMS_TEXT:
		fname=SETTINGS_SMS_TEXT_FNAME;
		size=sizeof(SETTINGS_SMS_TEXT);
		settings_buf=&BLBook->settings_sms_text;
		break;
	}
	
	if ( fstat( path, fname, &fstat_struct ) >=0 && fstat_struct.fsize==size )
	{
		int f=_fopen( path, fname, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0 );
		fread( f, settings_buf, size );
		fclose( f );
	}
}


void SetSettings(BlackListBook * BLBook,int settingsID)
{
	wchar_t* fname;
	int size;
	void* settings_buf;
	wchar_t path[50];
	path[0]=0;
	
	wstrcpy(path,GetDir(MEM_EXTERNAL | DIR_ELFS_CONFIG));
	wstrcat(path,MAIN_FPATH);
	
	switch(settingsID)
	{
	case SETTINGS_ID_COMMON:
		fname=SETTINGS_COMMON_FNAME;
		size=sizeof(SETTINGS_COMMON);
		settings_buf=&BLBook->settings_common;
		break;
	case SETTINGS_ID_CALLS:
		fname=SETTINGS_CALLS_FNAME;
		size=sizeof(SETTINGS_CALLS);
		settings_buf=&BLBook->settings_calls;
		break;
	case SETTINGS_ID_SMS_DIG:
		fname=SETTINGS_SMS_DIG_FNAME;
		size=sizeof(SETTINGS_SMS_DIG);
		settings_buf=&BLBook->settings_sms_dig;
		break;
	case SETTINGS_ID_SMS_TEXT:
		fname=SETTINGS_SMS_TEXT_FNAME;
		size=sizeof(SETTINGS_SMS_TEXT);
		settings_buf=&BLBook->settings_sms_text;
		break;
	}
	
	int f=_fopen( path, fname, FSX_O_WRONLY|FSX_O_TRUNC, FSX_S_IREAD|FSX_S_IWRITE, 0 );
	fwrite( f, settings_buf, size );
	fclose( f );
}


void InitLanguage(BlackListBook * BLBook)
{
	switch (BLBook->settings_common.language)
	{
	case LANGUAGE_EN:
		SET_CALLS_TXT=EN_SET_CALLS_TXT;
		SET_SMS_DIG_TXT=EN_SET_SMS_DIG_TXT;
		SET_SMS_TEXT_TXT=EN_SET_SMS_TEXT_TXT;
		SET_CONTROL_TXT=EN_SET_CONTROL_TXT;
		SET_BLOCK_TYPE_TXT=EN_SET_BLOCK_TYPE_TXT;
		SET_BLOCK_MODE_TXT=EN_SET_BLOCK_MODE_TXT;
		SET_LOG_TXT=EN_SET_LOG_TXT;
		SET_NOTIFICATION_TXT=EN_SET_NOTIFICATION_TXT;
		SET_LANGUAGE_TXT=EN_SET_LANGUAGE_TXT;
		STATE_ON_TXT=EN_STATE_ON_TXT;
		STATE_OFF_TXT=EN_STATE_OFF_TXT;
		BLOCK_TYPE_BUSY_TXT=EN_BLOCK_TYPE_BUSY_TXT;
		BLOCK_TYPE_OUT_OF_SERVICE_TXT=EN_BLOCK_TYPE_OUT_OF_SERVICE_TXT;
		BLOCK_MODE_ALL_TXT=EN_BLOCK_MODE_ALL_TXT;
		BLOCK_MODE_BL_TXT=EN_BLOCK_MODE_BL_TXT;
		BLOCK_MODE_WL_TXT=EN_BLOCK_MODE_WL_TXT;
		SETTINGS_TXT=EN_SETTINGS_TXT;
		EDIT_LISTS_TXT=EN_EDIT_LISTS_TXT;
		NUMBER_EXISTS_TXT=EN_NUMBER_EXISTS_TXT;
		ILLEGAL_SYMBOLS_ERROR_TXT=EN_ILLEGAL_SYMBOLS_ERROR_TXT;
		ENTER_NUMBER_TXT=EN_ENTER_NUMBER_TXT;
		ENTER_MASK_TXT=EN_ENTER_MASK_TXT;
		EMPTY_TXT=EN_EMPTY_TXT;
		FROM_PB_TXT=EN_FROM_PB_TXT;
		MANUAL_TXT=EN_MANUAL_TXT;
		UNKNOWN_PNUM_TXT=EN_UNKNOWN_PNUM_TXT;
		MASK_TXT=EN_MASK_TXT;
		LANGUAGE_EN_TXT=EN_LANGUAGE_EN_TXT;
		LANGUAGE_RU_TXT=EN_LANGUAGE_RU_TXT;
		BLOCKED_TXT=EN_BLOCKED_TXT;
		BLOCKED_TYPE_SMS_TXT=EN_BLOCKED_TYPE_SMS_TXT;
		BLOCKED_TYPE_CALL_TXT=EN_BLOCKED_TYPE_CALL_TXT;
		ALL_UNKNOWN_TXT=EN_ALL_UNKNOWN_TXT;
		ALL_UNKNOWN_SECOND_LINE_TXT=EN_ALL_UNKNOWN_SECOND_LINE_TXT;
		ALL_KNOWN_TXT=EN_ALL_KNOWN_TXT;
		ALL_KNOWN_SECOND_LINE_TXT=EN_ALL_KNOWN_SECOND_LINE_TXT;
		break;
	case LANGUAGE_RU:
		SET_CALLS_TXT=RU_SET_CALLS_TXT;
		SET_SMS_DIG_TXT=RU_SET_SMS_DIG_TXT;
		SET_SMS_TEXT_TXT=RU_SET_SMS_TEXT_TXT;
		SET_CONTROL_TXT=RU_SET_CONTROL_TXT;
		SET_BLOCK_TYPE_TXT=RU_SET_BLOCK_TYPE_TXT;
		SET_BLOCK_MODE_TXT=RU_SET_BLOCK_MODE_TXT;
		SET_LOG_TXT=RU_SET_LOG_TXT;
		SET_NOTIFICATION_TXT=RU_SET_NOTIFICATION_TXT;
		SET_LANGUAGE_TXT=RU_SET_LANGUAGE_TXT;
		STATE_ON_TXT=RU_STATE_ON_TXT;
		STATE_OFF_TXT=RU_STATE_OFF_TXT;
		BLOCK_TYPE_BUSY_TXT=RU_BLOCK_TYPE_BUSY_TXT;
		BLOCK_TYPE_OUT_OF_SERVICE_TXT=RU_BLOCK_TYPE_OUT_OF_SERVICE_TXT;
		BLOCK_MODE_ALL_TXT=RU_BLOCK_MODE_ALL_TXT;
		BLOCK_MODE_BL_TXT=RU_BLOCK_MODE_BL_TXT;
		BLOCK_MODE_WL_TXT=RU_BLOCK_MODE_WL_TXT;
		SETTINGS_TXT=RU_SETTINGS_TXT;
		EDIT_LISTS_TXT=RU_EDIT_LISTS_TXT;
		NUMBER_EXISTS_TXT=RU_NUMBER_EXISTS_TXT;
		ILLEGAL_SYMBOLS_ERROR_TXT=RU_ILLEGAL_SYMBOLS_ERROR_TXT;
		ENTER_NUMBER_TXT=RU_ENTER_NUMBER_TXT;
		ENTER_MASK_TXT=RU_ENTER_MASK_TXT;
		EMPTY_TXT=RU_EMPTY_TXT;
		FROM_PB_TXT=RU_FROM_PB_TXT;
		MANUAL_TXT=RU_MANUAL_TXT;
		UNKNOWN_PNUM_TXT=RU_UNKNOWN_PNUM_TXT;
		MASK_TXT=RU_MASK_TXT;
		LANGUAGE_EN_TXT=RU_LANGUAGE_EN_TXT;
		LANGUAGE_RU_TXT=RU_LANGUAGE_RU_TXT;
		BLOCKED_TXT=RU_BLOCKED_TXT;
		BLOCKED_TYPE_SMS_TXT=RU_BLOCKED_TYPE_SMS_TXT;
		BLOCKED_TYPE_CALL_TXT=RU_BLOCKED_TYPE_CALL_TXT;
		ALL_UNKNOWN_TXT=RU_ALL_UNKNOWN_TXT;
		ALL_UNKNOWN_SECOND_LINE_TXT=RU_ALL_UNKNOWN_SECOND_LINE_TXT;
		ALL_KNOWN_TXT=RU_ALL_KNOWN_TXT;
		ALL_KNOWN_SECOND_LINE_TXT=RU_ALL_KNOWN_SECOND_LINE_TXT;
		break;
	}
}


int List_Find_BlockSMSText_cmp(void *r0,void *r1)
{
	filter_list_elem* elem = (filter_list_elem*)r0;
	
	if (str_mask_cmp((char*)r1,elem->data)) return(1);
	return(0);
}


int List_Find_BlockPnum_cmp(void *r0,void *r1)
{
	filter_list_elem* elem = (filter_list_elem*)r0;
	
	if ( elem->type == FILTERLIST_TYPE_PNUM && strcmp(elem->data+NUM_CHR_SUBS,(char*)r1+NUM_CHR_SUBS) ) return(1);
	if ( elem->type == FILTERLIST_TYPE_MASK && str_mask_cmp((char*)r1,elem->data) ) return(1);
	return(0);
}


int List_Find_AddPnum_cmp(void *r0,void *r1)
{
	filter_list_elem* elem = (filter_list_elem*)r0;
	
	if (strcmp(elem->data,(char*)r1)) return(1);
	return(0);
}


int BlackListBook_CheckNumber(BOOK * book,char * pnum)
{
#define ABSENT_IN_LISTS 0x0
#define PRESENT_IN_BLACK 0x1
#define PRESENT_IN_WHITE 0x2
	
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int res = ABSENT_IN_LISTS;
	char buf[50];
	
	PNUM2str(buf,pnum,49,50);
	if ( List_Find( BLBook->black_list_calls.block_list, buf, List_Find_BlockPnum_cmp) != LIST_ERROR ) res = res|PRESENT_IN_BLACK;
	if ( List_Find( BLBook->white_list_calls.block_list, buf, List_Find_BlockPnum_cmp) != LIST_ERROR ) res = res|PRESENT_IN_WHITE;
	
	return res;
}


int BlackListBook_EditLists(BOOK * book,char * pnum,char mode)
{
#define MODE_BL 0x1
#define MODE_WL 0x2
#define MODE_ADD 0x10
#define MODE_DEL 0x20
	
	BlackListBook * BLBook = (BlackListBook*)book;
	
	filter_list_elem* elem;
	BLOCK_LIST_STRUCT * list;
	wchar_t * fname;
	char buf[50];
	
	PNUM2str(buf,pnum,49,50);
	
	if ( (mode&0xF) == MODE_BL )
	{
		list = &BLBook->black_list_calls;
		fname = BLACKLIST_CALLS_FNAME;
	}
	if ( (mode&0xF) == MODE_WL )
	{
		list = &BLBook->white_list_calls;
		fname = WHITELIST_CALLS_FNAME;
	}
	
	switch (mode&0xF0)
	{
	case MODE_ADD:
		elem = new filter_list_elem;
		elem->data = new char[strlen(buf)+1];
		strcpy(elem->data, buf);
		elem->type = FILTERLIST_TYPE_PNUM;
		List_InsertLast(list->block_list, elem);
		break;
		
	case MODE_DEL:
		filter_list_elem_free( List_RemoveAt( list->block_list, List_Find(list->block_list, buf,List_Find_AddPnum_cmp) ) );
		break;
	}
	
	WriteBlockList(list,fname);
	
	return BlackListBook_CheckNumber(book,pnum);
}


//smsc, pdu - first byte is  a size of data (without size-byte)
int BlackListBook_FilterSMS(BOOK * book,char* smsc,char* pdu)
{
#define BLOCK_SMS 0x1
#define ALLOW_SMS 0x0
	
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int res = ALLOW_SMS;
	char need_to_block = FALSE;
	
	char num_of_parts = 0;
	char partID = 0;
	char* tp_udh = 1/*size of pdu*/ + pdu + 1/*tp-mti*/ + 1/*tp-oa_size*/ + 1/*tp-oa_type*/ + (*(pdu+2)+1)/2/*tp-oa*/ + 1/*tp-pid*/ + 1/*tp_dcs*/ + 7/*tp_scts*/ + 1/*tp-udl*/;
	char IEI = *(tp_udh+1/*tp-udhl*/);
	
	if ( (*(pdu+1)&0x40) != 0 )			//if tp-udhi
	{
		if ( (IEI&0xF) == 0x8 )
		{
			num_of_parts = *(tp_udh+5);
			partID = *(tp_udh+6);
		}
		else
		{
			num_of_parts = *(tp_udh+4);
			partID = *(tp_udh+5);
		}
	}
	
	if ( (pdu[2+1] & 0x70) != 0x50)
	//address - digits
	{
		int pnum_len = pdu[1+1];
		
		char * number=new char[pnum_len+1];
		PNUM2str(number,pdu+3+1,pnum_len,pnum_len+1);
		
		{
			char * number_unk=new char[pnum_len+1];
			strcpy(number_unk, number);
			
			MMIPROC( CheckUnk, (int) BLBook, number_unk );
			
			do
			{
				delay(5);
			}while(BLBook->isUnk==0xFF);
		}
		
		if (BLBook->settings_sms_dig.sms_dig_control_state==CONTROL_STATE_ON)
		{
			switch (BLBook->settings_sms_dig.sms_dig_block_mode)
			{
			case MODE_ALL:
				need_to_block = TRUE;
				break;
			case MODE_BLACK:
				if (BLBook->isUnk == TRUE && BLBook->black_list_sms_dig.unknown == TRUE) need_to_block = TRUE;
				else if (BLBook->isUnk == FALSE && BLBook->black_list_sms_dig.known == TRUE) need_to_block = TRUE;
				else if (List_Find(BLBook->black_list_sms_dig.block_list,number,List_Find_BlockPnum_cmp) != LIST_ERROR) need_to_block = TRUE;
				break;
			case MODE_WHITE:
				if (BLBook->isUnk == TRUE && BLBook->white_list_sms_dig.unknown == TRUE) need_to_block = FALSE;
				else if (BLBook->isUnk == FALSE && BLBook->white_list_sms_dig.known == TRUE) need_to_block = FALSE;
				else if (List_Find(BLBook->white_list_sms_dig.block_list,number,List_Find_BlockPnum_cmp) == LIST_ERROR) need_to_block = TRUE;
				break;
			}
		}
		
		BLBook->isUnk=0xFF;
		
		if (need_to_block==TRUE)
		{
			int num_len = strlen(number);
			
			if (BLBook->settings_sms_dig.sms_dig_notification == STATE_NOTIFIC_ON)
			{
				if (partID == num_of_parts)
				{
					char * number_not=new char[num_len+1];
					memcpy(number_not, number, num_len+1);
					MMIPROC( ShowMsgBox, (int) number_not, (void*)NOTIFIC_TYPE_SMS );
				}
			}
			
			if (BLBook->settings_sms_dig.sms_dig_log==STATE_LOG_ON)
			{
				char * pdu_log=new char[pdu[0]];
				memcpy(pdu_log, pdu+1, pdu[0]);
				MMIPROC( WriteLog_SMS, (int) pdu_log, (void*)BLBook );
			}
			
			res = BLOCK_SMS;
		}
		
		delete number;
	}
	else
	//address - text
	{
		int gsm_address_len = (pdu[1+1] + 1)/2;		//length in bytes
		int text_address_len = gsm_address_len + gsm_address_len/7;
		
		unsigned char * address_text = new unsigned char[text_address_len+1];
		gsm7_to_ascii((unsigned char*)pdu+3+1,address_text,gsm_address_len);
		
		if (BLBook->settings_sms_text.sms_text_control_state==CONTROL_STATE_ON)
		{
			switch (BLBook->settings_sms_text.sms_text_block_mode)
			{
			case MODE_ALL:
				need_to_block = TRUE;
				break;
			case MODE_BLACK:
				if (List_Find(BLBook->black_list_sms_text.block_list,address_text,List_Find_BlockSMSText_cmp) != LIST_ERROR) need_to_block = TRUE;
				break;
			case MODE_WHITE:
				if (List_Find(BLBook->white_list_sms_text.block_list,address_text,List_Find_BlockSMSText_cmp) == LIST_ERROR) need_to_block = TRUE;
				break;
			}
		}
		
		if (need_to_block==TRUE)
		{
			if (BLBook->settings_sms_text.sms_text_notification == STATE_NOTIFIC_ON)
			{
				if (partID == num_of_parts)
				{
					char * address_text_not = new char[text_address_len+1];
					memcpy(address_text_not, address_text, text_address_len+1);
					MMIPROC( ShowMsgBox, (int) address_text_not, (void*)NOTIFIC_TYPE_SMS_TEXT );
				}
			}
			
			if (BLBook->settings_sms_text.sms_text_log==STATE_LOG_ON)
			{
				char * pdu_log=new char[pdu[0]];
				memcpy(pdu_log, pdu+1, pdu[0]);
				MMIPROC( WriteLog_SMS, (int) pdu_log, (void*)BLBook );
			}
			
			res = BLOCK_SMS;
		}
	}
	
	return res;
}


int BlackListBook_FilterCall(BOOK * book,MYFUNCTIONS * func_array, void * pICBCallManager, TCallSupplementaryCli * Cli, int CallType, int CallId, void * ClientData)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	char need_to_block = FALSE;
	
	{
		char * number_unk=new char[0x51];
		memcpy(number_unk, Cli->Number.Digits, 0x51);
		
		MMIPROC( CheckUnk, (int) BLBook, number_unk );
		
		do
		{
			delay(5);
		}while(BLBook->isUnk==0xFF);
	}
	
	if (BLBook->settings_calls.calls_control_state==CONTROL_STATE_ON)
	{
		switch (BLBook->settings_calls.calls_block_mode)
		{
		case MODE_ALL:
			need_to_block = TRUE;
			break;
		case MODE_BLACK:
			if (BLBook->isUnk == TRUE && BLBook->black_list_calls.unknown == TRUE) need_to_block = TRUE;
			else if (BLBook->isUnk == FALSE && BLBook->black_list_calls.known == TRUE) need_to_block = TRUE;
			else if (List_Find(BLBook->black_list_calls.block_list,Cli->Number.Digits,List_Find_BlockPnum_cmp) != LIST_ERROR) need_to_block = TRUE;
			break;
		case MODE_WHITE:
			if (BLBook->isUnk == TRUE && BLBook->white_list_calls.unknown == TRUE) need_to_block = FALSE;
			else if (BLBook->isUnk == FALSE && BLBook->white_list_calls.known == TRUE) need_to_block = FALSE;
			else if (List_Find(BLBook->white_list_calls.block_list,Cli->Number.Digits,List_Find_BlockPnum_cmp) == LIST_ERROR) need_to_block = TRUE;
			break;
		}
	}
	
	BLBook->isUnk=0xFF;
	
	if (need_to_block==TRUE)
	{
		if (BLBook->settings_calls.calls_notification==STATE_NOTIFIC_ON)
		{
			char * number=new char[0x51];
			memcpy(number, Cli->Number.Digits, 0x51);
			MMIPROC( ShowMsgBox, (int) number, (void*)NOTIFIC_TYPE_CALL );
		}
		if (BLBook->settings_calls.calls_log==STATE_LOG_ON)
		{
			char * number=new char[0x51];
			memcpy(number, Cli->Number.Digits, 0x51);
			MMIPROC( WriteLog_Call, (int) number );
		}
		
		switch (BLBook->settings_calls.calls_block_type)
		{
		case TYPE_BUSY:
			return func_array->CLH_Wrapper_ICallManager_RejectSetup((func_array->CLH_Server_GetData())->pICallManager,0x11);
		case TYPE_OUT_OF_SERVICE:
			return 0;
		}
	}
	return func_array->CClhServer_ICBCallManager_OnSetupPresent(pICBCallManager,Cli,CallType,CallId,ClientData);
}

//====================================================================

void SI_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void SI_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
}


void SI_Accept_Action(BOOK * book, wchar_t * numb, int len)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int is_smstext = FALSE;
	if ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) == 2 ) is_smstext = TRUE;
	
	filter_list_elem* elem = new filter_list_elem;
	elem->data = new char[len+1];
	
	wstr2strn(elem->data,numb,len);
	elem->data[len]=0;
	
	if (is_smstext == TRUE)
		elem->type = FILTERLIST_TYPE_MASK;
	else if (my_strspn(elem->data,accept_digits) == 0)
		elem->type = FILTERLIST_TYPE_PNUM;
	else if (my_strspn(elem->data,accept_mask) == 0)
		elem->type = FILTERLIST_TYPE_MASK;
	else
	{
		MessageBox(EMPTY_TEXTID,TextID_Create(ILLEGAL_SYMBOLS_ERROR_TXT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,0,2000,BLBook);
		return;
	}
	
	if (List_Find(BLBook->cur_list->block_list, elem->data, List_Find_AddPnum_cmp) != LIST_ERROR)
	{
		MessageBox(EMPTY_TEXTID,TextID_Create(NUMBER_EXISTS_TXT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,0,2000,book);
		filter_list_elem_free( elem );
	}
	else
	{
		if (BLBook->edit_flag==FALSE)
			List_InsertLast(BLBook->cur_list->block_list, elem);
		else
		{
			int pos = ListMenu_GetSelectedItem(BLBook->editblocklist_list);
			if ( is_smstext == FALSE ) pos = pos-2;
			filter_list_elem_free( List_RemoveAt(BLBook->cur_list->block_list,pos) );
			List_Insert(BLBook->cur_list->block_list,pos,elem);
		}
	}
	
	BookObj_ReturnPage(BLBook,ACCEPT_EVENT);
}


int BlackListBook_SI_Page_Enter_Action(void * data,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	TEXTID editable_strID;
	int pos;
	
	int is_smstext = FALSE;
	if ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) == 2 ) is_smstext = TRUE;
	
	if (BLBook->edit_flag==FALSE)
		editable_strID = EMPTY_TEXTID;
	else
	{
		pos = ListMenu_GetSelectedItem(BLBook->editblocklist_list);
		if ( is_smstext == FALSE ) pos = pos-2;
		filter_list_elem* elem = (filter_list_elem*)List_Get(BLBook->cur_list->block_list, pos);
		editable_strID = TextID_Create(elem->data, ENC_LAT1, TEXTID_ANY_LEN);
	}
	
	BLBook->str_inp = CreateStringInputVA( 0,
																				 VAR_STRINP_MODE( 1 ),
																				 VAR_BOOK( book ),
																				 //VAR_HEADER_TEXT ( TextID_Create( header_text, ENC_UCS2, TEXTID_ANY_LEN ) ),
																				 //VAR_STRINP_FIXED_TEXT( TextID_Create( ENTER_NUMBER_TXT, ENC_UCS2, TEXTID_ANY_LEN ) ),
																				 VAR_STRINP_TEXT( editable_strID ),
																				 VAR_STRINP_MIN_LEN( 2 ),
																				 VAR_STRINP_MAX_LEN( 20 ),
																				 VAR_PREV_ACTION_PROC( SI_Prev_Action ),
																				 VAR_LONG_BACK_PROC( SI_Cancel_Action ),
																				 VAR_OK_PROC( SI_Accept_Action ),
																				 0 );
	return 0;
}


int BlackListBook_SI_Page_Exit_Action(void * data,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->str_inp)
	{
		GUIObject_Destroy(BLBook->str_inp);
		BLBook->str_inp=0;
	}
	
	BLBook->edit_flag=FALSE;
	
	return 0;
}

//====================================================================

int BlackListBook_SearchPB_Page_Cancel_Action(void * data,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 1;
}


int BlackListBook_SearchPB_Page_Prev_Action(void * data,BOOK * book)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
	return 1;
}


int BlackListBook_SearchPB_Page_Accept_Action(void * data,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	PB_SEARCH_DATA * pb_data = (PB_SEARCH_DATA*)data;
	
	int len = strlen(pb_data->pnum_str);
	
	filter_list_elem* elem = new filter_list_elem;
	elem->data = new char[len+1];
	
	strncpy(elem->data,pb_data->pnum_str,len);
	elem->data[len]=0;
	
	elem->type = FILTERLIST_TYPE_PNUM;
	
	if (List_Find(BLBook->cur_list->block_list, elem->data, List_Find_AddPnum_cmp)!=LIST_ERROR)
	{
		MessageBox(EMPTY_TEXTID,TextID_Create(NUMBER_EXISTS_TXT, ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 0, 2000, book);
		filter_list_elem_free( elem );
	}
	else
	{
		List_InsertLast(BLBook->cur_list->block_list,elem);
	}
	
	BookObj_ReturnPage(BLBook,ACCEPT_EVENT);
	
	return 1;
}


int BlackListBook_SearchPB_Page_Enter_Action(void * ,BOOK * book)
{
	pb_ui_search_bk_create_search_menu(BookObj_GetBookID(book),MODE_PB | MODE_SIM | MODE_UNSAVED);
	
	return 1;
}

//====================================================================

void EditBlockList_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void EditBlockList_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
}


void EditBlockList_Delete_Action(BOOK * book, GUI * gui)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int sel_item = ListMenu_GetSelectedItem(BLBook->editblocklist_list);
	
	if ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) != 2 ) sel_item = sel_item-2;
	
	if (sel_item >= BLBook->cur_list->block_list->FirstFree || sel_item < 0) return;
	
	filter_list_elem_free( List_RemoveAt(BLBook->cur_list->block_list, sel_item) );
	
	BlackListBook_EditBlockList_Page_Enter_Action(0,BLBook);
}


void EditBlockList_AddPB_Action(BOOK * book, GUI * gui)
{
	BookObj_CallPage( book, &BlackListBook_SearchPB_Page );
}


void EditBlockList_AddSI_Action(BOOK * book, GUI * gui)
{
	BookObj_CallPage( book, &BlackListBook_SI_Page );
}


void EditBlockList_Edit_Action(BOOK * book, GUI * gui)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	BLBook->edit_flag=TRUE;
	BookObj_CallPage( book, &BlackListBook_SI_Page );
}


void EditBlockList_Select_Action(BOOK * book, GUI * gui)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	IMAGEID imgID;
	int item_num;
	
	switch (ListMenu_GetSelectedItem(BLBook->editblocklist_list))
	{
	case 0:
		item_num = 0;
		if (BLBook->cur_list->unknown)
		{
			BLBook->cur_list->unknown = 0;
			imgID = BLBook->chkbox_unmarked;
		}
		else
		{
			BLBook->cur_list->unknown = 1;
			imgID = BLBook->chkbox_marked;
		}
		break;
	case 1:
		item_num = 1;
		if (BLBook->cur_list->known)
		{
			BLBook->cur_list->known = 0;
			imgID = BLBook->chkbox_unmarked;
		}
		else
		{
			BLBook->cur_list->known = 1;
			imgID = BLBook->chkbox_marked;
		}
		break;
	}
	
	ListMenu_SetItemIcon(BLBook->editblocklist_list, item_num, NOIMAGE, 0, imgID);
}


void EditBlockList_RefreshSoftkeys(BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	switch ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) )
	{
	case 0:
	case 1:
		if (ListMenu_GetSelectedItem(BLBook->editblocklist_list) < 2)
		{
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_EDIT, FALSE );
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_SELECT1, TRUE );
		}
		else
		{
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_EDIT, TRUE );
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_SELECT1, FALSE );
		}
		break;
	case 2:
		if (!BLBook->cur_list->block_list->FirstFree)
		{
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_EDIT, FALSE );
			ListMenu_SetNoItemText( BLBook->editblocklist_list, TextID_Create( EMPTY_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
		}
		else
			GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_EDIT, TRUE );
		
		GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_SELECT1, FALSE );
		break;
	}
}


int onCallback_EditBlockList( GUI_MESSAGE* msg )
{
	BlackListBook * BLBook = (BlackListBook*)GUIonMessage_GetBook( msg );
	
	TEXTID item_name;
	TEXTID sec_text;
	IMAGEID imgID;
	filter_list_elem* elem;
	int created_item;
	int is_smstext = FALSE;
	
	if ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) == 2 ) is_smstext = TRUE;
	
	if (!BLBook->cur_list->block_list->FirstFree && is_smstext == TRUE)
	{
		ListMenu_SetNoItemText( BLBook->editblocklist_list, TextID_Create( EMPTY_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
		return 1;
	}
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		created_item = GUIonMessage_GetCreatedItemIndex( msg );
		
		if (is_smstext == TRUE)
		{
			elem = (filter_list_elem*)List_Get( BLBook->cur_list->block_list, created_item );
			item_name=TextID_Create( elem->data, ENC_LAT1, TEXTID_ANY_LEN );
		}
		else
		{
			if (created_item == 0)
			{
				item_name=TextID_Create( ALL_UNKNOWN_TXT, ENC_UCS2, TEXTID_ANY_LEN );
				sec_text=TextID_Create( ALL_UNKNOWN_SECOND_LINE_TXT, ENC_UCS2, TEXTID_ANY_LEN );
				if (BLBook->cur_list->unknown == 0) imgID = BLBook->chkbox_unmarked;
				else imgID = BLBook->chkbox_marked;
				
				GUIonMessage_SetMenuItemIcon( msg, 0, imgID );
			}
			else if (created_item == 1)
			{
				item_name=TextID_Create( ALL_KNOWN_TXT, ENC_UCS2, TEXTID_ANY_LEN );
				sec_text=TextID_Create( ALL_KNOWN_SECOND_LINE_TXT, ENC_UCS2, TEXTID_ANY_LEN );
				if (BLBook->cur_list->known == 0) imgID = BLBook->chkbox_unmarked;
				else imgID = BLBook->chkbox_marked;
				
				GUIonMessage_SetMenuItemIcon( msg, 0, imgID );
			}
			else
			{
				elem = (filter_list_elem*)List_Get( BLBook->cur_list->block_list, created_item-2 );
				
				if (elem->type == FILTERLIST_TYPE_MASK)
				{
					item_name=TextID_Create( elem->data, ENC_LAT1, TEXTID_ANY_LEN );
					sec_text=TextID_Create( MASK_TXT, ENC_UCS2, TEXTID_ANY_LEN );
				}
				else
				{
					char pnum[50];
					str2PNUM(pnum,elem->data,50,50);
					item_name = PNUM2Name(pnum,1,0);
					if (item_name==EMPTY_TEXTID)
					{
						item_name=TextID_Create( elem->data, ENC_LAT1, TEXTID_ANY_LEN );
						sec_text=TextID_Create( UNKNOWN_PNUM_TXT, ENC_UCS2, TEXTID_ANY_LEN );
					}
					else
						sec_text=TextID_Create( elem->data, ENC_LAT1, TEXTID_ANY_LEN );
				}
			}
			
			GUIonMessage_SetMenuItemSecondLineText( msg, sec_text );
		}
		
		GUIonMessage_SetMenuItemText( msg, item_name );
		break;
	case LISTMSG_HighlightChanged:
		EditBlockList_RefreshSoftkeys(BLBook);
		break;
	}
	
	return 1;
}


int BlackListBook_EditBlockList_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int from_pb_visible;
	int item_style;
	int item_count;
	wchar_t * title;
	
	switch ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) )
	{
	case 0:
	case 1:
		from_pb_visible = TRUE;
		item_style = 3;
		item_count = BLBook->cur_list->block_list->FirstFree + 2;
		break;
	case 2:
		from_pb_visible = FALSE;
		item_style = 0;
		item_count = BLBook->cur_list->block_list->FirstFree;
		break;
	}
	
	switch ( ListMenu_GetSelectedItem(BLBook->chooselisttoedit_list) )
	{
	case 0:
		title=BLOCK_MODE_BL_TXT;
		break;
	case 1:
		title=BLOCK_MODE_WL_TXT;
		break;
	}
	
	if (BLBook->editblocklist_list)
	{
		int pos = ListMenu_GetSelectedItem(BLBook->editblocklist_list);
		ListMenu_DestroyItems(BLBook->editblocklist_list);
		ListMenu_SetItemCount(BLBook->editblocklist_list, item_count);
		ListMenu_SetCursorToItem(BLBook->editblocklist_list, pos);
		EditBlockList_RefreshSoftkeys(BLBook);
		return 0;
	}
	BLBook->editblocklist_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->editblocklist_list, onCallback_EditBlockList );
	ListMenu_SetItemCount( BLBook->editblocklist_list, item_count );
	ListMenu_SetCursorToItem( BLBook->editblocklist_list, 0 );
	ListMenu_SetItemStyle( BLBook->editblocklist_list, item_style );
	GUIObject_SetTitleText( BLBook->editblocklist_list, TextID_Create( title, ENC_UCS2, TEXTID_ANY_LEN) );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, ACTION_SELECT1, EditBlockList_Select_Action );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, ACTION_BACK, EditBlockList_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, ACTION_LONG_BACK, EditBlockList_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, ACTION_EDIT, EditBlockList_Edit_Action );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, ACTION_DELETE, EditBlockList_Delete_Action );
	GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, ACTION_DELETE, FALSE );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, 0, EditBlockList_AddPB_Action );
	GUIObject_SoftKeys_SetText( BLBook->editblocklist_list, 0, TextID_Create( FROM_PB_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetVisible( BLBook->editblocklist_list, 0, from_pb_visible );
	GUIObject_SoftKeys_SetAction( BLBook->editblocklist_list, 1, EditBlockList_AddSI_Action );
	GUIObject_SoftKeys_SetText( BLBook->editblocklist_list, 1, TextID_Create( MANUAL_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	EditBlockList_RefreshSoftkeys(BLBook);
	GUIObject_Show( BLBook->editblocklist_list );
	return 0;
}


int BlackListBook_EditBlockList_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	wchar_t * fname;
	
	switch ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) )
	{
	case 0:
		switch ( ListMenu_GetSelectedItem(BLBook->chooselisttoedit_list) )
		{
		case 0:
			fname = BLACKLIST_CALLS_FNAME;
			break;
		case 1:
			fname = WHITELIST_CALLS_FNAME;
			break;
		}
		break;
	case 1:
		switch ( ListMenu_GetSelectedItem(BLBook->chooselisttoedit_list) )
		{
		case 0:
			fname = BLACKLIST_SMS_DIG_FNAME;
			break;
		case 1:
			fname = WHITELIST_SMS_DIG_FNAME;
			break;
		}
		break;
	case 2:
		switch ( ListMenu_GetSelectedItem(BLBook->chooselisttoedit_list) )
		{
		case 0:
			fname = BLACKLIST_SMS_TEXT_FNAME;
			break;
		case 1:
			fname = WHITELIST_SMS_TEXT_FNAME;
			break;
		}
		break;
	}
	
	WriteBlockList(BLBook->cur_list,fname);
	
	if (BLBook->editblocklist_list)
	{
		GUIObject_Destroy(BLBook->editblocklist_list);
		BLBook->editblocklist_list=0;
	}
	
	return 0;
}

//====================================================================

void ChooseListToEdit_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void ChooseListToEdit_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,ACCEPT_EVENT);
}


void ChooseListToEdit_Select_Action(BOOK * book, GUI * gui)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	switch ( ListMenu_GetSelectedItem(BLBook->choosemodetoedit_list) )
	{
	case 0:
		switch ( ListMenu_GetSelectedItem(gui) )
		{
		case 0:
			BLBook->cur_list=&BLBook->black_list_calls;
			break;
		case 1:
			BLBook->cur_list=&BLBook->white_list_calls;
			break;
		}
		break;
	case 1:
		switch ( ListMenu_GetSelectedItem(gui) )
		{
		case 0:
			BLBook->cur_list=&BLBook->black_list_sms_dig;
			break;
		case 1:
			BLBook->cur_list=&BLBook->white_list_sms_dig;
			break;
		}
		break;
	case 2:
		switch ( ListMenu_GetSelectedItem(gui) )
		{
		case 0:
			BLBook->cur_list=&BLBook->black_list_sms_text;
			break;
		case 1:
			BLBook->cur_list=&BLBook->white_list_sms_text;
			break;
		}
		break;
	}
	
	BookObj_CallPage( book, &BlackListBook_EditBlockList_Page );
}


int onCallback_ChooseListToEdit( GUI_MESSAGE* msg )
{
	wchar_t * item_name;
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
		{
		case 0:
			item_name=BLOCK_MODE_BL_TXT;
			break;
		case 1:
			item_name=BLOCK_MODE_WL_TXT;
			break;
		}
		
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	
	return 1;
}


int BlackListBook_ChooseListToEdit_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	BLBook->chooselisttoedit_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->chooselisttoedit_list, onCallback_ChooseListToEdit );
	ListMenu_SetItemCount( BLBook->chooselisttoedit_list, 2 );
	ListMenu_SetCursorToItem( BLBook->chooselisttoedit_list, 0 );
	GUIObject_SetTitleText( BLBook->chooselisttoedit_list, TextID_Create( EDIT_LISTS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetAction( BLBook->chooselisttoedit_list, ACTION_BACK, ChooseListToEdit_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->chooselisttoedit_list, ACTION_LONG_BACK, ChooseListToEdit_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->chooselisttoedit_list, ACTION_SELECT1, ChooseListToEdit_Select_Action );
	GUIObject_Show( BLBook->chooselisttoedit_list );
	return 0;
}


int BlackListBook_ChooseListToEdit_Page_Cancel_Action(void * ,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 0;
}


int BlackListBook_ChooseListToEdit_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->chooselisttoedit_list)
	{
		GUIObject_Destroy(BLBook->chooselisttoedit_list);
		BLBook->chooselisttoedit_list=0;
	}
	
	return 0;
}

//====================================================================

void ChooseModeToEdit_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void ChooseModeToEdit_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,ACCEPT_EVENT);
}


void ChooseModeToEdit_Select_Action(BOOK * book, GUI * gui)
{
	BookObj_CallPage( book, &BlackListBook_ChooseListToEdit_Page );
}


int onCallback_ChooseModeToEdit( GUI_MESSAGE* msg )
{
	wchar_t * item_name;
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
		{
		case 0:
			item_name=SET_CALLS_TXT;
			break;
		case 1:
			item_name=SET_SMS_DIG_TXT;
			break;
		case 2:
			item_name=SET_SMS_TEXT_TXT;
			break;
		}
		
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	
	return 1;
}


int BlackListBook_ChooseModeToEdit_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	BLBook->choosemodetoedit_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->choosemodetoedit_list, onCallback_ChooseModeToEdit );
	ListMenu_SetItemCount( BLBook->choosemodetoedit_list, 3 );
	ListMenu_SetCursorToItem( BLBook->choosemodetoedit_list, 0 );
	GUIObject_SetTitleText( BLBook->choosemodetoedit_list, TextID_Create( EDIT_LISTS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetAction( BLBook->choosemodetoedit_list, ACTION_BACK, ChooseModeToEdit_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->choosemodetoedit_list, ACTION_LONG_BACK, ChooseModeToEdit_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->choosemodetoedit_list, ACTION_SELECT1, ChooseModeToEdit_Select_Action );
	GUIObject_Show( BLBook->choosemodetoedit_list );
	return 0;
}


int BlackListBook_ChooseModeToEdit_Page_Cancel_Action(void * ,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 0;
}


int BlackListBook_ChooseModeToEdit_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->choosemodetoedit_list)
	{
		GUIObject_Destroy(BLBook->choosemodetoedit_list);
		BLBook->choosemodetoedit_list=0;
	}
	
	return 0;
}

//====================================================================

void OOMList_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
}


void OOMList_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void OOMList_Select_Action(BOOK * book, GUI * gui)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int selected=OneOfMany_GetSelected(gui);
	
	switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
	{
	case 0:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			BLBook->settings_calls.calls_control_state=selected;
			break;
		case 1:
			BLBook->settings_calls.calls_block_type=selected;
			break;
		case 2:
			BLBook->settings_calls.calls_block_mode=selected;
			break;
		case 3:
			BLBook->settings_calls.calls_log=selected;
			break;
		case 4:
			BLBook->settings_calls.calls_notification=selected;
			break;
		}
		break;
	case 1:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			BLBook->settings_sms_dig.sms_dig_control_state=selected;
			break;
		case 1:
			BLBook->settings_sms_dig.sms_dig_block_mode=selected;
			break;
		case 2:
			BLBook->settings_sms_dig.sms_dig_log=selected;
			break;
		case 3:
			BLBook->settings_sms_dig.sms_dig_notification=selected;
			break;
		}
		break;
	case 2:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			BLBook->settings_sms_text.sms_text_control_state=selected;
			break;
		case 1:
			BLBook->settings_sms_text.sms_text_block_mode=selected;
			break;
		case 2:
			BLBook->settings_sms_text.sms_text_log=selected;
			break;
		case 3:
			BLBook->settings_sms_text.sms_text_notification=selected;
			break;
		}
		break;
	case 3:
		BLBook->settings_common.language=selected;
		InitLanguage(BLBook);
		break;
	}
	
	BookObj_ReturnPage( BLBook, ACCEPT_EVENT );
}


int onCallback_OOMList( GUI_MESSAGE* msg )
{
	BlackListBook * BLBook = (BlackListBook*)GUIonMessage_GetBook( msg );
	
	wchar_t * item_name;
	int created_index = GUIonMessage_GetCreatedItemIndex( msg );
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
		{
		case 0:
			switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
			{
			case 0:
				switch ( created_index )
				{
				case CONTROL_STATE_ON:
					item_name=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				switch ( created_index )
				{
				case TYPE_BUSY:
					item_name=BLOCK_TYPE_BUSY_TXT;
					break;
				case TYPE_OUT_OF_SERVICE:
					item_name=BLOCK_TYPE_OUT_OF_SERVICE_TXT;
					break;
				}
				break;
			case 2:
				switch ( created_index )
				{
				case MODE_ALL:
					item_name=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 3:
				switch ( created_index )
				{
				case STATE_LOG_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 4:
				switch ( created_index )
				{
				case STATE_NOTIFIC_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		case 1:
			switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
			{
			case 0:
				switch ( created_index )
				{
				case CONTROL_STATE_ON:
					item_name=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				switch ( created_index )
				{
				case MODE_ALL:
					item_name=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 2:
				switch ( created_index )
				{
				case STATE_LOG_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 3:
				switch ( created_index )
				{
				case STATE_NOTIFIC_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		case 2:
			switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
			{
			case 0:
				switch ( created_index )
				{
				case CONTROL_STATE_ON:
					item_name=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				switch ( created_index )
				{
				case MODE_ALL:
					item_name=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 2:
				switch ( created_index )
				{
				case STATE_LOG_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			case 3:
				switch ( created_index )
				{
				case STATE_NOTIFIC_ON:
					item_name=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		case 3:
			switch ( created_index )
			{
			case LANGUAGE_EN:
				item_name=LANGUAGE_EN_TXT;
				break;
			case LANGUAGE_RU:
				item_name=LANGUAGE_RU_TXT;
				break;
			}
			break;
		}
		
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	return 1;
}


int BlackListBook_SettingsOOM_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	TEXTID title;
	int num_of_item;
	int checked;
	
	switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
	{
	case 0:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			title = TextID_Create(SET_CONTROL_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_calls.calls_control_state;
			break;
		case 1:
			title = TextID_Create(SET_BLOCK_TYPE_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_calls.calls_block_type;
			break;
		case 2:
			title = TextID_Create(SET_BLOCK_MODE_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 3;
			checked = BLBook->settings_calls.calls_block_mode;
			break;
		case 3:
			title = TextID_Create(SET_LOG_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_calls.calls_log;
			break;
		case 4:
			title = TextID_Create(SET_NOTIFICATION_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_calls.calls_notification;
			break;
		}
		break;
	case 1:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			title = TextID_Create(SET_CONTROL_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_dig.sms_dig_control_state;
			break;
		case 1:
			title = TextID_Create(SET_BLOCK_MODE_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 3;
			checked = BLBook->settings_sms_dig.sms_dig_block_mode;
			break;
		case 2:
			title = TextID_Create(SET_LOG_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_dig.sms_dig_log;
			break;
		case 3:
			title = TextID_Create(SET_NOTIFICATION_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_dig.sms_dig_notification;
			break;
		}
		break;
	case 2:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_calls_sms_list) )
		{
		case 0:
			title = TextID_Create(SET_CONTROL_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_text.sms_text_control_state;
			break;
		case 1:
			title = TextID_Create(SET_BLOCK_MODE_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 3;
			checked = BLBook->settings_sms_text.sms_text_block_mode;
			break;
		case 2:
			title = TextID_Create(SET_LOG_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_text.sms_text_log;
			break;
		case 3:
			title = TextID_Create(SET_NOTIFICATION_TXT,ENC_UCS2,TEXTID_ANY_LEN);
			num_of_item = 2;
			checked = BLBook->settings_sms_text.sms_text_notification;
			break;
		}
		break;
	case 3:
		title = TextID_Create(SET_LANGUAGE_TXT,ENC_UCS2,TEXTID_ANY_LEN);
		num_of_item = 2;
		checked = BLBook->settings_common.language;
		break;
	}
	
	BLBook->settings_oom=CreateOneOfMany(BLBook);
	GUIObject_SetTitleText(BLBook->settings_oom,title);
	OneOfMany_SetItemCount(BLBook->settings_oom,num_of_item);
	OneOfMany_SetChecked(BLBook->settings_oom,checked);
	OneOfMany_SetOnMessage(BLBook->settings_oom,onCallback_OOMList);
	GUIObject_SoftKeys_SetAction(BLBook->settings_oom,ACTION_BACK,OOMList_Prev_Action);
	GUIObject_SoftKeys_SetAction(BLBook->settings_oom,ACTION_LONG_BACK,OOMList_Cancel_Action);
	GUIObject_SoftKeys_SetAction(BLBook->settings_oom,ACTION_SELECT1,OOMList_Select_Action);
	GUIObject_Show(BLBook->settings_oom);
	
	return 0;
}


int BlackListBook_SettingsOOM_Page_Cancel_Action(void * ,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 0;
}


int BlackListBook_SettingsOOM_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->settings_oom)
	{
		GUIObject_Destroy(BLBook->settings_oom);
		BLBook->settings_oom=0;
	}
	
	return 0;
}

//====================================================================

void SettingsCallsSMSList_Select_Action(BOOK * book, GUI * gui)
{
	BookObj_CallPage( book, &BlackListBook_SettingsOOM_Page );
}


void SettingsCallsSMSList_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void SettingsCallsSMSList_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,ACCEPT_EVENT);
}


int onCallback_SettingsCallsSMSList( GUI_MESSAGE* msg )
{
	BlackListBook * BLBook = (BlackListBook*)GUIonMessage_GetBook( msg );
	
	wchar_t * item_name;
	wchar_t * item_name_2;
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
		{
		case 0:
			switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
			{
			case 0:
				item_name=SET_CONTROL_TXT;
				switch ( BLBook->settings_calls.calls_control_state )
				{
				case CONTROL_STATE_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				item_name=SET_BLOCK_TYPE_TXT;
				switch ( BLBook->settings_calls.calls_block_type )
				{
				case TYPE_BUSY:
					item_name_2=BLOCK_TYPE_BUSY_TXT;
					break;
				case TYPE_OUT_OF_SERVICE:
					item_name_2=BLOCK_TYPE_OUT_OF_SERVICE_TXT;
					break;
				}
				break;
			case 2:
				item_name=SET_BLOCK_MODE_TXT;
				switch ( BLBook->settings_calls.calls_block_mode )
				{
				case MODE_ALL:
					item_name_2=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name_2=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name_2=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 3:
				item_name=SET_LOG_TXT;
				switch ( BLBook->settings_calls.calls_log )
				{
				case STATE_LOG_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 4:
				item_name=SET_NOTIFICATION_TXT;
				switch ( BLBook->settings_calls.calls_notification )
				{
				case STATE_NOTIFIC_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		case 1:
			switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
			{
			case 0:
				item_name=SET_CONTROL_TXT;
				switch ( BLBook->settings_sms_dig.sms_dig_control_state )
				{
				case CONTROL_STATE_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				item_name=SET_BLOCK_MODE_TXT;
				switch ( BLBook->settings_sms_dig.sms_dig_block_mode )
				{
				case MODE_ALL:
					item_name_2=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name_2=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name_2=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 2:
				item_name=SET_LOG_TXT;
				switch ( BLBook->settings_sms_dig.sms_dig_log )
				{
				case STATE_LOG_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 3:
				item_name=SET_NOTIFICATION_TXT;
				switch ( BLBook->settings_sms_dig.sms_dig_notification )
				{
				case STATE_NOTIFIC_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		case 2:
			switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
			{
			case 0:
				item_name=SET_CONTROL_TXT;
				switch ( BLBook->settings_sms_text.sms_text_control_state )
				{
				case CONTROL_STATE_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case CONTROL_STATE_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 1:
				item_name=SET_BLOCK_MODE_TXT;
				switch ( BLBook->settings_sms_text.sms_text_block_mode )
				{
				case MODE_ALL:
					item_name_2=BLOCK_MODE_ALL_TXT;
					break;
				case MODE_BLACK:
					item_name_2=BLOCK_MODE_BL_TXT;
					break;
				case MODE_WHITE:
					item_name_2=BLOCK_MODE_WL_TXT;
					break;
				}
				break;
			case 2:
				item_name=SET_LOG_TXT;
				switch ( BLBook->settings_sms_text.sms_text_log )
				{
				case STATE_LOG_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_LOG_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			case 3:
				item_name=SET_NOTIFICATION_TXT;
				switch ( BLBook->settings_sms_text.sms_text_notification )
				{
				case STATE_NOTIFIC_ON:
					item_name_2=STATE_ON_TXT;
					break;
				case STATE_NOTIFIC_OFF:
					item_name_2=STATE_OFF_TXT;
					break;
				}
				break;
			}
			break;
		}
		
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
		GUIonMessage_SetMenuItemSecondLineText( msg, TextID_Create( item_name_2, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	return 1;
}


int BlackListBook_Settings_CallsSMS_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	int num_of_items;
	
	switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
	{
	case 0:
		num_of_items = sizeof(SETTINGS_CALLS);
		break;
	case 1:
		num_of_items = sizeof(SETTINGS_SMS_DIG);
		break;
	case 2:
		num_of_items = sizeof(SETTINGS_SMS_TEXT);
		break;
	}
	
	if (BLBook->settings_calls_sms_list)
	{
		int pos = ListMenu_GetSelectedItem( BLBook->settings_calls_sms_list );
		ListMenu_DestroyItems( BLBook->settings_calls_sms_list );
		GUIObject_SoftKeys_SetVisible(BLBook->settings_calls_sms_list, ACTION_SELECT1, TRUE);
		ListMenu_SetItemCount( BLBook->settings_calls_sms_list, num_of_items );
		GUIObject_SetTitleText( BLBook->settings_calls_sms_list, TextID_Create( SETTINGS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
		ListMenu_SetCursorToItem( BLBook->settings_calls_sms_list,pos );
		return 0;
	}
	BLBook->settings_calls_sms_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->settings_calls_sms_list, onCallback_SettingsCallsSMSList );
	ListMenu_SetItemCount( BLBook->settings_calls_sms_list, num_of_items );
	ListMenu_SetCursorToItem( BLBook->settings_calls_sms_list, 0 );
	ListMenu_SetItemStyle( BLBook->settings_calls_sms_list, 2 );
	GUIObject_SetTitleText( BLBook->settings_calls_sms_list, TextID_Create( SETTINGS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetAction( BLBook->settings_calls_sms_list, ACTION_BACK, SettingsCallsSMSList_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->settings_calls_sms_list, ACTION_LONG_BACK, SettingsCallsSMSList_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->settings_calls_sms_list, ACTION_SELECT1, SettingsCallsSMSList_Select_Action );
	GUIObject_Show( BLBook->settings_calls_sms_list );
	return 0;
}


int BlackListBook_Settings_CallsSMS_Page_Cancel_Action(void * data,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 1;
}


int BlackListBook_Settings_CallsSMS_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	int settings_id;
	
	switch ( ListMenu_GetSelectedItem(BLBook->settings_list) )
	{
	case 0:
		settings_id = SETTINGS_ID_CALLS;
		break;
	case 1:
		settings_id = SETTINGS_ID_SMS_DIG;
		break;
	case 2:
		settings_id = SETTINGS_ID_SMS_TEXT;
		break;
	}
	
	SetSettings(BLBook,settings_id);
		
	
	if (BLBook->settings_calls_sms_list)
	{
		GUIObject_Destroy(BLBook->settings_calls_sms_list);
		BLBook->settings_calls_sms_list=0;
	}
	
	return 0;
}

//====================================================================

void SettingsList_Select_Action(BOOK * book, GUI * gui)
{
	switch ( ListMenu_GetSelectedItem(gui) )
	{
	case 0:
	case 1:
	case 2:
		BookObj_CallPage( book, &BlackListBook_Settings_CallsSMS_Page );
		break;
	case 3:
		BookObj_CallPage( book, &BlackListBook_SettingsOOM_Page );
		break;
	}
}


void SettingsList_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
}


void SettingsList_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_ReturnPage(book,ACCEPT_EVENT);
}


int onCallback_SettingsList( GUI_MESSAGE* msg )
{
	BlackListBook * BLBook = (BlackListBook*)GUIonMessage_GetBook( msg );
	
	wchar_t * item_name;
	
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
		{
		case 0:
			item_name=SET_CALLS_TXT;
			break;
		case 1:
			item_name=SET_SMS_DIG_TXT;
			break;
		case 2:
			item_name=SET_SMS_TEXT_TXT;
			break;
		case 3:
			item_name=SET_LANGUAGE_TXT;
			break;
		}
		
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	return 1;
}


int BlackListBook_Settings_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->settings_list)
	{
		int pos = ListMenu_GetSelectedItem( BLBook->settings_list );
		ListMenu_DestroyItems( BLBook->settings_list );
		GUIObject_SoftKeys_SetVisible(BLBook->settings_list, ACTION_SELECT1, TRUE);
		ListMenu_SetItemCount( BLBook->settings_list, 4 );
		GUIObject_SetTitleText( BLBook->settings_list, TextID_Create( SETTINGS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
		ListMenu_SetCursorToItem( BLBook->settings_list,pos );
		return 0;
	}
	BLBook->settings_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->settings_list, onCallback_SettingsList );
	ListMenu_SetItemCount( BLBook->settings_list, 4 );
	ListMenu_SetCursorToItem( BLBook->settings_list, 0 );
	GUIObject_SetTitleText( BLBook->settings_list, TextID_Create( SETTINGS_TXT, ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetAction( BLBook->settings_list, ACTION_BACK, SettingsList_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->settings_list, ACTION_LONG_BACK, SettingsList_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->settings_list, ACTION_SELECT1, SettingsList_Select_Action );
	GUIObject_Show( BLBook->settings_list );
	return 0;
}


int BlackListBook_Settings_Page_Cancel_Action(void * data,BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return 1;
}


int BlackListBook_Settings_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	SetSettings(BLBook,SETTINGS_ID_COMMON);
	
	if (BLBook->settings_list)
	{
		GUIObject_Destroy(BLBook->settings_list);
		BLBook->settings_list=0;
	}
	
	return 0;
}

//====================================================================

void MainList_Select_Action(BOOK * book, GUI * gui)
{
	switch ( ListMenu_GetSelectedItem(gui) )
	{
	case 0:
		BookObj_CallPage( book, &BlackListBook_Settings_Page );
		break;
	case 1:
		BookObj_CallPage( book, &BlackListBook_ChooseModeToEdit_Page );
		break;
	}
}


void MainList_Prev_Action(BOOK * book, GUI * gui)
{
	BookObj_GotoPage(book,&BlackListBook_Idle_Page);
}


void MainList_Cancel_Action(BOOK * book, GUI * gui)
{
	BookObj_GotoPage(book,&BlackListBook_Idle_Page);
}


int onCallback_MainList( GUI_MESSAGE* msg )
{
	wchar_t * item_name;
	switch( GUIonMessage_GetMsg( msg ) )
	{
	case LISTMSG_GetItem:
		switch ( GUIonMessage_GetCreatedItemIndex( msg ) )
		{
		case 0:
			item_name=SETTINGS_TXT;
			break;
		case 1:
			item_name=EDIT_LISTS_TXT;
			break;
		}
		GUIonMessage_SetMenuItemText( msg, TextID_Create( item_name, ENC_UCS2, TEXTID_ANY_LEN ) );
	}
	return 1;
}


int BlackListBook_Main_Page_Enter_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->main_list)
	{
		int pos = ListMenu_GetSelectedItem( BLBook->main_list );
		ListMenu_DestroyItems( BLBook->main_list );
		GUIObject_SoftKeys_SetVisible(BLBook->main_list, ACTION_SELECT1, TRUE);
		ListMenu_SetItemCount( BLBook->main_list, 2 );
		ListMenu_SetCursorToItem( BLBook->main_list,pos );
		return 0;
	}
	BLBook->main_list=CreateListMenu( BLBook, 0 );
	ListMenu_SetOnMessage( BLBook->main_list, onCallback_MainList );
	ListMenu_SetItemCount( BLBook->main_list, 2 );
	ListMenu_SetCursorToItem( BLBook->main_list, 0 );
	GUIObject_SetTitleText( BLBook->main_list, TextID_Create( L"Black List", ENC_UCS2, TEXTID_ANY_LEN ) );
	GUIObject_SoftKeys_SetAction( BLBook->main_list, ACTION_BACK, MainList_Prev_Action );
	GUIObject_SoftKeys_SetAction( BLBook->main_list, ACTION_LONG_BACK, MainList_Cancel_Action );
	GUIObject_SoftKeys_SetAction( BLBook->main_list, ACTION_SELECT1, MainList_Select_Action );
	GUIObject_Show( BLBook->main_list );
	return 0;
}


int BlackListBook_Main_Page_Exit_Action(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->main_list)
	{
		GUIObject_Destroy(BLBook->main_list);
		BLBook->main_list=0;
	}
	
	return 0;
}


int BlackListBook_Main_Page_Cancel_Action(void * ,BOOK * book)
{
	BookObj_GotoPage(book,&BlackListBook_Idle_Page);
	
	return 0;
}

//====================================================================

int BlackListBook_Idle_Page_EnterAction(void * ,BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	BookObj_WindowSetWantsFocus(BLBook,0,FALSE);
	BookObj_Hide(BLBook,0);
	
	return 0;
}

//====================================================================

void Create_BL_GUI(BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	BookObj_WindowSetWantsFocus(BLBook,0,TRUE);
	BookObj_Show(BLBook,0);
	if (!BLBook->main_list)
		BookObj_GotoPage(BLBook,&BlackListBook_Main_Page);
}


void destroy_list(LIST* list)
{
	List_DestroyElements(list, elem_filter, filter_list_elem_free);
	List_Destroy(list);
}


void BlackListBook_Destroy(BOOK * book)
{
	BlackListBook * BLBook = (BlackListBook*)book;
	
	if (BLBook->black_list_calls.block_list)
	{
		destroy_list(BLBook->black_list_calls.block_list);
	}
	if (BLBook->white_list_calls.block_list)
	{
		destroy_list(BLBook->white_list_calls.block_list);
	}
	if (BLBook->black_list_sms_dig.block_list)
	{
		destroy_list(BLBook->black_list_sms_dig.block_list);
	}
	if (BLBook->white_list_sms_dig.block_list)
	{
		destroy_list(BLBook->white_list_sms_dig.block_list);
	}
	if (BLBook->black_list_sms_text.block_list)
	{
		destroy_list(BLBook->black_list_sms_text.block_list);
	}
	if (BLBook->white_list_sms_text.block_list)
	{
		destroy_list(BLBook->white_list_sms_text.block_list);
	}
	if (BLBook->concat_sms_list)
	{
		List_DestroyElements(BLBook->concat_sms_list, elem_filter, sms_concat_elem_free);
		List_Destroy(BLBook->concat_sms_list);
	}
	SUBPROC(elf_exit);
}


BlackListBook* CreateBlackListBook()
{
	BlackListBook * BLBook = new BlackListBook;
	CreateBook(BLBook,BlackListBook_Destroy, &BlackListBook_Base_Page, BOOK_NAME, -1, 0);
	BLBook->FilterCall=BlackListBook_FilterCall;
	BLBook->CheckNumber=BlackListBook_CheckNumber;
	BLBook->EditLists=BlackListBook_EditLists;
	BLBook->FilterSMS=BlackListBook_FilterSMS;
	BLBook->CreateGUI=Create_BL_GUI;
	{
		BLBook->settings_common.language=LANGUAGE_EN;
	}
	{
		BLBook->settings_calls.calls_control_state=CONTROL_STATE_OFF;
		BLBook->settings_calls.calls_block_type=TYPE_BUSY;
		BLBook->settings_calls.calls_block_mode=MODE_ALL;
		BLBook->settings_calls.calls_log=STATE_LOG_OFF;
		BLBook->settings_calls.calls_notification=STATE_NOTIFIC_OFF;
	}
	{
		BLBook->settings_sms_dig.sms_dig_control_state=CONTROL_STATE_OFF;
		BLBook->settings_sms_dig.sms_dig_block_mode=MODE_ALL;
		BLBook->settings_sms_dig.sms_dig_log=STATE_LOG_OFF;
		BLBook->settings_sms_dig.sms_dig_notification=STATE_NOTIFIC_OFF;
	}
	{
		BLBook->settings_sms_text.sms_text_control_state=CONTROL_STATE_OFF;
		BLBook->settings_sms_text.sms_text_block_mode=MODE_ALL;
		BLBook->settings_sms_text.sms_text_log=STATE_LOG_OFF;
		BLBook->settings_sms_text.sms_text_notification=STATE_NOTIFIC_OFF;
	}
	BLBook->edit_flag=FALSE;
	BLBook->main_list=0;
	BLBook->settings_list=0;
	BLBook->settings_calls_sms_list=0;
	BLBook->settings_oom=0;
	BLBook->choosemodetoedit_list=0;
	BLBook->chooselisttoedit_list=0;
	BLBook->editblocklist_list=0;
	BLBook->str_inp=0;
	BLBook->black_list_calls.unknown=0;
	BLBook->black_list_calls.known=0;
	BLBook->black_list_calls.block_list=List_Create();
	BLBook->white_list_calls.unknown=0;
	BLBook->white_list_calls.known=0;
	BLBook->white_list_calls.block_list=List_Create();
	BLBook->black_list_sms_dig.unknown=0;
	BLBook->black_list_sms_dig.known=0;
	BLBook->black_list_sms_dig.block_list=List_Create();
	BLBook->white_list_sms_dig.unknown=0;
	BLBook->white_list_sms_dig.known=0;
	BLBook->white_list_sms_dig.block_list=List_Create();
	BLBook->black_list_sms_text.unknown=0;
	BLBook->black_list_sms_text.known=0;
	BLBook->black_list_sms_text.block_list=List_Create();
	BLBook->white_list_sms_text.unknown=0;
	BLBook->white_list_sms_text.known=0;
	BLBook->white_list_sms_text.block_list=List_Create();
	BLBook->concat_sms_list=List_Create();
	BLBook->isUnk=0xFF;
		
	GetSettings(BLBook,SETTINGS_ID_COMMON);
	GetSettings(BLBook,SETTINGS_ID_CALLS);
	GetSettings(BLBook,SETTINGS_ID_SMS_DIG);
	GetSettings(BLBook,SETTINGS_ID_SMS_TEXT);
	InitLanguage(BLBook);
	ReadBlockList(&BLBook->black_list_calls,BLACKLIST_CALLS_FNAME);
	ReadBlockList(&BLBook->white_list_calls,WHITELIST_CALLS_FNAME);
	ReadBlockList(&BLBook->black_list_sms_dig,BLACKLIST_SMS_DIG_FNAME);
	ReadBlockList(&BLBook->white_list_sms_dig,WHITELIST_SMS_DIG_FNAME);
	ReadBlockList(&BLBook->black_list_sms_text,BLACKLIST_SMS_TEXT_FNAME);
	ReadBlockList(&BLBook->white_list_sms_text,WHITELIST_SMS_TEXT_FNAME);
	
	int image;
	iconidname2id(CHECKBOX_EMPTY_ICON,TEXTID_ANY_LEN,&image);
	BLBook->chkbox_unmarked = image;
	iconidname2id(CHECKBOX_MARK_ICON,TEXTID_ANY_LEN,&image);
	BLBook->chkbox_marked = image;
	
	return BLBook;
}


int isBlackListBook(BOOK* book)
{
	if(!strcmp(book->xbook->name,BOOK_NAME)) return(1);
	return(0);
}


int main (wchar_t * elfname,wchar_t * fpath,wchar_t * fname)
{
	BlackListBook * BLBook = (BlackListBook*)FindBook(isBlackListBook);
	
	if (!BLBook)
	{
		BLBook = CreateBlackListBook();
		BookObj_GotoPage(BLBook,&BlackListBook_Idle_Page);
	}
	else
	{
		BLBook->CreateGUI(BLBook);
		SUBPROC(elf_exit);
	}
	return 0;
}
