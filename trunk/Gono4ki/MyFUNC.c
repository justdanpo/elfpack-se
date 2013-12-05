#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"

void vibra()
{
  PAudioControl pAC = AudioControl_Init();
  if( !pAC ) pAC = *GetAudioControlPtr();
  AudioControl_Vibrate(pAC, 200, 50, 200);
}


wchar_t*NamesOfPunkts(int x)
{
  switch(x)
  {
  case 0:
    return L"Звук старта";
  case 1:
    return L"Звук очка";
  case 2:
    return L"Звук конца игры";
  case 3:
    return L"Звук рекорда";
  case 4:
    return L"Звук паузы";
  case 5:
    return L"Звук перемещения";
  }
  return L" ";
}
    

int GetStrokaIMG(wchar_t*stroka, int x)
{
  int met[2];
  if(x) iconidname2id(L"CHECKMARK_IN_BOX_ICN",-1, &met[0]);
  else iconidname2id(L"CHECKBOX_ICN", -1, &met[0]);
  met[0]=(0x78000000+met[0]);
  met[1]=TextID_Create(stroka, ENC_UCS2, TEXTID_ANY_LEN);
  return (TextID_Create(met, ENC_TEXTID, 2));
}

BOOK*isBookX(char const*isbook, int x)
{
  int j, i, k;
  BOOK*bk;
  j=root_list_get_session_count();
  if(x)
  {
    bk=(BOOK*)List_Get(root_list_get_session(j-1)->listbook,root_list_get_session(j-1)->listbook->FirstFree-1);
    if(!strcmp(bk->xbook->name,isbook)) return bk;
  }
  else
  {
    for(i=0; i<j; i++)
    {
      for(k=0;k<root_list_get_session(i)->listbook->FirstFree;k++)
      {
        bk=(BOOK*)List_Get(root_list_get_session(i)->listbook,k);
        if(!strcmp(bk->xbook->name,isbook)) return bk;
      }
    }
  }
  return(0);
}

MyBook*MyBK()
{
  return ((MyBook*)isBookX(NameMyElf,0));
}


wchar_t * StatusSettings1(int s)
{
  if(s==0) return L"Вкл.";
  return L"Откл.";
}

wchar_t * StatusSettings(int s)
{
  if(s==1) return L"Включенно";
  return L"Отключенно";
}

int Record(TABL*tab, int new_result, int N)
{
  int i=0;
  while(i<N)
  {
    if(new_result>(tab->rezultat[i]))
    {
      return i+1;
    }
    i++;
  }
  return 0;
}
int Perestava(TABL*tab, wchar_t new_name[8], int new_result, int N)
{
  int i=0;
  int j;
  while(i<N)
  {
    if(new_result>(tab->rezultat[i]))
    {
      for(j=N-2; j>=i; j+=-1)
      {
        wstrcpy(tab->name[j+1], tab->name[j]);
        tab->rezultat[j+1]=tab->rezultat[j];
      }
      tab->rezultat[i]=new_result;
      wstrcpy(tab->name[i], new_name);
      return i+1;
    }
    i++;
  }
  return 0;
}

wchar_t * GetConfigPath()
{
  wchar_t*cpath=new wchar_t[wstrlen(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL))+wstrlen(L"/Games/Gono4ki")+1];
  wstrcpy(cpath,GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
  wstrcat(cpath,L"/Games/Gono4ki");
  return(cpath);
};

int GetDis()
{
  switch(Display_GetHeight(0))
  {
  case 220:
    return 0;
  case 320:
    return 1;
  }
  return 2;
}

int GetFSize(int f)
{
  if(GetDis()==0 && f==FONT_E_30R) return 30;
  if(GetDis()==0 && f==FONT_E_12R) return 12;
  if(GetDis()==1 && f==FONT_E_40R) return 40;
  if(GetDis()==1 && f==FONT_E_14R) return 14;
  if(GetDis()==2 && f==FONT_E_18R) return 18;
  if(GetDis()==2 && f==FONT_D_7R) return 7;
  return 0;
}

int FindFile(wchar_t*path, wchar_t const*name)
{
  FSTAT _fstat;
  if(fstat(path,name,&_fstat)==0) return 0;
  return 1;
}

int CheckMusic(int x)
{
  int result=!FindFile(GetConfigPath(),MyBK()->nast->sound->names[x])*MyBK()->nast->sound->sound*MyBK()->nast->sound->status[x]*!FindBook(get_IsFmRadioBook())*!FindBook(get_IsAudioPlayerBook());
  return result;
}

void DELTABL(TABL*t)
{
  for(int i=0; i<10; i++)
  {
    t->rezultat[i]=0;
    wstrcpy(t->name[i], L"Unknow");
  }
}

int PlayMyMusic(int x)
{
  if(CheckMusic(x)) PlayFile(GetConfigPath(),MyBK()->nast->sound->names[x]);
  return 0;
}
