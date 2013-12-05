#include "inc\mc.h"
#include "inc\gui.h"
#include "inc\lng.h"
#include "inc\zslib.h"
#include "inc\mui.h"
#include "inc\ColorMap.h"

int scr_w;
int scr_h;
int txt_h;
int head_h;
int attr_h;
int ico_h;
int ico_w;
int itm_ch;
int itms_max;
int itms_bs;

wchar_t gui_buf[MAX_PATH*2];

void InitScr()
{
  int old_font;
  int tmp;
  scr_w = Display_GetWidth(0);
  scr_h = Display_GetHeight(0);
  old_font=SetFont(CONFIG_FONT_FILES);
  txt_h=GetImageHeight(L'A');
  SetFont(CONFIG_FONT_HEADER);
  head_h=GetImageHeight(L'A');
  SetFont(CONFIG_FONT_ATTR);
  attr_h=GetImageHeight(L'A');
  
  ico_h=GetImageHeight(STD_ICONS[ICN_FOLDER]);
  ico_w=GetImageWidth(STD_ICONS[ICN_FOLDER]);
  tmp = ico_h - ITM_B * 2;
  itm_ch = tmp > txt_h ? tmp : txt_h;
  SetFont(old_font);
  int i=ITM_FH+ATTR_FH;
  itms_max=1;
  while((i+ITM_FH)<(FLS_H - ITM_S*2))
  {
    i+=ITM_FH;
    itms_max++;
  }
  //itms_max = ((FLS_H - ITM_S * 2) / ITM_FH)-1;
  itms_bs = FLS_Y + ( ( (FLS_H - ITM_S * 2)-(itms_max * ITM_FH + ATTR_FH)) / 2 );
  //itms_bs = FLS_Y + ( ( (FLS_H - ITM_S * 2)-((itms_max + 1) * ITM_FH) ) / 2 );
}

void DrwCurTab(GC *gc, RECT *rc)
{
  TEXTID str=EMPTY_TEXTID;  
  RECT rt;
  rt.x1=TAB_X;
  rt.y1=TOP_Y;
  rt.x2=TAB_X+TAB_W-1;
  rt.y2=TOP_Y+TOP_H-1;
  GC_SetPenColor(gc, Colors[clSlTabBD]);
  GC_SetBrushColor(gc, Colors[clSlTabBG]);
  is_db2010?DrawRect(rt.x1,rt.y1,rt.x2,rt.y2,GC_GetPenColor(gc),GC_GetBrushColor(gc)):GC_DrawRoundRect(gc ,&rt,4,4,1,1);
  
  SetFont(CONFIG_FONT_HEADER);
  str=curtab+'1'+0x78000000;
  DrawString(str,2,TAB_X,TOP_Y+TOP_B,TAB_X+TAB_W-2,TOP_Y+TOP_B+TOP_H-1,0,0,Colors[clTabTxt],0);
}


void DrwDrvBg(int ind, GC *gc, RECT *rc)
{
  int x = DRV_X+DRV_O*ind;
  RECT rt;
  rt.x1=x;
  rt.y1=TOP_Y;
  rt.x2=x+DRV_W-1;
  rt.y2=TOP_Y+TOP_H-1;
  GC_SetPenColor(gc, Colors[clUSTabBD]);
  GC_SetBrushColor(gc, Colors[clUSTabBG]);
  is_db2010?DrawRect(rt.x1,rt.y1,rt.x2,rt.y2,GC_GetPenColor(gc),GC_GetBrushColor(gc)):GC_DrawRoundRect(gc ,&rt,4,4,1,1);
}

const char stn[]={'n','e','s','d','?'};

void DrwSort(void *gc, RECT *rc)
{
  int i = _CurTab->sort & STV_MASK;
  if (i > ST_DATE) i = 4;
  
  char s = stn[i];
  if (_CurTab->sort & STD_MASK) s -= 'a' - 'A';
  TEXTID str_id=s+0x78000000;
  SetFont(CONFIG_FONT_HEADER);
  DrawString(str_id,0,SRT_X,TOP_Y+TOP_B,IND_X1,TOP_Y+TOP_B+TOP_H-1,0,0,
		Colors[clIndexTxt],0);
}

