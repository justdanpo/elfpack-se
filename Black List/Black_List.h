#ifndef _BLACKLIST_H_
#define _BLACKLIST_H_

#define NUM_CHR_SUBS 1
#define BOOK_NAME "Black List"
#define SMS_LOG_MASK L"From: %ls (%s)\r\nWhen: 20%02x-%02x-%02x, %02x:%02x:%02x\r\n%ls\r\n\r\n"

#define CHECKBOX_EMPTY_ICON L"CHECKBOX_ICN"
#define CHECKBOX_MARK_ICON L"CHECKMARK_IN_BOX_ICN"

#define MAIN_FPATH L"/BlackList"
#define SETTINGS_COMMON_FNAME L"settings_common.bin"
#define SETTINGS_CALLS_FNAME L"settings_calls.bin"
#define SETTINGS_SMS_DIG_FNAME L"settings_sms_dig.bin"
#define SETTINGS_SMS_TEXT_FNAME L"settings_sms_text.bin"
#define BLACKLIST_CALLS_FNAME L"black_list_calls.bin"
#define WHITELIST_CALLS_FNAME L"white_list_calls.bin"
#define BLACKLIST_SMS_DIG_FNAME L"black_list_sms_dig.bin"
#define WHITELIST_SMS_DIG_FNAME L"white_list_sms_dig.bin"
#define BLACKLIST_SMS_TEXT_FNAME L"black_list_sms_text.bin"
#define WHITELIST_SMS_TEXT_FNAME L"white_list_sms_text.bin"
#define LOG_CALLS_FNAME L"blocked.txt"
#define LOG_SMS_FNAME L"sms_blocked.txt"

#define EN_SET_CALLS_TXT L"Calls"
#define EN_SET_SMS_DIG_TXT L"SMS (digits)"
#define EN_SET_SMS_TEXT_TXT L"SMS (text)"
#define EN_SET_CONTROL_TXT L"Control"
#define EN_SET_BLOCK_TYPE_TXT L"Block Type"
#define EN_SET_BLOCK_MODE_TXT L"Block Mode"
#define EN_SET_LOG_TXT L"Log"
#define EN_SET_NOTIFICATION_TXT L"Notification"
#define EN_SET_LANGUAGE_TXT L"Language"
#define EN_STATE_ON_TXT L"On"
#define EN_STATE_OFF_TXT L"Off"
#define EN_BLOCK_TYPE_BUSY_TXT L"Busy"
#define EN_BLOCK_TYPE_OUT_OF_SERVICE_TXT L"Out of Service"
#define EN_BLOCK_MODE_ALL_TXT L"All"
#define EN_BLOCK_MODE_BL_TXT L"Black List"
#define EN_BLOCK_MODE_WL_TXT L"White List"
#define EN_SETTINGS_TXT L"Settings"
#define EN_EDIT_LISTS_TXT L"Edit Lists"
#define EN_NUMBER_EXISTS_TXT L"Already exists"
#define EN_ILLEGAL_SYMBOLS_ERROR_TXT L"Only digits, * and ? allowed"
#define EN_ENTER_NUMBER_TXT L"Enter number"
#define EN_ENTER_MASK_TXT L"Enter mask"
#define EN_EMPTY_TXT L"Empty"
#define EN_FROM_PB_TXT L"From PB"
#define EN_MANUAL_TXT L"Manual"
#define EN_UNKNOWN_PNUM_TXT L"Unsaved number"
#define EN_MASK_TXT L"Mask"
#define EN_LANGUAGE_EN_TXT L"English"
#define EN_LANGUAGE_RU_TXT L"Russian"
#define EN_BLOCKED_TXT L"Blocked"
#define EN_BLOCKED_TYPE_SMS_TXT L"SMS"
#define EN_BLOCKED_TYPE_CALL_TXT L"Call"
#define EN_ALL_UNKNOWN_TXT L"Unknowns"
#define EN_ALL_UNKNOWN_SECOND_LINE_TXT L"All unknowns"
#define EN_ALL_KNOWN_TXT L"Phonebook"
#define EN_ALL_KNOWN_SECOND_LINE_TXT L"All from PB"


