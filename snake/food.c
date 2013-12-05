#include "myclass.h"
#include "rand.h"
int food::gettime()
{
  DEBUG;
  return timeleft;
};
void food::timedown(int dtime)
{
  DEBUG;
  timeleft-=dtime;
};
void food::eated()
{
  DEBUG;
  state=true;
};
bool food::getstate()
{
  DEBUG;
  return state;
};
int food::gety()
{
  DEBUG;
  return _y;
};
int food::getx()
{
  DEBUG;
  return _x;
};
food::~food()
{
  DEBUG;
};

food::food(int x,int y,COLOR col)
{
  DEBUG;
  _col=col;
  _x=x;
  _y=y;
  timeleft=rand()%25000;
  state=false;
};

void food::out(display*disp)
{
  DEBUG;
  disp->set(_x,_y,_col,1);
};
void food::out2(map*m)
{
  DEBUG;
  m->set(_x,_y);
};

int food::getcol()
{
  DEBUG;
  return _col;
};
