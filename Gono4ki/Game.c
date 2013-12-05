#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Menu.h"
#include "MyFUNC.h"
#include "rand.h"
#include "NewRecord.h"
u16 ftimer;

typedef GUI GUI_CUSTOM;

DISP_DESC *dd;
GUI_CUSTOM* np;

static const char custom_guiname[]="gui_custom";

const PAGE_MSG bk_msglst_go[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    CreateGO,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_go = {"Game_GuiMenu_Page",0,bk_msglst_go};

int custom_onCreate(DISP_OBJ *DO)
{
  return 1;
};

void custom_onClose(DISP_OBJ *DO)
{
  return;
};

void custom_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,custom_guiname);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)custom_onCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)custom_onClose);
};

void custom_destr( GUI* )
{
  return;
};

GUI_CUSTOM *custom_create(BOOK *bk)
{
  GUI_CUSTOM *gui_cs=new GUI_CUSTOM;
  if (!GUIObject_Create(gui_cs,custom_destr, custom_constr,bk,0,0,0))
  {
    delete gui_cs;
    return 0;    
  }
  if (bk) BookObj_AddGUIObject(bk,gui_cs);
  GUIObject_SetStyle(gui_cs, 4);
  GUIObject_SetTitleType(gui_cs, 1);
  GUIObject_SoftKeys_Hide(gui_cs);
  return gui_cs;
};

void DrawCub(int cx, int cy, int ob)
{
  int cub=MyBK()->dis[GetDis()]->cub;
  int x=MyBK()->dis[GetDis()]->x;
  int y=MyBK()->dis[GetDis()]->y;
  if(MyBK()->image->fi[2])
  {
    GC_PutChar(get_DisplayGC() , cx*cub, y-(cy*cub), 0, 0, MyBK()->image->im[2]->ImageID);
  }
  else
  {
    DrawRect(cx*cub, y-(cy*cub), cub+cx*cub, y+cub-(cy*cub), ob*0xFF000000, 0x00000000);
    DrawRect(cx*cub+2, y-(cy*cub)+2, cub+cx*cub-2, y+cub-(cy*cub)-2, ob*0xFF000000, 0xFF000000);
  }
}

void DrawP(PREGRADA*p)
{
  for(int i=0; i<4; i++)
  {
    if(p->x[i]==1) DrawCub(i*3, p->y, 1);
    if(p->x[i]==1) DrawCub(i*3+1, p->y, 1);    
    if(p->x[i]==1) DrawCub(i*3+2, p->y, 1);
  }
}
  

void DrawTank(int step)
{
  DrawCub(step*3, 1, 1);
  DrawCub(step*3+2, 1, 1);
  DrawCub(step*3+1, 2, 1);
  DrawCub(step*3, 3, 1);
  DrawCub(step*3+1, 3, 0);
  DrawCub(step*3+2, 3, 1);
  DrawCub(step*3+1, 4, 1);
}

void DrawText(MyBook*bk, int FSize, int x, int y, int cub)
{
  SetFont(FSize);
  int o[2];
  o[0]=STR("Очки:");
  o[1]=TextID_CreateIntegerID(bk->point);
  DrawString(o[0],0, cub*12+5, y-20*cub, cub*12+5+GetFSize(FSize)*5, y-20*cub+GetFSize(FSize), 1, 0x01,0xFFFF0000,0xFFFFFFFF);
  DrawString(o[1],0, cub*12+5, y-20*cub+GetFSize(FSize)+1, cub*12+5+GetFSize(FSize)*4, y-20*cub+2*GetFSize(FSize)+1, 1, 0x01,0xFFFF0000,0xFFFFFFFF);
  TextID_Destroy(o[0]);
  TextID_Destroy(o[1]);
  int met[3];
  int ID;
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC, &dt);
  ID=STR("Время:\n");
  DrawString(ID,0, cub*12+5, y-20*cub+2*GetFSize(FSize)+6, cub*12+5+GetFSize(FSize)*6, y-20*cub+3*GetFSize(FSize)+6, 1, 0x01,0xFFFF0000,0xFFFFFFFF);
  TextID_Destroy(ID);
  met[0]=TextID_CreateIntegerID(dt.time.hour);
  met[1]=STR(":");
  met[2]=TextID_CreateIntegerID(dt.time.min);
  ID=TextID_Create(met, ENC_TEXTID, 3);
  DrawString(ID,0, cub*12+5, y-20*cub+3*GetFSize(FSize)+7, cub*12+5+GetFSize(FSize)*5, y-20*cub+4*GetFSize(FSize)+6, 1, 0x01,0xFFFF0000,0xFFFFFFFF);
  TextID_Destroy(ID);
  TextID_Destroy(met[0]);
  TextID_Destroy(met[1]);
  TextID_Destroy(met[2]);
  ID=STR("Уровень:");
  DrawString(ID, 0, cub*12+5, y-20*cub+4*GetFSize(FSize)+9, cub*12+5+GetFSize(FSize)*8, y-20*cub+5*GetFSize(FSize)+9, 1, 0x01, 0xFFFF0000,0xFFFFFFFF);
  TextID_Destroy(ID);
  ID=TextID_CreateIntegerID(MyBK()->palka/10);
  DrawString(ID, 0, cub*12+5, y-20*cub+5*GetFSize(FSize)+10, cub*12+5+GetFSize(FSize)*3, y-20*cub+6*GetFSize(FSize)+10, 1, 0x01, 0xFFFF0000,0xFFFFFFFF);
  TextID_Destroy(ID);
}

