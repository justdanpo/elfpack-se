#include "myclass.h"
#include "rand.h"
bool CGame::IsGameOver()
{
  DEBUG;
  return isgameover;
};
int CGame::GetRefreshTime()
{
  DEBUG;
  if(isfast) return time/10;
  else return time;
};
bool CGame::GetEmpty(int &x,int &y)
{
  DEBUG;
  map*m=new map(20,25);
  sh->out2(m);
  food*f;
  for(int i=0;i<foodlist->FirstFree;i++)
  {
    f=(food*)List_Get(foodlist,i);
    f->out2(m);
  }
  x=rand()%20;
  y=rand()%25;
  bool flag=true;
  if(m->getstate(x,y)) flag=false;
  delete m;
  return flag;
};
  
void CGame::OnMove()
{
  DEBUG;
  if(!pause)
  {
    food*f;
    int i=0;
    XY*xy;
    bool add=false;
    while(i<foodlist->FirstFree)
    {
      f=(food*)List_Get(foodlist,i);
      if(!f->getstate()) f->timedown(GetRefreshTime());
      if(!f->getstate() && f->gettime()<0)
      {
        delete (food*)List_RemoveAt(foodlist,i);
        continue;
      }
      xy=sh->getfirst();
      if(f->getx()==xy->x && f->gety()==xy->y) f->eated();
      xy=sh->getlast();
      if(f->getx()==xy->x && f->gety()==xy->y) 
      {
        score+=f->getcol();
        time-=10;
        add=true;
        delete (food*)List_RemoveAt(foodlist,i);
        continue;
      }
      i++;
    }
    xy=sh->getlast();
    int x=xy->x;
    int y=xy->y;
    sh->move();
    if(add) sh->add(x,y);
    if((rand()%20)==6 || !foodlist->FirstFree)
    {
      if(GetEmpty(x,y))
      {
        f=new food(x,y,(COLOR)(1+rand()%7));
        List_InsertLast(foodlist,f);
      }
    };
    if(!sh->check()) isgameover=true;
  }
};
void CGame::OnDraw(GC*gc)
{
  DEBUG;
  disp->clear();
  sh->out(disp);
  food*f;
  for(int i=0;i<foodlist->FirstFree;i++)
  {
    f=(food*)List_Get(foodlist,i);
    f->out(disp);
  };
  disp->out(gc);
  if(pause) pausedisp->out(gc);
  DrawNumber(gc,0,0,score);
};
void CGame::OnKey(int key,int mode)
{
  DEBUG;
  if(key==KEY_DIGITAL_0 && mode==KBD_SHORT_RELEASE) pause=!pause;
  if((key==KEY_UP || key==KEY_DIGITAL_0+2) && mode==KBD_SHORT_RELEASE) sh->setway(0);
  if((key==KEY_RIGHT || key==KEY_DIGITAL_0+6) && mode==KBD_SHORT_RELEASE) sh->setway(1);
  if((key==KEY_DOWN || key==KEY_DIGITAL_0+8) && mode==KBD_SHORT_RELEASE) sh->setway(2);
  if((key==KEY_LEFT || key==KEY_DIGITAL_0+4) && mode==KBD_SHORT_RELEASE) sh->setway(3);
  if((key==KEY_ENTER || key==KEY_DIGITAL_0+5) && mode==KBD_SHORT_PRESS) isfast=true;
  if((key==KEY_ENTER || key==KEY_DIGITAL_0+5) && (mode==KBD_LONG_RELEASE || mode==KBD_SHORT_RELEASE)) isfast=false;
};
int CGame::GetScore()
{
  DEBUG;
  return score;
};
CGame::~CGame()
{
  DEBUG;
  while(foodlist->FirstFree) delete (food*)List_RemoveAt(foodlist,0);
  List_Destroy(foodlist);
  delete disp;
  delete pausedisp;
  delete sh;
};
CGame::CGame(int lvl)
{
  DEBUG;
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(0,&dt);
  foodlist=List_Create();
  iconidname2id(L"MIDI_COMP_BL_NBR_0_ICN",-1,&img[0]);
  iconidname2id(L"MIDI_COMP_BL_NBR_1_ICN",-1,&img[1]);
  iconidname2id(L"MIDI_COMP_BL_NBR_2_ICN",-1,&img[2]);
  iconidname2id(L"MIDI_COMP_BL_NBR_3_ICN",-1,&img[3]);
  iconidname2id(L"MIDI_COMP_BL_NBR_4_ICN",-1,&img[4]);
  iconidname2id(L"MIDI_COMP_BL_NBR_5_ICN",-1,&img[5]);
  iconidname2id(L"MIDI_COMP_BL_NBR_6_ICN",-1,&img[6]);
  iconidname2id(L"MIDI_COMP_BL_NBR_7_ICN",-1,&img[7]);
  iconidname2id(L"MIDI_COMP_BL_NBR_8_ICN",-1,&img[8]);
  iconidname2id(L"MIDI_COMP_BL_NBR_9_ICN",-1,&img[9]);
  srand( (dt.time.sec<<16) | (dt.time.min<<8) | (dt.time.hour) );
  score=0;
  time=1000;
  isgameover=false;
  isfast=false;
  pause=false;
  int pxl=6;
  int pxl1=Display_GetHeight(0)/25;
  int pxl2=Display_GetWidth(0)/20;
  if(pxl1>pxl2) pxl=pxl2;
  else pxl=pxl1;
  disp=new display(20,25,pxl);
  pausedisp=new display(20,25,pxl);
  for(int i=0;i<20;i++)
    for(int j=0;j<25;j++)
      pausedisp->set(i,j,BLACK,1);
  // [][][][][]
  pausedisp->set(1,11,WHITE,1);
  pausedisp->set(1,12,WHITE,1);
  pausedisp->set(1,13,WHITE,1);
  pausedisp->set(1,14,WHITE,1);
  pausedisp->set(1,15,WHITE,1);
  //    []  []
  pausedisp->set(2,11,WHITE,1);
  pausedisp->set(2,13,WHITE,1);
  //    [][][]
  pausedisp->set(3,11,WHITE,1);
  pausedisp->set(3,12,WHITE,1);
  pausedisp->set(3,13,WHITE,1);
  //
  //[][][][][]
  pausedisp->set(5,11,WHITE,1);
  pausedisp->set(5,12,WHITE,1);
  pausedisp->set(5,13,WHITE,1);
  pausedisp->set(5,14,WHITE,1);
  pausedisp->set(5,15,WHITE,1);
  //    []  []
  pausedisp->set(6,11,WHITE,1);
  pausedisp->set(6,13,WHITE,1);
  //[][][][][]
  pausedisp->set(7,11,WHITE,1);
  pausedisp->set(7,12,WHITE,1);
  pausedisp->set(7,13,WHITE,1);
  pausedisp->set(7,14,WHITE,1);
  pausedisp->set(7,15,WHITE,1);
  //
  //[][][][][]
  pausedisp->set(9,11,WHITE,1);
  pausedisp->set(9,12,WHITE,1);
  pausedisp->set(9,13,WHITE,1);
  pausedisp->set(9,14,WHITE,1);
  pausedisp->set(9,15,WHITE,1);
  //[]
  pausedisp->set(10,15,WHITE,1);
  //[][][][][]
  pausedisp->set(11,11,WHITE,1);
  pausedisp->set(11,12,WHITE,1);
  pausedisp->set(11,13,WHITE,1);
  pausedisp->set(11,14,WHITE,1);
  pausedisp->set(11,15,WHITE,1);
  //
  //[]  [][][]
  pausedisp->set(13,11,WHITE,1);
  pausedisp->set(13,12,WHITE,1);
  pausedisp->set(13,13,WHITE,1);
  pausedisp->set(13,15,WHITE,1);
  //[]  []  []
  pausedisp->set(14,11,WHITE,1);
  pausedisp->set(14,13,WHITE,1);
  pausedisp->set(14,15,WHITE,1);
  //[][][]  []
  pausedisp->set(15,11,WHITE,1);
  pausedisp->set(15,13,WHITE,1);
  pausedisp->set(15,14,WHITE,1);
  pausedisp->set(15,15,WHITE,1);
  //
  //[][][][][]
  pausedisp->set(17,11,WHITE,1);
  pausedisp->set(17,12,WHITE,1);
  pausedisp->set(17,13,WHITE,1);
  pausedisp->set(17,14,WHITE,1);
  pausedisp->set(17,15,WHITE,1);
  //[]  []  []
  pausedisp->set(18,11,WHITE,1);
  pausedisp->set(18,13,WHITE,1);
  pausedisp->set(18,15,WHITE,1);
  //[]  []  []
  pausedisp->set(19,11,WHITE,1);
  pausedisp->set(19,13,WHITE,1);
  pausedisp->set(19,15,WHITE,1);
  sh=new snake(10,12,3,0,20,25);
};

void CGame::DrawNumeral(GC*gc,int &x,int &y,int numeral)
{
  GC_PutChar(gc,x,y,0,0,img[numeral]);
  x+=GetImageWidth(img[numeral]);
};

void CGame::DrawNumber(GC*gc,int x,int y,int number)
{
  char buf[16];
  sprintf(buf,"%i",number);
  int len=strlen(buf);
  for(int i=0;i<len;i++) DrawNumeral(gc,x,y,buf[i]-0x30);
}

