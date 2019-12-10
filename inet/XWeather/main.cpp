#include "main.h"

#include "elfcount.h"
#include "getpostbase.hpp"
#include "Graphics.h"

const wchar_t *SPLASH = L"/";

static const char XWeatherGuiName[] = "XWeatherGUI";

typedef char *(*GET)( char * );

CURRENT_FORECAST *cur_forecast;

u16 timer;
char *xml;
DISP_OBJ *cur_disp;

int GetWeather(void);
int weather_co_ua___call_parser(void);
int UpdateLayer(void *mess, BOOK *book);
int GetTime();
void Timer_GetWeather(u16 timerID, LPARAM lparam);

wchar_t *array__wind_direction[17] = 
{
   L"С",
   L"С,С-В",
   L"С-В",
   L"В,С-В",
   L"В",
   L"В,Ю-В",
   L"Ю-В",
   L"Ю,Ю-В",
   L"Ю",
   L"Ю,Ю-З",
   L"Ю-З",
   L"З,Ю-З",
   L"З",
   L"З,С-З",
   L"С-З",
   L"С,С-З",
   L"С"
};

bool is_lim(int dig, int a, int b)
{
   if((a <= dig) && (dig < b)) return(true);
   return(false);
}

int Forecast_WindDirection_GetType(CURRENT_FORECAST *forecast)
{
   int w_rumb = forecast->w_rumb;
   
   if(is_lim(w_rumb, 0, 20)) return(0);
   if(is_lim(w_rumb, 20, 35)) return(1);
   if(is_lim(w_rumb, 35, 55)) return(2);
   if(is_lim(w_rumb, 55, 70)) return(3);
   if(is_lim(w_rumb, 70, 110)) return(4);
   if(is_lim(w_rumb, 110, 125)) return(5);
   if(is_lim(w_rumb, 125, 145)) return(6);
   if(is_lim(w_rumb, 145, 160)) return(7);
   if(is_lim(w_rumb, 160, 200)) return(8);
   if(is_lim(w_rumb, 200, 215)) return(9);
   if(is_lim(w_rumb, 215, 235)) return(10);
   if(is_lim(w_rumb, 235, 250)) return(11);
   if(is_lim(w_rumb, 250, 290)) return(12);
   if(is_lim(w_rumb, 290, 305)) return(13);
   if(is_lim(w_rumb, 305, 325)) return(14);
   if(is_lim(w_rumb, 325, 340)) return(15);
   if(is_lim(w_rumb, 340, 360)) return(16);
   return(RESULT_FAIL);
}

wchar_t *MakeFullName(wchar_t const *path, wchar_t *name)
{
   int len = wstrlen(path)+wstrlen(name)+1;
   wchar_t *fullname = new wchar_t[len+1];
   memset(fullname, 0, (len+1)*sizeof(wchar_t));
   wstrcpy(fullname, path);
   if(name && wstrlen(name))
   {
      wstrcat(fullname, SPLASH);
      wstrcat(fullname, name);
   }
   return(fullname);
}

int Cache_IsExists(void)
{
   W_FSTAT fst;
   bool result = false;
   wchar_t *fname = MakeFullName(CACHE_DIR, CACHE_FILE);
   if(w_fstat(fname, &fst) >= 0) result = true;
   mfree(fname);
   return(result);
}

int DirFile_IsExists(wchar_t const *dir, wchar_t *file)
{
   W_FSTAT fst;
   bool result = false;
   wchar_t *fname = MakeFullName(dir, file);
   if(w_fstat(fname, &fst) >= 0) result = true;
   mfree(fname);
   return(result);
}

CURRENT_FORECAST *Cache_LoadForecast(void)
{
   CURRENT_FORECAST *forecast = NULL;
   if(Cache_IsExists())
   {
      wchar_t *fname = MakeFullName(CACHE_DIR, CACHE_FILE);
      int file = w_fopen(fname, WA_Read, 0x1FF, 0);
      if(file != FILE_HANDLE_INVALID)
      {
         forecast = new CURRENT_FORECAST;
         w_fread(file, forecast, sizeof(CURRENT_FORECAST));
         w_fclose(file);
      }
      mfree(fname);
   }
   return(forecast);
}

