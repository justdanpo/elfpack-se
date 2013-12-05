#include "..\deleaker\mem2.h"

#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"
#include "rand.h"

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (void)
{
	trace_init();
	
	DATETIME dt;
	REQUEST_DATEANDTIME_GET(0,&dt);
	
	srand( (dt.time.sec<<16) | (dt.time.min<<8) | (dt.time.hour) );
	
	new CMyBook();
	
	return 0;
}
