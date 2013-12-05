#include "inc\mc.h"
#include "inc\lng.h"
#include "inc\mui.h"

const wchar_t mclg_deffile[]=L"default.mclg";
const wchar_t mclg_ext[]=L"mclg";

const MUI_STR mui[]={
	{id_no, sz_no},
	{id_yes, sz_yes},

	{id_byte, sz_byte},
	{id_kbyte, sz_kbyte},
	{id_mbyte, sz_mbyte},

	{id_reconfig, sz_reconfig},
	{id_lng, sz_lng},
	{id_lngname, sz_lngname},

	{id_dirmking, sz_dirmking},
	{id_copying, sz_copying},
	{id_moveing, sz_moveing},
	{id_deling, sz_deling},

	{id_mcmenu, sz_mcmenu},
	{id_select, sz_select},
	{id_back, sz_back},

	{id_open, sz_open},
	{id_file_m, sz_file_m},
	{id_chk, sz_chk},
	{id_chkall, sz_chkall},
	{id_invchk, sz_invchk},
	{id_unchall, sz_unchall},
	{id_oper_m, sz_oper_m},
	{id_past, sz_past},
	{id_cancel, sz_cancel},
	{id_copy, sz_copy},
	{id_move, sz_move},
	{id_del, sz_del},
	{id_rename, sz_rename},
	{id_newdir, sz_newdir},
	{id_prop, sz_prop},
	{id_drvinf, sz_drvinf},
	{id_view_m, sz_view_m},
	{id_sort_m, sz_sort_m},
	{id_sortn, sz_sortn},
	{id_sorte, sz_sorte},
	{id_sorts, sz_sorts},
	{id_sortd, sz_sortd},
	{id_sortr, sz_sortr},
	{id_filter, sz_filter},
	{id_refresh, sz_refresh},
	{id_newfile, sz_newfile},
	{id_bm_m, sz_bm_m},
	{id_bm, sz_bm},
	{id_add, sz_add},
	{id_bml, sz_bml},
	{id_misc_m, sz_misc_m},
	{id_settings, sz_settings},
	{id_about_m, sz_about_m},
	{id_exit, sz_exit},
        
        {id_sendfile, sz_sendfile},

	{id_name, sz_name},
	{id_fullname, sz_fullname},
	{id_size, sz_size},
	{id_global, sz_global},
	{id_folders, sz_folders},
	{id_files, sz_files},
	{id_date, sz_date},
	{id_attr, sz_attr},
	{id_readonly, sz_readonly},
	{id_hidden, sz_hidden},
	{id_system, sz_system},
	{id_arh, sz_arh},
	{id_subdirs, sz_subdirs},
	{id_ncsize, sz_ncsize},
	{id_csize, sz_csize},
	{id_cglobal, sz_cglobal},

	{id_pmt_del, sz_pmt_del},
	{id_pmt_copy, sz_pmt_copy},
	{id_pmt_move, sz_pmt_move},
	{id_pmt_stop, sz_pmt_stop},
	{id_pmt_cancel, sz_pmt_cancel},
	{id_pmt_exists, sz_pmt_exists},
	{id_pmt_exit, sz_pmt_exit},
	{id_pmt_impcs, sz_pmt_impcs},
	{id_pmt_implg, sz_pmt_implg},
	{id_pmt_rodel, sz_pmt_rodel},

	{id_err_attrset, sz_err_attrset},
	{id_err_makedir, sz_err_makedir},
	{id_err_rename, sz_err_rename},
	{id_err_delete, sz_err_delete},
	{id_err_badname_t, sz_err_badname_t},
	{id_err_baddrv_t, sz_err_baddrv_t},
	{id_err_resnok, sz_err_resnok},
	{id_err_nofiles, sz_err_nofiles},
	{id_err_badname, sz_err_badname},
	{id_err_badkey, sz_err_badkey},
	{id_err_badformat, sz_err_badformat},
	{id_err_nomemory, sz_err_nomemory},
	{id_err_makefile, sz_err_makefile},
	{id_err_ziperr, sz_err_ziperr},

	{id_msg_zreading, sz_msg_zreading},
	{id_msg_showhid_files, sz_msg_showhid_files},
	{id_msg_showsys_files, sz_msg_showsys_files},
	{id_msg_showhidsys_files, sz_msg_showhidsys_files},
	{id_msg_showhid_drv, sz_msg_showhid_drv},
};

#define MUI_COUNT		 (sizeof(mui)/sizeof(MUI_STR))

wchar_t* mui_ld[MUI_COUNT];

int getmuiind(int id)
{
	for(int cc=0; cc < MUI_COUNT; cc++)
		if (mui[cc].id == id) return cc;
	return -1;  
}

wchar_t* muitxt(int ind)
{
	if (ind < 0 || ind >= MUI_COUNT) return NULL;
	return mui_ld[ind] ? mui_ld[ind] : mui[ind].df_str;
}

wchar_t* muibuff=NULL;
int muibuffsz=0;


