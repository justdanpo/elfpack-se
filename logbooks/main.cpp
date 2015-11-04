#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elf_exit()
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (void)
{
	trace_init(L"logbooks_deleaker.txt");

	CMyBook* pbook = CMyBook::GetExisting();

	if( !pbook )
	{
		new CMyBook();
	}else
	{
		pbook->UI_Event( ELF_TERMINATE_EVENT );
		elf_exit();
	}

	return 0;
}