void Cache_SaveForecast(CURRENT_FORECAST *forecast)
{
   wchar_t *fname = MakeFullName(CACHE_DIR, CACHE_FILE);
   int file = w_fopen(fname, WA_Create | WA_Truncate | WA_Write, 0x1FF, 0);
   if(file != FILE_HANDLE_INVALID)
   {
      w_fwrite(file, forecast, sizeof(CURRENT_FORECAST));
      w_fclose(file);
   }
   mfree(fname);
}

char *strchr(char *str, char c)
{
   for(; *str; str++)
      if(*str == c) return(str);
   return(NULL);
}

wchar_t *wstrchr(wchar_t *wstr, wchar_t c)
{
   for(; *wstr; wstr++)
      if(*wstr == c) return(wstr);
   return(NULL);
}

char *GetData10(char *param)
{
   char *value = strchr(param, '=') + 2;
   char *end_str = strchr(value+1, '"');
   int value_len = end_str - value;
   char *ret = new char[value_len+1];
   memset(ret, 0, value_len+1);
   return(strncpy(ret, value, value_len));
}

wchar_t *days_str[7] =
{
   MON,
   TUE,
   WEN,
   THU,
   FRI,
   SAT,
   SUN
};

int GetIntParam(char *p, GET f)
{
   int ret;
   char *param = f(p);
   debug_printf("\n!! GetData: %s\n", param);
   if(param[0] == '-')
   {
      sscanf(param+1, "%d", &ret);
      ret = 0 - ret;
   }
   else if(param[0] == '+') sscanf(param+1, "%d", &ret);
   else sscanf(param, "%d", &ret);
   mfree(param);
   debug_printf("\n!! param: %d\n", ret);
   return(ret);
}

char *GetData12(char *param)
{
   char *value = strchr(param, '>') + 1;
   char *end_str = strchr(value, '<');
   int value_len = end_str - value;
   char *ret = new char[value_len+1];
   memset(ret, 0, value_len+1);
   return(strncpy(ret, value, value_len));
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
            //elf_release();
            //debug_printf("\n!!!!!! =) data i3/i4 %x %x s %s\n", i3, i4, temp);
         }
      }
      
      //нет больше данных - убиваемся
      if(!MoreData)
      {
         debug_printf("\n!!!!!! =) data: %s\n", xml);
         weather_co_ua___call_parser(); //вызываем парсер
         elf_addref();
         Release();
      }
   }
   
   void OnProgress(int CurrPos, int TotSize, int, int status)
   {
      debug_printf("\n!!!!!!!!!! %s\n",__FUNCTION__);
   }
};

int UTF8toUCS2symb(int utf8_symb)
{
   int a = ((utf8_symb&0xFF00)>>8)&0x3F;
   int b = ((utf8_symb&0xFF)&0x1F)<<6;
   return(a+b);
}

int Char_convUTF8toUCS2String(char *src_ptr, int src_len, wchar_t *trg_ptr, int trg_len)
{
   int i, j;
   for(i=0, j=0; j < src_len && j < trg_len && src_ptr[i]; i++, j++)
   {
      if(src_ptr[i] <= 0x7F) {
         trg_ptr[j] = src_ptr[i];
      } else if (src_ptr[i] >= 0xC2 && src_ptr[i] <= 0xDF){
         trg_ptr[j] = UTF8toUCS2symb((src_ptr[i+1]<<8)+src_ptr[i]);
         i++;
      } else {
         trg_ptr[j] = '?';
      }
   }
   trg_ptr[j] = 0;
   return(RESULT_OK);
}

