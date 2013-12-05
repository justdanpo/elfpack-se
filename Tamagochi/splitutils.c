#include "misc_util.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

struct
{
        char* name;
        int value;
}cfgmacro []=
{
        "AUDIO",DIR_AUDIO
        ,"IMAGE",DIR_IMAGE
        ,"VIDEO",DIR_VIDEO
        ,"THEME",DIR_THEME
        ,"OTHER",DIR_OTHER
        ,"ELFS",DIR_ELFS
        ,"ELFS_DAEMONS",DIR_ELFS_DAEMONS
        ,"ELFS_CONFIG",DIR_ELFS_CONFIG
        ,"INI",DIR_INI
};

void splitfilename(const wchar_t* src, wchar_t* destpath, wchar_t* destname)
{
        wchar_t* fname=wstrrchr(src,'/');
        if(fname)
        {
          wstrncpy(destpath,src,fname-src);
          destpath[fname-src] = 0;
          wstrcpy(destname,fname+1);
        }
        else
        {
          destpath[0] = 0;
          wstrcpy(destname,src);
        }
}

void extractdir(wchar_t* path,wchar_t* name, const char* src)
{
        int dir=-1;
        wchar_t dest[256];

        for(int i=0;i<sizeof(cfgmacro)/sizeof(cfgmacro[0]);i++)
        {
                char tmp[32];
                int sz;
                sz=sprintf(tmp,"%%%s_INT%%",cfgmacro[i].name);
                if(!strncmpi(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_INTERNAL;
                        src+=sz;
                        break;
                }
                sz=sprintf(tmp,"%%%s_EXT%%",cfgmacro[i].name);
                if(!strncmpi(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_EXTERNAL;
                        src+=sz;
                        break;
                }
        }
        if(dir!=-1)
                wstrcpy(dest,GetDir(dir));
        else
                dest[0]=0;
        str2wstr(dest+wstrlen(dest),src);
        splitfilename(dest,path,name);
}

void getdir(wchar_t* path, const char* src)
{
        int dir=-1;

        for(int i=0;i<sizeof(cfgmacro)/sizeof(cfgmacro[0]);i++)
        {
                char tmp[32];
                int sz;
                sz=sprintf(tmp,"%%%s_INT%%",cfgmacro[i].name);
                if(!strncmpi(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_INTERNAL;
                        src+=sz;
                        break;
                }
                sz=sprintf(tmp,"%%%s_EXT%%",cfgmacro[i].name);
                if(!strncmpi(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_EXTERNAL;
                        src+=sz;
                        break;
                }
        }
        if(dir!=-1)
                wstrcpy(path,GetDir(dir));
        else
                path[0]=0;
        str2wstr(path+wstrlen(path),src);
}

