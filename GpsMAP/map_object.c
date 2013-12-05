#define USEPNG
#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "main.h"
#include "map_object.h"
#include "pngread.h"
#include "draw.h"
#include "read_csv.h"
#include "math.h"


float x1_map_real=39.532383;
float x2_map_real=39.87645;
float y1_map_real=47.3333;
float y2_map_real=47.06715;

NEIGHBOUR_CELLDATA *NetData=(NEIGHBOUR_CELLDATA *)0x202EBA10;
CUR_CELLDATA *CurNetData=(CUR_CELLDATA *)0x2018EBE4;

int GetRxLev_ByCh(int ch)
{
  if (ch==0)  return (-80);
  if (ch>7) return (-120);
  return (NetData[ch-1].rxlvl); 
}

int GetCid_ByCh(int ch)
{
  if (ch==0)  return (CurNetData->cid);
  if (ch>7) return (0);
  return (NetData[ch-1].cid); 
}

int GetLac_ByCh(int ch)
{
  if (ch==0)  return (CurNetData->lac);
  if (ch>7) return (0);
  return (NetData[ch-1].lac); 
}

int ReadPngFile(LIST *png_lst, char *town, int index_x, int index_y)
{
  wchar_t dir[64];
  wchar_t name[64];
  int result=0;
  int f;
  FSTAT stat;
  char *buf;
  PNG_CONVERT *usr_png;
  snwprintf(dir,63,L"%ls/GpsMAP/%s", GetDir(DIR_ELFS),town);
  snwprintf(name,63,L"%s_%02u_%02u.png",town,index_y,index_x);
  if (fstat(dir,name,&stat)>=0)
  {
    if ((f=_fopen(dir,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,NULL))>=0)
    {
      if ((buf=new char[stat.fsize]))
      {
        usr_png=InitPng(png_lst,index_x,index_y);
        fread(f,buf,stat.fsize);
        fclose(f);
        process_data(usr_png,buf,stat.fsize);
        result=1;
        Destr_Png(usr_png);
        delete buf;
      }
    }
  }
  return (result);
}

#define COLOR(R,G,B,A) ((B&0xFF)|((G&0xFF)<<8)|((R&0xFF)<<16)|((A&0xFF)<<24))

void InitDirPng(TOWN *town)
{
  wchar_t dir[64];
  wchar_t name[64];
  FSTAT stat;
  int width, height;
  int map_width=0,map_height=0;
  snwprintf(dir,63,L"%ls/GpsMAP/%s",GetDir(DIR_ELFS),town->name);
  int x_size=0, y_size=0;
  int f;
  while(x_size<100)
  {
    snwprintf(name,63,L"%s_00_%02u.png",town->name,x_size);
    if (fstat(dir,name,&stat)<0) break;
    x_size++;
  }
  while(y_size<100)
  {
    snwprintf(name,63,L"%s_%02u_00.png",town->name,y_size);
    if (fstat(dir,name,&stat)<0) break;
    y_size++;
  }
  town->sizes_x=new int [x_size];
  town->sizes_y=new int [y_size];
  for (int i=0; i<x_size; i++)
  {
    snwprintf(name,63,L"%s_00_%02u.png",town->name,i);
    f=get_png_info(dir,name,&width,&height);
    town->sizes_x[i]=f?width:0;
    map_width+=town->sizes_x[i];
  }  
  for (int i=0; i<y_size; i++)
  {
    snwprintf(name,63,L"%s_%02u_00.png",town->name,i);
    f=get_png_info(dir,name,&width,&height);
    town->sizes_y[i]=f?height:0;
    map_height+=town->sizes_y[i];
  }
  town->map_arr_x=x_size;
  town->map_arr_y=y_size;
  town->map_width=map_width;
  town->map_height=map_height;
}

void DeInitDirPng(TOWN *town)
{
  delete town->sizes_x;
  delete town->sizes_y;
}


int PngCmpProc(void *what, void *where)
{
  PNG_LIST *png_lst=(PNG_LIST *)what;
  int x=((int *)where)[0];
  int y=((int *)where)[1];
  return  (png_lst->index_x==x && png_lst->index_y==y)?0:1;
}

void RecalkPos(DISP_OBJ_MAP *db)
{
  int cidlac[2];
  float Xres=0, Yres=0, Kres=0, Kn;
  for (int i=1; i<7; i++)
  {
    cidlac[0]=GetCid_ByCh(i);
    cidlac[1]=GetLac_ByCh(i);
    int index=List_Find(db->ini.cur_town->bs_dat,&cidlac,FindBSByCiLac);
    if (index!=LIST_ERROR)
    {
      int rx=GetRxLev_ByCh(i);
      BS_POINT *bs=(BS_POINT *)List_Get(db->ini.cur_town->bs_dat,index);
      Kn=powf(10,(rx+30)/5);
      Xres+=bs->point_x*Kn;
      Yres+=bs->point_y*Kn;
      Kres+=Kn;
    }
  }
  db->Xres=Xres/=Kres;
  db->Yres=Yres/=Kres;
  db->total_y_pos=(fabsf(db->ini.cur_town->start_y-Yres))*db->ini.cur_town->map_height/(fabsf(db->ini.cur_town->end_y-db->ini.cur_town->start_y));
  db->total_x_pos=(fabsf(db->ini.cur_town->start_x-Xres))*db->ini.cur_town->map_width/(fabsf(db->ini.cur_town->end_x-db->ini.cur_town->start_x));
}

