#include "..\\include\Lib_Clara.h" 
#include "..\\include\Dir.h" 
#include "header\string.h"
#include "header\iofunc.h"
//------------------------------------------------------------------
void images_add(wchar_t *path, wchar_t *name, int index);
//------------------------------------------------------------------
#define STANDBY_SOFTKEY 0
#define LEFT_PRESSED 1
#define MIDDLE_PRESSED 2
#define RIGHT_PRESSED 3
#define STANDBY 4
#define DESKTOP 5
#define NAVIGATION 6
#define INT_NUM 6

char *params[]={        "Softkey_text_left Color", 
                        "Softkey_text_left_pressed Color", 
                        "Softkey_text_middle Color", 
                        "Softkey_text_middle_pressed Color", 
                        "Softkey_text_right Color", 
                        "Softkey_text_right_pressed Color"};

char *params_wstr[]={   "Standby_softkey_image Source",
                        "Softkey_left_pressed_image Source",
                        "Softkey_middle_pressed_image Source",
                        "Softkey_right_pressed_image Source",
                        "Standby_image Source",
                        "Desktop_image Source",
                        "Background_image Source"};

int colors[INT_NUM];
//------------------------------------------------------------------
int xml_get_maxlen(char *param)
{
  int x=0;
  for (x=0;x<strlen(param);x++)
  {
    if (param[x]==0x22)
    {
      return x;
    }
  }
  return 0;
};

char *get_xml_param(char *xml, char *param)
{
  char *fname=strstr(xml, param);
  if (fname)
  { 
    while ((*fname++)!=0)
    {
      if (fname[0]==0x22)
      {
        *fname++;
        char *test=new char[xml_get_maxlen(fname) + 1];
        strncpy(test,fname,xml_get_maxlen(fname));
        test[xml_get_maxlen(fname)]='\0';
        return test;          
      }
    }
  }
  return 0;
};

int skin_execute_xml(wchar_t *folder, wchar_t *name)
{
  memset(colors,0,sizeof(int)*INT_NUM);
  int len=0;
  char *buf=file_read(folder, name, &len);
  int x;
  for (x=0;x<INT_NUM;x++)
  {
    if (x==2)
    {
      colors[x]=colors[0];
    }
    else if (x==3)
    {
      colors[x]=colors[1];
    }
    char *val=get_xml_param(buf, params[x]);
    if (val)
    {
      abc2ABC(val);
      int cl=h2i(val);
      cl=cl|0xFF000000;
      colors[x]=cl;
      delete(val);
    }
  }
  for (x=0;x<7;x++)
  {
    char *val=get_xml_param(buf, params_wstr[x]);
    if (val)
    {
      wchar_t *wval=new wchar_t[strlen(val)+1];
      str2wstr(wval,val);
      images_add(folder, wval, x);
      delete(wval);
      delete(val);
    }
  }
  delete(buf);
  return 0;
};
//------------------------------------------------------------------
