#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177
#ifndef ENG
  #define TURN_ON_MSG "Включать"
  #define CTRL_BUTTOM_MSG "Кнопка управления"
  #define HEADSET_MSG "Гарнитура"
  #define START_WHEN_CONNECTED_MSG "Включать при подключении"
  #define STOP_WHEN_DISCONNECTED_MSG "Выключать при отключении"
  #define START_MSG "Включение кнопкой"
  #define SHORT_PRESS_MSG "Короткое нажатие"
  #define LONG_PRESS_MSG "Длинное нажатие"
  #define DOUBLE_PRESS_MSG "Двойное нажатие"
  #define RADIO_MSG "Радио"
  #define TURN_OFF_MSG "Выключение"
  #define NEXT_STATION_MSG "Следующая станция"
  #define PREV_STATION_MSG "Предыдущая станция"
  #define PLAYER_MSG "Плеер"
  #define NEXT_TRACK_MSG "Следующий трек"
  #define PREV_TRACK_MSG "Предыдущий трек"
  #define DOUBLE_PRESS_TIME_MSG "Время на двойное нажатие (мсек)"
#else
  #define TURN_ON_MSG "Turn on"
  #define CTRL_BUTTOM_MSG "Сontrol button"
  #define HEADSET_MSG "Headsfree"
  #define START_WHEN_CONNECTED_MSG "Turn on when connected"
  #define STOP_WHEN_DISCONNECTED_MSG "Turn off when disconnected"
  #define START_MSG "Activate"
  #define SHORT_PRESS_MSG "Short press"
  #define LONG_PRESS_MSG "Long press"
  #define DOUBLE_PRESS_MSG "Double press"
  #define RADIO_MSG "Radio"
  #define TURN_OFF_MSG "Turn off"
  #define NEXT_STATION_MSG "Next station"
  #define PREV_STATION_MSG "Previous station"
  #define PLAYER_MSG "Walkman"
  #define NEXT_TRACK_MSG "Next track"
  #define PREV_TRACK_MSG "Previous track"
  #define DOUBLE_PRESS_TIME_MSG "Double-press (ms)"
#endif

__root const CFG_HDR cfghdr_button={CFG_KEYCODE,CTRL_BUTTOM_MSG,0,0};
__root const int KeyControl=0;
__root const int KeyControlPressMode=3;

__root const CFG_HDR cfghdr_turn_0={CFG_LEVEL,HEADSET_MSG,1,0};
__root const CFG_HDR cfghdr_turn_1={CFG_LEVEL,START_WHEN_CONNECTED_MSG,1,0};
__root const CFG_HDR cfghdr_turnon_={CFG_CHECKBOX,TURN_ON_MSG,0,0};
  __root const int turn_on=0;  
  __root const CFG_HDR cfghdr_turnon={CFG_CBOX,START_WHEN_CONNECTED_MSG,0,2};
  __root const int turn_on_pr=0;
  __root const CFG_CBOX_ITEM cfgcbox_turn_on[2]={PLAYER_MSG,RADIO_MSG};
  __root const CFG_HDR cfghdr_turn_1x={CFG_LEVEL,"",0,0};  
  __root const CFG_HDR cfghdr_turnoff={CFG_CHECKBOX,STOP_WHEN_DISCONNECTED_MSG,0,0};
  __root const int turn_off=0;
__root const CFG_HDR cfghdr_turn_x={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_on={CFG_LEVEL,START_MSG,1,2};
  __root const CFG_HDR cfghdr_playerOn={CFG_CBOX,PLAYER_MSG,0,2};
  __root const int playerOn=0;
  __root const CFG_CBOX_ITEM cfgcbox_playerOn[2]={SHORT_PRESS_MSG,LONG_PRESS_MSG};
  __root const CFG_HDR cfghdr_radioOn={CFG_CBOX,RADIO_MSG,0,2};
  __root const int radioOn=1;
  __root const CFG_CBOX_ITEM cfgcbox_radioOn[2]={SHORT_PRESS_MSG,LONG_PRESS_MSG};
__root const CFG_HDR cfghdr_on_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_radio_0={CFG_LEVEL,RADIO_MSG,1,0};
  __root const CFG_HDR cfghdr_radio_on={CFG_CBOX,TURN_OFF_MSG,0,3};
  __root const int radioOff=2;
  __root const CFG_CBOX_ITEM cfgcbox_radio_on[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

  __root const CFG_HDR cfghdr_radio_next={CFG_CBOX,NEXT_STATION_MSG,0,3};
  __root const int radioNext=0;
  __root const CFG_CBOX_ITEM cfgcbox_radio_next[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

  __root const CFG_HDR cfghdr_radio_prev={CFG_CBOX,PREV_STATION_MSG,0,3};
  __root const int radioPrev=1;
  __root const CFG_CBOX_ITEM cfgcbox_radio_prev[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

  __root const CFG_HDR cfghdr_radio_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_player_0={CFG_LEVEL,PLAYER_MSG,1,0};
  __root const CFG_HDR cfghdr_player_on={CFG_CBOX,TURN_OFF_MSG,0,3};
  __root const int playerOff=2;
  __root const CFG_CBOX_ITEM cfgcbox_player_on[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

  __root const CFG_HDR cfghdr_player_next={CFG_CBOX,NEXT_TRACK_MSG,0,3};
  __root const int playerNext=0;
  __root const CFG_CBOX_ITEM cfgcbox_player_next[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

  __root const CFG_HDR cfghdr_player_prev={CFG_CBOX,PREV_TRACK_MSG,0,3};
  __root const int playerPrev=1;
  __root const CFG_CBOX_ITEM cfgcbox_player_prev[3]={SHORT_PRESS_MSG,LONG_PRESS_MSG,DOUBLE_PRESS_MSG};

__root const CFG_HDR cfghdr_player_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr7={CFG_UINT,DOUBLE_PRESS_TIME_MSG,10,10000};
__root const unsigned int detectTime=1000;