void FillScreenBuffer(DISP_OBJ_MAP *db)
{
  int block_x1=0, block_x2=0, block_y1=0, block_y2=0;
  int width=db->x2-db->x1, height=db->y2-db->y1;  // ширина и высота экрана
  int cur_x_offs=db->total_x_pos<(width/2)?0:db->total_x_pos>=(db->ini.cur_town->map_width-width/2)?db->ini.cur_town->map_width-width:db->total_x_pos-width/2;
  int cur_pos_x=db->total_x_pos-cur_x_offs;
  int cur_y_offs=db->total_y_pos<(height/2)?0:db->total_y_pos>=(db->ini.cur_town->map_height-height/2)?db->ini.cur_town->map_height-height:db->total_y_pos-height/2;
  int cur_pos_y=db->total_y_pos-cur_y_offs;
  
  int *scr_buf=db->scr_buf;
  for (int i=0,x_offs=0; i<db->ini.cur_town->map_arr_x; i++)   // Подсчитываем какие куски надо прочитать
  {
    if (x_offs<=cur_x_offs) block_x1=i;
    x_offs+=db->ini.cur_town->sizes_x[i];
    block_x2=i;
    if (x_offs>(cur_x_offs+width)) break;
  }
  for (int i=0,y_offs=0; i<db->ini.cur_town->map_arr_y; i++)
  {
    if (y_offs<=cur_y_offs) block_y1=i;
    y_offs+=db->ini.cur_town->sizes_y[i];
    block_y2=i;
    if (y_offs>(cur_y_offs+height)) break; 
  }
  for (int i=0; i<db->png_cache->FirstFree; i++)
  {
    PNG_LIST *png_lst=(PNG_LIST *)List_Get(db->png_cache,db->png_cache->FirstFree-1);
    if ((png_lst->index_x<block_x1 || png_lst->index_x>block_x2 ||
        png_lst->index_y<block_y1 || png_lst->index_y>block_y2) && db->png_cache->FirstFree>10)
    {
      png_lst=(PNG_LIST *)List_RemoveAt(db->png_cache,db->png_cache->FirstFree-1);
      RemovePngListElement(png_lst);
    }
  }

  for(int y=block_y1; y<=block_y2; y++)     // Если чего то не хватает загружаем
  {
    for(int x=block_x1; x<=block_x2; x++)
    {
      int xy[2];
      xy[0]=x;
      xy[1]=y;
      if (List_Find(db->png_cache,&xy,PngCmpProc)==LIST_ERROR) ReadPngFile(db->png_cache,db->ini.cur_town->name,x,y);
    }
  }
  
  for (int y=0,y_offs=0; y<=block_y2; y++)
  {
    if (y>=block_y1)
    {
      for(int x=0,x_offs=0; x<=block_x2; x++)
      {
        if (x>=block_x1)
        {
          int index;
          int xy[2];
          xy[0]=x;
          xy[1]=y;
          index=List_Find(db->png_cache,&xy,PngCmpProc);
          if (index!=LIST_ERROR)
          {
            PNG_LIST *png_lst=(PNG_LIST *)List_Get(db->png_cache,index);
            int img_x_offs=x_offs-cur_x_offs;
            int img_y_offs=y_offs-cur_y_offs;
            for (int y1=0; y1<height; y1++)
            {
              if (y1>=(img_y_offs) && y1<(img_y_offs+png_lst->height))
              {
                for (int x1=0; x1<width; x1++)
                {
                  if (x1>=(img_x_offs) && x1<(img_x_offs+png_lst->width))
                  {
                    unsigned int c=png_lst->image[(y1-img_y_offs)*png_lst->width+x1-img_x_offs];
                    scr_buf[y1*width+x1]=COLOR((c&0xE0),((c&0x1C)<<3),((c&3)<<6),0xFF); 
                  }
                }
              }
            }
          }
        }
        x_offs+=db->ini.cur_town->sizes_x[x];
      }
    }
    y_offs+=db->ini.cur_town->sizes_y[y];
  }
  // Рисуем крестик
  for (int i=-5, x=cur_pos_x; i<=5 ; i++) if ((x+i)>=0 && (x+i)<width ) scr_buf[cur_pos_y*width+x+i]=0xFF000000;
  for (int i=-5, y=cur_pos_y; i<=5 ; i++) if ((y+i)>=0 && (y+i)<height ) scr_buf[(y+i)*width+cur_pos_x]=0xFF000000;
  //drawcircle(scr_buf,width, height, 0,0, 32, 0xFF000000);
  //drawline(scr_buf,width, height, 0, 0, width-1, height-1, 0xFF000000);
}

