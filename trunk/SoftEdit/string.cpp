#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"
#define F_READ 0x001
#define F_RIGHTS 0x180
#define F_WRITE 0x204

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
  if ((en-start)<=0)
  {
    return 0;
  }
  ret = new char[en-start+1];
  memcpy(ret, &line[start], en-start);
  ret[en-start]='\0';
  return ret;
};

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
  if ((en-start)<=0)
  {
    return 0;
  }
  int len=en-start;
  wchar_t *wret=new wchar_t[len/2+1];
  memcpy(wret,&line[start], len);
  wret[len/2]=L'\0';
  return wret;
};

void *memmem(const unsigned char *haystack, size_t haystacklen, const void *needle, size_t needlelen)
{
  const unsigned char *cursor;
  const unsigned char *last_possible_needle_location = (unsigned char *)haystack + haystacklen - needlelen;
  if (needlelen > haystacklen) return(NULL);
  if (needle == NULL) return(NULL);
  if (haystack == NULL) return(NULL);
  if (needlelen == 0) return(NULL);
  if (haystacklen == 0) return(NULL);
  
  for (cursor = haystack; cursor <= last_possible_needle_location; cursor++) {
    if (memcmp(needle, cursor, needlelen) == 0) {
      return((void *) cursor);
    }
  }
  return(NULL);
};

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
    ret=new char[len+1];
    memcpy(ret,str,len);
    ret[len]='\0';
    return ret;
  }
  return 0;
};

char *file_read(wchar_t *path, wchar_t *name, int *len)
{
  int file;
  FSTAT fst;
  if (fstat(path,name,&fst)==0)
  {
    if ((file=_fopen(path,name,F_READ,F_RIGHTS,0))>=0)
    {
      char *buf=0;
      buf=new char[fst.fsize+1];
      fread(file,buf,fst.fsize);
      fclose(file);
      if (len)
      {
        len[0]=fst.fsize;
      }
      return buf;
    }
    fclose(file);
  }
  return 0;
};
