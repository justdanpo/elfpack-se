#include "..\..\deleaker\mem2.h"
#include "elfcount.h"
#include "mybook.h"
#include "..\..\include\dir.h"

int main(wchar_t* filename)
{
	trace_init(L"nist_deleaker.txt");
	CMyBook* book = CMyBook::GetExisting();
	if(book != NULL)
	{
		book->UI_Event( ELF_TERMINATE_EVENT );
		MessageBox(EMPTY_TEXTID,STR("stopped"),NOIMAGE,1,1000,0);
	}else
	{
		new CMyBook();
		if(NULL == wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)) )
			MessageBox(EMPTY_TEXTID,STR("started"),NOIMAGE,1,1000,0);
	}

	elf_release();

	return 0;
}
