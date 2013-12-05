#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0={CFG_LEVEL,"Запись радио",1,0};


__root const CFG_HDR cfghdr1={CFG_CBOX,"Тип",0,3};
__root const int FmRadioRecorderType=0;
__root const CFG_CBOX_ITEM cfgcbox0[3]={"Как в MMS","Диктофон","Как в TrackID"};

__root const CFG_HDR cfghdr2={CFG_CBOX,"Стиль",0,2};
__root const int FmStyle=0;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"Окно","На весь экран"};

__root const CFG_HDR cfghdr3={CFG_UTF16_STRING,"Заголовок",0,39};
__root const wchar_t _FmTitle[40]=L"Запись радио";

__root const CFG_HDR cfghdr4={CFG_CBOX,"Ограничение",0,3};
__root const int LimitRadioType=0;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"Нет","По времени","По размеру"};

__root const CFG_HDR cfghdr5={CFG_TIME,"Время",0,0};
__root const TIME FmTIME={0,1,0};

__root const CFG_HDR cfghdr6={CFG_UINT,"Размер (Кб)",0,0xFFFFFF};
__root const unsigned int FmSize=30;

__root const CFG_HDR cfghdr7={CFG_CHECKBOX,"Запись в свой файл",0,0};
__root const int RecordFmToFile=1;

__root const CFG_HDR cfghdr8={CFG_UTF16_STRING,"Папка",0,199};
__root const wchar_t FmFolder[200]=L"/usb/other";

__root const CFG_HDR cfghdr9={CFG_UTF16_STRING,"Имя",0,199};
__root const wchar_t FmName[200]=L"Radio.amr";

__root const CFG_HDR cfghdr10={CFG_CHECKBOX,"Добавить пункт в меню Радио",0,0};
__root const int Hook=1;

__root const CFG_HDR cfghdr11={CFG_UTF16_STRING,"Название пункта",0,39};
__root const wchar_t _ItemText[40]=L"Записать";

__root const CFG_HDR cfghdr12={CFG_KEYCODE,"Клавиша",0,0};
__root const int KEY1=0;
__root const int KEYMODE1=0;


__root const CFG_HDR cfghdr13={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr14={CFG_LEVEL,"Диктофон",1,0};


__root const CFG_HDR cfghdr15={CFG_CBOX,"Тип",0,3};
__root const int UserSoundRecorderType=0;
__root const CFG_CBOX_ITEM cfgcbox3[3]={"Как в MMS","Диктофон","Как в TrackID"};

__root const CFG_HDR cfghdr16={CFG_CBOX,"Стиль",0,2};
__root const int UserStyle=0;
__root const CFG_CBOX_ITEM cfgcbox4[2]={"Окно","На весь экран"};

__root const CFG_HDR cfghdr17={CFG_UTF16_STRING,"Заголовок",0,39};
__root const wchar_t _UserTitle[40]=L"Звукозапись";

__root const CFG_HDR cfghdr18={CFG_CBOX,"Ограничение",0,3};
__root const int LimitSoundType=0;
__root const CFG_CBOX_ITEM cfgcbox5[3]={"Нет","По времени","По размеру"};

__root const CFG_HDR cfghdr19={CFG_TIME,"Время",0,0};
__root const TIME UserTIME={0,1,0};

__root const CFG_HDR cfghdr20={CFG_UINT,"Размер (Кб)",0,0xFFFFFF};
__root const unsigned int UserSize=30;

__root const CFG_HDR cfghdr21={CFG_CHECKBOX,"Запись в свой файл",0,0};
__root const int RecordUserToFile=1;

__root const CFG_HDR cfghdr22={CFG_UTF16_STRING,"Папка",0,199};
__root const wchar_t UserFolder[200]=L"/usb/other";

__root const CFG_HDR cfghdr23={CFG_UTF16_STRING,"Имя",0,199};
__root const wchar_t UserName[200]=L"Sound.amr";

__root const CFG_HDR cfghdr24={CFG_KEYCODE,"Клавиша",0,0};
__root const int KEY2=0;
__root const int KEYMODE2=0;


__root const CFG_HDR cfghdr25={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr26={CFG_LEVEL,"Запись разговора",1,0};


__root const CFG_HDR cfghdr27={CFG_KEYCODE,"Клавиша",0,0};
__root const int KEY3=0;
__root const int KEYMODE3=0;

__root const CFG_HDR cfghdr28={CFG_LEVEL,"",0,0};