int weather_co_ua___call_parser(void)
{
   int result = RESULT_FAIL;
   char *temp = xml;
   if(!strstr(temp, "forecast"))
   { //тут обрабатывается первый запрос, получаем id города
      temp = strstr(temp, "city id");
      if(!temp) return(result); //нет такого города
      
      int city_id = GetIntParam(temp+1, GetData10);
      char *lang = lang_get_name(get_system_langID());
      wchar_t *site = L"xml.weather.co.ua/1.2/forecast";
      
      wchar_t page[256];
      snwprintf(page, 255, L"%d?dayf=%d&lang=%s", city_id, DAYS, lang);
      
      char *uri = CreateURI(site, page, GetURIScheme(http));
      debug_printf("\n uri: %s\n", uri);
      
      MyGetPost* mygp = new MyGetPost();
      
	if( NULL != mygp && 0 <= mygp->Get( uri, "Accept: text/xml" ) )
	{
         debug_printf("\n!!!!!!!!!! %s: get ok\n",__FUNCTION__);
         result = RESULT_OK;
	}
      else
	{
         debug_printf("\n!!!!!!!!!! %s: get error\n",__FUNCTION__);
         elf_addref();
         mygp->Release();
	}
      mfree(uri);
      return(result);
   }
   else
   { //тут обрабатывается второй запрос, получаем погоду
      DATETIME dt;
      REQUEST_DATEANDTIME_GET(SYNC, &dt);
      
      if(strstr(temp, "<current last"))
      {
         CURRENT_FORECAST *forecast = new CURRENT_FORECAST;
         memset(forecast, 0, sizeof(CURRENT_FORECAST));
         
         //weekday
         DATE_GetWeekDay(&dt.date, &forecast->weekday);
         //cloud
         forecast->cloud = GetIntParam(temp = strstr(temp, "<cloud>"), GetData12);
         //pict
         char *pict = GetData12(temp = strstr(temp, "<pict>"));
         str2wstr(forecast->pic, pict);
         mfree(pict);
         *wstrrchr(forecast->pic, '.') = 0;
         //temperature
         forecast->temperature = GetIntParam(temp = strstr(temp, "<t>"), GetData12);
         //pressure
         forecast->pressure = GetIntParam(temp = strstr(temp, "<p>"), GetData12);
         //wind
         forecast->wind = GetIntParam(temp = strstr(temp, "<w>"), GetData12);
         //w_rumb
         forecast->w_rumb = GetIntParam(temp = strstr(temp, "<w_rumb>"), GetData12);
         //humidity
         forecast->humidity = GetIntParam(temp = strstr(temp, "<h>"), GetData12);
         
         if(cur_forecast) mfree(cur_forecast);
         cur_forecast = forecast;
         Cache_SaveForecast(cur_forecast);
         
         result = RESULT_OK;
      }
      
      if(xml) mfree(xml);
      
      DispObject_InvalidateRect(cur_disp, NULL);
      return(result);
   }
}

int TerminateElf(void * msg, BOOK *bk)
{
   FreeBook(bk);
   return(1);
}

int ShowAuthorInfo(void *mess, BOOK *bk)
{
   MessageBox(EMPTY_TEXTID,
              STR("XWeather\n\n(c) Joker XT"),
              NOIMAGE, 1 , 0, NULL);
   return(1);
}

GUI *BookObj_GetGUIbyName(BOOK *b, char *my_gui_name)
{
   for(int i = 0; i < b->xguilist->guilist->FirstFree; i++)
   {
      GUI *gui = (GUI *)List_Get(b->xguilist->guilist, i);
      char *cur_name = DispObject_GetName(GUIObject_GetDispObject(gui));
      if(!strcmp(cur_name,my_gui_name)) return(gui);
   }
   return(NULL);
}

static int ReconfigElf(void *mess, BOOK *book)
{
   RECONFIG_EVENT_DATA *reconf = (RECONFIG_EVENT_DATA *)mess;
   int result = 0;
   if(!wstrcmpi(reconf->path,successed_config_path) && !wstrcmpi(reconf->name,successed_config_name))
   {
      InitConfig();
      if(IS_RECONF)
      {
         GetWeather();
         Timer_ReSet(&timer, GetTime(), Timer_GetWeather, NULL);
      }
      DispObject_InvalidateRect(cur_disp, NULL);
      result = 1;
   }
   return(result);
}

