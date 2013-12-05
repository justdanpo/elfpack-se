#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "untar.h"

int oct2int(const char *oct)
{
  int ch;
  unsigned value = 0;
  
  while (((ch = *oct++) >= '0') &&
         (ch <= '7') || (ch == ' '))
  {
    if (ch==' ')ch='0';
    value <<= 3;
    value += (ch - '0');
  }
  
  return value;
};
 
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

TAR_HEADER *tar_getnextheader(int file)
{
  TAR_HEADER *h=new TAR_HEADER;
  memset(h,0,sizeof(TAR_HEADER));
  fread(file,h->fname,0x64);
  lseek(file,0x18,SEEK_CUR);
  char size[13];
  fread(file,size,12);
  size[12]='\0';
  h->size=oct2int(size);
  if (h->size>0)
  {
    lseek(file,HEADER_SIZE-0x64-0x18-12, SEEK_CUR);
  }
  else
  {
    delete(h);
    return 0;
  }
  return h;
};

int tar_savefile(int file, int size, wchar_t *path, wchar_t *fname)
{
  int sfile;
  if ((sfile=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    int x=0;
    for (x=0;x<size;x=x+30)
    {
      int ts=30;
      if ((size-x)>=30)
      {
        ts=30;
      }
      else if ((size-x)<30)
      {
        ts=size-x;
      }
      char *buf=new char[ts+1];
      fread(file, buf, ts);
      fwrite(sfile, buf, ts);
      delete(buf);
    }
  }
  fclose(sfile);
  return 0;
};

int tar_uncompress(wchar_t *path, wchar_t *name)
{
  int res=0;
  int file;
  FSTAT fst;
  if (fstat(path,name,&fst)==0)
  {
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      wchar_t folder[512];
      wstrcpy(folder,GetDir(MEM_EXTERNAL+DIR_THEME));
      wchar_t *nname= new wchar_t[wstrlen(name)-4+2];
      wstrncpy(nname,name,wstrlen(name)-4);
      nname[wstrlen(name)-4]=L'\0';
      wstrcat(folder,L"/");
      wstrcat(folder,nname);
      wstrcat(folder,L"/");
      delete(nname);
      int pos=0;
      while (pos<fst.fsize)
      {
        lseek(file,pos,SEEK_SET);
        TAR_HEADER *h=0;
        if (pos<fst.fsize)
        {
          h=tar_getnextheader(file);
          pos=pos+0x200;
          if (h)
          {
             wchar_t fname[0x64+1];
             str2wstr(fname, h->fname);
             tar_savefile(file, h->size, folder, fname);
             pos=pos+h->size;
             pos = pos2bs(pos);
             delete(h);
            res++;
          }
          else
          {
            pos = pos2bs(pos);
          }
        }
        else
        {
          break;
        }
      }
    }
    fclose(file);
  }
  return res;
};
