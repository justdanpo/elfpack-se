/*
*===========================================================================
*                   Модуль GUI_REMIND, напоминальщика. Второй вариант.
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\lng.h"

#define MBK(a) MyBOOK *mbk=(MyBOOK*)a;
#define GET_BLUE(x) ((unsigned int)x&0xFF)
#define GET_GREEN(x) (((unsigned int)x>>8)&0xFF)
#define GET_RED(x) (((unsigned int)x>>16)&0xFF)
#define GET_ALPHA(x) (((unsigned int)x>>24)&0xFF)
#define BG_COLOR 0xCC000000

extern char h,m,s;
extern MyBOOK *ECBook;

static const char RemindGuiName[]="gui_reminder";

int GuiRemind_NextRemind(GUI_REMIND *g);
int GuiRemind_NextRemind(GUI_REMIND *g);
void GuiRemind_CheckSelected(GUI_REMIND *g);
wchar_t *checkfile(wchar_t*);
/*
*===========================================================================
*                    Создание списка напоминаний
*===========================================================================
*/
int GUI_REMIND_OnCreate(DISP_OBJ_REMIND *db)
{
  db->num=0;
  if (!ECBook->remlst)
    db->rems=List_Create();
  else
    db->rems=ECBook->remlst;
  return 1;
};

int GetIconID(wchar_t *txt)
{
  int id=NOIMAGE;
  iconidname2id(txt,0xFFFFFFFF,&id);
  return id;
};
/*
*===========================================================================
*                         Уничтожение списка напоминаний
*===========================================================================
*/
void kill_rems(LIST *lst, MyBOOK *mbk, bool check)
{
  if (lst)
  {
    int x=0;
    while (1)
    {
      if (x>=lst->FirstFree || x<0)break;
      REMIND* rem=(REMIND*)List_Get(lst,x);
      if (rem && (rem->checked || check==false))
      {
        if (rem->text)delete(rem->text);
        if (rem->utext)delete(rem->utext);
        if (rem->time)delete(rem->time);
        delete(rem);
        List_RemoveAt(lst, x);
      }
      else
      {
        x++;
      }
    }
    wchar_t icon=GetIconID(L"CALE_RN_TASK_STATUS_ICN");
    if (lst->FirstFree==0)
    {
      List_Destroy(lst);
      mbk->remlst=0;
      SetTrayIcon(icon, 0);
    }
    else
    {
      mbk->remlst=lst;
      SetTrayIcon(icon, 1);
    }
  }
};
/*
*===========================================================================
*                     Рисуем горизонтальный градиент
*===========================================================================
*/
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

/*
*===========================================================================
*                      Рисуем вертикальный градиент
*===========================================================================
*/
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

void GUI_REMIND_OnClose(DISP_OBJ_REMIND *db)
{
  if (db->rems)kill_rems(db->rems, ECBook, true);
  db->rems=0;
};

/*
*===========================================================================
*                      Рисование прогрессбара с вертикальным градиентом
*===========================================================================
*/
void DrawProgressVert(RECT rect, int progress, int time, int upc, int loc)
{
  float Length;
  float CurLength=(float)progress;
  float inLength=(float)time;
  float Procent=(float)CurLength/inLength;
  float size=(float)((float)rect.x2-(float)rect.x1);
  float start = (float)rect.x1;
  Length = start+((Procent)*size);
  DrawGradientVert(rect.x1, rect.y1, (int)Length, rect.y2, upc, loc);
};

