#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

#ifndef ENG
  #define BLINK_SPEED_MSG "Скорость миг."
  #define PRESS_CALL_MSG "При нажатии 'вызвать'"
  #define INCOMING_CALL_MSG "При входящем вызове"
  #define CONNECT_MSG "При соединении"
  #define TAKE_A_CALL_MSG "При поднятии трубки"
  #define END_CONVERSATION_MSG "При окончании разг."
  #define AT_MINUTE_END_MSG "В конце минуты"
  #define AT_N_MINUTE_END_MSG "В конце N-ой минуты"
  #define LAMP_ON_MUTE_MSG "Фонарик при выкл. зв."
  #define BLINK_LAMP_MSG "Миг. фонариком"
  #define BLINK_TIME_MSG "Длит. миг.(сек)"
  #define VIBRA_MSG "Вибра"
  #define VIBRA_TIME_MSG "Длит. вибры(мсек)"
  #define BLINK_SCREEN_MSG "Миг. экраном"
  #define BLINK_LED_MSG "Миг. светодиодами"
  #define VIBRA_MSG_OF "В конце какой мин."
  #define VIBRA_TIME_LEAD_MSG "За сколько до окон.(сек)"
  #define OPTIONS_MSG "Настройки..."
  #define LAMP_MSG "Фонарик"
  #define LED_MSG "Светодиоды"
  #define LED_MODE_MSG "Режим мигания"
  #define ONE_LED_MSG "Одним светодиодом"
  #define ALL_LEDS_MSG "Всеми светодиодами"
  #define SCREEN_MSG "Экран"
  #define SCREEN_LEVEL_MSG "Уровень подсветки экрана"
  #define BLINK_MSG "Мигать"
  #define VIBRATE_MSG "Вибрировать"
  #define PHONE_TYPE_MSG "Телефон"
  #define PHONE_TYPE0_MSG "Без светодиодов"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "Другой"
  #define PHONE_W580_MSG "Только для W580..."
  #define LEFT_LED_MSG "Левым светодиодом"
  #define RIGHT_LED_MSG "Правым светодиодом"
  #define COLOR_MSG "Цвет"
  #define BRIGHTNESS_MSG "Яркость"
  #define DELAY_MSG "Задержка включения"
  #define BLINK_ON_MUTE_MSG "Мигать при выкл. зв."
  #define REDLED_MSG "Красный светодиод"
  #define ONTIME_MSG "Время во вкл. сост.(мсек)"
  #define OFFTIME_MSG "Время в выкл. сост.(мсек)"
  #define TIMERS_MSG "Таймеры"
  #define AUTO_REJECT_MSG "Aвтосброс (сек)"
  #define AUTO_MUTE_MSG "Aвтоотключение звука (сек)"
#else // ENG
  #define BLINK_SPEED_MSG "Blink speed"
  #define PRESS_CALL_MSG "On press 'CALL'"
  #define INCOMING_CALL_MSG "Incoming call"
  #define CONNECT_MSG "Connect"
  #define TAKE_A_CALL_MSG "Take a call"
  #define END_CONVERSATION_MSG "End conversation"
  #define AT_MINUTE_END_MSG "At minute end"
  #define AT_N_MINUTE_END_MSG "At N minutes end"
  #define LAMP_ON_MUTE_MSG "Lamp on mute"
  #define BLINK_LAMP_MSG "Blink lamp"
  #define BLINK_TIME_MSG "Blinking time(sec)"
  #define VIBRA_MSG "Vibration"
  #define VIBRA_TIME_MSG "Vibr. time(msec)"
  #define BLINK_SCREEN_MSG "Blink screen"
  #define BLINK_LED_MSG "Blink leds"
  #define VIBRA_MSG_OF "At the end of minutes"
  #define VIBRA_TIME_LEAD_MSG "Lead time(sec)"
  #define OPTIONS_MSG "Options..."
  #define LAMP_MSG "Lamp"
  #define LED_MSG "Leds"
  #define LED_MODE_MSG "Blink mode"
  #define ONE_LED_MSG "One led"
  #define ALL_LEDS_MSG "All leds"
  #define SCREEN_MSG "Screen"
  #define SCREEN_LEVEL_MSG "Backlight max level"
  #define BLINK_MSG "Blink"
  #define VIBRATE_MSG "Vibrate"
  #define PHONE_TYPE_MSG "Phone"
  #define PHONE_TYPE0_MSG "Without leds"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "Other"
  #define PHONE_W580_MSG "For W580 only..."
  #define LEFT_LED_MSG "Left led"
  #define RIGHT_LED_MSG "Right led"
  #define COLOR_MSG "Color"
  #define BRIGHTNESS_MSG "Brightness"
  #define DELAY_MSG "Turn-on delay"
  #define BLINK_ON_MUTE_MSG "Blink on mute"
  #define REDLED_MSG "Red LED"
  #define ONTIME_MSG "On time (ms)"
  #define OFFTIME_MSG "Off time (ms)"
  #define TIMERS_MSG "Timers"
  #define AUTO_REJECT_MSG "Auto-reject (sec)"
  #define AUTO_MUTE_MSG "Auto-mute (sec)"
