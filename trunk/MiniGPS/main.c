#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "main.h"
#include "menu.h"
#include "conf_loader.h"
#include "config_data.h"
#include "language.h"
#include "util.h"

extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);
void onTimerNewAction(u16 timerID, LPARAM lparam);

#define place(num) \
  {  &cfg_place_enable##num, \
      cfg_place_name##num, \
     &cfg_place_profile##num, \
     &cfg_place_vibrate##num, \
     &cfg_place_sound_enable##num, \
      cfg_sound_file##num}

MyBOOK *MiniGPSBook = 0;

DISP_OBJ_ONREDRAW_METHOD Status_oldReDraw = 0;
DISP_OBJ ** GUI_status = 0;
DISP_DESC * Status_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw = 0;
DISP_OBJ * GUI_display = 0;
DISP_DESC * Display_desc = 0;

DISP_OBJ_ONREDRAW_METHOD Soft_oldReDraw = 0;
DISP_OBJ * GUI_soft = 0;
DISP_DESC * Soft_desc = 0;

int DisplayWidth  = 0; //ширина дисплея в пикселях
int DisplayHeight = 0; //высота дисплея в пикселях
int SoftBarY   = 0;
int StatusBarY = 0;

wchar_t logFolder[256] = {0,};

char logCellName[256] = {0,};
wchar_t CellName[256]={0,};
wchar_t CellNameAction[256]={0,};
wchar_t prevCellName[256]={0,};
wchar_t prevCellNameAction[256]={0,};
wchar_t CellNameStatus[256]={0,};

wchar_t tmppath[256] = {0,};
wchar_t tmpname[256] = {0,};

u16 timer_disable = 0;
bool IsInStandby = false;

bool visible = true;
bool showCellID   = false;
bool AutoLocation = false;

wchar_t SIwstr[MAX_AUTOLOCATION_LEN] = {0,};
wchar_t  wsnewdb[MAX_AUTOLOCATION_LEN];
char snewdb[MAX_AUTOLOCATION_LEN];

u16 currentLAC = 0;
u16 currentCID = 0;
u16 prevLAC = 0;
u16 prevCID = 0;
u16 prevLAC_action = 0;
u16 prevCID_action = 0;

int prevLocFound = -1;

u16 timer = 0;
u16 timerNewAction = 0;

int NewActionTimeout = 600000;

int encode_type = 0;
int CODEMAP_ADDR = 0x45D40000;
wchar_t encode_buf[256];
bool isUCodeFile = false;

char * db_buf = 0;

IMAGEID auto_image = NOIMAGE;
TEXTID CellNameID = empty;

IMAGEID location_image = NOIMAGE;
int imageWidth = 0;
int imageHeight = 0;

int wnd = 0x06;
const int items[10] = {0x07, 0x06, 0x00, 0x09, 0x0D, 0x0B, 0x0C, 0x0F, 0x0E, 0x06};

static char myappname[]=ELFNAME;

const int *cfg_manual_profile[MAX_MANUAL_PROFILE_NUM] = {
  &cfg_manual_profile1, &cfg_manual_profile2,
  &cfg_manual_profile3, &cfg_manual_profile4,
  &cfg_manual_profile5, &cfg_manual_profile6,
  &cfg_manual_profile7
};

cfg_place_t cfg_place[MAX_PLACE_NUM] = {
  place(1),  place(2),  place(3),  place(4),
  place(5),  place(6),  place(7),  place(8),
  place(9),  place(10), place(11), place(12),
  place(13), place(14), place(15), place(16)
};

int isImageViewerBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,"ImageViewerBook")) return(1);
  return(0);
}

int myModifyUIHook(int event1 , int event2 , int (*PROC)(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event), int mode)
{
  if((event1) && (event1 != 0xFFFFFFFF)) return ModifyUIPageHook(event1, PROC, 0, mode);
  return ModifyUIPageHook(event2, PROC, 0, mode);
}

int load_encode_file()
{
  int f;
  W_FSTAT _fstat;

  isUCodeFile = false;
  getdir(tmpname, cfg_encode_file);
  if(w_fstat(tmpname, &_fstat) == 0)
  {
    if( ((f = w_fopen(tmpname, WA_Read, 0x1FF, NULL)) >= 0) && _fstat.st_size == 512 )
    {
      char*buf=(char*)malloc(512);
      w_fread(f, buf, 512);
      for(int i=0; i<256; i++)
      {
        encode_buf[i] = (buf[2 * i] << 8)  | buf[2 * i + 1];
      }
      mfree(buf);
      isUCodeFile=true;
    }
    w_fclose(f);
  }
  return isUCodeFile;
}

