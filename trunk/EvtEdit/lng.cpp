#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\lng.h"
#include "header\string.h"

extern wchar_t *items[];

char *getparam2(char *buf, char *mask, char end, int fsize);
void Lang_Initialize();
void Lang_Remove();
void char2unicode(wchar_t *ws, char const *s, int len);

int fsize=0;

wchar_t *lng[]={
  L"����� ������",
  L"����� �����",
  L"�������", 
  L"���", 
  L"���� ������", 
  L"���� �����",
  L"��������",
  L"�����������", 
  L"GPS",
  L"�������",
  L"�����������",
  L"�������",
  L"�����",
  L"�������",
  L"�������",
  L"�������",
  L"�����������",
  L"������� ���",
  L"��������",
  L"��� ���",
  L"�� ������:",
  L"����� ������:",
  L"�� �����:",
  L"����� �����:", 
  L"������",
  L"��� �����������",
  L"�� ����",
  L"����",
  L"��� �������",
  L"���. �����",
  L"��������",
  L"����.",
  L"���.",
  L"���. (��� �����)",
  L"�������",
  L"����� �������",
  L"�������������",
  L"���� ����",
  L"�� ������",
  L"���",
  L"�������� �������",
  L"����� ������ �������",
  L"����� ����� ������� (���� ������ ����� '��������')",
  L"��������, ������������� �� ������ �����",
  L"��� ���������� ������� (���� ������ ����� '�� ����')",
  L"���� ������ (���� ������ ����� '����')",
  L"���� ����� (���� ������ ����� '����')",
  L"�������� �������, ������� ����� �������������� � ������������, � ����� �� ������� ����� ������ EvtDesk",
  L"����� �����������",
  L"GPS-�����. ������������ �������� ���� CID-LAC. ��������, '2028-4ABC'.",
  L"��������������",
  L"������ ������� ������������ ��� '������������' ������� ����� ����������� ���������� �������",
  L"���������",
  L"������� �����",
  L"�����. ����",
  L"������� ����",
  L"��������",
  L"����",
  L"�������",
  L"������� ��������",
  L"������� ����.���-��",
  L"����������� ���������",
  L"�����",
  L"���������",
  L"���",
  L"��������",
  L"���.����",
  L"������� ������",
  L"���.���� �� ����������",
  L"����",
  L"�����",
  L"���������",
  L"�� ����� ������",
  L"�� �������",
  0,0,0
};


bool isbetween(int f, int f1, int f2)
{
  if (f>=f1 && f<=f2)return true;
  return false;
};

bool isNative(void *f)
{
  return isbetween((int)f, (int)&ELF_BEGIN, ((int)&ELF_BEGIN)+fsize);
};


int lng_load(wchar_t *path, wchar_t *name)
{
  Lang_Initialize();
  int res=1;
  int file;
  FSTAT fst;
  if (fstat(path,name,&fst)==0)
  {
    res=0;
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      char * buf;
      char * param;
      buf=new char[fst.fsize+1];
      fread(file,buf,fst.fsize);
      int x=0;
      char endsymbol=';';
      
      while (1)
      {
        char pattern[128];
        sprintf(pattern,"lnged_%d)",x);
        if (param=getparam2(buf,pattern,endsymbol, fst.fsize))
        {
          
          if (lng[x])
          {
            if (!isNative(lng[x]))
            {
              delete(lng[x]);
            }
          }
          lng[x]=new wchar_t[strlen(param)+1];
          char2unicode(lng[x],param,strlen(param));
          delete(param);
        }
        else break;
        x++;
      }
      x=0;
      while (1)
      {
        char pattern[128];
        sprintf(pattern,"items_%d)",x);
        if (param=getparam2(buf,pattern,endsymbol, fst.fsize))
        {
          if (items[x])
          {
            
            if (!isNative(items[x]))
            {
              delete(items[x]);
            }
          }
          items[x]=new wchar_t[strlen(param)+1];
          char2unicode(items[x],param,strlen(param));
          delete(param);
        }
        else 
          goto L_away;
        x++;
      }
    L_away:
      ;
    }
    fclose(file);
  }
  return res;
};

void destroy_innative_lng()
{
  Lang_Remove();
  int x=0;
  while (1)
  {
    if (!items[x])break;
    if (!isNative(items[x]))
    {
      delete(items[x]);
    }
    x++;
  };
  x=0;
  while (1)
  {
    if (!lng[x])return;
    if (!isNative(lng[x]))
    {
      delete(lng[x]);
    }
    x++;
  };
};
extern wchar_t *extFolder;
extern wchar_t *intFolder;
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

wchar_t *checkfile(wchar_t*);
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

void Lang_Initialize()
{
  // ��������� ���� � ������ � FBUF
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
};

void Lang_Remove()
{
  if (fbuf)
  {
    delete(fbuf);
    fbuf=0;
  }
};
