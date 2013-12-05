#include "..\..\\include\Lib_Clara.h"
//#include "..\..\\include\Dir.h"
#include "..\..\\include\dll.h"

#define DLL_PRIVATE_AREA  // использовать ТОЛЬКО!! при компиляции самой DLL

#include "export\HelloWorldDll.h"

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
// Чтобы не было проблем при компиляции с несколькими DLL, имена - DLLFILENAME_FunctionName.
//

void func0( HELLO_WORLD_DLL_DATA * data)
{
  debug_printf("\nhelloworld.dll: func0\n");
  StatusIndication_ShowNotes(STR("func0"));
}

void func1( HELLO_WORLD_DLL_DATA * data, int n)
{
  MessageBox(EMPTY_TEXTID,STR("Hello DLL!\n\nfunc1 has been called"),NOIMAGE, 1 ,5000, 0);
  debug_printf("\nhelloworld.dll: func1 - %d\n",n);
  StatusIndication_ShowNotes(TextID_CreateIntegerID(n));
}

void func2( HELLO_WORLD_DLL_DATA * data)
{
  debug_printf("\nhelloworld.dll: func2 - %d\n",data->private_int1);
  StatusIndication_ShowNotes(TextID_CreateIntegerID(data->private_int1++));
}

void func3( HELLO_WORLD_DLL_DATA * data)
{
  debug_printf("\nhelloworld.dll: func3 - %ls\n",data->private_wstr);
  StatusIndication_ShowNotes(TextID_Create(data->private_wstr,ENC_UCS2,TEXTID_ANY_LEN));
}
// -------------------  Private area  ------------------
// Ввиду того, что нельзя использовать глобальные переменные в Public методах,
// то описываем переменнуй в DLLFILENAME_private.h используем.
// Эта переменная будет видна в пределах DLL ( func2 , func3 , main )
// -----------------------------------------------------

int main ( int Action , HELLO_WORLD_DLL_DATA * data )
{
  HELLO_WORLD_DLL_DATA * p;
  switch (Action)
  {
  case DLL_INIT:
    // инициалицация Dll_System area. вызывается только один раз, при загрузке в память.
    // если вернуть не ноль, то загрузка прекращается с кодом DLL_ERROR_INIT
    // data == NULL
    debug_printf("\nhelloworld.dll: dll init\n");
    usage_count=0;


    return(0);

  case DLL_LOAD:
    debug_printf("\nhelloworld.dll: dll load\n");
    // инициалицация Private / Public area. вызывается каждый раз, при вызове LoadDLL.
    // data == NULL

    usage_count++;                      // увеличение счетчика использования этой DLL

    if (!(p = new HELLO_WORLD_DLL_DATA)) return(0);;       // создание this и инициализация свойств и методов

    memset(p,0,sizeof(HELLO_WORLD_DLL_DATA));

    // Public area
    p->method0=func0;
    p->method1=func1;
    p->method2=func2;
    p->method3=func3;

    p->property0=12345;
    p->property1=-12345;

    // Private area
    p->private_int1=0;
    p->private_wstr=new wchar_t[50];
    wstrcpy(p->private_wstr,L"HelloWorld");

    return((int)p);

  case DLL_UNLOAD:
    debug_printf("\nhelloworld.dll: dll unload\n");
    // освобождение Private / Public area. вызывается каждый раз, при вызове UnLoadDLL.
    // data == this

    //  освобождеем при необходимости data->...
    if (data->private_wstr) mfree(data->private_wstr);

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
