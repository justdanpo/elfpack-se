#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"

#include "inc\log.h"
#include "inc\mc.h"
#include "inc\fslib.h"
#include "inc\main.h"
#include "inc\gui.h"
#include "inc\mui.h"
#include "inc\ColorMap.h"
#include "inc\zslib.h"
#include "inc\config.h"
#include "inc\conf_loader.h"
#include "inc\zslib.h"
#include "inc\7zslib.h"
#include "inc\keys.h"
#include "inc\file_op.h"

extern void CloseAllFiles(MyBOOK *mbk);
MyBOOK * MCBook;
DISP_OBJ *main_obj=NULL;
volatile int Busy = 0;
volatile int Terminate = 0; // флаг необходимости завершени€ работы
volatile int RedrawGUI = 0; // флаг необходимости перерисовки гу€
int is_db2010=0;

wchar_t STD_ICONS[TOTAL_ICONS];

void FreeData();
void FreeDrvInfo();

#pragma segment="ELFBEGIN"
void elf_exit(void){
  trace_done();
  WriteLog("elf_exit");
  kill_data(__segment_begin("ELFBEGIN"), (void(*)(void*))mfree_adr());
}

#define MESSAGE(__STR__) MessageBox(EMPTY_TEXTID,__STR__,NOIMAGE, 1 ,11000,NULL);

void MsgBoxError(int lgind, char* str)
{
  snwprintf(msgbuf,MAXELEMS(msgbuf), muitxt(lgind), str);
  TEXTID q=TextID_Create(msgbuf,ENC_UCS2,TEXTID_ANY_LEN);
  MessageBox(LGP_NULL,q, NOIMAGE, 1 ,11000,(BOOK*)MCBook);
}

void MsgBoxError(int lgind, wchar_t* str)
{
  snwprintf(msgbuf,MAXELEMS(msgbuf), muitxt(lgind), str);
  TEXTID q=TextID_Create(msgbuf,ENC_UCS2,TEXTID_ANY_LEN);
  MessageBox(LGP_NULL,q, NOIMAGE, 1 ,11000,(BOOK*)MCBook);
}

void MsgBoxError(char *err)
{
  win12512unicode(msgbuf,err,MAXELEMS(msgbuf)-1);
  TEXTID q=TextID_Create(msgbuf,ENC_UCS2,TEXTID_ANY_LEN);
  MessageBox(LGP_NULL,q, NOIMAGE, 1 ,11000,(BOOK*)MCBook);
}

void MsgBoxError(wchar_t *err, int a)
{
  snwprintf(msgbuf,MAXELEMS(msgbuf), err, a);
  TEXTID q=TextID_Create(msgbuf,ENC_UCS2,TEXTID_ANY_LEN);
  MessageBox(LGP_NULL,q, NOIMAGE, 1 ,11000,(BOOK*)MCBook);
}

void MsgBoxError(wchar_t *err)
{
  TEXTID q=TextID_Create(err,ENC_UCS2,TEXTID_ANY_LEN);
  MessageBox(LGP_NULL,q, NOIMAGE, 1 ,11000,(BOOK*)MCBook);
}

static void YSYes(BOOK * bk, void *)
{
  MyBOOK *mcb=(MyBOOK *)bk;
  mcb->YesNoFunc(1);
  FREE_GUI(mcb->yes_no);
}

static void YSNo (BOOK * bk, void *)
{
  MyBOOK *mcb=(MyBOOK *)bk;
  mcb->YesNoFunc(0);
  FREE_GUI(mcb->yes_no);
}

void MsgBoxYesNo(wchar_t *qv, void(*f)(int))
{
  TEXTID q=TextID_Create(qv,ENC_UCS2,TEXTID_ANY_LEN);
  MCBook->YesNoFunc=f;
  MCBook->yes_no=CreateYesNoQuestionVA(0,
                                       VAR_BOOK(MCBook),
                                       VAR_YESNO_QUESTION(q),
                                       VAR_YESNO_YES_ACTION(YSYes),
                                       VAR_YESNO_NO_ACTION(YSNo),
                                       VAR_PREV_ACTION_PROC(YSNo),
                                       0);
}

