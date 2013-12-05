#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\extern.h"
#include "header\string.h"
LIST *customsofts=0;
int set_ui(char *buf, int fsize);

extern UI_SOFTKEY sk[3];

void addline(char *line)
{
  char between=0xFF;
  ITEM *it=new ITEM;
  memset(it,0,sizeof(ITEM));
  int x=0;
  it->name=getwchr(line,&x,between);
  it->lsi=getwchr(line,&x,between);
  it->msi=getwchr(line,&x,between);
  it->rsi=getwchr(line,&x,between);
  it->style=getint(line,&x,':');
  int howmany=getint(line,&x,':');
  if (howmany>0)
  {
    int j;
    for (j=0;j<howmany;j++)
    {
      if (line[x]==';')break;
      KEY *key=new KEY;
      key->oldkey=getint(line,&x,'-');
      key->newkey=getint(line,&x,':');
      if (!it->keys)it->keys=List_Create();
      List_InsertLast(it->keys, key);
    }
  }
  else
  {
    it->keys=List_Create();
  }
  if (!customsofts)customsofts=List_Create();
  List_InsertLast(customsofts, it);
};

void destroylists()
{
  LIST *csofts=customsofts;
  if (csofts)
  {
    while (csofts->FirstFree)
    {
      ITEM *it=(ITEM*)List_RemoveAt(csofts,0);
      DELETE(it->name);
      DELETE(it->lsi);
      DELETE(it->msi);
      DELETE(it->rsi);
      if (it->keys)
      {
        while (it->keys->FirstFree)
        {
          KEY *k=(KEY*)List_RemoveAt(it->keys,0);
          DELETE(k);
        }
        List_Destroy(it->keys);
        it->keys=0;
      }
      DELETE(it);
    }
    List_Destroy(csofts);
    csofts=0;
  }
};

int readcustomcfg(wchar_t *path, wchar_t *name)
{
  int fsize=0;
  char *buf=file_read(path, name, &fsize);
  if (buf)
  {
    char *param=0;
    int x=0;
    if (!customsofts)customsofts=List_Create();
    while (1)
    {
      char pattern[128];
      sprintf(pattern,"%d)",x);
      if (param=getparam(buf,pattern,0xFE, fsize))
      {
        addline(param);
        delete(param);
      }
      else
      {
        break;
      }
      x++;
    }
    set_ui(buf,fsize);
    delete(buf);
    return 0;
  }
  return -1;
};

void savecustomcfg(wchar_t *path, wchar_t *name)
{
  int file;
  char cr=0x0D;
  char lf=0x0A;
  char endstr=0xFE;
  char between=0xFF;
  if ((file=_fopen(path,name,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    int x=0;
    if (customsofts)
    {
      for (x=0;x<customsofts->FirstFree;x++)
      {
        //Запишем в файлик объекты, парся их в чар, перекодируя в вин1251
        ITEM *it=(ITEM*)List_Get(customsofts,x);
        if (it)
        {
          char pattern[512];
          sprintf(pattern,"%d): ", x);
          fwrite(file,pattern, strlen(pattern));
          if (it->name)
          {
            fwrite(file,it->name, wstrlen(it->name)*2);
          }
          fwrite(file,&between,1);
          if (it->lsi)
          {
            fwrite(file,it->lsi, wstrlen(it->lsi)*2);
          }
          fwrite(file,&between,1);
          if (it->msi)
          {
            fwrite(file,it->msi, wstrlen(it->msi)*2);
          }
          fwrite(file,&between,1);
          if (it->rsi)
          {
            fwrite(file,it->rsi, wstrlen(it->rsi)*2);
          }
          fwrite(file,&between,1);
          sprintf(pattern,"%d:",it->style);
          fwrite(file,pattern, strlen(pattern));
          if (it->keys)
          {
            sprintf(pattern,"%d:",it->keys->FirstFree);
            fwrite(file,pattern, strlen(pattern));
            int y=0;
            LIST *lst=it->keys;
            for (y=0;y<lst->FirstFree;y++)
            {
              KEY *key=(KEY*)List_Get(lst,y);
              if (key && key->oldkey>0 && key->oldkey<100 && key->newkey>0 && key->newkey<100)
              {
                char pat[500];
                sprintf(pat,"%d-%d:", key->oldkey, key->newkey);
                fwrite(file,pat, strlen(pat));
              }
            }
          }
          else fwrite(file,"0:", 2);
          fwrite(file,&endstr,1);
          fwrite(file,&cr,1);
          fwrite(file,&lf,1);
        }
      }
    }
    int z;
    for (z=0;z<3;z++)
    {
      char pat[500];
      sprintf(pat,"%dsk): %d:%d:%d:%d:%d:%d:%d:%d;", z, sk[z].x,sk[z].y,sk[z].ct,sk[z].font,sk[z].hx,sk[z].hy,sk[z].hct,sk[z].hfont);
      fwrite(file,pat, strlen(pat));
      fwrite(file,&cr,1);
      fwrite(file,&lf,1);
    }
  }
  fclose(file);
};
