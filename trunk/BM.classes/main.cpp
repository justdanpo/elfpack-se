#include "..\deleaker\mem2.h"
#include "daemon.hpp"

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (void)
{
	trace_init(L"bmclassesmem.txt");

	CDaemonBook* pbook = CDaemonBook::GetExisting();
	if( !pbook )
	{
		new CDaemonBook();
	}else
	{
		pbook->UI_Event( ELF_TERMINATE_EVENT );
		elf_exit();
	}

	return 0;
}
