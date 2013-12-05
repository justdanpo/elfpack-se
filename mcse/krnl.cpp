#include "..\\include\Dir.h"
#include "inc\mc.h"
#include "inc\lng.h"
#include "inc\zslib.h"
#include "inc\krnl.h"
#include "inc\exts.h"
#include "inc\gui.h"
#include "inc\7zslib.h"
#include "inc\menus.h"
#include "inc\ColorMap.h"
#include "inc\mui.h"
#include "inc\bookmarks.h"
#include "inc\file_op.h"

wchar_t msgbuf[256];
wchar_t mcpath[MAX_PATH];
wchar_t pathbuf[MAX_PATH];
wchar_t arcpathbuf[MAX_PATH];
wchar_t szLastNewFile[MAX_PATH];
wchar_t szLastNewDir[MAX_PATH];
wchar_t wsbuf[MAX_PATH*2];
int back_tab;

const wchar_t def_filter[] = L"*.*";

int progr_stop  = 0;
int progr_act   = 0;

int progr_max   = 0;
int progr_cur   = 0;
int progr_start = 0;

int progrsp_max = 0;
int progrsp_cur = 0;
int progrsp_start = 0;

wchar_t in_open_path[MAX_PATH];





DRVINFO Drives[MAX_DRV] = {
	{NULL, L"",  0},
	{NULL, L"",  0},
	{NULL, L"",  0},
};

int curtab=0;
TABINFO* tabs[MAX_TABS+1];
FILEINF* FileListBase[MAX_TABS+1];


wchar_t* MCFilePath(const wchar_t* sfile)
{
  snwprintf(pathbuf,MAXELEMS(pathbuf)-1, _ls_ls, mcpath, sfile);
  return pathbuf;
}

void SetTabIndex(int tab, int num, int slide)
{
  TABINFO *tmp = tabs[tab];
  
  if (tmp->ccFiles == 0)
  {
    num = -1;
  }
  else
  {
    if (slide)
    {
      if ( num >= tmp->ccFiles) num = 0;
      else if (num < 0) num = tmp->ccFiles - 1;
    }
    else
    {
      if (num >= tmp->ccFiles) num = tmp->ccFiles - 1;
      else if (num < 0) num = 0;
    }
  }
  tmp->iIndex[tmp->CurDrv] = num;
  DisableScroll();
  RedrawGUI=1;
}

void SetCurTabIndex(int num, int slide)
{
  SetTabIndex(curtab, num, slide);
}

int GetTabIndex(int tab)
{
  return tabs[tab]->iIndex[ tabs[tab]->CurDrv ];
}

wchar_t* GetTabPath(int tab)
{
  if (IsArchiveOpened(tab))
    return tabs[tab]->zipInfo->szCurDir;
  else
    return tabs[tab]->szDirs[tabs[tab]->CurDrv];
}

void FillFileInfo(FILEINF *file)
{
  TEXTID str;
  if (!file || file->inited) return;
  
  wchar_t buf[MAX_PATH/2];
  if (file->ws_attr==LGP_NULL)
  {
    attr2s(file->attr, buf);
    str=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
    file->ws_attr=str;
  }
  if (file->ws_size==LGP_NULL)
  {
    sz2s(file->size, buf);
    str=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
    file->ws_size=str;
  }
  if (file->ws_time==LGP_NULL)
  {
    unixt2s(file->time, buf);
    str=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
    file->ws_time=str;
  }
  if (!file->icon)
  {
    file->icon = GetIcon(file->ws_name, file->attr);
  }
  if (file->ftype == TYPE_ZIP_FILE && file->ws_ratio==LGP_NULL)
  {
    if (file->size && file->csize)
    {
      char cbuf[32];
      float ratio = (float)file->csize * (float)100 / (float)file->size;
      sprintf(cbuf, "%.0f%%", ratio); // без цифр после точки
      win12512unicode(buf,cbuf,MAXELEMS(buf));
      file->ws_ratio=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
    }
  }
  wchar_t* sext = GetFileExt(file->ws_name);
  if (sext && !file->ws_showname && wstrcmpi(sext, mcbm_ext) == 0)
  {
    // Откидываем расширение
    wstrcpy(buf, file->ws_name);
    buf[sext - file->ws_name - 1] = '\0';
    file->ws_showname = new wchar_t[wstrlen(buf) + 1];
    wstrcpy(file->ws_showname,buf);
  }
  
  if (file->ws_short==LGP_NULL)
  {
    wchar_t *ws;
    if (file->ws_showname) ws=file->ws_showname;
    else ws=file->ws_name;
    file->uccnt = getLVC(ws, CONFIG_FONT_FILES);
    if (file->uccnt)
    {
      cutname(ws, buf, file->uccnt>MAX_PATH/2?MAX_PATH/2:file->uccnt);
      file->ws_short=TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN);
    }
  }
  wchar_t *ws;
  if (curtab == systab && file->ws_showname) ws=file->ws_showname;
  else ws=file->ws_name;
  file->sid_name=TextID_Create(ws,ENC_UCS2,TEXTID_ANY_LEN);
  file->inited = 1;
}

