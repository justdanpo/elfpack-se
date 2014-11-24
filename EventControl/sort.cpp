/*
*===========================================================================
*                 ������ ���������� ������ �������
*===========================================================================
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h"
#include "header\structs.h"

/*
*===========================================================================
*         ������� ��������� ���� ��������� ��������.
*===========================================================================
* 1) ���� ������ ��� ������ ������, �� ������������ 1.
* 2) ���� ������ ��� ������ ������, �� ������������ -1.
* 3) ���� ��������� ���������, �� ������������ 0.
*/
int time_cmp(TIME t1, TIME t2)
{
  int n1=(t1.hour*3600) + (t1.min*60) + t1.sec;
  int n2=(t2.hour*3600) + (t2.min*60) + t2.sec;
  if (n1<n2)return 1;
  if (n1==n2)return 0;
  if (n1>n2)return -1;
  return 0;
};

int ev_cmp(EVENT *ev1, EVENT *ev2)
{
  return time_cmp(ev1->time, ev2->time);
};

/*
*===========================================================================
*                ������� ���������� �����
*===========================================================================
*/
void sort_array(void *arr[], int n)
{
  int i;
  int j;
  void * tmp;
  
  for(i = 0; i <= n-1; i++)
  {
    for(j = 0; j <= n-2-i; j++)
    {
      if (ev_cmp((EVENT*)arr[j],(EVENT*)arr[j+1])==-1)
      {
        tmp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = tmp;
      }
    }
  }
};

void SortList(LIST *lst)
{
  sort_array(lst->listdata, lst->FirstFree);
};