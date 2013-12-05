#ifndef _FILE_OP_H
#define _FILE_OP_H

int isdir(const wchar_t *name);
int cd(int tab, const wchar_t *dname, int drv);
int cdsys(wchar_t *dname);
void _NewDir(wchar_t *wsname);
void S_Delit(void);
void S_Paste(void);
int M_MoveCopy(FILEINF *file, int param);

extern FN_LIST buffer;




#endif