#endif //ENG

__root const CFG_HDR cfghdr_press_call={CFG_LEVEL,PRESS_CALL_MSG,1,0};
    __root const CFG_HDR cfghdr_lamp_0={CFG_LEVEL,LAMP_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_lamp_0={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_flashon = 0;
	__root const CFG_HDR cfghdr_blink_time_0={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_flashon_time = 1;
    __root const CFG_HDR cfghdr_lamp_0x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_vibra_0={CFG_LEVEL,VIBRA_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_vibra_0={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibraon = 0;
	__root const CFG_HDR cfghdr_vibra_time_0={CFG_UINT,VIBRA_TIME_MSG,50,100000};
	__root const int cfg_vibraon_time = 1000;
    __root const CFG_HDR cfghdr_vibra_0x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_redled_0={CFG_LEVEL,REDLED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_redled_0={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_redledon = 0;
	__root const CFG_HDR cfghdr_blink_redled_time_0={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_redledon_time = 2;
    __root const CFG_HDR cfghdr_redled_0x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_led_0={CFG_LEVEL,LED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_led_0={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_ledon = 0;
	__root const CFG_HDR cfghdr_blink_led_time_0={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_ledon_time = 2;
    __root const CFG_HDR cfghdr_led_0x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_screen_0={CFG_LEVEL,SCREEN_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_screen_0={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_screenon = 0;
	__root const CFG_HDR cfghdr_blink_screen_time_0={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_screenon_time = 2;
    __root const CFG_HDR cfghdr_screen_0x={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_0x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_incoming_call={CFG_LEVEL,INCOMING_CALL_MSG,1,0};
    __root const CFG_HDR cfghdr_lamp_1={CFG_LEVEL,LAMP_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_lamp_1={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_flash = 0;
	__root const CFG_HDR cfghdr_blink_time_1={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_flash_time = 1;
	__root const CFG_HDR cfghdr_13={CFG_CHECKBOX,LAMP_ON_MUTE_MSG,0,0};
	__root const int cfg_flash_silent = 0;
    __root const CFG_HDR cfghdr_lamp_1x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_vibra_1={CFG_LEVEL,VIBRA_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_vibra_1={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibra = 0;
    __root const CFG_HDR cfghdr_vibra_1x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_redled_1={CFG_LEVEL,REDLED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_redled_1={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_redled = 0;
	__root const CFG_HDR cfghdr_blink_redled_time_1={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_redled_time = 2;
	__root const CFG_HDR cfghdr_redled_silent_1={CFG_CHECKBOX,BLINK_ON_MUTE_MSG,0,0};
	__root const int cfg_redled_silent = 0;
    __root const CFG_HDR cfghdr_redled_1x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_led_1={CFG_LEVEL,LED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_led_1={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_led = 0;
	__root const CFG_HDR cfghdr_blink_led_time_1={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_led_time = 2;
	__root const CFG_HDR cfghdr_led_silent_1={CFG_CHECKBOX,BLINK_ON_MUTE_MSG,0,0};
	__root const int cfg_led_silent = 0;
    __root const CFG_HDR cfghdr_led_1x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_screen_1={CFG_LEVEL,SCREEN_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_screen_1={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_screen = 0;
	__root const CFG_HDR cfghdr_blink_screen_time_1={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_screen_time = 2;
	__root const CFG_HDR cfghdr_screen_silent_1={CFG_CHECKBOX,BLINK_ON_MUTE_MSG,0,0};
	__root const int cfg_screen_silent = 0;
    __root const CFG_HDR cfghdr_screen_1x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_timers_1={CFG_LEVEL,TIMERS_MSG,1,0};
	__root const CFG_HDR cfghdr_timers_time_1={CFG_UINT,AUTO_REJECT_MSG,0,100000};
	__root const int cfg_reject_time = 0;
	__root const CFG_HDR cfghdr_timers_time_2={CFG_UINT,AUTO_MUTE_MSG,0,100000};
	__root const int cfg_mute_time = 0;
    __root const CFG_HDR cfghdr_timers_1x={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_1x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_connect={CFG_LEVEL,CONNECT_MSG,1,0};
    __root const CFG_HDR cfghdr_lamp_2={CFG_LEVEL,LAMP_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_lamp_2={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_flashring = 0;
	__root const CFG_HDR cfghdr_blink_time_2={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_flashring_time = 1;
    __root const CFG_HDR cfghdr_lamp_2x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_vibra_2={CFG_LEVEL,VIBRA_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_vibra_2={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibraring = 0;
	__root const CFG_HDR cfghdr_vibra_time_2={CFG_UINT,VIBRA_TIME_MSG,50,100000};
	__root const int cfg_vibraring_time = 1000;
    __root const CFG_HDR cfghdr_vibra_2x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_redled_2={CFG_LEVEL,REDLED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_redled_2={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_redledring = 0;
	__root const CFG_HDR cfghdr_blink_redled_time_2={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_redledring_time = 2;
    __root const CFG_HDR cfghdr_redled_2x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_led_2={CFG_LEVEL,LED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_led_2={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_ledring = 0;
	__root const CFG_HDR cfghdr_blink_led_time_2={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_ledring_time = 2;
    __root const CFG_HDR cfghdr_led_2x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_screen_2={CFG_LEVEL,SCREEN_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_screen_2={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_screenring = 0;
	__root const CFG_HDR cfghdr_blink_screen_time_2={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_screenring_time = 2;
    __root const CFG_HDR cfghdr_screen_2x={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_2x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_take_call={CFG_LEVEL,TAKE_A_CALL_MSG,1,0};
    __root const CFG_HDR cfghdr_lamp_3={CFG_LEVEL,LAMP_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_lamp_3={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_flashcon = 0;
	__root const CFG_HDR cfghdr_blink_time_3={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_flashcon_time = 1;
    __root const CFG_HDR cfghdr_lamp_3x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_vibra_3={CFG_LEVEL,VIBRA_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_vibra_3={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibracon = 0;
	__root const CFG_HDR cfghdr_vibra_time_3={CFG_UINT,VIBRA_TIME_MSG,50,100000};
	__root const int cfg_vibracon_time = 1000;
    __root const CFG_HDR cfghdr_vibra_3x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_redled_3={CFG_LEVEL,REDLED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_redled_3={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_redledcon = 0;
	__root const CFG_HDR cfghdr_blink_redled_time_3={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_redledcon_time = 2;
    __root const CFG_HDR cfghdr_redled_3x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_led_3={CFG_LEVEL,LED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_led_3={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_ledcon = 0;
	__root const CFG_HDR cfghdr_blink_led_time_3={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_ledcon_time = 2;
    __root const CFG_HDR cfghdr_led_3x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_screen_3={CFG_LEVEL,SCREEN_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_screen_3={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_screencon = 0;
	__root const CFG_HDR cfghdr_blink_screen_time_3={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_screencon_time = 2;
    __root const CFG_HDR cfghdr_screen_3x={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_3x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_end_conversation={CFG_LEVEL,END_CONVERSATION_MSG,1,0};
    __root const CFG_HDR cfghdr_lamp_4={CFG_LEVEL,LAMP_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_lamp_4={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_flashend = 0;
	__root const CFG_HDR cfghdr_blink_time_4={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_flashend_time = 1;
    __root const CFG_HDR cfghdr_lamp_4x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_vibra_4={CFG_LEVEL,VIBRA_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_vibra_4={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibraend = 0;
	__root const CFG_HDR cfghdr_vibra_time_4={CFG_UINT,VIBRA_TIME_MSG,50,100000};
	__root const int cfg_vibraend_time = 1000;
    __root const CFG_HDR cfghdr_vibra_4x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_redled_4={CFG_LEVEL,REDLED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_redled_4={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_redledend = 0;
	__root const CFG_HDR cfghdr_blink_redled_time_4={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_redledend_time = 2;
    __root const CFG_HDR cfghdr_redled_4x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_led_4={CFG_LEVEL,LED_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_led_4={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_ledend = 0;
	__root const CFG_HDR cfghdr_blink_led_time_4={CFG_UINT,BLINK_TIME_MSG,1,10000};
	__root const int cfg_ledend_time = 2;
    __root const CFG_HDR cfghdr_led_4x={CFG_LEVEL,"",0,0};
    __root const CFG_HDR cfghdr_screen_4={CFG_LEVEL,SCREEN_MSG,1,0};
	__root const CFG_HDR cfghdr_blink_screen_4={CFG_CHECKBOX,BLINK_MSG,0,0};
	__root const int cfg_screenend = 0;
	__root const CFG_HDR cfghdr_blink_screen_time_4={CFG_UINT,BLINK_TIME_MSG,0,10000};
	__root const int cfg_screenend_time = 2;
    __root const CFG_HDR cfghdr_screen_4x={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_4x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_at_minute_end={CFG_LEVEL,AT_MINUTE_END_MSG,1,0};
	__root const CFG_HDR cfghdr_vibra_5={CFG_CHECKBOX,VIBRATE_MSG,0,0};
	__root const int cfg_vibra_at_minute_end = 0;
	__root const CFG_HDR cfghdr_vibra_time_5={CFG_UINT,VIBRA_TIME_MSG,50,3540};
	__root const int cfg_vibra_at_minute_end_time = 500;
	__root const CFG_HDR cfghdr_53={CFG_UINT,VIBRA_TIME_LEAD_MSG,1,59};
	__root const int cfg_vibra_at_minute_end_lead = 10;
__root const CFG_HDR cfghdr_5x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_at_n_minute_end={CFG_LEVEL,AT_N_MINUTE_END_MSG,1,0};
	__root const CFG_HDR cfghdr_vibra_6={CFG_UINT,VIBRA_MSG_OF,0,60};
	__root const int cfg_vibra_at_n_minute_end = 0;
	__root const CFG_HDR cfghdr_vibra_time_6={CFG_UINT,VIBRA_TIME_MSG,50,3540};
	__root const int cfg_vibra_at_n_minute_end_time = 1000;
	__root const CFG_HDR cfghdr_63={CFG_UINT,VIBRA_TIME_LEAD_MSG,1,59};
	__root const int cfg_vibra_at_n_minute_end_lead = 20;
__root const CFG_HDR cfghdr_6x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_options={CFG_LEVEL,OPTIONS_MSG,1,0};

  __root const CFG_HDR cfghdr_options_1={CFG_LEVEL,LAMP_MSG,1,0};
    __root const CFG_HDR cfghdr_blink_speed={CFG_UINT,BLINK_SPEED_MSG,25,125};
    __root const int cfg_flash_blink_speed = 75;
  __root const CFG_HDR cfghdr_options_2={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_options_9={CFG_LEVEL,REDLED_MSG,1,0};
    __root const CFG_HDR cfghdr_opt_redled_1={CFG_UINT,ONTIME_MSG,1,10000};
    __root const int cfg_redled_ontime = 250;
    __root const CFG_HDR cfghdr_opt_redled_2={CFG_UINT,OFFTIME_MSG,1,10000};
    __root const int cfg_redled_offtime = 250;
  __root const CFG_HDR cfghdr_options_10={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_options_3={CFG_LEVEL,LED_MSG,1,0};
    //Тип телефона
    __root const CFG_HDR cfghdr_phone_type={CFG_CBOX,PHONE_TYPE_MSG,0,3};
    __root const int cfg_phone_type=0;
    __root const CFG_CBOX_ITEM cfgcbox_phone_type[3]={PHONE_TYPE0_MSG,PHONE_TYPE1_MSG,PHONE_TYPE2_MSG};
    //;Как мигать
    // 0 - одним светодиодом, 1 - всеми.
    __root const CFG_HDR cfghdr_opt_led_3={CFG_CBOX,LED_MODE_MSG,0,2};
    __root const int cfg_led_blink_mode = 0;
    __root const CFG_CBOX_ITEM cfgcbox_led_4[2]={ONE_LED_MSG,ALL_LEDS_MSG};
    //Скорость мигания
    __root const CFG_HDR cfghdr_opt_led_5={CFG_UINT,BLINK_SPEED_MSG,1,1000};
    __root const int cfg_led_blink_speed = 100;

    __root const CFG_HDR cfghdr_options_7={CFG_LEVEL,PHONE_W580_MSG,1,0};
      // 0 - левым, 1 - правым, 2 - всеми.
      __root const CFG_HDR cfghdr_opt_led580_3={CFG_CBOX,LED_MODE_MSG,0,3};
      __root const int cfg_led580_blink_mode = 2;
      __root const CFG_CBOX_ITEM cfgcbox_led580_4[3]={LEFT_LED_MSG,RIGHT_LED_MSG,ALL_LEDS_MSG};
      __root const CFG_HDR cfghdr_led580_5={CFG_COLOR_INT,COLOR_MSG,0,0};
      __root const unsigned int cfg_led580_color=0xFFFFFFFF;
    __root const CFG_HDR cfghdr_options_8={CFG_LEVEL,"",0,0};
  __root const CFG_HDR cfghdr_options_4={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_options_5={CFG_LEVEL,SCREEN_MSG,1,0};
    //Скорость мигания
    __root const CFG_HDR cfghdr_screen_blink_speed={CFG_UINT,BLINK_SPEED_MSG,20,1000};
    __root const int cfg_screen_blink_speed = 500;
    __root const CFG_HDR cfghdr_screen_level={CFG_UINT,SCREEN_LEVEL_MSG,0,100};
    __root const int cfg_screen_level = 30;
  __root const CFG_HDR cfghdr_options_6={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_options_x={CFG_LEVEL,"",0,0};

