#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\cfg_items.h"
#include "header\skin_loader.h"

#include "cfg_2_0.h"
#include "cfg_2_0_corrupted.h"
#include "cfg_2_1.h"

wchar_t successed_skin_path[512];
wchar_t successed_skin_name[512];
int SaveSkinData(const wchar_t *path,const wchar_t *fname);

#define BCFGLOADER_COMPLETE 0
#define BCFGLOADER_CONFIG_OLD_OR_CORRUPTED -1
#define BCFGLOADER_NOT_FOUND -2
#define BCFGLOADER_MEMORY_NOT_ALLOCATED -3
#define BCFGLOADER_PATH_OR_NAME_CORRUPTED -4
#define BCFGLOADER_UNKNOWN_ERROR -5
#define BCFGLOADER_SUCCESSED(a) a>=0 ? 1 : 0
#define BCFGLOADER_FAILED(a) a<0 ? 1 : 0
#pragma segment="SKIN_C"
int LoadSkinData(const wchar_t *path,const wchar_t *fname)
{
  if (path==0 || fname==0)
    return BCFGLOADER_PATH_OR_NAME_CORRUPTED;
  int f;
  int result=BCFGLOADER_UNKNOWN_ERROR;
  void *cfg;
  FSTAT _fstat;
  unsigned int rlen;
  
  cfg=(char *)__segment_begin("SKIN_C");

  unsigned int len=(char *)__segment_end("SKIN_C")-(char *)__segment_begin("SKIN_C");
  if (fstat(path,fname,&_fstat)!=-1)
  {
    if (_fstat.fsize==len)
    {
      if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
      {
        rlen=fread(f, cfg, len);
        fclose(f);
        result=1;
      }
    }
    else
    {
      char *buf=new char[_fstat.fsize];
      if (buf)
      {
        if ((f=_fopen(path,fname,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
        {
          rlen=fread(f, buf, _fstat.fsize);
          fclose(f);
        }
        int migrate_from_2_0corrupted_to_2_1(CFG_2_0_corrupted *oldcfg, CFG_2_1 *cfg);
        int migrate_from_2_0_to_2_1(CFG_2_0 *oldcfg, CFG_2_1 *cfg);
        if (rlen==sizeof(CFG_2_0_corrupted))
        {
          migrate_from_2_0corrupted_to_2_1((CFG_2_0_corrupted*)buf, (CFG_2_1*)cfg);
          SaveSkinData(path,fname);
          MessageBox(EMPTY_TEXTID,TextID_Create(L"Migrated from 2.0 rus",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
          return 1;
        }
        else if (rlen==sizeof(CFG_2_0))
        {
          migrate_from_2_0_to_2_1((CFG_2_0*)buf, (CFG_2_1*)cfg);
          SaveSkinData(path,fname);
          MessageBox(EMPTY_TEXTID,TextID_Create(L"Migrated from 2.0 eng",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
          return 1;
        }
        delete(buf);
      }
      else
      {
        result=BCFGLOADER_MEMORY_NOT_ALLOCATED;
      }
    }
  }
  if (BCFGLOADER_SUCCESSED(result))
  {
    wstrcpy(successed_skin_path,path);
    wstrcpy(successed_skin_name,fname);
    debug_printf("\nWalkmanDisplay: skindata loaded from %ls/%ls\n", successed_skin_path, successed_skin_name);
  }
  return(result);
};

int SaveSkinData(const wchar_t *path,const wchar_t *fname)
{
  void *cfg=(char *)__segment_begin("SKIN_C");

  unsigned int len=(char *)__segment_end("SKIN_C")-(char *)__segment_begin("SKIN_C");
  int f;
  int result=BCFGLOADER_UNKNOWN_ERROR;
  if ((f=_fopen(path,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    if (fwrite(f,cfg,len)==len) result=BCFGLOADER_COMPLETE;
    fclose(f);
  }
  if (BCFGLOADER_SUCCESSED(result))
  {
    wstrcpy(successed_skin_path,path);
    wstrcpy(successed_skin_name,fname);
    debug_printf("\nWalkmanDisplay: skindata saved to %ls/%ls\n", successed_skin_path, successed_skin_name);
  }
  return result;
};
#include "header\config_data.h"
void skin_InitConfig(wchar_t *path, int mode)
{
  if (skin_cfg==1 && mode==0)
  {
    goto L_SkipSkinData;
  }
  if (BCFGLOADER_FAILED( LoadSkinData(path, L"WalkmanDisplay_Skin.bcfg") ))
  {
  L_SkipSkinData:
    if (BCFGLOADER_FAILED( LoadSkinData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),L"WalkmanDisplay_Skin.bcfg") ))
    {
      if (BCFGLOADER_FAILED( LoadSkinData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),L"WalkmanDisplay_Skin.bcfg") ))
      {
        if (BCFGLOADER_FAILED( SaveSkinData(GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL),L"WalkmanDisplay_Skin.bcfg") ))
        {
          SaveSkinData(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL),L"WalkmanDisplay_Skin.bcfg");
        }
      }
    }
  }
};
