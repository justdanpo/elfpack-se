#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "structs.h"
#include "Externals.h"

extern ANIM_LIST anlst;

int c2i(char *prt)
{
  int res=0; while (prt[0]!=';') { res=res * 10 + (prt[0]-0x30); *prt++; } return res;
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


void c2anim(char *line)
{
  // Добавляем анимацию в список
  ANIM_ELEM *anel=new ANIM_ELEM;
  memset(anel,0,sizeof(ANIM_ELEM));
  int pos=0;
  anel->lblID=getint(line, &pos, ':');
  anel->X=getint(line, &pos, ':');
  anel->Y=getint(line, &pos, ':');
  anel->maxX=getint(line, &pos, ':');
  anel->maxY=getint(line, &pos, ':');
  anel->Color=getint(line, &pos, ':');
  anel->Color2=getint(line, &pos, ':');
  anel->Font=getint(line, &pos, ':');
  anel->CT=getint(line, &pos, ':');
  anel->DrawWithNext=getint(line, &pos, ';');
  List_InsertLast(anlst.lst,anel);
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
  while(*h!='\0' || *h!=0);
  return(res);
};
