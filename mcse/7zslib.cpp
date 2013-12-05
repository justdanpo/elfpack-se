#include "inc\mc.h"
#include "inc\7zslib.h"
#include "inc\mui.h"

const wchar_t _7z_ext[] = L"7z";


void Close7Z(ARCHINFO* arc);

int IsIt7ZipFile(wchar_t* fname)
{
  if (CONFIG_7Z_DETECT_BY) // by content
  {
    unsigned char buf[k7zSignatureSize];
    int f =w_fopen(fname, WA_Read, 0x1FF, NULL);
    if (f>=0)
    {
      if (w_fread(f, &buf, k7zSignatureSize) == k7zSignatureSize)
      w_fclose(f);
    }
    return (memcmp(buf,k7zSignature,k7zSignatureSize)==0);
  }
  else // by extension
  {
    return (wstrcmpi(GetFileExt(fname), _7z_ext) == 0);
  }
}

#ifdef _LZMA_IN_CB

#define kBufferSize (1 << 12)
Byte g_Buffer[kBufferSize];

SZ_RESULT SzFileReadImp(void *object, void **buffer, size_t maxRequiredSize, size_t *processedSize)
{
  CFileInStream *s = (CFileInStream *)object;
  size_t processedSizeLoc;
  if (maxRequiredSize > kBufferSize)
    maxRequiredSize = kBufferSize;
  processedSizeLoc = w_fread(s->File, g_Buffer, maxRequiredSize);
  *buffer = g_Buffer;
  if (processedSize != 0)
    *processedSize = processedSizeLoc;
  return SZ_OK;
}

#else

SZ_RESULT SzFileReadImp(void *object, void *buffer, size_t size, size_t *processedSize)
{
  CFileInStream *s = (CFileInStream *)object;
  size_t processedSizeLoc = w_fread(s->File, buffer, size);
  if (processedSize != 0)
    *processedSize = processedSizeLoc;
  return SZ_OK;
}

#endif

SZ_RESULT SzFileSeekImp(void *object, CFileSize pos)
{
  CFileInStream *s = (CFileInStream *)object;
  w_lseek(s->File, (long)pos, WSEEK_SET);
  return SZ_OK;
}

int Open7ZFile(ARCHINFO* arc, wchar_t* zipFileName)
{
  int zerr = UNZ_ERRNO;

  {
    _7zinfo *zinfo=&arc->info._7z;
    zinfo->archiveStream.File = w_fopen(zipFileName, WA_Read, 0x1FF,0);
    arc->total_names=0;
    if (zinfo->archiveStream.File < 0)
    {
      return 1;
    }
    zinfo->archiveStream.InStream.Read = SzFileReadImp;
    zinfo->archiveStream.InStream.Seek = SzFileSeekImp;
    
    zinfo->allocImp.Alloc = SzAlloc;
    zinfo->allocImp.Free = SzFree;
    
    zinfo->allocTempImp.Alloc = SzAllocTemp;
    zinfo->allocTempImp.Free = SzFreeTemp;
    WriteLog("InitCrcTable");  
    InitCrcTable();
    WriteLog("SzArDbExInit(&zinfo->db)");
    SzArDbExInit(&zinfo->db);
    WriteLog("SzArchiveOpen");
    zerr = SzArchiveOpen(&zinfo->archiveStream.InStream, &zinfo->db, &zinfo->allocImp, &zinfo->allocTempImp);
    WriteLog(zerr);
    if (zerr == SZ_OK)
    {
      arc->type=_7Z_ARCH;
      arc->szCurDir[0] = '\0';
      wstrcpy(arc->szZipPath, zipFileName);
      
      if (zinfo->db.Database.NumFiles >0)
      {
        arc->total_names=zinfo->db.Database.NumFiles;
        int useProgressBar = !progr_start;
        if (useProgressBar)
        {
          // запускаем прогресс бар, если уже не запущен другой
          Busy = 1;
          initprogr(ind_msg_zreading);
          progr_max = arc->total_names;
          incprogr(0);
        }
        arc->pszNames = new wchar_t *[arc->total_names];
        if (arc->pszNames)
        {
          memset(arc->pszNames, 0, arc->total_names*sizeof(wchar_t *));
        }
        for (int i = 0; i < arc->total_names; i++)
        {
          CFileItem *f = zinfo->db.Database.Files + i;
          arc->pszNames[i] = new wchar_t[strlen(f->Name)+1];
          int len=utf8_to_utf16(f->Name,strlen(f->Name),arc->pszNames[i]);
          arc->pszNames[i][len]=0;
          WriteLog(arc->pszNames[i]);
          if (useProgressBar) incprogr(1);
        }
        
        if (useProgressBar)
        {
          endprogr(); // останавливаем прогресс бар
          Busy = 0;
        }
      }
    }
  }
  if (zerr != SZ_OK)
    Close7Z(arc);
  return zerr;
}

