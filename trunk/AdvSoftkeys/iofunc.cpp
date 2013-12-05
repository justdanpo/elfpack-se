#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#define COPYSIZE 500

char *file_read(wchar_t *path, wchar_t *name, int *len)
{
  int file;
  FSTAT fst;
  if (fstat(path,name,&fst)==0)
  {
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
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

void copyfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2)
{
  FSTAT fst;
  int file;
  if (fstat(path,name,&fst)==0)
  {
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      int sfile;
      if ((sfile=_fopen(path2,name2,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        int size=fst.fsize;
        int x=0;
        for (x=0;x<size;x=x+COPYSIZE)
        {
          int ts=COPYSIZE;
          if ((size-x)>=COPYSIZE)
          {
            ts=COPYSIZE;
          }
          else if ((size-x)<COPYSIZE)
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
    }
    fclose(file);
  }
};

#define CMPSIZE 500
int cmpfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2)
{
  int res=0;
  FSTAT fst;
  int file;
  if (fstat(path,name,&fst)==0)
  {
    FSTAT fst2;
    if (fstat(path2,name2,&fst2)==0)
    {
      if (fst.fsize!=fst2.fsize)
      {
        res=0;
      }
      else
      {
        if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
        {
          int sfile;
          if ((sfile=_fopen(path2,name2,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
          {
            int size=fst.fsize;
            int x=0;
            for (x=0;x<size;x=x+CMPSIZE)
            {
              int ts=CMPSIZE;
              if ((size-x)>=CMPSIZE)
              {
                ts=CMPSIZE;
              }
              else if ((size-x)<CMPSIZE)
              {
                ts=size-x;
              }
              char *buf=new char[ts+1];
              fread(file, buf, ts);
              char *buf2=new char[ts+1];
              fread(sfile,buf2,ts);
              if (memcmp(buf,buf2,ts)!=0)
              {
                res=0;
                break;
              }
              else
              {
                res=1;
              }
              delete(buf2);
              delete(buf);
            }
          }
          fclose(sfile);
        }
        fclose(file);
      }
    }
  }
  return res;
};
