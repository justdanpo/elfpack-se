#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"
#include "main.h"


void elf_exit(void){
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int isUSSDBook(BOOK *book)
{
	if(!strcmp(book->xbook->name,"USSDBook")) return true;
	return false;
}

typedef struct
{
	BOOK  book;
}MyBOOK;

DISP_OBJ * StatusIndication;
DISP_OBJ_ONREDRAW_METHOD SIonRedraw;

wchar_t cashfname[64];
wchar_t cashpath[64];

int isLiveBalanceBook(BOOK * struc);

#define MAX_CASH_SIZE (4)

int MaxCASH[MAX_CASH_SIZE];
int CurrentCASH[MAX_CASH_SIZE];

int CASH_SIZE=0;

static const char * const patterns[MAX_CASH_SIZE]=
{
	CashPat0,
	CashPat1,
	CashPat2,
	CashPat3
};

static const int *const progress_colors[MAX_CASH_SIZE]=
{
	&COLOR_CASHPB1,
	&COLOR_CASHPB2,
	&COLOR_CASHPB3,
	&COLOR_CASHPB4
};

volatile int ussdreq_sended=0;

int char16to8(int c)
{
	if (c==0x401) c=0xA8;
	if (c==0x404) c=0xAA;
	if (c==0x407) c=0xAF;
	if (c==0x451) c=0xB8;
	if (c==0x454) c=0xBA;
	if (c==0x457) c=0xBF;
	if ((c>=0x410)&&(c<0x450)) c-=0x350;
	return (c);
}

void imsi2str(const char *imsi, char *str)
{
	unsigned int c, c1;
	int j, m;
	m=0;
	j=0;
	while(j<IMSI_DATA_BYTE_LEN)
	{
		if (m&1) {c1=c>>4; j++;}
		else c1=(c=imsi[j])&0x0F;
		*str++=c1+'0';
		m++;
	}
	*str=0;
}

static int BookHide(void *data, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16)
{
	if(FindBook(isUSSDBook) == book)
	{
		if(!strcmp(page_desc->name,"USSD_Progress_Page"))
		{
			BookObj_Hide(book,0);
			ModifyUIPageHook(PAGE_ENTER_EVENT,BookHide,0,PAGE_HOOK_REMOVE);
		}
	}
	return(1);
}

int SaveCash(void)
{FUNCTION
	int f;
 int res=0;
 if (*cashpath && *cashfname)
 {
	 if ((f=_fopen(cashpath,cashfname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))!=-1)
	 {
		 fwrite(f,CurrentCASH,sizeof(CurrentCASH));
		 fwrite(f,MaxCASH,sizeof(MaxCASH));
		 fclose(f);
		 res=1;
	 }
 }
 return (res);
}

int LoadCash(void)
{FUNCTION
	int s=0;
 int r=0;
 CASH_SIZE=0;
 memset(CurrentCASH,0,sizeof(CurrentCASH));
 memset(MaxCASH,0,sizeof(MaxCASH));
 int f=_fopen(cashpath,cashfname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
 if (f!=-1)
 {
	 s=fread(f,CurrentCASH,sizeof(CurrentCASH));     //by BoBa 4.07.07
	 s=fread(f,MaxCASH,sizeof(MaxCASH));
	 fclose(f);
	 if (s==sizeof(MaxCASH)) r=1;
 }
 if (!r)
 {
	 memcpy(MaxCASH,CurrentCASH,sizeof(MaxCASH));
	 r=SaveCash();
 }
 return (r);
}

void InitCache(void)
{FUNCTION
	char imsi_str[IMSI_DATA_BYTE_LEN*2+1];
 imsi2str(GetIMSI(),imsi_str);
 snwprintf(cashfname,MAXELEMS(cashfname)-1,L"CallCenter_cash_%s.tmp",imsi_str);
 snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL));
 if (LoadCash()<=0)
 {
	 snwprintf(cashpath,MAXELEMS(cashpath)-1,L"%ls/var",GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
	 LoadCash();
 }
 while ((CASH_SIZE<MAX_CASH_SIZE)&&(*patterns[CASH_SIZE])) CASH_SIZE++;
}


static u16 ussd_tmr=0;
static u16 hours_tmr=0;

void EndUSSDtimer(void)
{FUNCTION
	Timer_Kill(&ussd_tmr);
 ussdreq_sended=0;
}

static void ussd_timeout(u16 timer, LPARAM)
{FUNCTION
	ussdreq_sended=0;
 StartHoursTimer();
}

static void ussd_send(u16 timer, LPARAM)
{FUNCTION
	if ( Display_GetFocusedDispObject(0) == GUIObject_GetDispObject(SBY_GetMainInput(Find_StandbyBook()))
		|| FindBook(get_IsAudioPlayerBook()) || FindBook(get_IsFmRadioBook()) || FindBook(get_IsScreenSaverBook())
			|| !ConnectionManager_Connection_GetState())
	{
		char mv[256];
		wchar_t ws[80];
		ussdreq_sended=1;
		VCALL_Init(mv);
		str2wstr(ws,CASHREQNUM);
		VCALL_SetNumber(mv,ws,wstrlen(ws));
		MakeVoiceCall(BookObj_GetSessionID(Find_StandbyBook()), mv, 4);
		Timer_ReSet(&ussd_tmr,60*1000,ussd_timeout,NULL);
		ModifyUIPageHook(PAGE_ENTER_EVENT,BookHide,0,PAGE_HOOK_ADD_AFTER);
	}
	else
	{
		SendCashReq();
	}
}

void SendCashReq(void)
{FUNCTION
	if (ussdreq_sended) return; //Ужо летим ;)
 Timer_ReSet(&ussd_tmr,3*1000,ussd_send,NULL);
}


static void HoursTimerProc(u16 timer, LPARAM)
{FUNCTION
	SendCashReq();
}

void StartHoursTimer(void)
{FUNCTION
	if (CHECK_HOURS)
	{
		DATETIME dt;
		int sc;
		REQUEST_DATEANDTIME_GET(SYNC,&dt);
		sc = 3600*CHECK_HOURS - 60*dt.time.min - dt.time.sec;
		Timer_ReSet(&hours_tmr,sc*1000, HoursTimerProc, NULL);
	}
}


void My_OnRedraw(DISP_OBJ *db,int i1,int i2, int i3)
{
	if (SIonRedraw)
		SIonRedraw(db,i1,i2,i3);

	int scr_w=Display_GetWidth(0);
	int old_font=SetFont(TEXT_FONTSZ);
	int font_size=GetImageHeight(' ')+1;
	int str_id;
	wchar_t ws[32];
	int x=IDLE_X, y=IDLE_Y-((scr_w==240)?24:((scr_w==176)?18:14));
	for (int i=0; i<CASH_SIZE; i++)
	{
		int start_y, end_y, fill, cur, max;
		cur=CurrentCASH[i];
		max=MaxCASH[i];
		fill=((long long)((cur<0)?0:cur))*(scr_w-x-4)/max;
		start_y=y+i*font_size;
		end_y=y+(i+1)*font_size-1;
		DrawRect(x, start_y, scr_w-x-1, end_y, COLOR_TEXTPB, 0x00000000);
		DrawRect(x+1, start_y+1, fill+2, end_y-1, *progress_colors[i], *progress_colors[i]);
		snwprintf(ws, MAXELEMS(ws)-1, L"%s%u.%02u/%u.%02u", ((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100, ((max<0)?0:max)/100, ((max<0)?0:max)%100);
		str_id=TextID_Create(ws,ENC_UCS2,MAXELEMS(ws)-1);
		DrawString(str_id, 2, 0, start_y+1, scr_w, end_y-1, 0, 0, COLOR_TEXTPB, 0x00000000);
		TextID_Destroy(str_id);
	}
	SetFont(old_font);
}

void Send_REDRAW_RELEASE()
{
	// посылаем всем  мессагу со старым и своим методами Redraw
	REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
	memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
	sbm->SB_OldOnRedraw=SIonRedraw;
	sbm->SB_NewOnRedraw=My_OnRedraw;
	UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
}

static int TerminateElf(void * ,BOOK *book)
{FUNCTION
	Send_REDRAW_RELEASE();
 FreeBook(book);
 return(1);
}

static int OnTurnOff(void * ,BOOK *book)
{FUNCTION
	int book_id = BookObj_GetBookID(book);
 FreeBook(book);
 UI_CONTROLLED_SHUTDOWN_RESPONSE(book_id);
 return(1);
}

int state=0;
// 0 - в идле
// 1 - звоним
// 2 - дозвонились
// 3 - завершили

enum
{
	CALLMANAGER_IDLE            =0,
	CALLMANAGER_CALL_CONNECTED  =1, /*Поднятие трубки*/
	CALLMANAGER_CALL_SETUP      =2, /*Нажатие вызвать*/
	CALLMANAGER_CALL_ALERT      =4, /*Входящий вызов*/
	CALLMANAGER_CALL_WAITING    =5,
	CALLMANAGER_CALL_TERMINATED =6  /*Завершение соединения*/
};

static int OnCallManager(void *mess ,BOOK *book)
{FUNCTION
	int callstate;
 switch(GetChipID() & CHIPID_MASK)
 {
 case CHIPID_DB2000:
 case CHIPID_DB2010:
 case CHIPID_DB2020:
	 callstate = ((CALLMANAGER_EVENT_DATA *)mess)->CallState;
	 break;
 case CHIPID_DB3150:
 case CHIPID_DB3200:
 case CHIPID_DB3210:
 case CHIPID_DB3350:
	 callstate = ((CALLMANAGER_EVENT_DATA_A2 *)mess)->CallState;
	 break;
 default:
	 callstate = CALLMANAGER_IDLE;
 }

 switch(callstate)
 {
 case CALLMANAGER_IDLE:  // Завершили вызов выходим
	 if (state==3)  SendCashReq();
	 state=0;
	 break;
 case CALLMANAGER_CALL_SETUP:  // Звоним куда то
 case CALLMANAGER_CALL_ALERT:  // Звонят нам
	 if (ENA_CASHTRACE) state=1;
	 break;
 case CALLMANAGER_CALL_CONNECTED:  // Дозвонились
	 if (state==1) state=2;
	 break;
 case CALLMANAGER_CALL_TERMINATED:  // Закончили вызов
	 if (state==2) state=3;
	 break;
 }
 return(1);
}

static int onNeedCashReq(void *mess, BOOK *book)
{FUNCTION
	SendCashReq();
 return (0);
}

static int ReconfigElf(void * mess,BOOK *book)
{FUNCTION
	RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
 if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
 {
	 InitConfig();
	 StartHoursTimer();
 }
 return(1);
}

static int FindCash(const char *s)
{FUNCTION
	int n=0; //Номер
 const char *pat;
 int i;
 char *ep;
 const char *p;
 const char *pval;
 int c;
 int cc;
 const char *s0=s;

 while(n<CASH_SIZE)
 {
	 //if (!patterns[n]) break; //Больше паттернов нет
 L_RESTARTPAT:
	 p=s; //Текущая позиция в строке
	 s++;
	 pval=NULL; //Число пока не нашли
	 pat=patterns[n]; //Начинаем с начала паттерна
	 while((c=*pat++,cc=*p)) //Пока не дошли до конца строки, в которой ищем
	 {
		 if (c=='#') //А не число ли сейчас требуется
		 {
			 //Проверяем на число
			 if (((cc<'0')||(cc>'9'))&&(cc!='-')) goto L_RESTARTPAT; //Первая цифра не число - рестарт поиска
			 pval=p; //Возможно число будет здесь
			 do
			 {
				 cc=*++p; //Следующий символ
			 }
			 while(((cc>='0')&&(cc<='9'))||(cc=='-')||(cc=='.')||(cc==',')); //Пока это еще число
		 }
		 else
		 {
			 //Обычное сравнение
			 if (!c) break; //Дошли до конца паттерна, значит все совпало, выходим
			 //Сравниваем символы
			 if (cc!=c) goto L_RESTARTPAT; //Нет совпадения, рестартуем
			 p++;
		 }
	 }
	 if (c) { //Не дошли до конца паттерна
		 s=s0;
		 n++;
		 continue;
	 }
	 if (!pval) break; //Не нашли число
	 s=p; //Поиск будем продолжать отсюда
	 i=strtol(pval,&ep,10)*100;
	 pval=ep;
	 if ((*pval=='.')||(*pval==','))
	 {
		 pval++;

		 int j=strtol(pval, &ep, 10);//надо домножить число после запятой...
		 int k=ep-pval;//... на 10^(2-k)
		 if (k==1)
			 j*=10;
		 else
			 while (k>2) {//делим j на 10^(k-2)
				 j/=10; k--;
			 }//ибо, бывают ещё балансы в у.е. с 3-4 знаками после запятой. Сам видел у МТСа. Lost, 15.01.2008

		 if (i>=0) i+=j; //на случай если баланс отрицательный. Раньше об этом и не думали, вот так то. Mahmuds, 13.05.2011
		 else i-=j;

		 pval=ep;
	 }
	 if (i>(CurrentCASH[n]+(MaxCASH[n]/100))) //Если новый больше чем текущий +1 процент от максимального
	 {
		 MaxCASH[n]=i;
	 }
	 CurrentCASH[n]=i;
	 n++;
 }
 //while(n<CASH_SIZE);  Может быть ни одного пока идет поиск сети
 SaveCash();
 return (n);
}

wchar_t *(*USSD_GetText)(void *msg)=NULL;
wchar_t *USSD_GetText_2020(void *msg) {return (msg?((wchar_t **)msg)[0x2C/4]:NULL);}
wchar_t *USSD_GetText_Other(void *msg) {return (msg?((wchar_t **)msg)[0x24/4]:NULL);}
wchar_t *USSD_GetText_dummy(void *msg)
{
	return NULL;
}

void SetGetTextFunc()
{FUNCTION
	switch (GetChipID()&CHIPID_MASK)
	{
	case CHIPID_DB2020:
	case CHIPID_DB3150:
	case CHIPID_DB3200:
	case CHIPID_DB3210:
	case CHIPID_DB3350:
		USSD_GetText=USSD_GetText_2020;
		break;
	case CHIPID_DB2000:
	case CHIPID_DB2010:
		USSD_GetText=USSD_GetText_Other;
		break;
	default:
		USSD_GetText=USSD_GetText_dummy;
	}
}

static void WriteLog(char *text, int len)
{
	int f;
	if ((f=_fopen(cashpath, L"LiveBalance_cash.txt", FSX_O_CREAT|FSX_O_APPEND, FSX_S_IREAD|FSX_S_IWRITE, 0)) != -1) {
		DATETIME dt;
		REQUEST_DATEANDTIME_GET(SYNC, &dt);

		char s[40];
		int tmp=sprintf(s, "%02d.%02d.%04d %02d:%02d ", dt.date.day, dt.date.mon, dt.date.year, dt.time.hour, dt.time.min);
		fwrite(f, s, tmp);
		fwrite(f, text, len);
		tmp=sprintf(s, " %d %d %d %d\r\n", CurrentCASH[0], CurrentCASH[1], CurrentCASH[2], CurrentCASH[3]);
		fwrite(f, s, tmp);
		fclose(f);
	}
}

static int OnReceiveUssd(void * data, BOOK *book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event)
{FUNCTION
	char *s1, *s2;
 wchar_t *ws;
 int len;
 int c;
 int i=0;
 if (ussdreq_sended)
 {
	 ws=USSD_GetText(data);
	 EndUSSDtimer();
	 if (ws)
	 {
		 if ((len=wstrlen(ws)))
		 {
			 s1=s2=new char[len+3];
			 while(c=*ws++)
			 {
				 c=char16to8(c);
				 if (c<32) c='?';
				 *s2++=c;
			 }
			 *s2=0;
			 i=FindCash(s1);
			 if (ENA_CASHTRACELOG)
				 WriteLog(s1, len);
			 delete s1;
			 if (i)  // Если что то нашли
			 {
				 DISP_OBJ *StBy_DispObj=GUIObject_GetDispObject(SBY_GetStatusIndication(Find_StandbyBook()));
				 DispObject_InvalidateRect(StBy_DispObj,0);
			 }
		 }
		 FreeBook(FindBook(isUSSDBook));
	 }
	 StartHoursTimer();
 }
 return(i?BLOCK_EVENT_GLOBALLY:1);
}

static void onMyBookClose(BOOK * book)
{FUNCTION
	ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,0,PAGE_HOOK_REMOVE);
 DISP_OBJ *StBy_DispObj = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
 DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication),SIonRedraw);
 EndUSSDtimer();
 Timer_Kill(&hours_tmr);
 SaveCash();
 SUBPROC(elf_exit);
}

