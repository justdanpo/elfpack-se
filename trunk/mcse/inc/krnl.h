#ifndef _KRNL_H
#define _KRNL_H


#define MAX_PATH		0x100
#define MAX_EXT			8

typedef struct
{
	short id;
	int	ftype;
        short attr;
	unsigned int size;
	unsigned int csize;
	unsigned int time;
	int	uccnt;
	int	inited;
	wchar_t* ws_name;
	wchar_t* ws_showname;
        wchar_t icon;
	TEXTID ws_attr;
	TEXTID ws_size;
	TEXTID ws_time;
	TEXTID ws_ratio;
	TEXTID ws_short;
        TEXTID sid_name;
	void* next;
} FILEINF;

extern int progr_stop;
extern int progr_act;
extern int progr_max;
extern int progr_cur;
extern int progr_start;
extern int progrsp_max;
extern int progrsp_cur;
extern int progrsp_start;

wchar_t* MCFilePath(const wchar_t* sfile);
void AddFile(int tab, int findex, const wchar_t *fname, unsigned int fsize, short fattr,
				  unsigned int ftime, int fcsize, int ftype);
void DelFiles(int tab);
int InitTab(int tab);
void FreeTab(int tab);

void initprogr(int act);
void incprogr(int inc);
void incprogrsp(int inc);
void endprogr();
void endprogrsp();

extern void DoErrKey();
extern wchar_t mcpath[MAX_PATH];
extern wchar_t pathbuf[MAX_PATH];
extern wchar_t arcpathbuf[MAX_PATH];
extern wchar_t wsbuf[MAX_PATH*2];
extern wchar_t szLastNewFile[MAX_PATH];
extern wchar_t szLastNewDir[MAX_PATH];
extern wchar_t msgbuf[256];

void FillFileInfo(FILEINF *file);
void _cd_tab(int tab, int drv, const wchar_t *dname);
int SetTabDrv(int tab, int num);
void SetTabIndex(int tab, int num, int slide);
wchar_t* CurFullPath(wchar_t* sfile);
wchar_t* CurFullPathBM(wchar_t* buff, wchar_t* sfile, int len);
void SetCurTabIndex(int num, int slide);
int DoRefresh();
void UpdateAll();
int GetCurTabFileIndex(wchar_t* fname);
FILEINF* _CurFile();

extern wchar_t in_open_path[MAX_PATH];
void ExitFromZip();
void S_ZipOpen(void);
void S_7ZOpen(void);

typedef void (*IFN_RET_PROC) (wchar_t *wsname);
void TextInput(wchar_t *prmpt, int TestFileName, const wchar_t *wsname, IFN_RET_PROC retproc);


void DoExecFile(const wchar_t *path, const wchar_t *fname);

typedef int (*ENUM_SEL_PROC) (FILEINF *file, int param);
int EnumChk(ENUM_SEL_PROC EnumProc, int param);
int EnumSel(ENUM_SEL_PROC EnumProc, int param);

void CB_Cancel(int id);
void DestroySendList(LIST *lst);

void DoSwapTab();
void DoPrvDrv();
void DoNxtDrv();
void DoUp();
void DoDwn();
void DoSortN();
void DoSortE();
void DoSortS();
void DoSortD();
void DoSetSort();
void DoSortR();
void DoOpen();
void DoSysOpen();
void DoBackK();
void DoChk();
void DoChkAll();
void DoUnCAll();
void DoInvChk();
void DoDel();
void DoMenu();
void DoExit();
void DoPaste();
void DoCopy();
void DoNewDir();
void DoCancel();
void DoFilter();
void DoMove();
void DoRen();
void DoPgUp();
void DoPgDwn();
void DoTabCopy();
void DoTabMove();
void DoRoot();
void DoShowPath();
void DoBegin();
void DoSendFile();
#endif