void DrawPause(int FSize, int x, int y)
{
  DrawRect(0,0,x,y, 0xa0000000, 0xa0000000);
  SetFont(FSize);
  int ID=STR("Пауза");
  DrawString(ID, 0, (x-GetFSize(FSize)*5)/2, (y-GetFSize(FSize))/2, (x-GetFSize(FSize)*5)/2+5*GetFSize(FSize), (y-GetFSize(FSize))/2+GetFSize(FSize), 1, 0x01,0xFFFF0000,0xFFFFFFFF);
}

void myOnRedraw(DISP_OBJ *DO, int a, int b, int c)
{
  int cub=MyBK()->dis[GetDis()]->cub;
  int x=MyBK()->dis[GetDis()]->x;
  int y=MyBK()->dis[GetDis()]->y;
  if(MyBK()->image->fi[0] && MyBK()->nast->im->on)
  {
    GC_PutChar(get_DisplayGC() , 0, 0, 0, 0, MyBK()->image->im[0]->ImageID);
  }
  else
  {
    DrawRect(0,0,x,y, 0xFF0000FF, 0xFF000000);
  }
  if(MyBK()->image->fi[1] && MyBK()->nast->im->on)
  {
    GC_PutChar(get_DisplayGC() , 0, y-(20*cub), 0, 0, MyBK()->image->im[1]->ImageID);
  }
  else
  {
    DrawRect(0, y-(20*cub), 12*cub, y, 0xFF0000FF, 0xFFFFFFFF);
  }
  DrawP(MyBK()->p[0]);
  DrawP(MyBK()->p[1]);
  DrawTank(((MyBook*)isBookX(NameMyElf, 0))->tank->x);
  DrawText(MyBK(), MyBK()->dis[GetDis()]->FS,x,y,cub);
  int id;
  if(MyBK()->nast->sound->sound)
  {    
    iconidname2id(L"TR_ALARM_ON_ICN", -1, &id);
  }
  else
  {
    iconidname2id(L"TR_ALARM_OFF_ICN", -1, &id);
  }
  GC_PutChar(get_DisplayGC() , MyBK()->dis[GetDis()]->x-2*cub, MyBK()->dis[GetDis()]->y-2*cub, 0, 0, id);
  if(MyBK()->pause)
  {
    DrawPause(MyBK()->dis[GetDis()]->FP, x, y);
  }
  //DrawRect(((MyBook*)isBookX(NameMyElf, 0))->tank->x, ((MyBook*)isBookX(NameMyElf, 0))->tank->y, ((MyBook*)isBookX(NameMyElf, 0))->tank->x+10,((MyBook*)isBookX(NameMyElf, 0))->tank->y+10, 0xFFFF0000, 0xFF000000);  
}

int Check(PREGRADA*p, COORDINAT*t)
{
  if(p->y<=4)
  {
    for(int i=0; i<4; i++)
    {
      if(t->x==i&&p->x[i]==1) return 0;
    }
  }
  return 1;
}

