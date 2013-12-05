#include "myclass.h"
int CGameDisp::onCreate()
{
  DEBUG;
  isondraw=false;
  go_state=false;
  game=new CGame(0);
  InvalidateRect(NULL);
  SetRefreshTimer(game->GetRefreshTime());
  return 1;
};

void CGameDisp::onDestroy()
{
  DEBUG;
  delete game;
};

void CGameDisp::onRefresh()
{
  DEBUG;
  if(!go_state) 
  {
    game->OnMove();
    SetRefreshTimer(game->GetRefreshTime());
    if(game->IsGameOver()) go_state=true;
  }
  if(!isondraw) InvalidateRect(NULL);
};
  
void CGameDisp::onKey(int key,int,int repeat,int type)
{
  DEBUG;
  if(!go_state) game->OnKey(key,type);
  else
  {
    if(key==KEY_DIGITAL_0+5)
    {
      go_state=false;
      delete game;
      game=new CGame(0);
      SetRefreshTimer(game->GetRefreshTime());
    }
  }
  if(!isondraw) InvalidateRect(NULL);
};
void CGameDisp::onDraw(int a,int b,int c)
{
  DEBUG;
  isondraw=true;
  GC*gc=get_DisplayGC();
  GC_DrawFRect(gc,0xFF000000,0,0,Display_GetWidth(0),Display_GetHeight(0));
  game->OnDraw(gc);
  if(go_state) GC_DrawFRect(gc,0xAA000000,0,0,Display_GetWidth(0),Display_GetHeight(0));
  isondraw=false;
};
char*CGameDisp::getName()
{
  DEBUG;
  return "SnakeGameDisp";
};
