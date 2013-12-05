#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\sort.h"


typedef struct {
	wchar_t *name;
	wchar_t *ext;
	FILEINF *dataPointer;
} FileSortInfo;

wchar_t* fn_prepare(wchar_t* src, wchar_t* dst)
{
  int c;
  wchar_t *d=dst;
  while((c=*src++))
  {
    *d++=wcharlow(c);    
  }
  *d=0;
  return (dst);
}

// Сравнение имен файлов, если различие только в последних символах и они являются цифрами,
// то цифры обрабатываются как числа
int filenamecmp(FileSortInfo* fs1, FileSortInfo* fs2)
{
  wchar_t *str1 = fs1->name;
  wchar_t *str2 = fs2->name;
  wchar_t c1 = 0;
  wchar_t c2 = 0;
  
  while (1)
  {
    c1 = *str1;
    c2 = *str2;
    
    if (c1 != c2) break;
    if (c1 == 0 && c2 == 0) return 0;
    str1++; str2++;
  }
  
  // Если попали сюда, то строки не равны
  if (wstrcmpi(fs1->ext, fs2->ext) == 0 && isNumericWstr(str1) && isNumericWstr(str2))
  {
    // Если расширения одинаковые и неравные подстроки - цифры...
    
    // если длина меньше, то заведомо и число меньше
    // если одинаковая, то идем дальше, т.к. достаточно сравнить только найденные отличающиеся цифры
    int ls1 = wstrlen(str1);
    int ls2 = wstrlen(str2);
    if (ls1 != ls2) return (ls1 - ls2);
  }
  return (c1 - c2);
}

int isNameGreater(void* a, void* b)
{
  FileSortInfo* fa = (FileSortInfo*)a;
  FileSortInfo* fb = (FileSortInfo*)b;
  int res = filenamecmp(fa, fb);
  if (res == 0)
    res = wstrcmpi(fa->ext, fb->ext); // если имена равны то сравниваем расширения
  return res > 0;
}

int isExtGreater(void* a, void* b)
{
  FileSortInfo* fa = (FileSortInfo*)a;
  FileSortInfo* fb = (FileSortInfo*)b;
  int res = wstrcmpi(fa->ext, fb->ext);
  if (res == 0)
    res = filenamecmp(fa, fb); // если расширения равны то сравниваем имена
  return  res > 0;
}

int isSizeGreater(void* a, void* b)
{
  FILEINF* fa = (FILEINF*)a;
  FILEINF* fb = (FILEINF*)b;
  int res = fa->size - fb->size;
  if (res == 0)
    return wstrcmpi(fa->ws_name, fb->ws_name) > 0;
  else
    return res > 0;
}

int isDateGreater(void* a, void* b)
{
  FILEINF* fa = (FILEINF*)a;
  FILEINF* fb = (FILEINF*)b;
  int res = fa->time - fb->time;
  if (res == 0)
    return wstrcmpi(fa->ws_name, fb->ws_name) > 0;
  else
    return res > 0;
}

