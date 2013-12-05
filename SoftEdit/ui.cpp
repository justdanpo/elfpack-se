#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
#include "header\structs.h"
#include "header\extern.h"
extern const PAGE_DESC bk_skin;

UI_SOFTKEY sk[3]={ {0,296,0,0x18,320,220,1,0x12}, {0,293,2,0x2C,320,115,1,0x2C}, {240,296,1,0x18,320,0,1,0x12} };
int sk240[24]=   {  0,296,0,0x18,320,220,1,0x12  ,  0,293,2,0x2C,320,115,1,0x2C  ,  240,296,1,0x18,320,0,1,0x12 };
int sk176[24]=   {  0,198,0,0x18,220,156,1,0x12  ,  0,193,2,0x2C,220,80 ,1,0x2C  ,  176,198,1,0x18,220,0,1,0x12 };

void auto_select()
{
  int a=Display_GetHeight(0);
  switch (a)
  {
  case 320:
    memcpy(&sk[0], &sk240[0], 3*8*4);
    break;
  case 220:
    memcpy(&sk[0], &sk176[0], 3*8*4);
    break;
  default:
    memcpy(&sk[0], &sk176[0], 3*8*4);
    break;
  }
};

wchar_t *horiz[]={L"Lower softkey",L"Middle softkey",L"Upper softkey"};
wchar_t *vert[]={L"Left softkey",L"Middle softkey",L"Right softkey"};
wchar_t *cts[]={L"Left",L"Right",L"Center"};

int getint(char *line, int *pos,char end);
char *getparam(char *buf, char *mask, char end, int fsize);
int isSBook(BOOK *bk);
int isSoftEditBook(BOOK*);

void DrawHighlightID2(int font,int text,int ct, int XPos, int YPos, int borderColor, int NormalColor, bool clean);

void addui(char *line,int elem)
{
  int x=0;
  //--------------------
  sk[elem].x=getint(line, &x,':');
  sk[elem].y=getint(line, &x,':');
  sk[elem].ct=getint(line, &x,':');
  sk[elem].font=getint(line, &x,':');
  sk[elem].hx=getint(line, &x,':');
  sk[elem].hy=getint(line, &x,':');
  sk[elem].hct=getint(line, &x,':');
  sk[elem].hfont=getint(line, &x,';');
  //--------------------
};

int set_ui(char *buf, int fsize)
{
  auto_select();
  int x;
  for (x=0;x<3;x++)
  {
    char pattern[128];
    sprintf(pattern,"%dsk)",x);
    char *param;
    if (param=getparam(buf,pattern,';', fsize))
    {
      addui(param,x);
      delete(param);
    }
    else
    {
      break;
    }
  }
  return 1;
};

int SkinGui_OnCreate(DISP_OBJ_SKIN *db)
{
  return 1;
};

void SkinGui_OnClose(DISP_OBJ_SKIN *db)
{
  delete(db->fonts);
};

wchar_t *Font_GetNameByFontId(int id)
{
  wchar_t *txt=L"Undefined";
  FONT_DESC *font=GetFontDesc();
  for (int i=0, max=*GetFontCount(); i<max; i++)
  {
    if (id==font[i].id)
    {
      txt=font[i].name;
      break;
    }
  }
  return txt;
};
void SkinGui_OnRedraw(DISP_OBJ_SKIN *db,int ,int,int)
{
  int width=Display_GetWidth(0);
  int height=Display_GetHeight(0);
  DrawRect(0, 0, 500, 500, clBlack,    clBlack);
  DrawRect(0, 0, width,  25, 0xAA00FF00, 0xAA00FF00);
  DrawRect(0, 0, width,  25, 0xAA00FF00, 0xAA00FF00);
  if (BookObj_GetDisplayOrientation(FindBook(isSoftEditBook))==0)
  {
    wchar_t hz[50];
    snwprintf(hz, 49, L"%d-%d", sk[db->curkey].x, sk[db->curkey].y);
    DrawHighlightID2(FONT_E_20R, TextID_Create(L"SoftkeysUI", ENC_UCS2, TEXTID_ANY_LEN), 2, 0, 0,  clBlack, clWhite, true);
    DrawHighlightID2(FONT_E_20R, TextID_Create(vert[db->curkey], ENC_UCS2, TEXTID_ANY_LEN), 0, 0, 22, clBlack, clWhite, true);
    DrawHighlightID2(FONT_E_20R, TextID_Create(hz,ENC_UCS2,TEXTID_ANY_LEN), 0, 0, 44, clBlack, clWhite, true);
    DrawHighlightID2(FONT_E_20R, TextID_Create(cts[sk[db->curkey].ct],ENC_UCS2,TEXTID_ANY_LEN), 0, 0, 66, clBlack, clWhite, true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press vol up/down",ENC_UCS2,TEXTID_ANY_LEN),1,width,24,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press joystick",ENC_UCS2,TEXTID_ANY_LEN),1,width,46,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press press 'C'",ENC_UCS2,TEXTID_ANY_LEN),1,width,68,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_20R,TextID_Create(Font_GetNameByFontId(sk[db->curkey].font),ENC_UCS2,TEXTID_ANY_LEN),0,0,90,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press 1/3",ENC_UCS2,TEXTID_ANY_LEN),1,width,90,clBlack,clWhite,true);
  }
  else
  {
    wchar_t hz[50];
    snwprintf(hz,49,L"%d-%d",sk[db->curkey].hx,sk[db->curkey].hy);
    DrawHighlightID2(FONT_E_20R,TextID_Create(L"SoftkeysUI", ENC_UCS2, TEXTID_ANY_LEN),2,0,0,clBlack,clGreen, true);
    DrawHighlightID2(FONT_E_20R,TextID_Create(horiz[db->curkey],ENC_UCS2,TEXTID_ANY_LEN),0,0,22,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_20R,TextID_Create(hz,ENC_UCS2,TEXTID_ANY_LEN),0,0,44,clBlack,clWhite, true);
    DrawHighlightID2(FONT_E_20R,TextID_Create(cts[sk[db->curkey].hct],ENC_UCS2,TEXTID_ANY_LEN),0,0,66,clBlack,clWhite, true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press vol up/down",ENC_UCS2,TEXTID_ANY_LEN),1,width,24,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press joystick",ENC_UCS2,TEXTID_ANY_LEN),1,width,46,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press press 'C'",ENC_UCS2,TEXTID_ANY_LEN),1,width,68,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_20R,TextID_Create(Font_GetNameByFontId(sk[db->curkey].hfont),ENC_UCS2,TEXTID_ANY_LEN),0,0,90,clBlack,clWhite,true);
    DrawHighlightID2(FONT_E_14R,TextID_Create(L"press 1/3",ENC_UCS2,TEXTID_ANY_LEN),1,width,90,clBlack,clWhite,true);
  }
};