void myOnKey(DISP_OBJ *db,int key,int a,int b,int type)
{  
  RECT*rs=new RECT;
  if((key==KEY_ENTER || key==KEY_DIGITAL_0+5) && (type==KBD_LONG_PRESS || type==KBD_SHORT_PRESS))
  {
    MyBK()->megaspeed=3;
  }
  if((key==KEY_ENTER || key==KEY_DIGITAL_0+5)&& (type==KBD_LONG_RELEASE || type==KBD_SHORT_RELEASE))
  {
    MyBK()->megaspeed=1;
  }
  if(key==KEY_ESC && type==KBD_LONG_PRESS)
  {
    GUIObject_Destroy(np);
    BookObj_GotoPage(isBookX(NameMyElf, 0),&bk_go);
    Timer_Kill(&ftimer);
    return;
  }
  if(key==KEY_ESC && type==KBD_SHORT_RELEASE)
  {
    if(MyBK()->pause==0) MyBK()->pause=1;
    else MyBK()->pause=0;
    PlayMyMusic(4);
    DispObject_InvalidateRect(db,0);
    return;
  }
  if(key==KEY_DIGITAL_0+10 && type==KBD_SHORT_RELEASE)
  {
    BookObj_Hide((BOOK*)MyBK(), 0);
    if(MyBK()->pause==0) MyBK()->pause=1;
    return;
  }
  if(key==KEY_DIGITAL_0+11 && type==KBD_SHORT_RELEASE)
  {
    if(MyBK()->nast->sound->sound) MyBK()->nast->sound->sound=0;
    else MyBK()->nast->sound->sound=1;
    RECT*rs=new RECT;
    rs->x1=MyBK()->dis[GetDis()]->x-MyBK()->dis[GetDis()]->cub*2;
    rs->x2=MyBK()->dis[GetDis()]->x;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub*2;
    rs->y2=MyBK()->dis[GetDis()]->y;
    DispObject_InvalidateRect(db,rs);
    return;
  }
  if((key==KEY_LEFT || key==KEY_DIGITAL_0+4) && type==KBD_SHORT_RELEASE && !MyBK()->pause)
  {
    
    if(((MyBook*)isBookX(NameMyElf, 0))->tank->x>0)
    {
      rs->x2=(MyBK()->tank->x+1)*3*MyBK()->dis[GetDis()]->cub;
      ((MyBook*)isBookX(NameMyElf, 0))->tank->x+=-1;
      rs->x1=MyBK()->tank->x*3*MyBK()->dis[GetDis()]->cub;
      
    }
    //PlayMyMusic(5);
  }
  if((key==KEY_RIGHT || key==KEY_DIGITAL_0+6) && type==KBD_SHORT_RELEASE && !MyBK()->pause)
  {
    if(((MyBook*)isBookX(NameMyElf, 0))->tank->x<3)
    {
      rs->x1=MyBK()->tank->x*3*MyBK()->dis[GetDis()]->cub;
      ((MyBook*)isBookX(NameMyElf, 0))->tank->x+=1;
      rs->x2=(MyBK()->tank->x+1)*3*MyBK()->dis[GetDis()]->cub;
    }      
    //PlayMyMusic(5);
  }
  rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub*4;
  rs->y2=MyBK()->dis[GetDis()]->y;
  DispObject_InvalidateRect(db,rs);
  for(int i=0; i<2; i++)
  {
  if(!Check(MyBK()->p[i], MyBK()->tank))
    {
      vibra();
      GUIObject_Destroy(np);
      BookObj_GotoPage(isBookX(NameMyElf, 0),&bk_go);
      Timer_Kill(&ftimer);
      return;
    }
  }
}