IMAGEID RegisterImage(wchar_t *path, wchar_t *name)
{
   IMAGEID ImageID = NOIMAGE;
   void *dir = w_diropen(path);
   wchar_t *dir_item;
   while(dir_item = w_dirread(dir)) if(!wstrcmpni(dir_item, name, wstrlen(name))) break;
   w_dirclose(dir);
   if(dir_item)
      if(ImageID_Get(path, dir_item, &ImageID) < 0) ImageID = NOIMAGE;
   return(ImageID);
}

void GetCoordImage(IMGCOORD *coord, IMAGEID id, int x1, int y1, int scale)
{
  int w = coord->img_w = (GetImageWidth(id)*scale)/100;
  int h = coord->img_h = (GetImageHeight(id)*scale)/100;
  coord->x = x1 - w/2;
  coord->y = y1 - h/2;
}

int mask_print(wchar_t *buffer, wchar_t const *mask)
{
   wchar_t *save_buffer = buffer;
   wchar_t *temp = (wchar_t *)mask;
   if(!wstrchr(temp, '<')) return(RESULT_FAIL);
   
   int len = wstrlen(mask);
   wchar_t *cpy_mask = new wchar_t[len+1];
   wstrcpy(cpy_mask, mask);
   cpy_mask[len] = 0;
   
   int t=0, w=0, d=0, p=0, h=0;
   
   wchar_t *_t_ = wstrwstr(temp, L"<t>");
   if(_t_)
   {
      len = _t_ - mask;
      cpy_mask[len] = 0;
   }
   wchar_t *_w_ = wstrwstr(temp, L"<w>");
   if(_w_)
   {
      len = _w_ - mask;
      cpy_mask[len] = 0;
   }
   wchar_t *_d_ = wstrwstr(temp, L"<d>");
   if(_d_)
   {
      len = _d_ - mask;
      cpy_mask[len] = 0;
   }
   wchar_t *_p_ = wstrwstr(temp, L"<p>");
   if(_p_)
   {
      len = _p_ - mask;
      cpy_mask[len] = 0;
   }
   wchar_t *_h_ = wstrwstr(temp, L"<h>");
   if(_h_)
   {
      len = _h_ - mask;
      cpy_mask[len] = 0;
   }
   
   if(!_t_ && !_w_ && !_d_ && !_p_ && !_h_) return(RESULT_FAIL);
   
   for(int i = 1; ; i++)
   {
      temp = wstrchr(temp, '<');
      if(!temp) break;
      
      if(temp != mask && i == 1)
      {
         wstrcpy(buffer, cpy_mask);
         buffer += wstrlen(cpy_mask);
      }
      
      if(temp == _t_ || temp == _d_ || temp == _w_ || temp == _p_ || temp == _h_)
      {
         if(temp == _t_) t = i;
         if(temp == _w_) w = i;
         if(temp == _p_) p = i;
         if(temp == _h_) h = i;
         if(temp == _d_)
         {
            d = i;
            buffer[0] = '%';
            buffer[1] = 'l';
            buffer[2] = 's';
            buffer += 3;
         }
         else
         {
            buffer[0] = '%';
            buffer[1] = 'd';
            buffer += 2;
         }
         wchar_t *temp_ch = cpy_mask + (temp - mask) + 3;
         wstrcpy(buffer, temp_ch);
         buffer += wstrlen(temp_ch);
         temp += 1;
      }
      else --i;
   }
   
   wstrcpy(cpy_mask, save_buffer);
   
   unsigned int p1=0, p2=0, p3=0, p4=0, p5=0;
   
   if( t == 1 ) p1 = cur_forecast->temperature;
   else if( w == 1 ) p1 = cur_forecast->wind;
   else if( d == 1 ) p1 = (int)array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
   else if( p == 1 ) p1 = cur_forecast->pressure;
   else if( h == 1 ) p1 = cur_forecast->humidity;
   
   if( t == 2 ) p2 = cur_forecast->temperature;
   else if( w == 2 ) p2 = cur_forecast->wind;
   else if( d == 2 ) p2 = (int)array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
   else if( p == 2 ) p2 = cur_forecast->pressure;
   else if( h == 2 ) p2 = cur_forecast->humidity;
   
   if( t == 3 ) p3 = cur_forecast->temperature;
   else if( w == 3 ) p3 = cur_forecast->wind;
   else if( d == 3 ) p3 = (int)array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
   else if( p == 3 ) p3 = cur_forecast->pressure;
   else if( h == 3 ) p3 = cur_forecast->humidity;
   
   if( t == 4 ) p4 = cur_forecast->temperature;
   else if( w == 4 ) p4 = cur_forecast->wind;
   else if( d == 4 ) p4 = (int)array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
   else if( p == 4 ) p4 = cur_forecast->pressure;
   else if( h == 4 ) p4 = cur_forecast->humidity;
   
   if( t == 5 ) p5 = cur_forecast->temperature;
   else if( w == 5 ) p5 = cur_forecast->wind;
   else if( d == 5 ) p5 = (int)array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
   else if( p == 5 ) p5 = cur_forecast->pressure;
   else if( h == 5 ) p5 = cur_forecast->humidity;
   
   snwprintf(save_buffer, 511, cpy_mask, p1, p2, p3, p4, p5);
   mfree(cpy_mask);
   return(RESULT_OK);
}

