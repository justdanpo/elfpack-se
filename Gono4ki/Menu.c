#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Menu.h"
#include "MyFUNC.h"
#include "Game.h"
#include "ErrorIAR.h"
#include "Rec_Check.h"


const PAGE_MSG bk_msglst_gm[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    CreateGame,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_game = {"Game_GuiMenu_Page",0,bk_msglst_gm};

void ShowResults(TABL*t)
{
  int text[60];
  int i, j;
  j=0;
  for(i=0; i<10; i++)
  {
    text[j]=TextID_CreateIntegerID(i+1);
    j++;
    text[j]=STR(". ");
    j++;
    text[j]=TextID_Create(t->name[i], ENC_UCS2, TEXTID_ANY_LEN);
    j++;
    text[j]=STR(" - ");
    j++;
    text[j]=TextID_CreateIntegerID(t->rezultat[i]);
    j++;
    text[j]=STR("\n");
    j++;
  }
  int ID=TextID_Create(text, ENC_TEXTID, 60);
  MessageBox(STR("Результаты"), ID, NOIMAGE, 2, 0, 0);
}

void DestroyInputI()
{
  GUIObject_Destroy(MyBK()->gui1);
  MyBK()->gui1=0;
  GUIObject_Show(MyBK()->gui);
}


void OnEntInputI(BOOK * bk,wchar_t * string,int len)
{
  wstrcpy(((MyBook*)bk)->nast->im->names[ListMenu_GetSelectedItem(((MyBook*)bk)->gui)], string);
  GUIObject_Destroy(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  ImagesDel(bk);
  CheckResurs(MyBK());
  GUIObject_Show(((MyBook*)bk)->gui);
}

void CreateInputI(int x, IMGN*i, MyBook*bk)
{
  if(bk->gui1) GUIObject_Destroy(bk->gui1);
  bk->gui1=0;
  bk->gui1=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputI),
                                       VAR_LONG_BACK_PROC(DestroyInputI),
                                       VAR_BOOK((BOOK*)bk),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(L"Изменить имя:",ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(i->names[x], ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(127),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputI),
                                       0); 
}

void OnEntINM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item<3)
  {
    CreateInputI(item, ((MyBook*)bk)->nast->im, (MyBook*)bk);
  }
  else
  {
    if(MyBK()->nast->im->on) MyBK()->nast->im->on=0;
    else MyBK()->nast->im->on=1;
    GUIObject_Destroy(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    CreateINM((MyBook*)bk, 3);
  }      
}