void FreeData()
{
  WriteLog("FreeData");
  
  if (CONFIG_LOAD_CS) SaveCS(NULL);
  if (CONFIG_LOAD_MUI) SaveMUI(NULL);
  SaveCfg();
  fn_free(&buffer);
  FreeMUI();
  //FreeExt();
  for(int ii=0; ii<MAX_TABS+1; ii++)
  {
    DelFiles(ii);
    FreeTab(ii);
  }
  FreeDrvInfo();
  DestroyPathBuf();
  DestroySendList(MCBook->lst_send);
  List_Destroy(MCBook->lst_send);
}

wchar_t *SA_ICONS[]=
{
  L_ICN_FOLDER,
  L_ICN_CHMARK,
  L_ICN_CH_RB,
  L_ICN_RB,
  L_ICN_CH_CB,
  L_ICN_CB  
};

void InitDrvInfo()
{
  int drv_num=wstrcmpi(GetDir(DIR_OTHER|MEM_INTERNAL),GetDir(DIR_OTHER|MEM_EXTERNAL))?3:2;
  FreeDrvInfo();
  if (SHOW_ROOT_FOLDER)
  {
    Drives[drv_num-1].path=new wchar_t[2];
    wstrcpy(Drives[drv_num-1].path, L"/");
    wstrcpy(Drives[drv_num-1].name,L"root");
    Drives[drv_num-1].enabled=1;
  }
  else
  {
    Drives[drv_num-1].enabled=0;    
  }
  wchar_t *ws=new wchar_t[wstrlen(GetDir(DIR_OTHER|MEM_INTERNAL))+1];
  wstrcpy(ws, GetDir(DIR_OTHER|MEM_INTERNAL));
  wchar_t *last=wstrrchr(ws,L'/');
  if (last) *last=0;
  Drives[drv_num-2].path=ws;
  wstrcpy(Drives[drv_num-2].name,L"phone");
  Drives[drv_num-2].enabled=1;
  
  if (drv_num==3)
  {
    wchar_t *ws=new wchar_t[wstrlen(GetDir(DIR_OTHER|MEM_EXTERNAL))+1];
    wstrcpy(ws, GetDir(DIR_OTHER|MEM_EXTERNAL));
    wchar_t *last=wstrrchr(ws,L'/');
    if (last) *last=0;
    Drives[drv_num-3].path=ws;
    wstrcpy(Drives[drv_num-3].name,L"card");
    Drives[drv_num-3].enabled=1;
  }
  else
    Drives[drv_num-3].enabled=0;
}


void FreeDrvInfo()
{
  for (int i=0; i<MAX_DRV; i++)
  {
    delete (Drives[i].path);
    Drives[i].path=NULL;
    Drives[i].enabled=0;    
  }
}


int MainGuiOnCreate(DISP_OBJ_MAIN *db)
{
  int tmp;
  WriteLog("MainGuiOnCreate");
  main_obj=&db->dsp_obj;
  for (int i=0; i<TOTAL_ICONS; i++)
  {
    if (iconidname2id(SA_ICONS[i],-1,&tmp))
      STD_ICONS[i]=tmp;
    else
      STD_ICONS[i]=NOIMAGE;
  }
  is_db2010=(GetChipID()&CHIPID_MASK)==CHIPID_DB2010;
  InitDrvInfo();
  WriteLog("LoadKeys");
  LoadKeys();
  WriteLog("LoadCfg");
  LoadCfg();
  ZeroArchiveBuffer();
  fn_zero(&buffer);
  for(int ii=0; ii < MAX_TABS+1; ii++)
    InitTab(ii);
  if (CONFIG_SAVE_PATH)
  {
    WriteLog("Init Last Dirs");
    for(int ii=0;ii<MAX_TABS;ii++)
    {
      int srt=MCConfig.tabs[ii].sort;
      int srtH=(srt & STD_MASK);
      int srtL=(srt & STV_MASK);
      srt=(srtL<=ST_LAST?srtL:0) | srtH;
      tabs[ii]->sort=srt;
      if (MCConfig.tabs[ii].LastPath[0] /*&& isdir(MCConfig.tabs[ii].LastPath, &err)*/)
      {
        cd(ii, MCConfig.tabs[ii].LastPath, MCConfig.tabs[ii].drv);
        SetTabIndex(ii, MCConfig.tabs[ii].LastInd, 0);
      }
    }
    curtab = MCConfig.curtab;
  }
  WriteLog("InitScr");
  InitScr();
  WriteLog("InitCS");
  InitCS();
  WriteLog("LoadMUI");
  if (CONFIG_LOAD_MUI)
    LoadMUI(NULL);
  WriteLog("Create lst_send");
  MCBook->lst_send=List_Create();
  DispObject_SetRefreshTimer(&db->dsp_obj,100);
  if (in_open_path[0]=='/')
  {
    if (isdir(in_open_path))
    {
      cd(curtab = 0, in_open_path, -1);
    }
    else if (CONFIG_ZIP_ENABLE && IsItZipFile(in_open_path))
    {
      // переходим в папку с зипом
      cd(curtab = 0, GetFileDir(in_open_path, pathbuf,0), -1);
      // ищем наш зип файл
      int idx = GetCurTabFileIndex(GetFileName(in_open_path));
      SetCurTabIndex(idx, 0);
      // открываем
      wstrcpy(arcpathbuf, in_open_path);
      SUBPROC((void*)S_ZipOpen);
    }
    else if (CONFIG_7Z_ENABLE && IsIt7ZipFile(in_open_path))
    {
      // переходим в папку с зипом
      cd(curtab = 0, GetFileDir(in_open_path, pathbuf,0), -1);
      // ищем наш зип файл
      int idx = GetCurTabFileIndex(GetFileName(in_open_path));
      SetCurTabIndex(idx, 0);
      // открываем
      wstrcpy(arcpathbuf, in_open_path);
      SUBPROC((void*)S_7ZOpen);
    }
  }
  return (1);
}