void MyGuiOnRedraw(DISP_OBJ *disp, int, int, int)
{
   //DISP_OBJ *sk_disp = DispObject_SoftKeys_Get();
   //DISP_OBJ *sr_disp = *(StatusRow_p());
   //DISP_OBJ_ONREDRAW_METHOD sk_onredraw = DispObject_GetOnRedraw(sk_disp);
   //DISP_OBJ_ONREDRAW_METHOD sr_onredraw = DispObject_GetOnRedraw(sr_disp);
   
   
   if(!MainInput_getVisible(SBY_GetMainInput(Find_StandbyBook())))
   {
      if(IS_SUBSTRATE)
      {
         if(IS_SUBSTRATE_IMAGE && DirFile_IsExists(CLIPART_DIR, NULL))
         {
            IMAGEID id = RegisterImage((wchar_t *)CLIPART_DIR, L"bg");
            if(id != NOIMAGE)
            {
               GC *gc = get_DisplayGC();
               IMGCOORD imgcoord[1];
               if(IS_CONS) GetCoordImage(imgcoord, id, BG_X_POS+CONS_X, BG_Y_POS+CONS_Y, BG_SCALE);
               else GetCoordImage(imgcoord, id, BG_X_POS, BG_Y_POS, BG_SCALE);
               GC_PutChar(gc, imgcoord->x, imgcoord->y, imgcoord->img_w, imgcoord->img_h, id);
               ImageID_Free(id);
            }
         }
         else
         {
            int x, y, w, h;
            if(IS_CONS)
            {
               x = S_RECT.x1 + CONS_X;
               y = S_RECT.y1 + CONS_Y;
               w = S_RECT.x2 - S_RECT.x1;
               h = S_RECT.y2 - S_RECT.y1;
            }
            else
            {
               x = S_RECT.x1;
               y = S_RECT.y1;
               w = S_RECT.x2 - S_RECT.x1;
               h = S_RECT.y2 - S_RECT.y1;
            }
            SetFillColor(S_COLOR);
            FillRect(x, y, w, h);
            if(IS_STROKE)
            {
               SetForegroundColor(S_COLOR);
               DrawFrame(x, y, w, h);
            }
         }
      }
      
      wchar_t info[512];
      info[0] = 0; info[1] = 0;
      
      if(cur_forecast)
      {
         if(IS_MASK)
         {
            if(mask_print(info+1, MASK) == RESULT_FAIL) wstrcpy(info, L" Error in mask!");
         }
         else
         {
            if(IS_TEMP) snwprintf(info, 511, L"\n%d°C", cur_forecast->temperature);
            if(IS_W) snwprintf(info+wstrlen(info), 511, L"\n%d м/с", cur_forecast->wind);
            if(IS_WD)
            {
               wchar_t *wind_d = array__wind_direction[Forecast_WindDirection_GetType(cur_forecast)];
               snwprintf(info+wstrlen(info), 511, L"\n%ls", wind_d);
            }
            if(IS_P) snwprintf(info+wstrlen(info), 511, L"\n%d мм рт.ст.", cur_forecast->pressure);
            if(IS_H) snwprintf(info+wstrlen(info), 511, L"\n%d%%", cur_forecast->humidity);
         }
         
         if(IS_SHOW_CLIPART && DirFile_IsExists(CLIPART_DIR, NULL))
         {
            IMAGEID id = RegisterImage((wchar_t *)CLIPART_DIR, cur_forecast->pic);
            if(id != NOIMAGE)
            {
               GC *gc = get_DisplayGC();
               IMGCOORD imgcoord[1];
               if(IS_CONS) GetCoordImage(imgcoord, id, CLIPART_X_POS+CONS_X, CLIPART_Y_POS+CONS_Y, SCALE);
               else GetCoordImage(imgcoord, id, CLIPART_X_POS, CLIPART_Y_POS, SCALE);
               GC_PutChar(gc, imgcoord->x, imgcoord->y, imgcoord->img_w, imgcoord->img_h, id);
               ImageID_Free(id);
            }
         }
      }
      else
      {
         wstrcpy(info, L" N/A");
      }
      
      SetFont(FONT);
      SetForegroundColor(COLOR);
      SetBackgroundColor(H_COLOR);
      if(IS_CONS)
         DrawTextWithHighlight(info+1, X_POS+CONS_X, Y_POS+CONS_Y, ALIGN_LEFT, H_TYPE);
      else
         DrawTextWithHighlight(info+1, X_POS, Y_POS, ALIGN_LEFT, H_TYPE);
   }
}

