#include "myclass.h"
void snake::setway(char way)
{
  DEBUG;
  _waych=way;
};
void snake::out(display*disp)
{
  DEBUG;
  XY*elem;
  for(int i=0;i<list->FirstFree;i++)
  {
    elem=(XY*)List_Get(list,i);
    disp->set(elem->x,elem->y,BLACK,1);
  }
};
void snake::out2(map*m)
{
  DEBUG;
  XY*elem;
  for(int i=0;i<list->FirstFree;i++)
  {
    elem=(XY*)List_Get(list,i);
    m->set(elem->x,elem->y);
  }
};
void snake::add(int x,int y)
{
  DEBUG;
  XY*elem=new XY;
  elem->x=x;
  elem->y=y;
  List_InsertLast(list,elem);
};
XY*snake::getfirst()
{
  DEBUG;
  return (XY*)List_Get(list,0);
};
XY*snake::getlast()
{
  DEBUG;
  return (XY*)List_Get(list,list->FirstFree-1);
};
bool snake::check()
{
  DEBUG;
  XY*top_elem=(XY*)List_Get(list,0);
  XY*elem;
  for(int i=1;i<list->FirstFree;i++)
  {
    elem=(XY*)List_Get(list,i);
    if(top_elem->x==elem->x && top_elem->y==elem->y) return false;
  }
  return true;
};
void snake::move()
{
  DEBUG;
  XY*elem1;
  XY*elem2;
  for(int i=list->FirstFree-1;i>0;i--)
  {
    elem1=(XY*)List_Get(list,i);
    elem2=(XY*)List_Get(list,i-1);
    elem1->x=elem2->x;
    elem1->y=elem2->y;
  }
  elem1=(XY*)List_Get(list,0);
  if(_waych!=-1)
  {
    if((_waych-_way)!=2 && (_waych-_way)!=-2) _way=_waych;
    _waych=-1;
  };
  switch(_way)
  {
  case 0:
    elem1->y--;
    break;
  case 1:
    elem1->x++;
    break;
  case 2:
    elem1->y++;
    break;
  case 3:
    elem1->x--;
    break;
  }
  if(elem1->x>_mx-1) elem1->x=0;
  if(elem1->x<0) elem1->x=_mx-1;
  if(elem1->y>_my-1) elem1->y=0;
  if(elem1->y<0) elem1->y=_my-1;
};
snake::~snake()
{
  DEBUG;
  while(list->FirstFree) delete (XY*)List_RemoveAt(list,0);
  List_Destroy(list);
};
snake::snake(int x,int y,int n,char way,int maxx,int maxy)
{
  DEBUG;
  list=List_Create();
  _way=way;
  _mx=maxx;
  _my=maxy;
  _waych=-1;
  XY*elem=new XY;
  elem->x=x;
  elem->y=y;
  List_InsertFirst(list,elem);
  XY*nextelem;
  for(int i=1;i<n;i++)
  {
    nextelem=new XY;
    switch(way)
    {
    case 0:
      nextelem->x=elem->x;
      nextelem->y=elem->y+1;
      break;
    case 1:
      nextelem->x=elem->x-1;
      nextelem->y=elem->y;
      break;
    case 2:
      nextelem->x=elem->x;
      nextelem->y=elem->y-1;
      break;
    case 3:
      nextelem->x=elem->x+1;
      nextelem->y=elem->y;
      break;
    }
    List_InsertLast(list,nextelem);
    elem=nextelem;
  }
};

int snake::getlen()
{
  DEBUG;
  return list->FirstFree;
};
