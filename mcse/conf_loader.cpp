#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "inc\conf_loader.h"


const wchar_t *successed_config_path=L"";
const wchar_t *successed_config_name=L"";

#pragma segment="CONFIG_C"
int LoadConfigData(const wchar_t *path, const wchar_t *fname)
{
  int f;
  char *buf;
  int result=-1;
  void *cfg;
  W_FSTAT stat;
  unsigned int rlen;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=new char[len]))
  {
    if (w_fstat(fname,&stat)!=-1)
    {
      if ((f=w_fopen(fname,WA_Read,0x1FF,0))>=0)
      {
        rlen=w_fread(f,buf,len);
        w_fclose(f);
        if (rlen!=stat.st_size || rlen!=len)  goto L_SAVENEWCFG;
        memcpy(cfg,buf,len);
        result=0;
      }
    }
    else
    {
    L_SAVENEWCFG:
      if ((f=w_fopen(fname,WA_Write|WA_Create|WA_Truncate,0x1FF,0))>=0)
      {
        if (w_fwrite(f,cfg,len)==len) result=0;
        w_fclose(f);
      }
    }
    delete buf;
  }
  if (result>=0)
  {
    successed_config_path=path;
    successed_config_name=fname;
  }
  return(result);
}


void InitConfig(void)
{
  wchar_t *s;
  if (!w_chdir((s=GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL))))
  {
    if (LoadConfigData(s,L"mcse.bcfg")<0)
    {
      if (!w_chdir((s=GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL))))
        LoadConfigData(s,L"mcse.bcfg");
    }
  }
}
