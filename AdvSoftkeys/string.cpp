#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h"
 
int pos2bs(int n)
{
  int res=n;
  if (n<=512)
  {
    res=512;
  }
  else if ((n%512)>0)
  {
    res=n-(n%512)+512;
  }
  return res;
};

int oct2int(const char *oct)
{
  int ch;
  int value = 0;
  while (((ch = *oct++) >= '0') &&
         (ch <= '7'))
  {
    value <<= 3;
    value += (ch - '0');
  }
  return value;
};

void abc2ABC(char *ws)
{
  while (*ws) 
  {
    if (*ws>0x39)
    {
      *ws&=~0x20; 
    }
    *ws++;
  }
};

int h2i(char * h)
{
  char c;
  int res=0;
  while((*h++)!='X');
  do
  {
    c=*h++;
    if (c>0x60) c-=0x20; else c-=0x30;
    if (c>9) c-=7;
    res<<=4;
    res|=c;
  }
  while(*h!='\0');
  return(res);
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
  
  for (cursor = haystack; cursor <= last_possible_needle_location; cursor++) 
  {
    if (memcmp(needle, cursor, needlelen) == 0) 
    {
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