void MainGuiOnClose(DISP_OBJ_MAIN *db)
{
  WriteLog("MainGuiOnClose");
  DispObject_KillRefreshTimer(&db->dsp_obj);
  DisableScroll();
  FreeData();
}

void MainGuiOnRedraw(DISP_OBJ_MAIN *db,int ,RECT *cur_rc,int)
{
  int font_old, gc_xx;
  int font=CONFIG_FONT_ATTR;
  GC *gc=get_DisplayGC();
  gc_xx=GC_GetXX(gc);
  GC_SetXX(gc,1);
  font_old=SetFont(font);
  ShowFiles(gc, cur_rc);
  SetFont(font_old);
  GC_SetXX(gc,gc_xx);
}

static void CBStop(BOOK * bk, void *)
{
  progr_stop = 1;
  FREE_GUI(MCBook->stop_progr);
}

static void CBBack(BOOK * bk, void *)
{
  FREE_GUI(MCBook->stop_progr);
}

int lastKey = -1;
int lastIsLongPress = 0;
void MainGuiOnKey(DISP_OBJ_MAIN *db,int key,int,int repeat,int type)
{
  int isLongPress = 0;
  int ignore = 1;
  if (progr_start)
  {
    //2do: переделать на нормальные софты
    if (key==KEY_LEFT_SOFT || key==KEY_RIGHT_SOFT)
    {
      TEXTID q=TextID_Create(muitxt(ind_pmt_stop),ENC_UCS2,TEXTID_ANY_LEN);
      MCBook->stop_progr=CreateYesNoQuestionVA(0,
                                               VAR_BOOK(MCBook),
                                               VAR_YESNO_QUESTION(q),
                                               VAR_YESNO_YES_ACTION(CBStop),
                                               VAR_YESNO_NO_ACTION(CBBack),
                                               VAR_PREV_ACTION_PROC(CBBack),
                                               0);
    }
  }
  else if (!Busy)
  {
    if (GetKeyprocLongByKey(key) == NULL)
    {
      ignore = (type != KBD_SHORT_PRESS && type != KBD_REPEAT);
    }
    else
    {
      isLongPress = (type == KBD_REPEAT);
      ignore = (lastKey == key && lastIsLongPress && type == KBD_SHORT_RELEASE)
        	|| (type != KBD_SHORT_RELEASE && type != KBD_REPEAT);
    }
    
    
  }
  if (!ignore)
  {
    DoKey(isLongPress, key);
    DispObject_InvalidateRect(&db->dsp_obj,0);
  }
  lastKey = key;
  lastIsLongPress = isLongPress;
}

void MainGuiOnRefresh(DISP_OBJ_MAIN *db)
{
  if (RedrawGUI){
    DispObject_InvalidateRect(&db->dsp_obj,0);
    RedrawGUI=0;
  }
  DispObject_SetRefreshTimer(&db->dsp_obj,150);
}


static const char MainGuiName[]="Gui_MainMC";
void MainGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,MainGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_MAIN));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)MainGuiOnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)MainGuiOnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)MainGuiOnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)MainGuiOnKey);
  DISP_DESC_SetOnRefresh(desc,(DISP_OBJ_METHOD)MainGuiOnRefresh);
}

