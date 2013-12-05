#ifndef _ARCH_H
#define _ARCH_H

#include "..\7Z\7zHeader.h"
#include "..\7Z\7zCrc.h"
#include "..\7Z\7zIn.h"
#include "..\7Z\7zExtract.h"

enum ARCH_TYPES {
  NO_ARCH=0,
  ZIP_ARCH,
  _7Z_ARCH  
};

typedef struct {
  unzFile uf;
  unz_global_info gi;
  unz_file_info** pfi;
}zipinfo;

typedef struct 
{
  ISzInStream InStream;
  int File;
} CFileInStream;
    
typedef struct {
  CFileInStream archiveStream;
  ISzAlloc allocImp;
  ISzAlloc allocTempImp;
  CArchiveDatabaseEx db;
} _7zinfo;
    
typedef struct
{
  int type;
  wchar_t szZipPath[MAX_PATH];
  wchar_t szCurDir[MAX_PATH];
  wchar_t** pszNames;
  int total_names;
  char* password;
  union 
  {
    zipinfo zip;
    _7zinfo _7z;
  }info;
} ARCHINFO;

void FreeArchInfo(ARCHINFO* pzi);
int IsArchiveOpened(int tab);
int IsInArchive();

void ZeroArchiveBuffer();
void ArchiveBufferExtractBegin();
int ArchiveBufferExtract(int type, FN_ITM* pi, wchar_t* extractDir);
void ArchiveBufferExtractEnd();
#endif
