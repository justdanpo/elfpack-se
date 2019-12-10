#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int main (void)
{
	//можно было через IInternetAccountManager->DisconnectConnections(). фактически, это абсолютно тот же вызов
	Request_ICA_ShutdownAllConnections(SYNC);

	SUBPROC(elf_exit);
	return(0);
}
