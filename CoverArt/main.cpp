#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "revision.h"
#include "book.h"
#include "Standby.h"
#include "conf_loader.h"
#include "config_data.h"
#include "Externals.h"
#include "string.h"
#include "structs.h"
#include "decoder.h"

u16 StartupTimer;

int StatusSize=18;
int SoftSize=26;

wchar_t extFolder[512];
wchar_t intFolder[512];
int YSize;

wchar_t *checkfile(wchar_t *name)
{
  FSTAT fst;
  if (fstat(extFolder,name,&fst)==0)
  {
    return extFolder;
  }
  else if (fstat(intFolder,name,&fst)==0)
  {
    return intFolder;
  }
  return intFolder;
};


void GetSize()
{
  // Получаем размер статуса и софтов
  int a=Display_GetHeight(0);
  switch (a)
  {
  case 320:
    StatusSize=24;
    SoftSize=30;
    break;
  case 220:
    StatusSize=18;
    SoftSize=26;
    break;
  case 160:
    StatusSize=14;
    SoftSize=18;
    break;
  default:
    StatusSize=18;
    SoftSize=26;
    break;
  }
};

int Startup()
{
  // Функция запуска кавера
  Lang_Initialize();
  Anim_Init();
  CustomLbls();
  CreateCABook();
  Images_Load();
  GetSize();
  Standby_SetOnRedraw();
  if (AllowAnim)
  {
    locTime=AnimUpdateTime;
  }
  else
  {
    locTime=DefUpdateTime;
  }
  nowmusic.meta.HasImage=false;
  return 0;
};

void Startup_OnTimer(u16 timer,LPARAM lparam)
{
  if (StartupMode==1)
  {
    Startup();
    Timer_Kill(&StartupTimer);
  }
  else if (StartupMode==2)
  {
    if (FindBook(get_IsAudioPlayerBook()))
    {
      Startup();
      Timer_Kill(&StartupTimer);
    }
    else
    {
      Timer_ReSet(&StartupTimer,1000,Startup_OnTimer,0);
    }
  }
};

int check(int smth)
{
  if (smth!=0xFFFFFFFF && smth!=0)
  {
    return 1;
  }
  return 0;
};
int checkevents()
{
  int res=0;
  res=res+check(UI_MEDIAPLAYER_PLAY_PRESSED_EVENT);
  res=res+check(UI_MEDIAPLAYER_AUDIO_PLAYING_TIME_EVENT);
  res=res+check(UI_MEDIAPLAYER_CREATED_EVENT);
  res=res+check(UI_MEDIAPLAYER_DESTROYED_EVENT);
  res=res+check(UI_MEDIAPLAYER_PAUSE_PRESSED_EVENT);
  if (res<5)
  {
    return 0;
  }
  return 1;
};
//#define launchDEBUG
int main()
{   
#ifdef launchDEBUG
  TRACK_DESC *ntrack=TrackDesc_Get(FindBook(get_IsAudioPlayerBook()));
  wchar_t hz[500];
  snwprintf(hz,499,L"%ls/%ls\n\n%d:%d:%d",ntrack->path, ntrack->name, ntrack->fulltime, ntrack->pos, ntrack->tracks_count);
  MessageBox(SID_NULL,TextID_Create(hz,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
  return 0;
#else
  if (!checkevents())
  {
    MessageBox(SID_NULL,TextID_Create(lngAbsentConsts,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
    SUBPROC(CA_Kill);
  }
  if (FindBook(isCoverArtBook))
  {
    // Уже запущен кавер
    MessageBox(SID_NULL,TextID_Create(lngRunned,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
    SUBPROC(CA_Kill);
  }
  else
  {
    if (GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG))
    {
      wstrcpy(extFolder,GetDir(MEM_EXTERNAL+DIR_ELFS_CONFIG));
      wstrcat(extFolder,L"/CoverArt/");
    }
    else
    {
      wstrcpy(extFolder,GetDir(MEM_INTERNAL+DIR_ELFS_CONFIG));
      wstrcat(extFolder,L"/CoverArt/");
    }
    wstrcpy(intFolder,GetDir(MEM_INTERNAL+DIR_ELFS_CONFIG));
    wstrcat(intFolder,L"/CoverArt/");
    YSize=Display_GetHeight(0);
    InitConfig();
    switch (StartupMode)
    {
      case 0:
        Startup();
        break;
      case 1:
        StartupTimer=Timer_Set(60000, Startup_OnTimer, 0);
        break;
      case 2:
        StartupTimer=Timer_Set(1000, Startup_OnTimer, 0);
        break;
    }
  }
  return 0;
#endif
};