#define RU_SET_CALLS_TXT L"Звонки"
#define RU_SET_SMS_DIG_TXT L"СМС (цифры)"
#define RU_SET_SMS_TEXT_TXT L"СМС (текст)"
#define RU_SET_CONTROL_TXT L"Управление"
#define RU_SET_BLOCK_TYPE_TXT L"Режим отклонения"
#define RU_SET_BLOCK_MODE_TXT L"Режим блокировки"
#define RU_SET_LOG_TXT L"Логирование"
#define RU_SET_NOTIFICATION_TXT L"Оповещение"
#define RU_SET_LANGUAGE_TXT L"Язык"
#define RU_STATE_ON_TXT L"Включено"
#define RU_STATE_OFF_TXT L"Выключено"
#define RU_BLOCK_TYPE_BUSY_TXT L"Занято"
#define RU_BLOCK_TYPE_OUT_OF_SERVICE_TXT L"Не доступен"
#define RU_BLOCK_MODE_ALL_TXT L"Все"
#define RU_BLOCK_MODE_BL_TXT L"Черный список"
#define RU_BLOCK_MODE_WL_TXT L"Белый список"
#define RU_SETTINGS_TXT L"Настройки"
#define RU_EDIT_LISTS_TXT L"Редактировать списки"
#define RU_NUMBER_EXISTS_TXT L"Уже существует"
#define RU_ILLEGAL_SYMBOLS_ERROR_TXT L"Разрешено использование только цифр, * и ?"
#define RU_ENTER_NUMBER_TXT L"Введите номер"
#define RU_ENTER_MASK_TXT L"Введите маску"
#define RU_EMPTY_TXT L"Пусто"
#define RU_FROM_PB_TXT L"Из тел.книги"
#define RU_MANUAL_TXT L"Вручную"
#define RU_UNKNOWN_PNUM_TXT L"Несохраненный номер"
#define RU_MASK_TXT L"Маска"
#define RU_LANGUAGE_EN_TXT L"Английский"
#define RU_LANGUAGE_RU_TXT L"Русский"
#define RU_BLOCKED_TXT L"Заблокирован"
#define RU_BLOCKED_TYPE_SMS_TXT L"СМС"
#define RU_BLOCKED_TYPE_CALL_TXT L"Вызов"
#define RU_ALL_UNKNOWN_TXT L"Неизвестные"
#define RU_ALL_UNKNOWN_SECOND_LINE_TXT L"Все неизвестные"
#define RU_ALL_KNOWN_TXT L"Тел. книга"
#define RU_ALL_KNOWN_SECOND_LINE_TXT L"Все номера из тел.книги"


typedef enum
{
	SETTINGS_ID_COMMON,
	SETTINGS_ID_CALLS,
	SETTINGS_ID_SMS_DIG,
	SETTINGS_ID_SMS_TEXT
}SETTINGS_ID;


typedef struct
{
	void * unk;
	char * pnum_str;
}PB_SEARCH_DATA;


typedef struct
{
	char Digits[0x51];
	char TypeOfNumber;
	char NumberingPlanId;
}TNumber;


typedef struct
{
	TNumber Number;
	char PresentationIndicatorValue;
	char ScreeningIndicatorValue;
	char CauseOfNoCliValue;
}TCallSupplementaryCli;


typedef struct
{
	void * unk;
	void * pICallManager;
}CLH_SERVER_DATA;


typedef struct
{
	int (*CClhServer_ICBCallManager_OnSetupPresent)(void * pICBCallManager, TCallSupplementaryCli * Cli, int TTelephonyCallType, int CallId, void * ClientData);
	CLH_SERVER_DATA* (*CLH_Server_GetData)();
	int (*CLH_Wrapper_ICallManager_RejectSetup)(void * pICallManager, int DisconnectCause);
}MYFUNCTIONS;


typedef enum
{
	CONTROL_STATE_ON,
	CONTROL_STATE_OFF
}SETTINGS_CONTROL_STATE;


