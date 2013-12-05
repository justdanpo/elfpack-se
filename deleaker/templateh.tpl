#ifndef __INCLUDE_MEM__
#define __INCLUDE_MEM__

#include "..\include\Lib_Clara.h"
#include "Lib_Clara_original.h"

#ifdef NDEBUG

#define trace_init(f)
#define trace_done()

void* operator new(size_t sz);
void* operator new[](size_t sz);
void operator delete(void * p);
void operator delete[](void * p);

void* operator new(size_t size, void* p);
void* operator new[](size_t size, void* p);
void operator delete (void*, void*);
void operator delete[] (void*, void*);

#else

#define DELEAKER

enum trace_types
{
	trace_memory,
	trace_strid,
	trace_iconid,
	trace_timer,
	trace_file,
	trace_hook,
	trace_dll,
	trace_gc,
	trace_gvi,
	trace_gui,
	trace_book,
	trace_process,
	trace_osebuff,
	trace_metadatadesc,
	trace_fileitemstruct,
	trace_w_dir,

	trace_unallocated,

	trace_typescount
};

#ifndef __cplusplus
void trace_init(wchar_t*);
#else
void trace_init(wchar_t* logname=NULL);
#endif
void trace_done();
void* trace_alloc_ret(int mt, void* ptr, void* badvalue, const char* file, int line);
void __deleaker_pushfileline( const char* __file__, int __line__ );

void operator delete(void* p);
void operator delete[](void* p);

void* operator new(size_t sz);
void* operator new[](size_t sz);

void operator delete (void*, void*);
void operator delete[] (void*, void*);

void* operator new(size_t size, void* p);
void* operator new[](size_t size, void* p);

#define new (__deleaker_pushfileline(__FILE__, __LINE__),false) ? NULL : new
#define delete __deleaker_pushfileline(__FILE__, __LINE__), delete




//!!! обязательно проверять аргументы
#define CreateStringInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateStringInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateDateInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateDateInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreatePercentInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreatePercentInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateStringInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateStringInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateTimeInputVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateTimeInputVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )
#define CreateYesNoQuestionVA( a, ... ) (GUI*) trace_alloc_ret( trace_gui, __original_CreateYesNoQuestionVA( a, __VA_ARGS__), NULL, __FILE__, __LINE__ )