int isLiveBalanceBook(BOOK * struc)
{FUNCTION
	return(struc->onClose==onMyBookClose);
}

int SB_ELF_Killed(void *mess ,BOOK* book)
{
	// если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
	REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
	// его ли метод мы используем в качестве oldRedraw?
	if (sbm->SB_NewOnRedraw==SIonRedraw)
	{
		REDRAW_RELEASE_MESSAGE *res=new REDRAW_RELEASE_MESSAGE;

		// скопируем существующие методы в нашу мессагу
		memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));

		// если он был убит, то заменяем свой oldRedraw на его..
		if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) SIonRedraw=sbm->SB_OldOnRedraw;

		// ставим свой метод наверх
		DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication), My_OnRedraw);

		// и шлём мессагу снова, чтоб следующие эльфы сделали тоже самое
		res->SB_OldOnRedraw=EMPTY_REDRAW_METHOD;
		res->SB_NewOnRedraw=My_OnRedraw;
		UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,res,(void (*)(void*))mfree_adr());
		return BLOCK_EVENT_GLOBALLY;
	}
	return 0;
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
	SBY_REDRAW_RELEASE_EVENT,                           SB_ELF_Killed,
	ELF_RECONFIG_EVENT,                                 ReconfigElf,
	ELF_TERMINATE_EVENT,                                TerminateElf,
	//  ELF_SHOW_INFO_EVENT,                                ShowAuthorInfo,
	UI_MESSAGING_SMS_SENT_EVENT_TAG,                    onNeedCashReq,
	UI_MESSAGING_MMS_SENT_EVENT_TAG,                    onNeedCashReq,
	UI_CONNECTIONMANAGER_SESSION_TERMINATED_EVENT_TAG,  onNeedCashReq,
	ON_CALLMANAGER_EVENT_TAG,                           OnCallManager,
	UI_CONTROLLED_SHUTDOWN_REQUESTED_EVENT_TAG,         OnTurnOff,
	NIL_EVENT_TAG,                                      NULL
};

const PAGE_DESC bk_base = {"LiveBalance_Base_Page",0,bk_msglst_base};

int main()
{FUNCTION
	MyBOOK * myBook=new MyBOOK;
	memset(myBook,0,sizeof(MyBOOK));
	if (!CreateBook((BOOK*)myBook,onMyBookClose,&bk_base,"LiveBalance",-1,0))
	{
		delete myBook;
		SUBPROC(elf_exit);
		return (0);
	}
	SetGetTextFunc();
	InitConfig();
	InitCache();
	ModifyUIPageHook(USSD_RECIEVED_EVENT,OnReceiveUssd,0,PAGE_HOOK_ADD_BEFORE);

	StatusIndication=GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
	SIonRedraw=DispObject_GetOnRedraw(StatusIndication);
	DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication), My_OnRedraw);

	StartHoursTimer();
	return 0;
}
