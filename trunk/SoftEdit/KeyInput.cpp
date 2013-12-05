#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"
//в версии 1.0 данная переменная отсутствовала
//1.1 версия:
#define VERSION_ 0x00AAAA01
int isSBook(BOOK *bk);
extern const PAGE_DESC bk_base;
extern const PAGE_DESC bk_main;
extern const PAGE_DESC bk_keycode_input;

void KeyCode_KeyHook(BOOK *bk, int key, int unk, int unk2)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (unk2==KBD_SHORT_PRESS)
  {
    TEXTID sid[2];
    if (mbk->key_stage==0)
    {
      mbk->key_stage=1;
      mbk->curkey->oldkey=key;
    }
    else
    {
      mbk->curkey->newkey=key;
    }  
    sid[0]=TextID_Create(L"New key:\n\n",ENC_UCS2,TEXTID_ANY_LEN);
    if (mbk->curkey->newkey!=0)
    {
      sid[1]=KeyCode2Name(mbk->curkey->newkey);
    }
    else
    {
      sid[1]=TextID_Create(L"-empty-",ENC_UCS2,TEXTID_ANY_LEN);
    }
    Feedback_SetTextExtended(mbk->keyselect,TextID_Create(sid,ENC_TEXTID,2),0);
    Feedback_SetTimeout(mbk->keyselect,3000);
  }
};

void KeyCode_OnClose(BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (!mbk->curkey)return;
  if ((mbk->curkey->oldkey<=0 || mbk->curkey->oldkey>100) || (mbk->curkey->newkey<=0 || mbk->curkey->newkey>100))
  {
    if (mbk->curit)
    {
      if (mbk->curit->keys && mbk->curit->keys->FirstFree>0)
      {
        int old=mbk->curit->keys->FirstFree;
        KEY *k=(KEY*)List_Get(mbk->curit->keys,mbk->curit->keys->FirstFree-1);
        if (k==mbk->curkey)
        {
          delete((KEY*)List_RemoveAt(mbk->curit->keys,mbk->curit->keys->FirstFree-1));
          if ((old-1)==0)
          {
            List_Destroy(mbk->curit->keys);
            mbk->curit->keys=0;
          }
        }
      }
    }
  }
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
  BookObj_CallPage(bk,&bk_main);
};

int KeyCode_OnEnter(void *, BOOK * bk)
{
  SKBOOK *sk=(SKBOOK*)FindBook(isSBook);
  if (sk)
  {
    if (sk->version==VERSION_)
    {
      sk->SetWorkState(false);
    }
  }
  MyBOOK *mbk=(MyBOOK *)bk;
  FREE_GUI(mbk->keyselect);
  TEXTID sid[2];
  int key;
  if (mbk->key_stage==0)
  {
    key=mbk->curkey->oldkey;
    sid[0]=TextID_Create(L"Old key:\n\n",ENC_UCS2,TEXTID_ANY_LEN);
  }
  else
  {
    key=mbk->curkey->newkey;
    sid[0]=TextID_Create(L"NewKey:\n\n",ENC_UCS2,TEXTID_ANY_LEN);
  }
  mbk->keyselect=TextFeedbackWindow(&mbk->book,0);
  if (key!=0)
  {
    sid[1]=KeyCode2Name(key);
  }
  else
  {
    sid[1]=TextID_Create(L"-empty-",ENC_UCS2,TEXTID_ANY_LEN);
  }
  Feedback_SetTextExtended(mbk->keyselect,TextID_Create(sid,ENC_TEXTID,2),0);
  GUIObject_SetStyle(mbk->keyselect,1);
  Feedback_SetKeyHook(mbk->keyselect,KeyCode_KeyHook);
  Feedback_SetOnClose(mbk->keyselect,KeyCode_OnClose);
  GUIObject_Show(mbk->keyselect);
  return (1);
};

int KeyCode_OnExit(void *, BOOK * bk)
{
  SKBOOK *sk=(SKBOOK*)FindBook(isSBook);
  if (sk)
  {
    if (sk->version==VERSION_)
    {
      sk->SetWorkState(true);
    }
  }
  MyBOOK *mbk=(MyBOOK *)bk;
  mbk->curkey=0;
  FREE_GUI(mbk->keyselect);
  return (1);
};

const PAGE_MSG bk_msglst_keycodeinput[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,            KeyCode_OnEnter,
  PAGE_EXIT_EVENT_TAG,             KeyCode_OnExit,
  FLIP_OPEN_EVENT_TAG,             NULL,
  PHF_PRESSED_EVENT_TAG,           NULL,
  CAMERA_LENSCOVER_OPEN_EVENT_TAG, NULL,
  CAMERA_LENSCOVER_CLOSE_EVENT_TAG,NULL,
  MEDIAPLAYERKEY_PRESSED_EVENT_TAG,NULL,
  GOMUSICKEY_PRESSED_EVENT_TAG,    NULL,
  MULTIKEY_PRESSED_EVENT_TAG,      NULL,
  LAMPKEY_PRESSED_EVENT_TAG,       NULL,
  ONOFFKEY_SHORT_PRESS_EVENT_TAG,  NULL,
  ONOFFKEY_LONG_PRESS_EVENT_TAG,   NULL,
  LOCKKEY_EVENT_TAG,               NULL,
  OPERATORKEY_PRESSED_EVENT_TAG,   NULL,
  NIL_EVENT_TAG,                   NULL
};

const PAGE_DESC bk_keycode_input = {"SoftEdit_KeyInput",0,bk_msglst_keycodeinput};
