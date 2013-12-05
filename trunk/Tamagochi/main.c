// Эльф-тамагочи
// Eraser @2007
// anis_1981@mail.ru
// ICQ:308911945

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "Tamagochi.h"
#include "RANDOM.h"
#include "conf_loader.h"
#include "language.h"
#include "misc_util.h"
#include "crypt.h"

#define ELF_HELLO_MSG_EVENT 777
#define NAME_LEN  64

#define TESTPATH L"/tpa/system/settings"
#define TESTFILE L"filetype.bin"

typedef struct
{
  BOOK * book;
  wchar_t name[NAME_LEN];
}HELLO_MSG;

#define TMR_SECOND  1000

#define STATUS_HEIGHT_128  14
#define STATUS_HEIGHT_160  14
#define STATUS_HEIGHT_208  18
#define STATUS_HEIGHT_220  18
#define STATUS_HEIGHT_320  25

#define SOFT_HEIGHT_128  18
#define SOFT_HEIGHT_160  18
#define SOFT_HEIGHT_208  26
#define SOFT_HEIGHT_220  26
#define SOFT_HEIGHT_320  30

enum SLEEP_TYPE {
  NIGHT_SLEEP = 0,
  FATIGUE_SLEEP,
  HUNGER_SLEEP,
  COMMAND_SLEEP
};

enum FIND_NAME {
  NOT_FOUND = 0,
  GAME_FOUND,
  PLAYER_FOUND
};

typedef struct
{
  BOOK * book;
}MSG;

static const wchar_t * const SoundName[SOUND_NAME_COUNT]=
{
  L"global.wav",     //tuk-tuk 0
  L"hello.wav",      //privet 1
  L"birthday.wav",   //den' rozhdeniya 2
  L"alarm.wav",      //toropis'! 3
  L"comeon.wav",     //nu davaj-davaj 4
  L"illgetyou.wav",  //nu pogodi 5
  L"boring.wav",     //skuchno 6
  L"byebye.wav",     //poka 7
  L"fatality.wav",   //ubit 8
  L"firstblood.wav", //pervaya krov' 9
  L"laugh.wav",      //haha 10
  L"leaveme.wav",    //ostav' menya 11
  L"nooo.wav",       //neee 12
  L"ohdeer.wav",     //oj-e-e-e-ej 13
  L"sleep.wav",      //zevaet 14
  L"ouch.wav",       //oj bol'na 15
  L"ow.wav",         //au! 16
  L"yessir.wav",     //da ser 17
  L"cry.wav",
  L"cheer_female.wav",
  L"cheer_male.wav",
  L"bathe.wav"
};

static const wchar_t * const icons_names[ICONS_NAMES_COUNT]=
{
  L"smile.png",
  L"death.png",
  L"dream.png",
  L"hunger.png",
  L"Health.png",
  L"Happiness.png",
  L"Dirtiness.png",
  L"Boredom.png",
  L"Behaviour.png",
  L"single.png",
  L"in_love.png",
  L"bathe.png"
};

static const wchar_t * const icons_status[ICONS_STATUS_COUNT]=
{
  L"null.png",
  L"game.png",
  L"moon.png",
  L"music.png"
};

static const wchar_t * const icons_menu[ICONS_MENU_COUNT]=
{
  L"game.png",
  L"vaccinate.png",
  L"discipline.png",
  L"sleep.png",
  L"status.png",
  L"sm_smile.png",
  L"settings.png",
  L"about.png",
  L"sm_smile.png",
  L"heart.png",
  L"sm_hunger.png",
  L"sm_Happiness.png",
  L"sm_Dirtiness.png",
  L"sm_Boredom.png",
  L"sm_Behaviour.png",
  L"sm_dream.png",
  L"sm_sex.png",
  L"sm_male.png",
  L"sm_female.png",
  L"sm_family.png",
  L"sm_bathe.png",
  L"room.png"
};

DISP_OBJ_ONREDRAW_METHOD Status_oldReDraw = 0;
DISP_OBJ ** GUI_status = 0;
DISP_DESC * Status_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw = 0;
DISP_OBJ * GUI_display = 0;
DISP_DESC * Display_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Soft_oldReDraw = 0;
DISP_OBJ * GUI_soft = 0;
DISP_DESC * Soft_desc = 0;

int DisplayWidth  = 0; //ширина дисплея в пикселях
int DisplayHeight = 0; //высота дисплея в пикселях
int SoftBarHeight   = 0;
int StatusBarHeight = 0;
int SoftBarY   = 0;
int StatusBarY = 0;

MyBOOK *TamagochiBook = 0;

static char myappname[128]=ELFNAME;

IMAGEID img_names[ICONS_NAMES_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                    NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                    NOIMAGE,NOIMAGE,NOIMAGE};

IMAGEID img_status[ICONS_STATUS_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};

IMAGEID img_menu[ICONS_MENU_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                  NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                  NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                  NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                  NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,
                                  NOIMAGE,NOIMAGE};

int SmileSize = 0;
int StatSize  = 0;

//Статусы питомцев
TStatusPets Pets[MAX_PET_COUNT_GLB];

char game_list[GAME_PLAYER_LIST_LEN];
char player_list[GAME_PLAYER_LIST_LEN];

u16 timer_save    = 0;
u16 timer_refresh = 0;
u16 timer_life    = 0;

int screen = 0;
u16 timerScreen = 0;
u16 offtimerScreen = 0;

void InitStatus(int indx, int age, int sex);
void InitPetVar(int indx);
int SaveStatus(int indx);
void LoadAllPets();
int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*);
int isTamagochiBook(BOOK * book);
void onTimer_save (u16 timerID, LPARAM lparam);
void onTimer_refresh (u16 timerID, LPARAM lparam);
void onTimer_life (u16 timerID, LPARAM lparam);

int isBookmanDaemonBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"BookManagerDaemon")) return(1);
  return(0);
}

// проверка, обрабатывает ли base_page книги событие
int CheckEv(BOOK * bk, int ev)
{
  PAGE_MSG * pm;
  if (bk->base_page)
  {
    pm=(PAGE_MSG *)bk->base_page->msg_lst;
    if (pm)
    {
      while ((pm->msg) && (pm->proc))
      {
        if (pm->msg==ev)
        {
          return(1);
        }
        pm++;
      }
    }
  }
  return(0);
}

// получить имя жавы
int GetJavaName(BOOK * bk)
{
  wchar_t ws[100];
  TextID_GetWString(BookObj_GetSession(bk)->name,ws,100);
  if (!wstrncmp(ws,L"Foreign app",11))
  {
    return(JavaSession_GetName());
  }
  if (!wstrcmp(ws,L"Java"))
  {
    return(JavaSession_GetName());
  }
  return(0);
}

void WriteLog(char *buf)
{
  int flog;
  wchar_t path[256];
  char msg[512];
  DATETIME dt;

  REQUEST_DATEANDTIME_GET(SYNC,&dt);

  wstrcpy(path,GetDir(DIR_OTHER|MEM_EXTERNAL));
  if ((flog=_fopen(path,L"TamagochiLog.txt",FSX_O_CREAT|FSX_O_APPEND|FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0)) < 0) { return; }

  sprintf(msg, "%02d:%02d:%02d %s\n", dt.time.hour, dt.time.min, dt.time.sec, buf);
  fwrite(flog,msg,strlen(msg));
  fclose(flog);
}

/*
wchar_t ImageID=NOIMAGE;
wchar_t ImageHandle=NOIMAGE;

wchar_t RegisterImage(wchar_t * path, wchar_t * fname)
{
  FSTAT _fstat;
  char error_code;
  int sync=0;
  if(fstat(path,fname,&_fstat)==0)
  {
    if (!REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE(&sync,(u16 *)&ImageHandle,&error_code))
    {
      if(!Gif2ID(ImageHandle,path,fname,&ImageID))
      {
        if (!REQUEST_IMAGEHANDLER_INTERNAL_REGISTER(&sync,ImageHandle,path,fname,0,&ImageID,&error_code))
        {
          if (error_code) ImageHandle=NOIMAGE;
        }
      }
    }
  }
  return(ImageID);
}

wchar_t UnRegisterImage()
{
  char error_code;
  REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER(SYNC,ImageHandle,0,0,ImageID,1,&error_code);
  ImageID=NOIMAGE;
  ImageHandle=NOIMAGE;
  return 0;
};
*/

void free_strings()
{

}

void load_strings()
{
  free_strings();

}

