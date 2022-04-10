#define K630_R1FA032
#define A2
#define DB3150

#include "..\..\include\Types.h"
#include "classes.h"

#define GetSignalQuality_Send_SignalID 0x12C00080
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x2A27F040
#define MainInput_CurPos_Offset 0x3

void (*DisplayGC_AddRef)(GC *, void *) = (void (*)(GC *, void *))0x1163837D;
int (*Illumination_LedID_SetLevel)(int LED_ID, int level) = (int (*)(int LED_ID, int level))0x115845B5;
int (*Illumination_LedID_Off)(int LED_ID) = (int (*)(int LED_ID))0x11584745;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int *sync, void *buf) = (int (*)(const int *sync, void *buf))0x10E82C59;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM *dest, AB_STR_ITEM *source) = (void (*)(AB_STR_ITEM *dest, AB_STR_ITEM *source))0x10F117E9;
int (*GetImageHeight_int)(wchar_t ImageID) = (int (*)(wchar_t ImageID))0x1163444D;
void (*Display_SetBrightnessLevel)(int level) = (void (*)(int level))0x115B9181;
TEXTID (*TextID_CreateIntegerID_int)(int num, char *filename, int line_num) = (TEXTID(*)(int num, char *filename, int line_num))0x11617E31;
TEXTID (*TextID_Create_int)(const void *wstr, int flag, int len, char *filename, int line_num) = (TEXTID(*)(const void *wstr, int flag, int len, char *filename, int line_num))0x11617FF1;
int (*TextID_GetWString_int)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num) = (int (*)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num))0x1161845D;
int (*TextID_GetLength_int)(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x1161835D;
void (*TextID_Destroy_int)(TEXTID strid, char *filename, int line_num) = (void (*)(TEXTID strid, char *filename, int line_num))0x11618289;
TEXTID (*TextID_Copy_int)(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x11618195;
int (*Request_SIM_IMSI_Read)(const int *sync, const int unk, char *imsi, char *error_cause) = (int (*)(const int *sync, const int unk, char *imsi, char *error_cause))0x10786F0D;
int (*get_imsi_const)() = (int (*)())0x106E99B5;
int (*Request_SL_GetRadioState)(const int *sync, char *level, char *flag, char *error_cause) = (int (*)(const int *sync, char *level, char *flag, char *error_cause))0x2A2024AD;
int (*GetChipID_int)() = (int (*)())0x1000295D;
int (*ConnectionManager_Connection_GetState_int)(char *buf) = (int (*)(char *buf))0x113DD28D;
void (*TextID_GetString_int)(TEXTID, char *str, int maxlen, int null) = (void (*)(TEXTID, char *str, int maxlen, int null))0x11618595;
void (*Timer_ReSet_int)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (void (*)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x11594231;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (u16(*)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x115941FD;
void (*Timer_Kill_int)(u16 *timerID, char *filename, int line_num) = (void (*)(u16 *timerID, char *filename, int line_num))0x11594289;
