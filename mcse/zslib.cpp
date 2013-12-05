#include "inc\mc.h"
#include "inc\zslib.h"
#include "inc\mui.h"
#include "inc\arch.h"


const wchar_t zip_ext[] = L"zip";

void ZipError(int zipErrNum, char* procName)
{
  snwprintf(msgbuf, MAXELEMS(msgbuf),muitxt(ind_err_ziperr), zipErrNum, procName);
  MsgBoxError(msgbuf);
}



void CloseZip(ARCHINFO* pzi)
{
  if (pzi->info.zip.uf)
  {
    unzClose(pzi->info.zip.uf);
    FreeZipInfo(pzi);
  }
}

void CloseTabZip(int tab)
{
  CloseZip(tabs[tab]->zipInfo);
}

void FreeZipInfo(ARCHINFO* pzi)
{
  if (pzi)
  {
    if (pzi->info.zip.pfi)
    {
      for (int i=0; i < pzi->info.zip.gi.number_entry; i++)
        if (pzi->info.zip.pfi[i]) delete (pzi->info.zip.pfi[i]);
      delete(pzi->info.zip.pfi);
    }
    FreeArchInfo(pzi);
  }
}


// Считываем и сохраняем информацию о содержимом архива в память
int ReadZipInfo(ARCHINFO* pzi)
{
	int zerr = UNZ_OK;

	if (pzi->info.zip.uf)
	{
		zerr = unzGetGlobalInfo(pzi->info.zip.uf, &pzi->info.zip.gi);
		if (zerr == UNZ_OK)
		{
			if (pzi->info.zip.gi.number_entry > 0)
			{
                                pzi->total_names=pzi->info.zip.gi.number_entry;
				int useProgressBar = !progr_start;
				if (useProgressBar)
				{
					// запускаем прогресс бар, если уже не запущен другой
					Busy = 1;
					initprogr(ind_msg_zreading);
					progr_max = pzi->total_names;
					incprogr(0);
				}
			
				pzi->pszNames = new wchar_t *[pzi->total_names];
				if (pzi->pszNames)
				{
					memset(pzi->pszNames, 0, pzi->total_names*sizeof(wchar_t *));
					pzi->info.zip.pfi = new unz_file_info*[pzi->total_names];
					if (pzi->info.zip.pfi) memset(pzi->info.zip.pfi, 0, pzi->total_names*sizeof(unz_file_info *));
				}
				
				if (pzi->pszNames && pzi->info.zip.pfi)
				{
					zerr = unzGoToFirstFile(pzi->info.zip.uf);
					if (zerr == UNZ_OK)
					{
						for (int i=0; i < pzi->total_names && zerr == UNZ_OK && !progr_stop; i++)
						{
							pzi->pszNames[i] = new wchar_t[MAX_PATH_INZIP];
							pzi->info.zip.pfi[i] = new unz_file_info;
							if (pzi->pszNames[i] != NULL && pzi->info.zip.pfi[i] != NULL)
							{
                                                                char zname[MAX_PATH_INZIP];
								zerr = unzGetCurrentFileInfo(pzi->info.zip.uf, pzi->info.zip.pfi[i], zname, MAX_PATH_INZIP,
									NULL, 0, NULL, 0);
								if (zerr == UNZ_OK)
								{
									// Фиксим слеши
									wstrreplace(pzi->pszNames[i], '\\', '/');
									
									// Переводим
									dos2utf16(pzi->pszNames[i], zname);
									
									// Переходим к следующему файлу
									if ((i+1) < pzi->info.zip.gi.number_entry)
									{
										zerr = unzGoToNextFile(pzi->info.zip.uf);
										if (zerr != UNZ_OK) ZipError(zerr, "unzGoToNextFile");
									}
								}
								else ZipError(zerr, "unzGetCurrentFileInfo");
							}
							else ZipError(zerr = UNZ_ERRNO, "no memory for fileinfo");
							
							if (useProgressBar) incprogr(1);
						}
					}
					else ZipError(zerr, "unzGoToFirstFile");
				}
				else ZipError(zerr = UNZ_ERRNO, "no memory");
				
				if (useProgressBar)
				{
					endprogr(); // останавливаем прогресс бар
					Busy = 0;
				}
			}
		}
		else ZipError(zerr, "unzGetGlobalInfo");
	}

	if (progr_stop) zerr = -11111;
	
	return zerr;
}


