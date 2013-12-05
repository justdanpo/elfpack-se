#ifndef _TAMAGOCHI_H_
  #define _TAMAGOCHI_H_

#define ELFNAME "Tamagochi"
#define LELFNAME L"Tamagochi"
#define LELFVERSION L"v4.1"
#define LAUTHORINFO L"© BigHercules & Ploik\n\nOriginal idea: Eraser (Siemens)"
#define LTRANSLATED L"Translated by "

#define MIN_PET_COUNT_GLB  2
#define MAX_PET_COUNT_GLB  32
#define PET_NAME_LEN       64

#define BATHE_THRESHOLD 3

#define FREE_GUI(a) if (a) {GUIObject_Destroy(a); a = 0;}

#define SOUND_NAME_COUNT   22
#define ICONS_NAMES_COUNT  12
#define ICONS_STATUS_COUNT 4
#define ICONS_MENU_COUNT   22

#define GAME_PLAYER_LIST_LEN   1024

#define CFG_NAME L"Pet.cfg"
#define CFG_NAME_FORMAT L"Pet%d.cfg"

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

typedef struct
{
  BOOK  book;
  GUI_ONEOFMANY *cbox_gui;
  GUI *text_input;
  int cur_indx;
  GUI *yesno;
  GUI_TABMENUBAR * gui;
  GUI_LIST * menu_list[MAX_PET_COUNT_GLB];
  DISP_OBJ_ONKEY_METHOD Menu_oldOnKey[MAX_PET_COUNT_GLB];
  GUI_LIST * stat_list;
  DISP_OBJ_ONKEY_METHOD Status_oldOnKey;
}MyBOOK;

enum PET_SND {
  GLOBAL_SND = 0, //tuk-tuk 0
  HELLO_SND,      //privet 1
  BIRTHDAY_SND,   //den' rozhdeniya 2
  ALARM_SND,      //toropis'! 3
  COMEON_SND,     //nu davaj-davaj 4
  ILLGETYOU_SND,  //nu pogodi 5
  BORING_SND,     //skuchno 6
  BYEBYE_SND,     //poka 7
  FATALITY_SND,   //ubit 8
  FIRSTBLOOD_SND, //pervaya krov' 9
  LAUGH_SND,      //haha 10
  LEAVEME_SND,    //ostav' menya 11
  NOOO_SND,       //neee 12
  OHDEER_SND,     //oj-e-e-e-ej 13
  SLEEP_SND,      //zevaet 14
  OUCH_SND,       //oj bol'na 15
  OW_SND,         //au! 16
  YESSIR_SND,     //da ser 17
  CRY_SND,        //plachet 18
  CHEER_F_SND,    //  19
  CHEER_M_SND,    //  20
  BATHE_SND,      //  21
  LAST_SND
};

enum PET_ICN {
  SMILE_ICN = 0,
  DEATH_ICN,
  DREAM_ICN,
  HUNGER_ICN,
  HEALTH_ICN,
  HAPPINESS_ICN,
  DIRTINESS_ICN,
  BOREDOM_ICN,
  BEHAVIOUR_ICN,
  SINGLE_ICN,
  INLOVE_ICN,
  BATHE_ICN,
  PET_LAST_ICN
};

enum STATUS_ICN {
  NULL_ICN = 0,
  GAME_ICN,
  MOON_ICN,
  MUSIC_ICN,
  STATUS_LAST_ICN
};

enum MENU_ICN {
  PLAY_ICN = 0,
  VACCINATE_ICN,
  DISCIPLINE_ICN,
  SLEEP_ICN,
  STATUS_ICN,
  NAME_ICN,
  SETTINGS_ICN,
  ABOUT_ICN,
  SM_SMILE_ICN,
  SM_HEALTH_ICN,
  SM_HUNGER_ICN,
  SM_HAPPINESS_ICN,
  SM_DIRTINESS_ICN,
  SM_BOREDOM_ICN,
  SM_BEHAVIOUR_ICN,
  SM_FATIGUE_ICN,
  SM_SEX_ICN,
  SM_MALE_ICN,
  SM_FEMALE_ICN,
  SM_FAMILY_ICN,
  SM_BATHE_ICN,
  ROOM_ICN,
  MENU_LAST_ICN
};

