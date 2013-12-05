#ifndef _SLIB_H
  #define _SLIB_H

typedef void (*INIPROC) (char* name, char* value, int is_utf);
#define MAX_INIBUF	256

extern const wchar_t _ls_ls[];
extern const wchar_t _lsls[];
extern const wchar_t _ls_stars[];
extern const wchar_t str_empty[];
extern const wchar_t _d[];
extern const char _ss[];
extern const wchar_t _ls_i_ls[];

char* strtolower(char* src, char* dst, int sz);
char* strreplace(char* s, char cprev, char cnew);
wchar_t* wstrreplace(wchar_t* s,  wchar_t cprev, wchar_t cnew);
wchar_t wcharlow(wchar_t wc);
wchar_t *sz2s(unsigned int size, wchar_t *buf);
wchar_t *unixt2s(unsigned int time, wchar_t *buf);
wchar_t *attr2s(int attr, wchar_t *buf);
int getLVC(wchar_t *wsbody, int font);
void cutname(wchar_t *fname, wchar_t *sname, int len);
int match(wchar_t *pattern,wchar_t *string);

int isNumericWstr(wchar_t *s);
int stricmp(char* str1, char* str2);
int strnicmp(char* str1, char* str2, int count);
int EnumIni(int local, const wchar_t *ininame, INIPROC proc);
#endif
