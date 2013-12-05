#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"


BOOK * myBook;

typedef struct
{
  char dummy0[0x18];
  GUI * FmGui;
  char dummy1[0x320];
  int ID_33C;
}FM_RADIO_BOOK;


typedef struct
{
  char dummy[0x40C];
  char t[0x10];
  char RecordTimeFlag;
  char RecordOpenFlag;
  u16  _unk;
  int  RadioVal;
  TEXTID RecorderTitle;
  char RecorderStyle;
}SOUND_RECORDER_DESC;


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int SoundRecorderDesc_SetRecordTime(SOUND_RECORDER_DESC * sd,...)
{
  if(!sd) return 0;
  memcpy(sd->t,&sd+sizeof(SOUND_RECORDER_DESC *),0x10);
  sd->RecordTimeFlag=1;
  return 1;
}

int SoundRecorderDesc_SetRadioVal(SOUND_RECORDER_DESC* desc,int value)
{
  if(!desc) return 0;
  desc->RadioVal=value;
  return 1;
}

int SoundRecorderDesc_SetTitle(SOUND_RECORDER_DESC * desc,TEXTID Title)
{
  if(!desc) return 0;
  desc->RecorderTitle = Title;
  return 1;
}

int SoundRecorderDesc_SetStyle(SOUND_RECORDER_DESC * desc,int style)
{
  if(!desc) return 0;
  desc->RecorderStyle = style;
  return 1;
}

int Record_FM(FM_RADIO_BOOK * fm_book)
{
  SOUND_RECORDER_DESC * desc = (SOUND_RECORDER_DESC*)SoundRecorderDesc_Create();
  SoundRecorderDesc_SetBookID(desc,BookObj_GetBookID(myBook));
  SoundRecorderDesc_SetType(desc,1+FmRadioRecorderType);
  if(wstrlen(_FmTitle))SoundRecorderDesc_SetTitle(desc,TextID_Create(_FmTitle,ENC_UCS2,40));
  if(LimitRadioType)
  {
    if(LimitRadioType==1) SoundRecorderDesc_SetRecordTime(desc,(int)FmTIME.hour,(int)FmTIME.min,0,0);
    if(LimitRadioType==2 && FmSize) SoundRecorderDesc_SetRecordSize(desc,FmSize*1000);
  }
  SoundRecorderDesc_SetRadioVal(desc,fm_book->ID_33C);
  if(RecordFmToFile)
  {
    SoundRecorderDesc_SetFolder(desc,FmFolder);
    SoundRecorderDesc_SetFname(desc,FmName);
  }
  SoundRecorderDesc_SetStyle(desc,FmStyle);
  SoundRecorder_Create(desc);
  SoundRecorderDesc_Destroy(desc);
  return 1;
}

 
int Record_User(BOOK * bk)
{
  SOUND_RECORDER_DESC* desc = (SOUND_RECORDER_DESC*)SoundRecorderDesc_Create();
  SoundRecorderDesc_SetBookID(desc,BookObj_GetBookID(myBook));
  SoundRecorderDesc_SetType(desc,1+UserSoundRecorderType);
  if(wstrlen(_UserTitle))SoundRecorderDesc_SetTitle(desc,TextID_Create(_UserTitle,ENC_UCS2,40));
  if(LimitSoundType)
  {
    if(LimitSoundType==1)SoundRecorderDesc_SetRecordTime(desc,(int)UserTIME.hour,(int)UserTIME.min,0,0);
    if(LimitSoundType==2 && UserSize)SoundRecorderDesc_SetRecordSize(desc,UserSize*1000);
  }
  if(RecordUserToFile)
  {
    SoundRecorderDesc_SetFolder(desc,UserFolder);
    SoundRecorderDesc_SetFname(desc,UserName);
  }
  SoundRecorderDesc_SetStyle(desc,UserStyle);
  SoundRecorder_Create(desc);
  SoundRecorderDesc_Destroy(desc);
  return 1;
}
  


static int TerminateElf(void * ,BOOK *book)
{
  FreeBook(book);
  return(1);
}

typedef struct
{
  BOOK * book;
}MSG;

static int ShowAuthorInfo(void *mess ,BOOK *book)
{
  MSG * msg = (MSG *)mess;
  MessageBox(EMPTY_TEXTID,STR("AdvancedSoundRecorder v.1.1\n (c) 2007KrasH\n Respect to SlaveMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}

void RecFM(BOOK * fm,GUI*)
{
  Record_FM((FM_RADIO_BOOK*)fm);
}

static int SetHook(void *mess,BOOK *book)
{
  if(!Hook) return 0;
  FM_RADIO_BOOK * fm_bk = (FM_RADIO_BOOK*)FindBook(get_IsFmRadioBook());
  GUIObject_SoftKeys_SetAction(fm_bk->FmGui,0x11,RecFM);
  GUIObject_SoftKeys_SetText(fm_bk->FmGui,0x11,TextID_Create(_ItemText,ENC_UCS2,40));
  return 1;
}

static int UnsetHook()
{
  if(FindBook(get_IsFmRadioBook()))
  {
    FM_RADIO_BOOK * fm = (FM_RADIO_BOOK*)FindBook(get_IsFmRadioBook());
    GUIObject_SoftKeys_SetAction(fm->FmGui,0x11,0);
    GUIObject_SoftKeys_SetText(fm->FmGui,0x11,0);
    return 1;
  }
  return 0;
}

static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    if(FindBook(get_IsFmRadioBook()))
    {
      if(Hook){
        SetHook(0,0);
      }else{
        UnsetHook();
      }
    }
    result=1;
  }
  return(result);
}

int isOngoingCallBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"OngoingCallBook")) return(1);
  return(0);
}

int NewKey(int key,int r1,int mode,LPARAM,DISP_OBJ*)
{
  if(!isKeylocked())
  {
    if(key==KEY1 && mode==KEYMODE1 && FindBook(get_IsFmRadioBook())!=0)
    {
      Record_FM((FM_RADIO_BOOK*)FindBook(get_IsFmRadioBook()));
    }
    if(key==KEY2 && mode==KEYMODE2)
    {
      Record_User((BOOK*)myBook);
    }
    if(key==KEY3 && mode==KEYMODE3 && FindBook(isOngoingCallBook)!=0)
    {
      SoundRecorder_RecordCall(FindBook(isOngoingCallBook));
    }
  }
  return(0);
}

static void onMyBookClose(BOOK * book)
{
  UnsetHook();
  ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
  SUBPROC(elf_exit);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,          TerminateElf,
  ELF_SHOW_INFO_EVENT,          ShowAuthorInfo,
  ELF_RECONFIG_EVENT,           ReconfigElf,
  UI_FMRADIO_CREATED_EVENT_TAG, SetHook,
  NIL_EVENT_TAG,                NULL
};

const PAGE_DESC bk_base = {"ASR_Base_Page",0,bk_msglst_base};

int main()
{
  BOOK * myBook = new BOOK;
  memset(myBook,0,sizeof(myBook));
  CreateBook(myBook,onMyBookClose,&bk_base,"ASR",-1,0);
  InitConfig();
  ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
  return 0;
}
