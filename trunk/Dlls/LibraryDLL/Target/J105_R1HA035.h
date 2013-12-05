#define J105_R1HA035
#define A2
#define DB3200

#include "..\..\include\Types.h"
#include "classes.h"

#define GetSignalQuality_Send_SignalID 0x12C00040
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x4BA25794
#define MainInput_CurPos_Offset 0x8

void (*DisplayGC_AddRef)(GC*,void *)=(void(*)(GC*,void *))0x1419CA95;
void (*TextObject_SetText)(IRichText * pTextObject,TEXTID strid)=(void(*)(IRichText * pTextObject,TEXTID strid))0x14225D4D;
void (*TextObject_SetFont)(IRichText * pTextObject,void * pFont,const int _0x8000000A,const int _0x7FFFFFF5)=(void(*)(IRichText * pTextObject,void * pFont,const int _0x8000000A,const int _0x7FFFFFF5))0x1419D3BD;
int (*Illumination_LedID_SetLevel_Gradually)(int LED_ID,int level,int fade_time)=(int(*)(int LED_ID,int level,int fade_time))0x14443BA1;
int (*Illumination_LedID_SetLevel)(int LED_ID,int level)=(int(*)(int LED_ID,int level))0x14516125;
int (*Illumination_LedID_Off)(int LED_ID)=(int(*)(int LED_ID))0x143A771D;
int (*RichTextLayout_GetTextWidth)(TEXTID strid,IRichTextLayout * pRichTextLayout,IFont * pFont)=(int(*)(TEXTID strid,IRichTextLayout * pRichTextLayout,IFont * pFont))0x1433B019;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int* sync,void * buf)=(int(*)(const int* sync,void * buf))0x144D45D1;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM * dest,AB_STR_ITEM * source)=(void(*)(AB_STR_ITEM * dest,AB_STR_ITEM * source))0x152500ED;
void (*Display_SetBrightnessLevel)(int level)=(void(*)(int level))0x153D5AC5;
TEXTID (*TextID_CreateIntegerID_int)(int num,char * filename,int line_num)=(TEXTID(*)(int num,char * filename,int line_num))0x14522D1D;
TEXTID (*TextID_Create_int)(const void* wstr,int flag,int len,char * filename,int line_num)=(TEXTID(*)(const void* wstr,int flag,int len,char * filename,int line_num))0x14227419;
int (*TextID_GetWString_int)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num)=(int(*)(TEXTID strid,wchar_t * dest,int maxlen,char * filename,int line_num))0x14226685;
int (*TextID_GetLength_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x14225DED;
void (*TextID_Destroy_int)(TEXTID strid,char * filename,int line_num)=(void(*)(TEXTID strid,char * filename,int line_num))0x14226A41;
TEXTID (*TextID_Copy_int)(TEXTID strid,char * filename,int line_num)=(int(*)(TEXTID strid,char * filename,int line_num))0x14226D41;
int (*Request_SIM_IMSI_Read)(const int* sync,const int unk,char * imsi,char * error_cause)=(int(*)(const int* sync,const int unk,char * imsi,char * error_cause))0x101441ED;
int (*get_imsi_const)()=(int(*)())0x1007B281;
int (*Request_SL_GetRadioState)(const int* sync,char * level,char * flag,char * error_cause)=(int(*)(const int* sync,char * level,char * flag,char * error_cause))0x10567C65;
int (*Network_INetworkConnection_GetConnectionInfo)(CONNECTION_INFO_DESC * desc)=(int(*)(CONNECTION_INFO_DESC * desc))0x14154EC9;
int (*GetChipID_int)()=(int(*)())0x107A4305;
OSBOOLEAN (*get_mem_int)(int,int,void*)=(OSBOOLEAN(*)(int,int,void*))0x101EA8A0;
int (*ConnectionManager_Connection_GetState_int)(char * buf)=(int(*)(char * buf))0x144BE989;
void (*TextID_GetString_int)(TEXTID, char* str, int maxlen, int null)=(void(*)(TEXTID, char* str, int maxlen, int null))0x14226B25;
void (*Timer_ReSet_int)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(void(*)(u16* timer, int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x142DB25D;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num)=(u16(*)(int time, TIMERPROC onTimer, LPARAM lparam,char * filename,int line_num))0x14503355;
void (*Timer_Kill_int)(u16* timerID,char * filename,int line_num)=(void(*)(u16* timerID,char * filename,int line_num))0x142DB1FD;