wchar_t *char2unicode(wchar_t *ws, char *s, int len)
{
  int c;
  wchar_t *ws_ret = ws;
  while((c = *s++) && ((len--) > 0))
  {
    if (c == 0x7E) c = 0x0A;
    if (isUCodeFile) c = encode_buf[c];
    *ws++ = c;
  }
  *ws=0;
  return ws_ret;
}

char w2c(wchar_t s)
{
  if (isUCodeFile)
  {
    for(char i=0; i<256; i++)
    {
      if(encode_buf[i] == s) return i;
    }
  }
  return s;
}

char *unicode2char(char *s, wchar_t *ws, int len)
{
  wchar_t x;
  char *s_ret = s;
  while((x = *ws++) && ((len--) > 0))
  {
    *s++ = w2c(x);
  }
  *s=0;
  return s_ret;
}

void gb2un(wchar_t *des, char *sou, int len)
{
  size_t i = 0;
  size_t j = 0;
  unsigned char *sour = (unsigned char *)sou;
  SWAP_T a;
  int sou_len = strlen(sou);
  while((i < sou_len) && ((len--) > 0))
  {
    if(sour[i] < 0x80)
    {
      des[j] = sour[i];
      i++;
    }
    else if((sour[i] > 0x80) || (sour[i] == 0x80 && sour[i+1] >= 0x41))
    {
      a.low = sour[i];
      a.high = sour[i + 1];
      long tmp = a.data - BASE_CODE;
      des[j] = *((wchar_t *)CODEMAP_ADDR + tmp);
      i += 2;
    }
    else
    {
      des[j] = '?';

    }
    j++;
  }
  des[j] = 0;
}

void un2gb(char *des, wchar_t *sou, int len)
{
  int i;
  int j = 0;
  SWAP_T a;
  int sou_len = wstrlen(sou);
  if(sou_len > len) sou_len = len;

  for(i = 0; i < sou_len; i++)
  {
    if(*(sou + i) <= 0x80)
    {
      des[j] = sou[i];
      j++;
    }
    /*
    else if(*(sou + i) == 0x3000)
    {
      des[j] = 0xA1;
      des[j + 1] = 0xA1;
      j += 2;
    }
    */
    else if(*(sou + i) >= BASE_MIN)
    {
      int k = 0;
      a.data = *(sou + i);
      while(k <= BASE_LEN)
      {
        if(*((wchar_t*)CODEMAP_ADDR + k) == a.data)
        {
          a.data = k + BASE_CODE;
          des[j] = a.low;
          des[j + 1] = a.high;
          j += 2;
          break;
        }
        k++;
      }
    }
    else
    {
      des[j] = '?';
      j++;
    }
  }
  des[j] = 0;
}