FILEINF* CreateFileInfo(int findex, const wchar_t* fpathOriginal,
				unsigned int fsize, short fattr, unsigned int ftime,
				int fcsize, int ftype)
{
	FILEINF *file = new FILEINF;
        memset(file,0,sizeof(FILEINF));
        
        wchar_t *name=new wchar_t[wstrlen(fpathOriginal)+1];
        wstrcpy(name,fpathOriginal);

	file->id		= findex;
	file->attr		= fattr;
	file->size		= fsize;
	file->time		= ftime;
        file->ws_name           = name;
	file->csize		= fcsize;
	file->ftype		= ftype;
        
        file->ws_attr=LGP_NULL;
        file->ws_size=LGP_NULL;
        file->ws_time=LGP_NULL;
        file->ws_ratio=LGP_NULL;
        file->ws_short=LGP_NULL;
        file->sid_name=LGP_NULL;
	return file;
}

void AddFile(int tab, int findex, const wchar_t *fname, unsigned int fsize, short fattr,
				  unsigned int ftime, int fcsize, int ftype)
{
	FILEINF *file = CreateFileInfo(findex, fname, fsize, fattr, ftime, fcsize, ftype);
	file->next = FileListBase[tab]->next;
	FileListBase[tab]->next = file;
}

void AddFileFromDE(int tab, int findex, const wchar_t *name, W_FSTAT *fs)
{
  AddFile(tab, findex, name , fs->st_size, fs->attr, fs->unk_0x1C, 0, fs->attr&FA_DIRECTORY ? TYPE_COMMON_DIR : TYPE_COMMON_FILE);
}


void FreeFileInfo(FILEINF* file)
{
  if (file)
  {
    if (file->ws_name)	delete(file->ws_name);
    if (file->ws_showname) delete(file->ws_showname);
    TextID_Destroy(file->ws_attr);
    TextID_Destroy(file->ws_size);
    TextID_Destroy(file->ws_time);
    TextID_Destroy(file->ws_ratio);
    TextID_Destroy(file->ws_short);
    TextID_Destroy(file->sid_name);
    delete (file);
  }
}

void DelFiles(int tab)
{
  if (tabs[tab]->ccFiles)
  {
    //scfile=NULL;
    while(FileListBase[tab]->next!=FileListBase[tab])
    {
      FILEINF *file = (FILEINF *) FileListBase[tab]->next;	// Второй элемент
      FileListBase[tab]->next = (FILEINF *) file->next;		// Следующий у FileListBase - на третий
      FreeFileInfo(file);
      tabs[tab]->ccFiles--;
    }
  }
}



int FillRealPathFiles(int tab, wchar_t* dname)
{
  W_FSTAT fs;
  wchar_t *next;
  int num = 0;
  void *handle=w_diropen(dname); 
  if (handle)
  {
    w_chdir(dname);
    while((next=w_dirread(handle)))
    {
      w_fstat(next,&fs);
      if (!tabs[tab]->szFilter[0] || fs.attr&FA_DIRECTORY)
      {
        AddFileFromDE(tab, num++, next, &fs);
      }
    }
    w_dirclose(handle);
  }
  if (tabs[tab]->szFilter[0])
  {
    handle=w_diropen(dname);
    if (handle)
    {
      w_chdir(dname);
      while((next=w_dirread(handle)))
      {
        w_fstat(next,&fs);
        if (!(fs.attr&FA_DIRECTORY))
        {
          if (match(tabs[tab]->szFilter, next))
          {
            AddFileFromDE(tab, num++, next, &fs);
          }
        }
      }
      w_dirclose(handle);
    }
  }
  return num;
}

// Заполняет список файлов текущей директории
int FillFiles(int tab, wchar_t* dname) 
{
  if (tabs[tab]->ccFiles) DelFiles(tab);
  if (IsArchiveOpened(tab)==ZIP_ARCH)
    tabs[tab]->ccFiles = FillZipFiles(tab, dname);
  else if (IsArchiveOpened(tab)==_7Z_ARCH)
    tabs[tab]->ccFiles = Fill7ZFiles(tab, dname);  
  else
    tabs[tab]->ccFiles = FillRealPathFiles(tab, dname);
  SortFiles(tab);
  return tabs[tab]->ccFiles;
}

int SetTabDrv(int tab, int num)
{
  if (num >= MAX_DRV) num = 0;
  else if (num < 0) num = MAX_DRV - 1;
  tabs[tab]->CurDrv = num;
  return FillFiles(tab, GetTabPath(tab));
}