int GetWeather(void)
{
   int result = RESULT_FAIL;
   
   if(CITY[0] == 0) return(result);
   
   MyGetPost *mygp = new MyGetPost(); //тестовый объект убьёт себя сам после приёма данных
   
   wchar_t *search = L"xml.weather.co.ua/1.2/city";
   wchar_t *page = L"?search=";
   
   wchar_t name[256];
   snwprintf(name, 255, L"%ls%ls", page, CITY);
   
   char *uri = CreateURI(search, name, GetURIScheme(http));
   debug_printf("\n uri: %s\n", uri);
   
   if((mygp != NULL) && (mygp->Get( uri, "Accept: text/xml" ) >= 0))
   {
      debug_printf("\n!!!!!!!!!! %s: get ok\n",__FUNCTION__);
      result = RESULT_OK;
   }
   else
   {
      debug_printf("\n!!!!!!!!!! %s: get error\n",__FUNCTION__);
      elf_addref();
      mygp->Release();
   }
   mfree(uri);
   return(result);
}

int GetTime(void)
{
   int sec = UP_TIME.sec;
   int min = UP_TIME.min;
   int hour = UP_TIME.hour;
   int time = (hour*60*60 + min*60 + sec)*1000;
   return(time);
}

void Timer_GetWeather(u16 timerID, LPARAM lparam)
{
   GetWeather();
   Timer_ReSet(&timer, GetTime(), Timer_GetWeather, NULL);
}

int MyGuiOnCreate(DISP_OBJ *disp)
{
   mkdir(CACHE_DIR);
   xml = NULL;
   cur_disp = disp;
   cur_forecast = Cache_LoadForecast();
   if(!cur_forecast) GetWeather();
   
   DispObject_Show(disp, true);
   UpdateLayer(NULL, NULL);
   Timer_ReSet(&timer, GetTime(), Timer_GetWeather, NULL);
   return(1);
}

void MyGuiOnClose(DISP_OBJ *disp)
{
   if(timer) Timer_Kill(&timer);
   if(cur_forecast) mfree(cur_forecast);
}

