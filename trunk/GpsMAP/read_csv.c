#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "read_ini.h"
#include "read_csv.h"


extern "C" long strtol(const char *, char **, int);

enum CSV_STATE {CSV_CI, CSV_LAC, CSV_POINT_X, CSV_POINT_Y, CSV_SKIP};

int ReadCsvFile(TOWN *town)
{
  wchar_t dir[64];
  FSTAT stat;
  int f;
  char *buf,*s;
  int c;
  char tmp[64],*d;
  int state, created_elem=0;
  int ci, lac;
  float point_x, point_y;
  snwprintf(dir,63,L"%ls/GpsMAP/%s",GetDir(DIR_ELFS),town->name);
  if (fstat(dir,L"bs_dat.csv",&stat)>=0)
  {
    if ((f=_fopen(dir,L"bs_dat.csv",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,NULL))>=0)
    {
      int fsize=stat.fsize;
      s=buf=new char[fsize+1];
      buf[fread(f,buf,fsize)]=0;
      fclose(f);
      state=CSV_CI;
      d=tmp;
      for(;;)
      {
        c=*s;
        if ((d-tmp)<63) *d++=c;
        if (c==';' || c=='\r' || c=='\n' || !c)
        {
          *(d-1)=0;
          switch(state)
          {
          case CSV_CI:
            ci=strtol(tmp,0,10);
            state=CSV_LAC;
            break;
          case CSV_LAC:
            lac=strtol(tmp,0,10);
            state=CSV_POINT_X;
            break;
          case CSV_POINT_X:
            point_x=str_tof(tmp,0);
            state=CSV_POINT_Y;
            break;
          case CSV_POINT_Y:
            point_y=str_tof(tmp,0);
            state=CSV_SKIP;
            break;
          }
          if (c=='\r' || c=='\n' || !c)
          {
            if (state==CSV_SKIP)
            {
              BS_POINT *bs=new BS_POINT;
              bs->ci=ci;
              bs->lac=lac;
              bs->point_x=point_x;
              bs->point_y=point_y;
              created_elem++;
              List_InsertLast(town->bs_dat,bs);
            }
            if (!c) break;
            state=CSV_CI;
          }
          d=tmp;
        }
        s++;
      }
      delete buf;
    }
  }
  return created_elem;
}

void DestroyCsvList(TOWN *town)
{
  while(town->bs_dat->FirstFree)
  {
    BS_POINT *bs=(BS_POINT *)List_RemoveAt(town->bs_dat,0);
    delete bs;    
  }
}



int FindBSByCiLac(void *what, void *where)
{
  BS_POINT *bs=(BS_POINT *)what;
  int cid=((int *)where)[0];
  int lac=((int *)where)[1];
  return (cid/10==bs->ci && lac==bs->lac)?0:1;
}

int FindTownById(void *what, void *where)
{
  return ((int)where==((TOWN *)what)->map_index)?0:1;  
}

TOWN *FindTownByCiLac(MAIN_INI *ini, int ci, int lac)
{
  for (int t=0; t<ini->towns->FirstFree; t++)
  {
    TOWN *town=(TOWN *)List_Get(ini->towns,t);
    if (ReadCsvFile(town))
    {
      for (int b=0; b<town->bs_dat->FirstFree; b++)
      {
        BS_POINT *bs=(BS_POINT *)List_Get(town->bs_dat,b);
        if (bs->ci==ci/10 && bs->lac==lac)
        {
          if (bs->point_x>=town->start_x && bs->point_x<=town->end_x &&
              bs->point_y>=town->start_y && bs->point_y<=town->end_y)  return (town);
        }
      }
      DestroyCsvList(town);  // Если не то то выкидываем на хуй
    }
  }
  return (NULL);
}
