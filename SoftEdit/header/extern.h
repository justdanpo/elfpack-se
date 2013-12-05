#ifndef __EXTERN_H
#define __EXTERN_H
void DrawHighlightID(int Font,int SIDtime,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor);

int NewKey(int key, int r1 , int mode);
void destroy_all_images();
MyBOOK * CreateA2Book();
void copyfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2);
int cmpfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2);
int skin_execute_xml(wchar_t *folder, wchar_t *name);
void DispDraw(DISP_OBJ* DO,int a,int b,int c);
void destroy_all_images();
void Init();
extern LIST *customsofts;
#define SID_NULL 0x6FFFFFFF
#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)
#define DELETE(a) if (a) {delete(a);a=0;}
#define IDN_SAVE_BEFORE_EXIT L"HPB_SAVE_BEFORE_EXITQ_TXT"
#define IDN_CHANGES_HAVE_BEEN_MADE L"HPB_SAVE_BEFORE_EXITD_TXT"
#define IDN_CHECKBOX_UNCHECKED_ICON L"CHECKBOX_ICN"
#define IDN_CHECKBOX_CHECKED_ICON L"CHECKMARK_IN_BOX_ICN"

//в версии 1.0 данная переменная отсутствовала
//1.1 версия:
#define VERSION_ 0x00AAAA01

#endif

