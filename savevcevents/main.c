#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

#pragma swi_number=0x8159
__swi __arm  int (*strcmp_addr())(const char*, const char*);


void elf_exit(void)
{
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

typedef char* (*GETEVENTNAME)(int event);

struct APPDESCDATA
{
	void* somefn;
	void* page;
	char* appname;
	void* getpageevents;
	GETEVENTNAME getevname;
};

struct APPDESC
{
	int num;
	APPDESCDATA* data;
};

enum
{
	FIRST,
	NOTFIRST,
	LAST
};
struct KEYSTR2STR
{
	char* keystring;
	char* constname;
	int   position;
	int   count;
}keystr2str[]=
{
	NULL,"KEY_ENTER",FIRST,0,//[0]

	"#","KEY_DIEZ",FIRST,0,
	"*","KEY_STAR",FIRST,0,
	"0","KEY_DIGITAL_0",FIRST,0,
	"Back","KEY_ESC",FIRST,0,

	// ,"Camera",""
	"Camera capture","KEY_CAMERA_SHOT",FIRST,0,
	"Camera focus","KEY_CAMERA_FOCUS",FIRST,0,

	"Clear","KEY_DEL",FIRST,0,
	"Flip closed","KEY_FLIP_CLOSE",FIRST,0,
	"Flip opened","KEY_FLIP_OPEN",FIRST,0,
	"Jack knife close","KEY_JACKKNIFE_CLOSE",FIRST,0,
	"Jack knife open","KEY_JACKKNIFE_OPEN",FIRST,0,
	"Left action","KEY_LEFT_SOFT",FIRST,0,
	"Lens cover close","KEY_LENS_COVER_CLOSE",FIRST,0,
	"Lens cover open","KEY_LENS_COVER_OPEN",FIRST,0,
	"On/Off","KEY_POWER",FIRST,0,
	"PHF end off","KEY_PHF_ENDOF",FIRST,0,
	"PHF send","KEY_PHF1",FIRST,0,
	"Portable handsfree","KEY_PHF",FIRST,0,
	"Portable{LF}handsfree","KEY_PHF",FIRST,0,
	"Right action","KEY_RIGHT_SOFT",FIRST,0,
	"SideDown","KEY_VOL_DOWN",FIRST,0,
	"SideUp","KEY_VOL_UP",FIRST,0,
	"{0x2190}","KEY_LEFT",FIRST,0,
	"{0x2192}","KEY_RIGHT",FIRST,0,
	"Music player","KEY_MEDIAPLAYER",FIRST,0,
	"WALKMAN","KEY_MEDIAPLAYER",FIRST,0,//w800
	"Go Music","KEY_PLAY",FIRST,0,//w800
	"Play/stop button","KEY_PLAY",FIRST,0,

	"Game A","KEY_GAME_A",LAST,0,
	"Game B","KEY_GAME_B",LAST,0,
	"Operator","KEY_OPERATOR",LAST,0,
	"Menu","KEY_TASK",LAST,0,

	"{0x2191}","KEY_UP",FIRST,0,
	"{0x2191}","KEY_SLIDER_OPEN",LAST,0,
	"{0x2193}","KEY_DOWN",FIRST,0,
	"{0x2193}","KEY_SLIDER_CLOSE",LAST,0,
};

int findkeyname(char* str)
{
	for(int i=0;i<MAXELEMS(keystr2str);i++)
	{
		if(!strcmp(str,keystr2str[i].keystring))
		{
			if( (keystr2str[i].position==FIRST && keystr2str[i].count==0)
			   ||(keystr2str[i].position==NOTFIRST && keystr2str[i].count!=0)
				   ||(keystr2str[i].position==LAST)
					   )
			{
				keystr2str[i].count++;
				return i;
			}
		}
	}
	return -1;
}

char* getsecondevent(char* e)
{
	static char tmp[64];
	strcpy(tmp,e);
	char* pos=strstr(tmp,"_EVENT");
	if(pos)
		strcpy(pos,"_SECOND_EVENT");
	return tmp;
}

struct ABNAMES
{
	char* constname;
	char* text;
} abnames[]=
{
	"AB_ITEM_FIELD_NAME", "Name:",
	"AB_ITEM_FIELD_NEW_ITEM", "New number:",
	"AB_ITEM_FIELD_N_MOBILE", "Mobile",
	"AB_ITEM_FIELD_N_MOBILE", "Mobile:",
	"AB_ITEM_FIELD_N_HOME", "Home",
	"AB_ITEM_FIELD_N_HOME", "Home:",
	"AB_ITEM_FIELD_N_WORK", "Work",
	"AB_ITEM_FIELD_N_FAX", "Fax",
	"AB_ITEM_FIELD_N_FAX", "Fax:",
	"AB_ITEM_FIELD_N_OTHER", "Other",
	"AB_ITEM_FIELD_N_OTHER", "Other:",
	"AB_ITEM_FIELD_EMAIL1", "Email:",
	"AB_ITEM_FIELD_EMAIL2", "Email2:",
	"AB_ITEM_FIELD_EMAIL3", "Email3:",
	"AB_ITEM_FIELD_USERNAME", "My friends ID:",
	"AB_ITEM_FIELD_WEB", "Web address:",
	"AB_ITEM_FIELD_IMAGE", "Picture:",
	"AB_ITEM_FIELD_RINGTONE", "Ringtone:",
	"AB_ITEM_FIELD_VOICE_CMD", "Voice commands:",
	//  "AB_ITEM_FIELD_VOICE_CMD", "Voice commands:",
	//  "AB_ITEM_FIELD_VOICE_CMD", "Voice commands:",
	//  "AB_ITEM_FIELD_VOICE_CMD", "Voice commands:",
	//  "AB_ITEM_FIELD_VOICE_CMD", "Voice commands:",
	"AB_ITEM_FIELD_TITLE", "Title:",
	"AB_ITEM_FIELD_COMPANY", "Company:",
	"AB_ITEM_FIELD_STREET", "Street:",
	"AB_ITEM_FIELD_CITY", "City:",
	"AB_ITEM_FIELD_REGION", "State:",
	"AB_ITEM_FIELD_POST_INDEX", "Zip code:",
	"AB_ITEM_FIELD_COUNTRY", "Country:",
	"AB_ITEM_FIELD_INFO", "Info:",
	"AB_ITEM_FIELD_DR", "Birthday:"
};

struct ABICONS
{
	wchar_t* name;
	int   id;
}abicons[]=
{
	L"HPB_LONELY_MAN_ICN",0,
	L"HPB_MOBILE_ICN",0,
	L"HPB_HOME_ICN",0,
	L"HPB_WORK_ICN",0,
	L"HPB_FAX_ICN",0,
	L"HPB_OTHER_ICN",0,
	L"HPB_EMAIL_ICN",0,
	L"HPB_EMAIL_ICN",0,
	L"HPB_EMAIL_ICN",0,
	L"WV_PRESENCE_HPB_ICN",0,
	L"HPB_URL_ICN",0,
	L"PIC_IN_LIST_ICN",0,
	L"VOICETAG_ICN",0,
	L"VOICETAG_ICN",0,
	L"VOICETAG_ICN",0,
	L"VOICETAG_ICN",0,
	L"VOICETAG_ICN",0,
	L"HPB_EMAIL_ICN",0
};

struct ABTEXTIDS
{
	wchar_t* name;
	int   id;
}abstrids[]=
{
	L"HPB_NAMEP_TXT",0,
	L"HPB_NEW_NUMBERP_TXT",0,
	L"HPB_MOBILE_NUMBER_TXT",0,
	L"HPB_HOME_NUMBER_TXT",0,
	L"HPB_WORK_NUMBER_TXT",0,
	L"HPB_FAX_NUMBER_TXT",0,
	L"HPB_OTHER_NUMBER_TXT",0,
	L"HPB_EMAIL_ADDRESSP_TXT",0,
	L"HPB_EMAIL2_ADDRESSP_TXT",0,
	L"HPB_EMAIL3_ADDRESSP_TXT",0,
	L"WV_USER_ID_HPBP_TXT",0,
	L"HPB_WAPP_TXT",0,
	L"HPB_PICTUREP_TXT",0,
	L"HPB_PERSONAL_RINGP_TXT",0,
	L"HPB_VOICE_COMMANDSP_TXT",0,
	L"HPB_VOICE_COMMANDSP_TXT",0,
	L"HPB_VOICE_COMMANDSP_TXT",0,
	L"HPB_VOICE_COMMANDSP_TXT",0,
	L"HPB_VOICE_COMMANDSP_TXT",0,
	L"HPB_TITLEP_TXT",0,
	L"HPB_COMPANYP_TXT",0,
	L"HPB_STREETP_TXT",0,
	L"HPB_CITYP_TXT",0,
	L"HPB_STATEP_TXT",0,
	L"HPB_ZIPP_TXT",0,
	L"HPB_COUNTRYP_TXT",0,
	L"HPB_INFORMATIONP_TXT",0,
	L"HPB_BIRTHDAYP_TXT",0
};


//тупая замена
struct PATCHNAMES
{
	char* old;
	char* _new;
}patchnames[]=
{
	"UI_ALIEN_BATTERY_CHARGER_CONNECTED_EVENT", "ALIEN_BATTERY_CHARGER_CONNECTED_EVENT",
	"UI_ALIEN_BATTERY_CHARGER_DISCONNECTED_EVENT", "ALIEN_BATTERY_CHARGER_DISCONNECTED_EVENT",
	"UI_BASIC_CLOCK_ALARMCLOCK_STATUS_EVENT", "CLOCK_ALARMCLOCK_STATUS_EVENT",
	"UI_BASIC_CLOCK_DATEANDTIME_IND_EVENT", "CLOCK_DATEANDTIME_IND_EVENT",
	"UI_BASIC_DISPATCH_ALARMRECURSTATUS_IND_EVENT", "DISPATCH_ALARMRECURSTATUS_IND_EVENT",
	"UI_BASIC_PROFILE_ACTIVATED_EVENT", "PROFILE_ACTIVATED_EVENT",
	"UI_BASIC_SYS_CTL_ON_SYSTEM_MODE_CHANGED_EVENT", "SYS_CTL_ON_SYSTEM_MODE_CHANGED_EVENT",
	"UI_BATTERY_BATTERY_CONNECTED_EVENT", "BATTERY_CONNECTED_EVENT",
	"UI_BATTERY_BATTERY_DISCONNECTED_EVENT","BATTERY_DISCONNECTED_EVENT",
	"UI_BATTERY_CAPACITY_CHANGED_EVENT", "BATTERY_CAPACITY_CHANGED_EVENT",
	"UI_BATTERY_CHARGER_CONNECTED_EVENT", "BATTERY_CHARGER_CONNECTED_EVENT",
	"UI_BATTERY_CHARGER_DISCONNECTED_EVENT", "BATTERY_CHARGER_DISCONNECTED_EVENT",
	"UI_BATTERY_CHARGING_ALERT_EVENT", "BATTERY_CHARGING_ALERT_EVENT",
	"UI_BATTERY_CHARGING_STATE_EVENT", "BATTERY_CHARGING_STATE_EVENT",
	"UI_BATTERY_SYSTEM_VOLTAGE_STATUS_EVENT", "BATTERY_SYSTEM_VOLTAGE_STATUS_EVENT",
	"UI_BATTERY_UPDATE_BATTERY_ICON_EVENT", "UPDATE_BATTERY_ICON_EVENT",
	"MAIN_WINDOW_MASTER_GOT_FOCUS_EVENT", "MASTER_SESSION_WINDOW_GOT_FOCUS_EVENT",
	"MAIN_WINDOW_MASTER_LOST_FOCUS_EVENT", "MASTER_SESSION_WINDOW_LOST_FOCUS_EVENT",
	"FM_RADIO_AUDIOCONTROL_CHANNEL_OPENED_EVENT", "UI_FMRADIO_CREATED_EVENT",
	"VOLUMECONTROL_DECREASE_REPEAT_SECOND_EVENT", "KEYLOCK_BYPASS_VOLUMEDOWN_SHORT_EVENT"
};

char* patchevent(char* evtname)
{
	if(evtname)
		for(int i=0; i<MAXELEMS(patchnames); i++)
			if(!strcmp(patchnames[i].old, evtname))
				return patchnames[i]._new;
	return evtname;
}


int main (void)
{
	char* lng=lang_get_name(get_system_langID());
	if(strcmp(lng,"en"))
	{
		MessageBox(STR("error"),STR("please change phone language to english"),NOIMAGE,1,0,0);
	}else
	{

		char outs[512];
		int f=_fopen( GetDir(DIR_OTHER|MEM_INTERNAL), L"vcevts.txt", FSX_O_CREAT|FSX_O_WRONLY, FSX_S_IWRITE|FSX_S_IREAD, 0);


		LIST * evtsused=List_Create();

		//main events
		for(int i=0;i<=1000;i++)
		{
			char* x=UIEventName(i);
			x = patchevent(x);
			if(x)
			{
				if(LIST_FIND(evtsused,x,strcmp_addr())!=LIST_ERROR)
				{
					sprintf(outs,"#define %s 0x%X\r\n",getsecondevent(x),i);
				}
				else
					sprintf(outs,"#define %s 0x%X\r\n",x,i);
				fwrite(f,outs,strlen(outs));
				List_InsertLast(evtsused,x);
			}
		}
		sprintf(outs,"\r\n\r\n"); fwrite(f,outs,strlen(outs));

		//app events
		APPDESC* adt=(APPDESC*)get_APP_DESC_TABLE();
		while(adt->data)
		{
			if(adt->data->getevname)
			{
				for(int i=adt[0].num*300+1001;i<adt[1].num*300+1001;i++)
				{
					char* evtname=adt->data->getevname(i);
					evtname = patchevent(evtname);
					if(evtname)
					{
						if(LIST_FIND(evtsused,evtname,strcmp_addr())!=LIST_ERROR)
						{
							sprintf(outs,"#define %s 0x%X\r\n",getsecondevent(evtname),i);
						}
						else
							sprintf(outs,"#define %s 0x%X\r\n",evtname,i);
						fwrite(f,outs,strlen(outs));
						List_InsertLast(evtsused,evtname);
					}
				}
			}
			adt++;
		}

		List_Destroy(evtsused);

		sprintf(outs,"\r\n\r\n"); fwrite(f,outs,strlen(outs));

		//fonts
		FONT_DESC *fd = GetFontDesc();
		int fc=*GetFontCount();
		for(int i=0;i<fc;i++)
		{
			char fontname[32];
			strcpy(fontname,"FONT_");
			wstr2strn (&fontname[5], (wchar_t const*)&fd[i].name, sizeof(fd[i].name));

			if(strstr("FONT_ICON_16BIT_V2",fontname))strcpy(fontname,"FONT_ICON_16BIT_V2");
			if(strstr("FONT_ICON_2BIT_V2_2NDLCD",fontname))strcpy(fontname,"FONT_ICON_2BIT_V2_2NDLCD");

			//похожие шрифты
			if(strstr("FONT_E_26R",fontname))strcpy(fontname,"FONT_E_25R");
			if(strstr("FONT_E_76R",fontname))strcpy(fontname,"FONT_E_75R");

			sprintf(outs,"#define %s 0x%X\r\n",fontname,fd[i].id); fwrite(f,outs,strlen(outs));
		}

		sprintf(outs,"\r\n\r\n"); fwrite(f,outs,strlen(outs));

		//2 icons
		int nicon=0;
		if(iconidname2id( L"CALE_TASK_PHONECALL_ICN", -1, &nicon))
		{
			sprintf(outs,"#define ICON_OLD_GREEN_PHONE 0x%X\r\n",nicon); fwrite(f,outs,strlen(outs));
		}
		if(iconidname2id( L"MESSAGING_MENU_NEW_MESSAGE_ICN", -1, &nicon))
		{
			sprintf(outs,"#define ICON_WRITE_NEW 0x%X\r\n",nicon); fwrite(f,outs,strlen(outs));
		}

		sprintf(outs,"\r\n\r\n"); fwrite(f,outs,strlen(outs));

		//keys
		int njoykey=0;
		char cjoykey[10];
		iconidname2id( L"JOYSTICK_SYMB_ICN", -1, &njoykey);
		sprintf(cjoykey,"{0x%04X}",njoykey);
		keystr2str[0].keystring=cjoykey;

		for(int i=0;i<255;i++)
		{
			TEXTID kn=KeyCode2Name(i);
			char keyname[32];
			int constname;
			TextID_GetString(kn,keyname,sizeof(keyname)-1);
			keyname[sizeof(keyname)-1]=0;
			for(int tmpi=strlen(keyname)-1;(tmpi>=0)&& keyname[tmpi]==' ';tmpi--)
				keyname[tmpi]=0;

			constname=findkeyname(keyname);
			if(constname!=-1)
			{
				sprintf(outs,"#define %s 0x%X\r\n",keystr2str[constname].constname,i);
				fwrite(f,outs,strlen(outs));
			}else
			{
				if(keyname[0] && keyname[0]!='?')
				{
					sprintf(outs,"//#define %s 0x%X\r\n",keyname,i); fwrite(f,outs,strlen(outs));
				}
			}
			if(kn!=EMPTY_TEXTID)TextID_Destroy(kn);
		}

		sprintf(outs,"\r\n\r\n"); fwrite(f,outs,strlen(outs));




		///////////////////////////////////////////////////////////////////////////

		//addressbook items
		bool found=false;
		unsigned short* abdescs = (unsigned short*)get_AB_ITEMS_DESC();
		int AB_ITEM_DESC_SIZE;
		for(AB_ITEM_DESC_SIZE=0; AB_ITEM_DESC_SIZE<0x30; AB_ITEM_DESC_SIZE+=2)
		{
			int i;
			for(i=0;i<5;i++)
				if(abdescs[AB_ITEM_DESC_SIZE/2*i]!=i)
					break;
			if(i==5)
			{
				found=true;
				break;
			}
		}
		if(found)
		{
			sprintf(outs,"#define AB_ITEM_DESC_SIZE 0x%X\r\n",AB_ITEM_DESC_SIZE); fwrite(f,outs,strlen(outs));

			int AB_ITEM_FPI;//items
			for(AB_ITEM_FPI=0; AB_ITEM_FPI<255; AB_ITEM_FPI++)
			{
				if(abdescs[AB_ITEM_DESC_SIZE/2*AB_ITEM_FPI]!=AB_ITEM_FPI)break;
			}
			sprintf(outs,"#define AB_ITEM_FPI 0x%X\r\n",AB_ITEM_FPI); fwrite(f,outs,strlen(outs));

			sprintf(outs,"#define AB_ITEM_DESC_NUM_DISPLACE 0x0\r\n"); fwrite(f,outs,strlen(outs));

			int AB_ITEM_DESC_ID_DISPLACE;
			for(AB_ITEM_DESC_ID_DISPLACE=2; AB_ITEM_DESC_ID_DISPLACE<AB_ITEM_DESC_SIZE; AB_ITEM_DESC_ID_DISPLACE+=2)
			{
				bool found = true;
				for(int i=0; i<AB_ITEM_FPI; i++)
				{
					for(int j=0; j<AB_ITEM_FPI; j++)
					{
						if(i!=j)
							if(abdescs[AB_ITEM_DESC_SIZE/2*i+AB_ITEM_DESC_ID_DISPLACE/2]==
							   abdescs[AB_ITEM_DESC_SIZE/2*j+AB_ITEM_DESC_ID_DISPLACE/2])
							{
								found = false;
								break;
							}
					}
					if(!found)break;
				}
				if(found)
				{
					sprintf(outs,"#define AB_ITEM_DESC_ID_DISPLACE 0x%X\r\n",AB_ITEM_DESC_ID_DISPLACE); fwrite(f,outs,strlen(outs));
					break;
				}
			}

			for(int i=0;i<sizeof(abicons)/sizeof(abicons[0]);i++)
			{
				iconidname2id(abicons[i].name, -1, &abicons[i].id);
				if(abicons[i].id == NOIMAGE)abicons[i].id = 0xFFFFFFFF;
			}

			int AB_ITEM_DESC_ICON_DISPLACE;
			for(AB_ITEM_DESC_ICON_DISPLACE=2; AB_ITEM_DESC_ICON_DISPLACE<AB_ITEM_DESC_SIZE; AB_ITEM_DESC_ICON_DISPLACE+=2)
			{
				bool found=false;

				for(int i=0; i<AB_ITEM_FPI; i++)
				{
					for(int j=0; j<sizeof(abicons)/sizeof(abicons[0]); j++)
					{
						if(abdescs[i*AB_ITEM_DESC_SIZE/2+AB_ITEM_DESC_ICON_DISPLACE/2]==(u16)abicons[j].id)
						{
							sprintf(outs,"#define AB_ITEM_DESC_ICON_DISPLACE 0x%X\r\n",AB_ITEM_DESC_ICON_DISPLACE); fwrite(f,outs,strlen(outs));
							found=true;
							break;
						}
					}
					if(found)break;
				}
				if(found)break;
			}

			for(int i=0;i<sizeof(abstrids)/sizeof(abstrids[0]);i++)
			{
				textidname2id(abstrids[i].name, -1, &abstrids[i].id);
				if(abstrids[i].id == 0xFFFF)abstrids[i].id = 0xFFFFFFFF;
			}

			int AB_ITEM_DESC_TEXTID_DISPLACE;
			for(AB_ITEM_DESC_TEXTID_DISPLACE=2; AB_ITEM_DESC_TEXTID_DISPLACE<AB_ITEM_DESC_SIZE; AB_ITEM_DESC_TEXTID_DISPLACE+=2)
			{
				bool found=false;

				for(int i=0; i<AB_ITEM_FPI; i++)
				{
					for(int j=0; j<sizeof(abstrids)/sizeof(abstrids[0]); j++)
					{
						if(abdescs[i*AB_ITEM_DESC_SIZE/2+AB_ITEM_DESC_TEXTID_DISPLACE/2]==(u16)abstrids[j].id)
						{
							found=true;
							break;
						}
					}
					if(found)break;
				}
				if(found)
				{
					int voicecmd=1;
					for(int i=0;i<AB_ITEM_FPI;i++)
					{
						char tmp[64];
						TextID_GetString( abdescs[i*AB_ITEM_DESC_SIZE/2+AB_ITEM_DESC_TEXTID_DISPLACE/2], tmp, sizeof(tmp) );

						BOOL found2=false;
						for(int j=0;j<sizeof(abnames)/sizeof(abnames[0]);j++)
						{
							if(!strcmp(tmp, abnames[j].text))
							{
								if(!strcmp(tmp,"Voice commands:"))
								{
									sprintf(outs,"#define %s%d 0x%X\r\n",abnames[j].constname, voicecmd++, abdescs[i*AB_ITEM_DESC_SIZE/2/*+AB_ITEM_DESC_NUM_DISPLACE/2*/]); fwrite(f,outs,strlen(outs));
								}else
								{
									sprintf(outs,"#define %s 0x%X\r\n",abnames[j].constname, abdescs[i*AB_ITEM_DESC_SIZE/2/*+AB_ITEM_DESC_NUM_DISPLACE/2*/]); fwrite(f,outs,strlen(outs));
								}
								found2=true;
								break;
							}
						}
						if(!found2)
						{
							sprintf(outs,"//#define AB_UNKNOWN \"%s\" 0x%X\r\n",tmp, abdescs[i*AB_ITEM_DESC_SIZE/2/*+AB_ITEM_DESC_NUM_DISPLACE/2*/]); fwrite(f,outs,strlen(outs));
						}
					}
					break;
				}
			}
		}
		//end ab_items
		///////////////////////////////////////////////////////////////////////////


		fclose(f);

	}

	elf_exit();
	return(0);
}
