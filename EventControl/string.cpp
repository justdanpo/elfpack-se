/*
****************************************************************************
*              Модуль, содержащий функции парсинга строк
****************************************************************************
*/
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 

/*
*===========================================================================
*           Получение знакового числа из строки до ограничителя
*===========================================================================
*/
int getint(char *line, int *pos,char end)
{
  bool minus=false;
  if (line[pos[0]]=='-')
  {
    minus=true;
    pos[0]++;
  }
  int ret=0;
  while (line[pos[0]]!=end)
  {
    ret=(ret*10)+(line[pos[0]]-0x30);
    pos[0]++;
  }
  pos[0]++;
  if (minus)ret=-ret;
  return ret;
};

/*
*===========================================================================
*            Получение ansi-строки из другой строки до ограничителя
*===========================================================================
*/
char *getchr(char *line, int *pos, char end)
{
  char *ret;
  int start,en;
  start=pos[0];
  while (line[pos[0]]!=end && line[pos[0]]!=0)
  {
    pos[0]++;
  }
  en=pos[0];
  pos[0]++;
  int len=en-start;
  if (len>0)
  {  
    ret = new char[len+1];
    memcpy(ret, &line[start], len);
    ret[len]='\0';
    return ret;
  }
  return 0;
};

/*
*===========================================================================
*        Получение юникодной строки из другой строки до ограничителя
*===========================================================================
*/
wchar_t *getwchr(char *line, int *pos, char end)
{
  int start,en;
  start=pos[0];
  while (line[pos[0]]!=end)
  {
    pos[0]++;
  }
  en=pos[0];
  pos[0]++;
  int len=en-start;
  if (len>0)
  {
    wchar_t *wret=new wchar_t[len/2+1];
    memcpy(wret,&line[start], len);
    wret[len/2]=L'\0';
    return wret;
  }
  return 0;
};

/*
*===========================================================================
*               Находим первое вхождение needle в haystack
*===========================================================================
*/
void *memmem(const unsigned char *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
  const unsigned char *cursor;
  const unsigned char *last_possible_needle_location = (unsigned char *)haystack + haystacklen - needlelen;
  if (needlelen > haystacklen) return(NULL);
  if (needle == NULL) return(NULL);
  if (haystack == NULL) return(NULL);
  if (needlelen == 0) return(NULL);
  if (haystacklen == 0) return(NULL);
  
  for (cursor = haystack; cursor <= last_possible_needle_location; cursor++) 
  {
    if (memcmp(needle, cursor, needlelen) == 0) 
    {
      return((void *) cursor);
    }
  }
  return(NULL);
};

/*
*===========================================================================
*          Получение параметра из буфера "смешанного" содержания...
*===========================================================================
* 1) Отличие от библиотечной в том, что строка тут обрезается до 
* специального ограничителя, а в библиотечной - до нуль символа или конца строки
*/
char *getparam(char *buf, char *mask, char end, int fsize)
{
  char *str=0;
  if (str=(char*)memmem((unsigned char const*)buf,fsize,mask,strlen(mask)))
  {
    char *ret=0;;
    int addr=(int)str;
    str=(char*)(addr+strlen(mask)+2);
    int x=0;
    int len=0;
    while (str[x]!=end)
    {
      x++;
    }
    len=x+1;
    if (len>1)
    {
      ret=new char[len+1];
      memcpy(ret,str,len);
      ret[len]='\0';
      return ret;
    }
    return 0;
  }
  return 0;
};

