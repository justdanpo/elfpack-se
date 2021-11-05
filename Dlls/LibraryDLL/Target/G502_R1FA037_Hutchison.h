#define G502_R1FA037_Hutchison
#define A2
#define DB3150

#include "..\..\include\Types.h"
#include "classes.h"

#define GetSignalQuality_Send_SignalID 0x12C00080
#define GetSignalQuality_Receive_SignalID 0x12C007C0
#define MainInput_data 0x2A28513C
#define MainInput_CurPos_Offset 0x3

void (*DisplayGC_AddRef)(GC *, void *) = (void (*)(GC *, void *))0x116A45C9;
int (*Illumination_LedID_SetLevel)(int LED_ID, int level) = (int (*)(int LED_ID, int level))0x115F0965;
int (*Illumination_LedID_Off)(int LED_ID) = (int (*)(int LED_ID))0x115F0AF5;
int (*REQUEST_HPHONEBOOK_READSTRING)(const int *sync, void *buf) = (int (*)(const int *sync, void *buf))0x10ED9471;
void (*AB_STR_ITEM_Copy)(AB_STR_ITEM *dest, AB_STR_ITEM *source) = (void (*)(AB_STR_ITEM *dest, AB_STR_ITEM *source))0x10F681DD;
int (*GetImageHeight_int)(wchar_t ImageID) = (int (*)(wchar_t ImageID))0x116A0689;
void (*Display_SetBrightnessLevel)(int level) = (void (*)(int level))0x11625591;
TEXTID (*TextID_CreateIntegerID_int)
(int num, char *filename, int line_num) = (TEXTID(*)(int num, char *filename, int line_num))0x1168426D;
TEXTID (*TextID_Create_int)
(const void *wstr, int flag, int len, char *filename, int line_num) = (TEXTID(*)(const void *wstr, int flag, int len, char *filename, int line_num))0x1168442D;
int (*TextID_GetWString_int)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num) = (int (*)(TEXTID strid, wchar_t *dest, int maxlen, char *filename, int line_num))0x11684899;
int (*TextID_GetLength_int)(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x11684799;
void (*TextID_Destroy_int)(TEXTID strid, char *filename, int line_num) = (void (*)(TEXTID strid, char *filename, int line_num))0x116846C5;
TEXTID (*TextID_Copy_int)
(TEXTID strid, char *filename, int line_num) = (int (*)(TEXTID strid, char *filename, int line_num))0x116845D1;
int (*Request_SIM_IMSI_Read)(const int *sync, const int unk, char *imsi, char *error_cause) = (int (*)(const int *sync, const int unk, char *imsi, char *error_cause))0x107F1DB1;
int (*get_imsi_const)() = (int (*)())0x107558E9;
int (*Request_SL_GetRadioState)(const int *sync, char *level, char *flag, char *error_cause) = (int (*)(const int *sync, char *level, char *flag, char *error_cause))0x2A202251;
int (*GetChipID_int)() = (int (*)())0x10002965;
int (*ConnectionManager_Connection_GetState_int)(char *buf) = (int (*)(char *buf))0x114479B1;
void (*TextID_GetString_int)(TEXTID, char *str, int maxlen, int null) = (void (*)(TEXTID, char *str, int maxlen, int null))0x116849D1;
void (*Timer_ReSet_int)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (void (*)(u16 *timer, int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x116005E1;
u16 (*Timer_Set_int)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num) = (u16(*)(int time, TIMERPROC onTimer, LPARAM lparam, char *filename, int line_num))0x116005AD;
void (*Timer_Kill_int)(u16 *timerID, char *filename, int line_num) = (void (*)(u16 *timerID, char *filename, int line_num))0x11600639;