int SetCurTabDrv(int num)
{
  return SetTabDrv(curtab, num);
}


FILEINF* _CurTabFile(int tab)
{
  int ind = GetTabIndex(tab);
  if (ind < 0) return NULL;
  
  FILEINF* file = FileListBase[tab];
  for(int ii=0; ii<=ind; ii++)
    if (file)
      file =  (FILEINF *)file->next;
    else
      return NULL;
  return file;
}

int GetFileIndex(int tab, wchar_t* fname)
{
  if (tabs[tab]->ccFiles)
  {
    int ind=0;
    FILEINF* file =  (FILEINF *)FileListBase[tab]->next;
    while(file != FileListBase[tab])
    {
      if (!wstrcmp(fname, file->ws_name))
        return ind;
      file = (FILEINF *)file->next;
      ind++;
    }
  }
  return -1;
}

int GetCurTabFileIndex(wchar_t* fname)
{
  return GetFileIndex(curtab, fname);
}

int RefreshTab(int tab)
{
  FILEINF* cfile = _CurTabFile(tab);
  wchar_t* lpname;
  if (cfile)
  {
    lpname = new wchar_t[wstrlen(cfile->ws_name) + 1];
    wstrcpy(lpname, cfile->ws_name);
  }
  int res = FillFiles(tab, GetTabPath(tab));
  
  int ind;
  if (cfile)
  {
    ind = GetFileIndex(tab, lpname);
    delete(lpname);
  }
  else
    ind = 0;
  SetTabIndex(tab, ind, 0);
  return res;
}

int DoRefresh()
{
  return RefreshTab(curtab);
}

void UpdateAll()
{
  for(int ii=0; ii < MAX_TABS + 1; ii++)
    RefreshTab(ii);
}

void _cd_tab(int tab, int drv, const wchar_t *dname)
{
  if (IsArchiveOpened(tab))
  {
    if (wstrcmp(tabs[tab]->zipInfo->szCurDir, dname))
    {
      tabs[tab]->iBase[drv] = 0;
      tabs[tab]->iIndex[drv] = 0;
      wstrcpy(tabs[tab]->zipInfo->szCurDir, dname);
    }
  }
  else
  {
    if (wstrcmp(tabs[tab]->szDirs[drv], dname))
    {
      tabs[tab]->iBase[drv] = 0;
      tabs[tab]->iIndex[drv] = 0;
      wstrcpy(tabs[tab]->szDirs[drv], dname);
    }
  }
}


int InitTab(int tab)
{
  tabs[tab] = new TABINFO;
  {
    tabs[tab]->sort = ST_FIRST;
    tabs[tab]->szFilter[0]=0;
  }
  tabs[tab]->zipInfo = new ARCHINFO;
  {
    memset(tabs[tab]->zipInfo, 0, sizeof(ARCHINFO));
  }
  FileListBase[tab] = new FILEINF;
  {
    memset(FileListBase[tab],0, sizeof(FILEINF));
    FileListBase[tab]->id = -1;
    FileListBase[tab]->next = FileListBase[tab];
  }
  for(int num = 0; num < MAX_DRV; num++)
  {
    _cd_tab(tab, num, Drives[num].path);
  }
  return SetTabDrv(tab, 0);
}

void FreeTab(int tab)
{
  delete (FileListBase[tab]);
  if (IsArchiveOpened(tab)==ZIP_ARCH)
    CloseTabZip(tab);
  else if (IsArchiveOpened(tab)==_7Z_ARCH)
    CloseTab7Z(tab);
  delete (tabs[tab]->zipInfo);
  delete (tabs[tab]);
}

void DoErrKey()
{
  //MessageBox
  //MsgBoxError(1, (int) muitxt(ind_err_badkey));
}



FILEINF* _CurFile()
{
  return _CurTabFile(curtab);
}

wchar_t* CurFullPath(wchar_t* sfile)
{
  if (IsInArchive())
    snwprintf(pathbuf,MAXELEMS(pathbuf)-1, _ls_ls, _CurTab->zipInfo->szCurDir, sfile);
  else
  {
    if (_CurPath[0]=='/' && _CurPath[1]==0)
    {
      wstrcpy(pathbuf+1, sfile);
    }
    else
    {
      snwprintf(pathbuf,MAXELEMS(pathbuf)-1,_ls_ls, _CurPath, sfile);
    }
  }   
  return pathbuf;
}

wchar_t* CurFullPathBM(wchar_t* buff, wchar_t* sfile, int len)
{
  if (buff)
    snwprintf(buff, len, _ls_ls, _CurPath, sfile);
  return buff;
}

void initprogr(int act)
{
  progr_start = 1;
  progr_max = 0;
  progr_stop = 0;
  progr_act = act;
}


void incprogr(int inc)
{
  if (inc > 0) progr_cur += inc;
  else if (!inc) progr_cur = 0;
  RedrawGUI=1;
}

