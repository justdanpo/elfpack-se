/* 7zBuffer.h */

#ifndef __7Z_BUFFER_H
#define __7Z_BUFFER_H

#include "..\include\Lib_Clara.h"
#include "7zTypes.h"
#include "LzmaTypes.h"

typedef struct _CSzByteBuffer
{
  size_t Capacity;
  Byte *Items;
}CSzByteBuffer;

#ifdef __cplusplus
extern "C" {
#endif
  
void SzByteBufferInit(CSzByteBuffer *buffer);
int SzByteBufferCreate(CSzByteBuffer *buffer, size_t newCapacity, void * (*allocFunc)(size_t size));
void SzByteBufferFree(CSzByteBuffer *buffer, void (*freeFunc)(void *));

#ifdef __cplusplus
}
#endif

#endif
