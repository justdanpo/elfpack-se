/*
*===========================================================================
*                       Модуль MessageBox'а
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\lng.h"
extern MyBOOK *ECBook;
#define MBK(a) MyBOOK *mbk=(MyBOOK*)a;
/*
*===========================================================================
*                  Обработка автоматического закрытия мессагбокса..
*===========================================================================
*/
void mbox_OnClose(BOOK *bk)
{
  MBK(bk);
  GUIObject_Destroy(mbk->g_mbox);
  mbk->g_mbox=0;
  if (bk==&ECBook->bk)
    BookObj_Hide(bk, 0);
};

/*
*===========================================================================
*                          Обработка нажатия ОК
*===========================================================================
*/
void mbox_onOK(BOOK *bk, GUI* )
{
  MBK(bk);
  GUIObject_Destroy(mbk->g_mbox);
  mbk->g_mbox=0;
  if (bk==&ECBook->bk)
    BookObj_Hide(bk, 0);
};

/*
*===========================================================================
*                     Создаём мессагбокс...
*===========================================================================
*/
void mbox_Create(BOOK * bk, wchar_t *text, int minutes, bool vibra)
{
  if (vibra)
  {
    PAudioControl pAC = AudioControl_Init();
    if( !pAC )pAC = *GetAudioControlPtr();
    AudioControl_Vibrate(pAC, 500, 50, 3000);
  }
  if (!bk)bk=&ECBook->bk;
  MBK(bk);
  bool created=false;
  if (!mbk->g_mbox)
  {
    mbk->g_mbox=TextFeedbackWindow(bk, 0);
    created=true;
  }
  GUI_FEEDBACK *gMBox=mbk->g_mbox;
  Feedback_SetTextExtended(gMBox,TextID_Create(text, ENC_UCS2, TEXTID_ANY_LEN),0);
  if (minutes)
  {
    int strids[2];
    int min=minutes;
    if (min<0)min=-min;
    if (minutes<0)
    {
      strids[0]=TextID_CreateIntegerID(min);
      strids[1]=TextID_Create(lng[LNG_MIN_REMAIN], ENC_UCS2, TEXTID_ANY_LEN);
    }
    else
    {
      strids[0]=TextID_CreateIntegerID(min);
      strids[1]=TextID_Create(lng[LNG_MIN_AGO], ENC_UCS2, TEXTID_ANY_LEN);
    }
    Feedback_SetTextExtended(gMBox,TextID_Create(strids,ENC_TEXTID,2),1);
  }
  Feedback_SetOnClose(gMBox, mbox_OnClose);
  GUIObject_SoftKeys_SetAction(gMBox,ACTION_OK,mbox_onOK);
  GUIObject_SoftKeys_SetText(gMBox,ACTION_OK,TextID_Create(L"OK",ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(gMBox,ACTION_BACK,mbox_onOK);
  GUIObject_SetStyle(gMBox, 4);
  GUIObject_SetTitleType(gMBox, 1);
  GUIObject_SoftKeys_Hide(gMBox);
  Feedback_SetTimeout(gMBox, 5000);
  if (created==true)
  {
    GUIObject_Show(gMBox);
    BookObj_Show(bk, 0);
    BookObj_SetFocus(bk, 0);
  }
};

