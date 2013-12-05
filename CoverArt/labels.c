#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "config_data.h"
#include "structs.h"
#include "Externals.h"

LABEL labels[NUM];

int GetLabelSize(wchar_t *lbl, int FONT)
{
  // Получаем размер надписи функцией тела
  SetFont(FONT);
  int str=TextID_Create(lbl,ENC_UCS2,TEXTID_ANY_LEN);
  int res = Disp_GetStrIdWidth(str, wstrlen(lbl));
  TextID_Destroy(str);
  return res;
};

void caprintf(wchar_t *prt, wchar_t *where)
{
  // Функция записывает в *where переделанную строку prt
  wchar_t returnvalue[256];
  int x;
  int nox=0;
  for (x=0;x<wstrlen(prt);x++)
  {
    if (prt[x]!=L'$')
    {
      returnvalue[nox]=prt[x];
      nox++;
    }
    else
    {
      if (prt[x+1]==L't' || prt[x+1]==L'0')
      {
	wstrcpy(&returnvalue[nox],nowmusic.meta.Title);
	nox=nox+wstrlen(nowmusic.meta.Title);
      }
      else if (prt[x+1]==L'a' || prt[x+1]==L'1')
      {
	wstrcpy(&returnvalue[nox],nowmusic.meta.Album);
	nox=nox+wstrlen(nowmusic.meta.Album);
      }
      else if (prt[x+1]==L'c' || prt[x+1]==L'2')
      {
	wstrcpy(&returnvalue[nox],nowmusic.meta.Artist);
	nox=nox+wstrlen(nowmusic.meta.Artist);
      }
      else if (prt[x+1]==L'g' || prt[x+1]==L'3')
      {
	wstrcpy(&returnvalue[nox],nowmusic.meta.Genre);
	nox=nox+wstrlen(nowmusic.meta.Genre);
      }
      else if (prt[x+1]==L'y' || prt[x+1]==L'4')
      {
	wstrcpy(&returnvalue[nox],nowmusic.meta.Year);
	nox=nox+wstrlen(nowmusic.meta.Year);
      }
      else if (prt[x+1]==L'b' || prt[x+1]==L'5')
      {
        wchar_t bitrate[50];
        if (nowmusic.hdr.VBR==true && AverageVBR==false)
        {
          wstrcpy(bitrate,L"VBR");
        }
        else
        {
          snwprintf(bitrate,TEXTID_ANY_LEN,L"%d",nowmusic.hdr.bitrate);
        }
	wstrcpy(&returnvalue[nox],bitrate);
	nox=nox+wstrlen(bitrate);
      }
      else if (prt[x+1]==L'f' || prt[x+1]==L'6')
      {
        wchar_t frequency[50];
        snwprintf(frequency,TEXTID_ANY_LEN,L"%d",nowmusic.hdr.frequency);
	wstrcpy(&returnvalue[nox],frequency);
	nox=nox+wstrlen(frequency);
      }
      else if (prt[x+1]==L'm' || prt[x+1]==L'7')
      {
        wchar_t mode[50];
        if (nowmusic.hdr.mode==0)
        {
          wstrcpy(mode,L"Stereo");
        }
        else if (nowmusic.hdr.mode==1)
        {
          wstrcpy(mode,L"Joint Stereo");
        }
        else if (nowmusic.hdr.mode==2)
        {
          wstrcpy(mode,L"Dual Channel");
        }
         else if (nowmusic.hdr.mode==3)
        {
          wstrcpy(mode,L"Single Channel");
        }
        else
        {
          wstrcpy(mode,L" ");
        }
	wstrcpy(&returnvalue[nox],mode);
	nox=nox+wstrlen(mode);
      }
      else if (prt[x+1]==L'l' || prt[x+1]==L'8')
      {
        int MIN=nowmusic.hdr.LengthInSeconds/60;
        int SEC=nowmusic.hdr.LengthInSeconds%60;
        wchar_t length[50];
        snwprintf(length,TEXTID_ANY_LEN,L"%02d:%02d",MIN,SEC);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else if (prt[x+1]==L'n' || prt[x+1]==L'9')
      {
        wchar_t length[50];
        snwprintf(length,TEXTID_ANY_LEN,L"%02d",nowmusic.meta.TrackNum);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else if (prt[x+1]==L's')
      {
        wchar_t length[50];
        int MIN=CurrentTime/60;
        int SEC=CurrentTime%60;
        snwprintf(length,TEXTID_ANY_LEN,L"%02d:%02d",MIN,SEC);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else if (prt[x+1]==L'e')
      {
        wchar_t length[50];
        int TimeLeft = currenttrack->fulltime-CurrentTime;
        int MIN=TimeLeft/60;
        int SEC=TimeLeft%60;
        snwprintf(length,TEXTID_ANY_LEN,L"%02d:%02d",MIN,SEC);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else if (prt[x+1]==L'i')
      {
        wchar_t length[50];
        int pos = currenttrack->pos;
        snwprintf(length,TEXTID_ANY_LEN,L"%d",pos);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else if (prt[x+1]==L'p')
      {
        wchar_t length[50];
        int count= currenttrack->tracks_count;
        snwprintf(length,TEXTID_ANY_LEN,L"%d",count);
        wstrcpy(&returnvalue[nox],length);
	nox=nox+wstrlen(length);
      }
      else
      {
	nox+=1;
      }
      x++;
    }
  }
  returnvalue[nox]='\0';
  wstrcpy(where,returnvalue);
};
void lbl_okill();
void lbl_kill();
void lbl_ofill();
void lbl_fill();
void optimized_InitLbls()
{
  wchar_t lbl[256];
  lbl_okill();
  if (labels[0].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL1_NAME,strlen(LABEL1_NAME));
    caprintf(lbl,labels[0].label);
  }
    if (labels[1].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL2_NAME,strlen(LABEL2_NAME));
    caprintf(lbl,labels[1].label);
  }
    if (labels[2].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL3_NAME,strlen(LABEL3_NAME));
    caprintf(lbl,labels[2].label);
  }
  
    if (labels[3].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL4_NAME,strlen(LABEL4_NAME));
    caprintf(lbl,labels[3].label);
  }
    if (labels[4].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL5_NAME,strlen(LABEL5_NAME));
    caprintf(lbl,labels[4].label);
  }
    if (labels[5].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL6_NAME,strlen(LABEL6_NAME));
    caprintf(lbl,labels[5].label);
  }
    if (labels[6].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL7_NAME,strlen(LABEL7_NAME));
    caprintf(lbl,labels[6].label);
  }
    if (labels[7].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL8_NAME,strlen(LABEL8_NAME));
    caprintf(lbl,labels[7].label);
  }
    if (labels[8].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL9_NAME,strlen(LABEL9_NAME));
    caprintf(lbl,labels[8].label);
  }
  if (labels[9].ChangeEveryTime)
  {
    char2unicode(lbl,LABEL10_NAME,strlen(LABEL10_NAME));
    caprintf(lbl,labels[9].label);
  }
  if (ScrollingMode)
  {
    labels[0].scr.SCROLL_FLEN=GetLabelSize(labels[0].label,labels[0].Font);
    labels[1].scr.SCROLL_FLEN=GetLabelSize(labels[1].label,labels[1].Font);
    labels[2].scr.SCROLL_FLEN=GetLabelSize(labels[2].label,labels[2].Font);
    labels[3].scr.SCROLL_FLEN=GetLabelSize(labels[3].label,labels[3].Font);
    labels[4].scr.SCROLL_FLEN=GetLabelSize(labels[4].label,labels[4].Font);
    labels[5].scr.SCROLL_FLEN=GetLabelSize(labels[5].label,labels[5].Font);
    labels[6].scr.SCROLL_FLEN=GetLabelSize(labels[6].label,labels[6].Font);
    labels[7].scr.SCROLL_FLEN=GetLabelSize(labels[7].label,labels[7].Font);
    labels[8].scr.SCROLL_FLEN=GetLabelSize(labels[8].label,labels[8].Font);
    labels[9].scr.SCROLL_FLEN=GetLabelSize(labels[9].label,labels[9].Font);
  }
  lbl_ofill();
};

