#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elfexit()
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (wchar_t* filename)
{
	trace_init(L"smsviewrew_dlk.txt");

	CMyBook* pbook = CMyBook::GetExisting();

	if( !pbook )
	{
		new CMyBook();
		if(NULL == wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)) )
			MessageBox(EMPTY_TEXTID,STR("started"),NOIMAGE,1,1000,0);
	}else
	{
		pbook->UI_Event( ELF_TERMINATE_EVENT );
		MessageBox(EMPTY_TEXTID,STR("stopped"),NOIMAGE,1,1000,0);		
		elfexit();
	}

	return 0;
}