void DrawHighlightID(int Font,int StrID,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
     SetFont(Font);
     if(cfg_highlight)
     {
         DrawString(StrID,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
         DrawString(StrID,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,0,1,borderColor,borderColor);
         DrawString(StrID,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
         DrawString(StrID,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,0,1,borderColor,borderColor);
     }
     DrawString(StrID,CEN,XPos,YPos,MaxXPos,MaxYPos,0,1,NormalColor,NormalColor);
}

char *load_db_file()
{
  int f;
  W_FSTAT _fstat;
  getdir(tmpname, cfg_location_file);
  if (w_fstat(tmpname, &_fstat) == 0) {
    if ((f = w_fopen(tmpname, WA_Read, 0x1FF, NULL)) >= 0) {
      char * buffer = new char[_fstat.st_size + 1];
      if(buffer) {
        buffer[w_fread(f, buffer, _fstat.st_size)] = 0;
        w_fclose(f);
        if(db_buf) delete(db_buf);
        db_buf = buffer;
        return db_buf;
      }
    }
  }

  if(db_buf) delete(db_buf);
  db_buf = 0;
  return db_buf;
}

int AddTo_db()
{
  int status = 0;
  int f;

  getdir(tmpname, cfg_location_file);
  if ((f = w_fopen(tmpname, WA_Append, 0x1FF, NULL)) >= 0)
  {
    if(cfg_cellidmode == 0){
      snwprintf(wsnewdb, MAX_AUTOLOCATION_LEN - 1, L"[%04X:%04X]:%ls\r\n", currentLAC, currentCID, SIwstr);
    }
    else {
      snwprintf(wsnewdb, MAX_AUTOLOCATION_LEN - 1, L"[%05d:%05d]:%ls\r\n", currentLAC, currentCID, SIwstr);
    }
    if(encode_type == 0)
    {
      unicode2char(snewdb, wsnewdb, MAX_AUTOLOCATION_LEN - 1);
    }
    else
    {
      un2gb(snewdb, wsnewdb, MAX_AUTOLOCATION_LEN - 1);
    }
    w_fwrite(f, snewdb, strlen(snewdb));
    w_fclose(f);
    status = 1;
  }
  load_db_file();
  if (!showCellID) wstrcpy(CellNameStatus, SIwstr);
  return status;
}

void vibrate(int mode)
{
  if (mode > 0) {
    PAudioControl pAC = AudioControl_Init();
    if( !pAC ) pAC = *GetAudioControlPtr();
    if (cfg_vibrate == 1) AudioControl_Vibrate(pAC, 70, 70, 350);
    if (cfg_vibrate == 2) AudioControl_Vibrate(pAC, 250, 0, 250);
    if (cfg_vibrate == 3) AudioControl_Vibrate(pAC, 500, 0, 500);
  }
}

void GetLocationImage(wchar_t *path, wchar_t *name)
{
  if(ImageID_Get(path,name,&location_image)<0)
  {
    location_image = NOIMAGE;
  }
  else
  {
    int Width  = GetImageWidth(location_image);
    int Height = GetImageHeight(location_image);
    int Width0  = cfg_location_image.x2 - cfg_location_image.x1;
    int Height0 = cfg_location_image.y2 - cfg_location_image.y1;
    if((imageWidth = (Width * Height0) / Height) <= Width0)
    {
      imageHeight = Height0;
    }
    else
    {
      imageWidth  = Width0;
      imageHeight = (Height * Width0) / Width;
    }
  }
}

void UpdateLocationImage()
{
  FSTAT _fstat;
  imageWidth = imageHeight = 0;
  if(location_image != NOIMAGE)
  {
    ImageID_Free(location_image);
    location_image = NOIMAGE;
  }

  if(cfg_show_type & 2)
  {
    getdir(tmppath, cfg_images_path);
    wstrcpy(tmpname,CellName);
    wstrcat(tmpname,L".png");

    if (fstat(tmppath,tmpname,&_fstat) == 0)
    {
      GetLocationImage(tmppath,tmpname);
    }
    else
    {
      wstrcpy(tmpname,CellName);
      wstrcat(tmpname,L".jpg");
      if (fstat(tmppath,tmpname,&_fstat) == 0)
      {
        GetLocationImage(tmppath,tmpname);
      }
    }
  }
}

void InitVar()
{
  InitConfig();

  StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, empty);
  wnd = items[cfg_location];

  NewActionTimeout = cfg_new_action_timeout * 60000;

  DisplayWidth  = Display_GetWidth(0);
  DisplayHeight = Display_GetHeight(0);

  StatusBarY    = DispObject_GetWindowHeight(*StatusRow_p());
  SoftBarY      = DisplayHeight - DispObject_GetWindowHeight(DispObject_SoftKeys_Get());

  FSTAT _fstat;
  if(auto_image != NOIMAGE)
  {
    ImageID_Free(auto_image);
    auto_image = NOIMAGE;
  }

  if(cfg_auto_image_show)
  {
    extractdir(tmppath, tmpname, cfg_auto_file);
    if(fstat(tmppath,tmpname,&_fstat)!=0)
    {
      MessageBox(EMPTY_TEXTID, TextID_Create(LG_AUTOLOCATIONIMG,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }
    else if(ImageID_Get(tmppath,tmpname,&auto_image)<0) auto_image = NOIMAGE;
  }

  isUCodeFile = false;
  if(cfg_encode_type == 0)
  {
    encode_type = 0;
    if(!load_encode_file())
    {
      MessageBox(EMPTY_TEXTID, TextID_Create(LG_GFGENCODEFILENOTFOUND,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }
  }
  else
  {
    encode_type = 1;
    CODEMAP_ADDR = (int)wcstoul(cfg_codemap_addr,0,0);
    if(CODEMAP_ADDR == 0)
    {
      encode_type = 0;
      MessageBox(EMPTY_TEXTID, TextID_Create(LG_GFGINCORRECTCODEMAPADDR,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    }
  }

  if(!load_db_file())
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LG_DBLOADERROR,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
  }

  getdir(logFolder, cfg_logs_path);
}

int getPlaceIndex(const wchar_t *name)
{
  for(unsigned int i=0; i<MAX_PLACE_NUM; i++)
  {
    if(*cfg_place[i].enable)
    {
      if(wstrcmp(cfg_place[i].name, name) == 0) return i;
    }
  }
  return -1;
}

int getProfile(int indx)
{
  if((indx >= 0) && (indx < MAX_PLACE_NUM))
  {
    if(*cfg_place[indx].profile) return *cfg_place[indx].profile - 1;
  }
  return -1;
}

int getVibrate(int indx)
{
  if((indx >= 0) && (indx < MAX_PLACE_NUM)) return *cfg_place[indx].vibrate;
  return -1;
}

const wchar_t * getSoundFile(int indx)
{
  if((indx >= 0) && (indx < MAX_PLACE_NUM))
  {
    if(*cfg_place[indx].sound_enable) return cfg_place[indx].sound_file;
  }
  return 0;
}

int equ2manual_profile(int profile)
{
  for(unsigned int i=0; i<MAX_MANUAL_PROFILE_NUM; i++)
  {
    if(*cfg_manual_profile[i])
    {
      if((*cfg_manual_profile[i] - 1) == profile) return 1;
    }
  }
  return 0;
}

void DrawParams(int y)
{
  GC *GC_DISP=get_DisplayGC ();

  if ((cfg_show_type & 2) && visible && (location_image != NOIMAGE))
  {
    GC_PutChar(GC_DISP, cfg_location_image.x1, cfg_location_image.y1 - y, imageWidth, imageHeight, location_image);
  }

  if(cfg_auto_image_show && AutoLocation && (auto_image != NOIMAGE))
  {
    GC_PutChar(GC_DISP, cfg_auto_image_x, cfg_auto_image_y - y, 0, 0, auto_image);
  }

  if((cfg_location == 9) && (cfg_show_type & 1) && visible && (CellNameID != empty))
  {
    if(cfg_cell_align == RIGHT)
      DrawHighlightID(cfg_cell_font, CellNameID, cfg_cell_align, 1, cfg_cell_y - y, cfg_cell_x, DisplayHeight, cfg_cell_border, cfg_cell_color);
    else
      DrawHighlightID(cfg_cell_font, CellNameID, cfg_cell_align, cfg_cell_x, cfg_cell_y - y, DisplayWidth, DisplayHeight, cfg_cell_border, cfg_cell_color);
  }

}

void onTimer_disable (u16 timerID, LPARAM lparam)
{
  IsInStandby = false;
}

void Display_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
  IsInStandby = true;
  Timer_ReSet(&timer_disable,cfg_update_interval << 1, onTimer_disable,0);
  DrawParams(StatusBarY);
  if (Display_oldReDraw) Display_oldReDraw(DO,a,b,c);
}

void Status_ReDraw(DISP_OBJ * d, int a, int b, int c)
{
  Status_oldReDraw(d, a, b, c);
  if(IsInStandby || (!cfg_standby_only)) DrawParams(0);
}

void Soft_ReDraw(DISP_OBJ* DO,int a,int b,int c)
{
  BOOK *bk1 = FindBook(isImageViewerBook);
  BOOK *bk2 = FindBook(get_IsCameraBook());
  bool flag = (!cfg_standby_only) && (!bk1) && (!bk2);

  if(IsInStandby)
  {
    DrawParams(SoftBarY);
    Soft_oldReDraw(DO,a,b,c);
  }
  else
  {
    if(flag) DrawParams(SoftBarY);
    Soft_oldReDraw(DO,a,b,c);
  }
}

void InvalidateAll()
{
    if(GUI_display) DispObject_InvalidateRect(GUI_display,0);
    if(GUI_status)  DispObject_InvalidateRect(*GUI_status,0);
    if(GUI_soft)    DispObject_InvalidateRect(GUI_soft,0);
}

int CheckCurrentCell()
{
  if(cfg_cellidmode == 0){
    snwprintf(CellName, 16, L"%04X:%04X", currentLAC, currentCID);
  }
  else {
    snwprintf(CellName, 16, L"%05d:%05d", currentLAC, currentCID);
  }

  wstrcpy(CellNameStatus, CellName);
  wstr2strn(logCellName, CellName, 15);

  char lacCidKey[20];
  sprintf(lacCidKey, "[%s]", logCellName);

  int LocFound = 0;
  if(db_buf)
  {
    if (char * param = manifest_GetParam(db_buf, lacCidKey, 0)) {
      strcpy(logCellName, param);
      if(encode_type == 0)
      {
        char2unicode(CellName, param, 255);
      }
      else
      {
        gb2un(CellName, param, 255);
      }
      if (!showCellID) wstrcpy(CellNameStatus, CellName);
      LocFound = 1;
      mfree(param);
    }
  }

  if ((LocFound == 0) && (AutoLocation == true)) {
    if(AddTo_db()) LocFound = 1;
  }

  if(CellNameID != empty) TextID_Destroy(CellNameID);
  CellNameID = TextID_Create(CellNameStatus,ENC_UCS2,TEXTID_ANY_LEN);

  UpdateLocationImage();

  if(cfg_show_type) InvalidateAll();

  return LocFound;
}

void CheckCellName()
{
  int file;

  vibrate(cfg_vibrate);

  prevLAC = currentLAC;
  prevCID = currentCID;
  wstrcpy(prevCellName, CellName);


  int LocFound = CheckCurrentCell();

  if((( wstrcmp(CellName, prevCellName) != 0) && (LocFound == 1)) || (LocFound != prevLocFound))
  {
    Timer_ReSet(&timerNewAction, NewActionTimeout, onTimerNewAction, 0);
  }

  prevLocFound = LocFound;

  if(cfg_log_mode == 0) return;
  if((cfg_log_mode == 0x02) && (wstrcmp(CellName, prevCellName) == 0)) return;
  if((cfg_log_mode == 0x03) && (LocFound == 0)) return;
  if((cfg_log_mode == 0x04) && (LocFound == 1)) return;

  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC, &dt);

  wchar_t * logFile = new wchar_t[16];
  if (cfg_date_format)
    snwprintf(logFile, 11, _T("%02d-%02d-%04d."), dt.date.day, dt.date.mon, dt.date.year);
  else
    snwprintf(logFile, 11, _T("%04d-%02d-%02d."), dt.date.year, dt.date.mon, dt.date.day);
  wstrcat(logFile, (cfg_log_ext ? _T("ini"):_T("txt")));

  if ((file = _fopen(logFolder, logFile, FSX_O_CREAT|FSX_O_APPEND|FSX_O_RDWR, FSX_S_IREAD|FSX_S_IWRITE, 0)) >= 0)
  {
    char * newlog = new char[strlen(logCellName) + 32];
    if(cfg_cellidmode == 0){
      sprintf(newlog, "%02d:%02d:%02d - [%04X:%04X] %s\n", dt.time.hour, dt.time.min, dt.time.sec, currentLAC, currentCID, logCellName);
    }
    else {
      sprintf(newlog, "%02d:%02d:%02d - [%05d:%05d] %s\n", dt.time.hour, dt.time.min, dt.time.sec, currentLAC, currentCID, logCellName);
    }

    fwrite(file, newlog, strlen(newlog));
    fclose(file);
    delete(newlog);
  }
  delete(logFile);
  return;
}

void onTimerNewAction(u16 timerID, LPARAM lparam)
{
  wstrcpy(prevCellNameAction, CellNameAction);
  wstrcpy(CellNameAction, CellName);

  int indx = getPlaceIndex(CellNameAction);
  if ( wstrcmp(CellNameAction, prevCellNameAction) != 0)
  {
    vibrate(getVibrate(indx));

    const wchar_t *soundFile = getSoundFile(indx);
    if(soundFile && (!GetSilent() || (cfg_silent==1)))
    {
      char vol;
      GetAudioSettings(2,&vol);
      extractdir(tmppath, tmpname, soundFile);
      PlayFileV(tmppath, tmpname, vol);
    }
  }

  int req_profile = getProfile(indx);
  int cur_profile;
  REQUEST_PROFILE_GETACTIVEPROFILE(SYNC , &cur_profile);
  if(req_profile >= 0)
  {
    if(!equ2manual_profile(cur_profile))
    {
      Profile_SetActive(req_profile,cfg_set_in_gdfs);
    }
  }
  else if(cfg_default_profile > 0)
  {
    req_profile = cfg_default_profile - 1;
    if(!equ2manual_profile(cur_profile))
    {
      Profile_SetActive(req_profile,cfg_set_in_gdfs);
    }
  }

  Timer_ReSet(&timerNewAction, NewActionTimeout, onTimerNewAction, 0);
}

void onTimer(u16 timerID, LPARAM lparam)
{
  PLMN_LAC_DESC plmn_lac;
  RAT_CI_DESC rat_ci;
  char CSReg;
  get_CellData(&plmn_lac,&rat_ci,&CSReg);
  currentLAC = plmn_lac.LAC[0]<<8|plmn_lac.LAC[1];
  currentCID  = rat_ci.CI;
  if((currentLAC != prevLAC) || (currentCID != prevCID)) CheckCellName();
  if (visible && (cfg_location < 9)) {
    StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, TextID_Create(CellNameStatus,ENC_UCS2,TEXTID_ANY_LEN));
  }
  Timer_ReSet(&timer, cfg_update_interval, onTimer, 0);
}

void Send_REDRAW_RELEASE()
{
  REDRAW_RELEASE_MESSAGE * sbm = new REDRAW_RELEASE_MESSAGE;

  // Status
  sbm->SI_OldOnRedraw = Status_oldReDraw;
  sbm->SI_NewOnRedraw = Status_ReDraw;
  // Screen
  sbm->SB_OldOnRedraw = Display_oldReDraw;
  sbm->SB_NewOnRedraw = Display_ReDraw;
  // Soft
  sbm->SK_OldOnRedraw = Soft_oldReDraw;
  sbm->SK_NewOnRedraw = Soft_ReDraw;

  // посылаем всем  мессагу со старыми и своими методами Redraw
  UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, sbm, (void (*)(void*))mfree_adr());
}

int SB_ELF_Killed(void *mess ,BOOK* book)
{
  // если был убит эльф рисующий на ГЭ или просто нужно перетосовать методы
  REDRAW_RELEASE_MESSAGE * sbm =(REDRAW_RELEASE_MESSAGE*)mess;
  REDRAW_RELEASE_MESSAGE * ms = 0;

  //Проверим статус
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SI_NewOnRedraw == Status_oldReDraw)
  {
    ms = new REDRAW_RELEASE_MESSAGE;
    memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SI_OldOnRedraw!=EMPTY_REDRAW_METHOD) Status_oldReDraw = sbm->SI_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(*GUI_status), Status_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SI_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SI_NewOnRedraw = Status_ReDraw;
  }

  //Проверим главный экран
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SB_NewOnRedraw == Display_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SB_OldOnRedraw!=EMPTY_REDRAW_METHOD) Display_oldReDraw = sbm->SB_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_display), Display_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SB_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SB_NewOnRedraw = Display_ReDraw;
  }

  //Проверим софт
  //его ли метод мы используем в качестве oldRedraw?
  if (sbm->SK_NewOnRedraw == Soft_oldReDraw)
  {
    if(!ms)
    {
      ms = new REDRAW_RELEASE_MESSAGE;
      memcpy(ms, sbm, sizeof(REDRAW_RELEASE_MESSAGE));
    }

    // если он был убит, то заменяем свой oldRedraw на его..
    if (sbm->SK_OldOnRedraw!=EMPTY_REDRAW_METHOD) Soft_oldReDraw = sbm->SK_OldOnRedraw;

    // ставим свой метод наверх
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(GUI_soft), Soft_ReDraw);

    // и шлём мессагу снова, чтоб следующие ельфы сделали тоже самое
    ms->SK_OldOnRedraw = EMPTY_REDRAW_METHOD;
    ms->SK_NewOnRedraw = Soft_ReDraw;
  }
  if(ms)
  {
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT ,ms,(void (*)(void*))mfree_adr());
    return(BLOCK_EVENT_GLOBALLY);
  }
  return(0);
}
int TerminateElf(void * ,BOOK* book)
{
  Send_REDRAW_RELEASE();
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
#if (!defined(LANG_RU) && !defined(LANG_EN))
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO _T("\n") LTRANSLATED TRANSLATED_BY,0,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
#else
  MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
#endif
  return(1);
}