void MyGuiOnLayout(DISP_OBJ *disp, void* layoutstruct)
{
   DispObject_SetLayerColor(disp, 0x00000000);
}

void MyGui_constr(DISP_DESC *desc)
{
   DISP_DESC_SetName(desc, XWeatherGuiName);
   DISP_DESC_SetSize(desc, sizeof(DISP_OBJ));
   DISP_DESC_SetOnCreate(desc, MyGuiOnCreate);
   DISP_DESC_SetOnClose(desc, MyGuiOnClose);
   DISP_DESC_SetOnRedraw(desc, MyGuiOnRedraw);
   DISP_DESC_SetOnLayout(desc, MyGuiOnLayout);
}

void MyGui_destr(GUI *desc)
{
}

GUI *CreateGUI(BOOK *book)
{
   GUI *gui = new GUI;
   if(!GUIObject_Create(gui, MyGui_destr, MyGui_constr, book, 0, 0, 0))
   {
      mfree(gui);
      return(NULL);
   }
   if(book) BookObj_AddGUIObject(book, gui);
   GUIObject_SetStyle(gui, UI_OverlayStyle_TrueFullScreen);
   GUIObject_SetTitleType(gui, UI_TitleMode_None);
   GUIObject_SoftKeys_Hide(gui);
   GUIObject_SetZOrder(gui, GuiObjectZOrderBelow, SBY_GetMainInput(Find_StandbyBook()));
   return(gui);
}

int BcfgConfig(void *mess, BOOK *book)
{
   MSG_BCFG *msg = (MSG_BCFG *)mess;
   wchar_t bcfg[FILENAME_MAX_LEN+1];
   memset(bcfg, 0, (FILENAME_MAX_LEN+1)*sizeof(wchar_t));
   wstrcpy(bcfg, msg->bcfg_p);
   wstrcat(bcfg, SPLASH);
   wstrcat(bcfg, msg->bcfg_n);
   elfload(bcfg, (void *)successed_config_path, (void *)successed_config_name, NULL);
   return(1);
}

void onBookClose(BOOK *book)
{
   GUI *XWeather = BookObj_GetGUIbyName(Find_StandbyBook(), (char *)XWeatherGuiName);
   if(XWeather) GUIObject_Destroy(XWeather);
   elf_release();
}

int UpdateLayer(void *mess, BOOK *book)
{
   //DISP_OBJ *mi = (DISP_OBJ *)mess;
   DispObject_InvalidateRect(cur_disp, NULL);
   return(1);
}

const PAGE_MSG bk_evlst_base[] @ "DYN_PAGE"  =
{
   ELF_TERMINATE_EVENT,                 TerminateElf,
   ELF_SHOW_INFO_EVENT,                 ShowAuthorInfo,
   ELF_RECONFIG_EVENT,                  ReconfigElf,
   ELF_BCFG_CONFIG_EVENT,               BcfgConfig,
   MAININPUT_HIDED_EVENT,               UpdateLayer,
   MAININPUT_SHOWED_EVENT,              UpdateLayer,
   NIL_EVENT_TAG,                       NULL
};

const PAGE_DESC bk_base = {"XWeather_Base_Page", 0, bk_evlst_base};

int isXWeatherBook(BOOK *book)
{
   if(!strcmp(book->xbook->name, BOOKNAME)) return(true);
   return(false);
}

BOOK *Find_XWeatherBook(void)
{
   return(FindBook(isXWeatherBook));
}

int main(void)
{
   BOOK *book = Find_XWeatherBook();
   if(!book)
   {
      book = new BOOK;
      memset(book, 0, sizeof(BOOK));
      if(CreateBook(book, onBookClose, &bk_base, BOOKNAME, -1, 0))
      {
         InitConfig();
         if(!CreateGUI(Find_StandbyBook())) FreeBook(book);
      }
      else
      {
         mfree(book);
         elf_release();
      }
   }
   else elf_release();
   return(RESULT_OK);
}
