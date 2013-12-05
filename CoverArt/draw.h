#ifndef _DRAW_H_
  #define _DRAW_H_

  void DrawLine(bool DrawHZ,int Font,/*wchar_t NAME[31]*/int SIDtime,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor, int len, bool isAnimated, LABEL *lbl);
  void DrawHighlightID(int Font,int SIDtime,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int borderColor, int NormalColor);
  void DrawProgress(int bColor, int cColor, int oColor, RECT rect, int progress, int time, signed int PlusY, int style);
  
#endif