void free_icons()
{
  for (int i=0;i<ICONS_NAMES_COUNT;i++)
  {
      if(img_names[i] != NOIMAGE)
      {
          ImageID_Free(img_names[i]);
          img_names[i] = NOIMAGE;
      }
  }
  for (int i=0;i<ICONS_STATUS_COUNT;i++)
  {
      if(img_status[i] != NOIMAGE)
      {
          ImageID_Free(img_status[i]);
          img_status[i] = NOIMAGE;
      }
  }
  for (int i=0;i<ICONS_MENU_COUNT;i++)
  {
      if(img_menu[i] != NOIMAGE)
      {
          ImageID_Free(img_menu[i]);
          img_menu[i] = NOIMAGE;
      }
  }

//  UnRegisterImage();
}

void load_icons()
{
  FSTAT _fstat;
  wchar_t path[128];
  getdir(path,PIC_PATH);

  free_icons();

  for(int i=0; i< ICONS_NAMES_COUNT; i++)
  {
    if(fstat(path,icons_names[i],&_fstat)==0)
    {
      if(ImageID_Get(path,icons_names[i],&img_names[i])<0) img_names[i] = NOIMAGE;
    }
  }

  SmileSize = GetImageWidth(img_names[0]);

  for(int i=0; i< ICONS_STATUS_COUNT; i++)
  {
    if(fstat(path,icons_status[i],&_fstat)==0)
    {
      if(ImageID_Get(path,icons_status[i],&img_status[i])<0) img_status[i] = NOIMAGE;
    }
  }

  StatSize = GetImageWidth(img_status[0]);

  for(int i=0; i< ICONS_MENU_COUNT; i++)
  {
    if(fstat(path,icons_menu[i],&_fstat)==0)
    {
      if(ImageID_Get(path,icons_menu[i],&img_menu[i])<0) img_menu[i] = NOIMAGE;
    }
  }
//  RegisterImage(path, L"test.gif");
}

// ----------------------------------------------------------------------------
static void UpdateName(void)
{
  sprintf(myappname,"%s - %d age",ELFNAME, Pets[0].Status.Age);
  TextID_Destroy(((BOOK *)TamagochiBook)->xbook->app_session->name);
  ((BOOK *)TamagochiBook)->xbook->app_session->name = TextID_Create(myappname,ENC_LAT1,TEXTID_ANY_LEN);
}

// ----------------------------------------------------------------------------
int isNight()
{
  if (Night_Ena)
  {
      //проверка ночи
      DATETIME dt;
      REQUEST_DATEANDTIME_GET(SYNC,&dt);

      if (Night_begin>=Night_end)
        if ((dt.time.hour>=Night_begin)||(dt.time.hour<Night_end))
          return 1;

      if (Night_begin<Night_end)
        if ((dt.time.hour>=Night_begin)&&(dt.time.hour<Night_end))
          return 1;
  }
  return 0;
}

// ----------------------------------------------------------------------------
void start_vibra(void)
{
  if(Is_Vibra_Enabled)
  {
    int full,on,off;
    if(VIBR_TYPE == 0)
    {
      on = vibraTime;
      off = 0;
      full = vibraTime;
    }
    else
    {
      int t = vibraTime/20;
      on = 9*t;
      off = t;
      full = 20*t;
    }
    PAudioControl pAC = AudioControl_Init();
    if( !pAC ) pAC = *GetAudioControlPtr();
    AudioControl_Vibrate(pAC, on, off, full);
  }
}

// ----------------------------------------------------------------------------
void offTimerScreen(u16 timerID, LPARAM lparam)
{
  if(timerScreen)
  {
    Timer_Kill(&timerScreen);
    timerScreen = 0;
    IndicationDevice_Backlight_FadeToLevel(0,cfg_screen_level);
  }
  screen = 0;
}

void onTimerScreen(u16 timerID, LPARAM lparam)
{
    IndicationDevice_Backlight_FadeToLevel(0,screen ^= cfg_screen_level);
    Timer_ReSet(&timerScreen,cfg_screen_blink_speed,onTimerScreen,0);
}

void Light()
{
  if(Is_Screen_Enabled)
  {
    BOOK * ScreenSaverBook = FindBook(get_IsScreenSaverBook());
    if (ScreenSaverBook)
    {
      UI_Event_toBookID(UI_SCREENSAVER_DEACTIVATE_EVENT,BookObj_GetBookID(ScreenSaverBook));
    }
    timerScreen=Timer_Set(cfg_screen_blink_speed,onTimerScreen,0);
    offtimerScreen=Timer_Set(cfg_screen_time*1000,offTimerScreen,0);
  }
}

// ----------------------------------------------------------------------------

void set_all_simg_status(int status)
{
  for(int i=0; i<MAX_PET_COUNT_GLB; i++) Pets[i].simg_status = status;
}

void set_all_old_img_status(int status)
{
  for(int i=0; i<MAX_PET_COUNT_GLB; i++) Pets[i].old_img_status = status;
}
// ----------------------------------------------------------------------------
int FindName()
{
  int i,j,k;
  int mask=((int)LastExtDB())&0xF8000000;
  BOOK * book;
  UI_APP_SESSION * session;

  i=root_list_get_session_count();
  for (j=0; j<i; j++)
  {
    session=root_list_get_session(j);
    for (k=0;k<session->listbook->FirstFree;k++)
    {
      book=(BOOK*)List_Get(session->listbook,k);
      if( (book->xguilist->guilist->FirstFree) || (((((int)book->onClose)&0xF8000000) != mask)))
      {
        char s[100];
        wchar_t ws[100];
        if(strcmp(book->xbook->name,"CUIDisplayableBook"))
        {
          str2wstr(ws,book->xbook->name);
        }
        else
        {
          TextID_GetWString(book->xbook->app_session->name,ws,100);
        }
        int nameID;
        if((nameID = GetJavaName(book)))
        {
          TextID_GetWString(nameID,ws,100);
        }
        wstr2strn (s, ws, 99);
        if(strstr(game_list,s)) return GAME_FOUND;
        if(strstr(player_list,s)) return PLAYER_FOUND;
      }
    }
  }
  return NOT_FOUND;
}

int GameDetected(void)
{
  int find=0;

  if((find = FindName()) != NOT_FOUND) // поиск по game_list и player_list
  {
    if(find == GAME_FOUND)
    {
      set_all_simg_status(GAME_ICN);
      return 1;
    }
    if(find == PLAYER_FOUND)
    {
      set_all_simg_status(MUSIC_ICN);
      return 1;
    }
  }
  else if(FindBook(get_IsAudioPlayerBook())) // ищем медиаплеер
  {
    set_all_simg_status(MUSIC_ICN);
    return 1;
  }
  else if(FindBook(get_IsFmRadioBook())) // ищем радио
  {
    set_all_simg_status(MUSIC_ICN);
    return 1;
  }
  return 0;
}

