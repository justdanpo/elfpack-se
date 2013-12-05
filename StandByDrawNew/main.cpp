#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (wchar_t* filename)
{
	trace_init(L"sbdrawnew_deleaker.txt");
	CMyBook* book = CMyBook::GetExisting();
	if(book != NULL)
	{
		book->UI_Event( ELF_TERMINATE_EVENT );
		MessageBox(EMPTY_TEXTID,STR("stopped"),NOIMAGE,1,1000,0);
		elf_exit();
	}else
	{
		new CMyBook();
		if(NULL == wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)) )
			MessageBox(EMPTY_TEXTID,STR("started"),NOIMAGE,1,1000,0);
	}

	return 0;
}
