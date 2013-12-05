#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
#include "header\string.h"

typedef struct
{
  int x;
  int y;
  int ct;
  int font;
  int hx;
  int hy;
  int hct;
  int hfont;
}UI_SOFTKEY;

UI_SOFTKEY sk[3]={ {0,296,0,0x18,320,220,1,0x12} , {0,293,2,0x2C,320,115,1,0x2C} , {240,296,1,0x18,320,0,1,0x12} };
int sk240[24]=   {  0,296,0,0x18,320,220,1,0x12  ,  0,293,2,0x2C,320,115,1,0x2C  ,  240,296,1,0x18,320,0,1,0x12 };
int sk176[24]=   {  0,198,0,0x18,220,156,1,0x12  ,  0,193,2,0x2C,220,80 ,1,0x2C  ,  176,198,1,0x18,220,0,1,0x12 };

void auto_select()
{
  int a=Display_GetHeight(0);
  switch (a)
  {
  case 320:
    memcpy(&sk[0], &sk240[0], 96);
    break;
  case 220:
    memcpy(&sk[0], &sk176[0], 96);
    break;
  default:
    memcpy(&sk[0], &sk176[0], 96);
    break;
  }
};

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
