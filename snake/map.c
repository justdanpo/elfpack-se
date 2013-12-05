#include "myclass.h"

bool map::getstate(int x,int y)
{
  DEBUG;
  return _map[y][x];
};

map::~map()
{
  DEBUG;
  for(int i=0;i<_y;i++) delete[] _map[i];
  delete[] _map;
};
    
void map::set(int x,int y)
{
  DEBUG;
  _map[y][x]=true;
};

map::map(int x,int y)
{
  DEBUG;
  _x=x;
  _y=y;
  _map=new bool*[y];
  for(int i=0;i<y;i++)
  {
    _map[i]=new bool[x];
    for(int j=0;j<x;j++) _map[i][j]=0;
  }
};