void MainGui_destr( GUI* ){}

void SK_DoMenu( BOOK*, GUI* gui)
{
  DoMenu();
}

void SK_DoOpen( BOOK*, GUI* gui)
{
  DoOpen();
  DispObject_InvalidateRect( GUIObject_GetDispObject( gui ),0);
}

void SK_DoBackK( BOOK*, GUI* gui)
{
  DoBackK();
  DispObject_InvalidateRect( GUIObject_GetDispObject( gui ),0);
}

static GUI *CreateMainGui(MyBOOK *mbk)
{
  MAIN_GUI *main_gui=new MAIN_GUI;
  if (!GUIObject_Create( main_gui,MainGui_destr,MainGui_constr, &mbk->book,0,0,0))
  {
    delete main_gui;
    return 0;    
  }
  GUI *gui = main_gui;
  if (mbk) BookObj_AddGUIObject(&mbk->book,gui);
  GUIObject_SetStyle(gui,4);
  GUIObject_SetTitleType(gui, 1);
  GUIObject_SoftKeys_Hide(gui);
  //GUIObject_SoftKeys_SetAction(myBook->coord,ACTION_BACK, OnBackCoordinatesEdit);
  GUIObject_SoftKeys_SetAction(gui, ACTION_OK, SK_DoOpen);
  GUIObject_SoftKeys_SetAction(gui, ACTION_MORE, SK_DoMenu);
  GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, SK_DoBackK);
  GUIObject_Show(gui);
  return gui;
}

static int MainPageOnEnter(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  WriteLog("MainPageOnEnter");
  mbk->main_gui=CreateMainGui(mbk);
  return (1);
}

static int MainPageOnExit(void *, BOOK *bk)
{
  MyBOOK *mbk=(MyBOOK *)bk;
  WriteLog("MainPageOnExit");
  FREE_GUI(mbk->main_gui);
  return (1);
}


int TerminateElf(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
}

static int ReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    InitDrvInfo();
    InitScr();
    UpdateAll();
    result=1;
  }
  return(result);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,     TerminateElf,
  ELF_RECONFIG_EVENT,      ReconfigElf,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"MC_Base_Page",0,bk_msglst_base};


const PAGE_MSG bk_msglst_main[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,    MainPageOnEnter,
  PAGE_EXIT_EVENT_TAG,     MainPageOnExit,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_main = {"MC_Main_Page",0,bk_msglst_main};

// при закрытии книги
static void onMyBookClose(BOOK * book)
{
  WriteLog("onMyBookClose");
  //SUBPROC(FreeData);
  SUBPROC(elf_exit);
}

int isMcBook(BOOK * struc)
{
  return(struc->onClose==onMyBookClose);
}

#pragma segment="DYN_CONST"
#pragma segment="DYN_CONST_DESC"
int CheckConst(void)
{
  int result=1;
  int const_count=*(int *)__segment_begin("DYN_CONST_DESC");
  int *constt=(int *)__segment_begin("DYN_CONST");
  for (int i=0; i<const_count; i++)
  {
    if (constt[i]==0xFFFFFFFF) {result=0; break;}
  }
  return (result);
}

int main(wchar_t *elfname, wchar_t *path, wchar_t *fname)
{
  trace_init();
  if (!CheckConst())
  {
    MESSAGE(TextID_Create(L"Go smoke bamboo or update dynconst",ENC_UCS2,TEXTID_ANY_LEN));
    SUBPROC(elf_exit);
    return 0;
  }
  MCBook=new MyBOOK;
  memset(MCBook,0,sizeof(MyBOOK));
  GetFileDir(elfname, mcpath,0);
  StartLog();
  WriteLog("Start");
  WriteLog("InitConfig");
  InitConfig();
  if (!CreateBook((BOOK*)MCBook,onMyBookClose,&bk_base,"mc",-1,0))
  {
    delete MCBook;
    SUBPROC(elf_exit);
    return (0);    
  }
  if (path && fname) 
  {
    wstrcpy(in_open_path, path);
    int l=wstrlen(path);
    in_open_path[l++]=L'/';
    wstrcpy(in_open_path+l, fname);
  }
  else *in_open_path=0;
  BookObj_GotoPage((BOOK *)MCBook,&bk_main);
  return 0;
}


