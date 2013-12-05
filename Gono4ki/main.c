#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "Menu.h"
#include "StructBook.h"
#include "Rec_Check.h"
#include "OpenNastroiki.h"
#include "Menu.h"
#include "ErrorIAR.h"
#include "MyFUNC.h"
#define del(a) if(a) delete a;

MyBook*Game;

MyBook * GetBookID()
{
  return Game;
}

int FreeElf(void*, BOOK*bk)
{
  FreeBook(bk);
  return 0;
}

const PAGE_MSG bk_msglst_gm[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    CreateGM,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_gm = {"Game_GuiMenu_Page",0,bk_msglst_gm};

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,     FreeElf,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"Game_Base_Page",0,bk_msglst_base};

#pragma segment="ELFBEGIN"
void elf_exit(void){
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

static void onMyBookClose(BOOK * bk)
{
  SaveSettings(((MyBook*)bk)->nast->sound, ((MyBook*)bk)->nast->im, ((MyBook*)bk)->nast->rez);
  ImagesDel(bk);
  SUBPROC(elf_exit);
}

int main()
{
  if((BOOK*)MyBK())
  {
    BookObj_SetFocus((BOOK*)MyBK(),0);    
    SUBPROC(elf_exit);
  }
  else
  {
  int j, i;
  Game=new MyBook;
  memset(Game,0,sizeof(MyBook));
  CreateBook((BOOK*)Game,onMyBookClose,&bk_base,NameMyElf,-1,0);
  Game->guido=0;
  Game->gui=0;
  for(i=0; i<10; i++)
  {
    for(j=0; j<10; j++)
    {
      Game->cub[i][j]=0;
    }
  }
  Game->tank=new COORDINAT;
  Game->tank->x=0;
  Game->tank->y=0;
  Game->image=new IMAGES;
  for(i=0; i<3; i++)
  {
    Game->image->im[i]=new IMG;
    Game->image->fi[i]=0;
  }
  Game->lvl=0;
  Game->speed=0;
  Game->point=0;
  Game->nast=new NASTROIKI;
  Game->nast->im=new IMGN;
  Game->nast->sound=new SOUND;
  Game->nast->rez=new TAB;
  for(i=0; i<6; i++)
  {
    Game->nast->sound->status[i]=1;
  }
  Game->nast->sound->sound=1;
  wstrcpy(Game->nast->sound->names[0], L"start.mp3");
  wstrcpy(Game->nast->sound->names[1], L"OK.mp3");
  wstrcpy(Game->nast->sound->names[2], L"gameover.mp3");
  wstrcpy(Game->nast->sound->names[3], L"newrecord.mp3");
  wstrcpy(Game->nast->sound->names[4], L"pause.mp3");
  wstrcpy(Game->nast->sound->names[5], L"hide.mp3");
  wstrcpy(Game->nast->im->names[0], L"fon.png");
  wstrcpy(Game->nast->im->names[1], L"fongame.png");
  wstrcpy(Game->nast->im->names[2], L"cub.png");
  Game->nast->rez->te=new TABL;
  Game->megaspeed=1;
  Game->nast->im->on=0;
  Game->nast->rez->tn=new TABL;
  Game->nast->rez->th=new TABL;
  for(i=0; i<10; i++)
  {
    Game->nast->rez->te->rezultat[i]=0;
    Game->nast->rez->tn->rezultat[i]=0;
    Game->nast->rez->th->rezultat[i]=0;
    wstrcpy(Game->nast->rez->te->name[i], L"Unknow");
    wstrcpy(Game->nast->rez->tn->name[i], L"Unknow");
    wstrcpy(Game->nast->rez->th->name[i], L"Unknow");
  }
  Game->dis[0]=new DISPLAY;
  Game->dis[0]->x=176;
  Game->dis[0]->y=220;
  Game->dis[0]->cub=10;
  Game->dis[0]->FP=FONT_E_30R;
  Game->dis[0]->FS=FONT_E_12R;
  Game->dis[1]=new DISPLAY;
  Game->dis[1]->x=240;
  Game->dis[1]->y=320;
  Game->dis[1]->cub=14;
  Game->dis[1]->FP=FONT_E_40R;
  Game->dis[1]->FS=FONT_E_14R;
  Game->dis[2]=new DISPLAY;
  Game->dis[2]->x=128;
  Game->dis[2]->y=168;
  Game->dis[2]->cub=7;
  Game->dis[2]->FP=FONT_E_18R;
  Game->dis[2]->FS=FONT_D_7R;
  Game->pause=0;
  Game->p[0]=new PREGRADA;
  Game->p[1]=new PREGRADA;
  for(i=0; i<2; i++)
  {
    Game->p[i]->y=0;
    for(j=0; j<4; j++)
    {
      Game->p[i]->x[j]=1;
    }
  }
  OpenSettings(Game);
  CheckResurs(Game);
  BookObj_GotoPage((BOOK *)Game,&bk_gm);
  }
  return 0;
}
