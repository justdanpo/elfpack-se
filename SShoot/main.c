#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

#pragma pack(1)
typedef struct
{
  u16 ID;
  int size;
  u16 zero;
  u16 zero1;
  int dataoffset;
  int sizeofheader;
  int width;
  int height;
  u16 planes;
  u16 bpp;
  int compression;
  int sizeofimage;
  int hresolution;
  int vresolution;
  int colors;
  int important;
}BMP_HDR;
#pragma pack()

int xsize;
int ysize;
int KeyActiv=0x20;
int KeyPressMode=0x03;

BMP_HDR *hdr;
wchar_t * path=0;

BOOK * SSBook;

int TerminateElf(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
}

typedef struct
{
  BOOK * book;
}MSG;

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("ScreenShooter v1.2\n\n(c)2007-2008 Hussein"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}

const PAGE_MSG SS_PageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  0,0
};

PAGE_DESC base_page ={"SS_BasePage",0,SS_PageEvents};



void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int h2i(char * h)
{
  char c;
  int res=0;
  while((*h++)!='x');
  do
  {
    c=*h++;
    if (c>0x60) c-=0x20; else c-=0x30;
    if (c>9) c-=7;
    res<<=4;
    res|=c;
  }
  while(*h!=';');
  return(res);
}

void Snap(void)
{
  int f;
  int x;
  int y;
  u16 cl=0;
  char *bmp =new char[((xsize*ysize*3)+54)];
  char *bp=bmp+54;
  wchar_t * fname =new wchar_t[50];
  wchar_t * fpath =new wchar_t[wstrlen(path)+20];
  DATETIME dt;
  int platform = GetChipID()&CHIPID_MASK;
  char * BUFF_p;
  switch (platform)
  {
  case CHIPID_DB2010:
    {
      BUFF_p=(char*)(**(int**)get_VBUFFER());
    }
    break;
  case CHIPID_DB2020:
    {
      SURFACE * mx =(SURFACE*)(*get_Surfaces());
      while ((mx->RefCount!=1)&&(!mx->Display)) mx=mx->next;
      BUFF_p=(char*)mx->Buffer;
    }
    break;
  default:
    return;
  }

  memcpy(bmp,hdr,54);
  y=ysize;
  do
  {

    char * p=BUFF_p+(2*xsize*(y-1));
    x=0;
    do
    {
      if (platform==CHIPID_DB2020)
      {
        cl=p[0]+(p[1]<<8);
      }
      else
      {
        cl=p[1]+(p[0]<<8);
      }

      p+=2;
      *bp++=(cl&0x001F)<<3;
      *bp++=(cl&0x07e0)>>3;
      *bp++=(cl&0xF800)>>8;
    }
    while (++x<xsize);
  }
  while(--y);

  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  wstrcpy(fpath,path);
  snwprintf(fname,25,L"/%02d-%02d-%02d",dt.date.day,dt.date.mon,dt.date.year);
  wstrcat(fpath,fname);
  snwprintf(fname,25,L"scr%02d-%02d-%02d.bmp",dt.time.hour,dt.time.min,dt.time.sec);

  if ((f=_fopen(fpath,fname,FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    fwrite(f,bmp,(xsize*ysize*3)+54);
    fclose(f);
  }

  delete(bmp);
  delete(fname);
  delete(fpath);

}

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
  if (key==KeyActiv)
  {
    if (mode==KeyPressMode)
    {
      Snap();
      return(-1);
    } else {
      return(0);
    }  
  }
  return(0);
}

void CreateHeader(int w, int h)
{
  hdr = new BMP_HDR;
  memset(hdr,0,sizeof(BMP_HDR));
  hdr->ID=0x4D42;
  hdr->size=w*h*3+52;
  hdr->dataoffset=0x36;
  hdr->sizeofheader=0x28;
  hdr->width=w;
  hdr->height=h;
  hdr->planes=1;
  hdr->bpp=0x18;
  hdr->sizeofimage=w*h*3;
}

void onCloseSSBook(BOOK * book)
{
  if (book)
  {
    ModifyKeyHook(NewKey,KEY_HOOK_REMOVE,NULL);
    delete(hdr);
    delete(path);
    SUBPROC(elf_exit);
  }
}

BOOK * CreateSSBook()
{
  SSBook= new BOOK;
  CreateBook(SSBook,onCloseSSBook,&base_page,"ScreenShooter",-1,0);
  return(SSBook);
}
// берем значения из ини-файла
char * get_ini_key()
{
  int file;
  char * buf=0;
  FSTAT _fstat;
  if (fstat(GetDir(DIR_INI),L"sshoot.ini",&_fstat)==0)
  {
    if ((file=_fopen(GetDir(DIR_INI),L"sshoot.ini",FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      buf=new char[_fstat.fsize+1];
      buf[fread(file,buf,_fstat.fsize)]=0;
      char * param;
      if (param=manifest_GetParam(buf,"[START_KEY]",0))
      {
        KeyActiv=h2i(param);
        mfree(param);
      }
      if (param=manifest_GetParam(buf,"[KEY_MODE]",0))
      {
        KeyPressMode=h2i(param);
        mfree(param);
      }
      if (param=manifest_GetParam(buf,"[PATH]",0))
      {
        path=new wchar_t[strlen(param)+1];
        str2wstr(path,param);
        mfree(param);
      }
      delete(buf);
      fclose(file);
    }
  }

  if (!path)
  {
    path = new wchar_t[wstrlen(GetDir(DIR_IMAGE | MEM_INTERNAL))+wstrlen(L"/SnapShots")+1];
    wstrcpy(path,GetDir(DIR_IMAGE | MEM_INTERNAL));
    wstrcat(path,L"/SnapShots");  return(buf);
  }
  return(0);
}


int main (void)
{
  CreateSSBook();
  get_ini_key();
  xsize=Display_GetWidth(0);
  ysize=Display_GetHeight(0);
  CreateHeader(xsize,ysize);
  ModifyKeyHook(NewKey,KEY_HOOK_ADD,NULL);
  return(0);
}