void Close7Z(ARCHINFO* arc)
{
  if (arc->type==_7Z_ARCH)
  {
    _7zinfo *zinfo=&arc->info._7z;
    SzArDbExFree(&zinfo->db, zinfo->allocImp.Free);
    w_fclose(zinfo->archiveStream.File);
    FreeArchInfo(arc);
  }
}

void CloseTab7Z(int tab)
{
  Close7Z(tabs[tab]->zipInfo);
}


int OpenTab7Z(int tab, wchar_t* zipFileName)
{
  int zerr = UNZ_ERRNO;
  
  if (!IsArchiveOpened(tab)) // не обрабатываем вложенные архивы
  {
    zerr = Open7ZFile(tabs[tab]->zipInfo, zipFileName);
  }
  return zerr;
}


int Fill7ZFiles(int tab, wchar_t* subdname)
{
  int num = 0;
  wchar_t buf[0x100];
  wchar_t* pszDirName = subdname[0] == '/' ? subdname + 1 : subdname; // ѕропустим в начале обратный слеш
  int dirLen = wstrlen(pszDirName);
  ARCHINFO* arc = tabs[tab]->zipInfo;
  
  if (arc->type==_7Z_ARCH)
  {
    _7zinfo *zinfo=&arc->info._7z;
    for (int i=0; i < zinfo->db.Database.NumFiles; i++)
    {
      int ignore = 1;
      wstrncpy(buf, arc->pszNames[i],0x100-1);
      wchar_t* pFileNameStart = &buf[0];
      if (dirLen == 0)
      {
        // ≈сли ищем корневые элементы
        // “о добавл€ем все файлы у которых нет слешей '\'
        wchar_t* slashPos = wstrwstr(buf, L"/");
        ignore = (slashPos != NULL);
        if (ignore)
        {
          // » добавл€ем все директории - у них один слеш, и тот последний
          ignore = (*(slashPos + 1) != 0);
          if (!ignore)
          {
            // «атрем этот слеш, чтобы не выводилс€
            *slashPos = 0;
          }
        }
      }
      else
      {
        // —равниваем начало, если равно и потом слеш - то это элементы нашей директории
        pFileNameStart = pFileNameStart + dirLen;
        ignore = (wstrcmpni(buf, pszDirName, dirLen) != 0 || *pFileNameStart != '/');
        if (!ignore)
        {
          pFileNameStart = pFileNameStart + 1; // пропустим слеш после имени директории
          // игнорим саму текущую, директорию
          ignore = (*pFileNameStart == 0);
          if (!ignore)
          {
            // проверка на поддиректорию
            wchar_t* pFileNameEnd = wstrwstr(pFileNameStart, L"/");
            if (pFileNameEnd != NULL)
            {
              // проверка на то что это не сама поддиректори€ а ее элементы, оные проигнорим
              ignore = (*(pFileNameEnd+1) != 0);
              if (!ignore)
              {
                *pFileNameEnd = 0; // грохнем слеш в конце
              }
            }
          }
        }
      }
      if (!ignore)
      {
        CFileItem *f = zinfo->db.Database.Files + i;
        AddFile(tab, i, pFileNameStart, f->Size, f->IsDirectory?FA_DIRECTORY:0, 0,
                0, f->IsDirectory? TYPE_7Z_DIR : TYPE_7Z_FILE);
        num++;
      }
    }
  }
  return num;
}



