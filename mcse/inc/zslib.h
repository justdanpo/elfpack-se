#ifndef _ZSLIB_H
#define _ZSLIB_H

#define CASESENSITIVITY (0)
#define MAX_PATH_INZIP  (256)
#define WRITEBUFFERSIZE (2048)

// Common functions
void FreeZipInfo(ARCHINFO* pzi);
int ReadZipInfo(ARCHINFO* pzi);
int OpenZip(ARCHINFO* pzi,wchar_t* zipFileName);
void CloseZip(ARCHINFO* pzi);
int ExtractCurrentFile(ARCHINFO* pzi, int ind, wchar_t* extractDir, int usePaths, int ip);
int ExtractFile(ARCHINFO* pzi, wchar_t* fname, wchar_t* extractDir, int usePaths);
int ExtractFileByID(ARCHINFO* pzi, int id, wchar_t* extractDir, int usePaths, int ip);
int ExtractDir(ARCHINFO* pzi, wchar_t* dname, wchar_t* extractDir, int usePaths);

// Tab functions
int OpenTabZip(int tab, wchar_t* zipFileName);
void CloseTabZip(int tab);
int FillZipFiles(int tab, wchar_t* subdname);

// Buffer functions
void ZipBufferExtractBegin(ARCHINFO* pzi);
int ZipBufferExtract(ARCHINFO* pzi, FN_ITM* pi, wchar_t* extractDir);
void ZipBufferExtractEnd(ARCHINFO* pzi);

int EnumZipFiles(ARCHINFO* pzi, char* subdname, ENUM_SEL_PROC enumproc, unsigned int param);
int IsItZipFile(wchar_t* fname);

#endif