void InitLbls()
{
  lbl_kill();
  wchar_t lbl[256];
  char2unicode(lbl,LABEL1_NAME,strlen(LABEL1_NAME));
  caprintf(lbl,labels[0].label);
  
  char2unicode(lbl,LABEL2_NAME,strlen(LABEL2_NAME));
  caprintf(lbl,labels[1].label);
  
  char2unicode(lbl,LABEL3_NAME,strlen(LABEL3_NAME));
  caprintf(lbl,labels[2].label);
  
  char2unicode(lbl,LABEL4_NAME,strlen(LABEL4_NAME));
  caprintf(lbl,labels[3].label);
  
  char2unicode(lbl,LABEL5_NAME,strlen(LABEL5_NAME));
  caprintf(lbl,labels[4].label);
  
  char2unicode(lbl,LABEL6_NAME,strlen(LABEL6_NAME));
  caprintf(lbl,labels[5].label);
                      
  char2unicode(lbl,LABEL7_NAME,strlen(LABEL7_NAME));
  caprintf(lbl,labels[6].label);
  
  char2unicode(lbl,LABEL8_NAME,strlen(LABEL8_NAME));
  caprintf(lbl,labels[7].label);
  
  char2unicode(lbl,LABEL9_NAME,strlen(LABEL9_NAME));
  caprintf(lbl,labels[8].label);
  
  char2unicode(lbl,LABEL10_NAME,strlen(LABEL10_NAME));
  caprintf(lbl,labels[9].label);
  
  if (ScrollingMode)
  {
    labels[0].scr.SCROLL_FLEN=GetLabelSize(labels[0].label,labels[0].Font);
    labels[1].scr.SCROLL_FLEN=GetLabelSize(labels[1].label,labels[1].Font);
    labels[2].scr.SCROLL_FLEN=GetLabelSize(labels[2].label,labels[2].Font);
    labels[3].scr.SCROLL_FLEN=GetLabelSize(labels[3].label,labels[3].Font);
    labels[4].scr.SCROLL_FLEN=GetLabelSize(labels[4].label,labels[4].Font);
    labels[5].scr.SCROLL_FLEN=GetLabelSize(labels[5].label,labels[5].Font);
    labels[6].scr.SCROLL_FLEN=GetLabelSize(labels[6].label,labels[6].Font);
    labels[7].scr.SCROLL_FLEN=GetLabelSize(labels[7].label,labels[7].Font);
    labels[8].scr.SCROLL_FLEN=GetLabelSize(labels[8].label,labels[8].Font);
    labels[9].scr.SCROLL_FLEN=GetLabelSize(labels[9].label,labels[9].Font);
  }
  lbl_fill();
};

