#ifndef UNTAR_H
  #define UNTAR_H

  #define HEADER_SIZE 0x200

  typedef struct
  {
    char fname[0x64+1];
    int size;
    int type;
  }TAR_HEADER;

  int tar_uncompress(wchar_t *path, wchar_t *name);
#endif