void DrawLine(int str, int ct, int x, int y, int x2, int y2, int between, int c)
{
  DrawString(str,ct,x,y,x2,y2,between,3,c,c);
  TextID_Destroy(str);
};
/*
*===========================================================================
*                      Прорисовка элементов гуя
*===========================================================================
*/
void GUI_REMIND_OnRedraw(DISP_OBJ_REMIND *db,int ,int,int)
{
  DispObject_SetLayerColor((DISP_OBJ*)db,BG_COLOR);
  if (db->rems && db->rems->FirstFree)
  {
    int width=Display_GetWidth(0);
    int height=Display_GetHeight(0);
    REMIND *rem=(REMIND*)List_Get(db->rems,db->num);
    int cl=0xFFAA0000;
    if (rem->checked)cl=0xFF00AA00;
    SetFont(FONT_E_16R);
    DrawGradientVert(0, 0, width, 25, cl, 0);//BG_COLOR);
    wchar_t str[50];
    snwprintf(str,49,L"%02d/%02d",db->num+1,db->rems->FirstFree);
    DrawLine(TextID_Create(L"EventControl",ENC_UCS2,TEXTID_ANY_LEN),0,25,0,width,25,20,clWhite);
    DrawLine(TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN),1,0,0,width,25,20,clWhite);

    int icon_id=NOIMAGE;
    iconidname2id(L"CALE_RN_APPOINTMENT_STATUS_ICN",-1,&icon_id);
    GC *gc=get_DisplayGC();
    GC_PutChar(gc,0,0,0,0,icon_id);
    SetFont(FONT_E_20I);
    DrawLine(TextID_Create(rem->text,ENC_UCS2,TEXTID_ANY_LEN),2,0,height/2-10,width,height,20,clWhite);
    SetFont(FONT_E_16R);
    DrawLine(TextID_Create(rem->utext,ENC_UCS2,TEXTID_ANY_LEN),2,0,height-44,width,height,0,clWhite);
    RECT rec={0,width,height-25,height};
    DrawProgressVert(rec, db->num+1, db->rems->FirstFree, clBlack, cl);
    DrawGradient(width-80, 60, width, 85, 0, cl);
    wchar_t time[50];
    snwprintf(time, 49, L"/%02d:%02d:%02d", h, m, s);
    SetFont(FONT_E_20R);
    DrawLine(TextID_Create(time,ENC_UCS2,TEXTID_ANY_LEN),1,0,60,width,80,0,clWhite);
    SetFont(FONT_E_16R);
    DrawLine(TextID_Create(rem->time,ENC_UCS2,TEXTID_ANY_LEN),1,0,63,width-80,80,0,clWhite);
    int font_y=GetImageHeight(L' ')+4;
    DrawLine(TextID_Create(lng[LNG_INFO],ENC_UCS2,TEXTID_ANY_LEN),0,0,height-font_y,width,height,0,clWhite);
    DrawLine(TextID_Create(lng[LNG_OK],ENC_UCS2,TEXTID_ANY_LEN),2,0,height-font_y,width,height,0,clWhite);
    DrawLine(TextID_Create(lng[LNG_MARK],ENC_UCS2,TEXTID_ANY_LEN),1,0,height-font_y,width,height,0,clWhite);
  }
};

