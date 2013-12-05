#include "header\mem.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\cl.h"


typedef struct 
{
  char dummy[0x2C]; 
}GUI_CUSTOM;

GUI_CUSTOM *gc=0;

static const char custom_guiname[]="gui_walkmandisplay";

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

void custom_destr(DISP_DESC *desc)
{
  return;
};

GUI_CUSTOM *custom_create(BOOK *bk)
{
  GUI_CUSTOM *gui_cs=new GUI_CUSTOM;
  if (!GUIObject_Create((GUI*)gui_cs,custom_destr, custom_constr,bk,0,0,0))
  {
    delete gui_cs;
    return 0;    
  }
  if (bk) BookObj_AddGUIObject(bk,(GUI*)gui_cs);
  return gui_cs;
};

extern MyBOOK *MyBook;
typedef void (*GUI_METHOD)(BOOK *, void*);  

typedef struct
{
  u16 action;
  u16 unk;
  BOOK * book;
  DISP_OBJ * DISP_OBJ1;
  u16 unk2; 
  u16 unk3;
  int text;
  int text_mini;
  int text_disabled;
  int text_help;
  u16 parent_action;
  u16 unk4;
  LIST * list;
  void (*proc)(BOOK *,void*);
  char unk5[12]; 
  char enable;
  char visible;
  char unk6[6];
}SOFTKEY;

GUI_METHOD getelem(LIST *lst, int act)
{
  int x;
  for (x=0;x<lst->FirstFree;x++)
  {
    SOFTKEY *sk=(SOFTKEY*)List_Get(lst,x);
    if (sk->action==act)
    {
      return sk->proc;
    }
  }
  return 0;
};
extern DISP_OBJ *g_DO;

typedef void (*TEST)(DISP_OBJ*DO, u16 action);
TEST DispObject_Softkey_ExecuteAction=(TEST)(0x4537E3E0+1);

typedef void (*BK_)(BOOK *, int null);
BK_ Set=(BK_)(0x452AE73C+1);
typedef struct
{
  char dummy[0x4C];
  char stat;
}BK;

int hidden=false;
void myOnKey(DISP_OBJ *DO,int key,int,int repeat,int type)
{
  if (type==KBD_SHORT_PRESS)
  {
    if (key==KEY_ESC)
    {
      if (BOOK*bk=FindBook(get_IsAudioPlayerBook()))
      {
        Set(bk,0);
        //GUIObject_Destroy((GUI*)gc);
        //gc=0;
        //BookObj_Hide(bk,0);
        //BookObj_Show(bk,0);
        //GUI *g=(GUI*)List_Get(book->xguilist->guilist,0);
        //DISP_OBJ *DO=g->DISP_OBJ;
        //DISP_OBJ_ONKEY_METHOD onkey=DispObject_GetOnKey (DO);
        //onkey(DO, KEY_CAMERA_FOCUS, 0 , 1/*èëè 0, õç*/ , KBD_SHORT_PRESS);
        //LIST *lst = DispObject_Softkeys_GetList(g_DO,bk, 0);
        //GUI_METHOD gm=getelem(lst, ACTION_BACK);
        //gm(bk,0);
        //UI_Event(0x2A49);
        //GUIObject_Destroy((GUI*)gc);
        //gc=0;
        BK *b=(BK*)bk;
        b->stat=2;
        UI_Event_toSID(0x2A49, BOOK_GetSessionID(bk));
        
        BookObj_Hide(bk,0);
        hidden=true;
      }
    }
  }
};

void myOnRedraw(DISP_OBJ *DO, int a, int b, int c)
{
};

void CreatePlayer(BOOK *bk)
{
  if (gc)
    GUIObject_Destroy((GUI*)gc);
  gc=custom_create(bk);
  
  DISP_DESC *dd=DispObject_GetDESC (((GUI*)gc)->DISP_OBJ); 
  DISP_DESC_SetOnKey(dd, myOnKey);
  DISP_DESC_SetOnRedraw(dd, myOnRedraw);
  GUIObject_Show(gc);
};

void Kill()
{
  if (gc)
    GUIObject_Destroy((GUI*)gc);
  gc=0;
};
