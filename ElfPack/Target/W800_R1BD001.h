#ifndef _W800_R1BD001_
#define _W800_R1BD001_

#define EXT_TABLE 0x4422C6D0
#define HELPER_SIG 0x6FF0

#define PATH_USER_INT         L"/tpa/user"
#define PATH_USER_EXT         L"/card/MSSEMC/Media files"
#define PATH_AUDIO_INT        L"/tpa/user/audio"
#define PATH_AUDIO_EXT        L"/card/MSSEMC/Media files/audio"
#define PATH_IMAGE_INT        L"/tpa/user/image"
#define PATH_IMAGE_EXT        L"/card/MSSEMC/Media files/image"
#define PATH_VIDEO_INT        L"/tpa/user/video"
#define PATH_VIDEO_EXT        L"/card/MSSEMC/Media files/video"
#define PATH_THEME_INT        L"/tpa/user/theme"
#define PATH_THEME_EXT        L"/card/MSSEMC/Media files/theme"
#define PATH_OTHER_INT        L"/tpa/user/other"
#define PATH_OTHER_EXT        L"/card/MSSEMC/Media files/other"

#define PATH_ELF_ROOT_INT     L"/tpa/user/other"
#define PATH_ELF_ROOT_EXT     L"/card/MSSEMC/Media files/other"
#define PATH_ELF_INT          L"/tpa/user/other/ZBin"
#define PATH_ELF_EXT          L"/card/MSSEMC/Media files/other/ZBin"
#define PATH_ELF_DAEMONS_INT  L"/tpa/user/other/Daemons"
#define PATH_ELF_DAEMONS_EXT  L"/card/MSSEMC/Media files/other/Daemons"
#define PATH_ELF_CONFIG_INT   L"/tpa/user/other/Config"
#define PATH_ELF_CONFIG_EXT   L"/card/MSSEMC/Media files/other/Config"
#define PATH_INI              L"/tpa/user/other/ini"
#define PATH_DLL              L"/tpa/user/other/DLL"
#define PATH_DEFAULT          L"/tpa/user/other"

#define DB_CMD_SETSMALLICON 0x35
#define DB_CMD_SETTHUMBNAILICON 0x36

#define PAGE_ENTER_EVENT 0x5
#define PAGE_EXIT_EVENT 0x6
#define ACCEPT_EVENT 0xF
#define PREVIOUS_EVENT 0x10
#define CANCEL_EVENT 0x11

#define KEY_LAST 0x144

#ifndef NOICONS
#define EP_EXTICON16x20
#define EP_EXTICON48x36
#endif

#endif
