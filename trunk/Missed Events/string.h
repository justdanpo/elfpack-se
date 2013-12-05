#include "..\\include\Types.h"

char* strchr(char* str,char c);
//wchar_t*  wstrchr(wchar_t* str,wchar_t c);
char* strrchr(char* str,char c);
//u16* wstrrchr(u16* str,u16 c); //declared in lib
char* strncpy(char* dest,const char* src,size_t len);
wchar_t*  wstrncpy(wchar_t* dest, const wchar_t* src,size_t len);
int   strncmp(char* s1,char* s2,size_t len);
int   wstrncmp(wchar_t* s1,wchar_t* s2,size_t len);
char  upper(char c);
int   strncmpi(const char* s1, const char* s2,size_t len);
char* strstr (const char * str1, const char * str2);
wchar_t*  wstrstr (const wchar_t * str1, const wchar_t * str2);