void DrwIndex(void *gc, RECT *rc)
{
  int chkcnt = EnumChk(NULL,0);
  TEXTID str=EMPTY_TEXTID;
  if (chkcnt)
    snwprintf(gui_buf,MAXELEMS(gui_buf)-1,L"%d(%d)", chkcnt, _CurCount);
  else
    snwprintf(gui_buf,MAXELEMS(gui_buf)-1,L"%d/%d", _CurIndex+1, _CurCount);
  str=TextID_Create(gui_buf,ENC_UCS2,TEXTID_ANY_LEN);  
  SetFont(CONFIG_FONT_HEADER);
  DrawString(str,2,IND_X1,TOP_Y+TOP_B,IND_X2-2,TOP_Y+TOP_B+TOP_H-1,0,0,
             Colors[clIndexTxt],0);
  TextID_Destroy(str);
}

int GetSBY(RECT *rc, int i, int c)
{
  return (c<=1)?0:((SB_H-SB_VS+1)*i/(c-1));
}

void DrwSB(GC *gc, RECT *rc)
{
  int sby = FLS_Y+1+GetSBY(rc, _CurIndex, _CurCount);
  GC_SetPenColor(gc , Colors[clScBarBG]);
  GC_DrawLine(gc,SB_X+SB_HS/2, FLS_Y+1+SB_B,SB_X+SB_HS/2, FLS_Y+SB_H-1);
  DrawRect(SB_X,sby,SB_X+SB_HS-1,sby+SB_VS-1,Colors[clScBarBG],Colors[clScBarBG]);
}

void DrwDrvAc(int ind, GC *gc, RECT *rc)
{
  TEXTID str=EMPTY_TEXTID;
  RECT rt;
  wchar_t* name;
  int x;	
  SetFont(CONFIG_FONT_HEADER);
  if (curtab < MAX_TABS)
  {
    x = DRV_X + DRV_O*ind;
    if (IsInArchive()==ZIP_ARCH)
      name = L"ZIP";
    else if (IsInArchive()==_7Z_ARCH)
      name = L"7Z";
    else
      name = Drives[ind].name;
  }
  else if (curtab == systab)
  {
    x = DRV_X;
    name = L"MC";
  }
  str=TextID_Create(name,ENC_UCS2,TEXTID_ANY_LEN);
  x+=rc->x1;
  
  rt.x1=x;
  rt.y1=TOP_Y;
  rt.x2=x+DRV_W-1;
  rt.y2=TOP_Y+TOP_H-1;
  GC_SetPenColor(gc, Colors[clSlTabBD]);
  GC_SetBrushColor(gc, Colors[clSlTabBG]);
  is_db2010?DrawRect(rt.x1,rt.y1,rt.x2,rt.y2,GC_GetPenColor(gc),GC_GetBrushColor(gc)):GC_DrawRoundRect(gc ,&rt,4,4,1,1);
  
  DrawString(str,2,x+1,TOP_Y+TOP_B,x+DRV_W-2,TOP_Y+TOP_B+TOP_H-1,0,0,
             Colors[clTabTxt],0);
  TextID_Destroy(str);
}

u16 sctm=0;
int max_scroll_disp;
int scroll_disp;

void ScrollTimerProc (u16 tmr , LPARAM)
{
  int time=CONFIG_SCROLL_TEXT_SPEED;
  int i=max_scroll_disp;
  if (i)
  {
    if (scroll_disp>=i)
    {
      scroll_disp=0;
    }
    else
    {
      scroll_disp++;
      if (scroll_disp==i)
      {
        time=CONFIG_SCROLL_TEXT_WAIT;
      }
    }
  }
  RedrawGUI=1;
  Timer_ReSet(&sctm, time, ScrollTimerProc, 0);
}

void DisableScroll ()
{
  Timer_Kill(&sctm);
  max_scroll_disp=0;
  scroll_disp=0;
}

