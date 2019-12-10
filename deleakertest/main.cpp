#include "..\deleaker\mem2.h"

#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

#include "..\classlib\cl.h"
#include "mybook.h"

void elf_exit(void)
{
	trace_done();
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}

class NestedAllocs
{
	char* x;
public:
	NestedAllocs()
	{
		x=new char[10];
	}
	~NestedAllocs()
	{
		delete x;
	}
};

int main (void)
{
	trace_init(L"deleakertest.txt");

	NestedAllocs* nested=new NestedAllocs; //тест вложенных делитов. для него ещё в mem2.cpp надо
	delete nested;                         //дебагвывод добавить, но не для свн

	char* x=new char[3]; //удаляется
	new ((char*)1) char; //placement new. не должен учитываться.

	delete[] x;
	new char[5];  //не удаляется
	List_Create();   //не удаляется
	malloc(3);    //не удаляется


	new CMyBook();  //удаляется. это самое главное - создание бука. весь эльф живёт в этом буке

	u16 t=TIMER_SET(10000,CMyBook::onTimer,NULL); //удаляется
	Timer_Kill(&t);

	return 0;
}