int get_fontindex(DISP_OBJ_SKIN *db, int font)
{
  int x;
  for (x=0;x<db->total_fonts;x++)
  {
    if (db->fonts[x]==font)
    {
      return x;
    }
  }
  return 0;
};
void SkinGui_OnKey(DISP_OBJ_SKIN *db,int key,int,int repeat,int type)
{
  if (type==KBD_SHORT_PRESS || type==KBD_LONG_PRESS || type==KBD_LONG_RELEASE)return;
  if (type==KBD_SHORT_RELEASE) db->step=1;
  else if (type==KBD_REPEAT && repeat>10) db->step=8;
  int stat=BookObj_GetDisplayOrientation(FindBook(isSoftEditBook));
  if (key==KEY_VOL_DOWN)
  {
    if (db->curkey==0)
    {
      db->curkey=2;
    }
    else
    {
      db->curkey--;
    }
    if (stat==0)
    {
      db->cur=get_fontindex(db, sk[db->curkey].font);
    }
    else
    {
      db->cur=get_fontindex(db, sk[db->curkey].hfont);
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_VOL_UP)
  {
    if (db->curkey==2)
    {
      db->curkey=0;
    }
    else
    {
      db->curkey++;
    }
    if (stat==0)
    {
      db->cur=get_fontindex(db, sk[db->curkey].font);
    }
    else
    {
      db->cur=get_fontindex(db, sk[db->curkey].hfont);
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_UP)
  {
    if (stat==0)
    {
      sk[db->curkey].y=sk[db->curkey].y-db->step;
    }
    else
    {
      sk[db->curkey].hy=sk[db->curkey].hy-db->step;
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_DOWN)
  {
    if (stat==0)
    {
      sk[db->curkey].y=sk[db->curkey].y+db->step;;
    }
    else
    {
      sk[db->curkey].hy=sk[db->curkey].hy+db->step;;
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_LEFT)
  {
    if (stat==0)
    {
      sk[db->curkey].x=sk[db->curkey].x-db->step;
    }
    else
    {
      sk[db->curkey].hx=sk[db->curkey].hx-db->step;
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_RIGHT)
  {
    if (stat==0)
    {
      sk[db->curkey].x=sk[db->curkey].x+db->step;
    }
    else
    {
      sk[db->curkey].hx=sk[db->curkey].hx+db->step;
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key==KEY_DEL)
  {
    if (stat==0)
    {
      if (sk[db->curkey].ct==2)
      {
        sk[db->curkey].ct=0;
      }
      else
      {
        sk[db->curkey].ct++;
      }
    }
    else
    {
      if (sk[db->curkey].hct==2)
      {
        sk[db->curkey].hct=0;
      }
      else
      {
        sk[db->curkey].hct++;
      }
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key-KEY_DIGITAL_0==1)
  {
    if (db->cur==0)
    {
      db->cur=db->total_fonts;
    }
    else 
    {
      db->cur--;
    }
    if (stat==0)
    {
      sk[db->curkey].font=db->fonts[db->cur];
    }
    else
    {
      sk[db->curkey].hfont=db->fonts[db->cur];
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  else if (key-KEY_DIGITAL_0==3)
  {
    if (db->cur==db->total_fonts)
    {
      db->cur=0;
    }
    else 
    {
      db->cur++;
    }
    if (stat==0)
    {
      sk[db->curkey].font=db->fonts[db->cur];
    }
    else
    {
      sk[db->curkey].hfont=db->fonts[db->cur];
    }
    DispObject_InvalidateRect((DISP_OBJ*)db,0);
  }
  SKBOOK *sb=(SKBOOK*)FindBook(isSBook);
  if (sb)
  {
    if (sb->change)
    {
      sb->change(&sk[0],0);
      sb->change(&sk[1],1);
      sb->change(&sk[2],2);
    }
  }
};

static const char SkinGuiName[]="Gui_SkinEditor";

void SkinGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,SkinGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_SKIN));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)SkinGui_OnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)SkinGui_OnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)SkinGui_OnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)SkinGui_OnKey);
};

void SkinGui_destr( GUI* )
{
  return;
};

void SkinGui_OnBack(BOOK * bk, void *)
{
  return;
};

GUI_SKIN *CreateSkinGUI(MyBOOK *mbk)
{
  GUI_SKIN *gui_skin=new GUI_SKIN;
  if (!GUIObject_Create( gui_skin,SkinGui_destr, SkinGui_constr,&mbk->book,0,0,0))
  {
    delete gui_skin;
    return 0;    
  }
  if (mbk) BookObj_AddGUIObject(&mbk->book, gui_skin);
  mbk->skin = gui_skin;
  return gui_skin;
};

void SkinGUI_OnBack( BOOK* bk, GUI* )
{
  BookObj_SetDisplayOrientation(bk,0);
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
};

void SkinGUI_Test( BOOK* bk, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)bk;
  DISP_OBJ_SKIN *db=(DISP_OBJ_SKIN*)GUIObject_GetDispObject(mbk->skin);
  if (BookObj_GetDisplayOrientation(bk)==0)
  {
    BookObj_SetDisplayOrientation(bk,1);
    GUIObject_SoftKeys_SetText(mbk->skin,0,TextID_Create(L"Normal",ENC_UCS2,TEXTID_ANY_LEN));
    db->cur=get_fontindex(db, sk[db->curkey].hfont);
  }
  else
  {
    BookObj_SetDisplayOrientation(bk,0);
    GUIObject_SoftKeys_SetText(mbk->skin,0,TextID_Create(L"Landscape",ENC_UCS2,TEXTID_ANY_LEN));
    db->cur=get_fontindex(db, sk[db->curkey].font);
  }
  BookObj_Hide(bk,0);
  BookObj_Show(bk,0);
};

