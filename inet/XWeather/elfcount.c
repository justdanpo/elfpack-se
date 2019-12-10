#include "..\..\include\lib_clara.h"

static int elfcount=1;

int elf_addref()
{
   debug_printf("\n! elf_addref: set elfcount = %d\n", elfcount+1);
   return ++elfcount;
}

int elf_release()
{
   debug_printf("\n! elf_release: set elfcount = %d\n", elfcount-1);
   if(0 == --elfcount)
   {
      debug_printf("\n! elf free...\n");
      SUBPROC( mfree_adr(), &ELF_BEGIN );
   }
   return elfcount;
}
