#include "Graphics.h"

int FillColor, BackgroundColor; //, ForegroundColor; //, BackgroundColor;

int DrawLine(int x1, int y1, int x2, int y2)
{
    GC_DrawLine(get_DisplayGC(), x1, y1, x2, y2);
    return RESULT_OK;
}

int FillRect(int x, int y, int w, int h) // прямоугольник с заливкой
{
    GC_DrawFRect(get_DisplayGC(), FillColor, x, y, w, h);
    return(RESULT_OK);
}

int DrawFrame(int x, int y, int w, int h)
{
   GC *gc = get_DisplayGC();
   int x1 = x;
   int x2 = x+w;
   int y1 = y;
   int y2 = y+h;
   GC_DrawLine(gc, x1, y1, x2, y1);
   GC_DrawLine(gc, x1, y1, x1, y2);
   GC_DrawLine(gc, x1, y2, x2, y2);
   GC_DrawLine(gc, x2, y1, x2, y2);
   return(RESULT_OK);
}

int SetFillColor(int color) // заливка для прямоугольника	
{
    FillColor = color;
    return RESULT_OK;
}

int SetForegroundColor(int color) // устанавливает цвет для линии, текста...
{
    //ForegroundColor = color;
    GC_SetPenColor(get_DisplayGC(), color);
    return RESULT_OK;
}

int SetBackgroundColor(int color) // устанавливает цвет для теней и тд
{
    BackgroundColor = color;
    return(RESULT_OK);
}

int DrawT(TEXTID id, int x, int y, int align)
{
    int ForegroundColor = GC_GetPenColor(get_DisplayGC());
    DrawString(id, align, x, y, Display_GetWidth(DISPLAY), Display_GetHeight(DISPLAY), 0, 0, ForegroundColor, ForegroundColor);
    TextID_Destroy(id);
    return(RESULT_OK);
}

int DrawText(wchar_t *wstr, int x, int y, int align)
{
  TEXTID id = TextID_Create(wstr, ENC_UCS2, TEXTID_ANY_LEN);
  return(DrawT(id, x, y, align));
}

int DrawTextS(char *str, int x, int y, int align)
{
  TEXTID id = TextID_Create(str, ENC_LAT1, strlen(str));
  return(DrawT(id ,x, y, align));
}

void DrawHighlightTextV1(wchar_t *wstr, int x, int y, int align)
{
  int ForegroundColor = GC_GetPenColor(get_DisplayGC());
  SetForegroundColor(BackgroundColor);
  DrawText(wstr, x-1, y-1, align);
  DrawText(wstr, x-1, y+1, align);
  DrawText(wstr, x+1, y-1, align);
  DrawText(wstr, x+1, y+1, align);
  SetForegroundColor(ForegroundColor);
  DrawText(wstr, x, y, align);
}

void DrawHighlightTextV2(wchar_t *wstr, int x, int y, int align)
{
  int ForegroundColor = GC_GetPenColor(get_DisplayGC());
  SetForegroundColor(BackgroundColor);
  DrawText(wstr, x+1, y, align);
  DrawText(wstr, x, y+1, align);
  DrawText(wstr, x-1, y, align);
  DrawText(wstr, x, y-1, align);
  SetForegroundColor(ForegroundColor);
  DrawText(wstr, x, y, align);
}

void DrawShadowText(wchar_t *wstr, int x, int y, int align)
{
  int ForegroundColor = GC_GetPenColor(get_DisplayGC());
  SetForegroundColor(BackgroundColor);
  DrawText(wstr, x+1, y+1, align);
  SetForegroundColor(ForegroundColor);
  DrawText(wstr, x, y, align);
}

void DrawTextWithHighlight(wchar_t *wstr, int x, int y, int align, int highlight)
{
  switch(highlight)
  {
  case 1:
    DrawHighlightTextV1(wstr, x, y, align);
    break;
  case 2:
    DrawHighlightTextV2(wstr, x, y, align);
    break;
  case 3:
    DrawShadowText(wstr, x, y, align);
    break;
  default:
    DrawText(wstr, x, y, align);
  }
}