static int onReconfigElf(void *mess , BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LG_UPDSETTING,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    InitVar();
    CheckCurrentCell();

    if (visible && (cfg_location < 9)) {
      StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, TextID_Create(CellNameStatus,ENC_UCS2,TEXTID_ANY_LEN));
    }

    result=1;
  }
  return(result);
}

int onBcfgConfig(void* mess, BOOK* bk)
{
  FSTAT _fstat;

  extractdir(tmppath, tmpname, cfg_bcfgedit);
  if(fstat(tmppath,tmpname,&_fstat)==0)
  {
  }
  else if(fstat(GetDir(DIR_ELFS|MEM_INTERNAL), tmpname,&_fstat)==0)
  {
    wstrcpy(tmppath,GetDir(DIR_ELFS|MEM_INTERNAL));
  }
  else if(fstat(GetDir(DIR_ELFS|MEM_EXTERNAL), tmpname,&_fstat)==0)
  {
    wstrcpy(tmppath,GetDir(DIR_ELFS|MEM_EXTERNAL));
  }
  else
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LG_GFGBCFGEDITNOTFOUND,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    return 1;
  }
  wstrcat(tmppath,L"/");
  wstrcat(tmppath, tmpname);
  elfload(tmppath, (void*)successed_config_path, (void*)successed_config_name,0);
  return 1;
}

