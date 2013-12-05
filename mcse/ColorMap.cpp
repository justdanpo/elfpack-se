#include "inc\mc.h"
#include "inc\ColorMap.h"

const wchar_t mccs_deffile[]=L"default.mccs";
const wchar_t mccs_ext[]=L"mccs";

int Colors[clMAX+1];
const int ColorIndex[clMAX+1] = {
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x00,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0x80,0x80,0xFF,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  
  COLOR_RGBA(0x00,0x00,0x00,0xFF),
  COLOR_RGBA(0x00,0x00,0x00,0xFF),
  COLOR_RGBA(0x80,0x80,0x8C,0xFF),
  COLOR_RGBA(0x4a,0x4A,0x4A,0xFF),
  COLOR_RGBA(0x80,0x00,0x00,0xFF),
  COLOR_RGBA(0x80,0x00,0x00,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0x80,0x80,0xFF,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0x00,0x00,0x80,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  COLOR_RGBA(0xFF,0xFF,0xFF,0xFF),
  
  COLOR_RGBA(0xC0,0xC0,0xC0,0xFF),
  COLOR_RGBA(0x00,0x00,0x00,0xFF)  
};

void InitCS()
{
  for(int ii=0;ii<=clMAX;ii++)
    Colors[ii]=ColorIndex[ii];
  if (CONFIG_LOAD_CS)LoadCS(NULL);
}

void SaveCS(wchar_t* filename)
{
  wchar_t *fn;
  if (filename)
  {
    fn=filename;
  }
  else
  {
    if (!w_chdir(mcpath))
      fn=(wchar_t *)mccs_deffile;
  }
  int f;
  if (fn && (f = w_fopen(fn, WA_Read+WA_Write+WA_Create+WA_Truncate, 0x1FF, NULL)) >=0)
  {
    MCCS_hdr hdr;
    hdr.sig=mccs_sig;
    hdr.ver=2;
    hdr.size=mccs_size;
    hdr.offset=mccs_offset;
    
    w_fwrite(f,&hdr,sizeof(hdr));
    w_fwrite(f,&Colors,sizeof(Colors));
    w_fclose(f);
  }
}

int LoadCS(wchar_t* filename)
{
  int res = 0;
  wchar_t *fn;
  if (filename)
  {
    fn=filename;
  }
  else
  {
    if (!w_chdir(mcpath))
      fn=(wchar_t *)mccs_deffile;
  }
  int f;
  if (fn && (f = w_fopen(fn, WA_Read, 0x1FF, NULL)) >=0)
  {
    MCCS_hdr hdr;
    if (w_fread(f, &hdr, sizeof(hdr))==sizeof(hdr))
    {
      if ((hdr.sig==mccs_sig) && (hdr.size>=mccs_size))
      {
        w_lseek(f,hdr.offset,WSEEK_SET);
        if (hdr.ver==2)
          res = w_fread(f, &Colors, sizeof(Colors))==sizeof(Colors);
        else if (hdr.ver==1)
        {
          char v1[sizeof(Colors)];
          char *t=v1;
          res = w_fread(f, v1, sizeof(Colors))==sizeof(Colors);
          for (int i=0; i<(clMAX+1); i++)
          {
            unsigned int color=0;
            color=COLOR_RGBA(*t,*(t+1),*(t+2),(*(t+3)*0xFF/0x64));
            Colors[i]=color;
            t+=4;
          }
        }
      }
    }
    w_fclose(f);
  }
  return res;
}
