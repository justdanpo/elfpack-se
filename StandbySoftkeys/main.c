#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "inc\conf_loader.h"
#include "inc\config_data.h"

// Standby Softkeys
// (c) DuXeN0N
// v.1.2

BOOK *MyBook;

#define ELF_BCFG_CONFIG_EVENT 994   // Для букманмода

#define DELETE(a) if (a) { delete(a); a = 0; }

#define BOOK_NAME "StandbySoftkeys"
#define BASEPAGE_NAME "StandbySoftkeys_BasePage"

#define ELF_FOLDER_NAME L"/StandbySoftkeys"
wchar_t *ElfFolder;

// Softkeys 
DISP_DESC *SoftKeys_DispDesc;
DISP_OBJ *SoftKeys_DispObj;
DISP_OBJ_ONREDRAW_METHOD SoftKeys_DefaultRedraw;

#define SOFTKEYS_COUNT 3

typedef struct
{
    IMAGEID ImageID;
    IMAGEID ImageID_Pressed;
    int ImageWidth;
    int ImageWidth_Pressed;
    BOOL Pressed;
}SOFTKEY_DATA;
    
LIST *DataList;

const wchar_t *SoftKeys_ImgNames[SOFTKEYS_COUNT] =
{
    LEFT_SOFT,
    CENTER,
    RIGHT_SOFT
};

const wchar_t *SoftKeys_Pressed_ImgNames[SOFTKEYS_COUNT] =
{
    LEFT_SOFT_PRESSED,
    CENTER_PRESSED,
    RIGHT_SOFT_PRESSED
};

enum SOFTKEY_INDEX
{
    SOFTKEY_LEFT = 0,
    SOFTKEY_CENTER = 1,
    SOFTKEY_RIGHT = 2
};

typedef struct
{
    BOOK *Book;
}MSG;

typedef struct
{
    BOOK *Book;
    wchar_t *BCFG_Path;
    wchar_t *BCFG_Name;
}MSG_BCFG;

int TerminateElf(void *Data, BOOK *Book)
{
    FreeBook(Book);
    return(1);
};

int ShowAuthorInfo(void *Message, BOOK *Book)
{
    MSG *Msg = (MSG*)Message;
    MessageBox(EMPTY_TEXTID, STR("StandbySoftkeys, v.1.2\n\n(c) DuXeN0N"), NOIMAGE, 1, 0, Msg->Book);
    return(1);
};

void Data_Free(void *DataItem)
{
    SOFTKEY_DATA *Data = (SOFTKEY_DATA*)DataItem;
    ImageID_Free(Data->ImageID);
    ImageID_Free(Data->ImageID_Pressed);
    delete(Data);
};

int Data_Filter(void *DataItem)
{
    if (DataItem) return(1);
    return(0);
};

void FreeDataList()
{
    if (DataList)
    {
        List_DestroyElements(DataList, Data_Filter, Data_Free);
        List_Destroy(DataList);
    }
};

SOFTKEY_DATA *FillSoftKeyData(int Index)
{
    SOFTKEY_DATA *Data = new SOFTKEY_DATA;
    Data->Pressed = false;
    ImageID_Get(ElfFolder, SoftKeys_Pressed_ImgNames[Index], &Data->ImageID_Pressed);
    ImageID_Get(ElfFolder, SoftKeys_ImgNames[Index], &Data->ImageID);
    Data->ImageWidth = GetImageWidth(Data->ImageID); // Сразу получаем размеры этих картинок. Чтобы при перерисовке постоянно не получать. 
    Data->ImageWidth_Pressed = GetImageWidth(Data->ImageID_Pressed);
    return(Data);
};

// Создаем лист и добавляем в него информацию о каждом из 3 софткеев
void FillSoftKeysData()
{
    DataList = List_Create();
    for (int i = 0; i < SOFTKEYS_COUNT; i++)
    {
        List_InsertLast(DataList, FillSoftKeyData(i));
    }
};

static int ReconfigElf(void *Message, BOOK *Book)
{
    RECONFIG_EVENT_DATA *EventData = (RECONFIG_EVENT_DATA*)Message;
    if (wstrcmpi(EventData->path, successed_config_path) == 0 && wstrcmpi(EventData->name, successed_config_name) == 0)
    {
        InitConfig();
        FreeDataList();       // Пересоздаем лист с SOFTKEY_DATA. Вдруг картинки изменились?
        FillSoftKeysData();
        DispObject_InvalidateRect(SoftKeys_DispObj, 0);
        return(1);
    }
    return(0);
};

int IsStandbySoftkeys(BOOK *Book)
{
    if (strcmp(Book->xbook->name, BOOK_NAME) == 0) return(1);
    return(0); 
};

int IsInStandby()
{
    if (Display_GetTopBook(0) == Find_StandbyBook())
    {
        if (MainInput_getVisible(SBY_GetMainInput(Find_StandbyBook())) == 0)
        {
            return(1);
        }
    }
    return(0);
};

