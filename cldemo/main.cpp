#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elf_exit(void)
{
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

int main (void)
{
	new CMyBook();

	return 0;
}
