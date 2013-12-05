#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\structs.h"
#include "header\presets.h"
#define SID_NULL 0x6FFFFFFF

bool pressed[3]={false,false,false};

extern RECT rect;
extern ITEM *lastitem;
extern DISP_OBJ *soft;
extern bool works;
extern LIST *csofts;
extern int DB;

ITEM *getbyname(wchar_t *name);
void getitem(BOOK *bk);
int isImageViewer(BOOK *bk);
void __getitem(BOOK *bk);
void save();
int isvisible(LIST *lst, int index);
bool isInIronPatch(int act);

int onkey(int key)
{
  if (works)
  {
    BOOK *bk=Display_GetTopBook(0);
    if (BookObj_GetDisplayOrientation(bk)==1 && DB==2010)return key;
    if (isImageViewer(bk))return key;
    __getitem(bk);
    if (lastitem)
    {
      if (lastitem->type==T_3SK_LAYOUT)return key;
      if (lastitem->style==1)
      {
        if (lastitem->type==T_YESNO && key==KEY_RIGHT_SOFT)
        {
          return KEY_RIGHT_SOFT;
        }
        if (lastitem->type==T_YESNO && key==KEY_LEFT_SOFT)
        {
          return 0xFF;
        } 
        if (lastitem->type==T_YESNO && key==KEY_DEL)
        {
          return KEY_ESC;
        }
      }
      if (lastitem->keys && csofts)
      {
        int y=0;
        while(y<lastitem->keys->FirstFree)
        {
          KEY *k=(KEY*)lastitem->keys->listdata[y++];
          if (k->oldkey==key)
          {
            return k->newkey;
          }
        }
      }
    }
  }
  return 0;
};