int SetTitlePunktsINM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    item=GUIonMessage_GetCreatedItemIndex(msg);
    for(int i=0; i<3; i++)
    {
      if(item==i)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(GetNamesOfIMN(i, 0),ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(((MyBook*)isBookX(NameMyElf, 0))->nast->im->names[i],ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"DB_LIST_PICTURE_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
    }
    if(item==3)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Картинки:",ENC_UCS2,TEXTID_ANY_LEN));
      if(MyBK()->nast->im->on)
      {
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Свои",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"CALLS_INCOMING_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      else
      {
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"Из эльфа",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"CALLS_OUTGOING_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
    }
  }
  return 0;
}

void ExitINM( BOOK* bk, GUI* )
{
  GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateNM((MyBook*)bk);
}

void CreateINM(MyBook*bk, int x)
{
  if(bk->gui) GUIObject_Destroy(bk->gui);
  bk->gui=0;
  GUI_LIST*INM=CreateListMenu((BOOK*)bk,0);
  ((MyBook*)bk)->gui=INM;
  GUIObject_SetTitleText(INM, TextID_Create(L"Картинки",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(INM, 4);
  OneOfMany_SetOnMessage(INM,SetTitlePunktsINM);
  ListMenu_SetCursorToItem(INM,x);
  ListMenu_SetItemStyle(INM,3);
  GUIObject_SoftKeys_SetAction(INM,ACTION_BACK,ExitINM);
  GUIObject_SoftKeys_SetAction(INM,ACTION_LONG_BACK,ExitINM);
  GUIObject_SoftKeys_SetAction(INM,ACTION_SELECT1,OnEntINM);
  GUIObject_SoftKeys_SetText(INM,ACTION_SELECT1,STR("Изменить"));
  GUIObject_Show(INM);
}

void ExitMNM( BOOK* bk, GUI* )
{
  GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateNM((MyBook*)bk);
}

void OnMNM1( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  for(int i=0; i<6; i++)
  {
    if(item==i+1)
    {
      if(((MyBook*)bk)->nast->sound->status[i]==0) ((MyBook*)bk)->nast->sound->status[i]=1;
      else ((MyBook*)bk)->nast->sound->status[i]=0;
    }
  }  
  GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  CreateMNM(((MyBook*)bk), item);
}


void DestroyInput()
{
  GUIObject_Destroy(MyBK()->gui1);
  MyBK()->gui1=0;
  GUIObject_Show(MyBK()->gui);
}


void OnEntInput(BOOK * bk,wchar_t * string,int len)
{
  int x=ListMenu_GetSelectedItem(((MyBook*)bk)->gui)-1;
  wstrcpy(((MyBook*)bk)->nast->sound->names[x], string);
  GUIObject_Destroy(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  GUIObject_Show(((MyBook*)bk)->gui);
}




void CreateInput(int x, SOUND*s, MyBook*bk)
{
  if(bk->gui1) GUIObject_Destroy(bk->gui1);
  bk->gui1=0;
  bk->gui1=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInput),
                                       VAR_LONG_BACK_PROC(DestroyInput),
                                       VAR_BOOK((BOOK*)bk),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(L"Изменить:",ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(s->names[x-1], ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(255),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInput),
                                       0); 
}



void OnEntMNM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    if(((MyBook*)bk)->nast->sound->sound==0) ((MyBook*)bk)->nast->sound->sound=1;
    else ((MyBook*)bk)->nast->sound->sound=0;
    GUIObject_Destroy(((MyBook*)bk)->gui);
    ((MyBook*)bk)->gui=0;
    CreateMNM(((MyBook*)bk), item);
  }
  if(item>0)
  {
    CreateInput(item, ((MyBook*)bk)->nast->sound, (MyBook*)bk);
  }
}

int SetTitlePunktsMNM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        iconidname2id(L"DB_LIST_SOUND_IMELODY_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Звук",ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,GetStrokaIMG(StatusSettings(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound),((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound));
      }
      for(int i=0; i<6; i++)
      {
        if(item==i+1)
        {
          GUIonMessage_SetMenuItemText(msg,TextID_Create(NamesOfPunkts(i),ENC_UCS2,TEXTID_ANY_LEN));
          GUIonMessage_SetMenuItemSecondLineText(msg,GetStrokaIMG(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->names[i],((MyBook*)isBookX(NameMyElf, 0))->nast->sound->status[i]));
          iconidname2id(L"DB_LIST_SOUND_EMELODY_ICN", -1, &ID);
          GUIonMessage_SetMenuItemIcon(msg, 0, ID);
        }
      }      
  }
  return(1);        
}

void Set(int item)
{
  if(item==0)
  {
    GUIObject_SoftKeys_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui,1,0);
    GUIObject_SoftKeys_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, 1, TextID_Create(L" ",ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, TextID_Create(StatusSettings1(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->sound),ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntMNM);
    //GUIonMessage_SetMenuItemIcon(msg, 0, IconID->I1);
  }
  for(int i=0; i<6; i++)
  {
    if(item==i+1)
    {
      GUIObject_SoftKeys_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui,1,OnMNM1);
      GUIObject_SoftKeys_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, 1, TextID_Create(StatusSettings1(((MyBook*)isBookX(NameMyElf, 0))->nast->sound->status[i]),ENC_UCS2,TEXTID_ANY_LEN));
      GUIObject_SoftKeys_SetText(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, TextID_Create(L"Изменить",ENC_UCS2,TEXTID_ANY_LEN));
      GUIObject_SoftKeys_SetAction(((MyBook*)isBookX(NameMyElf, 0))->gui, ACTION_SELECT1, OnEntMNM);
      //GUIonMessage_SetMenuItemIcon(msg, 0, IconID->I2);
    }
  }
}

void NewKey1(DISP_OBJ* p, int i1, int i2, int i3, int i4)
{
  ((MyBook*)isBookX(NameMyElf, 0))->OldKey1(p, i1, i2, i3, i4);
  int item=ListMenu_GetSelectedItem(MyBK()->gui);
  Set(item);
}

