#ifndef __HEADER_UNTAR__
  #define __HEADER_UNTAR__

  #define HEADER_SIZE 0x200

  typedef struct
  {
    char fname[0x64+1];
    int size;
  }TAR_HEADER;

  int tar_uncompress(wchar_t *path, wchar_t *name, wchar_t *folder);
#endif
