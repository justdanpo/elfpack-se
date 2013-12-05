#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\bookmarks.h"
#include "inc\file_op.h"

const wchar_t mcbm_path[] = L"Bookmarks";
const wchar_t mcbm_ext[] = L"mcbm";

void SaveBM(wchar_t* name, wchar_t* path)
{
  if (name && path)
  {
    MCFilePath(mcbm_path);
    mktree(pathbuf);
    snwprintf(pathbuf, MAXELEMS(pathbuf)-1, _ls_ls, pathbuf, name);
    
    int f;
    if ((f=w_fopen(pathbuf,WA_Read+WA_Write+WA_Create+WA_Truncate, 0x1FF, NULL))>=0)
    {
      w_fwrite(f,path,wstrlen(path)*2+2);
      w_fclose(f);
    }
  } 
}

void UseBM(wchar_t* filename)
{
  int f;
  if ((f=w_fopen(filename, WA_Read, 0x1FF, NULL)) >=0)
  {
    if (w_fread(f, pathbuf, MAX_PATH*sizeof(wchar_t)))
    {
      if (curtab >= MAX_TABS)
        curtab = back_tab < MAX_TABS ? back_tab : 0;
        
      if (isdir(pathbuf))
        cd(curtab, pathbuf, -1);
      else
        //if (wsbuf)
        {
          //str_2ws(wsbuf,pathbuf,MAX_PATH);
          //ExecuteFile(wsbuf,0,0);
        }
    }
    w_fclose(f);
  }
}

void _AddBM(wchar_t *wsname)
{
  FILEINF *file = _CurFile();
  if (file)
  {
    wchar_t *buf=new wchar_t[MAX_PATH], *buf2=new wchar_t[MAX_PATH];
    snwprintf(buf, MAX_PATH-1, L"%ls.%ls", wsname, mcbm_ext);
    CurFullPathBM(buf2, file->ws_name, MAX_PATH-1);
    SaveBM(buf, buf2);
    delete buf;
    delete buf2;
  }
}


void DoBMAdd()
{
  FILEINF *file = _CurFile();
  if (file)
  {
    TextInput(muitxt(ind_name), 1, file->ws_name, _AddBM);
  }
}

void DoBMList()
{
  MCFilePath(mcbm_path);
  mktree(pathbuf);
  cdsys(pathbuf);
}

