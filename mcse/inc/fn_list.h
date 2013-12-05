#ifndef _FN_LIST_H
  #define _FN_LIST_H

typedef struct
{
	int ftype;
	int pname;
	wchar_t* full;
	void* next;
	wchar_t* zipPath;
} FN_ITM;

typedef struct
{
	int count;
	int type;
	FN_ITM *items;
} FN_LIST;

void fn_zero(FN_LIST *list);
void fn_fill(FN_LIST *list, wchar_t* path);
void fn_add(FN_LIST *list, int type, int ftype, int pname, wchar_t* full, wchar_t* zipPath);
void fn_rev(FN_LIST *list);
void fn_free(FN_LIST *list);
void fn_log(FN_LIST *list);
void DestroyPathBuf(void);

#endif
