#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

int main (void)
{
	wchar_t tmp[256];
	wchar_t outs[512];

	wchar_t fname[32];

	char* lng=lang_get_name(get_system_langID());

	fname[0]=lng[0];
	fname[1]=lng[1];
	wstrcpy(&fname[2], L"_labels.txt");

	int f=_fopen( GetDir(DIR_OTHER|MEM_INTERNAL), fname, FSX_O_CREAT|FSX_O_WRONLY, FSX_S_IWRITE|FSX_S_IREAD, 0);
	if(f>=0)
		for(int i=1;;i++)
		{
			TextID_GetWString(i,tmp,256);
			snwprintf(outs,512,L"%04X: %ls\r\n",i,tmp);
			fwrite(f,outs,wstrlen(outs)*2);
			if(!wstrcmp(tmp,L"Last label."))break;
		}
	fclose(f);

	SUBPROC( mfree_adr(), &ELF_BEGIN );
	return(0);
}
