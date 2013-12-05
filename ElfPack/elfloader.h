#ifndef _ELFLOADER_H_
#define _ELFLOADER_H_

#ifdef wintel
typedef char *FILENAME;
#else
typedef wchar_t* FILENAME;
#endif

long elfload_int(const FILENAME filename, void* param1, void* param2, void* param3);

#endif