// ----------------------------------------------------------------------------
void ChangeStatusImage(int indx, BOOK *bk, int snd)
{
  TStatusPet *StatusPet = &Pets[indx].Status;
  int SIZE = MINSIZE + StatusPet->Age;
  if (SIZE > MAXSIZE) SIZE = MAXSIZE;

  Pets[indx].img_size = SmileSize * SIZE / 100;

  // Статусные картинки
  StatusPet->ImageStatus=SMILE_ICN;
  if (StatusPet->needFriend) StatusPet->ImageStatus=SINGLE_ICN;
  if (StatusPet->haveFriend == 1) StatusPet->ImageStatus=INLOVE_ICN;
  if (StatusPet->Behaviour < StatusPet->MaxBehaviour/3) StatusPet->ImageStatus=BEHAVIOUR_ICN;
  if (StatusPet->Boredom   > StatusPet->MaxBoredom/2)   StatusPet->ImageStatus=BOREDOM_ICN;
  if (StatusPet->Dirtiness > StatusPet->MaxDirtiness/2) StatusPet->ImageStatus=DIRTINESS_ICN;
  if (StatusPet->Hunger    > StatusPet->MaxHunger/2)    StatusPet->ImageStatus=HUNGER_ICN;
  if (StatusPet->Health    < StatusPet->MaxHealth/2)    StatusPet->ImageStatus=HEALTH_ICN;
  if (StatusPet->Happiness < StatusPet->MaxHappiness/2) StatusPet->ImageStatus=HAPPINESS_ICN;

  if (Pets[indx].Bathe) StatusPet->ImageStatus = BATHE_ICN;
  // сон
  if (Pets[indx].Sleep != 0) StatusPet->ImageStatus = DREAM_ICN;
  if (StatusPet->StatusDeath == 1) StatusPet->ImageStatus = DEATH_ICN;

  if (Pets[indx].old_simg_status != Pets[indx].simg_status)
  {
    Pets[indx].StatImg = &img_status[Pets[indx].simg_status];
    Pets[indx].old_simg_status = Pets[indx].simg_status;
  }

  if (Pets[indx].old_img_status != StatusPet->ImageStatus)
  {
    Pets[indx].Smile = &img_names[StatusPet->ImageStatus];
    start_vibra();
    Light();
    Pets[indx].old_img_status = StatusPet->ImageStatus;
  }

  if(StatusPet->StatusDeath == 1) return;

  const wchar_t *sound_name = 0;
  wchar_t  sound_path[128];
  getdir(sound_path,SOUND_PATH);

  if (StatusPet->Dirtiness >= (StatusPet->MaxDirtiness - 10))
  {
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGDIRTINESS,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    sound_name = SoundName[OHDEER_SND];
  }
  if (StatusPet->Boredom >= (StatusPet->MaxBoredom - 10))
  {
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGBOREDOM,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    sound_name = SoundName[BORING_SND];
  }
  if (StatusPet->Hunger >= (StatusPet->MaxHunger - 10))
  {
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGHUNGER,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    sound_name = SoundName[ILLGETYOU_SND];
  }
  if (StatusPet->Health <= 10)
  {
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGHEALTH,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    sound_name = SoundName[COMEON_SND];
  }
  if (StatusPet->Happiness <= 10)
    sound_name = SoundName[NOOO_SND];

  if(sound_name && snd) PlayFileV(sound_path, sound_name, sndVolume);
}

// ----------------------------------------------------------------------------
void VerifyStatus(int indx)
{
  TStatusPet *StatusPet = &Pets[indx].Status;
// проверки максимальных - минимальных значений
  if (StatusPet->Hunger > StatusPet->MaxHunger) StatusPet->Hunger = StatusPet->MaxHunger;
  if (StatusPet->Hunger < 0) StatusPet->Hunger = 0;
  if (StatusPet->Happiness > StatusPet->MaxHappiness) StatusPet->Happiness = StatusPet->MaxHappiness;
  if (StatusPet->Happiness < 0) StatusPet->Happiness = 0;
  if (StatusPet->Health > StatusPet->MaxHealth) StatusPet->Health = StatusPet->MaxHealth;
  if (StatusPet->Health < 0) StatusPet->Health = 0;
  if (StatusPet->Dirtiness > StatusPet->MaxDirtiness) StatusPet->Dirtiness = StatusPet->MaxDirtiness;
  if (StatusPet->Dirtiness < 0) StatusPet->Dirtiness = 0;
  if (StatusPet->Boredom > StatusPet->MaxBoredom) StatusPet->Boredom = StatusPet->MaxBoredom;
  if (StatusPet->Boredom < 0) StatusPet->Boredom = 0;
  if (StatusPet->Behaviour > StatusPet->MaxBehaviour) StatusPet->Behaviour = StatusPet->MaxBehaviour;
  if (StatusPet->Behaviour < 0) StatusPet->Behaviour = 0;
  if (Pets[indx].Fatigue > 100) Pets[indx].Fatigue = 100;
  if (Pets[indx].Fatigue < 0) Pets[indx].Fatigue=0;
}
// ---------------------------------------------------------------------------

