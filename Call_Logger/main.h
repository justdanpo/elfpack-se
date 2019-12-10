typedef enum
{
	CALLMANAGER_IDLE = 0x0,
	CALLMANAGER_CALL_CONNECTED = 0x1,
	CALLMANAGER_CALL_SETUP = 0x2,
	CALLMANAGER_CALL_ALERT = 0x4,
	CALLMANAGER_CALL_WAITING = 0x5,
	CALLMANAGER_CALL_TERMINATED = 0x6
}CALLMANAGER_CALL_STATE;
	

typedef struct
{
	char CallState;
	char CallID;
	BCD_TEXT PNUM;
}CALLMANAGER_DATA;


typedef struct
{
	char pos1;
	char cale_base;
	short pos3;
}CALE_SET;


typedef struct
{
	char dummy[0x2C];
	int unkID;
}ses_est;


typedef struct
{
	char dummy[0x34];
	int unkID;
}ses_term;


typedef struct
{
	wchar_t * w_PNUM;
	wchar_t * Name;
	int first_callstate;
	int last_callstate;
	DATETIME * startdatetime;
	DATETIME * begindatetime;
	char line;
}call_elem;


typedef struct
{
	int ID;
	DATETIME * SesEst;
}myList_gprs_elem;


typedef struct
{
	BOOK * book;
}MSG;


typedef struct _MYBOOK : BOOK
{
	LIST * active_calls;
	LIST * myList_gprs;
	DATETIME * cur_date;
	int incomleted_connection;
	int terminatedID;
}CallLogBook;