typedef enum
{
	TYPE_BUSY,
	TYPE_OUT_OF_SERVICE
}SETTINGS_BLOCK_TYPE;


typedef enum
{
	MODE_ALL,
	MODE_BLACK,
	MODE_WHITE
}SETTINGS_BLOCK_MODE;


typedef enum
{
	STATE_LOG_ON,
	STATE_LOG_OFF
}SETTINGS_LOG_STATE;


typedef enum
{
	STATE_NOTIFIC_ON,
	STATE_NOTIFIC_OFF
}SETTINGS_NOTIFIC_STATE;


typedef enum
{
	LANGUAGE_EN,
	LANGUAGE_RU
}SETTINGS_LANGUAGE;


typedef struct
{
	char calls_control_state;
	char calls_block_type;
	char calls_block_mode;
	char calls_log;
	char calls_notification;
}SETTINGS_CALLS;


typedef struct
{
	char language;
}SETTINGS_COMMON;


typedef struct
{
	char sms_dig_control_state;
	char sms_dig_block_mode;
	char sms_dig_log;
	char sms_dig_notification;
}SETTINGS_SMS_DIG;


typedef struct
{
	char sms_text_control_state;
	char sms_text_block_mode;
	char sms_text_log;
	char sms_text_notification;
}SETTINGS_SMS_TEXT;


typedef enum
{
	NOTIFIC_TYPE_CALL,
	NOTIFIC_TYPE_SMS,
	NOTIFIC_TYPE_SMS_TEXT
}NOTIFIC_TYPE;


typedef struct
{
	char part_ID;
	char text_size;		//in bytes
	char* text;
}sms_concat_part;


typedef struct
{
	wchar_t unique_smsID;
	char total_parts_num;
	char tp_dcs;
	LIST* parts_list;
}sms_concat;


typedef enum
{
	PNUM_TYPE_UNKNOWN,
	PNUM_TYPE_KNOWN
}PNUM_TYPE;


typedef enum
{
	FILTERLIST_TYPE_PNUM,
	FILTERLIST_TYPE_MASK
}FILTERLIST_TYPE;


typedef struct
{
	char type;
	char* data;
}filter_list_elem;


typedef struct
{
	char unknown;
	char known;
	LIST* block_list;
}BLOCK_LIST_STRUCT;


typedef struct _MYBOOK : BOOK
{
	int (*FilterCall)(BOOK * book,MYFUNCTIONS * func_array, void * pICBCallManager, TCallSupplementaryCli * Cli, int CallType, int CallId, void * ClientData);
	int (*CheckNumber)(BOOK * book,char * pnum);
	int (*EditLists)(BOOK * book,char * pnum,char mode);
	int (*FilterSMS)(BOOK * book,char* smsc,char* pdu);
	void (*CreateGUI)(BOOK * book);
	SETTINGS_COMMON settings_common;
	SETTINGS_CALLS settings_calls;
	SETTINGS_SMS_DIG settings_sms_dig;
	SETTINGS_SMS_TEXT settings_sms_text;
	char edit_flag;
	GUI_LIST* main_list;
	GUI_LIST* settings_list;
	GUI_LIST* settings_calls_sms_list;
	GUI_ONEOFMANY* settings_oom;
	GUI_LIST* choosemodetoedit_list;
	GUI_LIST* chooselisttoedit_list;
	GUI_LIST* editblocklist_list;
	GUI* str_inp;
	BLOCK_LIST_STRUCT black_list_calls;
	BLOCK_LIST_STRUCT white_list_calls;
	BLOCK_LIST_STRUCT black_list_sms_dig;
	BLOCK_LIST_STRUCT white_list_sms_dig;
	BLOCK_LIST_STRUCT black_list_sms_text;
	BLOCK_LIST_STRUCT white_list_sms_text;
	BLOCK_LIST_STRUCT* cur_list; //мапится существующий, не надо освобождать
	LIST* concat_sms_list;
	char isUnk;		 //костыль для младших платформ
	IMAGEID chkbox_unmarked;
	IMAGEID chkbox_marked;
}BlackListBook;


typedef struct
{
	BOOK * book;
}MSG;



#endif
