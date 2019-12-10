#ifndef _MAIN_H_
  #define _MAIN_H_

#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"
#include "..\..\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#define BOOKNAME "XWeather"

#define DISPLAY                0

#define RESULT_OK              0
#define RESULT_FAIL           -1
#define FILE_HANDLE_INVALID   -1
#define FILE_INVALID          -1

#define FILENAME_MAX_LEN      255

#define ELF_BCFG_CONFIG_EVENT 994
#define MAININPUT_HIDED_EVENT 0x1D11
#define MAININPUT_SHOWED_EVENT 0x1D12

#define CACHE_FILE            L"weather.co.uk.cache"
#define CACHE_DIR             L"/tpa/cache"

//#define CLIPART_DIR           L"XWeather/clipart"
//#define CONFIG_INTERNAL       GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL)

enum align_t
{
  ALIGN_LEFT = 0,
  ALIGN_RIGHT,
  ALIGN_CENTER
};

typedef struct
{
  BOOK *book;
  wchar_t *bcfg_p;
  wchar_t *bcfg_n;
}MSG_BCFG;

typedef struct
{
   char weekday; //день недели
   char cloud; //облачность
   char wind; //скорость ветра, в м/с
   char humidity; //относительная влажность воздуха, в %
   unsigned int temperature; //температура воздуха, в градусах Цельсия
   wchar_t pic[20]; //название картинки
   u16 pressure; //атмосферное давление, в мм.рт.ст.
   u16 w_rumb; //направление ветра, в градусах
   
} CURRENT_FORECAST;

typedef struct
{
  int img_w;
  int img_h;
  int x;
  int y;
} IMGCOORD;

#define MON _T("CALE_MONDAY_TXT")
#define TUE _T("CALE_TUESDAY_TXT")
#define WEN _T("CALE_WEDNESDAY_TXT")
#define THU _T("CALE_THURSDAY_TXT")
#define FRI _T("CALE_FRIDAY_TXT")
#define SAT _T("CALE_SATURDAY_TXT")
#define SUN _T("CALE_SUNDAY_TXT")

#endif