void DrwFile(GC *gc, RECT *rc, int ind, FILEINF* file)
{ 
  int y_offs=ind>_CurIndex-_CurBase?ATTR_FH:0;
  int y = itms_bs + ind*ITM_FH+y_offs;
  if (ind == _CurIndex-_CurBase) 
  {
    RECT rect;
    rect.x1=ITM_X1;
    rect.y1=y;
    rect.x2=ITM_X2;
    rect.y2=y+ITM_FH+ATTR_FH-1;
    GC_SetPenColor(gc, Colors[clSelItemBD]);
    GC_SetBrushColor(gc, Colors[clSelItemBG]);
    is_db2010?DrawRect(rect.x1,rect.y1,rect.x2,rect.y2,GC_GetPenColor(gc),GC_GetBrushColor(gc)):GC_DrawRoundRect(gc ,&rect,4,4,1,1);
    
    
    SetFont(CONFIG_FONT_ATTR);  
    int y2 = itms_bs+(ind+1)*ITM_FH;
    if (!(file->attr & FA_DIRECTORY))
    {
      if (file->ws_size!=LGP_NULL)
      {
        DrawString(file->ws_size,0,TXT_X,y2+ITM_B+1,ITM_X2-ITM_B-2,y2+ITM_B+txt_h,0,0,
                   Colors[clInfoTxt],0);
      }
      if (file->ftype == TYPE_ZIP_FILE && file->ws_ratio!=LGP_NULL)
      {
        // рисовать будем посередине + отступ на 1/2 символа
        int posX = (ITM_X2 - ITM_B - 2 - TXT_X) / 2 + TXT_X + txt_h / 2;
        DrawString(file->ws_ratio, 0, posX ,y2+ITM_B+1,ITM_X2-ITM_B-2,y2+ITM_B+txt_h,0,0,
                   Colors[clInfoTxt],0);
      }
    }
    if (file->ws_attr!=LGP_NULL)
    {
      DrawString(file->ws_attr,1,TXT_X,y2+ITM_B+1,ITM_X2-ITM_B-2,y2+ITM_B+txt_h,0,0,
                 Colors[clInfoTxt],0);
    }
  }
  
  wchar_t icon=file->attr & FA_CHECK?STD_ICONS[ICN_CHMARK]:file->icon;
  GC_PutChar(gc,ICO_X, y+ICO_DY, ico_w,ico_h,icon);
  
  
  int tc;
  //if (file->attr & FA_HIDDEN)
  //  tc=ind==_CurIndex-_CurBase?clSelFileHidden:clFileHidden;
  //else
  SetFont(CONFIG_FONT_FILES);
  RECT tmp_rc, txt_rc;
  GC_GetRect(gc, &tmp_rc);
  txt_rc.x1=TXT_X;
  txt_rc.x2=ITM_X2-ITM_B-2;
  txt_rc.y1=tmp_rc.y1;
  txt_rc.y2=tmp_rc.y2;
  GC_ValidateRect(gc, &txt_rc);
  if (ind==_CurIndex-_CurBase)
  {
    tc=clSelFileNormal;
    int d=Disp_GetStrIdWidth(file->sid_name,TextID_GetLength(file->sid_name));
    d-=(ITM_X2-ITM_B-2-TXT_X);
    if (d<0)
    {
      DisableScroll();
    }
    else
    {
      if (!max_scroll_disp)
      {
        Timer_Set(CONFIG_SCROLL_TEXT_SPEED, ScrollTimerProc, 0);
      }
      max_scroll_disp=d;
    }
    DrawString(file->sid_name,0, TXT_X-scroll_disp,y+ITM_B+1,ITM_X2-ITM_B-2,y+ITM_B+txt_h,0,0,
               Colors[tc],0);
  }
  else
  {
    tc=clFileNormal;
    TEXTID  fn = (file->uccnt ? file->ws_short :file->sid_name );
    DrawString(fn,0, TXT_X,y+ITM_B+1,ITM_X2-ITM_B-2,y+ITM_B+txt_h,0,0,
               Colors[tc],0);
  }
  GC_ValidateRect(gc, &tmp_rc);
}

