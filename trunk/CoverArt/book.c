#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#include "Standby.h"
#include "revision.h"
#include "structs.h"
#include "Externals.h"

extern MUSIC nowmusic;
extern bool isInStandby;
extern LABEL labels[10];
extern UI_IMG img;
bool PlayerRunned=false;

int CurrentTime;

BOOK * CABook;
void decoder_Init(wchar_t *path, wchar_t *name);
TRACK_DESC *currenttrack=0;
int event1;
int event2;

void CA_Kill()
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

int CABook_Terminate(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
};

int CABook_ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  wchar_t text[512];
  snwprintf( text, 512, L"CoverArt in Standby v3.56\r\n\r\n© UltraShot\n® IronMaster,\nJoker XT,\n2007KrasH\n\r\nbuild %d\n", BUILD);
  MessageBox( SID_NULL, TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, msg->book ); 
  return(1);
};
//int CABook_StandbyUnfocused(UI_MESSAGE *);
//int CABook_StandbyFocused(UI_MESSAGE *);

void CABook_OnClose(BOOK * book)
{
  if (book)
  {
    void Cover_Free();
    //ModifyUIHook(event1,CABook_StandbyFocused,0);
    //ModifyUIHook(event2,CABook_StandbyUnfocused,0);
    //Удаляем обложку, если она есть
    Cover_Free();
    // Закрываем всё открытое
    Lang_Remove();
    Images_Unload();
    Send_REDRAW_RELEASE();
    Anim_Free();
    Standby_ReturnRedraw();
    SUBPROC(CA_Kill);
  }
};

static int CABook_Reconfig(void *mess ,BOOK *book)
{
  // Переконфигурируемся
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  // Если конфиг наш, перегрузим его
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    if (AllowAnim==true)
    {
      Anim_Free();
    }
    InitConfig();
    Anim_Init();
    CustomLbls();
    InitLbls();
    Images_Unload();
    Images_Load();
    if (AllowAnim==true)
    {
      locTime=AnimUpdateTime;
    }
    else
    {
      locTime=DefUpdateTime;
    }
    result=1;
  }
  return(result);
};
extern bool SoftUsed;
extern bool StatusUsed;
int isCoverArtBook(BOOK *bk)
{
  if (!strcmp(bk->xbook->name,"CoverArt in Standby")) { return 1; }
  else { return 0; }
};

void Send_REDRAW_RELEASE()
{
  // посылаем всем  мессагу со старым и своим методами Redraw
  REDRAW_RELEASE_MESSAGE * sbm= new REDRAW_RELEASE_MESSAGE;
  memset(sbm,0,sizeof(REDRAW_RELEASE_MESSAGE));
  sbm->SB_OldOnRedraw=Standby_DefaultRedraw;
  sbm->SB_NewOnRedraw=Standby_CARedraw;
  if (StatusUsed)
  {
    sbm->SI_OldOnRedraw=StatusIndication_DefaultRedraw;
    sbm->SI_NewOnRedraw=StatusIndication_CARedraw;
  }
  if (SoftUsed)
  {
    sbm->SK_OldOnRedraw=Softkey_DefaultRedraw;
    sbm->SK_NewOnRedraw=Softkey_CARedraw;
  }
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,sbm,(void (*)(void*))mfree_adr());
};

int SB_ELF_Killed(void *mess ,BOOK* book)
{
  // если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
  REDRAW_RELEASE_MESSAGE * sbm=(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE *res=0;
  // его ли метод мы используем в качестве OldRedraw?
  if (sbm->SB_NewOnRedraw==Standby_DefaultRedraw)
  {
    res=new REDRAW_RELEASE_MESSAGE;
    
    // копируем существующие методы в нашу мессагу
    memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    
    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Standby_DefaultRedraw=sbm->SB_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(Standby_DO),Standby_CARedraw);

    // и шлём мессагу снова, чтоб следующие эльфы сделали тоже самое
    res->SB_OldOnRedraw=EMPTY_REDRAW_METHOD;
    res->SB_NewOnRedraw=Standby_CARedraw;
  }
  if (sbm->SI_NewOnRedraw==StatusIndication_DefaultRedraw)
  {
    if (!res)
    {
      res=new REDRAW_RELEASE_MESSAGE;
      memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    }
    if (sbm->SI_OldOnRedraw!=EMPTY_REDRAW_METHOD) StatusIndication_DefaultRedraw=sbm->SI_OldOnRedraw;
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusIndication_DO),StatusIndication_CARedraw);
    res->SI_OldOnRedraw=EMPTY_REDRAW_METHOD;
    res->SI_NewOnRedraw=StatusIndication_CARedraw;
  }
  if (sbm->SK_NewOnRedraw==Softkey_DefaultRedraw)
  {
    if (!res)
    {
      res=new REDRAW_RELEASE_MESSAGE;
      memcpy(res,sbm,sizeof(REDRAW_RELEASE_MESSAGE));
    }
    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) Softkey_DefaultRedraw=sbm->SK_OldOnRedraw;
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(Softkey_DO),Softkey_CARedraw);
    res->SK_OldOnRedraw=EMPTY_REDRAW_METHOD;
    res->SK_NewOnRedraw=Softkey_CARedraw;
  }
  if (res)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,res,(void (*)(void*))mfree_adr());
    return BLOCK_EVENT_GLOBALLY;
  }
  return 0;
};

