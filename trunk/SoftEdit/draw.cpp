#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 

void DrawHighlightID(int font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor)
{
  SetFont(font); //Устанавливаем шрифт из первого параметра функции.
  //Рисуем текст n-ное количество раз с разными координатами:
  DrawString(text,CEN,XPos-1,YPos-1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,CEN,XPos-1,YPos+1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,CEN,XPos+1,YPos-1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,CEN,XPos+1,YPos+1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  //Рисуем с основными координатами:
  DrawString(text,CEN,XPos,YPos,MaxXPos,MaxYPos,60,0x05,NormalColor,NormalColor);
};

void DrawHighlightID2(int font,int text,int ct, int XPos, int YPos, int borderColor, int NormalColor, bool clean)
{
  SetFont(font); //Устанавливаем шрифт из первого параметра функции.
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
  //Рисуем текст n-ное количество раз с разными координатами:
  DrawString(text,ct,XPos-1,YPos-1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,ct,XPos-1,YPos+1,MaxXPos-1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,ct,XPos+1,YPos-1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  DrawString(text,ct,XPos+1,YPos+1,MaxXPos+1,MaxYPos,60,0x05,borderColor,borderColor);
  //Рисуем с основными координатами:
  DrawString(text,ct,XPos,YPos,MaxXPos,MaxYPos,60,0x05,NormalColor,NormalColor);
  if (clean)TextID_Destroy(text);
};

