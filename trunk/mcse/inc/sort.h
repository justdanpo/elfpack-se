#ifndef _SORT_H
#define _SORT_H

// ��� ��������� �� �-�� ���������
typedef int (*IsGreaterFunc)(void*, void*);

void quickSort(void** a, long size, IsGreaterFunc gt);

#endif
