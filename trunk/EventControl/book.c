/*
*===========================================================================
*                      Модуль книги EventControl'а
*===========================================================================
*/
#define __version__ L"EventControl v2.0\r\n\r\n© UltraShot, Slaventino\n\r\n%s\n\r\nIn a memory of PSIX\n"
#define BUILD "public version"

#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"

extern PROCESS proc_;
extern u16 EventTimer;
extern wchar_t *extFolder;
extern wchar_t *intFolder;

int readConfig(MyBOOK *mbk, wchar_t *fpath, wchar_t *fname);
int InitializeEvents(MyBOOK *myBook);
void DestroyEvents(MyBOOK *myBook);

void CreateReminder(REMINDER_STRUCT * rem_str);
GUI_LIST *create_ed(BOOK *book);
void mbox_Create(BOOK * bk, wchar_t *text, int minutes, bool vibra);

void kill_rems(LIST *lst, MyBOOK *mbk, bool check);
int GetIconID(wchar_t*);
void Reminder_onOK(BOOK *bk, void *);
void OnBackGui(BOOK * bk, void *);

MyBOOK * ECBook=0;

typedef struct
{
  BOOK * book;
}MSG;

/*
 *===========================================================================
 *                         Очистка памяти, занятой эльфом
 *===========================================================================
 */
void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

/*
 *===========================================================================
 *                         Вызываем закрытие нашей книги из букмана
 *===========================================================================
 */
int ECBook_Terminate(void * ,BOOK* book)
{
  FreeBook(book);
  return 1;
};

wchar_t *checkfile(wchar_t *name)
{
  FSTAT fst;
  if (fstat(extFolder,name,&fst)==0)
  {
    return extFolder;
  }
  else if (fstat(intFolder,name,&fst)==0)
  {
    return intFolder;
  }
  return intFolder;
};
/*
 *===========================================================================
 *                         Показ информации об эльфе в окне букмана
 *===========================================================================
 */
int ECBook_ShowAuthorInfo(void *mess ,BOOK* book)
{
  MyBOOK *mbk=ECBook;
  PlayFile(checkfile(L"stop.mp3"),L"stop.mp3");
  if (mbk->remlst)
  {
    if (mbk->remlst->FirstFree>0)
    {
      REMINDER_STRUCT rem_str;
      rem_str.bk=&ECBook->bk;
      rem_str.text=0;
      rem_str.utext=0;
      rem_str.time=0;
      rem_str.vibra=0;
      rem_str.replace=0;
      rem_str.need_to_destroy=false;
      CreateReminder(&rem_str);
      return 1;
    }
  }
  wchar_t text[512];
  snwprintf(text,511, __version__ , BUILD);
  PLMN_LAC_DESC plmn_lac;
  RAT_CI_DESC rat_ci;
  char CSReg;
  if (get_CellData(&plmn_lac,&rat_ci,&CSReg))
  {
    snwprintf(text,511,L"%ls \nCurrent LAC-CID: %04X-%04X", text, plmn_lac.LAC[0]<<8|plmn_lac.LAC[1], rat_ci.CI);
  }
  mbox_Create(book, text, 0, false);
  return 1;
};
void destroy_innative_lng();
/*
 *===========================================================================
 *                         Функция закрытия нашей книги
 *===========================================================================
 * 1) Уничтожаем таймер
 * 2) Уничтожаем список событий
 * 3) Убираем эльфа из памяти
 */
void ECBook_OnClose(BOOK * book)
{
  if (book)
  {
    MyBOOK *mbk=(MyBOOK*)book;
    Timer_Kill(&EventTimer);
    kill_proc(proc_);
    if (extFolder)delete(extFolder);
    if (intFolder)delete(intFolder);
    wchar_t icon=GetIconID(L"CALE_RN_TASK_STATUS_ICN");
    SetTrayIcon(icon, 0);
    if (mbk->indpath)
      delete(mbk->indpath);
    if (mbk->indname)
      delete(mbk->indname);
    if (mbk->g_mbox)
      GUIObject_Destroy(mbk->g_mbox);
    if (mbk->name)
      delete(mbk->name);
    if (mbk->path)
      delete(mbk->path);
    if (mbk->g_test)
      GUIObject_Destroy(mbk->g_test);
    if (mbk->remind)
      GUIObject_Destroy(mbk->remind);
    kill_rems(mbk->remlst, mbk, 0);
    DestroyEvents((MyBOOK*)book);
    destroy_innative_lng();
    
    SUBPROC(elf_exit);
    
  }
};

/*
 *===========================================================================
 *                         Определение нашей книги для FindBook
 *===========================================================================
 */
int isEvtBook(BOOK *bk)
{
  if (bk->onClose == ECBook_OnClose)return 1;
  return 0;
};

int onUserInactivity(void * r0, BOOK * bk)
{
  MyBOOK *mbk=(MyBOOK*)bk;
  if (mbk->remind)
  {
    Reminder_onOK(bk, 0);
  }
  if (mbk->g_test)
  {
    OnBackGui(bk, 0);
  }
  return(0);
};
/*
*===========================================================================
*                         Список ивентов, обрабатываемых эльфом
*===========================================================================
*/
const PAGE_MSG EC_BasePageEvents[]@ "DYN_PAGE" ={
  ELF_TERMINATE_EVENT , ECBook_Terminate,
  ELF_SHOW_INFO_EVENT  , ECBook_ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};
PAGE_DESC base_page ={"EventControl_BasePage",0,EC_BasePageEvents};

/*
 *===========================================================================
 *                         Обновление расписания
 *===========================================================================
 * 1) Если путь/имя!=0, то читаем новый конфиг
 * 2) Если путь/имя==0, то читаем дефолтный
 */

void Update(wchar_t *fpath, wchar_t *fname)
{
  DestroyEvents(ECBook);
  if (fpath && fname)
  {
    if (fpath[0]!=0 && fname[0]!=0)
    {
      readConfig(ECBook,fpath,fname);
      return;
    }
  }
  InitializeEvents(ECBook);
};

/*
 *===========================================================================
 *                         Создание нашей книги
 *===========================================================================
 * 1) Создаём нашу книгу с сопутствующим "хранилищем" переменных...
 * 2) Устанавливаем update-функцию для обновления из редактора
 */
MyBOOK * EC_Create()
{
  ECBook = new MyBOOK;
  memset(ECBook,0,sizeof(MyBOOK));
  CreateBook(&ECBook->bk,ECBook_OnClose,&base_page,"EventControl",-1,0);
  ECBook->update=Update;
  return(ECBook);
};
