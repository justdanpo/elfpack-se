#ifndef _MAIN_H_
#define _MAIN_H_

#define ELFNAME "MiniGPS"
#define LELFNAME L"MiniGPS"
#define LELFVERSION L"2.6"
#define LAUTHORINFO L"© copini\ntweaks by\nzoyd, Ploik, mousex, Joker XT, Stonos, BigHercules"
#define LTRANSLATED L"Translated by "

#define FREE_GUI(a) if (a) {GUIObject_Destroy(a); a=0;}
#define COLOR_RGBA(r,g,b,a) (((unsigned int)r<<16)|((unsigned int)g<<8)|((unsigned int)b)|((unsigned int)a<<24))

#define empty 0x6FFFFFFF
#define MAX_MANUAL_PROFILE_NUM 7
#define MAX_PLACE_NUM          16
#define MAX_AUTOLOCATION_LEN   96

#define BASE_CODE 0x8140
#define BASE_INFO 0x4E02
#define BASE_LEN  0x7D60
#define BASE_MIN  0x2E80

typedef union _tagSWAP
{
  struct
  {
    char high;
    char low;
  };
  wchar_t data;
}SWAP_T;

typedef struct
{
  BOOK * book;
}MSG;

typedef struct
{
  BOOK book;
  GUI_LIST *menu;
  DISP_OBJ_ONKEY_METHOD oldOnKey;
  GUI *text_input;
}MyBOOK;

typedef struct
{
  const int *enable;
  const wchar_t *name;
  const int *profile;
  const int *vibrate;
  const int *sound_enable;
  const wchar_t *sound_file;
}cfg_place_t;

enum INPUT_TYPES {
  IT_REAL=0,
  IT_STRING=1,
  IT_INTEGER=2,
  IT_PHONE_NUMBER=3,
  IT_DIGITAL_PASS=4,
  IT_DIGITAL_IP=5,
  IT_URL=6,
  IT_UNSIGNED_DIGIT=9,
  IT_EXTRA_DIGIT=10,
  IT_ABC_OR_DIGIT=11,
  IT_EXTRA_DIGIT_2=12,
  IT_ABC_OR_DIGIT_2=13,
};

enum align_t {
    LEFT = 0,
    RIGHT,
    CENTER
};

enum SHOW_TYPES {
  SHOW_NOTHING =0,
  SHOW_NAME    =1,
  SHOW_IMAGE   =2,
  SHOW_BOTH    =3
};

int isMiniGPSBook(BOOK * book);
void ShowHideProc();
void CellIdProc();
void AutoLocationProc();
void InvalidateAll();
int CheckCurrentCell();
int onBcfgConfig(void* mess, BOOK* bk);
int ShowAuthorInfo(void *mess ,BOOK* book);

extern bool visible;
extern bool showCellID;
extern bool AutoLocation;
extern wchar_t SIwstr[MAX_AUTOLOCATION_LEN];

#endif //_MAIN_H_

