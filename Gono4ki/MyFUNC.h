#include "StructBook.h"

#ifndef _MyFUNC_H_
  #define _MyFUNC_H_
MyBook*MyBK();
wchar_t * GetConfigPath();
int FindFile(wchar_t*path, wchar_t const*name);
int Perestava(TABL*tab, wchar_t new_name[8], int new_result, int N);
wchar_t * StatusSettings(int s);
BOOK*isBookX(char const*isbook, int x);
wchar_t * StatusSettings1(int s);
int GetStrokaIMG(wchar_t*stroka, int x);
wchar_t*NamesOfPunkts(int x);
wchar_t*GetNamesOfIMN(int x, int);
void vibra();
int Record(TABL*tab, int new_result, int N);
int GetDis();
int GetFSize(int f);
int PlayMyMusic(int x);
void DELTABL(TABL*t);
#endif
