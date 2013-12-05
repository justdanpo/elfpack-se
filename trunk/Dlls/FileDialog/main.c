#include "..\\..\\include\Lib_Clara.h"
#include "..\\..\\include\Dir.h"
#include "..\..\\include\dll.h"
#include <stdarg.h>


#define DLL_PRIVATE_AREA  // ������������ ������!! ��� ���������� ����� DLL
#include "export\FileDialogDll.h"


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

// -----------------  Dll_System area  -----------------
// ������ ����� DLL. �� ����� �� ���������.

volatile int usage_count;



// -------------------  Public area  -------------------
// �������� � ������ ��������� �������.
// ������ ������� �������� Private area.
// ��� ������������� ����� ������� �������������� � h-�����.
// ����� �� ���� ������� ��� ���������� � ����������� DLL, ����� ��� - DLLFILENAME_FunctionName.
//

wchar_t * CreateFD (FILE_DIALOG* fd,
                    BOOK * book,
                    int flags,
                    wchar_t * header,
                    wchar_t * filters,
                    ...  // folders list
                      )
{
  wchar_t * res;
  va_list ap;
  va_start(ap, filters);
  res=CreateFileDialog (book, flags, header, filters, ap);
  va_end(ap);
  return(res);
}

// -------------------  Private area  ------------------
// ����� ����, ��� ������ ������������ ���������� ���������� � Public �������,
// �� ��������� ���������� � DLLFILENAME_private.h ����������.
// ��� ���������� ����� ����� � �������� DLL 
// -----------------------------------------------------

int main ( int Action , FILE_DIALOG * data )
{
  FILE_DIALOG * p;
  switch (Action)
  {
  case DLL_INIT:
    // ������������� Dll_System area. ���������� ������ ���� ���, ��� �������� � ������.
    // ���� ������� �� ����, �� �������� ������������ � ����� DLL_ERROR_INIT
    // data == NULL
    
    usage_count=0;
    
    
    return(0);
    
  case DLL_LOAD:
    // ������������� Private / Public area. ���������� ������ ���, ��� ������ LoadDLL.
    // data == NULL
    
    usage_count++;                      // ���������� �������� ������������� ���� DLL
    
    if (!(p = new FILE_DIALOG)) return(0);;       // �������� this � ������������� ������� � �������
    
    memset(p,0,sizeof(FILE_DIALOG));
    
    // Public area
    p->CreateFD=CreateFD;
    
    
    // Private area
    
    
    return((int)p);
    
  case DLL_UNLOAD:
    // ������������ Private / Public area. ���������� ������ ���, ��� ������ UnLoadDLL.
    // data == this
    
    //  ����������� ��� ������������� data->...
    
    
    //  ����������� ������ data
    mfree(data);
    usage_count--;                      // ���������� �������� ������������� ���� DLL
    
    // ���� ������ ����� �� ����������...
    if(!usage_count)
    {
      //
      //  ����������� ��� ������������� Dll_System area
      //
      SUBPROC(elf_exit);
    }
    return(usage_count);
    
  }
  return(0);
}


