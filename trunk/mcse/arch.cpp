#include "inc\mc.h"

#include "inc\mui.h"
#include "inc\arch.h"
#include "inc\7zslib.h"
#include "inc\zslib.h"
#include "inc\fn_list.h"

void FreeArchInfo(ARCHINFO* pzi)
{
  if (pzi->pszNames)
  {
    for (int i=0; i < pzi->total_names; i++)
      if (pzi->pszNames[i]) delete (pzi->pszNames[i]);
    delete(pzi->pszNames);
  }
    
  if (pzi->password)
    delete(pzi->password);
    
  memset(pzi, 0, sizeof(ARCHINFO));
  pzi->type = NO_ARCH;
}

int IsArchiveOpened(int tab)
{
  return (tabs[tab]->zipInfo->type);
}

int IsInArchive()
{
  return IsArchiveOpened(curtab);
}


ARCHINFO zi;

void ZeroArchiveBuffer()
{
  memset(&zi,0,sizeof(ARCHINFO));
  zi.type=NO_ARCH;
}

void ArchiveBufferExtractBegin()
{
}

int ArchiveBufferExtract(int type, FN_ITM* pi, wchar_t* extractDir)
{
  int i=0;
  ARCHINFO *pzi=&zi;
  if (pi && pi->zipPath && pi->full)
  {
    int usingZipFromTab = 0;
    for (int i = 0; i < MAX_TABS; i++)
    {
      if (IsArchiveOpened(i)==type && wstrcmpi(tabs[i]->zipInfo->szZipPath, pi->zipPath) == 0)
      {
        // Если уже открыт в табе, то используем его
        pzi = tabs[i]->zipInfo;
        usingZipFromTab = 1;
        break;
      }
    }
    if (!usingZipFromTab)
    {
      if (pzi->type==ZIP_ARCH && wstrcmpi(pzi->szZipPath, pi->zipPath) != 0)
        CloseZip(pzi);
      if (pzi->type==_7Z_ARCH && wstrcmpi(pzi->szZipPath, pi->zipPath) != 0)
        Close7Z(pzi);
      if (type==ZIP_ARCH)
      {
        // Если файл еще не открыт - то открываем
        if (pzi->type == 0)
          if (OpenZip(pzi, pi->zipPath) != UNZ_OK)
            return 0;
      }
      if (type==_7Z_ARCH)
      {
        // Если файл еще не открыт - то открываем
        if (pzi->type == 0)
          if (Open7ZFile(pzi, pi->zipPath) != SZ_OK)
            return 0;
      }
    }
    
    if (pi->ftype == TYPE_ZIP_FILE)
      return ExtractFile(pzi, pi->full, extractDir, 0);
    else if (pi->ftype == TYPE_ZIP_DIR)
      return ExtractDir(pzi, pi->full, extractDir, 1);
    else if (pi->ftype == TYPE_7Z_FILE)
      return ExtractFile7Z(pzi, pi->full, extractDir, 0);
    else if (pi->ftype == TYPE_7Z_DIR)
      return ExtractDir7Z(pzi, pi->full, extractDir, 1);
  }
  return (i);
}

void ArchiveBufferExtractEnd()
{
  if (zi.type==ZIP_ARCH)
    CloseZip(&zi);
  else if (zi.type==_7Z_ARCH)
    Close7Z(&zi);    
}
