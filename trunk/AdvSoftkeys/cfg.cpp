#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\structs.h"
#include "header\string.h"
#include "header\iofunc.h"
int set_ui(char *buf, int fsize);

LIST *csofts=0;
#define SID_NULL 0x6FFFFFFF
#define DELETE(a) if (a) delete(a);a=0
#define F_READ 0x001
#define F_RIGHTS 0x180
#define F_WRITE 0x204
#define F_APPEND 0x108

void addline(char *line)
{
  ITEM *it=new ITEM;
  memset(it,0,sizeof(ITEM));
  //--------------------
  char between=0xFF;
  int x=0;
  //--------------------
  it->name=getwchr(line,&x,between);
  //--------------------
  it->strids[0]=SID_NULL;
  it->strids[1]=SID_NULL;
  it->strids[2]=SID_NULL;
  //--------------------
  wchar_t *str=getwchr(line,&x,between);
  if (str)it->strids[0]=TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN);
  delete(str);
  //--------------------
  str=getwchr(line,&x,between);
  if (str)it->strids[1]=TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN);
  delete(str);
  //--------------------
  str=getwchr(line,&x,between);
  if (str)it->strids[2]=TextID_Create(str,ENC_UCS2,TEXTID_ANY_LEN);
  delete(str);
  //--------------------
  it->style=getint(line,&x,':');
  //--------------------
  int howmany=getint(line,&x,':');
  //--------------------
  int j;
  for (j=0;j<howmany;j++)
  {
    KEY *key=new KEY;
    key->oldkey=getint(line,&x,'-'); 
    key->newkey=getint(line,&x,':');
    if (!it->keys)it->keys=List_Create();
    List_InsertLast(it->keys, key);
  }
  //--------------------
  List_InsertLast(csofts, it);
};

int csofts_fill(wchar_t *path, wchar_t *name)
{
  int fsize=0;
  char *buf=file_read(path, name, &fsize);
  if (buf)
  {
    char *param=0;
    int x=0;
    if (!csofts)csofts=List_Create();
    while (1)
    {
      char pattern[128];
      sprintf(pattern,"%d)",x);
      if (param=getparam(buf,pattern,0xFE, fsize))
      {
        addline(param);
        delete(param);
      }
      else
      {
        break;
      }
      x++;
    }
    set_ui(buf,fsize);
    delete(buf);
    return 0;
  }
  return -1;
};
//------------------------------------------------------------
void csofts_destroy()
{
  if (csofts)
  {
    while (csofts->FirstFree)
    {
      ITEM *it=(ITEM*)List_RemoveAt(csofts,0);
      DELETE(it->name);
      if (it->strids[0]!=SID_NULL)TextID_Destroy(it->strids[0]);
      if (it->strids[1]!=SID_NULL)TextID_Destroy(it->strids[1]);
      if (it->strids[2]!=SID_NULL)TextID_Destroy(it->strids[2]);
      if (it->keys)
      {
        while (it->keys->FirstFree)
        {
          KEY *k=(KEY*)List_RemoveAt(it->keys,0);
          DELETE(k);
        }
        List_Destroy(it->keys);
        it->keys=0;
      }
      DELETE(it);
    }
    List_Destroy(csofts);
    csofts=0;
  }
};
//----------------------------------------------
void csofts_create_empty(wchar_t *path, wchar_t *name)
{
  int file;
  char cr=0x0D;
  char lf=0x0A;
  char endstr=0xFE;
  char between=0xFF;
  if ((file=_fopen(path,name,F_WRITE,F_RIGHTS,0))>=0)
  {
    //0):
    fwrite(file,"0): ",4);
    fwrite(file,L"DEFAULT",7*2);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,"1:4:2-3:3-4:8-2:4-8:;",strlen("1:4:2-3:3-4:8-2:4-8:;"));
    fwrite(file,&cr,1);
    fwrite(file,&lf,1);
    //1):
    fwrite(file,"1): ",4);
    fwrite(file,L"StandbyBook",11*2);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,&between,1);
    fwrite(file,"1:0:",strlen("1:0:"));
    fwrite(file,&endstr,1);
    fwrite(file,&cr,1);
    fwrite(file,&lf,1);
  }
  fclose(file);
};
