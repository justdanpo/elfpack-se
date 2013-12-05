#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\wdisplay.h"
#include "header\cl.h"
#include "header\config_data.h"
#include "header\images.h"

wchar_t playnow_h=NOIMAGE;
wchar_t playview_h=NOIMAGE;

wchar_t imageID=NOIMAGE;
wchar_t cover_h_=NOIMAGE;
wchar_t cover_v_=NOIMAGE;

wchar_t playicn=NOIMAGE;
wchar_t stopicn=NOIMAGE;
wchar_t pauseicn=NOIMAGE;


wchar_t images[]={NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE}; //left,right,left2,right2,play,stop,up,down
wchar_t *images_fnames[]={
  L"Prev_nav_icn.png",
  L"Next_nav_icn.png",
  L"Prev2_nav_icn.png",
  L"Next2_nav_icn.png",
  L"Play_nav_icn.png",
  L"Stop_nav_icn.png",
  L"Up_nav_icn.png",
  L"Down_nav_icn.png"
};
int images_offsets[]={
  -2,
  -3,
  -4,
  -6,
  -5,
  -1,
  -7,
  -10
};
wchar_t presets[]={NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE, NOIMAGE};
wchar_t navigimg=NOIMAGE;
wchar_t *presets_names[]={
  L"MP_EQ_NORMAL_ICN",
  L"MP_EQ_BASS_ICN",
  L"MP_EQ_MEGABASS_ICN",
  L"MP_EQ_VOICE_ICN",
  L"MP_EQ_TREBLEBOOST_ICN",
  L"MP_EQ_NORMAL_ICN",
  L"MP_EQ_MANUAL_ICN"
};

wchar_t media_images[]={NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};
wchar_t *media_names[]={
  L"MEDIAPLAYER_FAST_FORWARD_ICN",
  L"MEDIAPLAYER_REWIND_ICN",
  L"MEDIAPLAYER_MODE_LOOP_ICN",
  L"EMPTY_ICN",
  L"MEDIAPLAYER_MODE_RANDOM_ICN",
  L"EMPTY_ICN",
  L"EMPTY_ICN",
  L"EMPTY_ICN"
};

wchar_t *media_fnames[]={
  L"Forward_icn.png",
  L"Rewind_icn.png",
  L"Loop_mode_icn.png",
  L"StereoWidening_mode_off_icn.png",
  L"Random_mode_icn.png",
  L"Random_mode_off_icn.png",
  L"StereoWidening_mode_icn.png",
  L"Loop_mode_off_icn.png"
};

inline IMAGEID RegisterImage (wchar_t  * path, wchar_t * fname)
{ 
  FSTAT fst;
  if (fstat(path,fname,&fst)==0)
  {
    IMAGEID ret;
    ImageID_Get(path, fname,&ret);
    
    return ret;
  }
  else
  {
    return NOIMAGE;
  }
}; 

#define IDFREE(a) if (a!=NOIMAGE && a!=0) {ImageID_Free(a);} a=NOIMAGE

int GetIconID(wchar_t *txt)
{
  int id=NOIMAGE;
  iconidname2id(txt,0xFFFFFFFF,&id);
  return id;
};

wchar_t main_icons[]={NOIMAGE,NOIMAGE,NOIMAGE};
wchar_t *main_icons_fnames[]={
  L"Artist_icn.png",
  L"Album_icn.png",
  L"Title_icn.png"
};

#pragma optimize = s 9
void init_resources(wchar_t *path)
{
#define REGISTER(__iconname__,__internalname__,__filename__) if (img_priority==0 || img_priority==2) {__iconname__=GetIconID(__internalname__);} if (img_priority<2) {    wchar_t img=RegisterImage(checkfile_(__filename__,path),__filename__); if (img!=NOIMAGE) { __iconname__=img; }}
  
  IDFREE(playview_h);
  REGISTER(playview_h,L"EMPTY_ICN",L"PlayView_Background_h.png");
  
  IDFREE(playnow_h);
  REGISTER(playnow_h,L"EMPTY_ICN",L"PlayNow_Background_h.png");
  
  IDFREE(playicn);
  REGISTER(playicn,L"MEDIAPLAYER_PLAY_ICN",L"Play_icn.png");
    
  IDFREE(pauseicn);
  REGISTER(pauseicn,L"MEDIAPLAYER_STOP_ICN",L"Pause_icn.png");
  if (pauseicn==NOIMAGE)
  {
    REGISTER(pauseicn,L"MEDIAPLAYER_STOP_ICN",L"Stop_icn.png");
  }
  
  IDFREE(stopicn);
  REGISTER(stopicn,L"MEDIAPLAYER_STOP_ICN",L"Stop_icn.png");
  
  if (img_priority<2)
  {
    for (int j=0;j<3;j++)
    {
      wchar_t img=RegisterImage(checkfile_(main_icons_fnames[j],path),main_icons_fnames[j]);
      if (img!=NOIMAGE)
        main_icons[j]=img;
    }
    int imageID;
    imageID=GetIconID(L"MP_EQ_BASS_ICN");
    for (int j=0;j<8;j++)
    {
      images[j]=imageID+images_offsets[j];
      wchar_t img=RegisterImage(checkfile_(images_fnames[j],path),images_fnames[j]);
      if (img!=NOIMAGE)
        images[j]=img;
    }
  }
  
  IDFREE(cover_h_);
  REGISTER(cover_h_,L"EMPTY_ICN",L"cover_h.png");

  IDFREE(cover_v_);

  REGISTER(cover_v_,L"EMPTY_ICN",L"cover_v.png");
  
  for (int j=0;j<7;j++)
  {
    IDFREE(presets[j]);
    wchar_t prt_name[50];
    snwprintf(prt_name,49,L"eq_%02d.png",j);
    REGISTER(presets[j],presets_names[j],prt_name);
  }
  for (int j=0;j<8;j++)
  {
    IDFREE(media_images[j]);
    REGISTER(media_images[j],media_names[j],media_fnames[j]);
  }
  IDFREE(navigimg);
  REGISTER(navigimg,L"EMPTY_ICN",L"Navigation.png");
};

#pragma optimize = s 9
void deinit_resources()
{
  IDFREE(playnow_h);
  IDFREE(playview_h);

  IDFREE(cover_h_);
  IDFREE(cover_v_);
  
  IDFREE(playicn);
  IDFREE(pauseicn);
  IDFREE(stopicn);
  IDFREE(navigimg);
  for (int j=0;j<7;j++)
  {
    IDFREE(presets[j]);
  }
  for (int j=0;j<8;j++)
  {
    IDFREE(images[j]);
  }
  for (int j=0;j<6;j++)
  {
    IDFREE(media_images[j]);
  }
  for (int j=0;j<3;j++)
  {
    IDFREE(main_icons[j]);
  }
};