void CreateMNM(MyBook*bk, int x)
{
  if(bk->gui) GUIObject_Destroy(bk->gui);
  bk->gui=0;
  GUI_LIST*MNM=CreateListMenu((BOOK*)bk,0);
  ((MyBook*)bk)->gui=MNM;
  GUIObject_SetTitleText(MNM, TextID_Create(L"Музыка",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(MNM, 7);
  OneOfMany_SetOnMessage(MNM,SetTitlePunktsMNM);
  ListMenu_SetCursorToItem(MNM,x);
  ListMenu_SetItemStyle(MNM,3);
  GUIObject_SoftKeys_SetAction(MNM,ACTION_BACK,ExitMNM);
  GUIObject_SoftKeys_SetAction(MNM,ACTION_LONG_BACK,ExitMNM);
  Set(x);
  //GUIObject_SoftKeys_SetAction(MNM,ACTION_SELECT1,OnEntMNM);
  bk->OldKey1 = DispObject_GetOnKey( GUIObject_GetDispObject(MNM) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(MNM) ), NewKey1);
  GUIObject_Show(MNM);
}

void ExitNM( BOOK* bk, GUI* )
{
  GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  if(!((MyBook*)bk)->pause) CreateGM(0, bk);
  //CreatePM(bk);
}

void OnEntGameM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  ((MyBook*)bk)->speed=item+1;
  ((MyBook*)bk)->point=0;
  ((MyBook*)bk)->lvl=0;
  BookObj_GotoPage(bk,&bk_game);
  GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;  
}

void OnEntMR( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    ShowResults(((MyBook*)bk)->nast->rez->te);
  }
  if(item==1)
  {
    ShowResults(((MyBook*)bk)->nast->rez->tn);  
  }
  if(item==2)
  {
    ShowResults(((MyBook*)bk)->nast->rez->th);  
  }
}

void OnEntNM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    CreateMNM((MyBook*)bk, 0);
  }
  if(item==1)
  {
    CreateINM((MyBook*)bk, 0);   
  }
}

int SetTitlePunktsNM(GUI_MESSAGE * msg)
{
  int item, ID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Музыка",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"ENTERTAINMENT_MENU_MIDI_COMP_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Картинки",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"ENTERTAINMENT_MENU_PICTUREEDITOR_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
  }
  return(1);        
}

void DeleteRes( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    DELTABL(((MyBook*)bk)->nast->rez->te);
  }
  if(item==1)
  {
    DELTABL(((MyBook*)bk)->nast->rez->tn);  
  }
  if(item==2)
  {
    DELTABL(((MyBook*)bk)->nast->rez->th);  
  }
}

int SetTitlePunktsMR(GUI_MESSAGE * msg)
{
  int item;
  int ID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Легко",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ASHAMED_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Средне",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ANXIOUS_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Тяжело",ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(L"WV_BUDDYLIST_CONTACT_AVAILABLE_ANGRY_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
  }
  return(1);        
}

