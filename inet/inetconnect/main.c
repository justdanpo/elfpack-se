#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "..\common\IUnknown.hpp"
#include "..\common\Sockets.hpp"
#include "..\common\ConnectionManager.hpp"


void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int main (void)
{
	IConnManagerManager * pConnManagerManager=0;
	IConnManagerParameters * pConnManagerParameters=0;
	IConnManager * pConnManager=0;

	CoCreateInstance(&CID_CConnManagerManager,&IID_IConnManagerManager,PPINTERFACE(&pConnManagerManager));
	pConnManagerManager->CreateConnManagerParameters(0x102/*идентификатор аккаунта (см. inetaccountlist)*/,&pConnManagerParameters);
	pConnManagerParameters->SetConnectionNotify(0);

	pConnManagerManager->OpenConnection(0,0x0,0,pConnManagerParameters,&pConnManager);

	pConnManagerParameters->Release();
	pConnManagerManager->Release();


	SUBPROC(elf_exit);
	return(0);
}