void incprogrsp(int inc)
{
  if (inc) progrsp_cur += inc;
  else progrsp_cur = 0;
  RedrawGUI=1;
}

void endprogr()
{
  progr_start = 0;
  RedrawGUI=1;
}

void endprogrsp()
{
  progrsp_max = progrsp_cur = 0;
  RedrawGUI=1;
}


void CB_Paste(int id)
{
  if (id == IDYES) SUBPROC((void*)S_Paste);
}

void DoPaste()
{
  if (IsInArchive()) return; // Пока не обрабатывается
  
  if (buffer.count)
  {
    if (buffer.type == FNT_MOVE)
    {
      if (CONFIG_CONFIRM_MOVE)
        MsgBoxYesNo(muitxt(ind_pmt_move), CB_Paste);
      else
        CB_Paste(IDYES);
    }
    else if (buffer.type == FNT_COPY)
    {
      if (CONFIG_CONFIRM_COPY)
        MsgBoxYesNo(muitxt(ind_pmt_copy), CB_Paste);
      else
        CB_Paste(IDYES);
    }
  }
}




void CB_CS(int id)
{
  if (id==1)
  {
    if (!LoadCS(pathbuf))
      MsgBoxError(muitxt(ind_err_badformat));
    else
    {
      SaveCS(NULL);
      RedrawGUI=1;
    }
  }
}

void CB_LG(int id)
{
  if (id==1)
  {
    if (!LoadMUI(pathbuf))
      MsgBoxError(muitxt(ind_err_badformat));
    else
    {
      SaveMUI(NULL);
      snwprintf(msgbuf, MAXELEMS(msgbuf),_lsls, muitxt(ind_lng), muitxt(ind_lngname));
      MsgBoxError(msgbuf);
    }
  }
}


void S_ZipOpen(void)
{
  if (arcpathbuf[0])
  {
    int zerr = OpenTabZip(curtab, arcpathbuf);
    if (zerr == UNZ_OK)
    {
      cd(curtab, (wchar_t *)str_empty, -1);
      SetCurTabIndex(0, 0);
    }
    else if (zerr != -11111) // ignore propr_stop
    {
      snwprintf(msgbuf, MAXELEMS(msgbuf), L"OpenZip error %i", zerr);
      MsgBoxError(msgbuf);
    }
    RedrawGUI=1;
  }
}

void S_7ZOpen(void)
{
  if (arcpathbuf[0])
  {
    int zerr = OpenTab7Z(curtab, arcpathbuf);
    if (zerr == SZ_OK)
    {
      cd(curtab, (wchar_t *)str_empty, -1);
      SetCurTabIndex(0, 0);
    }
    else if (zerr != -11111) // ignore propr_stop
    {
      snwprintf(msgbuf, MAXELEMS(msgbuf), L"7Zip error %i", zerr);
      //MsgBoxError(msgbuf);
    }
    RedrawGUI=1;
  }
}



static int EF_OnEnter(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  if (mbk->subr)
    DataBrowser_ExecuteSubroutine(mbk->subr,DB_CMD_DESTROY_SUBROUTINE,0);
  mbk->subr=DataBrowser_CreateSubExecute(BookObj_GetBookID(&mbk->book), mbk->fitem);
  DataBrowser_ExecuteSubroutine(mbk->subr,DB_CMD_RUN,0);
  FILEITEM_Destroy(mbk->fitem);
  mbk->fitem=NULL;
  return (1);
}

static int EF_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  WriteLog("EF_OnExit");
  DataBrowser_ExecuteSubroutine(mbk->subr,DB_CMD_DESTROY_SUBROUTINE,0);
  mbk->subr=NULL;
  return (1);  
}

static int EF_OnPreviuos(void *data, BOOK *bk)
{
  WriteLog("EF_OnPreviuos");
  BookObj_ReturnPage(bk,PREVIOUS_EVENT);
  return (1);
}

