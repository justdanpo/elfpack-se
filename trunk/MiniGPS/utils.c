#include "util.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

struct
{
        wchar_t* name;
        int value;
}cfgmacro []=
{
        L"AUDIO",        DIR_AUDIO,
        L"IMAGE",        DIR_IMAGE,
        L"VIDEO",        DIR_VIDEO,
        L"THEME",        DIR_THEME,
        L"OTHER",        DIR_OTHER,
        L"ELFS",         DIR_ELFS,
        L"ELFS_DAEMONS", DIR_ELFS_DAEMONS,
        L"ELFS_CONFIG",  DIR_ELFS_CONFIG,
        L"INI",          DIR_INI
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

void extractdir(wchar_t* path,wchar_t* name, const wchar_t* src)
{
        int dir=-1;
        wchar_t dest[256];

        for(int i=0;i<sizeof(cfgmacro)/sizeof(cfgmacro[0]);i++)
        {
                wchar_t tmp[32];
                int sz;
                snwprintf(tmp,MAXELEMS(tmp),L"%%%ls_INT%%",cfgmacro[i].name);
                sz=wstrlen(tmp);
                if(!wstrncmp(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_INTERNAL;
                        src+=sz;
                        break;
                }
                snwprintf(tmp,MAXELEMS(tmp),L"%%%ls_EXT%%",cfgmacro[i].name);
                sz=wstrlen(tmp);
                if(!wstrncmp(src,tmp,sz))
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
        wstrcat(dest,src);
        splitfilename(dest,path,name);
}

void getdir(wchar_t* path, const wchar_t* src)
{
        int dir=-1;

        for(int i=0;i<sizeof(cfgmacro)/sizeof(cfgmacro[0]);i++)
        {
                wchar_t tmp[32];
                int sz;
                snwprintf(tmp,MAXELEMS(tmp),L"%%%ls_INT%%",cfgmacro[i].name);
                sz=wstrlen(tmp);
                if(!wstrncmp(src,tmp,sz))
                {
                        dir=cfgmacro[i].value|MEM_INTERNAL;
                        src+=sz;
                        break;
                }
                snwprintf(tmp,MAXELEMS(tmp),L"%%%ls_EXT%%",cfgmacro[i].name);
                sz=wstrlen(tmp);
                if(!wstrncmp(src,tmp,sz))
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
        wstrcat(path,src);
}