FILEINF* SortList(FILEINF* list, int sort)
{
  FILEINF* base = list;
  int i;
  int err = 0;
  
  int sortType = sort & STV_MASK;
  int isDirectSort = sort & ST_REVERS;
  
  // Считаем кол-во элементов
  int count = 0;
  FILEINF* cur = base;
  while (cur)
  {
    count++;
    cur = (FILEINF *)cur->next;
  }
  
  // Не сортируем если нет смысла
  if (count < 2) return list;
  
  if (sortType == ST_NAME || sortType == ST_EXT) // Косвенная сортировка по строкам
  {
    char buf[MAX_PATH];
    FileSortInfo** sortInfo = new FileSortInfo*[count];
    
    if (buf && sortInfo)
    {
      // Готовим массив
      i = 0;
      cur = base;
      while (cur)
      {
        sortInfo[i] = new FileSortInfo;
        
        if (!sortInfo[i])
        {
          err = 1; break;
        }
        sortInfo[i]->dataPointer = cur;
        sortInfo[i]->name = new wchar_t[MAX_PATH];
        if (!sortInfo[i]->name)
        {
          err = 1; break;
        }
        fn_prepare(cur->ws_name, sortInfo[i]->name);
        //WriteLog(sortInfo[i]->name);
        // Разбиваем на имя файла и расширение
        sortInfo[i]->ext = GetFileExt(sortInfo[i]->name);
        if (!sortInfo[i]->ext)
          sortInfo[i]->ext = (wchar_t *)str_empty;
        else
          *(sortInfo[i]->ext-1) = 0; // вместо точки ставим конец строки
        
        cur = (FILEINF *)cur->next;
        i++;
      }
      if (!err)
      {
        // Сортируем
        if (sortType == ST_NAME)
          quickSort((void**)sortInfo, count, isNameGreater);
        else if (sortType == ST_EXT)
          quickSort((void**)sortInfo, count, isExtGreater);
        
        // Переинициируем список и освободим память
        if (isDirectSort)
        {
          base = sortInfo[0]->dataPointer;
          for (i = 0; i < count - 1; i++)
            sortInfo[i]->dataPointer->next = sortInfo[i + 1]->dataPointer;
          sortInfo[count - 1]->dataPointer->next = 0;
        }
        else
        {
          base = sortInfo[count - 1]->dataPointer;
          for (i = count - 1; i > 0; i--)
            sortInfo[i]->dataPointer->next = sortInfo[i - 1]->dataPointer;
          sortInfo[0]->dataPointer->next = 0;
        }
      }
    }
    else
    {
      MsgBoxError(muitxt(ind_err_nomemory));
    }
    if (sortInfo)
    {
      for (i = count - 1; i >= 0; i--)
      {
        if (sortInfo[i])
        {
          if (sortInfo[i]->name)
            delete(sortInfo[i]->name);
          delete(sortInfo[i]);
        }
      }
      delete(sortInfo);
    }
  }
  else // Прямая сортировка по числам
  {
    // Готовим массив
    FILEINF** sortInfo = new FILEINF*[count];
    if (sortInfo)
    {
      i = 0;
      cur = base;
      while (cur)
      {
        sortInfo[i] = cur;
        cur = (FILEINF *)cur->next;
        i++;
      }
      // Сортируем
      if (sortType == ST_DATE)
        quickSort((void**)sortInfo, count, isDateGreater);
      else if (sortType == ST_SIZE)
        quickSort((void**)sortInfo, count, isSizeGreater);
      
      // Переинициируем список и освободим память
      if (isDirectSort)
      {
        base = sortInfo[0];
        for (i = 0; i < count - 1; i++)
          sortInfo[i]->next = sortInfo[i + 1];
        sortInfo[count - 1]->next = 0;
      }
      else
      {
        base = sortInfo[count - 1];
        for (i = count - 1; i > 0; i--)
          sortInfo[i]->next = sortInfo[i - 1];
        sortInfo[0]->next = 0;
      }
      delete(sortInfo);
    }
    else
    {
      MsgBoxError(muitxt(ind_err_nomemory));
    }
  }
  return base;
}



void SortFiles(int tab)
{
  if (tabs[tab]->ccFiles>1)
  {
    FILEINF *files=0;
    FILEINF *dirs=0;
    
    FILEINF *cur = (FILEINF *)(FileListBase[tab]->next);
    FILEINF *next;
    while(cur != FileListBase[tab])
    {
      next = (FILEINF *)cur->next;
      if (cur->attr & FA_DIRECTORY)
      {
        cur->next = dirs;
        dirs = cur;
      }
      else
      {
        cur->next = files;
        files = cur;
      }
      cur = next;
    }
    FileListBase[tab]->next = FileListBase[tab];

    int srt = tabs[tab]->sort;
    
    if (srt & STV_MASK == ST_SIZE)
      dirs = SortList(dirs,  srt & STD_MASK); // При сортировке по размеру папки сортируем по имени
    else
      dirs = SortList(dirs,  srt);
    
    files = SortList(files, srt);
    
    while(files)
    {
      next = (FILEINF *)files->next;
      files->next = FileListBase[tab]->next;
      FileListBase[tab]->next = files;
      files = next;
    }
    while(dirs)
    {
      next = (FILEINF *)dirs->next;
      dirs->next = FileListBase[tab]->next;
      FileListBase[tab]->next = dirs;
      dirs = next;
    }
  }
}
