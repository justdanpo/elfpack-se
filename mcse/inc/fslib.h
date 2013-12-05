#ifndef _FSLIB_H
#define _FSLIB_H

wchar_t* GetFileDir(wchar_t* fname, wchar_t* buf, int is_arch);
wchar_t* GetFileName(wchar_t* fname);
wchar_t* GetFileExt(wchar_t* fname);

typedef int (*ENUM_FILES_PROC) (wchar_t *path, wchar_t *name, W_FSTAT *fs, int param);

int fsrm(wchar_t* path, int ip);
int fscp(wchar_t* src, wchar_t* dst, int ip);
int fsmv(wchar_t* src, wchar_t* dst);
int GetFilesCnt(wchar_t* path);
int EnumFiles(wchar_t* dname, ENUM_FILES_PROC enumproc, unsigned int param);
int TestFileName(wchar_t* wsname);
void CorFileName(wchar_t* wsname);
int mktree(wchar_t* path);
int fexists(wchar_t* fname);
#endif