void ReInit()
{
  if (FindBook(get_IsAudioPlayerBook()))
  {
    TRACK_DESC *ntrack=TrackDesc_Get(FindBook(get_IsAudioPlayerBook()));
    if (TrackDesc_Compare(currenttrack,ntrack)==false)
    {
      TrackDesc_Free(currenttrack);
      currenttrack=ntrack;
      decoder_Init(currenttrack->path,currenttrack->name);
      InitLbls();
      nowmusic.hdr.LengthInSeconds=currenttrack->fulltime;
    }
    else
    {
      TrackDesc_Free(ntrack);
    }
  }
};

int CABook_onPlay(void * ,BOOK* bk)
{ 
  ReInit();
  PlayerRunned=true;
#ifdef DEBUG
  MessageBox(SID_NULL,TextID_Create(L"PlayEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
#endif
  return 0;
};

int CABook_onCreate(void *, BOOK *bk)
{
  if (FindBook(get_IsAudioPlayerBook()))
  {
    //PlayerRunned=true;
#ifdef DEBUG
    MessageBox(SID_NULL,TextID_Create(L"CreatedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
#endif
  }
  return 0;
};

int CABook_onDestroy(void *, BOOK *bk)
{
  PlayerRunned=false;
#ifdef DEBUG
  MessageBox(SID_NULL,TextID_Create(L"DestroyedEvent",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
#endif
  return 0;
};

int CABook_onPlayTimer(void *message,BOOK* bk)
{
  if (FindBook(get_IsAudioPlayerBook()))
  {
    if (!currenttrack)
    {
      ReInit();
    }
    PlayerRunned=true;
    PLAY_TIME *pl=(PLAY_TIME*)message;
    CurrentTime=(pl->hour * 60)+ ((pl->min * 60) + pl->sec);
    optimized_InitLbls();
  }
  return 0;
};



int CABook_onPause(void *, BOOK* book)
{
  if (FindBook(get_IsAudioPlayerBook()))
  {
    PlayerRunned=true;
  }
  else
  {
    PlayerRunned=false;
  }
  return 0;
};

const PAGE_MSG CA_PageEvents[] @ "DYN_PAGE" ={
  SBY_REDRAW_RELEASE_EVENT  ,                   SB_ELF_Killed,
  ELF_TERMINATE_EVENT ,                         CABook_Terminate,
  ELF_SHOW_INFO_EVENT  ,                        CABook_ShowAuthorInfo,
  ELF_RECONFIG_EVENT,                           CABook_Reconfig,
  UI_MEDIAPLAYER_PLAY_PRESSED_EVENT_TAG,        CABook_onPlay,
  UI_MEDIAPLAYER_AUDIO_PLAYING_TIME_EVENT_TAG,  CABook_onPlayTimer,
  UI_MEDIAPLAYER_CREATED_EVENT_TAG,             CABook_onCreate,
  UI_MEDIAPLAYER_DESTROYED_EVENT_TAG,           CABook_onDestroy,
  UI_MEDIAPLAYER_PAUSE_PRESSED_EVENT_TAG,       CABook_onPause,
  NIL_EVENT_TAG,                                NULL
};

PAGE_DESC base_page ={"CoverArtpage",
                      0,
                      CA_PageEvents};

int CheckStandby()
{
  if (Display_GetTopBook(0)==Find_StandbyBook())
  {
    return 1;
  }
  return 0;
}
/*
int CABook_StandbyUnfocused(UI_MESSAGE *)
{
  isInStandby=CheckStandby();
  return 0;
};

int CABook_StandbyFocused(UI_MESSAGE *)
{
  //Тел на ГЭ
  isInStandby=CheckStandby();
  return 0;
};
*/
BOOK * CreateCABook()
{
  CABook= new BOOK;
  memset(CABook,0,sizeof(BOOK));
  event1=STANDBY_IDLE_EVENT;
  event2=STANDBY_NOT_IDLE_EVENT;
  switch (GetChipID()&CHIPID_MASK)
  {
    case CHIPID_DB2000:
    case CHIPID_DB2010:
      event1=PHONE_IN_STBY_EVENT;
      event2=PREVIOUS_IN_STBY_IDLE_EVENT;
      break;
    case CHIPID_DB2020:
      event1=STANDBY_IDLE_EVENT;
      event2=STANDBY_NOT_IDLE_EVENT;
      break;
  }
  //ModifyUIHook(event1,CABook_StandbyFocused,1);
  //ModifyUIHook(event2,CABook_StandbyUnfocused,1);
  CreateBook(CABook,CABook_OnClose,&base_page,"CoverArt in Standby",-1,0);
  return(CABook);
};
