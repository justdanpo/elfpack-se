
typedef struct HELLO_WORLD_DLL_DATA
{
  wchar_t * dll_name; // об€зательное поле.не перемещать!!! необходимо дл€ UnLoadDLL
  void (*method0)(HELLO_WORLD_DLL_DATA*);
  void (*method1)( HELLO_WORLD_DLL_DATA * data, int n);
  void (*method2)( HELLO_WORLD_DLL_DATA * data);
  void (*method3)( HELLO_WORLD_DLL_DATA * data);
  int property0;
  int property1;

#ifdef DLL_PRIVATE_AREA
#include "HelloWorldDll_private.h"
#endif

}_HELLO_WORLD_DLL_DATA;


#define HELLOWORLD_Show_func0(__this__)  __this__->method0(__this__)
#define HELLOWORLD_Show_int(__this__,...) __this__->method1(__this__,__VA_ARGS__)
#define HELLOWORLD_Show_and_inc_private_int(__this__)  __this__->method2(__this__)
#define HELLOWORLD_Show_private_HelloWorld(__this__)  __this__->method3(__this__)
