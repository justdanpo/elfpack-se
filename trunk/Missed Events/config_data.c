#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

#ifndef ENG
  #define CHECK_MSG "Интервал между проверками (сек)"
  #define PRE_TIME_MSG "Время до начала (сек)"
  #define PERIOD_MSG "Между напоминаниями (sec)"
  #define SOUND_MSG "Мелодия"
  #define IS_ENABLED_MSG "Напоминать"
  #define SILENT_MSG "В режиме без звука"
  #define FILE_MSG "Звуковой файл"
  #define VIBRA_MSG "Вибрация"
  #define TIME_MSG "Длительность (сек)"
  #define FLASH_MSG "Мигание фонаря"
  #define FREQ_MSG "Скорость миг. (мсек)"
  #define LED_MSG "Светодиоды"
  #define LED_MODE_MSG "Режим мигания"
  #define ONE_LED_MSG "Одним светодиодом"
  #define ALL_LEDS_MSG "Всеми светодиодами"
  #define TIME_LIMIT_MSG "Огран. по врем."
  #define TIME_LIMIT_IS_ENABLED_MSG "Включено"
  #define ON_MISSED_MSG "Начало"
  #define OFF_MISSED_MSG "Конец"
  #define MODE_MSG "Режим работы..."
  #define MODE2_MSG "Режим"
  #define FOREVER_MSG "Постоянно"
  #define DURING_MSG "Опред. время"
  #define N_TIMES_MSG "N кло-во раз"
  #define DURATION_MSG "Продолжительность"
  #define TIMES_MSG "Настройка проверок..."
  #define SCREEN_MSG "Мигание экрана"
  #define SCREEN_LEVEL_MSG "Уровень подсветки экрана"
  #define PHONE_TYPE_MSG "Телефон"
  #define PHONE_TYPE0_MSG "Без светодиодов"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "Другой"
  #define PHONE_W580_MSG "Только для W580..."
  #define LEFT_LED_MSG "Левым светодиодом"
  #define RIGHT_LED_MSG "Правым светодиодом"
  #define COLOR_MSG "Цвет"
  #define PHONE_OTHER_MSG "Для других телефонов..."
  #define ITEMS_MSG "Чем напоминать"
  #define REDLED_MSG "Красный светодиод"
  #define ONTIME_MSG "Время во вкл. сост.(мсек)"
  #define OFFTIME_MSG "Время в выкл. сост.(мсек)"
#else
  #define CHECK_MSG "Check period (sec)"
  #define PRE_TIME_MSG "Pre Time (sec)"
  #define PERIOD_MSG "Notification period (sec)"
  #define SOUND_MSG "Sound"
  #define IS_ENABLED_MSG "Is enabled"
  #define SILENT_MSG "On silent mode"
  #define FILE_MSG "Notification file"
  #define VIBRA_MSG "Vibration"
  #define TIME_MSG "Duration time (sec)"
  #define FLASH_MSG "Flash"
  #define FREQ_MSG "Blink speed (msec)"
  #define LED_MSG "Leds"
  #define LED_MODE_MSG "Blink mode"
  #define ONE_LED_MSG "One led"
  #define ALL_LEDS_MSG "All leds"
  #define TIME_LIMIT_MSG "Time limit"
  #define TIME_LIMIT_IS_ENABLED_MSG "Is enabled"
  #define ON_MISSED_MSG "On time"
  #define OFF_MISSED_MSG "Off time"
  #define MODE_MSG "Notification mode..."
  #define MODE2_MSG "Mode"
  #define FOREVER_MSG "Forever"
  #define DURING_MSG "During the time"
  #define N_TIMES_MSG "N times"
  #define DURATION_MSG "Duration"
  #define TIMES_MSG "Times..."
  #define SCREEN_MSG "Blink screen"
  #define SCREEN_LEVEL_MSG "Backlight max level"
  #define PHONE_TYPE_MSG "Phone"
  #define PHONE_TYPE0_MSG "Without leds"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "Other"
  #define PHONE_W580_MSG "For W580 only..."
  #define LEFT_LED_MSG "Left led"
  #define RIGHT_LED_MSG "Right led"
  #define COLOR_MSG "Color"
  #define PHONE_OTHER_MSG "For other phone..."
  #define ITEMS_MSG "Items included"
  #define REDLED_MSG "Red LED"
  #define ONTIME_MSG "On time (ms)"
  #define OFFTIME_MSG "Off time (ms)"
#endif

__root const CFG_HDR cfghdr_times_0={CFG_LEVEL,TIMES_MSG,1,0};
  //Время (в сек) между проверками на пропущенные вызовы, SMS, EMAIL, MMS
  __root const CFG_HDR cfghdr_check_period={CFG_UINT,CHECK_MSG,3,1000};
  __root const unsigned int cfg_checkperiod=10;
  
  //Время (в сек) до начала (старта) напоминания после обнаружения пропущенного вызова/сообщения
  __root const CFG_HDR cfghdr_pre_time={CFG_UINT,PRE_TIME_MSG,3,1000};
  __root const unsigned int cfg_pretime=90;
  
  //Время (в сек) между напоминанием
  __root const CFG_HDR cfghdr7={CFG_UINT,PERIOD_MSG,30,1000};
  __root const unsigned int cfg_period=60;
