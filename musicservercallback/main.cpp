#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "mybook.h"

#include "elfcount.h"

int main (void)
{
	CMyBook* pbook = CMyBook::GetExisting();

	if( !pbook )
	{
		new CMyBook();
	}else
	{
		pbook->UI_Event( ELF_TERMINATE_EVENT );
	}
	
	elf_release();
}
