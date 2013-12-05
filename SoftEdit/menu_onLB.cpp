#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"

int isSoftEditBook(BOOK *bk);

int GetStrID(wchar_t *txt)
{
  int id=EMPTY_TEXTID;
  textidname2id(txt,0xFFFFFFFF,&id);
  return id;
};

int GetIconID(wchar_t *txt)
{
  int id=NOIMAGE;
  iconidname2id(txt,0xFFFFFFFF,&id);
  return id;
};

int onLBMessage(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    if (customsofts)
    {
      if (item<customsofts->FirstFree && item>=0)
      {
        ITEM *it=(ITEM *)List_Get(customsofts,item);
        if (it)
        {
          if (it->name)
          {
            GUIonMessage_SetMenuItemText(msg,TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN));
          }
          else
          {
            GUIonMessage_SetMenuItemText(msg,TextID_Create(L"-empty-",ENC_UCS2,TEXTID_ANY_LEN));
          }
          GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"HPB_PHONEBOOK_MENU_ICN"));
        }
      }
      else
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Create",ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"RN_SHORTCUT_LIST_SETTINGS_ICN"));
      }
    }
    else
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Create",ENC_UCS2,TEXTID_ANY_LEN));
      GUIonMessage_SetMenuItemIcon(msg,0,GetIconID(L"RN_SHORTCUT_LIST_SETTINGS_ICN"));
    }
    break;
  }
  return(1);
};

wchar_t *names[]={L"Book name",L"Left softkey string",L"Middle softkey string", L"Right softkey string",L"Use A2 style "};
int onLBMessage2(GUI_MESSAGE * msg)
{
  MyBOOK * myBook = (MyBOOK*)FindBook(isSoftEditBook);
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    bool has=false;
    if (myBook->curit->keys)
    {
      has=true;
    }
    if (item>=0 && item<5)
    {
      GUIonMessage_SetMenuItemText(msg,TextID_Create(names[item],ENC_UCS2,TEXTID_ANY_LEN));
      if (item==0)
      {
        if (myBook->curit->name)
        {
          GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(myBook->curit->name,ENC_UCS2,TEXTID_ANY_LEN));
          if (wstrcmp(myBook->curit->name,L"DEFAULT")==0 ||  wstrcmp(myBook->curit->name,L"StandbyBook")==0)
          {
            GUIonMessage_SetItemDisabled (msg,1);
          }
        }
        else
        {
          GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"-empty-",ENC_UCS2,TEXTID_ANY_LEN));
        }
      }
      else if (item==4)
      {
        int icon_id=NOIMAGE;;
        int ids;
        int id[2];
        if (myBook->curit->style==0)
        {
          iconidname2id(IDN_CHECKBOX_UNCHECKED_ICON,-1,&icon_id);
          id[0] = 0x78000000 + icon_id;
          id[1] = TextID_Create(L"(Emulator)",ENC_UCS2,TEXTID_ANY_LEN);
          ids = TextID_Create(&id[0],ENC_TEXTID,2);
        }
        else if (myBook->curit->style==1)
        {
          iconidname2id(IDN_CHECKBOX_CHECKED_ICON,-1,&icon_id);
          id[0] = 0x78000000 + icon_id;
          id[1] = TextID_Create(L"Full A2",ENC_UCS2,TEXTID_ANY_LEN);
          ids = TextID_Create(&id[0],ENC_TEXTID,2);
        }
        else
        {
          iconidname2id(IDN_CHECKBOX_UNCHECKED_ICON,-1,&icon_id);
          id[0] = 0x78000000 + icon_id;
          id[1] = TextID_Create(L"standart",ENC_UCS2,TEXTID_ANY_LEN);
          ids = TextID_Create(&id[0],ENC_TEXTID,2);
        }
        GUIonMessage_SetMenuItemSecondLineText(msg,ids);
      }
      else
      {
        wchar_t *text=0;
        if (item==1)
        {
          text=myBook->curit->lsi;
        }
        else if (item==2)
        {
          text=myBook->curit->msi;
        }
        else if (item==3)
        {
          text=myBook->curit->rsi;
        }
        if (text)
        {
          GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(text,ENC_UCS2,TEXTID_ANY_LEN));
        }
        else
        {
          GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(L"-empty-",ENC_UCS2,TEXTID_ANY_LEN));
        }
      }
      if (myBook->curit->style==2 && (item==1 || item==2 || item==3))
      {
        GUIonMessage_SetItemDisabled (msg,1);
      }
    }
    else if (item>4)
    {
      if (has==true)
      {
        if (item<myBook->curit->keys->FirstFree+5)
        {
          int ind=item-5;
          KEY *key=(KEY *)List_Get(myBook->curit->keys,ind);
          int sid[3];
          if (key->oldkey!=0)
          {
            sid[0]=KeyCode2Name(key->oldkey);
          }
          else
          {
            sid[0]=TextID_Create(L"empty",ENC_UCS2,TEXTID_ANY_LEN);
          }
          sid[1]=TextID_Create(L"->",ENC_UCS2,TEXTID_ANY_LEN);
          if (key->newkey!=0)
          {
            sid[2]=KeyCode2Name(key->newkey);
          }
          else
          {
            sid[2]=TextID_Create(L"empty",ENC_UCS2,TEXTID_ANY_LEN);
          }
          GUIonMessage_SetMenuItemText(msg,TextID_Create(sid,ENC_TEXTID,3));
        }
        else
        {
          GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Create",ENC_UCS2,TEXTID_ANY_LEN));
        }
      }
      else
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"Create",ENC_UCS2,TEXTID_ANY_LEN));
      }
    }

    break;
  }
  return(1);
};