static const char MapGuiName[]="Gui_GpsMAP";

int str_id=EMPTY_TEXTID;
int MapGuiOnCreate(DISP_OBJ_MAP *db)
{
  TOWN *town;
  db->scr_buf=new int[(db->disp_y=Display_GetHeight(0))*(db->disp_x=Display_GetWidth(0))];
  db->total_x_pos=db->total_y_pos=0;
  db->x1=db->y1=db->x2=db->y2=0;
  MainIniInit(&db->ini);
  town=FindTownByCiLac(&db->ini,GetCid_ByCh(0),GetLac_ByCh(0));
  if (!town) town=(TOWN *)List_Get(db->ini.towns,0);
  db->ini.cur_town=town;  
  InitDirPng(town);
  db->png_cache=List_Create();
  return (1);
}

void MapGuiOnClose(DISP_OBJ_MAP *db)
{
  mfree(db->scr_buf);
  DeInitDirPng(db->ini.cur_town);
  while(db->png_cache->FirstFree)
  {
    PNG_LIST *png=(PNG_LIST *)List_RemoveAt(db->png_cache,0);
    RemovePngListElement(png);    
  }
  List_Destroy(db->png_cache);
  MainIniDeInit(&db->ini);
}


void MapGuiOnRedraw(DISP_OBJ_MAP *db,int ,int,int)
{
  RECT rc_old;
  int font, gc_xx;
  GC *gc=get_DisplayGC();
  GC_GetRect(gc, &rc_old);
  if (db->x1!=rc_old.x1 || db->x2!=rc_old.x2 || db->y1!=rc_old.y1 || db->y2!=rc_old.y2)
  {
    db->x1=rc_old.x1;
    db->y1=rc_old.y1;
    db->x2=rc_old.x2;
    db->y2=rc_old.y2;
    FillScreenBuffer(db);
  }
  gc_xx=GC_GetXX(gc);
  GC_SetXX(gc,1);
  DrawRect(db->x1,db->y1,db->x2,db->y2,clWhite,clWhite);
  
  if (db->scr_buf)
  {
    GC_WritePixels(gc,db->x1,db->y1,db->x2,db->y2,db->scr_buf);
  }
  SetFont(FONT_E_20R);
  wchar_t buf[64];
  snwprintf(buf,63,L"%f,%f",db->Xres, db->Yres);
  if (str_id!=EMPTY_TEXTID) TextID_Destroy(str_id);
  str_id=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
  DrawString(str_id, 2, db->x1+2,db->y1+2,db->x2-2,db->y1+2+80,20,0,clBlack,0x00000000);
  GC_SetXX(gc,gc_xx);
}

void MapGuiOnKey(DISP_OBJ_MAP *db,int key,int,int repeat,int type)
{
  if (type==KBD_SHORT_RELEASE || type==KBD_REPEAT)
  {
    if (key==KEY_UP || key==KEY_DOWN || key==KEY_LEFT || key==KEY_RIGHT)
    {
      int per_key;
      int a;
      per_key=repeat*repeat;
      if (key==KEY_UP)
      {
        if ((a=db->total_y_pos-per_key)>0) db->total_y_pos=a;
        else db->total_y_pos=0;
      }
      else if (key==KEY_DOWN)
      {
        if ((a=db->total_y_pos+per_key)<(db->ini.cur_town->map_height-1)) db->total_y_pos=a;
        else db->total_y_pos=db->ini.cur_town->map_height-1;
      }
      else if (key==KEY_LEFT)
      {
        if ((a=db->total_x_pos-per_key)>0) db->total_x_pos=a;
        else db->total_x_pos=0;
      }
      else if (key==KEY_RIGHT)
      {
        if ((a=db->total_x_pos+per_key)<(db->ini.cur_town->map_width-1)) db->total_x_pos=a;
        else db->total_x_pos=db->ini.cur_town->map_width-1;
      }
      FillScreenBuffer(db);
    }
    if (key==KEY_DIGITAL_0+0)
    {
      RecalkPos(db);
      FillScreenBuffer(db);
    }
  }
  DispObject_InvalidateRect(&db->dsp_obj,0);
}


void MapGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,MapGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_MAP));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)MapGuiOnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)MapGuiOnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)MapGuiOnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)MapGuiOnKey);
}

void MapGui_destr( GUI* )
{
}

GUI_MAP *CreateMapGUI(BOOK *bk)
{
  GUI_MAP *gui_map=new GUI_MAP;
  if (!GUIObject_Create( gui_map,MapGui_destr,MapGui_constr, bk,0,0,0))
  {
    delete gui_map;
    return 0;    
  }
  if (bk) BookObj_AddGUIObject(bk, gui_map);
  GUIObject_SetStyle( gui_map,3);
  return gui_map;
}




