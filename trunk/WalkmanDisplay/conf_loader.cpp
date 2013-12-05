#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\cfg_items.h"
#include "header\conf_loader.h"

const wchar_t *successed_config_path=L"";
const wchar_t *successed_config_name=L"";

#define BCFGLOADER_COMPLETE 0
#define BCFGLOADER_CONFIG_OLD_OR_CORRUPTED -1
#define BCFGLOADER_NOT_FOUND -2
#define BCFGLOADER_MEMORY_NOT_ALLOCATED -3
#define BCFGLOADER_PATH_OR_NAME_CORRUPTED -4
#define BCFGLOADER_UNKNOWN_ERROR -5
#define BCFGLOADER_SUCCESSED(a) a>=0 ? 1 : 0
#define BCFGLOADER_FAILED(a) a<0 ? 1 : 0
#pragma segment="CONFIG_C"
int LoadConfigData(const wchar_t *path,const wchar_t *fname)
{
  int f;
  char *buf;
  int result=BCFGLOADER_UNKNOWN_ERROR;
  void *cfg;
  FSTAT _fstat;
  unsigned int rlen;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=new char[len]))
  {
    if (fstat(path,fname,&_fstat)!=-1)
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        rlen=fread(f,buf,len);
        fclose(f);
        if (rlen!=_fstat.fsize || rlen!=len)  return BCFGLOADER_CONFIG_OLD_OR_CORRUPTED;
        memcpy(cfg,buf,len);
        result=BCFGLOADER_COMPLETE;
      }
    }
    else
    {
      result=BCFGLOADER_NOT_FOUND;
    }
    delete buf;
  }
  else
  {
    result=BCFGLOADER_MEMORY_NOT_ALLOCATED;
  }
  if (BCFGLOADER_SUCCESSED(result))
  {
    successed_config_path=path;
    successed_config_name=fname;
    debug_printf("\nWalkmanDisplay: cfgdata loaded from %ls/%ls\n", successed_config_path, successed_config_name);
  }
  return(result);
};

int SaveConfigData(const wchar_t *path,const wchar_t *fname)
{
  int result=BCFGLOADER_PATH_OR_NAME_CORRUPTED;
  if (path && fname)
  {
    void *cfg=(char *)__segment_begin("CONFIG_C");
    
    unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");
    int f;
    
    if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      if (fwrite(f,cfg,len)==len) result=0;
      fclose(f);
    }
    if (BCFGLOADER_SUCCESSED(result))
    {
      successed_config_path=path;
      successed_config_name=fname;
      debug_printf("\nWalkmanDisplay: cfgdata saved to %ls/%ls\n", successed_config_path, successed_config_name);
    }
  }
  return result;
};

void InitConfig(void)
{
  if (BCFGLOADER_FAILED( LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),L"WalkmanDisplay.bcfg") ))
  {
    if (BCFGLOADER_FAILED( LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),L"WalkmanDisplay.bcfg") ))
    {
      if (BCFGLOADER_FAILED( SaveConfigData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),L"WalkmanDisplay.bcfg") ))
      {
        SaveConfigData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),L"WalkmanDisplay.bcfg");
      }
    }
  }
};