void MenuResultat(MyBook*bk)
{
  if(bk->gui) GUIObject_Destroy(bk->gui);
  bk->gui=0;
  GUI_LIST*MR=CreateListMenu((BOOK*)bk,0);
  ((MyBook*)bk)->gui=MR;
  GUIObject_SetTitleText(MR, TextID_Create(L"Результаты",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(MR, 3);
  OneOfMany_SetOnMessage(MR,SetTitlePunktsMR);
  ListMenu_SetCursorToItem(MR,0);
  ListMenu_SetItemStyle(MR,0);
  GUIObject_SoftKeys_SetAction(MR,ACTION_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(MR,ACTION_LONG_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(MR,ACTION_SELECT1,OnEntMR);
  GUIObject_SoftKeys_SetAction(MR,1,DeleteRes);
  GUIObject_SoftKeys_SetText(MR,1,STR("Сброс"));
  GUIObject_SoftKeys_SetText(MR,ACTION_SELECT1,STR("Просм."));
  GUIObject_Show(MR);
}

void CreateNM(MyBook*bk)
{
  if(bk->gui) GUIObject_Destroy(bk->gui);
  bk->gui=0;
  GUI_LIST*NM=CreateListMenu((BOOK*)bk,0);
  ((MyBook*)bk)->gui=NM;
  GUIObject_SetTitleText(NM, TextID_Create(L"Настройки",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(NM, 2);
  OneOfMany_SetOnMessage(NM,SetTitlePunktsNM);
  ListMenu_SetCursorToItem(NM,0);
  ListMenu_SetItemStyle(NM,0);
  GUIObject_SoftKeys_SetAction(NM,ACTION_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(NM,ACTION_LONG_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(NM,ACTION_SELECT1,OnEntNM);
  GUIObject_Show(NM);
}
  

void YesPress( BOOK* bk, GUI* )
{
  FreeBook(bk);
}

void NoPress( BOOK* bk, GUI* )
{
  GUIObject_Destroy(((MyBook*)bk)->gui1);
  ((MyBook*)bk)->gui1=0;
  GUIObject_Show(((MyBook*)bk)->gui);
}

void YesNo(MyBook*bk)
{
  bk->gui1=CreateYesNoQuestionVA(0,
                                    VAR_BOOK(bk),
                                    VAR_YESNO_QUESTION(TextID_Create(L"Выйти?", ENC_UCS2, TEXTID_ANY_LEN)),
                                    0);
  GUIObject_SoftKeys_SetAction(bk->gui1, ACTION_YES, YesPress);
  GUIObject_SoftKeys_SetAction(bk->gui1, ACTION_NO,  NoPress);
  GUIObject_SoftKeys_SetAction(bk->gui1, ACTION_BACK, NoPress);
  GUIObject_SoftKeys_SetAction(bk->gui1, ACTION_LONG_BACK, NoPress);
  GUIObject_Show(((MyBook*)bk)->gui1);
}

void ExitGM( BOOK* bk, GUI* )
{
  YesNo((MyBook*)bk);
}



void CreateGameM(MyBook*bk)
{
  if(bk->gui) GUIObject_Destroy(bk->gui);
  bk->gui=0;
  GUI_LIST*GameM=CreateListMenu((BOOK*)bk,0);
  ((MyBook*)bk)->gui=GameM;
  GUIObject_SetTitleText(GameM, TextID_Create(L"Выбор уровня",ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(GameM, 3);
  OneOfMany_SetOnMessage(GameM,SetTitlePunktsMR);
  ListMenu_SetCursorToItem(GameM,0);
  ListMenu_SetItemStyle(GameM,0);
  GUIObject_SoftKeys_SetAction(GameM,ACTION_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(GameM,ACTION_LONG_BACK,ExitNM);
  GUIObject_SoftKeys_SetAction(GameM,ACTION_SELECT1,OnEntGameM);
  GUIObject_Show(GameM);
}

void OnEntGM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(((MyBook*)bk)->gui);
  if(item==0)
  {
    CreateGameM((MyBook*)bk);
  }
  if(item==1)
  {
    CreateNM((MyBook*)bk);    
  }
  if(item==2)
  {
    MenuResultat((MyBook*)bk);
  }
  if(item==3)
  {
    ShowInfo();
  }
  if(item==4)
  {
    ExitGM(bk,0);
  }
}

int SetTitlePunktsGM(GUI_MESSAGE * msg)
{
  int item;
  int ID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        ID=TextID_Create(L"Новая игра",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemText(msg,ID);
        ID=TextID_Create(L"Начать игру!",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemSecondLineText(msg,ID);
        iconidname2id(L"PROFILE_INCAR_LIST_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==1)
      {
        ID=TextID_Create(L"Настройки",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemText(msg,ID);
        ID=TextID_Create(L"Поковыряца",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemSecondLineText(msg,ID);
        iconidname2id(L"MENU_SETTINGS_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==2)
      {
        ID=TextID_Create(L"Результаты",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemText(msg,ID);
        ID=TextID_Create(L"Просмотр результатов",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemSecondLineText(msg,ID);
        iconidname2id(L"MISCELLANEOUS_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==3)
      {
        ID=TextID_Create(L"Инфо",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemText(msg,ID);
        ID=TextID_Create(L"Информация",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemSecondLineText(msg,ID);
        iconidname2id(L"HELP_TEXT_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
      if(item==4)
      {
        ID=TextID_Create(L"Выход",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemText(msg,ID);
        ID=TextID_Create(L"Выйти из игры",ENC_UCS2,TEXTID_ANY_LEN);
        GUIonMessage_SetMenuItemSecondLineText(msg,ID);
        iconidname2id(L"FORBIDDEN_NETWORK_ICN", -1, &ID);
        GUIonMessage_SetMenuItemIcon(msg, 0, ID);
      }
  }
  return(1);        
}

int CreateGM(void*, BOOK*bk)
{
  int ID;
  GUI_LIST*GM=CreateListMenu((BOOK*)bk,0);
  if(((MyBook*)bk)->gui) GUIObject_Destroy(((MyBook*)bk)->gui);
  ((MyBook*)bk)->gui=0;
  ((MyBook*)bk)->gui=GM;
  ID=TextID_Create(L"Gono4ki",ENC_UCS2,TEXTID_ANY_LEN);
  GUIObject_SetTitleText(GM, ID);
  ListMenu_SetItemCount(GM, 5);
  OneOfMany_SetOnMessage(GM,SetTitlePunktsGM);
  ListMenu_SetCursorToItem(GM,0);
  ListMenu_SetItemStyle(GM,3);
  GUIObject_SoftKeys_SetAction(GM,ACTION_BACK,ExitGM);
  GUIObject_SoftKeys_SetAction(GM,ACTION_LONG_BACK,ExitGM);
  GUIObject_SoftKeys_SetAction(GM,ACTION_SELECT1,OnEntGM);
  GUIObject_Show(GM);
  return 0;
}
