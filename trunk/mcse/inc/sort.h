#ifndef _SORT_H
#define _SORT_H

// Тип указателя на ф-ию сравнения
typedef int (*IsGreaterFunc)(void*, void*);

void quickSort(void** a, long size, IsGreaterFunc gt);

#endif
