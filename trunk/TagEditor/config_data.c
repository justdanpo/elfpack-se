#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr_icons_0={CFG_LEVEL,"Иконки",1,0};

__root const CFG_HDR cfghdr1={CFG_UTF16_STRING,"ID3v1",0,127};
  __root const wchar_t  Iconq[128]=L"MESSAGING_MENU_NEW_MESSAGE_ICN";

__root const CFG_HDR cfghdr2={CFG_UTF16_STRING,"ID3v2",0,127};
  __root const wchar_t  Iconw[128]=L"MESSAGING_MENU_NEW_MESSAGE_ICN";

__root const CFG_HDR cfghdr3={CFG_UTF16_STRING,"Инфо",0,127};
  __root const wchar_t  Icone[128]=L"MENU_IMODE_IMENU_ICN";

__root const CFG_HDR cfghdr4={CFG_UTF16_STRING,"Название",0,127};
  __root const wchar_t  Iconr[128]=L"MP_BR_TITLETRACK_ICN";

__root const CFG_HDR cfghdr5={CFG_UTF16_STRING,"Исполнитель",0,127};
  __root const wchar_t  Icont[128]=L"MP_BR_TITLEARTIST_ICN";

__root const CFG_HDR cfghdr6={CFG_UTF16_STRING,"Альбом",0,127};
  __root const wchar_t  Icony[128]=L"MP_BR_TITLEALBUM_ICN";

__root const CFG_HDR cfghdr7={CFG_UTF16_STRING,"Год",0,127};
  __root const wchar_t  Iconu[128]=L"WV_YAHOO_BUDDYLIST_CONTACT_IDLE_ICN";

__root const CFG_HDR cfghdr8={CFG_UTF16_STRING,"Комментарий",0,127};
  __root const wchar_t  Iconi[128]=L"WV_MSN_BUDDYLIST_CONTACT_NEW_MESSAGE_ICN";

__root const CFG_HDR cfghdr9={CFG_UTF16_STRING,"Жанр",0,127};
  __root const wchar_t  Icono[128]=L"WV_MSN_BUDDYLIST_CONTACT_AVAILABLE_ICN";

__root const CFG_HDR cfghdr_icons_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_rename_0={CFG_LEVEL,"Переименовывание",1,0};

  __root const CFG_HDR cfghdr_rename_1={CFG_CHECKBOX,"*.mpt в *.mp3",0,0};
  __root const int MptToMp3=1;
  
  __root const CFG_HDR cfghdr_renameFile_0={CFG_LEVEL,"Имя Mp3",1,0};
  
    __root const CFG_HDR cfghdr_renameF_1={CFG_CHECKBOX,"Переименовать?",0,0};
    __root const int RenameFile=0;
    
    __root const CFG_HDR cfghdr_renameF_2={CFG_CBOX,"Брать из",0,2};
    __root const int FromRename=0;
    __root const CFG_CBOX_ITEM cfgcbox1[2]={"ID3v1","ID3v2"};
    
  __root const CFG_HDR cfghdr_renameFile_x={CFG_LEVEL,"",0,0};
  
__root const CFG_HDR cfghdr_rename_x={CFG_LEVEL,"",0,0};
