#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "elfcount.h"
#include "getpostbase.hpp"

char *xml;

char char2hex(char *buf)
{
   char ret = 0;
   
   char s = *buf++;
   if((s>=0x30) && (s<=0x39)) s-=0x30;
   else if((s>=0x41) && (s<=0x46)) s-=0x37;
   ret |= s<<4;
   
   s = *buf;
   if((s>=0x30) && (s<=0x39)) s-=0x30;
   else if((s>=0x41) && (s<=0x46)) s-=0x37;
   ret |= s;
   return(ret);
}

char *strchr(char *str, char c)
{
   for(; *str; str++)
      if(*str == c) return(str);
   return(NULL);
}

wchar_t *GetCity(char *city_in_hex)
{
   int i;
   char *value = city_in_hex;
   char city[512];
   for(i = 0; ; i++)
   {
      value = strchr(value, '%') + 1;
      if((int)value == 1) break;
      city[i] = char2hex(value);
   }
   city[i] = 0;
   wchar_t *ret = new wchar_t[i+1];
   memset(ret, 0, (i+1)*sizeof(wchar_t));
   str2wstr(ret, city);
   return(ret);
}

char *GetData(char *param)
{
   char *value = strchr(param, '=') + 2;
   char *end_str = strchr(value+1, '"');
   int value_len = end_str - value;
   char *ret = new char[value_len+1];
   memset(ret, 0, value_len+1);
   return(strncpy(ret, value, value_len));
}

wchar_t *array__time_of_day[4] = 
{
   L"Ночь",
   L"Утро",
   L"День",
   L"Вечер"
};

wchar_t *array__cloudiness[4] = 
{
   L"Ясно",
   L"Малооблачно",
   L"Облачно",
   L"Пасмурно"
};

wchar_t *array__precipitation[7] = 
{
   L"Дождь",
   L"Ливень",
   L"Снег",
   L"Снег",
   L"Гроза",
   L"Нет данных",
   L"Без осадков"
};

wchar_t *array__wind[8] = 
{
   L"Северный",
   L"Северо-восточный",
   L"Северо-западный",
   L"Южный",
   L"Юго-восточный",
   L"Юго-западный",
   L"Восточный",
   L"Западный",
};

typedef struct
{
   char cloudiness; //облачность по градациям:  0 - ясно, 1- малооблачно, 2 - облачно, 3 - пасмурно
   char precipitation; //тип осадков: 4 - дождь, 5 - ливень, 6,7 – снег, 8 - гроза, 9 - нет данных, 10 - без осадков
   char rpower; //интенсивность осадков, если они есть. 0 - возможен дождь/снег, 1 - дождь/снег
   char spower; //вероятность грозы, если прогнозируется: 0 - возможна гроза, 1 - гроза
   
} PHENOMENA;

typedef struct
{
   unsigned int max;
   unsigned int min;
} MAXMIN;

typedef struct
{
   char max;
   char min;
   char direction; //направление ветра в румбах, 0 - северный, 1 - северо-восточный,  и т.д.
   char reserve;
   
} WIND;

typedef struct
{
   DATE date; //дата, на которую составлен прогноз в данном блоке
   char hour; //местное время, на которое составлен прогноз
   char weekday; //день недели, 1 - воскресенье, 2 - понедельник, и т.д.
   char time_of_day_type; //время суток, для которого составлен прогноз: 0 - ночь 1 - утро, 2 - день, 3 - вечер
   PHENOMENA phenomena;
   MAXMIN pressure; //атмосферное давление, в мм.рт.ст.
   MAXMIN temperature; //температура воздуха, в градусах Цельсия
   WIND wind;
   MAXMIN relwet; //относительная влажность воздуха, в %
   MAXMIN heat; //комфорт - температура воздуха по ощущению одетого по сезону человека, выходящего на улицу
   char reserve;
   
} FORECAST;