int ExtractFileByID7Z(ARCHINFO* pzi, int id, wchar_t* extractDir, int usePaths, int ip)
{
  int res=SZ_OK;
  int fout=-1;
  wchar_t *filePathInZip = pzi->pszNames[id];
  wchar_t *extractFilePath=new wchar_t[MAX_PATH];
  wchar_t *temp=NULL;
  CFileItem *file = pzi->info._7z.db.Database.Files + id;
  if (filePathInZip == NULL)
  {
    // —читываем им€ сами если нужно
    temp=new wchar_t[MAX_PATH];
    int len=utf8_to_utf16(file->Name,MAX_PATH-1,temp);
    temp[len]=0;
    filePathInZip=temp;
  }
  wchar_t* fileNameInZip=GetFileName(filePathInZip);
  if (file->IsDirectory)
  {
    if (usePaths)
    {
      // ƒиректори€... создаем
      snwprintf(extractFilePath,MAX_PATH-1, _ls_ls, extractDir, filePathInZip);
      mktree(extractFilePath);
    }
  }
  else
  {
    // File
    if (usePaths)
      snwprintf(extractFilePath,MAX_PATH-1, _ls_ls, extractDir, filePathInZip);
    else
      snwprintf(extractFilePath,MAX_PATH-1, _ls_ls, extractDir, fileNameInZip);
  
    if (res == SZ_OK)
    {
      fileNameInZip=GetFileName(extractFilePath);
      fout = w_fopen(extractFilePath, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
       	// some zipfile don't contain directory alone before file 
      if (fout < 0 && usePaths && fileNameInZip != extractFilePath)
      {
        wchar_t c = *(fileNameInZip - 1);
        *(fileNameInZip - 1) = '\0';
        mktree(extractFilePath);
        *(fileNameInZip - 1) = c;
        fout = w_fopen(extractFilePath, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
      }
      if (fout<0)
        res = 3;
    }
    if (res == UNZ_OK)
    {
      UInt32 blockIndex = 0xFFFFFFFF; // it can have any value before first call (if outBuffer = 0) 
      Byte *outBuffer = 0; // it must be 0 before first call for each new archive. 
      size_t outBufferSize = 0;  // it can have any value before first call (if outBuffer = 0) 
      size_t offset;
      size_t outSizeProcessed;
      ISzAlloc allocImp;
      ISzAlloc allocTempImp;
      allocImp.Alloc = SzAlloc;
      allocImp.Free = SzFree;
    
      allocTempImp.Alloc = SzAllocTemp;
      allocTempImp.Free = SzFreeTemp;
      res = SzExtract(&pzi->info._7z.archiveStream.InStream, &pzi->info._7z.db, id, 
                      &blockIndex, &outBuffer, &outBufferSize, 
                      &offset, &outSizeProcessed,
                      &allocImp, &allocTempImp);
      w_fwrite(fout,outBuffer+offset,outSizeProcessed);
      w_fclose(fout);
      allocImp.Free(outBuffer);    
    }
  }
  if (temp) delete temp;
  delete extractFilePath;
  return res;
}

int ExtractFile7Z(ARCHINFO* pzi, wchar_t* fname, wchar_t* extractDir, int usePaths)
{
  // ѕозиционируемс€ на нужном файле
  int found = 0;
  wchar_t buf[MAX_PATH + 1];
  wstrcpy(buf + 1, fname);
  buf[0] = '/'; // ¬ начало запишем слеш на тот случай если fname без слеша
  
  wchar_t* nameWithoutSlash = ( (buf[1] == '/') ? (buf + 2) : (buf + 1) );
  wchar_t* nameWithSlash = ( (buf[1] == '/') ? (buf + 1) : (buf) );
  
  for (int i = 0; i < pzi->total_names; i++)
  {
    found = (wstrcmpi(nameWithoutSlash, pzi->pszNames[i]) == 0);
    if (!found)
    {
      // ѕробуем поискать со слешем
      found = (wstrcmpi(nameWithSlash, pzi->pszNames[i]) == 0);
    }
    if (found)
    {
      // »звлекаем его, если нашли
      return ExtractFileByID7Z(pzi, i, extractDir, usePaths, 0);
    }
  }
  return 1;   
}


int ExtractDir7Z(ARCHINFO* pzi, wchar_t* dname, wchar_t* extractDir, int usePaths)
{
  int found = 0;
  wchar_t *buf=new wchar_t[MAX_PATH + 1];
  wstrcpy(buf + 1, dname);
  buf[0] = '/'; // ¬ начало запишем слеш на тот случай если fname без слеша
  wchar_t* nameWithoutSlash = ( (buf[1] == '/') ? (buf + 2) : (buf + 1) );
  wchar_t* nameWithSlash = nameWithoutSlash-1;
  
  int zerr = UNZ_OK;
  int dlen = wstrlen(nameWithoutSlash);
  
  // ѕеребираем все файлы
  for (int i = 0; i < pzi->total_names; i++)
  {
    // ѕровер€ем начинаетс€ ли им€ файла в зипе с этой директории, если да - извлекаем
    found = (wstrcmpni(nameWithoutSlash, pzi->pszNames[i], dlen) == 0);
    if (!found)
    {
      // ѕробуем поискать со слешем
      found = (wstrcmpni(nameWithSlash, pzi->pszNames[i], dlen + 1) == 0);
    }
    if (found)
      ExtractFileByID7Z(pzi, i, extractDir, 1, 0);
  }
  delete buf;
  return zerr;
}


