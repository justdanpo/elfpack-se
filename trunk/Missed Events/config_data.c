#include "..\\include\cfg_items.h"
#include "config_data.h"

#pragma diag_suppress=Pe177

#ifndef ENG
  #define CHECK_MSG "�������� ����� ���������� (���)"
  #define PRE_TIME_MSG "����� �� ������ (���)"
  #define PERIOD_MSG "����� ������������� (sec)"
  #define SOUND_MSG "�������"
  #define IS_ENABLED_MSG "����������"
  #define SILENT_MSG "� ������ ��� �����"
  #define FILE_MSG "�������� ����"
  #define VIBRA_MSG "��������"
  #define TIME_MSG "������������ (���)"
  #define FLASH_MSG "������� ������"
  #define FREQ_MSG "�������� ���. (����)"
  #define LED_MSG "����������"
  #define LED_MODE_MSG "����� �������"
  #define ONE_LED_MSG "����� �����������"
  #define ALL_LEDS_MSG "����� ������������"
  #define TIME_LIMIT_MSG "�����. �� ����."
  #define TIME_LIMIT_IS_ENABLED_MSG "��������"
  #define ON_MISSED_MSG "������"
  #define OFF_MISSED_MSG "�����"
  #define MODE_MSG "����� ������..."
  #define MODE2_MSG "�����"
  #define FOREVER_MSG "���������"
  #define DURING_MSG "�����. �����"
  #define N_TIMES_MSG "N ���-�� ���"
  #define DURATION_MSG "�����������������"
  #define TIMES_MSG "��������� ��������..."
  #define SCREEN_MSG "������� ������"
  #define SCREEN_LEVEL_MSG "������� ��������� ������"
  #define PHONE_TYPE_MSG "�������"
  #define PHONE_TYPE0_MSG "��� �����������"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "������"
  #define PHONE_W580_MSG "������ ��� W580..."
  #define LEFT_LED_MSG "����� �����������"
  #define RIGHT_LED_MSG "������ �����������"
  #define COLOR_MSG "����"
  #define PHONE_OTHER_MSG "��� ������ ���������..."
  #define ITEMS_MSG "��� ����������"
  #define REDLED_MSG "������� ���������"
  #define ONTIME_MSG "����� �� ���. ����.(����)"
  #define OFFTIME_MSG "����� � ����. ����.(����)"
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
  //����� (� ���) ����� ���������� �� ����������� ������, SMS, EMAIL, MMS
  __root const CFG_HDR cfghdr_check_period={CFG_UINT,CHECK_MSG,3,1000};
  __root const unsigned int cfg_checkperiod=10;
  
  //����� (� ���) �� ������ (������) ����������� ����� ����������� ������������ ������/���������
  __root const CFG_HDR cfghdr_pre_time={CFG_UINT,PRE_TIME_MSG,3,1000};
  __root const unsigned int cfg_pretime=90;
  
  //����� (� ���) ����� ������������
  __root const CFG_HDR cfghdr7={CFG_UINT,PERIOD_MSG,30,1000};
  __root const unsigned int cfg_period=60;
__root const CFG_HDR cfghdr_times_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_music_0={CFG_LEVEL,SOUND_MSG "...",1,0};
  //���������� �������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_music_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_sound=0;
  //���������� ��� ��� �������� ���� ������� ����� "��� �����" (1 - ����������, 0 - ���)
  __root const CFG_HDR cfghdr_music_2={CFG_CHECKBOX,SILENT_MSG,0,2};
  __root const int cfg_silent=0;
  //���� �������
  __root const CFG_HDR cfghdr_music_file={CFG_STR_WIN1251,FILE_MSG,0,255};
  __root const char cfg_music_file[256]="%INI_INT%/notific.mp3";
__root const CFG_HDR cfghdr_music_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_vibra_0={CFG_LEVEL,VIBRA_MSG "...",1,0};
  //���������� ��������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_vibra_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_vibra=0;
  //������������ ����� � ���
  __root const CFG_HDR cfghdr_vibra_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_vibra_time=3;
__root const CFG_HDR cfghdr_vibra_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_flash_0={CFG_LEVEL,FLASH_MSG "...",1,0};
  //���������� �������� �������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_flash_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_flash=0;
  //;������������ ������� � ��� 
  __root const CFG_HDR cfghdr_flash_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_flash_time=3;
  //�������� �������
  __root const CFG_HDR cfghdr_flash_blink_speed={CFG_UINT,FREQ_MSG,25,125};
  __root const int cfg_flash_blink_speed = 75;
__root const CFG_HDR cfghdr_flash_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_redled_0={CFG_LEVEL,REDLED_MSG "...",1,0};
  //���������� �������� ���������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_redled_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_redled=0;
  //������������ ������� � ��� 
  __root const CFG_HDR cfghdr_redled_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_redled_time=3;
  //����� �� ���. ����.(����)
  __root const CFG_HDR cfghdr_opt_redled_3={CFG_UINT,ONTIME_MSG,1,10000};
  __root const int cfg_redled_ontime = 150;
  //����� � ����. ����.(����)
  __root const CFG_HDR cfghdr_opt_redled_4={CFG_UINT,OFFTIME_MSG,1,10000};
  __root const int cfg_redled_offtime = 150;
