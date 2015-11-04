#include "mybook.h"
#include "../classlib/softkeyutil.hpp"
#include "maindisp.hpp"
#include "globals.h"
#include "../include/dll.h"
#include "conf_loader.h"

extern void elf_exit();

int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	if(!FindBook(get_IsCameraBook()))
		book->Free();
	return 1;
}


int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID, STR(ABOUTTEXT), NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}


DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, MYBOOKBASEPAGE)
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_MSG( ELF_RECONFIG_EVENT, CMyBook::ReconfigElf )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook(MYBOOKNAME, &base_page)
{
	InitConfig();
	
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _PageHookCB, this, PAGE_HOOK_ADD_AFTER);
	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _PageHookCB2, this, PAGE_HOOK_ADD_BEFORE);
	
	acc_dll = (ACCELEROMETER_DLL_DATA*)LoadDLL(L"accelerometer.dll");
	
	if(INVALID(acc_dll))
		acc_dll = NULL;
	else if((ACCELEROMETER_Version(acc_dll) < ACCELEROMETER_DLL_VERSION_NUMBER)
			|| !ACCELEROMETER_IsPhoneSupported(acc_dll))
	{
		UnLoadDLL(acc_dll);
		acc_dll = NULL;
	}

	//todo: чтение старого значения
	type = 0;
}


CMyBook::~CMyBook()
{
	if(acc_dll)
	{
		ACCELEROMETER_Close(acc_dll);
		UnLoadDLL(acc_dll);
	}

	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _PageHookCB2, this, PAGE_HOOK_REMOVE);
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _PageHookCB, this, PAGE_HOOK_REMOVE);
	elf_exit();
}


int CMyBook::FindBookExCB_Name( BOOK* book , int* param )
{
	return 0 == strcmp( book->xbook->name, (char*)param );
}

int CMyBook::FindBookExCB_Book( BOOK* book , int* param )
{
	return book == (BOOK*)param;
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


CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*) FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME );
}


int CMyBook::_PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->PageHookCB( msg, book, page_desc, event );
}


int CMyBook::PageHookCB(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event)
{
	bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );
	
	if( exists )
	{
		if(!strcmp(book->xbook->name, "CameraBook") && !strcmp(page_desc->name,"CameraBook_Viewfinder_Page"))
		{
			//todo: проверка на дубли?
			CGuiBase* cameragui = reinterpret_cast<CGuiBase*>( List_Get(book->xguilist->guilist, 0) );
			CDispObjBase* lastdisp = cameragui->GetDISPObj();
			
			
			CGuiBase* gui = new CGuiT<CMainDisp>( book, 0 );

			gui->SetStyle( UI_OverlayStyle_FullScreen );
			gui->SetTitleType( UI_TitleMode_None );
		
			gui->GetDISPObj()->Show(1);//dispobj show instead of gui show to prevent focus change
			
			static_cast<CMainDisp*>( gui->GetDISPObj()) ->Setup(cameragui, acc_dll, &type);
		}
	}
	
	return 0;
}

int CMyBook::_PageHookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->PageHookCB2( msg, book, page_desc, event );
}


int CMyBook::PageHookCB2(void *msg, CBook* book, PAGE_DESC * page_desc, u16 event)
{
	if(!strcmp(book->xbook->name, "CameraBook") && !strcmp(page_desc->name,"CameraBook_Viewfinder_Page"))
	{
		for(size_t guinum = book->xguilist->guilist->FirstFree; guinum; --guinum)
		{
			CGuiBase* gui = reinterpret_cast<CGuiBase*>( List_Get(book->xguilist->guilist, guinum-1) );
			CDispObjBase* disp = gui->GetDISPObj();
			if(!strcmp(disp->GetDESC()->name, MYDISPNAME))
			{
				gui->Free();
			}
		}
	}
	return 0;
}
