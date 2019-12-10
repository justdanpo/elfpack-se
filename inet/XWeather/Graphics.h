#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"

int DrawLine(int x1, int y1, int x2, int y2);
int FillRect(int x, int y, int w, int h);
int DrawFrame(int x, int y, int w, int h);
int SetFillColor(int color);
int SetForegroundColor(int color);
int SetBackgroundColor(int color);
int DrawText(wchar_t *wstr, int x, int y, int align);
int DrawTextS(char *str, int x, int y, int align);
int DrawT(TEXTID id, int x, int y, int align);
void DrawTextWithHighlight(wchar_t *wstr, int x, int y, int align, int highlight);

#endif
