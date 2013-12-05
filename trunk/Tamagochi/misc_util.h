#ifndef _UTIL_H_
#define _UTIL_H_

#include "..\\include\Types.h"

inline int islower(int _C) { return (_C>='a' && _C<='z');}
inline int isupper(int _C) { return (_C>='A' && _C<='Z');}
inline int tolower(int _C) { return (isupper(_C)?_C-'A'+'a':_C);}
inline int toupper(int _C) { return (islower(_C)?_C-'a'+'A':_C);}

//#ifdef __cplusplus
//extern "C"
//{
//#endif /* __cplusplus */ 

int strncmpi(const char* s1, const char* s2, size_t len);
int towlower(int c);
int towupper(int c);

void splitfilename(const wchar_t* src, wchar_t* destpath, wchar_t* destname);
void extractdir(wchar_t* path,wchar_t* name, const char* src);
void getdir(wchar_t* path, const char* src);

//#ifdef __cplusplus
//}
//#endif /*__cplusplus*/

extern "C" unsigned long wcstoul(const wchar_t *, wchar_t **, int);
 
#endif /* _UTIL_H_ */ 
