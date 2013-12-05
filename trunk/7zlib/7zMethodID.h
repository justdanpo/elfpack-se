/* 7zMethodID.h */

#ifndef __7Z_METHOD_ID_H
#define __7Z_METHOD_ID_H

#include "..\include\Lib_Clara.h"
#include "7zTypes.h"
#include "LzmaTypes.h"

#define kMethodIDSize 15
  
typedef struct _CMethodID
{
  Byte ID[kMethodIDSize];
  Byte IDSize;
} CMethodID;

#ifdef __cplusplus
extern "C" {
#endif
int AreMethodsEqual(CMethodID *a1, CMethodID *a2);
#ifdef __cplusplus
}
#endif
#endif
