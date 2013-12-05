#ifndef _MUI_H
#define _MUI_H

#define id_no				1
#define id_yes				2

#define id_byte				10
#define id_kbyte			11
#define id_mbyte			12

#define id_reconfig			50
#define id_lng				51
#define id_lngname			52

#define id_dirmking			100
#define id_copying			101
#define id_moveing			102
#define id_deling			103

#define id_mcmenu			1000
#define id_select			1001
#define id_back				1002

#define id_open				1010
#define id_file_m			1020
#define id_chk				1021
#define id_chkall			1022
#define id_invchk			1024
#define id_unchall			1023
#define id_oper_m			1030
#define id_past				1031
#define id_cancel			1037
#define id_copy				1032
#define id_move				1033
#define id_del				1034
#define id_rename			1035
#define id_newdir			1036
#define id_newfile			1038
#define id_prop				1040
#define id_drvinf			1050
#define id_view_m			1090
#define id_sort_m			1080
#define id_sortn			1081
#define id_sorte			1082
#define id_sorts			1083
#define id_sortd			1084
#define id_sortr			1085
#define id_filter			1092
#define id_refresh			1091
#define id_bm_m				1060
#define id_bm				1061
#define id_add				1062
#define id_bml				1063
#define id_misc_m			1070
#define id_settings			1071
#define id_about_m			1072
#define id_exit				1100

#define id_sendfile                     1200

#define id_name				1500
#define id_fullname			1501
#define id_size				1502
#define id_global			1511
#define id_folders			1512
#define id_files			1513
#define id_date				1503
#define id_attr				1504
#define id_readonly			1505
#define id_hidden			1506
#define id_system			1507
#define id_arh				1508
#define id_subdirs			1509
#define id_ncsize			1514
#define id_csize			1515
#define id_cglobal			1516

#define id_pmt_del			3000
#define id_pmt_copy			3001
#define id_pmt_move			3002
#define id_pmt_stop			3003
#define id_pmt_cancel		3008
#define id_pmt_exists		3004
#define id_pmt_exit			3005
#define id_pmt_impcs		3006
#define id_pmt_implg		3007
#define id_pmt_rodel		3009

#define id_err_attrset		4001
#define id_err_makedir		4002
#define id_err_rename		4003
#define id_err_delete		4004
#define id_err_badname_t	4005
#define id_err_baddrv_t		4006
#define id_err_resnok		4007
#define id_err_nofiles		4008
#define id_err_badname		4009
#define id_err_badkey		4010
#define id_err_badformat	4011
#define id_err_nomemory		4012
#define id_err_makefile		4013
#define id_err_ziperr		4014

#define id_msg_zreading			5000
#define id_msg_showhid_files	5001
#define id_msg_showsys_files	5002
#define id_msg_showhidsys_files	5003
#define id_msg_showhid_drv		5004

typedef struct
{
	int id;
	wchar_t* df_str;
}MUI_STR;

typedef struct
{
	unsigned short id;
	unsigned short ofs;
}MC_LG;

#define MCLG_hdr mccfg_hdr

#define mclg_sig (*(int*)"mclg")
#define mclg_ver 3
#define mclg_size MUI_COUNT
#define mclg_offset sizeof(MCLG_hdr)

extern const wchar_t mclg_deffile[];
extern const wchar_t mclg_ext[];

void SaveMUI(wchar_t* filename);
int LoadMUI(wchar_t* filename);
void FreeMUI();
wchar_t* muitxt(int ind);





enum lng_ids {
  ind_no=0,
  ind_yes,
  ind_byte,
  ind_kbyte,
  ind_mbyte,

  ind_reconfig,
  ind_lng,
  ind_lngname,

  ind_dirmking,
  ind_copying,
  ind_moveing,
  ind_deling,

  ind_mcmenu,
  ind_select,
  ind_back,

  ind_open,
  ind_file_m,
  ind_chk,
  ind_chkall,
  ind_invchk,
  ind_unchall,
  ind_oper_m,
  ind_past,
  ind_cancel,
  ind_copy,
  ind_move,
  ind_del,
  ind_rename,
  ind_newdir,
  ind_prop,
  ind_drvinf,
  ind_view_m,
  ind_sort_m,
  ind_sortn,
  ind_sorte,
  ind_sorts,
  ind_sortd,
  ind_sortr,
  ind_filter,
  ind_refresh,
  ind_newfile,
  ind_bm_m,
  ind_bm,
  ind_add,
  ind_bml,
  ind_misc_m,
  ind_settings,
  ind_about_m,
  ind_exit,

  ind_sendfile,

  ind_name,
  ind_fullname,
  ind_size,
  ind_global,
  ind_folders,
  ind_files,
  ind_date,
  ind_attr,
  ind_readonly,
  ind_hidden,
  ind_system,
  ind_arh,
  ind_subdirs,
  ind_ncsize,
  ind_csize,
  ind_cglobal,

  ind_pmt_del,
  ind_pmt_copy,
  ind_pmt_move,
  ind_pmt_stop,
  ind_pmt_cancel,
  ind_pmt_exists,
  ind_pmt_exit,
  ind_pmt_impcs,
  ind_pmt_implg,
  ind_pmt_rodel,

  ind_err_attrset,
  ind_err_makedir,
  ind_err_rename,
  ind_err_delete,
  ind_err_badname_t,
  ind_err_baddrv_t,
  ind_err_resnok,
  ind_err_nofiles,
  ind_err_badname,
  ind_err_badkey,
  ind_err_badformat,
  ind_err_nomemory,
  ind_err_makefile,
  ind_err_ziperr,

  ind_msg_zreading,
  ind_msg_showhid_files,
  ind_msg_showsys_files,
  ind_msg_showhidsys_files,
  ind_msg_showhid_drv
};

#endif