void onfTimer (u16 tmr , LPARAM)
{
  if(!MyBK()->pause)
  {
    MyBK()->p[0]->y+=-1;
    MyBK()->p[1]->y+=-1;
    if(MyBK()->p[1]->y==0)
    {
      MyBK()->palka++;
      PlayMyMusic(1);
      MyBK()->point++;
      if((MyBK()->megaspeed)>1)
      {
        MyBK()->point+=rand();
      }
      MyBK()->p[1]->y=20;
      for(int i=0; i<4; i++)
      {
        MyBK()->p[1]->x[i]=1;
      }
      int r=rand();
      MyBK()->p[1]->x[r]=0;
      if(MyBK()->speed==1)
      {
        r=rand();
        MyBK()->p[1]->x[r]=0;
      }      
    }
    if(MyBK()->p[0]->y==0)
    {
      MyBK()->palka++;
      PlayMyMusic(1);
      MyBK()->point++;
      if((MyBK()->megaspeed)>1)
      {
        MyBK()->point+=rand();
      }
      MyBK()->p[0]->y=20;
      for(int i=0; i<4; i++)
      {
        MyBK()->p[0]->x[i]=1;
      }
      int r=rand();
      MyBK()->p[0]->x[r]=0;
      if(MyBK()->speed==1)
      {
        r=rand();
        MyBK()->p[0]->x[r]=0;
      } 
    }
    RECT*rs=new RECT;
    rs->x1=0;
    rs->x2=MyBK()->dis[GetDis()]->cub*12;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->p[0]->y*MyBK()->dis[GetDis()]->cub-MyBK()->dis[GetDis()]->cub;
    rs->y2=MyBK()->dis[GetDis()]->y-MyBK()->p[0]->y*MyBK()->dis[GetDis()]->cub+MyBK()->dis[GetDis()]->cub;
    DispObject_InvalidateRect( GUIObject_GetDispObject(np), rs);
    rs->x1=0;
    rs->x2=MyBK()->dis[GetDis()]->cub*12;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->p[1]->y*MyBK()->dis[GetDis()]->cub-MyBK()->dis[GetDis()]->cub;
    rs->y2=MyBK()->dis[GetDis()]->y-MyBK()->p[1]->y*MyBK()->dis[GetDis()]->cub+MyBK()->dis[GetDis()]->cub;
    DispObject_InvalidateRect( GUIObject_GetDispObject(np), rs);
    rs->x1=MyBK()->dis[GetDis()]->cub*12;
    rs->x2=MyBK()->dis[GetDis()]->x;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub*20;
    rs->y2=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub*20+MyBK()->dis[GetDis()]->FS*8;
    DispObject_InvalidateRect( GUIObject_GetDispObject(np), rs);
    rs->x1=0;
    rs->x2=MyBK()->dis[GetDis()]->cub*12;
    rs->y1=MyBK()->dis[GetDis()]->y-MyBK()->dis[GetDis()]->cub;
    rs->y2=MyBK()->dis[GetDis()]->y;
    DispObject_InvalidateRect( GUIObject_GetDispObject(np), rs);
    for(int i=0; i<2; i++)
    {
      if(!Check(MyBK()->p[i], MyBK()->tank))
      {
        PlayMyMusic(2);
        vibra();
        GUIObject_Destroy(np);
        BookObj_GotoPage(isBookX(NameMyElf, 0),&bk_go);
        Timer_Kill(&ftimer);
        return;
      }
    }
    if(!isBookX("Gono4ki", 1)) MyBK()->pause=1;
  }
  int time=(1000/MyBK()->speed-MyBK()->palka*MyBK()->speed)/MyBK()->megaspeed;
  if(time<=0) time=10;
  Timer_ReSet(&ftimer,time,onfTimer,0);
}


int CreateGame(void*, BOOK*bk)
{
  randInit();
  for(int i=0; i<2; i++)
  {
    for(int j=0; j<4; j++)
    {
      ((MyBook*)bk)->p[i]->x[j]=1;
    }
    ((MyBook*)bk)->p[i]->y=20-i*10;
    int r=rand();
    ((MyBook*)bk)->p[i]->x[r]=0;
  }
  MyBK()->megaspeed=1;
  MyBK()->point=0;
  MyBK()->pause=0;
  MyBK()->palka=0;
  np=custom_create(bk);
  dd=DispObject_GetDESC ( GUIObject_GetDispObject(np) ); 
  DISP_DESC_SetOnKey(dd, myOnKey);
  DISP_DESC_SetOnRedraw(dd, myOnRedraw);
  GUIObject_Show(np);
  BookObj_Show(bk, 0);
  BookObj_SetFocus(bk, 0);
  PlayMyMusic(0);
  ftimer=Timer_Set(1000/((MyBook*)bk)->speed,onfTimer,0);
  return 0;
}
