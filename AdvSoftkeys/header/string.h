#ifndef __HEADER_STRING__
  #define __HEADER_STRING__

  int pos2bs(int n);
  int oct2int(const char *oct);
  void abc2ABC(char *ws);
  int h2i(char *h);
  char *getparam(char *buf, char *mask, char end, int fsize);
  wchar_t *getwchr(char *line, int *pos, char end);
  char *getchr(char *line, int *pos, char end);
  int getint(char *line, int *pos,char end);
#endif
