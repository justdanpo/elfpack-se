
#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "header\cl.h"
#include "header\accelctrl.h"
#include "header\wdisplay.h"
#include "header\images.h"
#include "..\include\cfg_items.h"
#include "header\conf_loader.h"
#include "header\config_data.h"
#include "header\skin_data.h"
#include "header\skin_loader.h"

#include "header\rus.h"
#include "header\eng.h"

#include "cfg_2_0_corrupted.h"
#include "cfg_2_1.h"

int migrate_from_2_0corrupted_to_2_1(CFG_2_0_corrupted *oldcfg, CFG_2_1 *cfg)
{
  cfg->standartwnd=oldcfg->standartwnd;
  cfg->title_show=oldcfg->title_show;
  cfg->title_font=oldcfg->title_font;
  cfg->title_rc=oldcfg->title_rc;
  cfg->title_color=oldcfg->title_color;
  cfg->title_scolor=oldcfg->title_scolor;
  cfg->title_ct=oldcfg->title_ct;
  cfg->album_show=oldcfg->album_show;
  cfg->album_font=oldcfg->album_font;
  cfg->album_rc=oldcfg->album_rc;
  cfg->album_color=oldcfg->album_color;
  cfg->album_scolor=oldcfg->album_scolor;
  cfg->album_ct=oldcfg->album_ct;
  cfg->artist_show=oldcfg->artist_show;
  cfg->artist_font=oldcfg->artist_font;
  cfg->artist_rc=oldcfg->artist_rc;
  cfg->artist_color=oldcfg->artist_color;
  cfg->artist_scolor=oldcfg->artist_scolor;
  cfg->artist_ct=oldcfg->artist_ct;
  cfg->year_show=oldcfg->year_show;
  cfg->year_font=oldcfg->year_font;
  cfg->year_rc=oldcfg->year_rc;
  cfg->year_color=oldcfg->year_color;
  cfg->year_scolor=oldcfg->year_scolor;
  cfg->year_ct=oldcfg->year_ct;
  cfg->genre_show=oldcfg->genre_show;
  cfg->genre_font=oldcfg->genre_font;
  cfg->genre_rc=oldcfg->genre_rc;
  cfg->genre_color=oldcfg->genre_color;
  cfg->genre_scolor=oldcfg->genre_scolor;
  cfg->genre_ct=oldcfg->genre_ct;
  cfg->trackn_show=oldcfg->trackn_show;
  cfg->trackn_font=oldcfg->trackn_font;
  cfg->trackn_rc=oldcfg->trackn_rc;
  cfg->trackn_color=oldcfg->trackn_color;
  cfg->trackn_scolor=oldcfg->trackn_scolor;
  cfg->trackn_ct=oldcfg->trackn_ct;
  memcpy(cfg->trackn_mask, oldcfg->trackn_mask,49*2);
  cfg->tracks_show=oldcfg->tracks_show;
  cfg->tracks_font=oldcfg->tracks_font;
  cfg->tracks_rc=oldcfg->tracks_rc;
  cfg->tracks_color=oldcfg->tracks_color;
  cfg->tracks_scolor=oldcfg->tracks_scolor;
  cfg->tracks_ct=oldcfg->tracks_ct;
  memcpy(cfg->tracks_mask, oldcfg->tracks_mask,49*2);
  cfg->ctime_show=oldcfg->ctime_show;
  cfg->ctime_font=oldcfg->ctime_font;
  cfg->ctime_rc=oldcfg->ctime_rc;
  cfg->ctime_color=oldcfg->ctime_color;
  cfg->ctime_scolor=oldcfg->ctime_scolor;
  cfg->ctime_ct=oldcfg->ctime_ct;
  memcpy(cfg->ctime_mask, oldcfg->ctime_mask,49*2);
  cfg->ftime_show=oldcfg->ftime_show;
  cfg->ftime_font=oldcfg->ftime_font;
  cfg->ftime_rc=oldcfg->ftime_rc;
  cfg->ftime_color=oldcfg->ftime_color;
  cfg->ftime_scolor=oldcfg->ftime_scolor;
  cfg->ftime_ct=oldcfg->ftime_ct;
  memcpy(cfg->ftime_mask, oldcfg->ftime_mask,49*2);
  cfg->ltime_show=oldcfg->ltime_show;
  cfg->ltime_font=oldcfg->ltime_font;
  cfg->ltime_rc=oldcfg->ltime_rc;
  cfg->ltime_color=oldcfg->ltime_color;
  cfg->ltime_scolor=oldcfg->ltime_scolor;
  cfg->ltime_ct=oldcfg->ltime_ct;
  memcpy(cfg->ltime_mask, oldcfg->ltime_mask,49*2);
  cfg->bitrate_show=oldcfg->bitrate_show;
  cfg->bitrate_font=oldcfg->bitrate_font;
  cfg->bitrate_rc=oldcfg->bitrate_rc;
  cfg->bitrate_color=oldcfg->bitrate_color;
  cfg->bitrate_scolor=oldcfg->bitrate_scolor;
  cfg->bitrate_ct=oldcfg->bitrate_ct;
  memcpy(cfg->bitrate_mask, oldcfg->bitrate_mask,49*2);
  memcpy(cfg->bitrate_vbr, oldcfg->bitrate_vbr,49*2);
  cfg->freq_show=oldcfg->freq_show;
  cfg->freq_font=oldcfg->freq_font;
  cfg->freq_rc=oldcfg->freq_rc;
  cfg->freq_color=oldcfg->freq_color;
  cfg->freq_scolor=oldcfg->freq_scolor;
  cfg->freq_ct=oldcfg->freq_ct;
  memcpy(cfg->freq_mask, oldcfg->freq_mask,49*2);
  cfg->ch_show=oldcfg->ch_show;
  cfg->ch_font=oldcfg->ch_font;
  cfg->ch_rc=oldcfg->ch_rc;
  cfg->ch_color=oldcfg->ch_color;
  cfg->ch_scolor=oldcfg->ch_scolor;
  cfg->ch_ct=oldcfg->ch_ct;
  cfg->ProgressBar_Show=oldcfg->ProgressBar_Show;
  cfg->pbrect=oldcfg->pbrect;
  cfg->ProgressBar_cColor=oldcfg->ProgressBar_cColor;
  cfg->ProgressBar_oColor=oldcfg->ProgressBar_oColor;
  cfg->ProgressBar_bColor=oldcfg->ProgressBar_bColor;
  cfg->ProgressBar_highlight=oldcfg->ProgressBar_highlight;
  cfg->cover_show=oldcfg->cover_show;
  cfg->cover_rect=oldcfg->cover_rect;
  cfg->ticn_show=oldcfg->ticn_show;
  cfg->ticn_rect=oldcfg->ticn_rect;
  cfg->alicn_show=oldcfg->alicn_show;
  cfg->alicn_rect=oldcfg->alicn_rect;
  cfg->aricn_show=oldcfg->aricn_show;
  cfg->aricn_rect=oldcfg->aricn_rect;
  cfg->pls_show=oldcfg->pls_show;
  cfg->pls_rect=oldcfg->pls_rect;
  cfg->eq_preset_show=oldcfg->eq_preset_show;
  cfg->eq_preset_rect=oldcfg->eq_preset_rect;
  cfg->shuffle_show=oldcfg->shuffle_show;
  cfg->shuffle_rect=oldcfg->shuffle_rect;
  cfg->repeat_show=oldcfg->repeat_show;
  cfg->repeat_rect=oldcfg->repeat_rect;
  cfg->stereo_show=oldcfg->stereo_show;
  cfg->stereo_rect=oldcfg->stereo_rect;
  cfg->navigimg_show=oldcfg->navigimg_show;
  cfg->navigimg_rect=oldcfg->navigimg_rect;
  cfg->leftimg_show=oldcfg->leftimg_show;
  cfg->leftimg_rect=oldcfg->leftimg_rect;
  cfg->rightimg_show=oldcfg->rightimg_show;
  cfg->rightimg_rect=oldcfg->rightimg_rect;
  cfg->centerimg_show=oldcfg->centerimg_show;
  cfg->centerimg_rect=oldcfg->centerimg_rect;
  cfg->upimg_show=oldcfg->upimg_show;
  cfg->upimg_rect=oldcfg->upimg_rect;
  cfg->downimg_show=oldcfg->downimg_show;
  cfg->downimg_rect=oldcfg->downimg_rect;
  cfg->title_show_h=oldcfg->title_show_h;
  cfg->title_font_h=oldcfg->title_font_h;
  cfg->title_rc_h=oldcfg->title_rc_h;
  cfg->title_color_h=oldcfg->title_color_h;
  cfg->title_scolor_h=oldcfg->title_scolor_h;
  cfg->title_ct_h=oldcfg->title_ct_h;
  cfg->album_show_h=oldcfg->album_show_h;
  cfg->album_font_h=oldcfg->album_font_h;
  cfg->album_rc_h=oldcfg->album_rc_h;
  cfg->album_color_h=oldcfg->album_color_h;
  cfg->album_scolor_h=oldcfg->album_scolor_h;
  cfg->album_ct_h=oldcfg->album_ct_h;
  cfg->artist_show_h=oldcfg->artist_show_h;
  cfg->artist_font_h=oldcfg->artist_font_h;
  cfg->artist_rc_h=oldcfg->artist_rc_h;
  cfg->artist_color_h=oldcfg->artist_color_h;
  cfg->artist_scolor_h=oldcfg->artist_scolor_h;
  cfg->artist_ct_h=oldcfg->artist_ct_h;
  cfg->year_show_h=oldcfg->year_show_h;
  cfg->year_font_h=oldcfg->year_font_h;
  cfg->year_rc_h=oldcfg->year_rc_h;
  cfg->year_color_h=oldcfg->year_color_h;
  cfg->year_scolor_h=oldcfg->year_scolor_h;
  cfg->year_ct_h=oldcfg->year_ct_h;
  cfg->genre_show_h=oldcfg->genre_show_h;
  cfg->genre_font_h=oldcfg->genre_font_h;
  cfg->genre_rc_h=oldcfg->genre_rc_h;
  cfg->genre_color_h=oldcfg->genre_color_h;
  cfg->genre_scolor_h=oldcfg->genre_scolor_h;
  cfg->genre_ct_h=oldcfg->genre_ct_h;
  cfg->trackn_show_h=oldcfg->trackn_show_h;
  cfg->trackn_font_h=oldcfg->trackn_font_h;
  cfg->trackn_rc_h=oldcfg->trackn_rc_h;
  cfg->trackn_color_h=oldcfg->trackn_color_h;
  cfg->trackn_scolor_h=oldcfg->trackn_scolor_h;
  cfg->trackn_ct_h=oldcfg->trackn_ct_h;
  memcpy(cfg->trackn_mask_h, oldcfg->trackn_mask_h,49*2);
  cfg->tracks_show_h=oldcfg->tracks_show_h;
  cfg->tracks_font_h=oldcfg->tracks_font_h;
  cfg->tracks_rc_h=oldcfg->tracks_rc_h;
  cfg->tracks_color_h=oldcfg->tracks_color_h;
  cfg->tracks_scolor_h=oldcfg->tracks_scolor_h;
  cfg->tracks_ct_h=oldcfg->tracks_ct_h;
  memcpy(cfg->tracks_mask_h, oldcfg->tracks_mask_h,49*2);
  cfg->ctime_show_h=oldcfg->ctime_show_h;
  cfg->ctime_font_h=oldcfg->ctime_font_h;
  cfg->ctime_rc_h=oldcfg->ctime_rc_h;
  cfg->ctime_color_h=oldcfg->ctime_color_h;
  cfg->ctime_scolor_h=oldcfg->ctime_scolor_h;
  cfg->ctime_ct_h=oldcfg->ctime_ct_h;
  memcpy(cfg->ctime_mask_h, oldcfg->ctime_mask_h,49*2);
  cfg->ftime_show_h=oldcfg->ftime_show_h;
  cfg->ftime_font_h=oldcfg->ftime_font_h;
  cfg->ftime_rc_h=oldcfg->ftime_rc_h;
  cfg->ftime_color_h=oldcfg->ftime_color_h;
  cfg->ftime_scolor_h=oldcfg->ftime_scolor_h;
  cfg->ftime_ct_h=oldcfg->ftime_ct_h;
  memcpy(cfg->ftime_mask_h, oldcfg->ftime_mask_h,49*2);
  cfg->ltime_show_h=oldcfg->ltime_show_h;
  cfg->ltime_font_h=oldcfg->ltime_font_h;
  cfg->ltime_rc_h=oldcfg->ltime_rc_h;
  cfg->ltime_color_h=oldcfg->ltime_color_h;
  cfg->ltime_scolor_h=oldcfg->ltime_scolor_h;
  cfg->ltime_ct_h=oldcfg->ltime_ct_h;
  memcpy(cfg->ltime_mask_h, oldcfg->ltime_mask_h,49*2);
  cfg->bitrate_show_h=oldcfg->bitrate_show_h;
  cfg->bitrate_font_h=oldcfg->bitrate_font_h;
  cfg->bitrate_rc_h=oldcfg->bitrate_rc_h;
  cfg->bitrate_color_h=oldcfg->bitrate_color_h;
  cfg->bitrate_scolor_h=oldcfg->bitrate_scolor_h;
  cfg->bitrate_ct_h=oldcfg->bitrate_ct_h;
  memcpy(cfg->bitrate_mask_h, oldcfg->bitrate_mask_h,49*2);
  memcpy(cfg->bitrate_vbr_h, oldcfg->bitrate_vbr_h,49*2);
  cfg->freq_show_h=oldcfg->freq_show_h;
  cfg->freq_font_h=oldcfg->freq_font_h;
  cfg->freq_rc_h=oldcfg->freq_rc_h;
  cfg->freq_color_h=oldcfg->freq_color_h;
  cfg->freq_scolor_h=oldcfg->freq_scolor_h;
  cfg->freq_ct_h=oldcfg->freq_ct_h;
  memcpy(cfg->freq_mask_h, oldcfg->freq_mask_h,49*2);
  cfg->ch_show_h=oldcfg->ch_show_h;
  cfg->ch_font_h=oldcfg->ch_font_h;
  cfg->ch_rc_h=oldcfg->ch_rc_h;
  cfg->ch_color_h=oldcfg->ch_color_h;
  cfg->ch_scolor_h=oldcfg->ch_scolor_h;
  cfg->ch_ct_h=oldcfg->ch_ct_h;
  cfg->ProgressBar_Show_h=oldcfg->ProgressBar_Show_h;
  cfg->pbrect_h=oldcfg->pbrect_h;
  cfg->ProgressBar_cColor_h=oldcfg->ProgressBar_cColor_h;
  cfg->ProgressBar_oColor_h=oldcfg->ProgressBar_oColor_h;
  cfg->ProgressBar_bColor_h=oldcfg->ProgressBar_bColor_h;
  cfg->ProgressBar_highlight_h=oldcfg->ProgressBar_highlight_h;
  cfg->cover_show_h=oldcfg->cover_show_h;
  cfg->cover_rect_h=oldcfg->cover_rect_h;
  cfg->ticn_show_h=oldcfg->ticn_show_h;
  cfg->ticn_rect_h=oldcfg->ticn_rect_h;
  cfg->alicn_show_h=oldcfg->alicn_show_h;
  cfg->alicn_rect_h=oldcfg->alicn_rect_h;
  cfg->aricn_show_h=oldcfg->aricn_show_h;
  cfg->aricn_rect_h=oldcfg->aricn_rect_h;
  cfg->pls_show_h=oldcfg->pls_show_h;
  cfg->pls_rect_h=oldcfg->pls_rect_h;
  cfg->eq_preset_show_h=oldcfg->eq_preset_show_h;
  cfg->eq_preset_rect_h=oldcfg->eq_preset_rect_h;
  cfg->shuffle_show_h=oldcfg->shuffle_show_h;
  cfg->shuffle_rect_h=oldcfg->shuffle_rect_h;
  cfg->repeat_show_h=oldcfg->repeat_show_h;
  cfg->repeat_rect_h=oldcfg->repeat_rect_h;
  cfg->stereo_show_h=oldcfg->stereo_show_h;
  cfg->stereo_rect_h=oldcfg->stereo_rect_h;
  cfg->navigimg_show_h=oldcfg->navigimg_show_h;
  cfg->navigimg_rect_h=oldcfg->navigimg_rect_h;
  cfg->leftimg_show_h=oldcfg->leftimg_show_h;
  cfg->leftimg_rect_h=oldcfg->leftimg_rect_h;
  cfg->rightimg_show_h=oldcfg->rightimg_show_h;
  cfg->rightimg_rect_h=oldcfg->rightimg_rect_h;
  cfg->centerimg_show_h=oldcfg->centerimg_show_h;
  cfg->centerimg_rect_h=oldcfg->centerimg_rect_h;
  cfg->upimg_show_h=oldcfg->upimg_show_h;
  cfg->upimg_rect_h=oldcfg->upimg_rect_h;
  cfg->downimg_show_h=oldcfg->downimg_show_h;
  cfg->downimg_rect_h=oldcfg->downimg_rect_h;
  return 1;
};