void ShowFiles(GC *gc, RECT *rc)
{ 
  DrawRect(rc->x1,rc->y1,rc->x2,rc->y2,Colors[clBD],Colors[clBG]);
  
  if (curtab < MAX_TABS) DrwCurTab(gc,rc);
  for(int ii=0; ii<_CurDrv; ii++) DrwDrvBg(ii,gc,rc);
  
  DrwDrvAc(_CurDrv,gc,rc);
  
  DrawRect(rc->x1+FLS_B,FLS_Y,rc->x2-FLS_B-1,FLS_Y+FLS_H-1,Colors[clListBD],Colors[clListBG]);
  
  int ibase, icount;
  if (_CurCount<=itms_max)
  {
    icount=_CurCount;
    ibase=0;
  }
  else
  {
    icount=itms_max;
    ibase=_CurBase;
    
    int idist = _CurIndex - _CurBase;
    
    if (idist >= itms_max-1) ibase += idist - itms_max + 2;
    else if (_CurIndex <= _CurBase) ibase += idist - 1;
    
    if (ibase < 0) ibase = 0;
    else if (ibase > _CurCount - itms_max) ibase = _CurCount - itms_max;
  }
  _CurBase = ibase;
  
  FILEINF *file = FileListBase[curtab];
  
  //skip
  for(int ii=0; ii<ibase; ii++) file=(FILEINF *)file->next;
  
  for(int ii=0; ii<icount; ii++)
  {
    file = (FILEINF *)file->next;
    FillFileInfo(file);
    DrwFile(gc,rc,ii, file);
  }
  DrwSB(gc,rc);
  DrwSort(gc,rc);
  DrwIndex(gc,rc);
  if (progr_start) ShowProgr(gc, rc);
}

void ShowProgr(GC *gc, RECT *rc)
{
  TEXTID str=LGP_NULL;
  DrawRect(PRGB_X1,PRGB_Y,PRGB_X2,PRGB_Y+PRGB_H-1,Colors[clProgrBoxBD],Colors[clProgrBoxBG]);

  int p = progr_cur * 100 / progr_max;
  
  if (progr_act)
  {
    snwprintf(gui_buf,MAXELEMS(gui_buf)-1,L"%ls: ",muitxt(progr_act));
    str=TextID_Create(gui_buf,ENC_UCS2,TEXTID_ANY_LEN);
    DrawString(str,2,PRGB_X1+2,PRG_AT_Y,PRGB_X2-2,PRG_AT_Y+txt_h,0,0,Colors[clProgrTxt],Colors[clProgrTxtBD]);
    TextID_Destroy(str);
  }
  
  snwprintf(gui_buf,MAXELEMS(gui_buf)-1,L"%d%% %d/%d",p,progr_cur,progr_max);
  str=TextID_Create(gui_buf,ENC_UCS2,TEXTID_ANY_LEN);  
  DrawString(str,2,PRGB_X1+2,PRG_PT_Y,PRGB_X2-2,PRG_PT_Y+txt_h,0,0,Colors[clProgrTxt],Colors[clProgrTxtBD]);
  TextID_Destroy(str);
  
  p = progr_cur * (PRGP_X2-PRGP_X1-2) / progr_max;
  DrawRect(PRGP_X1,PRG_B1_Y,PRGP_X2,PRG_B1_Y+PRGP_H,Colors[clProgrBD],Colors[clProgrBG]);
  DrawRect(PRGP_X1+1,PRG_B1_Y+1,PRGP_X1+p+1,PRG_B1_Y-1+PRGP_H,Colors[clCurProgrBG],Colors[clCurProgrBG]);
  
  if (progrsp_start)
  {
    DrawRect(PRGP_X1,PRG_B2_Y,PRGP_X2,PRG_B2_Y+PRGP_H,Colors[clProgrBD],Colors[clProgrBG]);
    if (progrsp_max)
    {
      int p = progrsp_cur * (PRGP_X2-PRGP_X1-2) / progrsp_max;
      DrawRect(PRGP_X1+1,PRG_B2_Y+1,PRGP_X1+p+1,PRG_B2_Y-1+PRGP_H,Colors[clCurProgrBG],Colors[clCurProgrBG]);
    }
  }
}