int IsShowWhenMusic()
{
    if (HIDE_WHEN_MUSIC_PLAYING)
    {
        if (FindBook(get_IsAudioPlayerBook())) return(0);
    }
    return(1);
};

int GetXPos(int SoftIndex, int ImageSize)
{
    int DispWidth = Display_GetWidth(0);
    int CenterPos = DispWidth / 2 - ImageSize / 2;
    if (SoftIndex == SOFTKEY_LEFT) return(LEFT_INDENT);
    if (SoftIndex == SOFTKEY_CENTER) 
    {  
        if (CENTER_INDENT == 0) return(CenterPos);
        if (CENTER_INDENT == 1) return(CenterPos + CENTER_INDENT_VALUE);
        if (CENTER_INDENT == 2) return(CenterPos - CENTER_INDENT_VALUE);
    }
    if (SoftIndex == SOFTKEY_RIGHT) return(DispWidth - RIGHT_INDENT - ImageSize);
    return(0);
};

void SoftKeys_Redraw(DISP_OBJ *DispObj, int r1, int r2, int r3)
{
    if (IsInStandby() && IsShowWhenMusic()) 
    {
        GC *DispGC = get_DisplayGC();
        int i;
        for (i = 0; i < SOFTKEYS_COUNT; i++)
        {
            SOFTKEY_DATA *Data = (SOFTKEY_DATA*)List_Get(DataList, i);
            if (Data->Pressed)
            {
                GC_PutChar(DispGC, GetXPos(i, Data->ImageWidth_Pressed), Y_INDENT, 0, 0, Data->ImageID_Pressed);
            }
            else GC_PutChar(DispGC, GetXPos(i, Data->ImageWidth), Y_INDENT, 0, 0, Data->ImageID);
        }
    }
    else SoftKeys_DefaultRedraw(DispObj, r1, r2, r3);   
};

void Release_SoftKeys()
{
    REDRAW_RELEASE_MESSAGE *Message = new REDRAW_RELEASE_MESSAGE;
    memset(Message, 0, sizeof(REDRAW_RELEASE_MESSAGE));
    Message->SK_OldOnRedraw = SoftKeys_DefaultRedraw;
    Message->SK_NewOnRedraw = SoftKeys_Redraw;
    UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, Message, (void (*)(void*))mfree_adr());
};

int RedrawRelease(void *Data, BOOK *Book)
{
    REDRAW_RELEASE_MESSAGE *Message = (REDRAW_RELEASE_MESSAGE*)Data;
    {
        if (Message->SK_NewOnRedraw == SoftKeys_DefaultRedraw)
        {
            REDRAW_RELEASE_MESSAGE *NewMessage = new REDRAW_RELEASE_MESSAGE;
            memcpy(NewMessage, Message, sizeof(REDRAW_RELEASE_MESSAGE));
            if (Message->SK_OldOnRedraw) SoftKeys_DefaultRedraw = Message->SK_OldOnRedraw;
            DISP_DESC_SetOnRedraw(SoftKeys_DispDesc, SoftKeys_Redraw);
            NewMessage->SK_OldOnRedraw = 0;
            NewMessage->SK_NewOnRedraw = SoftKeys_Redraw;
            UI_Event_wData(SBY_REDRAW_RELEASE_EVENT, NewMessage, (void (*)(void*))mfree_adr());
            return(1);
        }
        return(0);
    }
};

void SetPressed(int SoftIndex, BOOL Pressed)
{
    SOFTKEY_DATA *Data = (SOFTKEY_DATA*)List_Get(DataList, SoftIndex);
    Data->Pressed = Pressed;
};

// Обновляем при возвращение в стендбай, чтобы не было "зависания" софтов
int RefreshSoftKeys(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event)
{
    SetPressed(SOFTKEY_LEFT, false); // Убираем Pressed у софтов
    SetPressed(SOFTKEY_CENTER, false);
    SetPressed(SOFTKEY_RIGHT, false);
   
    DispObject_InvalidateRect(SoftKeys_DispObj, 0);
    return(1);
};

int LoadBcfgConfig(void *Data, BOOK *Book)
{
    MSG_BCFG *Message = (MSG_BCFG*)Data;
    wchar_t *Temp = new wchar_t[256];
    wstrcpy(Temp, Message->BCFG_Path);
    wstrcat(Temp, L"/");
    wstrcat(Temp, Message->BCFG_Name);
    elfload(Temp, (void*)successed_config_path,(void*)successed_config_name, 0);
    DELETE(Temp);
    return(1);
};

const PAGE_MSG StandbySoftkeys_PageEvents[]@ "DYN_PAGE" =
{
    ELF_TERMINATE_EVENT, TerminateElf,
    ELF_SHOW_INFO_EVENT, ShowAuthorInfo, 
    ELF_RECONFIG_EVENT, ReconfigElf,
    ELF_BCFG_CONFIG_EVENT, LoadBcfgConfig,
    SBY_REDRAW_RELEASE_EVENT, RedrawRelease,
    NIL_EVENT_TAG, NULL
};

