#include "..\\..\\include\Lib_Clara.h"
#include "..\\..\\include\Dir.h"
#include "..\..\\include\dll.h"
#include <stdarg.h>


#define DLL_PRIVATE_AREA  // использовать ТОЛЬКО!! при компиляции самой DLL
#include "export\FileDialogDll.h"


void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

// -----------------  Dll_System area  -----------------
// данные самой DLL. их копии не создаются.

volatile int usage_count;



// -------------------  Public area  -------------------
// Свойсива и методы доступные снаружи.
// Внутри методов доступна Private area.
// Для читабельности имена методов переопределены в h-файле.
// Чтобы не было проблем при компиляции с несколькими DLL, имена даём - DLLFILENAME_FunctionName.
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
// Ввиду того, что нельзя использовать глобальные переменные в Public методах,
// то описываем переменнуй в DLLFILENAME_private.h используем.
// Эта переменная будет видна в пределах DLL 
// -----------------------------------------------------

int main ( int Action , FILE_DIALOG * data )
{
  FILE_DIALOG * p;
  switch (Action)
  {
  case DLL_INIT:
    // инициалицация Dll_System area. вызывается только один раз, при загрузке в память.
    // если вернуть не ноль, то загрузка прекращается с кодом DLL_ERROR_INIT
    // data == NULL
    
    usage_count=0;
    
    
    return(0);
    
  case DLL_LOAD:
    // инициалицация Private / Public area. вызывается каждый раз, при вызове LoadDLL.
    // data == NULL
    
    usage_count++;                      // увеличение счетчика использования этой DLL
    
    if (!(p = new FILE_DIALOG)) return(0);;       // создание this и инициализация свойств и методов
    
    memset(p,0,sizeof(FILE_DIALOG));
    
    // Public area
    p->CreateFD=CreateFD;
    
    
    // Private area
    
    
    return((int)p);
    
  case DLL_UNLOAD:
    // освобождение Private / Public area. вызывается каждый раз, при вызове UnLoadDLL.
    // data == this
    
    //  освобождеем при необходимости data->...
    
    
    //  освобождеем всегда data
    mfree(data);
    usage_count--;                      // уменьшение счетчика использования этой DLL
    
    // если больше никто не использует...
    if(!usage_count)
    {
      //
      //  освобождеем при необходимости Dll_System area
      //
      SUBPROC(elf_exit);
    }
    return(usage_count);
    
  }
  return(0);
}


