#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
void *memmem(const unsigned char *haystack, size_t haystacklen, const void *needle, size_t needlelen);

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

char *getparam2(char *buf, char *mask, char end, int fsize)
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
      ret[len-1]='\0';
      return ret;
    }
    return 0;
  }
  return 0;
};