const PAGE_MSG MiniGPS_BasePageEvents[]@ "DYN_PAGE" ={
  SBY_REDRAW_RELEASE_EVENT,  SB_ELF_Killed,
  ELF_TERMINATE_EVENT,       TerminateElf,
  ELF_SHOW_INFO_EVENT,       ShowAuthorInfo,
  ELF_RECONFIG_EVENT,        onReconfigElf,
  NIL_EVENT_TAG,             NULL
};

PAGE_DESC base_page ={"MiniGPS_BasePage",0,MiniGPS_BasePageEvents};


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int isMiniGPSBook(BOOK * book)
{
  if(!strcmp(book->xbook->name,myappname)) return 1;
  return 0;
}

void ShowHideProc()
{
  if(cfg_location < 9)
  {
    if (visible)
    {
      StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, empty);
    }
    else
    {
      StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, TextID_Create(CellNameStatus,ENC_UCS2,TEXTID_ANY_LEN));
    }
  }
  visible = !visible;
}

void CellIdProc()
{
  showCellID = !showCellID;
  CheckCurrentCell();
}

void OKPressed(BOOK * bk, wchar_t *string, int len)
{
  MyBOOK * myBook=(MyBOOK *)bk;
  wstrcpy(SIwstr, string);
  FREE_GUI(myBook->text_input);
  AutoLocation=true;
  CheckCurrentCell();
  BookObj_Hide((BOOK*)myBook,0);
//  BookObj_SetFocus(Find_StandbyBook(),0);
  return;
}

