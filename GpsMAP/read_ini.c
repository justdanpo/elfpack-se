#define USEPNG
#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "main.h"
#include "map_object.h"
#include "read_csv.h"
#include "read_ini.h"

extern "C" float strtof(const char *, char **);
extern "C" long strtol(const char *, char **, int);

void MainIniInit(MAIN_INI *ini)
{
  ini->towns=List_Create();
  ini->cur_town=NULL; 
  ReadMainIni(ini);
}

void MainIniDeInit(MAIN_INI *ini)
{
  TOWN *town;
  while(ini->towns->FirstFree)
  {
    town=(TOWN *)List_RemoveAt(ini->towns,0);
    DestroyCsvList(town);
    List_Destroy(town->bs_dat);
    delete town->name;
  }
  List_Destroy(ini->towns);
}

char *find_eol(char *s)
{
  int c;
  s--;
  do
  {
    s++;
    c=*s;
  }
  while((c)&&(c!='\r')&&(c!='\n'));
  return s;
}

float str_tof(char *str, char **end)
{
  float x=0;
  int len;
  int c;
  char *s, tmp[64];
  for (s=tmp; (c=*str)!='°'; str++)
  {
    if ((c>='0' && c<='9') || c=='.') *s++=c;
    if (!c) break;
  }
  *s=0;
  if (s!=tmp) x+=strtof(tmp,NULL);
  for (s=tmp; (c=*str)!='\''; str++)
  {
    if ((c>='0' && c<='9') || c=='.') *s++=c;
    if (!c) break;
  }
  *s=0;
  if (s!=tmp) x+=(strtof(tmp,NULL)/60);  
  for (s=tmp; (c=*str)!='\"'; str++)
  {
    if ((c>='0' && c<='9') || c=='.') *s++=c;
    if (!c) break;
  }
  *s=0;
  if (s!=tmp) x+=(strtof(tmp,NULL)/3600);  
  return x;
}

char *skip_probel(char *s)
{
  int c;
  s--;
  do
  {
    s++;
    c=*s;
  }
  while((c)&&((c==' ')||(c=='\t')));
  return s; 
}

void ReadMainIni(MAIN_INI *ini)
{
  char *buf, *s,*d;
  FSTAT stat;
  int fsize;
  int c;
  char tmp[32];
  int f;
  int len;
  int pindex, to_skip;
  TOWN *town=NULL;
  if (fstat(GetDir(DIR_ELFS_CONFIG),L"GpsMAP.ini",&stat)<0) return;
  if ((fsize=stat.fsize)>0)
  {
    if ((f=_fopen(GetDir(DIR_ELFS_CONFIG),L"GpsMAP.ini",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,NULL))>=0)
    {
      buf=new char[fsize+1];
      if ((s=buf))
      {
        buf[fread(f,buf,fsize)]=0;
        while((c=*s))
        {
          if (c=='\r' || c=='\n')  // Конец строки
          {
            s++;
            continue;
          }
          if (c==';') // Комментарий
          {
            s=find_eol(s);
            continue;
          }
          if (c=='[')
          {
            s++;
            if (strncmp(s,"Town ",5)==0)
            {
              s+=5;
              if (town) List_InsertFirst(ini->towns,town);
              town=new TOWN;
              memset(town,0,sizeof(TOWN));
              town->bs_dat=List_Create();
              town->map_index=strtol(s,NULL,10);
              s=find_eol(s);
              continue;
            }
          }
          s=skip_probel(s);
          if (strncmp(s,"Name",4)==0) {pindex=1; to_skip=4;}
          else if (strncmp(s,"MapStartX",9)==0) {pindex=2; to_skip=9;}
          else if (strncmp(s,"MapStartY",9)==0) {pindex=3; to_skip=9;}
          else if (strncmp(s,"MapEndX",7)==0) {pindex=4; to_skip=7;}
          else if (strncmp(s,"MapEndY",7)==0) {pindex=5; to_skip=7;}
          else
          {
            s=find_eol(s);
            continue;
          }
          s+=to_skip;
          s=skip_probel(s);
          if (*s=='=')
          {
            s++;
            s=skip_probel(s);
            d=find_eol(s);
            if ((len=d-s))
            {
              if (len>31) len=31;
              strncpy(tmp,s,len);
              tmp[len]=0;
              while(tmp[len-1]==' ' || tmp[len-1]=='\t') tmp[--len]=0;
            }
            if (town && len)
            {
              switch(pindex)
              {
              case 1:
                town->name=new char[len+1];
                strncpy(town->name,tmp,len);
                town->name[len]=0;
                break;
              case 2:
                town->start_x=str_tof(tmp,NULL);
                break;
              case 3:
                town->start_y=str_tof(tmp,NULL);
                break;
              case 4:
                town->end_x=str_tof(tmp,NULL);
                break;
              case 5:
                town->end_y=str_tof(tmp,NULL);
                break;               
              }
            }
          }
          s=find_eol(s);
        }
        if (town) List_InsertFirst(ini->towns,town);
        delete buf;
      }
      fclose(f);
    }
  }
}