int OpenZip(ARCHINFO* pzi, wchar_t* zipFileName)
{
  int zerr = UNZ_OK;
  
  unzFile uf = unzOpen(zipFileName);
  if (uf != NULL)
  {
    pzi->info.zip.uf = uf;
    pzi->szCurDir[0] = '\0';
    pzi->type = ZIP_ARCH;
    wstrcpy(pzi->szZipPath, zipFileName);
  }
  else zerr = UNZ_ERRNO;
  
  if (zerr == UNZ_OK)
    zerr = ReadZipInfo(pzi);
  
  if (zerr != UNZ_OK)
    CloseZip(pzi);
  
  return zerr;
}

int OpenTabZip(int tab, wchar_t* zipFileName)
{
  int zerr = UNZ_ERRNO;
  
  if (!IsArchiveOpened(tab)) // не обрабатываем вложенные архивы
    zerr = OpenZip(tabs[tab]->zipInfo, zipFileName);
  return zerr;
}

int FillZipFiles(int tab, wchar_t* subdname)
{
  int num = 0;
  wchar_t buf[MAX_PATH_INZIP];
  wchar_t* pszDirName = subdname[0] == '/' ? subdname + 1 : subdname; // Пропустим в начале обратный слеш
  int dirLen = wstrlen(pszDirName);
  ARCHINFO* pzi = tabs[tab]->zipInfo;
  
  
  if (pzi->info.zip.uf)
  {
    for (int i=0; i < pzi->info.zip.gi.number_entry; i++)
    {
      int ignore = 1;
      wstrncpy(buf, pzi->pszNames[i],MAX_PATH_INZIP-1);
      wchar_t* pFileNameStart = &buf[0];
      unz_file_info* pfi = pzi->info.zip.pfi[i];
      if (dirLen == 0)
      {
        // Если ищем корневые элементы
        // То добавляем все файлы у которых нет слешей '\'
        wchar_t* slashPos = wstrwstr(buf, L"/");
        ignore = (slashPos != NULL);
        if (ignore)
        {
          // И добавляем все директории - у них один слеш, и тот последний
          ignore = (*(slashPos + 1) != 0);
          if (!ignore)
          {
            // Затрем этот слеш, чтобы не выводился
            *slashPos = 0;
          }
        }
      }
      else
      {
        // Сравниваем начало, если равно и потом слеш - то это элементы нашей директории
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
              // проверка на то что это не сама поддиректория а ее элементы, оные проигнорим
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
        AddFile(tab, i, pFileNameStart, pfi->uncompressed_size, pfi->external_fa&ZIP_DIRECTORY?FA_DIRECTORY:0, pfi->dosDate,
                pfi->compressed_size, (pfi->external_fa & ZIP_DIRECTORY) ? TYPE_ZIP_DIR : TYPE_ZIP_FILE);
        num++;
      }
    }
  }
  return num;
}


#define LOCALHEADERMAGIC	(0x04034b50)

int IsItZipFile(wchar_t* fname)
{
  if (CONFIG_ZIP_DETECT_BY == 0) // by content
  {
    unsigned char buf[4];
    uLong header = 0;
    int f =w_fopen(fname, WA_Read, 0x1FF, NULL);
    if (f>=0)
    {
      if (w_fread(f, &buf, 4) == 4)
        header = (uLong)buf[0] + (((uLong)buf[1])<<8) + (((uLong)buf[2])<<16) + (((uLong)buf[3])<<24);
      w_fclose(f);
    }
    return (header == LOCALHEADERMAGIC);
  }
  else // by extension
  {
    return (wstrcmpi(GetFileExt(fname), zip_ext) == 0);
  }
}