void BackPressed(BOOK * bk, u16 *string, int len)
{
  MyBOOK * myBook=(MyBOOK *)bk;
  FREE_GUI(myBook->text_input);
  BookObj_Hide((BOOK*)myBook,0);
//  BookObj_SetFocus(Find_StandbyBook(),0);
  return;
}

int NewKey(int key, int r1, int mode, LPARAM, DISP_OBJ*)
{
  if(!isKeylocked() || cfg_ignore_keylock)
  {
    if(Display_GetTopBook(0)==Find_StandbyBook())
    {
      if(cfg_ctrlmode == 0)
      {
        if((key == cfg_menukey) && (mode == cfg_menukeymode))
        {
          ShowMenu(&MiniGPSBook->book);
        }
      }
      else
      {
        if((key == cfg_showkey) && (mode == cfg_showkeymode))
        {
          ShowHideProc();
        }
        else if((key == cfg_showcellid) && (mode == cfg_chowcellidmode))
        {
          CellIdProc();
        }
        else if((key == cfg_autolocation) && (mode == cfg_autolocationmode))
        {
          if (AutoLocation == false)
          {
            if(MiniGPSBook->text_input) GUIObject_Destroy(MiniGPSBook->text_input);
            TEXTID text = TextID_Create(SIwstr,ENC_UCS2,TEXTID_ANY_LEN);
            MiniGPSBook->text_input = CreateStringInputVA(0,
                                          VAR_BOOK(MiniGPSBook),
                                          VAR_STRINP_FIXED_TEXT(TextID_Create(LG_CURRENTLOCATION,ENC_UCS2,TEXTID_ANY_LEN)),
                                          VAR_STRINP_TEXT(text),
                                          VAR_STRINP_NEW_LINE(0),
                                          VAR_STRINP_ENABLE_EMPTY_STR(0),
                                          VAR_STRINP_MAX_LEN(MAX_AUTOLOCATION_LEN - 1),
                                          VAR_STRINP_MODE(IT_STRING),
                                          VAR_OK_PROC(OKPressed),
                                          VAR_PREV_ACTION_PROC(BackPressed),
                                          0);
            BookObj_SetFocus( &MiniGPSBook->book,0);
            GUIObject_Show(MiniGPSBook->text_input);
          }
          else
          {
            AutoLocation = false;
            MessageBox(EMPTY_TEXTID, TextID_Create(LG_AUTOLOCATIONOFF,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
          }
        }
        InvalidateAll();
      }
    }
  }
  return 0;
}

int StandbyModeActivatedHook(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event)
{
    IsInStandby = true;
    return(0);
}

int StandbyModeDeactivatedHook(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event)
{
    IsInStandby = false;
    return(0);
}

void onCloseMiniGPSBook(BOOK * book)
{
  MyBOOK *mbk = (MyBOOK *)book;
  if (mbk)
  {
    myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,0);
    myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,0);

    if(Display_desc) DISP_DESC_SetOnRedraw(Display_desc, Display_oldReDraw);
    if(Status_desc)  DISP_DESC_SetOnRedraw(Status_desc, Status_oldReDraw);
    if(Soft_desc)    DISP_DESC_SetOnRedraw(Soft_desc, Soft_oldReDraw);

    if(timer_disable) Timer_Kill(&timer_disable);
    if(timer) Timer_Kill(&timer);
    if(timerNewAction) Timer_Kill(&timerNewAction);
    ModifyKeyHook(NewKey, KEY_HOOK_REMOVE, NULL);

    StatusIndication_SetItemText(SBY_GetStatusIndication(Find_StandbyBook()), wnd, empty);

    FREE_GUI(mbk->menu);
    FREE_GUI(mbk->text_input);

    if(auto_image != NOIMAGE)
    {
      ImageID_Free(auto_image);
      auto_image = NOIMAGE;
    }

    if(location_image != NOIMAGE)
    {
      ImageID_Free(location_image);
      location_image = NOIMAGE;
    }

    if(CellNameID != empty)
    {
      TextID_Destroy(CellNameID);
      CellNameID = empty;
    }

    if(db_buf) delete(db_buf);
    SUBPROC(elf_exit);
  }
}

