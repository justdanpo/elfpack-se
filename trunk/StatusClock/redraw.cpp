#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "config_data.h"

DISP_OBJ_ONREDRAW_METHOD DREDRAW;

void DrawHighlightID(int font,int text,int ct, int XPos, int YPos, int borderColor, int shadowColor, int NormalColor)
{
  if (text && text!=EMPTY_TEXTID)
  {
    SetFont(font);
    int MaxXPos,MaxYPos;
    if (ct==0)
    {
      MaxXPos=XPos+500;
    }
    else if (ct==1)
    {
      MaxXPos=XPos;
      XPos=XPos-500;
    }
    else
    {
      MaxXPos=Display_GetWidth(0)+500;
      XPos=-500;
    }
    MaxYPos=500;
    //мод функции для работы с тенью текста...
    DrawString(text,ct,XPos+cfg_shad,YPos+cfg_shad,MaxXPos+cfg_shad,MaxYPos+cfg_shad,60,0x05,shadowColor,shadowColor);
    
    //Рисуем текст n-ное количество раз с разными координатами:
    DrawString(text,ct,XPos-cfg_bordsize,YPos-cfg_bordsize,MaxXPos-cfg_bordsize,MaxYPos,60,0x05,borderColor,borderColor);
    DrawString(text,ct,XPos-cfg_bordsize,YPos+cfg_bordsize,MaxXPos-cfg_bordsize,MaxYPos,60,0x05,borderColor,borderColor);
    DrawString(text,ct,XPos+cfg_bordsize,YPos-cfg_bordsize,MaxXPos+cfg_bordsize,MaxYPos,60,0x05,borderColor,borderColor);
    DrawString(text,ct,XPos+cfg_bordsize,YPos+cfg_bordsize,MaxXPos+cfg_bordsize,MaxYPos,60,0x05,borderColor,borderColor);
    
    //Рисуем с основными координатами:
    DrawString(text,ct,XPos,YPos,MaxXPos,MaxYPos,60,0x05,NormalColor,NormalColor);
  }
};


void DispDraw(DISP_OBJ* DO,int a,int b,int c)
{ 
  DREDRAW(DO,a,b,c);
  DrawRect(cfg_rect.x1,cfg_rect.y1,cfg_rect.x2,cfg_rect.y2,cfg_bg,cfg_bg);
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  wchar_t str[50];
  snwprintf(str,49,L"%02d:%02d",dt.time.hour,dt.time.min);
  int time=TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN);
  DrawHighlightID(cfg_font, time, cfg_ct, cfg_x, cfg_y, cfg_border, cfg_shadow, cfg_color);
  TextID_Destroy(time);
};

void redraw_init()
{
  DISP_OBJ *si=*StatusRow_p();
  if (si)
  {
    DREDRAW=DispObject_GetOnRedraw(si);
    DISP_DESC_SetOnRedraw(DispObject_GetDESC(si), DispDraw);
    //---------------------------------------------------------
  }
};
