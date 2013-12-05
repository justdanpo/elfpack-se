#include "myclass.h"
void display::out(GC*gc)
{
  DEBUG;
  GC_DrawFRect(gc,0xFFFFFFFF,0,0,_x*_pixel,_y*_pixel);
  GVI_GC gvi_gc_pixel;
  GVI_GC gvi_gc;
  CANVAS_Get_GviGC(gc->pcanvas ,&gvi_gc);
  for(int y=0;y<_y;y++)
    for(int x=0;x<_x;x++)
      if(_display[y][x].state)
      {
        CANVAS_Get_GviGC(gc_pixel[_display[y][x].col]->pcanvas ,&gvi_gc_pixel);
        GVI_BitBlt(gvi_gc,x*_pixel,y*_pixel,_pixel,_pixel,gvi_gc_pixel,0,0,204,0,0,0);
      }
        
};
int display::gety()
{
  DEBUG;
  return _y;
};
int display::getx()
{
  DEBUG;
  return _x;
};
void display::set(int x,int y,COLOR col,bool state)
{
  DEBUG;
  _display[y][x].state=state;
  _display[y][x].col=col;
};
void display::clear()
{
  DEBUG;
  for(int i=0;i<_y;i++)
    for(int j=0;j<_x;j++) _display[i][j].state=0;
};
display::~display()
{
  DEBUG;
  for(int i=0;i<_y;i++) delete[] _display[i];
  delete[] _display;
  for(int i=0;i<8;i++) GC_FreeGC(gc_pixel[i]);
};
display::display(int x,int y,int pixel)
{
  DEBUG;
  _x=x;
  _y=y;
  _pixel=pixel;
  _display=new PXL*[y];
  for(int i=0;i<y;i++) 
  {
    _display[i]=new PXL[x];
    for(int j=0;j<x;j++)
    {
      _display[i][j].state=false;
      _display[i][j].col=WHITE;
    }
  }      
  for(int i=BLACK;i<=WHITE;i++)
  {
    gc_pixel[i]=GC_CreateMemoryGC(pixel,pixel,16,0,0,0);
    GC_DrawFRect(gc_pixel[i],getColor((COLOR)i),0,0,pixel,pixel);
    GC_DrawFRect(gc_pixel[i],getColor(WHITE),1,1,pixel-2,pixel-2);
    GC_DrawFRect(gc_pixel[i],getColor((COLOR)i),2,2,pixel-4,pixel-4);
  }
  clear();
};
int display::getColor(COLOR col)
{
  DEBUG;
  switch(col)
  {
  case BLACK:
    return 0xFF000000;
  case BLUE:
    return 0xFF0000FF;
  case GREEN:
    return 0xFF00FF00;
  case LIGHTBLUE:
    return 0xFF00FFFF;
  case RED:
    return 0xFFFF0000;
  case PURPLE:
    return 0xFFFF00FF;
  case YELLOW:
    return 0xFFFFFF00;
  case WHITE:
    return 0xFFFFFFFF;
  default:
    return 0;
  }
};