typedef struct{
  int Age;          // возраст
  int TimeAge;      // минут после увеличения возраста
  int Health;       // здоровье
  int MaxHealth;    // максимальное здоровье
  int Hunger;       // голод
  int MaxHunger;    // максимальный голод
  int Happiness;    // счастье
  int MaxHappiness; // максимальное счастье
  int Dirtiness;    // грязь
  int MaxDirtiness; // максимальная грязь
  int Boredom;      // скука
  int MaxBoredom;   // максимальная скука
  int Behaviour;    // мораль
  int MaxBehaviour; // максимальная мораль
  int StatusDeath;  // статус смерти
  int ImageStatus;  // индекс статус-картинки на главный экран
  wchar_t name[PET_NAME_LEN]; // имя
  int Sex;          // пол
  int Count;        // количество питомцев
  int FamilyLife;   // семейная жизнь
  int needFriend;   //
  int haveFriend;   //
  unsigned int crc; //
} TStatusPet;

typedef struct{
  TStatusPet Status;
  int Fatigue; //усталость
  int Sleep;
  int Behaviour;
  int Bathe;
  // для отрисовки смайла и статуса
  IMAGEID *Smile;
  IMAGEID *StatImg;
  int old_img_status;
  int old_simg_status;
  int simg_status;
  int img_size;
  int x;
  int y;
  int dx;
  int dy;
} TStatusPets;

extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);
extern "C" long wcstol(const wchar_t *, wchar_t **, int);

extern int isTamagochiBook(BOOK * book);
extern void GamePlay(int indx, BOOK * book, int snd);
extern void Vaccinate(int indx, BOOK * book, int snd);
extern void Discipline(int indx, BOOK * book, int snd);
extern void SleepCommand(int indx, BOOK * book, int snd);
extern void BatheCommand(int indx, BOOK * book, int snd);
extern void InfoStatus(int indx, BOOK * book, int snd);
extern void ChangeName(int indx, BOOK * book, int snd);
extern void ShowStatus(BOOK * book);
extern void CloseStatus(BOOK * book);
extern void ShowMenu(BOOK * book);
extern void CloseMenu(BOOK * book);
extern void EditConfig(int indx, BOOK * book, int snd);
extern void life_proc(int indx, BOOK *bk, int snd);

extern TStatusPets Pets[MAX_PET_COUNT_GLB];

extern IMAGEID img_menu[ICONS_MENU_COUNT];

extern const unsigned int PARENT_AGE;
extern const unsigned int MAX_PET_COUNT;

extern const int ENA_HELLO_MSG;

extern const unsigned int REFRESH;
extern const int SpeedLife;

extern const int msgShowTime;

extern const int sndEnable;
extern const int sndOnNight;
extern const int sndOnSilent;
extern const unsigned int sndVolume;

extern const int Is_Vibra_Enabled;
extern const unsigned int vibraTime;
extern const int VIBR_TYPE;

extern const int Is_Screen_Enabled;
extern const unsigned int cfg_screen_time;
extern const int cfg_screen_blink_speed;
extern const int cfg_screen_level;

extern const int Night_Ena;
extern const unsigned int Night_begin;
extern const unsigned int Night_end;
extern const int Music_Ena;
extern const int Malware_Ena;
extern const int Scan_Game_Ena;
extern const int Motion_Ena;

extern const unsigned int POS_X;
extern const unsigned int POS_Y;

extern const unsigned  int MINSIZE;
extern const unsigned  int MAXSIZE;

extern const char PET_PATH[64];
extern const char PIC_PATH[64];
extern const char ROOM_PATH[64];
extern const char SOUND_PATH[64];
extern const char GAMELIST_PATH[64];
extern const char PLAYERLIST_PATH[64];
extern const char BCFGEDIT_PATH[64];

extern const int ACTIVE_KEY;
extern const int ACTIVE_KEY_STYLE;

#endif
