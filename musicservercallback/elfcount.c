#include "..\include\lib_clara.h"

static int elfcount=1;

int elf_addref()
{
	return ++elfcount;
}

int elf_release()
{
	if(0 == --elfcount)
		SUBPROC( mfree_adr(), &ELF_BEGIN );
	return elfcount;
}