/*
*===========================================================================
*                       Обработчик нажатия кнопки ОК/Назад
*===========================================================================
*/
void Reminder_onInfo(BOOK *bk, void *)
{
  MessageBox(EMPTY_TEXTID,TextID_Create(lng[LNG_FINFO],ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
};

/*
*===========================================================================
*                       Обработчик нажатия кнопки ОК/Назад
*===========================================================================
*/
void Reminder_onOK(BOOK *bk, void *)
{
  PlayFile(checkfile(L"stop.mp3"),L"stop.mp3");
  MBK(bk);
  if (!mbk->remind) 
  {
    BookObj_Hide(bk, 0);
    return;
  }
  int res=GuiRemind_NextRemind(mbk->remind);
  if (res==0)
  {
    GUIObject_Destroy(mbk->remind);
    mbk->remind=0;
    BookObj_Hide(bk, 0);
  }
};

/*
*===========================================================================
*                       Обработчик нажатия кнопки "C"
*===========================================================================
*/
void Reminder_onDel(BOOK *bk, void *)
{
  MBK(bk);
  GuiRemind_CheckSelected(mbk->remind);
};

/*
*===========================================================================
*                      Функция обработки нажатия клавиш
*===========================================================================
*/
void GUI_REMIND_OnKey(DISP_OBJ_REMIND *db,int key,int,int repeat,int type)
{
  if (type==KBD_SHORT_PRESS)
  {
    if (key==KEY_DEL || key==KEY_RIGHT_SOFT)
    {
      Reminder_onDel(&ECBook->bk,0);
    }
    if (key==KEY_ENTER || key==KEY_ESC)
    {
      Reminder_onOK(&ECBook->bk,0);
    }
    if (key==KEY_LEFT_SOFT)
    {
      Reminder_onInfo(&ECBook->bk,0);
    }
    if (key==KEY_LEFT)
    {
      if (db->num)db->num--;
      DispObject_InvalidateRect((DISP_OBJ*)db,0);
    }
    if (key==KEY_RIGHT)
    {
      if (db->num!=db->rems->FirstFree-1)db->num++;
      DispObject_InvalidateRect((DISP_OBJ*)db,0);
    }
  }
};


/*
*===========================================================================
*                     Функция вызова прорисовки
*===========================================================================
*/
void GUI_REMIND_OnRefresh(DISP_OBJ_REMIND *DO)
{
  DispObject_SetRefreshTimer((DISP_OBJ*)DO, 1000);
  DispObject_InvalidateRect((DISP_OBJ*)DO, 0);
};

/*
*===========================================================================
*                     Конструктор гуя
*===========================================================================
*/
void GUI_REMIND_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,RemindGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_REMIND));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)GUI_REMIND_OnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)GUI_REMIND_OnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)GUI_REMIND_OnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)GUI_REMIND_OnKey);
  DISP_DESC_SetOnRefresh(desc,(DISP_OBJ_METHOD)GUI_REMIND_OnRefresh);
};

/*
*===========================================================================
*                       Деструктор гуя
*===========================================================================
*/
void GUI_REMIND_destr( GUI* )
{
  return;
};

/*
*===========================================================================
*                             Создание гуя
*===========================================================================
*/
GUI_REMIND *GUI_REMIND_Create(BOOK *bk)
{
  GUI_REMIND *gui_read=new GUI_REMIND;
  if (!GUIObject_Create( gui_read,GUI_REMIND_destr, GUI_REMIND_constr,bk,0,0,0))
  {
    delete gui_read;
    return 0;    
  }
  DispObject_SetLayerColor( GUIObject_GetDispObject( gui_read), BG_COLOR);
  if (bk) BookObj_AddGUIObject(bk, gui_read);
  DispObject_SetLayerColor( GUIObject_GetDispObject( gui_read), BG_COLOR);
  DispObject_SetRefreshTimer( GUIObject_GetDispObject( gui_read), 1000);
  return gui_read;
};

/*
*===========================================================================
*                 Добавление в гуй нового напоминания
*===========================================================================
*/
void GuiRemind_AddNote(GUI_REMIND *g, REMIND *rem)
{
  if (!g)return;
  if (!rem)return;
  DISP_OBJ_REMIND *DO=(DISP_OBJ_REMIND*) GUIObject_GetDispObject(g);
  if (!DO)return;
  List_InsertLast(DO->rems,rem);
};

void GuiRemind_CheckSelected(GUI_REMIND *g)
{
  GUI *gb = g;
  DISP_OBJ_REMIND *db= (DISP_OBJ_REMIND*)GUIObject_GetDispObject(gb);
  if (db->rems && db->num<db->rems->FirstFree)
  {
    REMIND *rem=(REMIND*)List_Get(db->rems,db->num);
    if (rem->checked)
      rem->checked=false;
    else
    {
      rem->checked=true;
      GuiRemind_NextRemind(g);
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
};

int GuiRemind_NextRemind(GUI_REMIND *g)
{
  GUI *gb = g;
  DISP_OBJ_REMIND *db=(DISP_OBJ_REMIND*)GUIObject_GetDispObject(gb);
  if (db->rems)
  {
    if (db->num!=db->rems->FirstFree-1)
      db->num++;
    else
      return 0;
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else
    return 0;
  return 1;
};
