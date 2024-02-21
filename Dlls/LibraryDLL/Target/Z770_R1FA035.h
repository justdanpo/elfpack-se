#define Z770_R1FA035
#define A2
#define DB3150

#include "..\..\include\Types.h"
#include "classes.h"

#define GetSignalQuality_Send_SignalID 0x12C00080
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x2A286548
#define MainInput_CurPos_Offset 0x3

void (*DisplayGC_AddRef)(GC *, void *) = (void (*)(GC *, void *))0x116B7B5D;
int (*Illumination_LedID_SetLevel)(int LED_ID, int level) = (int (*)(int LED_ID, int level))0x11602E69;
int (*Illumination_LedID_Off)(int LED_ID) = (int (*)(int LED_ID))0x11602FF9;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int *sync, void *buf) = (int (*)(const int *sync, void *buf))0x10EFDD49;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM *dest, AB_STR_ITEM *source) = (void (*)(AB_STR_ITEM *dest, AB_STR_ITEM *source))0x10F8C379;
int (*GetImageHeight_int)(wchar_t ImageID) = (int (*)(wchar_t ImageID))0x116B3B8D;
void (*Display_SetBrightnessLevel)(int level) = (void (*)(int level))0x11638729;
TEXTID (*TextID_CreateIntegerID_int)(int num, char *filename, int line_num) = (TEXTID(*)(int num, char *filename, int line_num))0x116972DD;
TEXTID (*TextID_Create_int)(const void *wstr, int flag, int len, char *filename, int line_num) = (TEXTID(*)(const void *wstr, int flag, int len, char *filename, int line_num))0x1169749D;
int (*TextID_GetWString_int)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num) = (int (*)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num))0x11697909;
int (*TextID_GetLength_int)(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x11697809;
void (*TextID_Destroy_int)(TEXTID strid, char *filename, int line_num) = (void (*)(TEXTID strid, char *filename, int line_num))0x11697735;
TEXTID (*TextID_Copy_int)(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x11697641;
int (*Request_SIM_IMSI_Read)(const int *sync, const int unk, char *imsi, char *error_cause) = (int (*)(const int *sync, const int unk, char *imsi, char *error_cause))0x107F0A81;
int (*get_imsi_const)() = (int (*)())0x107545B1;
int (*Request_SL_GetRadioState)(const int *sync, char *level, char *flag, char *error_cause) = (int (*)(const int *sync, char *level, char *flag, char *error_cause))0x2A202241;
int (*GetChipID_int)() = (int (*)())0x10002965;
int (*ConnectionManager_Connection_GetState_int)(char *buf) = (int (*)(char *buf))0x1145C3C5;
void (*TextID_GetString_int)(TEXTID, char *str, int maxlen, int null) = (void (*)(TEXTID, char *str, int maxlen, int null))0x11697A41;
void (*Timer_ReSet_int)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (void (*)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x11612AD5;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (u16(*)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x11612AA1;
void (*Timer_Kill_int)(u16 *timerID, char *filename, int line_num) = (void (*)(u16 *timerID, char *filename, int line_num))0x11612B2D;
