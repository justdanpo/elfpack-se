#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\untar.h"
#include "header\string.h"
#define F_SAVESIZE 300

//Рабочий модуль для распаковки *.thm...
//может работать с некоторыми архивами некорректно, т.к. особо логику *.tar не разбирал...
//(C) UltraShot

TAR_HEADER *tar_getnextheader(int file)
{
  TAR_HEADER *h=new TAR_HEADER;
  memset(h,0,sizeof(TAR_HEADER));
  fread(file,h->fname,0x64);
  lseek(file,0x18,SEEK_CUR);
  char size[13];
  fread(file,size,12);
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
    for (x=0;x<size;x=x+F_SAVESIZE)
    {
      int ts=F_SAVESIZE;
      if ((size-x)>=F_SAVESIZE)
      {
        ts=F_SAVESIZE;
      }
      else if ((size-x)<F_SAVESIZE)
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

int tar_uncompress(wchar_t *path, wchar_t *name, wchar_t *folder)
{
  int res=0;
  int file;
  FSTAT fst;
  if (fstat(path,name,&fst)==0)
  {
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
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
          else break;
        }
        else break;
      }
    }
    fclose(file);
  }
  return res;
};