#define MON _T("CALE_MONDAY_TXT")
#define TUE _T("CALE_TUESDAY_TXT")
#define WEN _T("CALE_WEDNESDAY_TXT")
#define THU _T("CALE_THURSDAY_TXT")
#define FRI _T("CALE_FRIDAY_TXT")
#define SAT _T("CALE_SATURDAY_TXT")
#define SUN _T("CALE_SUNDAY_TXT")

wchar_t *days_str[7] =
{
   SUN,
   MON,
   TUE,
   WEN,
   THU,
   FRI,
   SAT
};

int GetIntParam(char *p)
{
   int ret;
   char *param = GetData(p);
   if(param[0] == '-')
   {
      sscanf(param+1, "%d", &ret);
      ret = 0 - ret;
   }
   else sscanf(param, "%d", &ret);
   mfree(param);
   return(ret);
}

void call_parser(void)
{
   char *temp = xml;
   char *city_in_hex = GetData(temp = strstr(temp, "sname"));
   wchar_t *city = GetCity(city_in_hex);
   mfree(city_in_hex);
   
   LIST *f_list = List_Create();
   
   while(true)
   {
      temp = strstr(temp, "FORECAST");
      if(!temp) break;
      
      FORECAST *f = new FORECAST;
      
      f->date.day = GetIntParam(temp = strstr(temp, "day")+1);
      f->date.mon = GetIntParam(temp = strstr(temp, "month")+1);
      f->date.year = GetIntParam(temp = strstr(temp, "year")+1);
      f->hour = GetIntParam(temp = strstr(temp, "hour")+1);
      f->time_of_day_type = GetIntParam(temp = strstr(temp, "tod")+1);
      f->weekday = GetIntParam(temp = strstr(temp, "weekday")+1);
      
      //phenomena
      f->phenomena.cloudiness = GetIntParam(temp = strstr(temp, "cloudiness")+1);
      f->phenomena.precipitation = GetIntParam(temp = strstr(temp, "precipitation")+1);
      f->phenomena.rpower = GetIntParam(temp = strstr(temp, "rpower")+1);
      f->phenomena.spower = GetIntParam(temp = strstr(temp, "spower")+1);
      
      //pressure
      f->pressure.max = GetIntParam(temp = strstr(temp, "max")+1);
      f->pressure.min = GetIntParam(temp = strstr(temp, "min")+1);
      
      //temperature
      f->temperature.max = GetIntParam(temp = strstr(temp, "max")+1);
      f->temperature.min = GetIntParam(temp = strstr(temp, "min")+1);
      
      //wind
      f->wind.min = GetIntParam(temp = strstr(temp, "min")+1);
      f->wind.max = GetIntParam(temp = strstr(temp, "max")+1);
      f->wind.direction = GetIntParam(temp = strstr(temp, "direction")+1);
      
      //relwet
      f->relwet.max = GetIntParam(temp = strstr(temp, "max")+1);
      f->relwet.min = GetIntParam(temp = strstr(temp, "min")+1);
      
      //heat
      f->heat.min = GetIntParam(temp = strstr(temp, "min")+1);
      f->heat.max = GetIntParam(temp = strstr(temp, "max")+1);
      
      char *end_forecast = "/FORECAST";
      temp = strstr(temp, end_forecast);
      temp += strlen(end_forecast);
      
      List_InsertLast(f_list, f);
   }
   
   DATETIME dt;
   REQUEST_DATEANDTIME_GET(SYNC, &dt);
   
   wchar_t info[512];
   info[0] = 0;
   
   for(int i = 0; i < f_list->FirstFree; i++)
   {
      FORECAST *f = (FORECAST *)List_Get(f_list, i);
      if(dt.date.day == f->date.day)
      {
         int time_min = f->hour - 3;
         int time_max = f->hour + 3;
         if(time_max > 24) time_max -= 24;
         
         if((time_min <= dt.time.hour) && (dt.time.hour < time_max))
         {
            wchar_t weekday[20];
            int icon_id;
            textidname2id(days_str[f->weekday-1], TEXTID_ANY_LEN, &icon_id);
            TextID_GetWString(icon_id, weekday, TEXTID_ANY_LEN);
            weekday[0] -= 0x20;
            
            snwprintf(info, 511,
                      L"Город: %ls\n%02d.%02d.%d\n%ls, %ls\n%ls, %ls\n%d...%d\n%d-%d м/с, %ls\n%d мм рт.ст.\n%d%%",
                      city,
                      f->date.day,
                      f->date.mon,
                      f->date.year,
                      weekday,
                      array__time_of_day[f->time_of_day_type],
                      array__cloudiness[f->phenomena.cloudiness],
                      array__precipitation[f->phenomena.precipitation-4],
                      f->temperature.min,
                      f->temperature.max,
                      f->wind.min,
                      f->wind.max,
                      array__wind[f->wind.direction],
                      (f->pressure.max + f->pressure.min)/2,
                      (f->relwet.max + f->relwet.min)/2);
            break;
         }
      }
   }
   
   mfree(city);
   mfree(xml);
   
   MessageBox(EMPTY_TEXTID,
              TextID_Create(info, ENC_UCS2, TEXTID_ANY_LEN),
              NOIMAGE, 2, 0, NULL);
   
   for(int i = f_list->FirstFree; i > 0; i--) mfree(List_RemoveAt(f_list, i-1));
   List_Destroy(f_list);
}

