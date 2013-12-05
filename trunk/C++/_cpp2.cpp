#include "..\include\Lib_Clara.h"


void* operator new(size_t sz){return malloc(sz);};
void* operator new[](size_t sz){return malloc(sz);};
void operator delete(void * p){mfree(p);};
void operator delete[](void * p){mfree(p);};

//placement version
void* operator new(size_t size, void* p){ return p; }
void* operator new[](size_t size, void* p){ return p; }
void operator delete (void*, void*) { }
void operator delete[] (void*, void*) { }
