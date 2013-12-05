#define W595_R3EG004_Orange
#define A2
#define DB3150

#include "..\..\include\Types.h"
#include "classes.h"

#define GetSignalQuality_Send_SignalID 0x12C00080
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x4BBAA2A8
#define MainInput_CurPos_Offset 0x3

void (*DisplayGC_AddRef)(GC*,void *)=(void(*)(GC*,void *))0x10A3D245;
int (*Illumination_LedID_SetLevel)(int LED_ID,int level)=(int(*)(int LED_ID,int level))0x10C39BA5;
int (*Illumination_LedID_Off)(int LED_ID)=(int(*)(int LED_ID))0x10C39DA5;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int* sync,void * buf)=(int(*)(const int* sync,void * buf))0x10ABB685;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM * dest,AB_STR_ITEM * source)=(void(*)(AB_STR_ITEM * dest,AB_STR_ITEM * source))0x10BD7E85;
int (*GetImageHeight_int)(wchar_t ImageID)=(int(*)(wchar_t ImageID))0x10A3A151;
void (*Display_SetBrightnessLevel)(int level)=(void(*)(int level))0x10E2E369;
TEXTID (*TextID_CreateIntegerID_int)(int num,char * filename,int line_num)=(TEXTID(*)(int num,char * filename,int line_num))0x10FCEE29;
TEXTID (*TextID_Create_int)(const void* wstr,int flag,int len,char * filename,int line_num)=(TEXTID(*)(const void* wstr,int flag,int len,char * filename,int line_num))0x10FCEFFD;
int (*TextID_GetWString_int)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num)=(int(*)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num))0x10FCF591;
int (*TextID_GetLength_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x10FCF441;
void (*TextID_Destroy_int)(TEXTID strid,char * filename,int line_num)=(void(*)(TEXTID strid,char * filename,int line_num))0x10FCF339;
TEXTID (*TextID_Copy_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x10FCF1DD;
int (*Request_SIM_IMSI_Read)(const int* sync,const int unk,char * imsi,char * error_cause)=(int(*)(const int* sync,const int unk,char * imsi,char * error_cause))0x11311035;
int (*get_imsi_const)()=(int(*)())0x1126E7B5;
int (*Request_SL_GetRadioState)(const int* sync,char * level,char * flag,char * error_cause)=(int(*)(const int* sync,char * level,char * flag,char * error_cause))0x4BB45785;
int (*GetChipID_int)()=(int(*)())0x12615345;
int (*ConnectionManager_Connection_GetState_int)(char * buf)=(int(*)(char * buf))0x10C918AD;
void (*TextID_GetString_int)(TEXTID, char* str, int maxlen, int null)=(void(*)(TEXTID, char* str, int maxlen, int null))0x10FCF709;
void (*Timer_ReSet_int)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(void(*)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x10E59ABD;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(u16(*)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x10E59A89;
void (*Timer_Kill_int)(u16* timerID,char * filename,int line_num)=(void(*)(u16* timerID,char * filename,int line_num))0x10E59B15;