int MainPageEnter(void *, BOOK *bk)
{
  myModifyUIHook(STANDBY_NOT_IDLE_EVENT,UI_STANDBY_UNFOCUS_EVENT,StandbyModeDeactivatedHook,1);
  myModifyUIHook(STANDBY_IDLE_EVENT,PHONE_IN_STBY_EVENT,StandbyModeActivatedHook,1);
  
  GUI_status = StatusRow_p();
  Status_desc = DispObject_GetDESC (* GUI_status);
  Status_oldReDraw = DispObject_GetOnRedraw (* GUI_status);
  DISP_DESC_SetOnRedraw (Status_desc, (DISP_OBJ_ONREDRAW_METHOD)Status_ReDraw);
  
  GUI_soft = DispObject_SoftKeys_Get();
  Soft_desc = DispObject_GetDESC (GUI_soft);
  Soft_oldReDraw = DispObject_GetOnRedraw(GUI_soft);
  DISP_DESC_SetOnRedraw(Soft_desc, (DISP_OBJ_ONREDRAW_METHOD)Soft_ReDraw);
  
  GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
  Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
  Display_desc = DispObject_GetDESC (GUI_display);
  DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
  
  ModifyKeyHook(NewKey, KEY_HOOK_ADD, NULL);
  Timer_ReSet(&timer, 1000, onTimer, 0);
  Timer_ReSet(&timerNewAction, NewActionTimeout, onTimerNewAction, 0);
  return(0);
}

