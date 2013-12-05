#ifndef _C510_R1HA035_
#define _C510_R1HA035_

#define EXT_TABLE 0x15B5CD30
#define HELPER_SIG 0x6FF0

#define PATH_USER_INT         L"/usb"
#define PATH_USER_EXT         L"/card"
#define PATH_AUDIO_INT        L"/usb/music"
#define PATH_AUDIO_EXT        L"/card/music"
#define PATH_IMAGE_INT        L"/usb/picture"
#define PATH_IMAGE_EXT        L"/card/picture"
#define PATH_VIDEO_INT        L"/usb/video"
#define PATH_VIDEO_EXT        L"/card/video"
#define PATH_THEME_INT        L"/usb/theme"
#define PATH_THEME_EXT        L"/card/theme"
#define PATH_OTHER_INT        L"/usb/other"
#define PATH_OTHER_EXT        L"/card/other"

#define PATH_ELF_ROOT_INT     L"/usb/other/ZBin"
#define PATH_ELF_ROOT_EXT     L"/card/other/ZBin"
#define PATH_ELF_INT          L"/usb/other/ZBin"
#define PATH_ELF_EXT          L"/card/other/ZBin"
#define PATH_ELF_DAEMONS_INT  L"/usb/other/ZBin/Daemons"
#define PATH_ELF_DAEMONS_EXT  L"/card/other/ZBin/Daemons"
#define PATH_ELF_CONFIG_INT   L"/usb/other/ZBin/Config"
#define PATH_ELF_CONFIG_EXT   L"/card/other/ZBin/Config"
#define PATH_INI              L"/usb/other/ini"
#define PATH_DLL              L"/usb/other/ZBin/DLL"
#define PATH_DEFAULT          L"/usb/other"

#define DB_CMD_SETSAVEDICON 0x38
#define DB_CMD_SETSMALLICON 0x39
#define DB_CMD_SETTHUMBNAILICON 0x3A

#define PAGE_ENTER_EVENT 0xF
#define PAGE_EXIT_EVENT 0x10
#define ACCEPT_EVENT 2
#define PREVIOUS_EVENT 3
#define CANCEL_EVENT 4

#define KEY_LAST 0x166

#define A2

#ifndef NOICONS
#define EP_EXTICON24x24
#define EP_EXTICON42x32
#endif

#endif