void SaveMUI(wchar_t* filename)
{
  wchar_t *fn;
  if (filename)
  {
    fn=filename;
  }
  else
  {
    if (!w_chdir(mcpath))
      fn=(wchar_t *)mclg_deffile;
  }
  int f;

  if (fn && (f = w_fopen(fn, WA_Read+WA_Write+WA_Create+WA_Truncate, 0x1FF, NULL)) >=0)
  {
    MCLG_hdr hdr;
    hdr.sig=mclg_sig;
    hdr.ver=mclg_ver;
    //	hdr.size=mclg_size;
    hdr.offset=mclg_offset;
    
    int bufsz=0;
    for(int cc=0;cc<MUI_COUNT;cc++)
      bufsz+=wstrlen(muitxt(cc))+1;
    wchar_t* buff;
    wchar_t* lp;
    lp=buff=new wchar_t[bufsz];
    
    hdr.size=mclg_size | (bufsz<<16);
    MC_LG* lgtbl;
    MC_LG* lg;
    lg=lgtbl=new MC_LG[MUI_COUNT];
    for(int cc=0;cc<MUI_COUNT;cc++)
    {
      lg->id=mui[cc].id;
      lg->ofs=lp-buff;
      wstrcpy(lp, muitxt(cc));
      lp+=wstrlen(muitxt(cc))+1;
      lg++;
    }
    w_fwrite(f,&hdr,sizeof(hdr));
    w_fwrite(f,lgtbl,MUI_COUNT*sizeof(MC_LG));
    w_fwrite(f,buff,bufsz*sizeof(wchar_t));
    delete (lgtbl);
    delete (buff);
    w_fclose(f);
  }
}


int LoadMUI(wchar_t* filename)
{
  int res = 0;
  int v1=0;
  wchar_t *fn;
  if (filename)
  {
    fn=filename;
  }
  else
  {
    if (!w_chdir(mcpath))
      fn=(wchar_t *)mclg_deffile;
  }
  int f;
  if (fn && (f = w_fopen(fn, WA_Read, 0x1FF, NULL)) >=0)
  {
    MCLG_hdr hdr;
    if (w_fread(f, &hdr, sizeof(hdr))==sizeof(hdr))
    {
      if (hdr.sig==mclg_sig)
      {
        w_lseek(f,hdr.offset,WSEEK_SET);
        int tblcn=(hdr.size & 0xffff);
        int tblsz=tblcn*sizeof(MC_LG);
        int bufsz=hdr.size>>16;
        FreeMUI();
        
        muibuff=new wchar_t[bufsz];
        muibuffsz=bufsz*sizeof(wchar_t);
        MC_LG* tbl;
        MC_LG* lg;
        lg=tbl=new MC_LG[tblcn];
        
        if (hdr.ver==mclg_ver)
        {
          res = 
            (w_fread(f, tbl, tblsz)==tblsz) &&
              (w_fread(f, muibuff, bufsz*sizeof(wchar_t))==bufsz*sizeof(wchar_t));
          if (res)
          {
            for(int cc=0;cc<tblcn;cc++)
            {
              int ind=getmuiind(lg->id);
              mui_ld[ind]=muibuff + lg->ofs;
              lg++;
            }
          }
          else
          {
            delete(muibuff);
            muibuff=NULL;
          }
        } 
        else if (hdr.ver==2)
        {
          char *oldm=new char[bufsz];
          res = 
            (w_fread(f, tbl, tblsz)==tblsz) &&
              (w_fread(f, oldm, bufsz)==bufsz);
          if (res)
          {
            for(int cc=0;cc<tblcn;cc++)
            {
              int ind=getmuiind(lg->id);
              wchar_t *tx=muibuff + lg->ofs;
              win12512unicode(tx,oldm+lg->ofs,strlen(oldm+lg->ofs));
              mui_ld[ind]=tx;
              lg++;
            }
          }
          else
          {
            delete(muibuff);
            muibuff=NULL;
          }
          delete oldm;  
        }
        delete(tbl);
      }
      else v1=1;
    }
    w_fclose(f);
  }
  int LoadMUIold(wchar_t* fname);
  if (v1) res=LoadMUIold(filename);
  return res;
}

void freeb(wchar_t* buf)
{
  if (buf)
    if ((int)buf<(int)muibuff || (int)buf>=(int)muibuff+muibuffsz)
      delete(buf);
}

void FreeMUI()
{
  for(int cc=0;cc<MUI_COUNT;cc++)
    if (mui_ld[cc])
    {
      freeb(mui_ld[cc]);
      mui_ld[cc]=NULL;
    }
  if (muibuff)
  {
    delete(muibuff);
    muibuff=NULL;
  }
}

void MUIProc(char *name, char *value, int is_utf)
{
  int id=strtol(name,0,10);
  int ind=getmuiind(id);
  int len;
  if (ind<0)return;
  
  freeb(mui_ld[ind]);
  wchar_t* buf=NULL; 
  len=strlen(value);
  if (!is_utf)
  {
    buf= new wchar_t[len+1];
    win12512unicode(buf,value,len);
  }
  else
  {
    int ulen=utf8_to_utf16(value,len,NULL);
    if (ulen!=-1)
    {
      buf= new wchar_t[ulen+1];
      ulen=utf8_to_utf16(value,len,buf);
      if (ulen!=-1) buf[ulen]=0; else buf[0]=0;
    }
  }
  for(int cc=0;buf[cc];cc++)
    if (buf[cc]==L'^')buf[cc]=L'\n';
  mui_ld[ind] = buf;
}

int LoadMUIold(wchar_t* fname)
{
  int l;
  wchar_t* fn;
  if (fname)
  {
    l=0;
    fn=fname;
  }
  else
  {
    l=1;
    fn=(wchar_t *)mclg_deffile;
  }
  int res = EnumIni(l, fn, MUIProc);
  return res;
}