int ExtractCurrentFile(ARCHINFO* pzi, int ind, wchar_t* extractDir, int usePaths, int ip)
{
  unzFile uf = pzi->info.zip.uf;
  wchar_t *filePathInZip = pzi->pszNames[ind];
  int res = UNZ_OK;
  int fout = 0;
  wchar_t *extractFilePath=new wchar_t[MAX_PATH];
  wchar_t *temp=NULL;
  char fnbuf[MAX_PATH];
  
  if (filePathInZip == NULL)
  {
    // Считываем имя сами если нужно
    temp=new wchar_t[MAX_PATH];
    unz_file_info fi;
    unzGetCurrentFileInfo(uf, &fi, fnbuf, MAX_PATH, NULL, 0, NULL, 0);
    dos2utf16(temp, fnbuf);
    filePathInZip = temp;
  }
  uInt size_buf = WRITEBUFFERSIZE;
  void* buf = (void*)malloc(size_buf);
  if (buf == NULL)
  {
    ZipError(1, "ExtractCurrentFile.malloc");
    return 1;
  }
  
  wchar_t* fileNameInZip = GetFileName(filePathInZip);
  if ((*fileNameInZip) == '\0')
  {
    if (usePaths)
    {
      // Директория... создаем
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
    
    if (fexists(extractFilePath))
    {
      // ToDo: добавить проверку - если Temp, то переписывать не спрашивая, иначе - диалог
    //  if (!unlink(extractFilePath, &err))
    //    ZipError(res = 2, "ExtractCurrentFile.fsrm");
    }
    if (res == UNZ_OK)
    {
      fout = w_fopen(extractFilePath, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
      
      	// some zipfile don't contain directory alone before file 
      if (fout < 0 && usePaths && fileNameInZip != filePathInZip)
      {
        wchar_t c = *(fileNameInZip - 1);
        *(fileNameInZip - 1) = '\0';
        mktree(filePathInZip);
        *(fileNameInZip - 1) = c;
        fout = w_fopen(extractFilePath, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
      }
      if (fout == -1)
        ZipError(res = 3, "ExtractCurrentFile.fopen");
    }
    if (res == UNZ_OK)
    {
      // Собственно извлекаем
      
      // Откроем файл
      if (unzOpenCurrentFilePassword(uf, pzi->password) == UNZ_OK)
      {
        int isnewprogr = 0;
        if (ip && !progrsp_start)
        {
          if (!progr_start)
          {
            initprogr(ind_msg_zreading);
            progr_max = 1;
            incprogr(0);
            isnewprogr = 1;
          }
          progrsp_start = 1;
          progrsp_max = pzi->info.zip.pfi[ind]->uncompressed_size;
          incprogrsp(0);
        }
        int cb = 0;
        do
        {
          if (ip && progr_stop)
          {
            res = UNZ_ERRNO;
            break;
          }
          cb = (int)unzReadCurrentFile(uf, buf, size_buf);
          if (cb < 0)
          {
            ZipError(cb, "ExtractCurrentFile.unzReadCurrentFile");
            res = 5;
            break;
          }
          else if (cb > 0)
          {
            if (w_fwrite(fout, buf, cb) != cb)
            {
              ZipError(res = 6, "ExtractCurrentFile.fwrite");
              break;
            }
            if (ip && progrsp_start) incprogrsp(cb);
          }
        }
        while (cb > 0);
        if (ip && progrsp_start)
        {
          endprogrsp();
          progrsp_start = 0;        
          if (isnewprogr)
          {
            endprogr();
            progr_start = 0;
          }
        }
        if (fout) { w_fclose(fout); fout = 0; }
        if (res == UNZ_OK)    
        {
          
					// Выставляем дату время извлеченному файлу
					// change_file_date(write_filename, file_info.dosDate, file_info.tmu_date);
        }
      }
      else ZipError(res = 7, "ExtractCurrentFile.unzOpenCurrentFile");
      if (fout) w_fclose(fout);
    }
    if (res == UNZ_OK)
    {
      if (unzCloseCurrentFile(uf) != UNZ_OK)
        ZipError(res = 8, "ExtractCurrentFile.unzCloseCurrentFile");
    }
    else unzCloseCurrentFile(uf); // don't lose the error 
  }
  mfree(buf);
  delete extractFilePath;
  if (temp) delete temp;
  return res;
}

int SetCurrentFileInZip(ARCHINFO* pzi, int id)
{
  int zerr = unzGoToFirstFile(pzi->info.zip.uf);
  if (zerr != UNZ_OK)
    return 1;
  
  for (int i = 1; i <= id; i++)
  {
    zerr = unzGoToNextFile(pzi->info.zip.uf);
    if (zerr != UNZ_OK)
      return i + 1;
  }
  return 0;	
}

int ExtractFileByID(ARCHINFO* pzi, int id, wchar_t* extractDir, int usePaths, int ip)
{
  // Позиционируемся на нужном файле
  if (SetCurrentFileInZip(pzi, id) == UNZ_OK)
  {
    // И извлекаем его
    return ExtractCurrentFile(pzi, id, extractDir, usePaths, ip);
  }
  else return 1;
}

int ExtractFile(ARCHINFO* pzi, wchar_t* fname, wchar_t* extractDir, int usePaths)
{
  // Позиционируемся на нужном файле
  int found = 0;
  wchar_t buf[MAX_PATH + 1];
  wstrcpy(buf + 1, fname);
  buf[0] = '/'; // В начало запишем слеш на тот случай если fname без слеша
  
  wchar_t* nameWithoutSlash = ( (buf[1] == '/') ? (buf + 2) : (buf + 1) );
  wchar_t* nameWithSlash = ( (buf[1] == '/') ? (buf + 1) : (buf) );
  
  for (int i = 0; i < pzi->info.zip.gi.number_entry; i++)
  {
    found = (wstrcmpi(nameWithoutSlash, pzi->pszNames[i]) == 0);
    if (!found)
    {
      // Пробуем поискать со слешем
      found = (wstrcmpi(nameWithSlash, pzi->pszNames[i]) == 0);
    }
    if (found)
    {
      // Извлекаем его, если нашли
      return ExtractFileByID(pzi, i, extractDir, usePaths, 0);
    }
  }
  return 1;
}

int ExtractDir(ARCHINFO* pzi, wchar_t* dname, wchar_t* extractDir, int usePaths)
{
  int found = 0;
  wchar_t *buf=new wchar_t[MAX_PATH + 1];
  wstrcpy(buf + 1, dname);
  buf[0] = '/'; // В начало запишем слеш на тот случай если fname без слеша
  wchar_t* nameWithoutSlash = ( (buf[1] == '/') ? (buf + 2) : (buf + 1) );
  wchar_t* nameWithSlash = ( (buf[1] == '/') ? (buf + 1) : (buf) );
  
  int zerr = UNZ_OK;
  int dlen = wstrlen(nameWithoutSlash);
  
  // Перебираем все файлы
  for (int i = 0; i < pzi->info.zip.gi.number_entry; i++)
  {
    if (i == 0) zerr = unzGoToFirstFile(pzi->info.zip.uf);
    else zerr = unzGoToNextFile(pzi->info.zip.uf);
    
    if (zerr) return zerr;
    
    // Проверяем начинается ли имя файла в зипе с этой директории, если да - извлекаем
    found = (wstrcmpni(nameWithoutSlash, pzi->pszNames[i], dlen) == 0);
    if (!found)
    {
      // Пробуем поискать со слешем
      found = (wstrcmpni(nameWithSlash, pzi->pszNames[i], dlen + 1) == 0);
    }
    if (found)
      ExtractCurrentFile(pzi, i, extractDir, 1, 0);
  }
  delete buf;
  return zerr;
}

// Кэш на один :) зип файл


int ZipBufferExtract(ARCHINFO* pzi, FN_ITM* pi, wchar_t* extractDir)
{
  if (pi && pi->zipPath && pi->full)
  {
    int usingZipFromTab = 0;
    
    for (int i = 0; i < MAX_TABS; i++)
    {
      if (IsArchiveOpened(i)==ZIP_ARCH && wstrcmpi(tabs[i]->zipInfo->szZipPath, pi->zipPath) == 0)
      {
        // Если уже открыт в табе, то используем его
        pzi = tabs[i]->zipInfo;
        usingZipFromTab = 1;
        break;
      }
    }
    if (!usingZipFromTab)
    {
      // Если файл уже открыт, но имена не совпадают, то закрываем старый
      if (pzi->info.zip.uf != 0 && wstrcmpi(pzi->szZipPath, pi->zipPath) != 0)
        CloseZip(pzi);
      
      // Если файл еще не открыт - то открываем
      if (pzi->info.zip.uf == 0)
        if (OpenZip(pzi, pi->zipPath) != UNZ_OK)
          return UNZ_ERRNO;
    }
    
    if (pi->ftype == TYPE_ZIP_FILE)
      return ExtractFile(pzi, pi->full, extractDir, 0);
    else if (pi->ftype == TYPE_ZIP_DIR)
      return ExtractDir(pzi, pi->full, extractDir, 1);
  }
  return 0;
}

void ZipBufferExtractEnd(ARCHINFO* pzi)
{
  if (pzi->type == ZIP_ARCH)
    CloseZip(pzi);
}