__root const CFG_HDR cfghdr_times_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_music_0={CFG_LEVEL,SOUND_MSG "...",1,0};
  //Напоминать мелодией (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_music_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_sound=0;
  //Напоминать или нет мелодией если включен режим "без звука" (1 - напоминать, 0 - нет)
  __root const CFG_HDR cfghdr_music_2={CFG_CHECKBOX,SILENT_MSG,0,2};
  __root const int cfg_silent=0;
  //Файл мелодии
  __root const CFG_HDR cfghdr_music_file={CFG_STR_WIN1251,FILE_MSG,0,255};
  __root const char cfg_music_file[256]="%INI_INT%/notific.mp3";
__root const CFG_HDR cfghdr_music_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_vibra_0={CFG_LEVEL,VIBRA_MSG "...",1,0};
  //Напоминать вибрацией (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_vibra_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_vibra=0;
  //Длительность вибры в сек
  __root const CFG_HDR cfghdr_vibra_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_vibra_time=3;
__root const CFG_HDR cfghdr_vibra_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_flash_0={CFG_LEVEL,FLASH_MSG "...",1,0};
  //Напоминать миганием фонарика (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_flash_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_flash=0;
  //;Длительность мигания в сек 
  __root const CFG_HDR cfghdr_flash_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_flash_time=3;
  //Скорость мигания
  __root const CFG_HDR cfghdr_flash_blink_speed={CFG_UINT,FREQ_MSG,25,125};
  __root const int cfg_flash_blink_speed = 75;
__root const CFG_HDR cfghdr_flash_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_redled_0={CFG_LEVEL,REDLED_MSG "...",1,0};
  //Напоминать миганием светодиода (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_redled_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_redled=0;
  //Длительность мигания в сек 
  __root const CFG_HDR cfghdr_redled_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_redled_time=3;
  //Время во вкл. сост.(мсек)
  __root const CFG_HDR cfghdr_opt_redled_3={CFG_UINT,ONTIME_MSG,1,10000};
  __root const int cfg_redled_ontime = 150;
  //Время в выкл. сост.(мсек)
  __root const CFG_HDR cfghdr_opt_redled_4={CFG_UINT,OFFTIME_MSG,1,10000};
  __root const int cfg_redled_offtime = 150;
