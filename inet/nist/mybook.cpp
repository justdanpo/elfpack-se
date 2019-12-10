#include "..\..\deleaker\mem2.h"
#include "mybook.h"
#include "consts.h"
#include "elfcount.h"
#include "conf_loader.h"

#include "updatetime.h"


int CMyBook::TerminateElf(CBook** pbookmanbook, CBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CBook* book)
{
	MessageBox(EMPTY_TEXTID, TextID_Create(WABOUTTEXT, ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 1 , 3000, *pbookmanbook);
	return 1;
}

int CMyBook::_OnInternetConnected(void*, CBook* book)
{
	updatetime();
	return 1;
}

int CMyBook::ReconfigElf(RECONFIG_EVENT_DATA* reconf, CMyBook* book)
{
	if (!wstrcmpi(reconf->path, successed_config_path) && !wstrcmpi(reconf->name, successed_config_name))
	{
		InitConfig();
		return 1;
	}

	return 0;
}


DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "CMyBook_BasePage")
DECLARE_PAGE_DESC_MSG(ELF_TERMINATE_EVENT, CMyBook::TerminateElf)
DECLARE_PAGE_DESC_MSG(ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo)
DECLARE_PAGE_DESC_MSG(UI_CONNECTIONMANAGER_SESSION_ESTABLISHED_EVENT_TAG, CMyBook::_OnInternetConnected)
DECLARE_PAGE_DESC_MSG(ELF_RECONFIG_EVENT, CMyBook::ReconfigElf)
DECLARE_PAGE_DESC_END

//бука создаётся
CMyBook::CMyBook()  : CBook(MAINBOOKNAME, &base_page)
{
	elf_addref();
	InitConfig();
}

//бука убивается
CMyBook::~CMyBook()
{
	elf_release();
}

int CMyBook::FindBookExCallback(BOOK* book , int* param)
{
	return 0 == strcmp(book->xbook->name, (char*)param);
}

CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*)FindBookEx(FindBookExCallback, (int*)MAINBOOKNAME);
}
