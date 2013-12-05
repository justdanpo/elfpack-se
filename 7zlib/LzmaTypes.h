/* 
LzmaTypes.h 

Types for LZMA Decoder

This file written and distributed to public domain by Igor Pavlov.
This file is part of LZMA SDK 4.40 (2006-05-01)
*/

#ifndef __LZMATYPES_H
#define __LZMATYPES_H

typedef unsigned char Byte;
#ifndef UInt16
typedef unsigned short UInt16;
#endif
#ifdef _LZMA_UINT32_IS_ULONG
typedef unsigned long UInt32;
#else
#ifndef UInt32
typedef unsigned int UInt32;
#endif
#endif

/* #define _LZMA_SYSTEM_SIZE_T */
/* Use system's size_t. You can use it to enable 64-bit sizes supporting */


typedef unsigned long SizeT;



#endif