class MyGetPost: public CGetPostBase
{
	void OnResult(int DataLength, int HeaderLength, int TotalSize, char MoreData, short StatusCode, int ClientData)
	{
		debug_printf("\n!!! onresult %x %x %x %x status %d error %d!\n", DataLength, HeaderLength, TotalSize, MoreData, StatusCode, ClientData);

		char* temp;

		if(HeaderLength)
		{
			temp = new char[HeaderLength+1];
			memset(temp,0,HeaderLength+1);
			int i3=0,i4=0;
			if(preq->ReadHeader(HeaderLength, temp, &i3, &i4)>=0)
			{
				debug_printf("\n!!!!!! =) header i3/i4 %x %x s %s\n",i3,i4,temp);
			}
			delete temp;
		}
		if(DataLength)
		{
               if(!xml)
               {
                  xml = new char[DataLength+1];
                  memset(xml, 0, DataLength+1);
                  temp = xml;
               }
               else
               {
                  
                  int len = strlen(xml);
                  temp = new char[len+DataLength+1];
                  memset(temp, 0, len+DataLength+1);
                  strcpy(temp, xml);
                  mfree(xml);
                  xml = temp;
                  temp = xml+len;
               }
               
               int i3=0,i4=0;
               if(preq->ReadData(DataLength, temp, &i3, &i4) >= 0)
               {
                  //debug_printf("\n!!!!!! =) data i3/i4 %x %x s %s\n", i3, i4, temp);
               }
		}

		//нет больше данных - убиваемся
		if(!MoreData)
		{
               debug_printf("\n!!!!!! =) data: %s\n", xml);
               call_parser(); //вызываем парсер
               Release();
		}
	}

	void OnProgress(int CurrPos, int TotSize, int, int status)
	{
		debug_printf("\n!!!!!!!!!! %s\n",__FUNCTION__);
	}
};

int main (void)
{
	MyGetPost* mygp = new MyGetPost(); //тестовый объект убьёт себя сам после приёма данных

      xml = NULL;
      
      wchar_t *site = L"informer.gismeteo.ru/xml";
      //wchar_t *name = L"28642_1.xml";
      
      int town_id = 28642;
      
      wchar_t name[32];
      snwprintf(name, 31, L"%d_1.xml", town_id);
      
      char *uri = CreateURI(site, name, GetURIScheme(http));
      debug_printf("\n uri: %s\n", uri);
      
	if( NULL != mygp && 0 <= mygp->Get( uri, "Accept: text/xml" ) )
	{
		debug_printf("\n!!!!!!!!!! %s: get ok\n",__FUNCTION__);
	}else
	{
		debug_printf("\n!!!!!!!!!! %s: get error\n",__FUNCTION__);
		mygp->Release();
	}

      mfree(uri);
	//завершение работы эльфа. реально эльф умрёт, если живых колбэков в памяти не останется
	elf_release();
}
