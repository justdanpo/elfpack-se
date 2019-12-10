#ifndef _MAIN_H_
#define _MAIN_H_

void elf_exit(void);

typedef struct
{
  BOOK * book;
}MSG;


typedef struct _MYBOOK : BOOK
{
  IConnManager * pIConnManager;
  ICBTestConnManager * pICBTestConnManager;
  ISocket * pISocket;
  ICBTestSocket * pICBTestSocket;
}TestSocketBook;

#endif
