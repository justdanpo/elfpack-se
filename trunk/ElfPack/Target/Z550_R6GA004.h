#ifndef _Z550_R6GA004_
#define _Z550_R6GA004_

#define EXT_TABLE 0x444E2ECC
#define HELPER_SIG 0x6FF0

#define PATH_USER_INT         L"/usb"
#define PATH_USER_EXT         L"/card/MSSEMC/Media files"
#define PATH_AUDIO_INT        L"/usb/audio"
#define PATH_AUDIO_EXT        L"/card/MSSEMC/Media files/audio"
#define PATH_IMAGE_INT        L"/usb/image"
#define PATH_IMAGE_EXT        L"/card/MSSEMC/Media files/image"
#define PATH_VIDEO_INT        L"/usb/vide"
#define PATH_VIDEO_EXT        L"/card/MSSEMC/Media files/video"
#define PATH_THEME_INT        L"/usb/theme"
#define PATH_THEME_EXT        L"/card/MSSEMC/Media files/theme"
#define PATH_OTHER_INT        L"/usb/other"
#define PATH_OTHER_EXT        L"/card/MSSEMC/Media files/other"

#define PATH_ELF_ROOT_INT     L"/usb/other/ZBin"
#define PATH_ELF_ROOT_EXT     L"/card/MSSEMC/Media files/other/ZBin"
#define PATH_ELF_INT          L"/usb/other/ZBin"
#define PATH_ELF_EXT          L"/card/MSSEMC/Media files/other/ZBin"
#define PATH_ELF_DAEMONS_INT  L"/usb/other/ZBin/Daemons"
#define PATH_ELF_DAEMONS_EXT  L"/card/MSSEMC/Media files/other/ZBin/Daemons"
#define PATH_ELF_CONFIG_INT   L"/usb/other/ZBin/Config"
#define PATH_ELF_CONFIG_EXT   L"/card/MSSEMC/Media files/other/ZBin/Config"
#define PATH_INI              L"/usb/other/ini"
#define PATH_DLL              L"/usb/other/ZBin/DLL"
#define PATH_DEFAULT          L"/usb/other"

#define DB_CMD_SETSMALLICON 0x36

#define PAGE_ENTER_EVENT 5
#define PAGE_EXIT_EVENT 6
#define ACCEPT_EVENT 0x0F
#define PREVIOUS_EVENT 0x10
#define CANCEL_EVENT 0x11

#define KEY_LAST 0x146

#ifndef NOICONS
#define EP_EXTICON16x20
#define EP_EXTICON48x36
#endif

#endif
