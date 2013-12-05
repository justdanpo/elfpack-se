#ifndef _CALLS_H_
#define _CALLS_H_

#include "..\include\Types.h"
#include "temp\target.h"

#if defined(DB2020)
//DB2020
extern void *memalloc(int zero, int size,int f1,int f2,const char *fname,int fline);
extern void memfree(int zero, void * mem,const char * fname ,int fline);
#elif defined(A2)
//A2
extern void *memalloc(int minus_one, int size,int f1,int f2,const char *fname,int fline);
extern void memfree(int zero, void * mem,const char * fname ,int fline);
#else
//DB2000/2010
extern void *memalloc(int size,int f1,int f2,const char *fname,int fline);
extern void memfree(void * mem,const char * fname ,int fline);
#endif

extern void* memset( void* mem, char chr, int size );
extern int memcpy( void* dest, const void* source, int cnt );
extern int _fopen( const wchar_t* filpath, const wchar_t* filname, unsigned int mode, unsigned int rights, unsigned int __0 );
extern wchar_t* wstrrchr( const wchar_t* wstr, wchar_t wchar );
extern int wstrlen( const wchar_t* wstr );
extern wchar_t* wstrcpy( wchar_t* dest, const wchar_t* source );
extern wchar_t* wstrcat( wchar_t* wstr, const wchar_t* subwstr );
extern int List_Find( LIST* lst, void* itemtofind, LISTFINDCALLBACK cmp_proc );
extern void* List_RemoveAt( LIST* lst, int index );
extern void* List_Get( LIST* lst, int index );
extern int List_InsertFirst( LIST* lst, void* item );
extern int List_IndexOf( LIST* lst, void* item );
extern int List_InsertLast( LIST* lst, void* item );
extern LIST* List_Create( void );
extern DIR_HANDLE* AllocDirHandle( const wchar_t* path );
extern FILELISTITEM* GetFname( DIR_HANDLE*, FILELISTITEM* );
extern void DestroyDirHandle( DIR_HANDLE* handle );
extern OSBOOLEAN set_envp(PROCESS pid, char *name, OSADDRESS value);
extern PROCESS get_bid(PROCESS pid);
extern PROCESS current_process(void);

#endif
