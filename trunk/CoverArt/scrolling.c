#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "config_data.h"
#include "structs.h"

#define NUM 10
#define LEFT 0
#define RIGHT 1
#define NMODE 2
#define NMODE2 3

extern LABEL labels[NUM];
extern bool isInStandby;

int isScrollable(LABEL *lbl, int mode)
{
   if (lbl->SHOW==true && lbl->scr.SCROLL_USE==true)
   {
     // ДравЛайну надо знать, скроллить или нет :-)
     int len=0;
     if (ScrollingMode==true)
     {
       len=lbl->scr.SCROLL_FLEN;
     }
     else
     {
       len=wstrlen(lbl->label);
     }
     if ((mode==1 || ScrollWhenNot==true) || (mode==1 && ScrollWhenNot==true))
     {
        // Если надо скроллить и если мы находимся в правильном месте
        if (len>=lbl->scr.SCROLL_STRLEN)
        {
          return 1;
        }
     }
   }
   return 0;
};

void NextScrollFrame(int mode)
{
  int x=0;
  for (x=0;x<NUM;x++)
  {
    if (isScrollable(&labels[x],mode))
    {
          labels[x].scr.SCROLL_NEED_SCROLL=true;
          if (labels[x].scr.SCROLL_WHERETO==RIGHT)
          {
            //Скролл идёт вправо
            if (labels[x].X<=labels[x].scr.SCROLL_RIGHT)
            {
              labels[x].X=labels[x].X+labels[x].scr.SCROLL_SPEED;
            }
            else
            {
              labels[x].X=labels[x].scr.SCROLL_LEFT-(labels[x].scr.SCROLL_LENGTH*wstrlen(labels[x].label));
            }
          }
          else if (labels[x].scr.SCROLL_WHERETO==LEFT)
          {
            //Скролл идёт вправо
            if (labels[x].X>=labels[x].scr.SCROLL_LEFT-(labels[x].scr.SCROLL_LENGTH*wstrlen(labels[x].label)))
            {
              labels[x].X=labels[x].X-labels[x].scr.SCROLL_SPEED;
            }
            else
            {
              labels[x].X=labels[x].scr.SCROLL_RIGHT;
            }
          }
          else if (labels[x].scr.SCROLL_WHERETO==NMODE)
          {
            //Режим "от X до X"
            int len2=0;
            if (ScrollingMode==true)
            {
              len2=labels[x].scr.SCROLL_FLEN;
            }
            else
            {
              len2=wstrlen(labels[x].label)*labels[x].scr.SCROLL_LENGTH;
            }
            if (labels[x].scr.SCROLL_WHERE==LEFT)
            {
              //Сейчас скролл идёт влево
              if (labels[x].X > labels[x].scr.SCROLL_LEFT)
              {
                labels[x].X = labels[x].X - labels[x].scr.SCROLL_SPEED;
              }
              else if (labels[x].X <= labels[x].scr.SCROLL_LEFT)
              {
                //Дошло до последней черты, идём влево
                labels[x].scr.SCROLL_WHERE=RIGHT;
                labels[x].X = labels[x].X + labels[x].scr.SCROLL_SPEED;
              }
            }
            else if (labels[x].scr.SCROLL_WHERE==RIGHT)
            {
              //Идёт скролл вправо
              if ((labels[x].X + len2) < labels[x].scr.SCROLL_RIGHT)
              {
                labels[x].X = labels[x].X + labels[x].scr.SCROLL_SPEED;
              }
              else if ((labels[x].X + len2) >= labels[x].scr.SCROLL_RIGHT)
              {
                //Идём вправо
                labels[x].scr.SCROLL_WHERE=LEFT;
                labels[x].X = labels[x].X - labels[x].scr.SCROLL_SPEED;
              }
            }
          }
          else if (labels[x].scr.SCROLL_WHERETO==NMODE2)
          {
            //Режим "от X до X" v2
            int len2=0;
            if (ScrollingMode==true)
            {
              len2=labels[x].scr.SCROLL_FLEN;
            }
            else
            {
              len2=wstrlen(labels[x].label)*labels[x].scr.SCROLL_LENGTH;
            }
            if (labels[x].scr.SCROLL_WHERE==LEFT)
            {
              //Сейчас скролл идёт влево
              if ((labels[x].X + len2) > labels[x].scr.SCROLL_LEFT)
              {
                labels[x].X = labels[x].X - labels[x].scr.SCROLL_SPEED;
              }
              else if ((labels[x].X + len2)<= labels[x].scr.SCROLL_LEFT)
              {
                //Дошло до последней черты, идём влево
                labels[x].scr.SCROLL_WHERE=RIGHT;
                labels[x].X = labels[x].X + labels[x].scr.SCROLL_SPEED;
              }
            }
            else if (labels[x].scr.SCROLL_WHERE==RIGHT)
            {
              //Идёт скролл вправо
              if (labels[x].X < labels[x].scr.SCROLL_RIGHT)
              {
                labels[x].X = labels[x].X + labels[x].scr.SCROLL_SPEED;
              }
              else if (labels[x].X >= labels[x].scr.SCROLL_RIGHT)
              {
                //Идём вправо
                labels[x].scr.SCROLL_WHERE=LEFT;
                labels[x].X = labels[x].X - labels[x].scr.SCROLL_SPEED;
              }
            }
          }
    }
    else
    {
      labels[x].scr.SCROLL_NEED_SCROLL=false;
      labels[x].X=labels[x].scr.SCROLL_DEF_X;
    }
  }
};