__root const CFG_HDR cfghdr_redled_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_led_0={CFG_LEVEL,LED_MSG "...",1,0};
  //Напоминать миганием светодиода (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_led_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_led=0;
  //;Длительность мигания в сек 
  __root const CFG_HDR cfghdr_led_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_led_time=3;
  //Скорость мигания
  __root const CFG_HDR cfghdr_led_5={CFG_UINT,FREQ_MSG,1,1000};
  __root const int cfg_led_blink_speed = 100;
  __root const CFG_HDR cfghdr_phone_type={CFG_CBOX,PHONE_TYPE_MSG,0,3};
  __root const int cfg_phone_type=0;
  __root const CFG_CBOX_ITEM cfgcbox_phone_type[3]={PHONE_TYPE0_MSG,PHONE_TYPE1_MSG,PHONE_TYPE2_MSG};
  __root const CFG_HDR cfghdr_options_1={CFG_LEVEL,PHONE_W580_MSG,1,0};
      // 0 - левым, 1 - правым, 2 - всеми.
      __root const CFG_HDR cfghdr_opt_led580_3={CFG_CBOX,LED_MODE_MSG,0,3};
      __root const int cfg_led580_blink_mode = 2;
      __root const CFG_CBOX_ITEM cfgcbox_led580_4[3]={LEFT_LED_MSG,RIGHT_LED_MSG,ALL_LEDS_MSG};
      __root const CFG_HDR cfghdr_led580_5={CFG_COLOR_INT,COLOR_MSG,0,0};
      __root const unsigned int cfg_led580_color=0xFFFFFFFF;
  __root const CFG_HDR cfghdr_options_2={CFG_LEVEL,"",0,0};
  __root const CFG_HDR cfghdr_options_3={CFG_LEVEL,PHONE_OTHER_MSG,1,0};
      //;Как мигать
      // 0 - одним светодиодом, 1 - всеми.
      __root const CFG_HDR cfghdr_led_3={CFG_CBOX,LED_MODE_MSG,0,2};
      __root const int cfg_led_blink_mode = 0;
      __root const CFG_CBOX_ITEM cfgcbox_led_4[2]={ONE_LED_MSG,ALL_LEDS_MSG};
  __root const CFG_HDR cfghdr_options_4={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_led_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_screen_0={CFG_LEVEL,SCREEN_MSG "...",1,0};
  //Напоминать миганием экрана (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_screen_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_screen=0;
  //Длительность мигания в сек
  __root const CFG_HDR cfghdr_screen_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_screen_time=3;
  //Скорость мигания
  __root const CFG_HDR cfghdr_screen_blink_speed={CFG_UINT,FREQ_MSG,20,1000};
  __root const int cfg_screen_blink_speed = 500;
  __root const CFG_HDR cfghdr_screen_level={CFG_UINT,SCREEN_LEVEL_MSG,0,100};
  __root const int cfg_screen_level = 30;
__root const CFG_HDR cfghdr_screen_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_time_1_0={CFG_LEVEL,TIME_LIMIT_MSG " 1...",1,0};
  //Включение режима ограничения работы по времени (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_time_1_1={CFG_CHECKBOX,TIME_LIMIT_IS_ENABLED_MSG,0,0};
  __root const int cfg_time1_limit=1;
  //Время включения
  __root const CFG_HDR cfghdr_time_1_2={CFG_TIME,ON_MISSED_MSG,0,0};
  __root const TIME cfg_on_time1={6,30,0};
  //Время выключения
  __root const CFG_HDR cfghdr_time_1_3={CFG_TIME,OFF_MISSED_MSG,0,0};
  __root const TIME cfg_off_time1={23,0,0};
  __root const CFG_HDR cfghdr_time_1_4={CFG_LEVEL,ITEMS_MSG,1,0};
    __root const CFG_HDR cfghdr_time_1_5={CFG_CHECKBOX,SOUND_MSG,0,0};
    __root const int cfg_time1_sound=1;
    __root const CFG_HDR cfghdr_time_1_6={CFG_CHECKBOX,VIBRA_MSG,0,0};
    __root const int cfg_time1_vibra=1;
    __root const CFG_HDR cfghdr_time_1_7={CFG_CHECKBOX,FLASH_MSG,0,0};
    __root const int cfg_time1_flash=1;
    __root const CFG_HDR cfghdr_time_1_8={CFG_CHECKBOX,REDLED_MSG,0,0};
    __root const int cfg_time1_redled=1;
    __root const CFG_HDR cfghdr_time_1_9={CFG_CHECKBOX,LED_MSG,0,0};
    __root const int cfg_time1_leds=1;
    __root const CFG_HDR cfghdr_time_1_10={CFG_CHECKBOX,SCREEN_MSG,0,0};
    __root const int cfg_time1_screen=1;
  __root const CFG_HDR cfghdr_time_1_11={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_time_1_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_time_2_0={CFG_LEVEL,TIME_LIMIT_MSG " 2...",1,0};
  //Включение режима ограничения работы по времени (1 - вкл, 0 - выкл)
  __root const CFG_HDR cfghdr_time_2_1={CFG_CHECKBOX,TIME_LIMIT_IS_ENABLED_MSG,0,0};
  __root const int cfg_time2_limit=0;
  //Время включения
  __root const CFG_HDR cfghdr_time_2_2={CFG_TIME,ON_MISSED_MSG,0,0};
  __root const TIME cfg_on_time2={23,1,0};
  //Время выключения
  __root const CFG_HDR cfghdr_time_2_3={CFG_TIME,OFF_MISSED_MSG,0,0};
  __root const TIME cfg_off_time2={6,29,0};
  __root const CFG_HDR cfghdr_time_2_4={CFG_LEVEL,ITEMS_MSG,1,0};
    __root const CFG_HDR cfghdr_time_2_5={CFG_CHECKBOX,SOUND_MSG,0,0};
    __root const int cfg_time2_sound=1;
    __root const CFG_HDR cfghdr_time_2_6={CFG_CHECKBOX,VIBRA_MSG,0,0};
    __root const int cfg_time2_vibra=1;
    __root const CFG_HDR cfghdr_time_2_7={CFG_CHECKBOX,FLASH_MSG,0,0};
    __root const int cfg_time2_flash=1;
    __root const CFG_HDR cfghdr_time_2_8={CFG_CHECKBOX,REDLED_MSG,0,0};
    __root const int cfg_time2_redled=1;
    __root const CFG_HDR cfghdr_time_2_9={CFG_CHECKBOX,LED_MSG,0,0};
    __root const int cfg_time2_leds=1;
    __root const CFG_HDR cfghdr_time_2_10={CFG_CHECKBOX,SCREEN_MSG,0,0};
    __root const int cfg_time2_screen=1;
  __root const CFG_HDR cfghdr_time_2_11={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_time_2_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_mode_0={CFG_LEVEL,MODE_MSG,1,0};
  //Режим работы
  __root const CFG_HDR cfghdr_mode_1={CFG_CBOX,MODE2_MSG,0,3};
  __root const int cfg_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox_mode_2[3]={FOREVER_MSG,DURING_MSG,N_TIMES_MSG};
  //Течении какого времени (в мин) или сколько раз напоминать (в зависимости от mode)
  __root const CFG_HDR cfghdr_mode_3={CFG_UINT,DURATION_MSG,1,100000};
  __root const unsigned int cfg_duration=60;
__root const CFG_HDR cfghdr_mode_x={CFG_LEVEL,"",0,0};
