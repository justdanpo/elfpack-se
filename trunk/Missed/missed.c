//#define DEBUG

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "string.h"
#include "util.h"

#define ELFNAME "missed events"
#define LELFNAME L"missed events"
#define LELFVERSION L" v0.5\nby den_po"
#define LINIFILENAME _T("missed.ini")

static char myappname[]=ELFNAME;

bool disabled=false;
int cfg_period=30;
wchar_t cfg_filepath[256];
wchar_t cfg_filename[256];

u16 timer;

int skipevents;
int checkevents;

void onTimer(u16 timerID, LPARAM lparam)
{
	if(!disabled)
	{
		char* me=MissedEvents();
		char mestatus=*me;//some FWs use char :(
		if( mestatus & checkevents )
		{
			if(!GetSilent())
			{
				char vol;
				GetAudioSettings(2,&vol);
				PlayFileV(cfg_filepath,cfg_filename,vol);
			}
			if(GetVibrator(0,0))
			{
				PAudioControl pAC=*GetAudioControlPtr();
				AudioControl_Vibrate(pAC,450,50,1500);
			}
		}
#ifdef DEBUG
		if( mestatus & ~(skipevents|checkevents))
		{
			wchar_t temp[256];
			snwprintf(temp, MAXELEMS(temp), _T("unknown event status\n%x: %x"), me, mestatus &~(skipevents|checkevents));
			MessageBox(EMPTY_TEXTID, TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)), NOIMAGE, 1, 0, 0);
			skipevents|=mestatus;
		}
#endif
	}
	Timer_ReSet(&timer,cfg_period*1000,onTimer,lparam);
}

int getcfg()
{
	int fcfg;
	char* pcfg=NULL;

	if( (fcfg=_fopen(GetDir(DIR_INI), LINIFILENAME , FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0))>=0)
	{
		FSTAT _fstat;
		if (fstat(GetDir(DIR_INI), LINIFILENAME ,&_fstat)==0)
		{
			pcfg=(char*)malloc(_fstat.fsize+1);
			fread(fcfg,pcfg,_fstat.fsize);

			if(char* pfilepath=manifest_GetParam(pcfg,"[FILE]",0))
			{
				extractdir(cfg_filepath,cfg_filename,pfilepath);
				mfree(pfilepath);
			}
			if(char* pperiod=manifest_GetParam(pcfg,"[PERIOD]",0))
			{
				cfg_period=str2int(pperiod);
				mfree(pperiod);
			}
			mfree(pcfg);
		}
		fclose(fcfg);
		return 1;
	}
	return 0;
}


//-------------------------------------------------------------------
void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

//////////////////////////////////////////////////////////////////////////////////////////

void bookOnDestroy(BOOK * book)
{
	Timer_Kill(&timer);
	MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME L" has been stopped",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,5000,0);
	SUBPROC(elf_exit);
}

int onExit(void* r0,BOOK* b)
{
	FreeBook(b);
	return 1;
}

int onAbout(void* mess,BOOK* b)
{
	MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *(BOOK**)mess);
	return 1;
}

enum
{
	CALLMANAGER_IDLE            =0,
	CALLMANAGER_CALL_CONNECTED  =1,
	CALLMANAGER_CALL_SETUP      =2,
	CALLMANAGER_CALL_ALERT      =4,
	CALLMANAGER_CALL_WAITING    =5,
	CALLMANAGER_CALL_TERMINATED =6
};

int OnCallManagerEvent(void* r0,BOOK* b)
{
	switch(((CALLMANAGER_EVENT_DATA*)r0)->CallState)
	{
	case CALLMANAGER_CALL_SETUP:
	case CALLMANAGER_CALL_ALERT:
		disabled=true;
		break;
	case CALLMANAGER_CALL_TERMINATED:
		Timer_ReSet(&timer,cfg_period*1000,onTimer, 0);
		disabled=false;
		break;
	}
	return 1;
}

const PAGE_MSG evtlist[] @ "DYN_PAGE"=
{
	ELF_TERMINATE_EVENT     ,onExit,
	ELF_SHOW_INFO_EVENT     ,onAbout,
	ON_CALLMANAGER_EVENT_TAG,OnCallManagerEvent,
	NIL_EVENT_TAG           ,NULL
};

const PAGE_DESC defaultpage = {"defaultpage",0,evtlist};

int isOurBook(BOOK * book)
{
	if(!strcmp(book->xbook->name,myappname)) return true;
	return false;
}

int main(wchar_t* filename)
{
	BOOK* alreadyrunned=FindBook(isOurBook);
	if(alreadyrunned)
	{
		FreeBook(alreadyrunned);
		SUBPROC(elf_exit);
	}else
	{
		wchar_t temp[256];
		//check cfg params------
		char* badarg=NULL;
		FSTAT _fstat;
		if(!getcfg())
			badarg="INI";
		else
			if(fstat(cfg_filepath,cfg_filename,&_fstat)!=0)
				badarg="[FILE]";
			else
				if(cfg_period<3)badarg="[PERIOD]";
		if(badarg)
		{
			snwprintf(temp, MAXELEMS(temp), _T("bad '%s'\nconfig:\npath:%ls\nname:%ls\ntime:%d"),badarg
				,cfg_filepath,cfg_filename,cfg_period);
			MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,0,0);
			SUBPROC(elf_exit);
			return 0;
		}
		//---------------------
		switch(GetChipID()&CHIPID_MASK)
		{
                case CHIPID_DB2000:
                case CHIPID_DB2010:
			skipevents=0;
			checkevents=1|2|4|8;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
			break;
                case CHIPID_DB2020:
                case CHIPID_DB3150:
                case CHIPID_DB3200:
                case CHIPID_DB3210:
                case CHIPID_DB3350:
			skipevents=1|2;//KEYLOCKED|NOSOUND
			checkevents=4|8|0x10|0x20;//MISSED_CALL|MISSED_SMS|MISSED_EMAIL|MISSED_MMS
		}

		BOOK *myBook=(BOOK*)malloc(sizeof(BOOK));
		memset(myBook,0,sizeof(BOOK));
		CreateBook(myBook,bookOnDestroy,&defaultpage,myappname,-1,0);

		timer=Timer_Set(cfg_period*1000, onTimer, 0);

		if(!wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)))
			MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME L" has been started",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,5000,0);
	}

	return 0;
}