PAGE_DESC StandbySoftkeys_Base_Page = {BASEPAGE_NAME, 0, StandbySoftkeys_PageEvents};

void elf_exit(void)
{
    kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
};

int SoftKeys_OnKey(int Key, int r1, int Mode, LPARAM, DISP_OBJ*)
{
    if (IsInStandby() && IsShowWhenMusic())
    {
        if (Mode == KBD_SHORT_PRESS)
        {
            if (Key == KEY_LEFT_SOFT)
            {
                SetPressed(SOFTKEY_LEFT, true);           // Перерисовка происходит только при нажатии на софт.                                      
                DispObject_InvalidateRect(SoftKeys_DispObj, 0);      // Чтобы не дергать перерисовку каждый раз при нажатии любой клавиши
            }
            if (Key == KEY_ENTER)
            {
                SetPressed(SOFTKEY_CENTER, true);
                DispObject_InvalidateRect(SoftKeys_DispObj, 0);
            } 
            if (Key == KEY_RIGHT_SOFT)
            {
                SetPressed(SOFTKEY_RIGHT, true);
                DispObject_InvalidateRect(SoftKeys_DispObj, 0);
            } 
        }
        if (Mode == KBD_SHORT_RELEASE || Mode == KBD_LONG_PRESS || Mode == KBD_LONG_RELEASE)
        {
            SetPressed(SOFTKEY_LEFT, false);
            SetPressed(SOFTKEY_CENTER, false);
            SetPressed(SOFTKEY_RIGHT, false);
            DispObject_InvalidateRect(SoftKeys_DispObj, 0);
        }
        return(1);
    }
    return(0);
};

int ModifyUIHookEx(int Event1, int Event2, int (*PROC)(void *msg, BOOK * book, PAGE_DESC * page_desc, LPARAM ClientData, u16 event), int Mode)       // (r) ploik
{
    if ((Event1) && (Event1 != 0xFFFFFFFF))
      return ModifyUIPageHook(Event1, PROC, 0, Mode);
    return ModifyUIPageHook(Event2, PROC, 0, Mode);
};

void OnCloseMyBook(BOOK *Book)
{
    if (Book)
    {
        Release_SoftKeys();
        ModifyKeyHook(SoftKeys_OnKey, KEY_HOOK_REMOVE, NULL);
        DISP_DESC_SetOnRedraw(SoftKeys_DispDesc, SoftKeys_DefaultRedraw);
        FreeDataList();
        ModifyUIHookEx(STANDBY_IDLE_EVENT, PHONE_IN_STBY_EVENT, RefreshSoftKeys, PAGE_HOOK_REMOVE);
        DELETE(ElfFolder);
        SUBPROC(elf_exit);
    }
};
   
void SoftKeys_SetOnRedraw()
{
    SoftKeys_DispObj = DispObject_SoftKeys_Get();
    SoftKeys_DefaultRedraw = DispObject_GetOnRedraw(SoftKeys_DispObj);
    SoftKeys_DispDesc = DispObject_GetDESC(SoftKeys_DispObj);
    DISP_DESC_SetOnRedraw(SoftKeys_DispDesc, SoftKeys_Redraw);
};

// Получаем путь к папке с картинками
wchar_t *GetElfFolder()
{
    wchar_t *CfgDir = GetDir(DIR_ELFS_CONFIG | MEM_INTERNAL);
    wchar_t *Path = new wchar_t[wstrlen(CfgDir) + wstrlen(ELF_FOLDER_NAME) + 1]; 
    wstrcpy(Path, CfgDir);
    wstrcat(Path, ELF_FOLDER_NAME);
    return(Path);
};

int main(void)
{
    if (FindBook(IsStandbySoftkeys)) // Если уже запущен
    {
        MessageBox(EMPTY_TEXTID, STR("StandbySoftkeys is running"), NOIMAGE, 1, 0, 0);
        SUBPROC(elf_exit);
        return(0);
    }
    MyBook = new BOOK;
    if (!CreateBook(MyBook, OnCloseMyBook, &StandbySoftkeys_Base_Page, BOOK_NAME, -1, 0))
    {
        delete(MyBook);
        SUBPROC(elf_exit);
        return(0);
    }
    ModifyUIHookEx(STANDBY_IDLE_EVENT, PHONE_IN_STBY_EVENT, RefreshSoftKeys, PAGE_HOOK_ADD_BEFORE);
    InitConfig();
    ElfFolder = GetElfFolder(); 
    FillSoftKeysData(); 
    SoftKeys_SetOnRedraw();
    ModifyKeyHook(SoftKeys_OnKey, KEY_HOOK_ADD, NULL); 
    return(0);
};
