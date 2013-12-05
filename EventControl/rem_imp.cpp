/*
*===========================================================================
*                     Модуль напоминальщика
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\lng.h"

#define MBK(a) MyBOOK *mbk=(MyBOOK*)a

extern MyBOOK *ECBook;

int isEvtBook(BOOK *bk);

GUI_REMIND *GUI_REMIND_Create(BOOK *bk);
void GuiRemind_AddNote(GUI_REMIND *g, REMIND *rem);
void kill_rems(LIST *lst, MyBOOK *mbk, bool check);
//#define DELETE(x) if (x)delete x; x=0

/*
*===========================================================================
*                     Создаём напоминальщика...
*===========================================================================
*/
void CreateReminder(REMINDER_STRUCT * rem_str)
{
  MBK(rem_str->bk);
  if (rem_str->vibra)
  {
    PAudioControl pAC = AudioControl_Init();
    if(!pAC)pAC = *GetAudioControlPtr();
    AudioControl_Vibrate(pAC, 500, 50, 3000);
  }
  bool created=false;
  if (mbk->remind)
  {
    if (rem_str->replace)
    {
      kill_rems(mbk->remlst, mbk, 0);
      GUIObject_Destroy(mbk->remind);
      mbk->remind=GUI_REMIND_Create(rem_str->bk);
      created=true;
    }
  }
  if (!mbk->remind)
  {
    mbk->remind=GUI_REMIND_Create(rem_str->bk);
    created=true;
  }
  GUI *gRemind = mbk->remind;
  if (rem_str->text && rem_str->utext && rem_str->time)
  {
    REMIND *rem=new REMIND;
    memset(rem,0,sizeof(REMIND));
    rem->text=new wchar_t[wstrlen(rem_str->text)+1];
    wstrcpy(rem->text, rem_str->text);
    rem->utext=new wchar_t[wstrlen(rem_str->utext)+1];
    wstrcpy(rem->utext,rem_str->utext);
    rem->time=new wchar_t[wstrlen(rem_str->time)+1];
    wstrcpy(rem->time,rem_str->time);
    GuiRemind_AddNote(mbk->remind, rem);
  }
  GUIObject_SetStyle(gRemind, 4);
  GUIObject_SetTitleType(gRemind, 1);
  GUIObject_SoftKeys_Hide(gRemind);
  if (created)
  {
    GUIObject_Show(gRemind);
    BookObj_Show(rem_str->bk, 0);
    BookObj_SetFocus(rem_str->bk, 0);
  }
  if (rem_str->need_to_destroy==true) delete rem_str;
};