void InfoStatus(int indx, BOOK * book, int)
{
  ShowStatus(book);
}
// ----------------------------------------------------------------------------
void GamePlay(int indx, BOOK * bk, int snd)
{
  wchar_t  sound_path[128];
  getdir(sound_path,SOUND_PATH);

  if(Pets[indx].Status.StatusDeath == 1)
  {
    if(snd) PlayFileV(sound_path, SoundName[FATALITY_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMDIE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  if(Pets[indx].Sleep != 0)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMSLEEP,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  Pets[indx].Status.Happiness += 10;
  Pets[indx].Status.Boredom   -= 10;
  Pets[indx].Status.Hunger    += 10;
  Pets[indx].Fatigue                 += 10;
  if(snd) PlayFileV(sound_path, SoundName[LAUGH_SND], sndVolume);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_COOL,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  ChangeStatusImage(indx, bk, snd);
  VerifyStatus(indx);
}
// ----------------------------------------------------------------------------
void Vaccinate(int indx, BOOK * bk, int snd)
{
  wchar_t  sound_path[128];
  getdir(sound_path,SOUND_PATH);

  if (Pets[indx].Status.StatusDeath == 1)
  {
    if(snd) PlayFileV(sound_path, SoundName[FATALITY_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMDIE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }
  if (Pets[indx].Sleep != 0)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMSLEEP,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }
  if (Pets[indx].Status.Behaviour < Pets[indx].Status.MaxBehaviour/4)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_DONTDISTURB,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  Pets[indx].Status.Happiness -= 10;
  Pets[indx].Status.Health    += 10;
  Pets[indx].Status.Boredom   -= 10;

  if(snd) PlayFileV(sound_path, SoundName[OUCH_SND], sndVolume);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_PAIN,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  ChangeStatusImage(indx, bk, snd);
  VerifyStatus(indx);
}
// ----------------------------------------------------------------------------
void Discipline(int indx, BOOK * bk, int snd)
{
  wchar_t  sound_path[128];
  getdir(sound_path,SOUND_PATH);

  if (Pets[indx].Status.StatusDeath == 1)
  {
    if(snd) PlayFileV(sound_path, SoundName[FATALITY_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMDIE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }
  if (Pets[indx].Sleep != 0)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMSLEEP,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  Pets[indx].Status.Happiness -= 10;
  Pets[indx].Status.Behaviour += 10;
  Pets[indx].Status.Boredom   -= 10;

  if(snd) PlayFileV(sound_path, SoundName[YESSIR_SND], sndVolume);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_SHREW,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  ChangeStatusImage(indx, bk, snd);
  VerifyStatus(indx);
}
// ----------------------------------------------------------------------------
void SleepProc(int indx, BOOK *bk, int snd, int type)
{
  wchar_t sound_path[128];
  const wchar_t *sound_name0;
  wchar_t *msg0;
  const wchar_t *sound_name1;
  wchar_t *msg1;
  getdir(sound_path,SOUND_PATH);

  if (Pets[indx].Status.StatusDeath==1)
  {
    if(snd) PlayFileV(sound_path, SoundName[FATALITY_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMDIE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  if (Pets[indx].Status.Behaviour < Pets[indx].Status.MaxBehaviour/4)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    if(Pets[indx].Sleep)
      MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMSLEEP,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    else
      MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_DONTDISTURB,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  if(type == NIGHT_SLEEP)
  {
    sound_name0 = SoundName[SLEEP_SND];
    sound_name1 = SoundName[OW_SND];
    msg0 = LG_MSGNIGHTSLEEP;
    msg1 = LG_MSGNIGHTNOTSLEEP;
  }
  else if(type == FATIGUE_SLEEP)
  {
    sound_name0 = SoundName[SLEEP_SND];
    sound_name1 = SoundName[OW_SND];
    msg0 = LG_MSGFATIGUESLEEP;
    msg1 = LG_MSGFATIGUENOTSLEEP;
  }
  else if(type == HUNGER_SLEEP)
  {
    sound_name0 = SoundName[ILLGETYOU_SND];
    sound_name1 = SoundName[ILLGETYOU_SND];
    msg0 = LG_MSGHUNGER;
    msg1 = LG_MSGHUNGER;
  }
  else
  {
    sound_name0 = SoundName[SLEEP_SND];
    sound_name1 = SoundName[ILLGETYOU_SND];
    msg0 = LG_MSGSLEEP;
    msg1 = LG_MSGNOTSLEEP;
  }

  if(Pets[indx].Sleep==0)
  {
    Pets[indx].Sleep=1;
    Pets[indx].Bathe=0;
    if(snd) PlayFileV(sound_path, sound_name0, sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(msg0,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  }
  else
  {
    Pets[indx].Sleep=0;
    if(snd) PlayFileV(sound_path, sound_name1, sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(msg1,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  }
  ChangeStatusImage(indx, bk, snd);

}

// ----------------------------------------------------------------------------
void NightSleep(int indx, BOOK *bk, int snd)
{
  SleepProc(indx, bk, snd, NIGHT_SLEEP);
}

void FatigueSleep(int indx, BOOK *bk, int snd)
{
  SleepProc(indx, bk, snd, FATIGUE_SLEEP);
}

void HungerSleep(int indx, BOOK *bk, int snd)
{
  SleepProc(indx, bk, snd, HUNGER_SLEEP);
}

void SleepCommand(int indx, BOOK *bk, int snd)
{
  SleepProc(indx, bk, snd, COMMAND_SLEEP);
}

// ----------------------------------------------------------------------------
void BatheCommand(int indx, BOOK *bk, int snd)
{
  wchar_t  sound_path[128];
  getdir(sound_path,SOUND_PATH);

  if (Pets[indx].Status.StatusDeath == 1)
  {
    if(snd) PlayFileV(sound_path, SoundName[FATALITY_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMDIE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }
  if (Pets[indx].Sleep != 0)
  {
    if(snd) PlayFileV(sound_path, SoundName[LEAVEME_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_IAMSLEEP,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    return;
  }

  Pets[indx].Bathe = 1;

  if(snd) PlayFileV(sound_path, SoundName[BATHE_SND], sndVolume);
  MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_BLISS,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  ChangeStatusImage(indx, bk, snd);
  VerifyStatus(indx);
}

// ----------------------------------------------------------------------------
void OnBackChangeName(BOOK * book, u16 *string, int len)
{
  MyBOOK * bk = (MyBOOK *)book;
  FREE_GUI(bk->text_input);
}

void OnOkChangeName(BOOK * book, wchar_t *string, int len)
{
  MyBOOK * bk=(MyBOOK *)book;
  int indx = bk->cur_indx;

  if ((len < 1) || (len > MAXELEMS(Pets[indx].Status.name)))
  {
    wchar_t ustr[64];
    snwprintf(ustr,MAXELEMS(ustr)-1,L"min_string_len: 1\nmax_string_len: %d",MAXELEMS(Pets[indx].Status.name));
    MessageBox(EMPTY_TEXTID,TextID_Create(ustr,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, book);
  }
  else
  {
    wstrncpy(Pets[indx].Status.name,string,MAXELEMS(Pets[indx].Status.name)-1);
    FREE_GUI(bk->text_input);
//    CloseMenu(book);
//    ShowMenu(book);
  }
}

void ChangeName(int indx, BOOK * book, int snd)
{
  TEXTID text, header_name;
  MyBOOK * bk = (MyBOOK *)book;
  bk->cur_indx = indx;
  header_name = TextID_Create(LG_NAME,ENC_UCS2,TEXTID_ANY_LEN);
  text = TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN);
  FREE_GUI(bk->text_input);
  bk->text_input = CreateStringInputVA(0,
                                          VAR_HEADER_TEXT(header_name),
                                          VAR_STRINP_MIN_LEN(1),
                                          VAR_STRINP_MAX_LEN(MAXELEMS(Pets[indx].Status.name)),
                                          VAR_STRINP_MODE(IT_STRING),
                                          VAR_BOOK(bk),
                                          VAR_STRINP_TEXT(text),
                                          VAR_PREV_ACTION_PROC(OnBackChangeName),
                                          VAR_OK_PROC(OnOkChangeName),
                                          VAR_STRINP_IS_PASS_MODE(0),
                                          0);
}

// ----------------------------------------------------------------------------
void FamilyLifeUpdate(int indx)
{
  if((indx == 0) && (Pets[0].Status.Count > 1))
  {
    if((Pets[0].Status.Age >= PARENT_AGE) && (Pets[1].Status.Age >= PARENT_AGE))
    {
      ++Pets[0].Status.FamilyLife;
      Pets[1].Status.FamilyLife = Pets[0].Status.FamilyLife;
    }
    else
    {
      Pets[0].Status.FamilyLife = 0;
      Pets[1].Status.FamilyLife = 0;
    }
  }
}

int NewBabyCheck(int indx)
{
  if(indx == 0)
  {
    if((Pets[0].Status.Count > 1) && (Pets[0].Status.Count < (MAX_PET_COUNT - 1)))
    {
      if(Pets[0].Status.FamilyLife >= ((Pets[0].Status.Count - 1) * 2))
      {
        int NewCount = (Pets[0].Status.FamilyLife/2) + 2;
        if(NewCount > MAX_PET_COUNT) NewCount = MAX_PET_COUNT;
        while(Pets[0].Status.Count < NewCount)
        {
          InitStatus(Pets[0].Status.Count, 0, 2);
          InitPetVar(Pets[0].Status.Count);
          ++Pets[0].Status.Count;
        }
        return 1;
      }
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------

void onTimer_save (u16 timerID, LPARAM lparam)
{
  Randomize();
  for(int i=0; i < Pets[0].Status.Count; i++) { SaveStatus(i); }
  Timer_ReSet(&timer_save,TMR_SECOND*60*5,onTimer_save,0);
}

void UpdatePosition(int indx)
{
  if((Pets[indx].Sleep == 0) && (Pets[indx].Status.StatusDeath != 1))
  {
    if ((Pets[indx].x > (DisplayWidth - Pets[indx].img_size - 5)) && (Pets[indx].dx > 0)) Pets[indx].dx = 0-Pets[indx].dx;
    if ((Pets[indx].x < 5) && (Pets[indx].dx < 0)) Pets[indx].dx = 0-Pets[indx].dx;
    Pets[indx].x += Pets[indx].dx;

    if ((Pets[indx].y > (DisplayHeight - SoftBarHeight - Pets[indx].img_size - 5)) && (Pets[indx].dy > 0)) Pets[indx].dy = 0-Pets[indx].dy;
    if ((Pets[indx].y < (StatusBarHeight + 5)) && (Pets[indx].dy < 0)) Pets[indx].dy = 0-Pets[indx].dy;
    Pets[indx].y += Pets[indx].dy;
  }
}

void onTimer_refresh (u16 timerID, LPARAM lparam)
{
  if(Motion_Ena)
  {
    for(int i=0; i<Pets[0].Status.Count; i++)
    {
      UpdatePosition(i);
    }
  }
  if(GUI_display) DispObject_InvalidateRect(GUI_display,0);
  Timer_ReSet(&timer_refresh,REFRESH*TMR_SECOND/10,onTimer_refresh,0);
}

//главная процедура жизненного цикла питомца
void life_proc(int indx, BOOK *bk, int snd)
{
  wchar_t  sound_path[128];

  if (Pets[indx].Status.StatusDeath == 1)
  {
    Pets[indx].simg_status = NULL_ICN;
    ChangeStatusImage(indx, bk, snd);
    return ;
  }

  BATT batt;
  int isCharging = 0;
  GetBatteryState(SYNC,&batt);
  if((batt.ChargingState == 2) || (batt.ChargingState == 8))
  {
    isCharging = 1;
  }

  if (Night_Ena)
  {
      //проверка ночи
      int fNight=isNight();
      //если включена зарядка и можно поесть, то просыпаемся и едим
      if(isCharging && (Pets[indx].Status.Hunger > 0)) fNight = 0;

      if (Pets[indx].Sleep!=5)
      {
        //не спит
        if (fNight)
        {
          if (Pets[indx].Sleep!=0) NightSleep(indx, bk, snd);
          Pets[indx].Sleep=5;
          Pets[indx].Bathe=0;
          Pets[indx].Status.ImageStatus=DREAM_ICN;
          Pets[indx].simg_status=MOON_ICN;
          ChangeStatusImage(indx, bk, snd);
          return;
        }
      }
      if (Pets[indx].Sleep==5)
      {
        //спит
        if (!fNight)
        {
         NightSleep(indx, bk, snd);
         Pets[indx].simg_status=NULL_ICN;
        }
        else
        {
         Pets[indx].Status.ImageStatus=DREAM_ICN;
         Pets[indx].simg_status=MOON_ICN;
         ChangeStatusImage(indx, bk, snd);
         return;
        }
      }
  }

  getdir(sound_path,SOUND_PATH);

// усталость =100 сон
// усталость =0 подъем
  if ((Pets[indx].Fatigue >= 100))
    FatigueSleep(indx, bk, snd);
  if ((Pets[indx].Sleep != 0) && (Pets[indx].Fatigue <= 0))
    FatigueSleep(indx, bk, snd);
  if (Pets[indx].Sleep == 0) ++Pets[indx].Fatigue; else --Pets[indx].Fatigue;

//  ++Behaviour;
  ++Pets[indx].Status.TimeAge;
  if (Pets[indx].Status.TimeAge >= Pets[indx].Status.Age*10+100)
  {
    ++Pets[indx].Status.Age;
    Pets[indx].Status.TimeAge       = 0;
    Pets[indx].Status.MaxHealth    += 2;
    Pets[indx].Status.MaxHunger    += 2;
    Pets[indx].Status.MaxHappiness += 2;
    Pets[indx].Status.MaxDirtiness += 2;
    Pets[indx].Status.MaxBoredom   += 2;
    Pets[indx].Status.MaxBehaviour += 2;

    //StatusPet.Health=StatusPet.MaxHealth;
    //StatusPet.Hunger=0;
    Pets[indx].Status.Happiness = Pets[indx].Status.MaxHappiness;
    Pets[indx].Status.Boredom=0;
    Pets[indx].Status.Behaviour = (int)2*Pets[indx].Status.MaxBehaviour/3;

    FamilyLifeUpdate(indx);
    if(NewBabyCheck(indx))
    {
        if(snd) PlayFileV(sound_path, SoundName[CRY_SND], sndVolume);
        start_vibra();
        Light();
        MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGNEWPET,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    }
    else
    {
      if(snd) PlayFileV(sound_path, SoundName[BIRTHDAY_SND], sndVolume);
      start_vibra();
      Light();
      MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGAGE,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    }

    UpdateName();
  }

  // сон - процессы замедляются
  if ((Pets[indx].Sleep != 0) && (Pets[indx].Sleep != 3))
  {
    ++ Pets[indx].Sleep;
    Pets[indx].Status.ImageStatus = DREAM_ICN;
    ChangeStatusImage(indx, bk, snd);
    return;
  }

  if (Pets[indx].Sleep != 0) Pets[indx].Sleep = 1;

  //во время сна
  if (Pets[indx].Sleep != 0)
  {
    ++Pets[indx].Status.Happiness;
    Pets[indx].Bathe = 0;
  }

  //когда не спит
  if (Pets[indx].Sleep == 0)
  {
      if(!isCharging) --Pets[indx].Status.Behaviour; // дисциплина
      // пакость = убивает процесс, если совсем дисциплина на нуле,
      // и чтобы сильно не ругали дисциплина увеличивается до 30%

       if ((Pets[indx].Status.Behaviour <= 0) && Malware_Ena)
       {
         int i,j,k;
         int mask=((int)LastExtDB())&0xF8000000;
         BOOK * book;
         UI_APP_SESSION * session;
         int fkill=1;

         i=root_list_get_session_count();
         for (j=0;j<i;j++)
         {
           session=root_list_get_session(j);
           for (k=0;k<session->listbook->FirstFree;k++)
           {
             book=(BOOK*)List_Get(session->listbook,k);
             if (fkill)
             if (!isBookmanDaemonBook(book))
             {
               if((book->xguilist->guilist->FirstFree)||(((((int)book->onClose)&0xF8000000)!=mask)))
               {
                 if((!IsVolumeControllerBook(book))&&(!IsRightNowBook(book)))
                 {
                   if (((((int)book->onClose)&0xF8000000)==mask))
                   {// book or java
                     if (Find_StandbyBook()!=book)
                     {
                       if (int nameID = GetJavaName(book))
                       {
                         JavaSession_Manager(0x0E);
                       }
                       else
                       {
                         UI_Event_toBookID(RETURN_TO_STANDBY_EVENT,BookObj_GetBookID(book));
                         UI_Event_toBookID(TERMINATE_SESSION_EVENT,BookObj_GetBookID(book));
                       }
                       fkill=0;
                       Pets[indx].Status.Behaviour = Pets[indx].Status.MaxBehaviour/3;
                       if(snd) PlayFileV(sound_path, SoundName[OHDEER_SND], sndVolume);
                     }
                   }
                   else
                   {// elf
                     if(!isTamagochiBook(book))
                     {
                       if(Find_StandbyBook()!=book)
                       {
                         if (CheckEv(book,ELF_TERMINATE_EVENT))
                         {
                           UI_Event_toBookID(ELF_TERMINATE_EVENT,BookObj_GetBookID(book));
                           fkill=0;
                           Pets[indx].Status.Behaviour = Pets[indx].Status.MaxBehaviour/3;
                           if(snd) PlayFileV(sound_path, SoundName[OHDEER_SND], sndVolume);
                         }
                       }
                     }
                   }
                 }
               }
             }
           }
         }
       }

      // проверка на наличие в процессах игр
      if (Scan_Game_Ena)
      {
        if (GameDetected())
        {
          --Pets[indx].Status.Boredom; // скука
          ++Pets[indx].Status.Happiness;
        }
        else
        {
          //--StatusPet.Happiness;
          if(Pets[indx].Bathe == 0) ++Pets[indx].Status.Boredom; // скука
          Pets[indx].simg_status=0;
        }
      }
      else
      {
          if(Pets[indx].Bathe == 0) ++Pets[indx].Status.Boredom; // скука
      }
      // если скучно включаем музыку (если разрешили)
      if(Music_Ena)
      if(Pets[indx].Status.Boredom > (Pets[indx].Status.MaxBoredom - 10))
      {
        if(!FindBook(get_IsAudioPlayerBook()) && (!FindBook(get_IsFmRadioBook())))
        {
          GoMusic();
        }
      }
  }
  else
  {
      // проверка на наличие в процессах игр
      if (Scan_Game_Ena)
      {
        // Если игры найдены, то просыпаемся поиграть
        if (GameDetected())
        {
          SleepCommand(indx, bk, snd);
          --Pets[indx].Status.Boredom; // скука
          ++Pets[indx].Status.Happiness;
        }
      }
  }

  // питание
  if(!isCharging) ++Pets[indx].Status.Hunger;
  // проверка наличия жратвы и наличие мусора
  int Eat=0;
  if (Pets[indx].Status.Hunger > (int)Pets[indx].Status.MaxHunger/3) Eat=1;
  // ест только когда не спит
  if (Pets[indx].Sleep!=0) Eat=0;
  //если зарядка, то будем кушать
  if(isCharging) Eat=1;

  int findgvn=0;
  int fEat=0;
  wchar_t eat_path[256];
  wchar_t eat_filter[128];
  DIR_HANDLE * dirHandle = 0;
  wchar_t * ext;

  getdir(eat_path, ROOM_PATH);
  wstrcpy(eat_filter,eat_path);
  wstrcat(eat_filter,L"/*.*");

  dirHandle = AllocDirHandle(eat_path);
  if(dirHandle)
  {
    DirHandle_SetFilterStr(dirHandle, eat_filter);
    FILELISTITEM *buf = (FILELISTITEM *)malloc(sizeof(FILELISTITEM));
    FILELISTITEM *item = GetFname(dirHandle,buf);
    FSTAT _fstat;
    while(item)
    {
      ext = getFileExtention(item->fname);
      fstat(item->path,item->fname,&_fstat);

      chmod(item->path,item->fname, 0x1FF);

      if(wstrncmp(ext,L"gvn",3) == 0)
      {
        int err = 0;
        if((findgvn < BATHE_THRESHOLD) && (Pets[indx].Bathe)) FileDelete(item->path,item->fname,&err);
        ++findgvn;
      }
      else
      {
        //кормимся
        if (Eat)
        if (Pets[indx].Status.Hunger > 0)
        // Если зарядка включена, то вместо файлов питаемся от зарядки.
        if(isCharging)
        {
          Pets[indx].Status.Hunger -= 5;
          fEat=1;
          Eat = 0; // при зарядке едим один раз за цикл
        }
        else
        {
          if (_fstat.fsize < (Pets[indx].Status.Age+1+5)*1024)
          {
            int in = (int)_fstat.fsize/1024;
            if (in == 0) in = 1;
            Pets[indx].Status.Hunger -= in;
            // файл перименовываем в мусор
            wchar_t name2[256];
            wstrcpy(name2,item->fname);
            wstrcat(name2,L".gvn");
            rename(item->path, item->fname, item->path, name2, 0);
            fEat=1;
          }
        }
      }
      item = GetFname(dirHandle,buf);
    }
    if(fEat && snd) PlayFileV(sound_path, SoundName[OHDEER_SND], sndVolume);
    mfree(buf);
    DestroyDirHandle(dirHandle);
  }
  else
  {
    if(snd) PlayFileV(sound_path, SoundName[OHDEER_SND], sndVolume);
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGROOMERR,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  }
  // голод
  if (Pets[indx].Status.Hunger > 2*Pets[indx].Status.MaxHunger/3)
  {
    if(!isCharging) --Pets[indx].Status.Happiness;
    if (Pets[indx].Sleep!=0) HungerSleep(indx, bk, snd);
  }
  // сытый
  if (Pets[indx].Status.Hunger==0) ++Pets[indx].Status.Happiness;

  // чистота
  Pets[indx].Status.Dirtiness = findgvn;
  if (Pets[indx].Status.Dirtiness == 0) Pets[indx].Bathe = 0;
  if (Pets[indx].Status.Dirtiness > Pets[indx].Status.MaxDirtiness/2) --Pets[indx].Status.Health;
  if (Pets[indx].Status.Dirtiness > 2*Pets[indx].Status.MaxDirtiness/3) --Pets[indx].Status.Happiness;

  if (Pets[indx].Status.Behaviour <= Pets[indx].Status.MaxBehaviour/10) --Pets[indx].Status.Health;
  if (Pets[indx].Status.Boredom   >  Pets[indx].Status.MaxBoredom/2) --Pets[indx].Status.Happiness;
  if (Pets[indx].Status.Happiness <= Pets[indx].Status.MaxHappiness/2) --Pets[indx].Status.Health;

  // проверки максимальных - минимальных значений
  VerifyStatus(indx);

  // только для основного питомца
  if(indx == 0)
  {
//    MyBOOK* book = (MyBOOK *)bk;
    if(Pets[0].Status.Age >= PARENT_AGE)
    if(Pets[0].Status.Count == 1)
    {
      if(Pets[0].Sleep == 0)
      if((Pets[0].Status.TimeAge & 0xF) == 0)
      if((Pets[0].Status.Sex == 0) || (Pets[0].Status.Sex == 1))
      {
        if(Pets[0].Status.Sex == 0)
          MessageBox(TextID_Create(Pets[0].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGNEEDFRIEND,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
        else
          MessageBox(TextID_Create(Pets[0].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGNEEDGIRL,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);

        if(snd) PlayFileV(sound_path, SoundName[OW_SND], sndVolume);
        start_vibra();
        Light();
        Pets[0].Status.needFriend = 1;
      }
    }
    else
    {
      Pets[0].Status.needFriend = 0;
    }
  }

  // суицид и смерть
  if ((Pets[indx].Status.Health == 0) ||
      (Pets[indx].Status.Hunger >= Pets[indx].Status.MaxHunger) ||
      (Pets[indx].Status.Happiness == 0))
  {
    Pets[indx].Status.StatusDeath=1;
    if(snd) PlayFileV(sound_path, SoundName[BYEBYE_SND], sndVolume);
    start_vibra();
    Light();
  }

  ChangeStatusImage(indx, bk, snd);

  // звуки и сообщения
  // дисциплина
  if (Pets[indx].Status.StatusDeath !=1)
  if (Pets[indx].Sleep==0)
  if (Pets[indx].Behaviour >= Pets[indx].Status.Behaviour)
  {
    if(snd) PlayFileV(sound_path, SoundName[GLOBAL_SND], sndVolume);
    start_vibra();
    Light();
    Pets[indx].Behaviour=0;
    MessageBox(TextID_Create(Pets[indx].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(SpeakMessage[Random() & mess_mask],ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  }
}

void onTimer_life (u16 timerID, LPARAM lparam)
{
  BOOK *bk = (BOOK *)lparam;
  int snd = (!GetSilent() || sndOnSilent) && sndEnable && (!isNight());

  for(int i=0; i<Pets[0].Status.Count; i++) life_proc(i, bk, snd);

  Timer_ReSet(&timer_life,TMR_SECOND*60*(SpeedLife+1),onTimer_life,lparam);
}

// ----------------------------------------------------------------------------

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
  if(!isKeylocked())
  {
    if (mode==ACTIVE_KEY_STYLE)
    {
      if (key==ACTIVE_KEY)
      {
        if (isKeylocked()) return(0);
        if(MyBOOK * bk = (MyBOOK *)FindBook(isTamagochiBook))
        {
          if(!bk->gui)
          {
            ShowMenu((BOOK *)bk);
            return(-1);
          }
          else
          {
            CloseMenu((BOOK *)bk);
          }
        }
      }
    }
  }
  return(0);
}

// ----------------------------------------------------------------------------
void DrawParams(int y)
{
  GC *GC_DISP=get_DisplayGC ();
  for(int i=0; i<Pets[0].Status.Count; i++)
  {
    GC_PutChar(GC_DISP, Pets[i].x, Pets[i].y - y, Pets[i].img_size, Pets[i].img_size, img_names[Pets[i].Status.ImageStatus]);
    if(Pets[i].simg_status) GC_PutChar(GC_DISP, Pets[i].x + Pets[i].img_size - StatSize, Pets[i].y + Pets[i].img_size - StatSize - y, StatSize, StatSize, img_status[Pets[i].simg_status]);
  }
//  if(ImageID != NOIMAGE) DrawString(ImageID + 0x78000000,0,50,50,150,150,1,1,clGreen,clRed);
//  if(ImageID != NOIMAGE) GC_PutChar(GC_DISP, 150, 150, 0, 0, ImageID);
}

void Display_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
  DrawParams(StatusBarY);
  if(Display_oldReDraw) Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ * d, int a, int b, int c)
{
  if(Status_oldReDraw) Status_oldReDraw(d, a, b, c);
//  DrawParams(0);
}

void Soft_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
//  DrawParams(SoftBarY);
  if(Soft_oldReDraw) Soft_oldReDraw(DO,a,b,c);
}

void InitStatus(int indx, int age, int sex)
{
    Pets[indx].Status.TimeAge      = 0;
    Pets[indx].Status.Age          = age;
    Pets[indx].Status.Health       = 100 + (2*age);
    Pets[indx].Status.MaxHealth    = 100 + (2*age);
    Pets[indx].Status.Hunger       = 0;
    Pets[indx].Status.MaxHunger    = 100 + (2*age);
    Pets[indx].Status.Happiness    = 100 + (2*age);
    Pets[indx].Status.MaxHappiness = 100 + (2*age);
    Pets[indx].Status.Dirtiness    = 0;
    Pets[indx].Status.MaxDirtiness = 100 + (2*age);
    Pets[indx].Status.Boredom      = 0;
    Pets[indx].Status.MaxBoredom   = 100 + (2*age);
    Pets[indx].Status.Behaviour    = 100 + (2*age);
    Pets[indx].Status.MaxBehaviour = 100 + (2*age);
    Pets[indx].Status.StatusDeath  = 0;
    Pets[indx].Status.ImageStatus  = SMILE_ICN;
    if(indx == 0)
      wstrncpy(Pets[indx].Status.name,L"Pet",MAXELEMS(Pets[indx].Status.name));
    else
      snwprintf(Pets[indx].Status.name,MAXELEMS(Pets[indx].Status.name)-1,L"Pet%d",indx);
    Pets[indx].Status.Sex = 2;
    Pets[indx].Status.Count = 1;
    if((sex == 0) || (sex == 1)) Pets[indx].Status.Sex = sex;
    Pets[indx].Status.FamilyLife = 0;
    Pets[indx].Status.needFriend = 0;
    Pets[indx].Status.haveFriend = 0;
    Pets[indx].Status.crc = myCrc32((unsigned char *)&Pets[indx].Status, sizeof(TStatusPet) - sizeof(unsigned int));
}

int LoadStatus(int indx)
{
  FSTAT _fstat,_fstat2;
  int f, l;
  wchar_t path[64];
  wchar_t name[64];
  unsigned  char inp[sizeof(TStatusPet)], out[sizeof(TStatusPet)];

  getdir(path,PET_PATH);
  if(indx == 0)
  {
    wstrncpy(name,CFG_NAME,MAXELEMS(name));
  }
  else
  {
    snwprintf(name,MAXELEMS(name)-1,CFG_NAME_FORMAT,indx);
  }

  if (fstat(path,name,&_fstat) != -1)
  {
    if ((f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      InitStatus(indx, 0, 2);
      l = fread(f,&inp, sizeof(TStatusPet));
      fclose(f);

      if(l !=_fstat.fsize)
      {
        return -1;
      }
      unsigned int len = _fstat.fsize - sizeof(unsigned int);
      unsigned int crc;
      memcpy(&crc, &inp[len], sizeof(unsigned int));
      decrypt(out, inp, len, crc);
      if(crc == myCrc32(out, len))
      {
        memcpy(&Pets[indx].Status, out, len);
      }
      else
      {
        if (fstat(TESTPATH,TESTFILE,&_fstat2) != -1)
        {
        }
        else
        {
          memcpy(&Pets[indx].Status, inp, _fstat.fsize);
        }
      }
      return 0;
    }
  }
  return -1;
}

int SaveStatus(int indx)
{
  int f;
  wchar_t path[64];
  wchar_t name[64];
  TStatusPet out;

  if(indx >= MAX_PET_COUNT_GLB) return -1;

  getdir(path,PET_PATH);
  if(indx == 0)
  {
    wstrncpy(name,CFG_NAME,MAXELEMS(name));
  }
  else
  {
    snwprintf(name,MAXELEMS(name)-1,CFG_NAME_FORMAT,indx);
  }
  if ((f=_fopen(path,name,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    unsigned int len = sizeof(TStatusPet) - sizeof(unsigned int);
    Pets[indx].Status.crc = myCrc32((unsigned char *)&Pets[indx].Status, len);
    encrypt(&out, &Pets[indx].Status, len, Pets[indx].Status.crc);
    out.crc = Pets[indx].Status.crc;
    fwrite(f,&out, sizeof(TStatusPet));
    fclose(f);
    return 0;
  }
  return -1;
}

void InitPetPos(int indx)
{
  Pets[indx].x = POS_X + 10 * indx;
  Pets[indx].y = POS_Y + 10 * indx;
  Pets[indx].dx = (Random() > 128) ? 2 : -2;
  Pets[indx].dy = (Random() > 128) ? 3 : -3;
}

void InitPetVar(int indx)
{
  Pets[indx].Fatigue=0; //усталость
  Pets[indx].Sleep=0;
  Pets[indx].Behaviour=0;
  Pets[indx].Bathe=0;

  // для отрисовки смайла и статуса
  Pets[indx].Smile = 0;
  Pets[indx].StatImg = 0;
  Pets[indx].old_img_status  = 99;
  Pets[indx].old_simg_status = 99;
  Pets[indx].simg_status     = 0;
  Pets[indx].img_size        = 0;
  InitPetPos(indx);
}

void CreateTestFile()
{
  FSTAT _fstat;
  int f;
  if (fstat(TESTPATH,TESTFILE,&_fstat) != -1) return;
  if ((f=_fopen(TESTPATH,TESTFILE,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0) fclose(f);
}

void LoadAllPets()
{
  LoadStatus(0);
  if(Pets[0].Status.Count > MAX_PET_COUNT) Pets[0].Status.Count = MAX_PET_COUNT;
  if(Pets[0].Status.Age >= PARENT_AGE)
  {
    for(int i = 1; i < Pets[0].Status.Count; i++)
    {
      if(LoadStatus(i))
      {
        if(i == 1)
        {
          InitStatus(i, 0, Pets[0].Status.Sex ^ 1);
        }
        else
        {
          InitStatus(i, 0, 2);
        }
      }
      InitPetVar(i);
    }
  }
}

// ----------------------------------------------------------------------------
static int MainPageOnCreate(void *, BOOK *bk)
{
  load_icons();
  load_strings();

  if(LoadStatus(0))
  {
    InitStatus(0, 0, 2);
    if(SaveStatus(0))
    {
      MessageBox(TextID_Create(Pets[0].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_ERRSTATUS,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    }
  }
  InitPetVar(0);

  //Если достигли возраста родителя, то пробуем загрузить остальных
  LoadAllPets();
  CreateTestFile();

  if(Pets[0].Status.Count > 1)
  {
    Pets[0].Status.needFriend = 0;
    Pets[0].Status.haveFriend = 1;
    Pets[1].Status.needFriend = 0;
    Pets[1].Status.haveFriend = 1;
  }
  else
    Pets[0].Status.haveFriend = 0;

  FSTAT _fstat;
  int f, sz;
  wchar_t path[256];
  wchar_t name[64];

  //Загрузка списка игр
  extractdir(path, name, GAMELIST_PATH);
  game_list[0] = 0;
  if(fstat(path,name,&_fstat)!= -1)
  {
    if ((f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      sz = fread(f,game_list, GAME_PLAYER_LIST_LEN - 1);
      fclose(f);
      if(sz >= 0) game_list[sz] = 0;
    }
  }

  //Загрузка списка плейеров
  extractdir(path, name, PLAYERLIST_PATH);
  player_list[0] = 0;
  if(fstat(path,name,&_fstat)!= -1)
  {
    if ((f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      sz = fread(f,player_list, GAME_PLAYER_LIST_LEN - 1);
      fclose(f);
      if(sz >= 0) player_list[sz] = 0;
    }
  }

  UpdateName();
  int snd = (!GetSilent() || sndOnSilent) && sndEnable && (!isNight());
  for(int i=0; i<Pets[0].Status.Count; i++) ChangeStatusImage(i, bk, snd);

  if(!GUI_status)
  {
    GUI_status = StatusRow_p();
    Status_desc = DispObject_GetDESC (* GUI_status);
    Status_oldReDraw = DispObject_GetOnRedraw (* GUI_status);
    DISP_DESC_SetOnRedraw (Status_desc, (DISP_OBJ_ONREDRAW_METHOD)Status_ReDraw);
  }
  if(!GUI_soft)
  {
    GUI_soft = DispObject_SoftKeys_Get();
    Soft_desc = DispObject_GetDESC (GUI_soft);
    Soft_oldReDraw = DispObject_GetOnRedraw(GUI_soft);
    DISP_DESC_SetOnRedraw(Soft_desc, (DISP_OBJ_ONREDRAW_METHOD)Soft_ReDraw);
  }
  if(!GUI_display)
  {
    GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
    Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
    Display_desc = DispObject_GetDESC (GUI_display);
    DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
  }

  //запуск жизненного цикла
  Timer_ReSet(&timer_life,TMR_SECOND,onTimer_life,(LPARAM)bk);
  Timer_ReSet(&timer_save,TMR_SECOND*60*5,onTimer_save,0);
  Timer_ReSet(&timer_refresh,REFRESH*TMR_SECOND/10,onTimer_refresh,0);

  return 1;
}

const PAGE_MSG Tamagochi_MainPageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG, MainPageOnCreate,
  NIL_EVENT_TAG,        NULL
};

PAGE_DESC main_page ={"Tamagochi_MainPage",0,Tamagochi_MainPageEvents};

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE * sbm = new REDRAW_RELEASE_MESSAGE;

  // Status
  sbm->SI_OldOnRedraw = Status_oldReDraw;
  sbm->SI_NewOnRedraw = Status_ReDraw;
  // Screen
  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;
  // Soft
  sbm->SK_OldOnRedraw = Soft_oldReDraw;
  sbm->SK_NewOnRedraw = Soft_ReDraw;

  // посылаем всем  мессагу со старыми и своими методами Redraw
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}

int SB_ELF_Killed(void *mess, BOOK* book)
{
  // если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  //Проверим статус
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SI_NewOnRedraw == Status_oldReDraw)
  {
    ms = new REDRAW_RELEASE_MESSAGE;
    memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SI_OldOnRedraw!=EMPTY_REDRAW_METHOD) Status_oldReDraw = sbm->SI_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*GUI_status), Status_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SI_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SI_NewOnRedraw = Status_ReDraw;
  }

  //Проверим главный экран
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Display_oldReDraw = sbm->SB_OldOnRedraw;

    // ставим сdой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SB_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }

  //Проверим софт
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SK_NewOnRedraw == Soft_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) Soft_oldReDraw = sbm->SK_OldOnRedraw;

    // ставим сdой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_soft), Soft_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SK_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SK_NewOnRedraw = Soft_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(BLOCK_EVENT_GLOBALLY);
  }
  return(0);
}

int TerminateElf(void *, BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
#if (!defined(LANG_RU) && !defined(LANG_EN))
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO _T("\n") LTRANSLATED TRANSLATED_BY,0,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, msg->book);
#else
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, msg->book);
#endif
  return(1);
}

static int onReconfigElf(void *mess , BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LG_UPDSETTING,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
    InitConfig();
    load_icons();
    load_strings();
    set_all_old_img_status(99);
    int snd = (!GetSilent() || sndOnSilent) && sndEnable;
    for(int i=0; i < Pets[0].Status.Count; i++) { SaveStatus(i); }

    LoadAllPets();

    for(int i=0; i<Pets[0].Status.Count; i++)
    {
      InitPetPos(i);
      ChangeStatusImage(i, book, snd);
    }
    result=1;
  }
  return(result);
}

int onBcfgConfig(void* mess, BOOK* bk)
{
  int snd = (!GetSilent() || sndOnSilent) && sndEnable;
  EditConfig(0, bk, snd);
  return 1;
}

int onHelloMsg(void* mess, BOOK* book)
{
  HELLO_MSG * msg = (HELLO_MSG*)mess;
  MyBOOK * bk = (MyBOOK *) FindBook(isTamagochiBook);
  if(!wstrcmp(msg->name,LELFNAME))
  {
    if((Pets[0].Status.needFriend == 1) && (Pets[0].Status.Count == 1) && ((Pets[0].Status.Sex == 0) || (Pets[0].Status.Sex == 1)))
    {
      int indx_snd;
      wchar_t  sound_path[128];
      getdir(sound_path,SOUND_PATH);

      if(Pets[0].Status.Sex == 0)
      {
        InitStatus(1,Pets[0].Status.Age+1,1);
        indx_snd = CHEER_F_SND;
      }
      else
      {
        InitStatus(1,Pets[0].Status.Age-1,0);
        indx_snd = CHEER_M_SND;
      }
      InitPetVar(1);
      int snd = (!GetSilent() || sndOnSilent) && sndEnable;
      Pets[0].Status.needFriend = 0;
      Pets[0].Status.haveFriend = 1;
      Pets[1].Status.haveFriend = 1;
      ChangeStatusImage(0, book, snd);
      ChangeStatusImage(1, book, snd);
      Pets[0].Status.Count = 2;

      if(snd) PlayFileV(sound_path, SoundName[indx_snd], sndVolume);
      MessageBox(TextID_Create(Pets[0].Status.name,ENC_UCS2,TEXTID_ANY_LEN), TextID_Create(LG_MSGNEWPET,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);

    }
    else
    {
      MessageBox(EMPTY_TEXTID, TextID_Create(LELFNAME _T("\n") LG_ALREADY_STARTED,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }
  }
  return 1;
}

const PAGE_MSG Tamagochi_BasePageEvents[]@ "DYN_PAGE" ={
  SBY_REDRAW_RELEASE_EVENT,  SB_ELF_Killed,
  ELF_TERMINATE_EVENT,       TerminateElf,
  ELF_SHOW_INFO_EVENT,       ShowAuthorInfo,
  ELF_RECONFIG_EVENT,        onReconfigElf,
  ELF_HELLO_MSG_EVENT,       onHelloMsg,
  NIL_EVENT_TAG,             NULL
};

PAGE_DESC base_page ={"Tamagochi_BasePage",0,Tamagochi_BasePageEvents};

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int isTamagochiBook(BOOK * book)
{
  if(!strncmp(book->xbook->name, myappname, strlen(ELFNAME))) return 1;
  return 0;
}

void onCloseTamagochiBook(BOOK * book)
{
  wchar_t  sound_path[128];
  MyBOOK *bk = (MyBOOK *)book;

  if (bk)
  {
    if(timer_refresh) { Timer_Kill(&timer_refresh); }
    if(timer_save)    { Timer_Kill(&timer_save); }
    if(timer_life)    { Timer_Kill(&timer_life); }

    if(timerScreen)    offTimerScreen(0,0);
    if(offtimerScreen) Timer_Kill(&offtimerScreen);

    if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
    if(Status_desc)  DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
    if(Soft_desc)    DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

    getdir(sound_path,SOUND_PATH);
    if((!GetSilent() || sndOnSilent) && sndEnable) PlayFileV(sound_path, SoundName[BYEBYE_SND], sndVolume);

    free_icons();
    free_strings();

    for(int i=0; i < Pets[0].Status.Count; i++) { SaveStatus(i); }

    if(bk->gui)
    {
      GUIObject_Destroy( bk->gui);
      bk->gui = 0;
    }

    FREE_GUI(bk->stat_list);
    FREE_GUI(bk->cbox_gui);
    FREE_GUI(bk->text_input);
    FREE_GUI(bk->yesno);

    for(int i=0; i < MAX_PET_COUNT_GLB; i++)
    {
      FREE_GUI(bk->menu_list[i]);
    }

    SUBPROC(elf_exit);
  }
}

int main (void)
{
  BOOK *bk;
  DisplayHeight = Display_GetHeight(0);
  DisplayWidth  = Display_GetWidth(0);

  switch(DisplayHeight){
    case 128:
        StatusBarY = STATUS_HEIGHT_128;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_128;
        StatusBarHeight = STATUS_HEIGHT_128;
        SoftBarHeight   = SOFT_HEIGHT_128;
        break;
    case 160:
        StatusBarY = STATUS_HEIGHT_160;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_160;
        StatusBarHeight = STATUS_HEIGHT_160;
        SoftBarHeight   = SOFT_HEIGHT_160;
        break;
    case 208:
        StatusBarY = STATUS_HEIGHT_208;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_208;
        StatusBarHeight = STATUS_HEIGHT_208;
        SoftBarHeight   = SOFT_HEIGHT_208;
        break;
    case 220:
        StatusBarY = STATUS_HEIGHT_220;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_220;
        StatusBarHeight = STATUS_HEIGHT_220;
        SoftBarHeight   = SOFT_HEIGHT_220;
        break;
    case 320:
        StatusBarY = STATUS_HEIGHT_320;
        SoftBarY   = DisplayHeight - SOFT_HEIGHT_320;
        StatusBarHeight = STATUS_HEIGHT_320;
        SoftBarHeight   = SOFT_HEIGHT_320;
        break;
  }

  bk = FindBook(isTamagochiBook);
  if(bk)
  {
    if(CheckEv(bk,ELF_HELLO_MSG_EVENT))
    {
      HELLO_MSG * msg = new HELLO_MSG;
      msg->book = 0;
      snwprintf(msg->name,NAME_LEN - 1,L"%ls",LELFNAME);
      UI_Event_toBookIDwData(ELF_HELLO_MSG_EVENT,BookObj_GetBookID(bk),msg,(void(*)(void*))mfree_adr());
    }
    else
    {
      MessageBox(EMPTY_TEXTID, TextID_Create(LELFNAME _T("\n") LG_ALREADY_STARTED,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }
    SUBPROC(elf_exit);
    return 0;
  }

  InitConfig();
  TamagochiBook = new MyBOOK;
  memset(TamagochiBook,0,sizeof(MyBOOK));
  if(!CreateBook((BOOK*)TamagochiBook,onCloseTamagochiBook,&base_page,myappname,-1,0))
  {
    delete TamagochiBook;
    SUBPROC(elf_exit);
    return 0;
  }
  ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
  if(ENA_HELLO_MSG) MessageBox(EMPTY_TEXTID, TextID_Create(LG_LOADING,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000*msgShowTime, 0);
  BookObj_GotoPage((BOOK *)TamagochiBook,&main_page);
  return 0;
}

/*
Revision history:
  4.1
      + Теперь, если умер, то ничего не просит.
      + The brazilian portuguese version was added.
      + улучшения/исправления/оптимизация
  4.0
      + Изменения в конигурационном файле.
      + Добавлена возможность помыть питомца.
      + Добавлен переход в комнату с едой из меню.
      + Добавлено мигание экраном.
      + улучшения/исправления/оптимизация
  3.0
      + Изменения в конигурационном файле.
      + Переработан алгоритм жизненного цикла :-)
      + улучшения/исправления/оптимизация
  2.0
      + Изменения в конигурационном файле.
      + Переработан код
      + Можно дать имя питомцу (отбражается в меню/статусе)
      + Можно задать/посмотреть пол питомца (отбражается в статусе)
      + Динамическое движение по экрану
      + Изменен алгоритм засыпания.
      + Загрузка внешнего списка игр/плейеров, размером не более 1кбайта (один раз при старте)
      + Просыпается если включается музыка/радио/игры
      + улучшения/исправления/оптимизация
  1.2.1
      + Небольшое исправление ошибок и оптимизация.
  1.2
      + Изменения в конигурационном файле.
      + При низкой дисциплине капризничает  (не ложится спать, не лечится)
      + Добавлена вибрация.
      + Добавлена возможность использовать для навигации цифровые клавиши в меню/статусе.
      + Выбор времени показа сообщений.
      + улучшения/исправления/оптимизация
  1.1
      + Изменения в конигурационном файле.
      + Отображение возраста в букменеджере
      + Возожность отключать звуки во время ночного сна (за исключением действий в меню).
      + Если не спит, то при скуке включает музыку (плейер/радио)
      + Кушает если включена зарядка
  1.0
      + Начальная версия
*/

