#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"


const char x[]="some string";

void elf_exit()
{
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

const int tmpxi = (int)x;

const PAGE_MSG bk_msglst_base[] = 
{
  (int)x,          0,
};

//const PAGE_DESC bk_base = {"ASR_Base_Page",0,bk_msglst_base};


int main (void)
{
	wchar_t tmp[256];
	
	snwprintf(tmp,MAXELEMS(tmp),_T("%x - %x"), tmpxi, bk_msglst_base[0].msg);

  MessageBox(EMPTY_TEXTID,TextID_Create(tmp,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,0,0);
	
	elf_exit();	
  return(0);
}