int Font_GetHeight(int FONT)
{
  SetFont(FONT);
  return GetImageHeight(' ');
};

void CustomLbls()
{
  memset(labels,0,sizeof(LABEL)*NUM);
  // Записываем всякую инфу в структуры надписей из бкфг.
  img.x=CustomRC.x1;
  img.y=CustomRC.y1;
  img.mx=CustomRC.x2-CustomRC.x1;
  img.my=CustomRC.y2-CustomRC.y1;
    
  locCover_X=CoverRC.x1;
  locCover_Y=CoverRC.y1;
  locCover_MaxX=CoverRC.x2-CoverRC.x1;
  locCover_MaxY=CoverRC.y2-CoverRC.y1;
  
  //Label1
  labels[0].SHOW=LABEL1_SHOW;
  
  labels[0].X=LABEL1_X_POS;
  labels[0].Y=LABEL1_Y_POS;

  labels[0].Color=LABEL1_COLOR1;
  labels[0].borderColor=LABEL1_COLOR2;
  labels[0].Font=LABEL1_FONT;
  labels[0].CT=LABEL1_CT;
  labels[0].scr.SCROLL_USE=LABEL1_SCROLL_YES;
  labels[0].scr.SCROLL_DEF_X=LABEL1_X_POS;
  labels[0].scr.SCROLL_WHERETO=LABEL1_SCROLL_LR;
  labels[0].scr.SCROLL_LEFT=LABEL1_SCROLL_LEFT;
  labels[0].scr.SCROLL_RIGHT=LABEL1_SCROLL_RIGHT;
  labels[0].scr.SCROLL_SPEED=LABEL1_SCROLL_MP;
  labels[0].scr.SCROLL_STRLEN=LABEL1_SCROLL_STRLEN;
  labels[0].ChangeEveryTime=LABEL1_NEED_REDRAW;
  labels[0].LINE_ON=LABEL1_LINE_ON;
  labels[0].LINE_DIST=LABEL1_LINE_DIST;
  labels[0].LINE_SIZE=LABEL1_LINE_SIZE;
  labels[0].FONT_SIZE=Font_GetHeight(labels[0].Font);
  labels[0].LINE_MINUS=LABEL1_LINE_MINUS;
  
  //Label2
  labels[1].SHOW=LABEL2_SHOW;
  labels[1].X=LABEL2_X_POS;
  labels[1].Y=LABEL2_Y_POS;
  
  labels[1].Color=LABEL2_COLOR1;
  labels[1].borderColor=LABEL2_COLOR2;
  labels[1].Font=LABEL2_FONT;
  labels[1].CT=LABEL2_CT;
  
  labels[1].scr.SCROLL_USE=LABEL2_SCROLL_YES;
  labels[1].scr.SCROLL_DEF_X=LABEL2_X_POS;
  labels[1].scr.SCROLL_WHERETO=LABEL2_SCROLL_LR;
  labels[1].scr.SCROLL_LEFT=LABEL2_SCROLL_LEFT;
  labels[1].scr.SCROLL_RIGHT=LABEL2_SCROLL_RIGHT;
  labels[1].scr.SCROLL_SPEED=LABEL2_SCROLL_MP;
  labels[1].scr.SCROLL_LENGTH=LABEL2_SCROLL_LENGTH;
  labels[1].scr.SCROLL_STRLEN=LABEL2_SCROLL_STRLEN;
  labels[1].ChangeEveryTime=LABEL2_NEED_REDRAW;
  labels[1].LINE_ON=LABEL2_LINE_ON;
  labels[1].LINE_DIST=LABEL2_LINE_DIST;
  labels[1].LINE_SIZE=LABEL2_LINE_SIZE;
  labels[1].FONT_SIZE=Font_GetHeight(labels[1].Font);
  labels[1].LINE_MINUS=LABEL2_LINE_MINUS;
  
  //Label3
  labels[2].SHOW=LABEL3_SHOW;
  labels[2].X=LABEL3_X_POS;
  labels[2].Y=LABEL3_Y_POS;

  labels[2].Color=LABEL3_COLOR1;
  labels[2].borderColor=LABEL3_COLOR2;
  labels[2].Font=LABEL3_FONT;
  labels[2].CT=LABEL3_CT;
  
  labels[2].scr.SCROLL_USE=LABEL3_SCROLL_YES;
  labels[2].scr.SCROLL_DEF_X=LABEL3_X_POS;
  labels[2].scr.SCROLL_WHERETO=LABEL3_SCROLL_LR;
  labels[2].scr.SCROLL_LEFT=LABEL3_SCROLL_LEFT;
  labels[2].scr.SCROLL_RIGHT=LABEL3_SCROLL_RIGHT;
  labels[2].scr.SCROLL_SPEED=LABEL3_SCROLL_MP;
  labels[2].scr.SCROLL_LENGTH=LABEL3_SCROLL_LENGTH;
  labels[2].scr.SCROLL_STRLEN=LABEL3_SCROLL_STRLEN;
  labels[2].ChangeEveryTime=LABEL3_NEED_REDRAW;
  labels[2].LINE_ON=LABEL3_LINE_ON;
  labels[2].LINE_DIST=LABEL3_LINE_DIST;
  labels[2].LINE_SIZE=LABEL3_LINE_SIZE;
  labels[2].FONT_SIZE=Font_GetHeight(labels[2].Font);
  labels[2].LINE_MINUS=LABEL3_LINE_MINUS;
  
  //Label4
  labels[3].SHOW=LABEL4_SHOW;
  labels[3].X=LABEL4_X_POS;
  labels[3].Y=LABEL4_Y_POS;

  labels[3].Color=LABEL4_COLOR1;
  labels[3].borderColor=LABEL4_COLOR2;
  labels[3].Font=LABEL4_FONT;
  labels[3].CT=LABEL4_CT;
  
  labels[3].scr.SCROLL_USE=LABEL4_SCROLL_YES;
  labels[3].scr.SCROLL_DEF_X=LABEL4_X_POS;
  labels[3].scr.SCROLL_WHERETO=LABEL4_SCROLL_LR;
  labels[3].scr.SCROLL_LEFT=LABEL4_SCROLL_LEFT;
  labels[3].scr.SCROLL_RIGHT=LABEL4_SCROLL_RIGHT;
  labels[3].scr.SCROLL_SPEED=LABEL4_SCROLL_MP;
  labels[3].scr.SCROLL_LENGTH=LABEL4_SCROLL_LENGTH;
  labels[3].scr.SCROLL_STRLEN=LABEL4_SCROLL_STRLEN;
  labels[3].ChangeEveryTime=LABEL4_NEED_REDRAW;
  labels[3].LINE_ON=LABEL4_LINE_ON;
  labels[3].LINE_DIST=LABEL4_LINE_DIST;
  labels[3].LINE_SIZE=LABEL4_LINE_SIZE;
  labels[3].FONT_SIZE=Font_GetHeight(labels[3].Font);
  labels[3].LINE_MINUS=LABEL4_LINE_MINUS;
  
  //Label5
  labels[4].SHOW=LABEL5_SHOW;
  labels[4].X=LABEL5_X_POS;
  labels[4].Y=LABEL5_Y_POS;

  labels[4].Color=LABEL5_COLOR1;
  labels[4].borderColor=LABEL5_COLOR2;
  labels[4].Font=LABEL5_FONT;
  labels[4].CT=LABEL5_CT;
  
  labels[4].scr.SCROLL_USE=LABEL5_SCROLL_YES;
  labels[4].scr.SCROLL_DEF_X=LABEL5_X_POS;
  labels[4].scr.SCROLL_WHERETO=LABEL5_SCROLL_LR;
  labels[4].scr.SCROLL_LEFT=LABEL5_SCROLL_LEFT;
  labels[4].scr.SCROLL_RIGHT=LABEL5_SCROLL_RIGHT;
  labels[4].scr.SCROLL_SPEED=LABEL5_SCROLL_MP;
  labels[4].scr.SCROLL_LENGTH=LABEL5_SCROLL_LENGTH;
  labels[4].scr.SCROLL_STRLEN=LABEL5_SCROLL_STRLEN;
  labels[4].ChangeEveryTime=LABEL5_NEED_REDRAW;
  labels[4].LINE_ON=LABEL5_LINE_ON;
  labels[4].LINE_DIST=LABEL5_LINE_DIST;
  labels[4].LINE_SIZE=LABEL5_LINE_SIZE;
  labels[4].FONT_SIZE=Font_GetHeight(labels[4].Font);
  labels[4].LINE_MINUS=LABEL5_LINE_MINUS;
  
  //Label6
  labels[5].SHOW=LABEL6_SHOW;
  labels[5].X=LABEL6_X_POS;
  labels[5].Y=LABEL6_Y_POS;

  labels[5].Color=LABEL6_COLOR1;
  labels[5].borderColor=LABEL6_COLOR2;
  labels[5].Font=LABEL6_FONT;
  labels[5].CT=LABEL6_CT;
  labels[5].scr.SCROLL_USE=LABEL6_SCROLL_YES;
  labels[5].scr.SCROLL_DEF_X=LABEL6_X_POS;
  labels[5].scr.SCROLL_WHERETO=LABEL6_SCROLL_LR;
  labels[5].scr.SCROLL_LEFT=LABEL6_SCROLL_LEFT;
  labels[5].scr.SCROLL_RIGHT=LABEL6_SCROLL_RIGHT;
  labels[5].scr.SCROLL_SPEED=LABEL6_SCROLL_MP;
  labels[5].scr.SCROLL_LENGTH=LABEL6_SCROLL_LENGTH;
  labels[5].scr.SCROLL_STRLEN=LABEL6_SCROLL_STRLEN;
  labels[5].ChangeEveryTime=LABEL6_NEED_REDRAW;
  labels[5].LINE_ON=LABEL6_LINE_ON;
  labels[5].LINE_DIST=LABEL6_LINE_DIST;
  labels[5].LINE_SIZE=LABEL6_LINE_SIZE;
  labels[5].FONT_SIZE=Font_GetHeight(labels[5].Font);
  labels[5].LINE_MINUS=LABEL6_LINE_MINUS;
  
  //Label7
  labels[6].SHOW=LABEL7_SHOW;
  labels[6].X=LABEL7_X_POS;
  labels[6].Y=LABEL7_Y_POS;

  labels[6].Color=LABEL7_COLOR1;
  labels[6].borderColor=LABEL7_COLOR2;
  labels[6].Font=LABEL7_FONT;
  labels[6].CT=LABEL7_CT;
  labels[6].scr.SCROLL_USE=LABEL7_SCROLL_YES;
  labels[6].scr.SCROLL_DEF_X=LABEL7_X_POS;
  labels[6].scr.SCROLL_WHERETO=LABEL7_SCROLL_LR;
  labels[6].scr.SCROLL_LEFT=LABEL7_SCROLL_LEFT;
  labels[6].scr.SCROLL_RIGHT=LABEL7_SCROLL_RIGHT;
  labels[6].scr.SCROLL_SPEED=LABEL7_SCROLL_MP;
  labels[6].scr.SCROLL_LENGTH=LABEL7_SCROLL_LENGTH;
  labels[6].scr.SCROLL_STRLEN=LABEL7_SCROLL_STRLEN;
  labels[6].ChangeEveryTime=LABEL7_NEED_REDRAW;
  labels[6].LINE_ON=LABEL7_LINE_ON;
  labels[6].LINE_DIST=LABEL7_LINE_DIST;
  labels[6].LINE_SIZE=LABEL7_LINE_SIZE;  
  labels[6].FONT_SIZE=Font_GetHeight(labels[6].Font);
  labels[6].LINE_MINUS=LABEL7_LINE_MINUS;
  
  //Label8
  labels[7].SHOW=LABEL8_SHOW;
  labels[7].X=LABEL8_X_POS;
  labels[7].Y=LABEL8_Y_POS;

  labels[7].Color=LABEL8_COLOR1;
  labels[7].borderColor=LABEL8_COLOR2;
  labels[7].Font=LABEL8_FONT;
  labels[7].CT=LABEL8_CT;
  labels[7].scr.SCROLL_USE=LABEL8_SCROLL_YES;
  labels[7].scr.SCROLL_DEF_X=LABEL8_X_POS;
  labels[7].scr.SCROLL_WHERETO=LABEL8_SCROLL_LR;
  labels[7].scr.SCROLL_LEFT=LABEL8_SCROLL_LEFT;
  labels[7].scr.SCROLL_RIGHT=LABEL8_SCROLL_RIGHT;
  labels[7].scr.SCROLL_SPEED=LABEL8_SCROLL_MP;
  labels[7].scr.SCROLL_LENGTH=LABEL8_SCROLL_LENGTH;
  labels[7].scr.SCROLL_STRLEN=LABEL8_SCROLL_STRLEN;
  labels[7].ChangeEveryTime=LABEL8_NEED_REDRAW;
  labels[7].LINE_ON=LABEL8_LINE_ON;
  labels[7].LINE_DIST=LABEL8_LINE_DIST;
  labels[7].LINE_SIZE=LABEL8_LINE_SIZE;
  labels[7].FONT_SIZE=Font_GetHeight(labels[7].Font);
  labels[7].LINE_MINUS=LABEL8_LINE_MINUS;
  
  //Label9
  labels[8].SHOW=LABEL9_SHOW;
  labels[8].X=LABEL9_X_POS;
  labels[8].Y=LABEL9_Y_POS;

  labels[8].Color=LABEL9_COLOR1;
  labels[8].borderColor=LABEL9_COLOR2;
  labels[8].Font=LABEL9_FONT;
  labels[8].CT=LABEL9_CT;
  labels[8].scr.SCROLL_USE=LABEL9_SCROLL_YES;
  labels[8].scr.SCROLL_DEF_X=LABEL9_X_POS;
  labels[8].scr.SCROLL_WHERETO=LABEL9_SCROLL_LR;
  labels[8].scr.SCROLL_LEFT=LABEL9_SCROLL_LEFT;
  labels[8].scr.SCROLL_RIGHT=LABEL9_SCROLL_RIGHT;
  labels[8].scr.SCROLL_SPEED=LABEL9_SCROLL_MP;
  labels[8].scr.SCROLL_LENGTH=LABEL9_SCROLL_LENGTH;
  labels[8].scr.SCROLL_STRLEN=LABEL9_SCROLL_STRLEN;
  labels[8].ChangeEveryTime=LABEL9_NEED_REDRAW;
  labels[8].LINE_ON=LABEL9_LINE_ON;
  labels[8].LINE_DIST=LABEL9_LINE_DIST;
  labels[8].LINE_SIZE=LABEL9_LINE_SIZE;
  labels[8].FONT_SIZE=Font_GetHeight(labels[8].Font);
  labels[8].LINE_MINUS=LABEL9_LINE_MINUS;
  
  //Label10
  labels[9].SHOW=LABEL10_SHOW;
  labels[9].X=LABEL10_X_POS;
  labels[9].Y=LABEL10_Y_POS;
  labels[9].Color=LABEL10_COLOR1;
  labels[9].borderColor=LABEL10_COLOR2;
  labels[9].Font=LABEL10_FONT;
  labels[9].CT=LABEL10_CT;
  labels[9].scr.SCROLL_USE=LABEL10_SCROLL_YES;
  labels[9].scr.SCROLL_DEF_X=LABEL10_X_POS;
  labels[9].scr.SCROLL_WHERETO=LABEL10_SCROLL_LR;
  labels[9].scr.SCROLL_LEFT=LABEL10_SCROLL_LEFT;
  labels[9].scr.SCROLL_RIGHT=LABEL10_SCROLL_RIGHT;
  labels[9].scr.SCROLL_SPEED=LABEL10_SCROLL_MP;
  labels[9].scr.SCROLL_LENGTH=LABEL10_SCROLL_LENGTH;
  labels[9].scr.SCROLL_STRLEN=LABEL10_SCROLL_STRLEN;
  labels[9].ChangeEveryTime=LABEL10_NEED_REDRAW;
  labels[9].LINE_ON=LABEL10_LINE_ON;
  labels[9].LINE_DIST=LABEL10_LINE_DIST;
  labels[9].LINE_SIZE=LABEL10_LINE_SIZE;
  labels[9].FONT_SIZE=Font_GetHeight(labels[9].Font);
  labels[9].LINE_MINUS=LABEL10_LINE_MINUS;
  //Поставим скроллинг влево на всех:
  int x;
  for (x=0;x<10;x++)
  {
    labels[x].scr.SCROLL_WHERE=0;
  }
};

void lbl_ofill()
{
  int x;
  for (x=0;x<10;x++)
  {
    if (labels[x].ChangeEveryTime)
    {
      labels[x].str=TextID_Create(labels[x].label,ENC_UCS2,TEXTID_ANY_LEN);
    }
  }
};

void lbl_okill()
{
  int x;
  for (x=0;x<10;x++)
  {
    if (labels[x].str!=0 && labels[x].str!=EMPTY_TEXTID && labels[x].ChangeEveryTime)
    {
      TextID_Destroy(labels[x].str);
      labels[x].str=EMPTY_TEXTID;
    }
  }
};

void lbl_fill()
{
  int x;
  for (x=0;x<10;x++)
  {
    labels[x].str=TextID_Create(labels[x].label,ENC_UCS2,TEXTID_ANY_LEN);
  }
};

void lbl_kill()
{
  int x;
  for (x=0;x<10;x++)
  {
    if (labels[x].str!=0 && labels[x].str!=EMPTY_TEXTID)
    {
      TextID_Destroy(labels[x].str);
      labels[x].str=EMPTY_TEXTID;
    }
  }
};
