#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\krnl.h"
#include "inc\gui.h"

const char _ss[] = "%s: %s";
const wchar_t _lsls[] = L"%ls: %ls";
const wchar_t _ls_ls[] = L"%ls/%ls";
const wchar_t _ls_stars[] = L"%ls/*";
const wchar_t str_empty[] = L"";
const wchar_t _d[] = L"%d";
const wchar_t _ls_i_ls[] = L"%ls(%i).%ls";

char* strtolower(char* src, char* dst, int sz)
{
  int sl = strlen(src)+1;
  int len = sz==-1?sl:sz;
  if (len>sl) len=sl;
  
  for(int ii=0; ii<len-1; ii++)
  {
    int ch = src[ii];
    if (ch>='A' && ch<='Z') ch=ch-'A'+'a';
    dst[ii]=ch;
  }
  dst[len-1]=0;
  return dst;
}

// Замена символов
char* strreplace(char* s, char cprev, char cnew)
{
	for (int i = 0; s[i]; s++)
		if (s[i] == cprev) s[i] = cnew;
	return s;
}


wchar_t* wstrreplace(wchar_t* s,  wchar_t cprev, wchar_t cnew)
{
	for (int i = 0; s[i]; s++)
		if (s[i] == cprev) s[i] = cnew;
	return s;
}
int isNumericWstr(wchar_t *s)
{
	// Внимание! пустую строку тоже считаем числовой
	for (int i = 0; s[i]; i++)
		if (s[i] < '0' || s[i] > '9') return 0;

	return 1;
}

wchar_t wcharlow(wchar_t wc)
{
	if (wc>=0x0041 && wc<=0x005a) return wc+(0x0061-0x0041); //A-Z
	if (wc>=0x0410 && wc<=0x042f) return wc+(0x0430-0x0410); //А-Я
	if (wc>=0x0400 && wc<=0x040f) return wc+(0x0450-0x0400);

	if (wc>=0x00c0 && wc<=0x00de && wc!=0x00d7) return wc+(0x00e0-0x00c0);

	if (wc>=0x0100 && wc<=0x0136 && ~(wc & 1)) return wc+1;
	if (wc>=0x0139 && wc<=0x0147 &&  (wc & 1)) return wc+1;
	if (wc>=0x014a && wc<=0x0176 && ~(wc & 1)) return wc+1;
	if (wc>=0x0179 && wc<=0x017d &&  (wc & 1)) return wc+1;
	if (wc>=0x01e4 && wc<=0x01ea && ~(wc & 1)) return wc+1;

	if (wc>=0x01fa && wc<=0x01fe && ~(wc & 1)) return wc+1;

	if (wc>=0x0490 && wc<=0x04e8 && ~(wc & 1)) return wc+1;

	if (wc==0x01a0 || wc==0x01af || wc==0x017d || 
		wc==0x01ee || wc==0x0218 || wc==0x021a || wc==0x0228) return wc+1;

	if (wc==0x0178) return 0x00ff;
	if (wc==0x018f) return 0x0259;
	if (wc==0x01b7) return 0x0292;
	return wc;
}


const wchar_t *strpbrkw(const wchar_t *s1, const wchar_t *w1)
{
  int c;
  while((c=*s1))
  {
    const wchar_t *w2=w1;
    int d;
    while((d=*w2++))
    {
      if(d==c) return s1;      
    }   
    s1++;    
  }
  return NULL;  
}

int match(wchar_t *pattern,wchar_t *string)
{
  for (;; ++string)
  {
    wchar_t stringc=wcharlow(*string);
    wchar_t patternc=wcharlow(*pattern++);
    switch (patternc)
    {
      case 0:
        return(stringc==0);
      case '?':
        if (stringc == 0)
          return(0);
        break;
      case '*':
        if (*pattern==0)
          return(1);
        if (*pattern=='.')
        {
          if (pattern[1]=='*' && pattern[2]==0)
            return(1);
          wchar_t *dot=wstrchr(string,'.');
          if (pattern[1]==0)
            return (dot==NULL || dot[1]==0);
          if (dot!=NULL)
          {
            string=dot;
            if (strpbrkw(pattern,L"*?")==NULL && wstrchr(string+1,'.')==NULL)
              return(wstrcmpi(pattern+1,string+1)==0);
          }
        }

        while (*string)
          if (match(pattern,string++))
            return(1);
        return(0);
      default:
        if (patternc != stringc)
          if (patternc=='.' && stringc==0)
            return(match(pattern,string));
          else
            return(0);
        break;
    }
  }
}

