#define USEPNG
#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "math.h"
#include "main.h"
#include "map_object.h"
#include "draw.h"



void SetPixel(unsigned int *buf, int width, int height,int x, int y,int color)
{
  if (x<width && x>=0 && y<height && y>=0)
  {
    buf[y*width+x]=color;   
  }
}

void drawline(unsigned int *buf, int width, int height, int x1, int y1, int x2, int y2,int color)
{
  int a;
  int dx,dy;
  int lengthX,lengthY;
  
  if ((a=x2-x1)>=0)
  {
    dx=1;
    lengthX=a;    
  }
  else
  {
    dx=-1;
    lengthX=-a;
  }
  if ((a=y2-y1)>=0)
  {
    dy=1;
    lengthY=a;
  }
  else
  {
    dy=-1;
    lengthY=-a;
  }
  int length=lengthX>lengthY?lengthX:lengthY;
  if (length == 0)
  {
    SetPixel(buf, width, height, x1, y1, color);
  }
  if (lengthY <= lengthX)
  {
    // Начальные значения
    int x = x1;
    int y = y1;
    int d = -lengthX;
    
    // Основной цикл
    length++;
    while(length--)
    {
      SetPixel(buf, width, height, x, y, color);
      x += dx;
      d += 2 * lengthY;
      if (d > 0) 
      {
        d -= 2 * lengthX;
        y += dy;
      }
    }
  }
  else
  {
    // Начальные значения
    int x = x1;
    int y = y1;
    int d = - lengthY;
    
    // Основной цикл
    length++;
    while(length--)
    {
      SetPixel(buf, width, height, x, y, color);
      y += dy;
      d += 2 * lengthX;
      if (d > 0) 
      {
        d -= 2 * lengthY;
        x += dx;
      }
    }
  }
}


void drawcircle(unsigned int *buf, int width, int height, int xc, int yc, int r, int color)
{
  int y;
  int d;
  y = r;
  d = 3-2*r;
  for (int x=0; y>=x; x++)
  {
    SetPixel(buf,width,height,x+xc,y+yc,color);
    SetPixel(buf,width,height,x+xc,-y+yc,color);
    SetPixel(buf,width,height,-x+xc,y+yc,color);
    SetPixel(buf,width,height,x+xc,-y+yc,color);
    SetPixel(buf,width,height,y+xc,x+yc,color);
    SetPixel(buf,width,height,y+xc,-x+yc,color);
    SetPixel(buf,width,height,-y+xc,x+yc,color);
    SetPixel(buf,width,height,-y+xc,-x+yc,color);
    if( d<0 )
    {
      d = d+4*x+6;
    }
    else
    {
      d = d+4*(x-y)+10;
      y = y-1;
    }
  }
}

void drawfilledrect(unsigned int *buf, int width, int height, int x1, int y1, int x2, int y2, int color)
{
  int dx, dy;
  dx=x2>=x1?1:-1;
  dy=y2>=y1?1:-1;
  
  while(y1!=y2)
  {
    while(x1!=x2)
    {
      SetPixel(buf,width,height,x1,y1,color);
      x1+=dx;
    }
    y1+=dx;
  }
}
