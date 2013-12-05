#ifndef _7ZSLIB_H
#define _7ZSLIB_H

#include "inc\arch.h"


int IsIt7ZipFile(wchar_t* fname);
int OpenTab7Z(int tab, wchar_t* zipFileName);
int Fill7ZFiles(int tab, wchar_t* subdname);
void CloseTab7Z(int tab);
int Open7ZFile(ARCHINFO* arc, wchar_t* zipFileName);
void Close7Z(ARCHINFO* arc);
int ExtractFileByID7Z(ARCHINFO* pzi, int id, wchar_t* extractDir, int usePaths, int ip);
int ExtractFile7Z(ARCHINFO* pzi, wchar_t* fname, wchar_t* extractDir, int usePaths);
int ExtractDir7Z(ARCHINFO* pzi, wchar_t* dname, wchar_t* extractDir, int usePaths);
#endif