__root const CFG_HDR cfghdr_redled_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_led_0={CFG_LEVEL,LED_MSG "...",1,0};
  //���������� �������� ���������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_led_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_led=0;
  //;������������ ������� � ��� 
  __root const CFG_HDR cfghdr_led_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_led_time=3;
  //�������� �������
  __root const CFG_HDR cfghdr_led_5={CFG_UINT,FREQ_MSG,1,1000};
  __root const int cfg_led_blink_speed = 100;
  __root const CFG_HDR cfghdr_phone_type={CFG_CBOX,PHONE_TYPE_MSG,0,3};
  __root const int cfg_phone_type=0;
  __root const CFG_CBOX_ITEM cfgcbox_phone_type[3]={PHONE_TYPE0_MSG,PHONE_TYPE1_MSG,PHONE_TYPE2_MSG};
  __root const CFG_HDR cfghdr_options_1={CFG_LEVEL,PHONE_W580_MSG,1,0};
      // 0 - �����, 1 - ������, 2 - �����.
      __root const CFG_HDR cfghdr_opt_led580_3={CFG_CBOX,LED_MODE_MSG,0,3};
      __root const int cfg_led580_blink_mode = 2;
      __root const CFG_CBOX_ITEM cfgcbox_led580_4[3]={LEFT_LED_MSG,RIGHT_LED_MSG,ALL_LEDS_MSG};
      __root const CFG_HDR cfghdr_led580_5={CFG_COLOR_INT,COLOR_MSG,0,0};
      __root const unsigned int cfg_led580_color=0xFFFFFFFF;
  __root const CFG_HDR cfghdr_options_2={CFG_LEVEL,"",0,0};
  __root const CFG_HDR cfghdr_options_3={CFG_LEVEL,PHONE_OTHER_MSG,1,0};
      //;��� ������
      // 0 - ����� �����������, 1 - �����.
      __root const CFG_HDR cfghdr_led_3={CFG_CBOX,LED_MODE_MSG,0,2};
      __root const int cfg_led_blink_mode = 0;
      __root const CFG_CBOX_ITEM cfgcbox_led_4[2]={ONE_LED_MSG,ALL_LEDS_MSG};
  __root const CFG_HDR cfghdr_options_4={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr_led_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_screen_0={CFG_LEVEL,SCREEN_MSG "...",1,0};
  //���������� �������� ������ (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_screen_1={CFG_CHECKBOX,IS_ENABLED_MSG,0,0};
  __root const int cfg_screen=0;
  //������������ ������� � ���
  __root const CFG_HDR cfghdr_screen_2={CFG_UINT,TIME_MSG,1,1000};
  __root const unsigned int cfg_screen_time=3;
  //�������� �������
  __root const CFG_HDR cfghdr_screen_blink_speed={CFG_UINT,FREQ_MSG,20,1000};
  __root const int cfg_screen_blink_speed = 500;
  __root const CFG_HDR cfghdr_screen_level={CFG_UINT,SCREEN_LEVEL_MSG,0,100};
  __root const int cfg_screen_level = 30;
__root const CFG_HDR cfghdr_screen_x={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_time_1_0={CFG_LEVEL,TIME_LIMIT_MSG " 1...",1,0};
  //��������� ������ ����������� ������ �� ������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_time_1_1={CFG_CHECKBOX,TIME_LIMIT_IS_ENABLED_MSG,0,0};
  __root const int cfg_time1_limit=1;
  //����� ���������
  __root const CFG_HDR cfghdr_time_1_2={CFG_TIME,ON_MISSED_MSG,0,0};
  __root const TIME cfg_on_time1={6,30,0};
  //����� ����������
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
  //��������� ������ ����������� ������ �� ������� (1 - ���, 0 - ����)
  __root const CFG_HDR cfghdr_time_2_1={CFG_CHECKBOX,TIME_LIMIT_IS_ENABLED_MSG,0,0};
  __root const int cfg_time2_limit=0;
  //����� ���������
  __root const CFG_HDR cfghdr_time_2_2={CFG_TIME,ON_MISSED_MSG,0,0};
  __root const TIME cfg_on_time2={23,1,0};
  //����� ����������
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
  //����� ������
  __root const CFG_HDR cfghdr_mode_1={CFG_CBOX,MODE2_MSG,0,3};
  __root const int cfg_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox_mode_2[3]={FOREVER_MSG,DURING_MSG,N_TIMES_MSG};
  //������� ������ ������� (� ���) ��� ������� ��� ���������� (� ����������� �� mode)
  __root const CFG_HDR cfghdr_mode_3={CFG_UINT,DURATION_MSG,1,100000};
  __root const unsigned int cfg_duration=60;
__root const CFG_HDR cfghdr_mode_x={CFG_LEVEL,"",0,0};