void SkinGUI_Dummy( BOOK* bk, GUI* )
{
  return;
};
extern bool smthchanged;
int Skin_OnEnter(void *, BOOK * bk)
{
  smthchanged=true;
  GUI * skin_gui = CreateSkinGUI((MyBOOK*)bk);
  bool pl;
  switch(GetChipID()&CHIPID_MASK)
  {
  case CHIPID_DB2000:
  case CHIPID_DB2010:
    GUIObject_SetStyle(skin_gui,3);
    pl=0;
    break;
  case CHIPID_DB2020:
    GUIObject_SetStyle(skin_gui,4);
    pl=1;
    break;
  }
  GUIObject_SetTitleType(skin_gui, 1);
  GUIObject_SoftKeys_SetAction(skin_gui,ACTION_BACK, SkinGUI_OnBack);
  GUIObject_SoftKeys_SetAction(skin_gui,0,SkinGUI_Test);
  if (BookObj_GetDisplayOrientation(bk)==0)
  {
    GUIObject_SoftKeys_SetText(skin_gui,0,TextID_Create(L"Landscape",ENC_UCS2,TEXTID_ANY_LEN));
  }
  else
  {
    GUIObject_SoftKeys_SetText(skin_gui,0,TextID_Create(L"Normal",ENC_UCS2,TEXTID_ANY_LEN));
  }
  GUIObject_SoftKeys_SetEnable(skin_gui,0,pl);
  GUIObject_SoftKeys_SetAction(skin_gui,1,SkinGUI_Dummy);
  GUIObject_SoftKeys_SetText(skin_gui,1,TextID_Create(L"-test-",ENC_UCS2,TEXTID_ANY_LEN));
  
  DISP_OBJ_SKIN *db=(DISP_OBJ_SKIN*)GUIObject_GetDispObject(skin_gui);
  db->curkey=0;
  db->setting=0;
  db->total_fonts=*GetFontCount();
  db->fonts=new u16[db->total_fonts];
  for (int i=0; i<db->total_fonts; i++)
  {
    db->fonts[i]=GetFontDesc()[i].id;
  }
  db->cur=0;
  db->step=1;
  GUIObject_Show(skin_gui);
  BookObj_Hide(bk,0);
  BookObj_Show(bk,0);
  return (1);
};

int Skin_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (mbk->skin)GUIObject_Destroy( mbk->skin);
  mbk->skin=0;
  return (1);
};

const PAGE_MSG bk_msglst_skin[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,            Skin_OnEnter,
  PAGE_EXIT_EVENT_TAG,             Skin_OnExit,
  NIL_EVENT_TAG,                   NULL
};

const PAGE_DESC bk_skin = {"SoftEdit_SkinEditor",0,bk_msglst_skin};


