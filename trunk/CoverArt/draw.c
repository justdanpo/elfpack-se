#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "structs.h"
#include "config_data.h"

extern bool isInStandby;

void DrawLine(bool MustDraw,int Font,int SIDtime/*wchar_t *NAME*/,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int len, bool isAnimated, LABEL *lbl)
{
  if (MustDraw==true){
    if (isAnimated==true)
    {
      // Анимированному тексту обязательно надо указывать все координаты.
      //int SIDtime;
      SetFont(Font);
      //SIDtime=TextID_Create(NAME,ENC_UCS2,len);
      DrawString(SIDtime,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos,YPos,MaxXPos,MaxYPos,60,0x05,NormalColor,NormalColor);
      //TextID_Destroy(SIDtime);
    }
    else
    {
      // Текст неанимированный, значит надо подобрать ему второстепенные координаты правильно :-)
      //int SIDtime;
      SetFont(Font);
      //SIDtime=TextID_Create(NAME,ENC_UCS2,len);
      int dist=lbl->FONT_SIZE;
      if (lbl->LINE_ON)
      {
        dist=(lbl->LINE_DIST+lbl->FONT_SIZE)-lbl->LINE_MINUS;
        if (CEN==2)
        {
          if (lbl->scr.SCROLL_NEED_SCROLL)
          {
            MaxXPos=XPos+lbl->LINE_SIZE;
            MaxYPos=YPos+500;
          }
          else
          {
            XPos=-(lbl->LINE_SIZE/2);
            MaxXPos=Display_GetWidth(0) + (lbl->LINE_SIZE/2);
            MaxYPos=YPos+500;
          }
        }
        else if (CEN==0)
        {
          MaxXPos=XPos+lbl->LINE_SIZE;
          MaxYPos=YPos+500;
        }
        else if (CEN==1)
        {
          MaxXPos=XPos;
          XPos=XPos-lbl->LINE_SIZE;
          MaxYPos=YPos+500;
        }
      }
      else
      {
        if (CEN==2)
        {
          if (lbl->scr.SCROLL_NEED_SCROLL)
          {
            MaxXPos=XPos+500;
            MaxYPos=YPos+500;
          }
          else
          {
            XPos=-500;
            MaxXPos=Display_GetWidth(0) + 500;
            MaxYPos=YPos+500;
          }
        }
        else if (CEN==0)
        {
          MaxXPos=XPos+500;
          MaxYPos=YPos+500;
        }
        else if (CEN==1)
        {
          MaxXPos=XPos;
          XPos=-500;
          MaxYPos=YPos+500;
        }
      }
      DrawString(SIDtime,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,dist,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,dist,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,dist,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,dist,0x05,borderColor,borderColor);
      DrawString(SIDtime,CEN,XPos,YPos,MaxXPos,MaxYPos,dist,0x05,NormalColor,NormalColor);
      //TextID_Destroy(SIDtime);
    }
  }
};

void DrawHighlightID(int Font,int SIDtime,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(Font); // Устанавливаем шрифт из первого параметра функции.
  // Рисуем текст n-ное количество раз с разными координатами:
  DrawString(SIDtime,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(SIDtime,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  // Рисуем с основными координатами:
  DrawString(SIDtime,CEN,XPos,YPos,MaxXPos,MaxYPos,60,0x05,NormalColor,NormalColor);
};

void DrawProgress(int bColor, int cColor, int oColor, RECT rect, int progress, int time, signed int PlusY, int type)
{
  if (ProgressBar_ColorFromTheme)
  {
    bColor=GetThemeColor(0 , THEMEITEM_PROGRESS_BAR_OUTLINE);
    oColor=GetThemeColor(0 , THEMEITEM_PROGRESS_BAR_FILL);
    cColor=GetThemeColor(0 , THEMEITEM_PROGRESS_BAR_SLIDER);
  }
  if (type)
  {
    // Закруглённый прогрессбар
    DrawRect(rect.x1+1,rect.y1+PlusY,rect.x2-1,rect.y1+PlusY+1, bColor, bColor);//Сверху
    DrawRect(rect.x2-1,rect.y1+PlusY+1,rect.x2,rect.y2+PlusY-1, bColor, bColor);//Справа
    
    DrawRect(rect.x1,rect.y1+PlusY+1,rect.x1+1,rect.y2+PlusY-1, bColor, bColor); //Слева
    DrawRect(rect.x1+1,rect.y2+PlusY-1,rect.x2-1,rect.y2+PlusY, bColor, bColor); //Снизу

    DrawRect(rect.x1+1,rect.y1+1+PlusY,rect.x2-1,rect.y2-1+PlusY, oColor, oColor);
    float Length;
    float CurLength=(float)progress;
    float inLength=(float)time;
    float Procent=(float)CurLength/inLength;
    float size=(float)((float)rect.x2-(float)rect.x1);
    float start = (float)rect.x1;
    Length = start+((Procent)*size);
    DrawRect(rect.x1+1, rect.y1+1+PlusY,(int)Length - 1,rect.y2-1+PlusY,cColor,cColor);
  }
  else
  {
    // Обычный прогрессбар
    DrawRect(rect.x1+1,rect.y1+PlusY,rect.x2-1,rect.y1+PlusY+1, bColor, bColor);//Сверху
    DrawRect(rect.x2-1,rect.y1+PlusY,rect.x2,rect.y2+PlusY, bColor, bColor);//Справа
    
    DrawRect(rect.x1,rect.y1+PlusY,rect.x1+1,rect.y2+PlusY, bColor, bColor); //Слева
    DrawRect(rect.x1+1,rect.y2+PlusY-1,rect.x2-1,rect.y2+PlusY, bColor, bColor); //Снизу
    
    DrawRect(rect.x1+1,rect.y1+1+PlusY,rect.x2-1,rect.y2-1+PlusY, oColor, oColor);
    float Length;
    float CurLength=(float)progress;
    float inLength=(float)time;
    float Procent=(float)CurLength/inLength;
    float size=(float)((float)rect.x2-(float)rect.x1);
    float start = (float)rect.x1;
    Length = start+((Procent)*size);
    DrawRect(rect.x1+1, rect.y1+1+PlusY,(int)Length - 1,rect.y2-1+PlusY,cColor,cColor);
  }
};

