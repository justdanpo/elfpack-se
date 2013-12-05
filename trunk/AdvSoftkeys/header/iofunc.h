#ifndef __HEADER_IOFUNC__
#define __HEADER_IOFUNC__
char *file_read(wchar_t *path, wchar_t *name, int *len);
void copyfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2);
int cmpfile(wchar_t *path, wchar_t *name, wchar_t *path2, wchar_t *name2);
#endif
