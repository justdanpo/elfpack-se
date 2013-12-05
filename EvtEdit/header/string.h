#ifndef STRING_H
  #define STRING_H

int getint(char *line, int *pos,char end);
char *getchr(char *line, int *pos, char end);
wchar_t *getwchr(char *line, int *pos, char end);
#endif
