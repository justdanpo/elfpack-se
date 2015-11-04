#define __time_t_defined

#include "..\deleaker\mem2.h"
#include "mybook.h"
#include "consts.h"
#include <stddef.h>
#include "conf_loader.h"
#include "config_data.h"
#include <vector>

extern void elf_exit(void);


#include "IUnknown.hpp"
#include "IRegistry.hpp"


void CMyBook::UpdateRingtone()
{
    ISEMCRegistryManager* pISEMCRegistryManager=NULL;

    CoCreateInstance(&CID_CSEMCRegistryManager, &IID_ISEMCRegistryManager, PPINTERFACE(&pISEMCRegistryManager));
    if(pISEMCRegistryManager)
    {
        IRegistryAccess* pIRegistryAccess=NULL;

        if( (S_OK == pISEMCRegistryManager->CreateRegistryAccess(&pIRegistryAccess)) && pIRegistryAccess )
        {

            FILELISTITEM * fli;
            FILELISTITEM* mem = (FILELISTITEM* ) malloc(sizeof(FILELISTITEM));
            DIR_HANDLE * handle = AllocDirHandle( musicpath );
            if(handle)
            {
                vector<vector<wchar_t> > files;

                while(fli=(FILELISTITEM*)GetFname(handle,mem))
                {
                    int len = wstrlen(fli->fname);

                    vector<wchar_t> tmp(len+1);
                    memcpy(&tmp[0], fli->fname, (len+1)*sizeof(wchar_t));

                    files.push_back( tmp );
                }

                DestroyDirHandle(handle);

                if(!files.empty())
                {
                    DATETIME dt;
                    REQUEST_DATEANDTIME_GET( SYNC, &dt );
                    int filenum = datetime2unixtime(&dt) % files.size();

                    vector<wchar_t> newfilename(wstrlen(musicpath)+1+files[filenum].size()+1);
                    wstrcpy(&newfilename[0], musicpath);
                    wstrcat(&newfilename[0], _T("/"));
                    wstrcat(&newfilename[0], &files[filenum][0]);

                    pIRegistryAccess->WriteWString("/apps/call-handling/ui:ringtone-path-line1-user", &newfilename[0]);
                }
            }
            mfree( mem );

            pIRegistryAccess->Release();
        }

        pISEMCRegistryManager->Release();
    }
}




int CMyBook::TerminateElf(CBook** pbookmanbook, CBook* book)
{
    book->Free();
    return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CBook* book)
{
    MessageBox(EMPTY_TEXTID,TextID_Create(WABOUTTEXT,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE, 1 ,5000, *pbookmanbook);
    return 1;
}

int CMyBook::OnCallManagerEvent(char* evtdata, CMyBook* book)
{
    switch(evtdata[book->evtdatastatepos])
    {
    case CALLMANAGER_CALL_ALERT:
        book->UpdateRingtone();
        break;
    }
    return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "HW_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_MSG( ON_CALLMANAGER_EVENT_TAG, CMyBook::OnCallManagerEvent )
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  :CBook(MAINBOOKNAME, &base_page)
{
    InitConfig();

    switch(GetChipID() & CHIPID_MASK)
    {
    case CHIPID_DB2000:
    case CHIPID_DB2010:
    case CHIPID_DB2020:
        evtdatastatepos = offsetof(CALLMANAGER_EVENT_DATA, CallState);
        break;
    case CHIPID_DB3150:
    case CHIPID_DB3200:
    case CHIPID_DB3210:
    case CHIPID_DB3350:
        evtdatastatepos = offsetof(CALLMANAGER_EVENT_DATA_A2, CallState);
        break;
    default:
        evtdatastatepos = 0;
    }
}

//бука убивается
CMyBook::~CMyBook()
{
    elf_exit();
}


int CMyBook::FindBookExCallback(BOOK* book ,int * param)
{
    return 0 == strcmp( book->xbook->name, (char*)param);
}

CMyBook* CMyBook::GetExisting()
{
    return (CMyBook*)FindBookEx(FindBookExCallback, (int*)MAINBOOKNAME);
}


int CMyBook::ReconfigElf( RECONFIG_EVENT_DATA* reconf, CMyBook* book )
{
    if( !wstrcmpi( reconf->path, successed_config_path ) && !wstrcmpi( reconf->name, successed_config_name ) )
    {
        InitConfig();
        return 1;
    }
    return 0;
}
