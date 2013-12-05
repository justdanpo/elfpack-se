#include "inc\mc.h"
#include "inc\config.h"


const wchar_t mccf_config[]=L"mcconfig.cfg";
MC_CFG MCConfig;


wchar_t* GetCfgPath()
{
  if (!CONFIG_MCCONFIG_PATH[0])
    wstrcpy((wchar_t*)CONFIG_MCCONFIG_PATH, mcpath);
  
  int pathLen = wstrlen(CONFIG_MCCONFIG_PATH);
  if (pathLen > 0)
  {
    if (((wchar_t*)CONFIG_MCCONFIG_PATH)[pathLen - 1] == '/')
      ((wchar_t*)CONFIG_MCCONFIG_PATH)[pathLen - 1] = 0;
    snwprintf(pathbuf, MAXELEMS(pathbuf)-1,_ls_ls, CONFIG_MCCONFIG_PATH, mccf_config);
    return pathbuf;
  }
  return NULL;
}

void InitCfg()
{
  memset(&MCConfig, 0, sizeof(MCConfig));
  if (CONFIG_SAVE_PATH)
  {
    if (curtab >= MAX_TABS) 
      MCConfig.curtab = back_tab < MAX_TABS ? back_tab : 0;
    else
      MCConfig.curtab = curtab;
    
    for(int ii=0; ii < MAX_TABS; ii++)
    {
      int dr = tabs[ii]->CurDrv;
      MCConfig.tabs[ii].sort = tabs[ii]->sort;
      MCConfig.tabs[ii].LastInd = tabs[ii]->iIndex[dr];
      MCConfig.tabs[ii].drv = tabs[ii]->CurDrv;
      wstrcpy(MCConfig.tabs[ii].LastPath, tabs[ii]->szDirs[dr]);
    }
  }
}

void SaveCfg()
{
  int f;
  wchar_t* cfgPath = GetCfgPath();
  if (cfgPath)
  {
    if ((f=w_fopen(cfgPath, WA_Read+WA_Write+WA_Create+WA_Truncate, 0x1FF, NULL))>=0)
    {
      MCCF_hdr hdr;
      hdr.sig=mccf_sig;
      hdr.ver=mccf_ver;
      hdr.size=mccf_size;
      hdr.offset=mccf_offset;
      
      InitCfg();
      w_fwrite(f,&hdr,sizeof(hdr));
      w_fwrite(f,&MCConfig,sizeof(MCConfig));
      w_fclose(f);
    }
  }
}


int LoadCfg()
{
  int res = 0;
  int f;
  wchar_t* cfgPath = GetCfgPath();
  if (cfgPath)
  {
    if ((f=w_fopen(cfgPath, WA_Read, 0x1FF, NULL))>=0)
    {
      MCCF_hdr hdr;
      if (w_fread(f, &hdr, sizeof(hdr))==sizeof(hdr) &&
          (hdr.sig==mccf_sig) && (hdr.size>=mccf_size))
      {
        w_lseek(f,hdr.offset,WSEEK_SET);
        res = w_fread(f, &MCConfig, sizeof(MCConfig))==sizeof(MCConfig);
      }
      w_fclose(f);
    }
  }
  if (!res)memset(&MCConfig, 0, sizeof(MCConfig));
  return res;
}
