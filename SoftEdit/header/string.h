#ifndef __STRING_H
#define __STRING_H

int getint(char *line, int *pos,char end);
wchar_t *getwchr(char *line, int *pos, char end);
char *getchr(char *line, int *pos, char end);
char *getparam(char *buf, char *mask, char end, int fsize);
char *file_read(wchar_t *path, wchar_t *name, int *len);
#endif
