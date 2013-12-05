#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "header\wdisplay.h"

wchar_t *checkfile(wchar_t *name)
{
  FSTAT fst;
  if (fstat(extFolder,name,&fst)==0)
  {
    return extFolder;
  }
  else if (fstat(intFolder,name,&fst)==0)
  {
    return intFolder;
  }
  return intFolder;
};

char *fbuf=0;

void char2unicode(wchar_t *ws, char const *s, int len) 
{
  if (fbuf)
  {
    int c;
    char * buf=fbuf;
    while((c = *s++) && ((len--) > 0))   
    {
      if (c == 0x7E) c = 0x0A;
      if (fbuf) c = (buf[2 * c] << 8)  | buf[2 * c + 1];
      *ws++ = c;
  
    }
    *ws=0;
  }
  else
  {
    str2wstr(ws, s);
  }
};

char *Lang_Initialize()
{
  // Открываем лэнг и читаем в FBUF
  fbuf=0;
  int file;
  FSTAT fst;
  if(fstat(checkfile(L"encoding.bin"), L"encoding.bin", &fst) == 0)
  {
    if( ((file = _fopen(checkfile(L"encoding.bin"), L"encoding.bin", FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0)) >= 0) && fst.fsize == 512 )
    {
      fbuf = new char[512];
      fread(file, fbuf, 512);
    }
    fclose(file);
  }
  return fbuf;
};

void Lang_Remove()
{
  if (fbuf)
  {
    delete(fbuf);
    fbuf=0;
  }
};
