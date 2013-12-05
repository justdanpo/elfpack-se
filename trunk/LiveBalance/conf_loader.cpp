#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"

const wchar_t *successed_config_path=L"";
const wchar_t *successed_config_name=L"";

#pragma segment="CONFIG_C"
int LoadConfigData(const wchar_t *path,const wchar_t *fname)
{
  int f;
  char *buf;
  int result=-1;
  void *cfg;
  FSTAT stat;
  unsigned int rlen;

  cfg=(char *)__segment_begin("CONFIG_C");

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=new char[len]))
  {
    if (fstat(path,fname,&stat)!=-1)
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        rlen=fread(f,buf,len);
        fclose(f);
        if (rlen!=stat.fsize || rlen!=len)  goto L_SAVENEWCFG;
        memcpy(cfg,buf,len);
        result=0;
      }
    }
    else
    {
    L_SAVENEWCFG:
      if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        if (fwrite(f,cfg,len)==len) result=0;
        fclose(f);
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


unsigned int GetCC_NCfromIMSI(char *imsi)
{
  unsigned int cc, cc2, nc;
  cc=(*(imsi+1)>>4)<<8;
  cc2=*(imsi+2);
  cc2=((cc2&0x0F)<<4)|(cc2>>4);
  cc|=cc2;
  nc=*(imsi+3);
  nc=((nc&0x0F)<<4)|(nc>>4);
  return ((cc<<16)|nc);
}

static wchar_t config_name[64];
void InitConfig(void)
{
  unsigned int cc;
  unsigned int nc;
  char *imsi=GetIMSI();
  cc=GetCC_NCfromIMSI(imsi);
  nc=cc&0xFFFF;
  cc>>=16;
  snwprintf(config_name,MAXELEMS(config_name)-1,L"LiveBalance_%02X-%02X%02X%02X%02X-%03X.bcfg",nc,imsi[5],imsi[6],imsi[7],imsi[8],cc);
  if (LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),config_name)<0)
  {
    LoadConfigData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),config_name);
  }
}