const PAGE_MSG bk_msglst_execfile[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    EF_OnEnter,
  PAGE_EXIT_EVENT_TAG,     EF_OnExit,
  PREVIOUS_EVENT_TAG,      EF_OnPreviuos,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_execfile = {"MC_ExecuteFile_Page",0,bk_msglst_execfile};

void DoExecFile(const wchar_t *path, const wchar_t *fname)
{
  MyBOOK *mbk=MCBook;
  if (mbk->fitem)
    FILEITEM_Destroy(mbk->fitem);
  mbk->fitem=FILEITEM_Create();
  FILEITEM_SetPathAndContentType(mbk->fitem,path);
  FILEITEM_SetFnameAndContentType(mbk->fitem,fname);
  BookObj_CallPage(&MCBook->book,&bk_execfile);
}

void ExecuteFile(wchar_t *fname, int, int)
{
  wchar_t *name;
  wchar_t *w=wstrrchr(fname, '/');
  if (w)
  {
    name=w+1;
    *w=0;
    DoExecFile(fname,name);
    *w='/';
  }
}

void _Open(int isSysOpen)
{
  if (_CurIndex < 0) return;
  
  FILEINF* file = _CurFile();
  if (file && pathbuf && wstrlen(file->ws_name))
  {
    CurFullPath(file->ws_name);
    if (file->attr & FA_DIRECTORY)
      cd(curtab, pathbuf, _CurDrv);  
    else
    {
      wchar_t* sz = GetFileExt(file->ws_name);
      if (sz)
      {
        if (!wstrcmpni(sz, mcbm_ext, MAX_EXT))
        {
          UseBM(pathbuf);
          return;
        }
        else if (!wstrcmpni(sz, mccs_ext, MAX_EXT))
        {
          MsgBoxYesNo(muitxt(ind_pmt_impcs), CB_CS);
          return;
        }
        else if (!wstrcmpni(sz, mclg_ext, MAX_EXT))
        {
          MsgBoxYesNo(muitxt(ind_pmt_implg), CB_LG);
          return;
        }
      }
      if (!isSysOpen && CONFIG_ZIP_ENABLE && !IsInArchive() && IsItZipFile(pathbuf))
      {
        wstrcpy(arcpathbuf, pathbuf);
        SUBPROC((void*)S_ZipOpen);
        return;
      }
      if (!isSysOpen && CONFIG_7Z_ENABLE && !IsInArchive() && IsIt7ZipFile(pathbuf))
      {
        wstrcpy(arcpathbuf, pathbuf);
        WriteLog("открываем 7z");
        SUBPROC((void*)S_7ZOpen);
        return;
      }
      if (IsInArchive())
      {
        //SUBPROC((void*)S_ZipOpenFile);
        return;
      }
      else
      {
        ExecuteFile(pathbuf, 0, 0);
        WriteLog(pathbuf);
      }
    }
  }
}

void DoOpen()
{
	_Open(0);
}

void DoSysOpen()
{
	_Open(1);
}

u16 ExitTimer;

void DoExitIdYes(u16 , LPARAM );
void ExitFrom7Z();
void CB_Exit(int id)
{
  if (id == IDYES)
  {
    Terminate = progr_stop = 1;
    if (!Busy)
    {
      // Выходим из зипа на текущем табе с восстановлением позиции
      if (IsInArchive()==ZIP_ARCH) ExitFromZip();
      else if (IsInArchive()==_7Z_ARCH) ExitFrom7Z();
      
      // Закрываем остальные зипы при выходе
      for (int i = 0; i < MAX_TABS; i++)
      {
        if (i != curtab)
        {
          if (IsArchiveOpened(i)==ZIP_ARCH) CloseTabZip(i);
          else if (IsArchiveOpened(i)==_7Z_ARCH) CloseTab7Z(i);
        }
      }
        
  
  /*
  // Очищаем темп, если разрешено в конфиге
  if (CONFIG_DELETE_TEMP_FILES_ON_EXIT && tmp_files.count)
  {
    DeleteTempFiles();
    fn_free(&tmp_files);
  }*/
      FreeBook(&MCBook->book);
    }
    else ExitTimer=Timer_Set(3000,DoExitIdYes,0);
  }
}

void DoExitIdYes(u16 id, LPARAM f)
{
  CB_Exit(IDYES);
}

void DoExit()
{
  if (CONFIG_CONFIRM_EXIT)
    MsgBoxYesNo(muitxt(ind_pmt_exit), CB_Exit);
  else
    CB_Exit(IDYES);
}

void DoCopy()
{
  EnumSel(M_MoveCopy, FNT_COPY);
}

void DoMove()
{
  if (IsInArchive()) return; // Пока не обрабатывается
  EnumSel(M_MoveCopy, FNT_MOVE);
}

void DoSwapTab()
{
  curtab++;
  if (curtab >= MAX_TABS) curtab = 0;
  
  // Останавливаем скроллинг при смене таба
  //scfile = NULL;
}


int cordrv(int num, int inc)
{
  do
  {
    num = (MAX_DRV + num + inc) % MAX_DRV;
  }
  while (!Drives[num].enabled);
  return num;
}

void DoPrvDrv()
{
  if (curtab < MAX_TABS && !IsInArchive())
    SetCurTabDrv(cordrv(_CurDrv, -1));
}

void DoNxtDrv()
{
  if (curtab < MAX_TABS && !IsInArchive())
    SetCurTabDrv(cordrv(_CurDrv, +1));
}

void DoUp()
{
  if (_CurIndex > 0 || CONFIG_LOOP_NAVIGATION_ENABLE)
    SetCurTabIndex(--_CurIndex, 1);
}

void DoDwn()
{
  if (CONFIG_LOOP_NAVIGATION_ENABLE || _CurIndex < _CurCount - 1)
    SetCurTabIndex(++_CurIndex, 1);
}

void DoPgUp()
{
	_CurIndex -= LONG_SCRL;
	if (_CurIndex < 0) _CurIndex = 0;		
	SetCurTabIndex(_CurIndex, 0);
}

void DoPgDwn()
{
	_CurIndex += LONG_SCRL;
	_CurBase  = _CurIndex;
	if (_CurIndex >= _CurCount) _CurIndex = _CurCount - 1;
	SetCurTabIndex(_CurIndex, 0);
}

void DoBegin()
{
	SetCurTabIndex(0, 0);
}

void DoRoot()
{
  if (IsInArchive())
    cd(curtab, str_empty, -1);
  else if (curtab < MAX_TABS)
    cd(curtab, Drives[_CurDrv].path, _CurDrv);
}

void DoShowPath()
{
  MsgBoxError(_CurPath);
}

void DoSortN()
{
	_CurTab->sort=(_CurTab->sort & STD_MASK) | ST_NAME;
	DoRefresh();
}

void DoSortE()
{
	_CurTab->sort=(_CurTab->sort & STD_MASK) | ST_EXT;
	DoRefresh();
}

void DoSortS()
{
	_CurTab->sort=(_CurTab->sort & STD_MASK) | ST_SIZE;
	DoRefresh();
}

void DoSortD()
{
	_CurTab->sort=(_CurTab->sort & STD_MASK) | ST_DATE;
	DoRefresh();
}

void DoSetSort()
{
	int srt = (_CurTab->sort & STV_MASK);
	srt=(srt>=ST_LAST)?srt=ST_FIRST:srt+1;
	_CurTab->sort=(_CurTab->sort & STD_MASK) | srt;
	DoRefresh();
}

void DoSortR()
{
	_CurTab->sort=(_CurTab->sort & STV_MASK) | (~_CurTab->sort & STD_MASK);
	DoRefresh();
}

void DoTabCopy()
{
  // Очищаем буфер
  CB_Cancel(IDYES);
  
  // Копируем текущие файлы в буфер
  DoCopy();
  
  // Переходим на другой таб
  DoSwapTab();
  
  // Вставляем файлы на него
  DoPaste();
}

void DoTabMove()
{
  // Очищаем буфер
  CB_Cancel(IDYES);
  
  // Копируем текущие файлы в буфер
  DoMove();
  
  // Переходим на другой таб
  DoSwapTab();
  
  // Вставляем файлы на него
  DoPaste();
}

void ExitFromZip()
{
  if (IsInArchive()==ZIP_ARCH)
  {
    //Сохраняем имя тек. зипа
    wchar_t lpname[MAX_PATH/2];
    wstrncpy(lpname, GetFileName(_CurTab->zipInfo->szZipPath), MAXELEMS(lpname));
    
    CloseTabZip(curtab);
    RefreshTab(curtab);
    
    //Ищем файл из которого вышли
    int ind = GetCurTabFileIndex(lpname);
    SetCurTabIndex(ind, 0);
  }
}


void ExitFrom7Z()
{
  if (IsInArchive()==_7Z_ARCH)
  {
    //Сохраняем имя тек. зипа
    wchar_t lpname[MAX_PATH/2];
    wstrncpy(lpname, GetFileName(_CurTab->zipInfo->szZipPath), MAXELEMS(lpname));
    
    CloseTab7Z(curtab);
    RefreshTab(curtab);
    
    //Ищем файл из которого вышли
    int ind = GetCurTabFileIndex(lpname);
    SetCurTabIndex(ind, 0);
  }
}

int DoBack()
{
  int res = 0;
  if (curtab < MAX_TABS)
  {
    wchar_t lpname[MAX_PATH/2];
    wchar_t* path = IsInArchive() ? _CurTab->zipInfo->szCurDir : _CurPath;
    
    // Читаем родительскуюя папку в pathbuf
    GetFileDir(path, pathbuf, IsInArchive());
    // Проверка на корень
    int goOut;
    if (IsInArchive())
      goOut=*path == '\0';
    else
      goOut=!wstrcmpi(path, Drives[_CurDrv].path);
    
    WriteLog(path);
    if (!goOut)
    {
      	//Сохраняем имя тек. папки
      wstrncpy(lpname, GetFileName(path),MAXELEMS(lpname)-1);
      res = cd(curtab, pathbuf, _CurDrv);
      
      //Ищем папку из которой вышли
      int ind = GetCurTabFileIndex(lpname);
      SetCurTabIndex(ind, 0);
    }
    else if (IsInArchive()==ZIP_ARCH) ExitFromZip();
    else if (IsInArchive()==_7Z_ARCH) ExitFrom7Z();
    else if (CONFIG_BACK_EXIT) DoExit();
  }
  else
  {
    curtab = back_tab < MAX_TABS ? back_tab : 0;
  }
  return res;
}

void DoBackK()
{
  DoBack();
}

int EnumChk(ENUM_SEL_PROC EnumProc, int param)
{
  int cc = 0;
  if (tabs[curtab]->ccFiles)
  {
    FILEINF *file = (FILEINF *)FileListBase[curtab]->next;
    while(file != FileListBase[curtab])
    {
      if (file->attr & FA_CHECK)
      {
        cc++;
        if (EnumProc)
          if (EnumProc(file, param)==0) break;
      }
      file = (FILEINF *)file->next;
    }
  }
  return cc;
}

int EnumSel(ENUM_SEL_PROC EnumProc, int param)
{
  int cc = EnumChk(EnumProc, param);
  if (cc == 0)
  {
    if (EnumProc)
    {
      FILEINF* cfile = _CurFile();
      if (cfile)
      {
        EnumProc(cfile, param);
        cc++;
      }
    }
  }
  return cc;
}


int ChkAll(int chk)
{
  if (tabs[curtab]->ccFiles)
  {
    FILEINF *file = (FILEINF *)FileListBase[curtab]->next;
    while(file!=FileListBase[curtab])
    {
      if (chk>0 || (chk<0 && !(file->attr & FA_CHECK)))
        file->attr = file->attr | FA_CHECK;
      else
        file->attr = file->attr & ~FA_CHECK;
      file = (FILEINF *)file->next;
    }
    return 1;
  }
  return 0;
}


void ChkFile(FILEINF *file)
{
  if (file)
  {
    file->attr = file->attr & FA_CHECK ? file->attr & ~FA_CHECK : file->attr | FA_CHECK;
  }
}

void DoChk()
{
  ChkFile(_CurFile());
  if (CONFIG_CUR_DOWN_ON_CHECK && _CurIndex < _CurCount - 1)
    SetCurTabIndex(++_CurIndex, 1);
}

void DoChkAll()
{
  ChkAll(1);
}

void DoUnCAll()
{
  ChkAll(0);
}

void DoInvChk()
{
  ChkAll(-1);
}

void CB_Del(int id)
{
  if (id==IDYES) SUBPROC((void*)S_Delit);
}

void DoDel()
{
  if (IsInArchive()) return; // Пока не обрабатывается
  
  if (CONFIG_CONFIRM_DELETE)
    MsgBoxYesNo(muitxt(ind_pmt_del), CB_Del);
  else
    CB_Del(1);
}

void DoMenu(void) {MM_Main();}

void DoNewDir()
{
  wchar_t *ws;
  if (IsInArchive()) return; // Пока не обрабатывается
  
  if (*szLastNewDir)
    ws=szLastNewDir;
  else
    ws=(wchar_t *)str_empty;
  TextInput(muitxt(ind_name), 1, ws, _NewDir);
}

void CB_Cancel(int id)
{
  if (id==IDYES) fn_free(&buffer);
}

void DoCancel()
{
  if (buffer.count)
    MsgBoxYesNo(muitxt(ind_pmt_cancel), CB_Cancel);
}

void _Filter(wchar_t *wsname)
{
  wstrncpy(_CurTab->szFilter, wsname, MAX_PATH);
  if (wstrcmp(_CurTab->szFilter, def_filter) == 0)
    _CurTab->szFilter[0] = 0;
  
  DoRefresh();
}

void DoFilter()
{
  const wchar_t *ws;
  if (_CurTab->szFilter[0])
    ws=_CurTab->szFilter;
  else
    ws=def_filter;
  TextInput(muitxt(ind_name), 0, ws, _Filter);
}

void CB_RenEx(int id)
{
  if (id==IDYES || id==-1 || id==-2)
  {
    wchar_t src[MAX_PATH];
    FILEINF *file = _CurFile();
    snwprintf(src, MAXELEMS(src)-1, _ls_ls, _CurPath, file->ws_name);
    if (id==IDYES) w_remove(pathbuf);
    int res=1;
    if (id==-2)
    {
      //char tmpBuf[MAX_PATH];
      //TmpFullPath2(tmpBuf, file->sname);
      //res = fmove(src, tmpBuf, &err) && fmove(tmpBuf, pathbuf, &err);
    }
    else
      res = w_rename(src, pathbuf);
    
    DoRefresh();
    if (!res)
    {
      //Ищем
      int ind = GetCurTabFileIndex(GetFileName(pathbuf));
      SetCurTabIndex(ind, 0);
    }
    else
      MsgBoxError(muitxt(ind_err_rename));
  }
}

void _Rename(wchar_t *wsname)
{
  CurFullPath(wsname);
  if (wstrcmpi(wsname, _CurFile()->ws_name))
  {
    if (fexists(pathbuf))
    {
      if (CONFIG_CONFIRM_REPLACE)
        MsgBoxYesNo(muitxt(ind_pmt_exists), CB_RenEx);
      else
        CB_RenEx(IDYES);
    }
    else
    {
      CB_RenEx(-1);
    }
    
  }
  else
    CB_RenEx(-2);
}

void DoRen()
{
  if (IsInArchive()) return; // Пока не обрабатывается
  
  FILEINF *cfile = _CurFile();
  if (cfile)
    TextInput(muitxt(ind_name), 1, cfile->ws_name, _Rename);
}



int M_Send(FILEINF *file, int param)
{
  if (file && wstrlen(file->ws_name))
  {
    int pname = wstrlen(_CurPath) + 1;
    CurFullPath(file->ws_name);
    fn_add(&buffer, param, file->ftype, pname, pathbuf, IsInArchive() ? _CurTab->zipInfo->szZipPath : NULL);
    return 1;
  }
  return 0;
}

void DestroySendList(LIST *lst)
{
  WriteLog("DestroySendList");
  while(lst->FirstFree)
  {
    void *f=List_RemoveAt(lst,0);
    delete (f);
  }
}

extern "C" void SendObexFile(void *send);

void SendBT_page()
{
  if (IsInArchive()) return; // Пока не обрабатывается
  // Очищаем буфер
  CB_Cancel(IDYES);
  // Копируем текущие файлы в буфер
  EnumSel(M_Send, FNT_SEND);
  if (buffer.count)
  {
    DestroySendList(MCBook->lst_send);
    SEND_OBEX_STRUCT *send=new SEND_OBEX_STRUCT;
    memset(send,0,sizeof(SEND_OBEX_STRUCT));
    FN_ITM* itm = buffer.items;
    while(itm)
    {
      if(!isdir(itm->full))
      {
        wchar_t *fname=wstrrchr(itm->full, L'/');
        if (fname)
        {
          if (buffer.count==1)
          {
            if (is_db2010) {
              GetFileDir(itm->full, send->btf2010.fpath,0);
              wstrcpy(send->btf2010.fname,fname+1);
            }
            else {
              GetFileDir(itm->full, send->btf2020.fpath,0);
              wstrcpy(send->btf2020.fname,fname+1);
            }
          }else{
            if (is_db2010) {
              BT_FILE_2010 *f=new BT_FILE_2010;
              GetFileDir(itm->full, f->fpath,0);
              wstrcpy(f->fname,fname+1);
              List_InsertFirst(MCBook->lst_send, f);
            }
            else {
              BT_FILE_2020 *f=new BT_FILE_2020;
              GetFileDir(itm->full, f->fpath,0);
              wstrcpy(f->fname,fname+1);
              List_InsertFirst(MCBook->lst_send, f);
            }
          }
        }
      }
      itm=(FN_ITM *)itm->next;
    }
    if (MCBook->lst_send->FirstFree)
    {
      send->is_multiple=1;
      send->lst=MCBook->lst_send;
    }else if(buffer.count==1){
      send->is_multiple=0;
    }
    send->Book_ID=BookObj_GetBookID(&MCBook->book);
    send->send=TextID_Create(L"Первая строка",ENC_UCS2,TEXTID_ANY_LEN);
    send->sent=TextID_Create(L"Вторая строка",ENC_UCS2,TEXTID_ANY_LEN);
    send->obex_flag=2;
    ObexSendFile(send);
    delete (send);
    fn_free(&buffer);
  }
}

static int SF_OnEnter(void *, BOOK * bk)
{
  SendBT_page();
  return (1);
}

static int SF_OnExit(void *, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  WriteLog("SF_OnExit");
  DestroySendList(mbk->lst_send);
  return (1);  
}

static int SF_OnAccept(void *data, BOOK *bk)
{
  WriteLog("SF_OnAccept");
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
  return (1);
}

static int SF_OnCancel(void *data, BOOK *bk)
{
  WriteLog("SF_OnCancel");
  BookObj_ReturnPage(bk, PREVIOUS_EVENT);
  return (1);
}

static int SF_OnPreviuos(void *data, BOOK *bk)
{
  WriteLog("SF_OnPreviuos");
  BookObj_ReturnPage(bk,PREVIOUS_EVENT);
  return (1);
}

const PAGE_MSG bk_msglst_sendfile[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    SF_OnEnter,
  PAGE_EXIT_EVENT_TAG,     SF_OnExit,
  ACCEPT_EVENT_TAG,        SF_OnAccept,
  PREVIOUS_EVENT_TAG,      SF_OnPreviuos,
  CANCEL_EVENT_TAG,        SF_OnCancel,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_sendfile = {"MC_SendFile_Page",0,bk_msglst_sendfile};

void DoSendFile()
{
  BookObj_CallPage(&MCBook->book,&bk_sendfile);
}
