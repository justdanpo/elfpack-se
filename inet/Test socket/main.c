#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "..\common\IUnknown.hpp"
#include "..\common\Sockets.hpp"
#include "..\common\ConnectionManager.hpp"

#include "Callbacks.h"
#include "main.h"


void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int TerminateElf(void * ,BOOK * book)
{
	FreeBook(book);
	return(1);
}


int ShowAuthorInfo(void *mess ,BOOK * book)
{
	MSG * msg = (MSG*)mess;
	MessageBox(EMPTY_TEXTID,STR("Test Socket\n\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
	return(1);
}



int TestSocketBook_MainPage_Enter(void * r0,BOOK * book)
{
	TestSocketBook * TSBook = (TestSocketBook*)book;
	
	CreateCallbacks(&TSBook->pICBTestConnManager,&TSBook->pICBTestSocket);
	
	IConnManagerManager * pConnManagerManager=0;
	IConnManagerParameters * pConnManagerParameters=0;
	IUnknown * pConnManagerBaseParameters=0;
	
	CoCreateInstance(&CID_CConnManagerManager,&IID_IConnManagerManager,PPINTERFACE(&pConnManagerManager));
	pConnManagerManager->CreateConnManagerParameters(0x101,&pConnManagerParameters);
	pConnManagerParameters->SetConnectionNotify(5);
	pConnManagerParameters->QueryInterface(&IID_IConnManagerBaseParameters,PPINTERFACE(&pConnManagerBaseParameters));
	pConnManagerManager->OpenConnection(TSBook->pICBTestConnManager,0x0,TSBook,pConnManagerBaseParameters,&TSBook->pIConnManager);
	
	pConnManagerBaseParameters->Release();
	pConnManagerParameters->Release();
	pConnManagerManager->Release();
	
	return 0;
}



const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, TestSocketBook_MainPage_Enter,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC TestSocketBook_Main_Page = { "TestSocketBook_Main_Page", 0, bk_main_msglst };


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
	ELF_TERMINATE_EVENT , TerminateElf,
	ELF_SHOW_INFO_EVENT , ShowAuthorInfo,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC TestSocketBook_Base_Page = { "TestSocketBook_Base_Page", 0, bk_base_msglst };


void TSBook_Destroy( BOOK * book )
{
	TestSocketBook * TSBook = (TestSocketBook*)book;
	
	if (TSBook->pIConnManager)
	{
		TSBook->pIConnManager->CloseConnection();
		TSBook->pIConnManager->Release();
	}
	if (TSBook->pICBTestConnManager) TSBook->pICBTestConnManager->Release();
	if (TSBook->pICBTestSocket) TSBook->pICBTestSocket->Release();
	if (TSBook->pISocket) TSBook->pISocket->Release();
	
	//  SUBPROC(elf_exit);
}


void CreateTestSocketBook()
{
	TestSocketBook * TSBook = (TestSocketBook*)malloc(sizeof(TestSocketBook));
	CreateBook(TSBook,TSBook_Destroy,&TestSocketBook_Base_Page,"Test Socket",-1,0);
	TSBook->pICBTestConnManager=0;
	TSBook->pIConnManager=0;
	TSBook->pICBTestSocket=0;
	TSBook->pISocket=0;
	
	BookObj_GotoPage(TSBook,&TestSocketBook_Main_Page);
}


int isTestSocketBook(BOOK* book)
{
	if(!strcmp(book->xbook->name,"Test Socket")) return(1);
	return(0);
}


int main (void)
{
	if (FindBook(isTestSocketBook))
	{
		MessageBox(EMPTY_TEXTID, STR("Already runed"), NOIMAGE, 1, 5000, 0);
		SUBPROC(elf_exit);
	}
	else
	{
		CreateTestSocketBook();
	}
	return 0;
}
