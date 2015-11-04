#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

int main (void)
{
	CMyBook* pbook = CMyBook::GetExisting();

	if( !pbook )
	{
		new CMyBook();
	}else
	{
		pbook->UI_Event( ELF_TERMINATE_EVENT );
		SUBPROC( mfree_adr(), &ELF_BEGIN );
	}

	return 0;
}
