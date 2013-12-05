#define C902_R3EG004AS
#define A2
#define DB3150

#include "..\..\include\Types.h"
#include "classes.h"

#define RedLED_ID 5
#define GetSignalQuality_Send_SignalID 0x12C00080
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x4BBAA3E4
#define MainInput_CurPos_Offset 0x3

void (*DisplayGC_AddRef)(GC*,void *)=(void(*)(GC*,void *))0x109B1315;
int (*Illumination_LedID_SetLevel)(int LED_ID,int level)=(int(*)(int LED_ID,int level))0x10BAE5C5;
int (*Illumination_LedID_Off)(int LED_ID)=(int(*)(int LED_ID))0x10BAE7C5;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int* sync,void * buf)=(int(*)(const int* sync,void * buf))0x10A2F641;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM * dest,AB_STR_ITEM * source)=(void(*)(AB_STR_ITEM * dest,AB_STR_ITEM * source))0x10B4C52D;
int (*GetImageHeight_int)(wchar_t ImageID)=(int(*)(wchar_t ImageID))0x109AE221;
void (*Display_SetBrightnessLevel)(int level)=(void(*)(int level))0x10DA5331;
TEXTID (*TextID_CreateIntegerID_int)(int num,char * filename,int line_num)=(TEXTID(*)(int num,char * filename,int line_num))0x10F45F6D;
TEXTID (*TextID_Create_int)(const void* wstr,int flag,int len,char * filename,int line_num)=(TEXTID(*)(const void* wstr,int flag,int len,char * filename,int line_num))0x10F46141;
int (*TextID_GetWString_int)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num)=(int(*)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num))0x10F466D5;
int (*TextID_GetLength_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x10F46585;
void (*TextID_Destroy_int)(TEXTID strid,char * filename,int line_num)=(void(*)(TEXTID strid,char * filename,int line_num))0x10F4647D;
TEXTID (*TextID_Copy_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x10F46321;
int (*Request_SIM_IMSI_Read)(const int* sync,const int unk,char * imsi,char * error_cause)=(int(*)(const int* sync,const int unk,char * imsi,char * error_cause))0x1128D4A5;
int (*get_imsi_const)()=(int(*)())0x111E4E61;
int (*Request_SL_GetRadioState)(const int* sync,char * level,char * flag,char * error_cause)=(int(*)(const int* sync,char * level,char * flag,char * error_cause))0x4BB44415;
int (*GetChipID_int)()=(int(*)())0x125DDDD5;
int (*ConnectionManager_Connection_GetState_int)(char * buf)=(int(*)(char * buf))0x10C0631D;
void (*TextID_GetString_int)(TEXTID, char* str, int maxlen, int null)=(void(*)(TEXTID, char* str, int maxlen, int null))0x10F4684D;
void (*Timer_ReSet_int)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(void(*)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x10DD0A5D;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(u16(*)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x10DD0A29;
void (*Timer_Kill_int)(u16* timerID,char * filename,int line_num)=(void(*)(u16* timerID,char * filename,int line_num))0x10DD0AB5;
