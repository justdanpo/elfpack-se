#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\sort.h"

#define MAXSTACK 2048		// максимальный размер стека
#define SWAP(x, y) { void* t = a[x]; a[x] = a[y]; a[y] = t;  }

void quickSort(void** a, long size, IsGreaterFunc gt)
{

	long i, j;   		// указатели, участвующие в разделении
	long lb, ub;  		// границы сортируемого в цикле фрагмента
	long* lbstack = new long[MAXSTACK];
	if (!lbstack)
	{
		MsgBoxError(muitxt(ind_err_nomemory));
		return;
	}
	long* ubstack = new long[MAXSTACK]; // стек запросов
	if (!ubstack)
	{
		delete (lbstack);
		MsgBoxError(muitxt(ind_err_nomemory));
		return;
	}
	// каждый запрос задается парой значений,
	// а именно: левой(lbstack) и правой(ubstack) 
	// границами промежутка
	long stackpos = 1;   	// текущая позиция стека
	long ppos;			// середина массива
	void* pivot;		  // опорный элемент

	lbstack[1] = 0;
	ubstack[1] = size-1;

	do
	{
		// Взять границы lb и ub текущего массива из стека.
		lb = lbstack[stackpos];
		ub = ubstack[stackpos];
		stackpos--;

		do
		{
			// Шаг 1. Разделение по элементу pivot
			ppos = ( lb + ub ) >> 1;
			i = lb; j = ub; pivot = a[ppos];

			do
			{
				while (gt(pivot, a[i])) i++;
				while (gt(a[j], pivot)) j--;

				if (i <= j)
				{
					SWAP(i, j);
					i++; j--;
				}
			}
			while ( i <= j );

			// Сейчас указатель i указывает на начало правого подмассива,
			// j - на конец левого (см. иллюстрацию выше), lb ? j ? i ? ub.
			// Возможен случай, когда указатель i или j выходит за границу массива

			// Шаги 2, 3. Отправляем большую часть в стек  и двигаем lb,ub

			if (i < ppos) // правая часть больше
			{	 
				if (i < ub) //  если в ней больше 1 элемента - нужно 
				{	 
					stackpos++;	   //  сортировать, запрос в стек
					lbstack[stackpos] = i;
					ubstack[stackpos] = ub;
				}
				ub = j;			 //  следующая итерация разделения
				//  будет работать с левой частью
			}
			else // левая часть больше
			{	   		
				if ( j > lb )
				{ 
					stackpos++;
					lbstack[stackpos] = lb;
					ubstack[stackpos] = j;
				}
				lb = i;
			}

		}
		while (lb < ub);		// пока в меньшей части более 1 элемента
	}
	while (stackpos != 0);	// пока есть запросы в стеке
	
	delete (ubstack);
	delete (lbstack);
}

void quickSort1(void** a, long size, IsGreaterFunc gt)
{
  long i = 0, j = size - 1; // начальные значения
  void *temp;
  void *p = a[ size>>1 ]; // выбираем середину
  // процедура разделения
  do {
    while (gt(p,a[i])) i++;
    while (gt(a[j],p)) j--;
    
    if (i <= j) {
      SWAP(i, j);
      i++; j--;
    }
  } while (i <= j);
  
  // рекурсивные вызовы, если есть, что сортировать
  if ( j > 0 ) quickSort(a, j,gt);
  if ( size > i ) quickSort(a+i, size-i,gt);
}