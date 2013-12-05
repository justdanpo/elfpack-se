#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "config_data.h"

typedef struct
{
    BOOK bk;
    void * update;
    wchar_t *path;
    wchar_t *name;
    GUI *remind;
    LIST *lst;
    LIST *remlst;
    GUI_LIST *g_test;
    GUI_FEEDBACK *g_mbox;
}EVTBOOK;

  typedef struct
  {
    bool done;
    signed int tm_start;
    signed int tm_end;
    signed int ask_before;
    signed int ask_after;
    int remdays[7];
    int remdays2[7];
    bool after_done;
    bool before_done;
    //-------------------
    int type;
    int AskBefore;
    bool isMinusInBefore;
    int AskAfter;
    bool isGps;
    bool isMinusInAfter;
    int dtype;
    DATE date;
    DATE date2;
    int days[7];
    int ttype;
    TIME time;
    TIME time2;
    wchar_t *param;
    wchar_t *txt;
    char *gps;
    DATE date_0;
    DATE date_1;
    int d_shift_0;
    int d_shift_1;
    int day_1;
    int day_2;
    int rem;
}EVENT;
  
typedef struct
{
    wchar_t *text;
    wchar_t *utext;
    wchar_t *time;
    bool checked;
}REMIND;

DISP_OBJ_ONREDRAW_METHOD Display_oldReDraw = 0;
DISP_OBJ * GUI_display = 0;
DISP_DESC * Display_desc = 0;

int Height,Width;
u16 timer;
BOOK * CreateEvtDeskBook();

int _x=100;
int _y=100;
int _dx=2;
int _dy=3;

#define GET_BLUE(x) ((unsigned int)x&0xFF)
#define GET_GREEN(x) (((unsigned int)x>>8)&0xFF)
#define GET_RED(x) (((unsigned int)x>>16)&0xFF)
#define GET_ALPHA(x) (((unsigned int)x>>24)&0xFF)

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

void onTimer (u16 tmr , LPARAM)
{
  DispObject_InvalidateRect(GUI_display,0);
  Timer_ReSet(&timer,1000,onTimer,0);
};

void DrawGradient(int x, int y, int x2, int y2, int upc, int loc)
{
  int r1=GET_RED(upc),g1=GET_GREEN(upc),b1=GET_BLUE(upc);
  int r2=GET_RED(loc),g2=GET_GREEN(loc),b2=GET_BLUE(loc);
  int a1=GET_ALPHA(upc),a2=GET_ALPHA(loc);
  int i;
  for(i=0;i<x2-x;i++)
  { 
    int r,g,b,a;
    r = r1 + (i * (r2-r1) / (x2-x));
    g = g1 + (i * (g2-g1) / (x2-x));
    b = b1 + (i * (b2-b1) / (x2-x));
    a = a1 + (i * (a2-a1) / (x2-x));
    int cl=a*0x1000000 + r * 0x10000 + g *0x100 + b;
    DrawRect(x+i,y,x+i+1,y2,cl,cl);
  }
};

void DrawGradientVert(int x, int y, int x2, int y2, int upc, int loc)
{
  int r1=GET_RED(upc),g1=GET_GREEN(upc),b1=GET_BLUE(upc);
  int r2=GET_RED(loc),g2=GET_GREEN(loc),b2=GET_BLUE(loc);
  int a1=GET_ALPHA(upc),a2=GET_ALPHA(loc);
  int i;
  for(i=0;i<y2-y;i++)
  { 
    int r,g,b,a;
    r = r1 + (i * (r2-r1) / (y2-y));
    g = g1 + (i * (g2-g1) / (y2-y));
    b = b1 + (i * (b2-b1) / (y2-y));
    a = a1 + (i * (a2-a1) / (y2-y));
    int cl=a*0x1000000 + r * 0x10000 + g *0x100 + b;
    DrawRect(x, y+i, x2, y+i+1, cl, cl);
  }
};
void DrawLine(int str, int ct, int x, int y, int x2, int y2, int between, int c)
{
  DrawString(str,ct,x,y,x2,y2,between,3,c,c);
  TextID_Destroy(str);
};

int isEvtBook(BOOK *bk)
{
  if (strcmp(bk->xbook->name,"EventControl")==0)return 1;
  return 0;
};

  int cmpdate(DATE d1, DATE d2)
{
  if      (d1.year<d2.year)return 2;
  else if (d1.year>d2.year)return 0;
  else
  {
    if      (d1.mon<d2.mon) return 2;
    else if (d1.mon>d2.mon) return 0;
    else
    {
      if (d1.day<d2.day)return 2;
      else if (d1.day>d2.day)return 0;
      else return 1;
    }
  }
};

