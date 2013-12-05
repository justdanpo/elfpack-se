#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "config_data.h"
#include "structs.h"
#include "Externals.h"
#include "string.h"

extern LABEL labels[10];
extern UI_IMG img;

ANIM_LIST anlst;
int AnimUpdateTime=1000;
int AnimYPlus=0;

void MakeUsed(char *line)
{
  int x=0;
  while (line[x]!=';')
  {
    labels[line[x]-0x30].UsedInAnim=true;
    x++;
  }
};

void MakeHidden(char *line)
{
  int x=0;
  while (line[x]!=';')
  {
    labels[line[x]-0x30].Hide=true;
    x++;
  }
};

void Anim_Init()
{
  //Создадим список анимационных экшнов
  anlst.lst=List_Create();
  int Count=0;
  anlst.index=-1;
  if (AllowAnim==true)
  {
    int file;
    FSTAT fst;
    wchar_t *name=new wchar_t[strlen(ANIM_NAME)+strlen(".cas")+1];
    char2unicode(name, ANIM_NAME,strlen(ANIM_NAME));
    wstrcat(name,L".cas");
    if (fstat(checkfile(name),name,&fst)==0)
    {
        if ((file=_fopen(checkfile(name),name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
        {
          char * buf;
          char * param;
          buf=new char[fst.fsize+1];
          fread(file,buf,fst.fsize);
          
          if (param=manifest_GetParam(buf,"[ACTIONS_COUNT]",0))
          {   
            Count=c2i(param);
            mfree(param);
          }
          if (param=manifest_GetParam(buf,"[UPDATE_TIME]",0))
          {
            AnimUpdateTime=c2i(param);
            mfree(param);
          }
          if (param=manifest_GetParam(buf,"[USED_LABELS]",0))
          {
            MakeUsed(param);
            mfree(param);
          }
          if (param=manifest_GetParam(buf,"[HIDE_WHEN_ISNT_IN_STANDBY]",0))
          {
            MakeHidden(param);
            mfree(param);
          }
          int x=0;
          for (x=0;x<Count;x++)
          {
             char pattern[128];
             sprintf(pattern,"%d)",x);
             if (param=manifest_GetParam(buf,pattern,0))
             {
               c2anim(param);
               mfree(param);
             }
          }
        }
    }
    delete(name);
  }
};

void Anim_Free()
{
  //Удалим список экшнов
  ANIM_ELEM *anel;
  while (anlst.lst->FirstFree)
  {
    anel=(ANIM_ELEM*)List_RemoveAt(anlst.lst,0);
    delete(anel);
  }
  anlst.index=0;
  List_Destroy(anlst.lst);
  int x;
  for (x=0;x<10;x++)
  {
    labels[x].UsedInAnim=false;
    labels[x].Hide=false;
  }
};

int Anim_NextFrame()
{
  //Следующий анимационный "кадр"
  ANIM_ELEM *anel=0;
  if (anlst.index==(anlst.lst->FirstFree))
  {
    anlst.index=0;
    anel=(ANIM_ELEM*)List_Get(anlst.lst, 0);
  }
  else
  {
    anlst.index++;
    anel=(ANIM_ELEM*)List_Get(anlst.lst, anlst.index);
  }
  if (anel)
  {
    if (anel->lblID<10)
    {
      labels[anel->lblID].X=anel->X;
      labels[anel->lblID].Y=anel->Y;
      labels[anel->lblID].MaxX=anel->maxX;
      labels[anel->lblID].MaxY=anel->maxY;
    
      if (anel->CT!=3)
      {
        labels[anel->lblID].CT=anel->CT;
      }
      if (anel->Font!=0)
      {
        labels[anel->lblID].Font=anel->Font;
        labels[anel->lblID].FONT_SIZE=Font_GetHeight(anel->Font);
      }
      if (anel->Color!=0)
      {
        labels[anel->lblID].Color=anel->Color;
      }
      if (anel->Color2!=0)
      {
        labels[anel->lblID].borderColor=anel->Color2;
      }
    }
    else
    {
      if (anel->lblID==10)
      {
        img.x=anel->X;
        img.y=anel->Y;
        img.mx=anel->maxX;
        img.my=anel->maxY;
      }
      if (anel->lblID==11)
      {
        locCover_X=anel->X;
        locCover_Y=anel->Y;
        locCover_MaxX=anel->maxX;
        locCover_MaxY=anel->maxY;
      }
    }
    return anel->DrawWithNext;
  }
  return 0;
};