const PAGE_MSG MiniGPS_MainPageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,      MainPageEnter,
  NIL_EVENT_TAG,             NULL
};

PAGE_DESC main_page ={"MiniGPS_MainPage",0,MiniGPS_MainPageEvents};

int main (void)
{
  if(FindBook(isMiniGPSBook))
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(LELFNAME _T("\n") LG_ALREADY_STARTED,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 1, 5000, 0);
    SUBPROC(elf_exit);
  }
  else
  {

    InitVar();
    MiniGPSBook = new MyBOOK;
    memset(MiniGPSBook,0,sizeof(MyBOOK));
    if(!CreateBook((BOOK*)MiniGPSBook,onCloseMiniGPSBook,&base_page,ELFNAME,-1,0))
    {
      delete MiniGPSBook;
      SUBPROC(elf_exit);
      return 0;
    }
    
    BookObj_GotoPage((BOOK*)MiniGPSBook,&main_page);

  }
  return 0;
}

/*
Revision history:
  2.6
      + Изменения в структуре конфигурационного файла.
      + Возможность вкл/выкл меню.
      + Возможность выбора где отображать местополжение - только в режиме ожидания или везде.
      + В меню добавлен пункт "Об эльфе".
      + Исправлена ошибка с отображением "софтов" в альбомной ориентации в камере и при просмотре изображений.
  2.5.1
      + Исправлена мелкая ошибка
  2.5
      + Изменения в структуре конфигурационного файла.
      + Добавлено меню
      + Формат картинки, соответсвующей данному месту, может быть как png, так и jpg
        Примечание. Если если есть картинки обоих форматов, то будет загружена в формате png.
  2.4
      + Возвращена возможность вывода имени в фиксированные места.
      + Добавлена поддержка китайского языка.
      + Исправлены мелкие ошибки
  2.3
      + Немного переделан алгоритм перерисовки.
      + Исправлены мелкие ошибки
  2.2
      + Изменения в структуре конфигурационного файла.
      + Возможность задать шрифт, цвет, выравнивание и координаты для выводимого
        на ГЭ имени местоположения.
      + Отображение на экране в виде иконки включенного режима "автодобавления места".
      + Отображение на экране картинки соответсвующей данному месту.
        Имя картинки в виде <имя места>.png (например, дом.png)
      + Возможность выбора что отображать.
      + Немного изменен алгоритм переключения профиля в зависимости от местоположения.
        Теперь переключение происходит не только при смене места, но и при выключении
        профиля, включенного "вручную".
      + Немного изменен алгоритм включения профиля по умолчанию. Теперь он так же
        включается если в течении времени "Задержка действия (мин)" телефон не был
        подключен ни к одной из базовых станций, имена которых введены в "Места"
      + Небольшая оптипизация кода
  2.1
      + Исправлена проблема с некорректным сохранением и последущем выводом на
        экран имени места, введенного не латинскими сиволами (например, русскими т.д.)
      + Теперь при включении режима "автодобавления места" в базу добавляются не
        только вновь найденные станции, но и текущая (если ее там не было)
      + Небольшая оптипизация кода и исправление ошибок
  2.0
      + Переход на bcfg (за основу взята версия 1.19.2)
      + Автоматический выбор громкости воспроизведения звуков
      + Небольшая оптипизация кода и исправление ошибок

*/