void Display_ReDraw(DISP_OBJ * db,int r1, int r2,int r3)
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(0,&dt);
  int now=(dt.time.hour*3600) + (dt.time.min*60) + dt.time.sec;
  char day;
  DATE_GetWeekDay(&dt.date, &day);
  SetFont(ev_font);
  DrawGradientVert(rc.x1, rc.y1, rc.x2, rc.y2, upp_color, low_color);
  DrawLine(TextID_Create(str_events,ENC_UCS2,TEXTID_ANY_LEN), 0, rc.x1,rc.y1,rc.x2,rc.y2,40, ev_color);
  EVTBOOK *bk=(EVTBOOK*)FindBook(isEvtBook);
  int pos=rc.y2;
  if (!bk)
  {
    DrawGradientVert(rc2.x1, pos, rc2.x2, pos+rc2.y2, low_color, low_color);
    DrawLine(TextID_Create(str_off,ENC_UCS2,TEXTID_ANY_LEN), 2, rc2.x1, pos, rc2.x2, pos+rc2.y2, 40, clWhite);
    return;
  }
  SetFont(text_font);
  int x=0;
  int res=0;
  if (bk->remlst && bk->remlst->FirstFree)
  {
    while (x<bk->remlst->FirstFree)
    {
      REMIND *rem=(REMIND*)bk->remlst->listdata[x++];
      DrawGradientVert(rc2.x1, pos, rc2.x2, pos+rc2.y2, miss_color, miss_color);
      DrawLine(TextID_Create(rem->text,ENC_UCS2,TEXTID_ANY_LEN), 0, rc2.x1, pos, rc2.x2, pos+rc2.y2, 40, text_color);
      pos+=rc2.y2;
      res++;
    }
  }
  x=0;
  if (bk->lst)
  {
    while (x<bk->lst->FirstFree)
    {
      EVENT *ev=(EVENT*)bk->lst->listdata[x++];
      if (now<ev->tm_start)
      {
        wchar_t *txt=0;
        if (ev->txt && (ev->days[day] || cmpdate(ev->date, dt.date)==1) && ev->done!=true)
        {
          SetFont(text_font);
          txt=ev->txt;
          DrawGradientVert(rc2.x1, pos, rc2.x2, pos+rc2.y2, low_color, low_color);
          DrawLine(TextID_Create(txt,ENC_UCS2,TEXTID_ANY_LEN), 0, rc2.x1, pos, rc2.x2, pos+rc2.y2, 40, text_color);
          txt=new wchar_t[50];
          snwprintf(txt,49,L"%02d:%02d:%02d", ev->time.hour, ev->time.min, ev->time.sec);
          SetFont(time_font);
          DrawLine(TextID_Create(txt,ENC_UCS2,TEXTID_ANY_LEN), 1, rc2.x1, pos, rc2.x2, pos+rc2.y2, 40, time_color);
          delete(txt);
          pos+=rc2.y2;
          res++;
        }
      }
    }
  }
  if (res==0)
  {
    SetFont(text_font);
    DrawGradientVert(rc2.x1, pos, rc2.x2, pos+rc2.y2, low_color, low_color);
    DrawLine(TextID_Create(str_noevents,ENC_UCS2,TEXTID_ANY_LEN), 2, rc2.x1, pos, rc2.x2, pos+rc2.y2, 40, clWhite);
  }
   if(Display_oldReDraw) Display_oldReDraw(db,r1,r2,r3);
};

void InitVar()
{
  Height=Display_GetHeight(0);
  Width=Display_GetWidth(0);
  
  if(!GUI_display)
    {
      GUI_display = GUIObject_GetDispObject( SBY_GetStatusIndication(Find_StandbyBook()) );
      Display_oldReDraw = DispObject_GetOnRedraw(GUI_display);
      Display_desc = DispObject_GetDESC (GUI_display);
      DISP_DESC_SetOnRedraw(Display_desc, Display_ReDraw);
    }
}

#include "conf_loader.h"
int main (void)
{
  InitVar();
  InitConfig();
  
  CreateEvtDeskBook();
  
  timer=Timer_Set(1000,onTimer,0);
  return(0);
};