wchar_t *sz2s(unsigned int size, wchar_t *buf)
{
  float s = size;
  wchar_t *pref;
  char cbuf[64];
  pref=muitxt(ind_byte);
  if (s >= 1024)
  {
    s /= 1024;
    pref=muitxt(ind_kbyte);
    if (s >= 1024)
    {
      s /= 1024;
      pref=muitxt(ind_mbyte);
    }
    sprintf(cbuf, "%.2f", s);
  }
  else
    sprintf(cbuf, "%d", size);
  snwprintf(buf, 63, L"%s %ls", cbuf, pref);
  return buf;
}


wchar_t *unixt2s(unsigned int time, wchar_t *buf)
{
  DATETIME dt;
  unixtime2datetime(time,&dt);
  DATE *d=&dt.date;
  TIME *t=&dt.time;
  snwprintf(buf,63,L"%.2d.%.2d.%.2d %.2d:%.2d",d->day,d->mon,d->year,t->hour,t->min);
  return buf;
}

#define MAX_TXT_W		(ITM_X2-ITM_B-TXT_X-2)
int getLVC(wchar_t *wsbody, int font)
{
  int old_font=SetFont(font);
  int width=0;
  int i=0;
  int f=0;
  TEXTID str=TextID_Create(wsbody,ENC_UCS2,TEXTID_ANY_LEN);
  while(*wsbody++)
  {
    width=Disp_GetStrIdWidth(str,i);
    if (width>=MAX_TXT_W) {f=1; break;}
    i++;
  }
  TextID_Destroy(str);
  SetFont(old_font);
  return (f?i:0);
}

void cutname(wchar_t *fname, wchar_t *sname, int len)
{
  sname[len]=0;
  for(int ii=0;ii<len;ii++)
  {
    sname[ii]=(ii>=len-2)?'.':fname[ii];
  }
  //  sname->wsbody[len+1]=0;
}


// Аналог strcmp, но без чувствительности к регистру
int stricmp(char* str1, char* str2)
{
	return strnicmp(str1, str2, 1000); // тысячи хватит на сравнение, надеюсь
}


int strnicmp(char* str1, char* str2, int count)
{
	for(int i=0; i<count; i++)
	{
		unsigned char c1 = str1[i];
		unsigned char c2 = str2[i];

		if (c1 >= 'A' && c1 <= 'Z') c1 = c1 - 'A' + 'a';
		if (c2 >= 'A' && c2 <= 'Z') c2 = c2 - 'A' + 'a';
		if (c1 < c2) return -1;
		if (c1 > c2) return +1;
		if (c1 == 0) return 0;
	}
	return 0;
}

int EnumIni(int local, const wchar_t *ininame, INIPROC proc)
{
  int res=0;
  char *buf;
  char *cfg;
  int ch;
  int p=0;
  char name[MAX_INIBUF];
  char value[MAX_INIBUF];
  const wchar_t* fn=NULL;
  unsigned int size_cfg;
  fn=ininame;
  if (local)
  {
    w_chdir(mcpath);
  }
  int f;
  int is_utf8=0;
  if (fn && (f = w_fopen(fn, WA_Read, 0x1FF, NULL)) >=0)
  {
    size_cfg = w_lseek(f,0,WSEEK_END);
    w_lseek(f, 0, WSEEK_SET);
    cfg = buf = new char[size_cfg + 1];
    if (cfg)
    {
      size_cfg = w_fread(f, buf, size_cfg);
      buf[size_cfg] = 0;
      if (size_cfg>3)
      {
        if (buf[0]==0xEF && buf[1]==0xBB && buf[2]==0xBF) 
        {
          is_utf8=1;
          buf+=3;
        }
      }
      do
      {
        // Камент
        if (*buf == ';')
        {
          while( ((ch = *buf++) >= 32));
          if (!ch) break;
          else continue;
        }
        // Название
        p=0;
        while( (ch = *buf++) != '=')
        {
          if (ch < 32) break;
          if (p < MAX_INIBUF-1) name[p++] = ch;
        }
        if (ch < 32) continue;
        name[p]=0;
        
        // Значение
        p=0;
        while( (ch = *buf++) >= 32 )
        {
          if (p < MAX_INIBUF-1) value[p++] = ch;
        }
        value[p]=0;
        if (proc)
          proc(name, value, is_utf8);
      }
      while (ch);
    }
    delete (cfg);
    w_fclose(f);
    res = 1;
  }
  return res;
}

wchar_t *attr2s(int attr, wchar_t *buf)
{
  *buf++=attr & 0x124?L'r':L'-';
  *buf++=attr & 0x92?L'w':L'-';
  *buf++=attr & 0x49?L'x':L'-';
  *buf++=attr & FA_DIRECTORY?L'd':L'-';
  *buf = 0;
  return buf;
}