int NewKey(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
  if (mode==KBD_SHORT_PRESS)
  {
    if (key==KEY_LEFT_SOFT)
    {
      pressed[0]=true;
      pressed[1]=false;
      pressed[2]=false;
      DispObject_InvalidateRect(soft,&rect);
      DispObject_SetRefreshTimer(soft,100);
    }
    else if (key==KEY_ENTER)
    {
      pressed[0]=false;
      pressed[1]=true;
      pressed[2]=false;
      DispObject_InvalidateRect(soft,&rect);
      DispObject_SetRefreshTimer(soft,100);
    }
    else if (key==KEY_RIGHT_SOFT)
    {
      pressed[0]=false;
      pressed[1]=false;
      pressed[2]=true;
      DispObject_InvalidateRect(soft,&rect);
      DispObject_SetRefreshTimer(soft,100);
    }
    else
    {
      pressed[0]=false;
      pressed[1]=false;
      pressed[2]=false;
    }
  }
#ifdef mDEBUG
  if (key==KEY_POWER)
  {
    if (mode==KBD_SHORT_PRESS)
    {
      save();
      BOOK*bk=Display_GetTopBook(0);
      wchar_t ws[100];
      if (strcmp(bk->xbook->name,"CUIDisplayableBook")==0)
      {
        TextID_GetWString(bk->xbook->app_session->name,ws,100);
      }
      else
      {
        str2wstr(ws,bk->xbook->name);
      }
      MessageBox(EMPTY_TEXTID,TextID_CreateIntegerID(lastitem->type),NOIMAGE,1,0,0);
      MessageBox(EMPTY_TEXTID,TextID_Create(ws,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
      return -1;
    }
  }
#endif
  return (onkey(key));
};

char *unicode2win1251(char *s, wchar_t *ws, int len)
{
  char *d=s;
  int c;
  while((c=*ws++)&&((len--)>0))
  {
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    *s++=c;
  }
  *s=0;
  return(d);
};

typedef struct
{
  char unk[200];
}JUST_BYTEARRAY;

void save()
{
  BOOK *bk=Display_GetTopBook(0);
  LIST *lst = DispObject_SoftKeys_GetList(Display_GetFocusedDispObject(0),bk, 0);
  int file;
  FSTAT fst;
  char cr=0x0D;
  char lf=0x0A;
  char idle=';';
  if ((file=_fopen(GetDir(MEM_EXTERNAL+DIR_OTHER),L"Softs.txt",FSX_O_RDWR|FSX_O_TRUNC,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
  {
    if (fstat(GetDir(MEM_EXTERNAL+DIR_OTHER),L"Softs.txt",&fst)==0)
    {
      JUST_BYTEARRAY *xls=(JUST_BYTEARRAY*)bk;//DispObject_SoftKeys_GetParams(Display_GetFocusedDispObject(0));
      int x;
      for (x=0;x<199;x++)
      {
        char hz[100];
        sprintf(hz,"%02x:",xls->unk[x]);
        
        fwrite(file,hz,strlen(hz));
        if ((x+1)>=4)
        {
          if ((x+1)%4==0)
          {
            fwrite(file,";",1);
            fwrite(file,&cr,1);
            fwrite(file,&lf,1);
          }
        }
      }
      fwrite(file,";",1);
      fwrite(file,&cr,1);
      fwrite(file,&lf,1);
      for (x=0;x<lst->FirstFree;x++)
      {
        SOFTKEY * sk = (SOFTKEY*)List_Get(lst,x);
        char pattern[1024];
        
        wchar_t name[128];
        char name2[128];
        if (sk->text!=EMPTY_TEXTID)
        {
          TextID_GetWString(sk->text,name,100);
          unicode2win1251(name2, name, 100);
        }
        else
        {
          strcpy(name2,"Not Defined");
        }
        char name3[128];
        char name4[128];
        char name5[128];
        if (sk->text_mini!=SID_NULL)
        {
          TextID_GetWString(sk->text_mini, name,100);
          unicode2win1251(name3, name, 100);
        }
        else
        {
          strcpy(name3,"Not Defined");
        }
        if (sk->text_disabled!=SID_NULL)
        {
          TextID_GetWString(sk->text_disabled, name,100);
          unicode2win1251(name4, name, 100);
        }
        else
        {
          strcpy(name4,"Not Defined");
        }
        if (sk->text_help!=SID_NULL)
        {
          TextID_GetWString(sk->text_help, name,100);
          unicode2win1251(name5, name, 100);
        }
        else
        {
          strcpy(name5,"Not Defined");
        }
        sprintf(pattern,"%x-%x:%x:%x) %s:%s:%s:%s: %d:%d::%X:0x%X:%X:%X",sk->action,sk->unk, sk->unk2, sk->unk3, name2, name3, name4, name5, sk->enable, sk->visible, sk->parent_action, (int)sk->proc, (int)sk->list, sk->unk4);
        fwrite(file,pattern,strlen(pattern));
        fwrite(file,&idle,1);
        fwrite(file,&cr,1);
        fwrite(file,&lf,1);
      }
      fwrite(file,&cr,1);
      fwrite(file,&lf,1);
      fwrite(file,"Visible:",8);
      
      fwrite(file,&cr,1);
      fwrite(file,&lf,1);
      for (x=0;x<lst->FirstFree;x++)
      {
        SOFTKEY * sk = (SOFTKEY*)List_Get(lst,x);
        if (sk->visible && sk->action!=ACTION_BACK && isvisible(lst,x) && isInIronPatch(sk->action)!=true && sk->text!=EMPTY_TEXTID && sk->parent_action==0xFFFF)
        {
          char pattern[1024];
          
          wchar_t name[128];
          char name2[128];
          if (sk->text!=EMPTY_TEXTID)
          {
            TextID_GetWString(sk->text,name,100);
            unicode2win1251(name2, name, 100);
          }
          else
          {
            strcpy(name2,"Not Defined");
          }
          char name3[128];
          char name4[128];
          char name5[128];
          if (sk->text_mini!=SID_NULL)
          {
            TextID_GetWString(sk->text_mini, name,100);
            unicode2win1251(name3, name, 100);
          }
          else
          {
            strcpy(name3,"Not Defined");
          }
          if (sk->text_disabled!=SID_NULL)
          {
            TextID_GetWString(sk->text_disabled, name,100);
            unicode2win1251(name4, name, 100);
          }
          else
          {
            strcpy(name4,"Not Defined");
          }
          if (sk->text_help!=SID_NULL)
          {
            TextID_GetWString(sk->text_help, name,100);
            unicode2win1251(name5, name, 100);
          }
          else
          {
            strcpy(name5,"Not Defined");
          }
          sprintf(pattern,"%x-%x-%x) %s:%s:%s:%s: %d:%d::%X:0x%X:%X:%d:%X:%X:%X:%X:%X:%X",sk->action,sk->book,sk->DISP_OBJ1,name2, name3, name4, name5, sk->enable, sk->visible, sk->parent_action, (int)sk->proc, (int)sk->list, sk->unk, sk->unk6[0], sk->unk6[1],sk->unk6[2],sk->unk6[3], sk->unk6[4], sk->unk6[5]);
          fwrite(file,pattern,strlen(pattern));
          fwrite(file,&idle,1);
          fwrite(file,&cr,1);
          fwrite(file,&lf,1);
        }
      }
    }
  }
  fclose(file);
};

