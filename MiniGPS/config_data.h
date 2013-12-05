#ifndef _CONFIGDATA_H_
  #define _CONFIGDATA_H_

// --------------------[ Control settings ]---------------------------------- //
extern const int cfg_ctrlmode;
extern const int cfg_menukey;
extern const int cfg_menukeymode;
extern const int cfg_showkey;
extern const int cfg_showkeymode;
extern const int cfg_showcellid;
extern const int cfg_chowcellidmode;
extern const int cfg_autolocation;
extern const int cfg_autolocationmode;
extern const int cfg_ignore_keylock;
// -------------------------------------------------------------------------- //

// --------------------[ Main settings ]------------------------------------- //
extern const int cfg_show_type;
extern const int cfg_cellidmode;
extern const int cfg_location;
extern const unsigned int cfg_cell_font;
extern const unsigned int cfg_cell_color;
extern const unsigned int cfg_cell_border;
extern const int cfg_cell_align;
extern const int cfg_highlight;
extern const unsigned int cfg_cell_x;
extern const unsigned int cfg_cell_y;
extern const int cfg_vibrate;
extern const unsigned int cfg_new_action_timeout;
extern const int cfg_auto_image_show;
extern const unsigned int cfg_auto_image_x;
extern const unsigned int cfg_auto_image_y;
extern const wchar_t cfg_auto_file[256];
extern const RECT cfg_location_image;
extern const wchar_t cfg_images_path[256];
extern const wchar_t cfg_location_file[256];
// -------------------------------------------------------------------------- //

// --------------------[ Action settings ]----------------------------------- //
extern const int cfg_default_profile;
extern const int cfg_manual_profile1;
extern const int cfg_manual_profile2;
extern const int cfg_manual_profile3;
extern const int cfg_manual_profile4;
extern const int cfg_manual_profile5;
extern const int cfg_manual_profile6;
extern const int cfg_manual_profile7;

#define extern_place(num) \
  extern const int cfg_place_enable##num; \
  extern const wchar_t cfg_place_name##num[128]; \
  extern const int cfg_place_profile##num; \
  extern const int cfg_place_vibrate##num; \
  extern const int cfg_place_sound_enable##num; \
  extern const wchar_t cfg_sound_file##num[256]

extern_place(1);
extern_place(2);
extern_place(3);
extern_place(4);
extern_place(5);
extern_place(6);
extern_place(7);
extern_place(8);
extern_place(9);
extern_place(10);
extern_place(11);
extern_place(12);
extern_place(13);
extern_place(14);
extern_place(15);
extern_place(16);

//extern const wchar_t cfg_sound_path[256];
// -------------------------------------------------------------------------- //

// --------------------[ Log settings ]-------------------------------------- //
extern const int cfg_log_mode;
extern const int cfg_date_format;
extern const int cfg_log_ext;
extern const wchar_t cfg_logs_path[256];
// -------------------------------------------------------------------------- //

// --------------------[ Other settings ]------------------------------------ //
extern const unsigned int cfg_update_interval;
extern const int cfg_standby_only;
extern const int cfg_set_in_gdfs;
extern const int cfg_silent;
extern const int cfg_encode_type;
extern const wchar_t cfg_encode_file[256];
extern const wchar_t cfg_codemap_addr[16];
extern const wchar_t cfg_bcfgedit[256];
// -------------------------------------------------------------------------- //

#endif //_CONFIGDATA_H_
