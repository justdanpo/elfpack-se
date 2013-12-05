/*
****************************************************************************
*                    Модуль вычисления даты
****************************************************************************
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 

#define IS_LARGE_YEAR(a) ((a%4==0 && a%100!=0) || a%400==0)

static int mon_list[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
*===========================================================================
*                   Вычисление даты...
*===========================================================================
* 1) в р0 необходимо отправить текущую дату, в р1 - на какое кол-во дней 
*    надо увеличить дату
* 2) в day, mon, year запишется день, месяц и год
*/
void date_increase(DATE date, int num, char *day, char *mon, u16 *year)
{
  int dy=0,mn=0,yr=0;
  dy=date.day;
  mn=date.mon;
  yr=date.year;
  dy+=num;
  if(IS_LARGE_YEAR(yr) == 0)
  {
    mon_list[1] = 28;
  }
  else
  {
    mon_list[1] = 29;
  }
  if (dy>mon_list[mn-1])
  {
  P_large_again:
    dy=dy-mon_list[mn-1];
    mn++;
    if (mn>12)
    {
      yr++;
      mn=1;
    }
    if (dy>mon_list[mn-1])goto P_large_again;
    goto P_go_away;
  }
  else if (dy<=1)
  {
  P_neg_again:
    mn--;
    if (mn==0)
    {
      mn=12;
      yr--;
    }
    dy=mon_list[mn-1]+dy;
    if (dy<1)goto P_neg_again;
    goto P_go_away;
  }
P_go_away:
  day[0]=dy;
  mon[0]=mn;
  year[0]=yr;
};
