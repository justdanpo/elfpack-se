#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "header\wdisplay.h"

#pragma optimize = s 9
void metadata_reset(METADATA *meta)
{
  wstrcpy(meta->Title,L" ");
  wstrcpy(meta->Album,L" ");
  wstrcpy(meta->Artist,L" ");
  wstrcpy(meta->Year,L" ");
  wstrcpy(meta->Genre,L" ");
  
  memset(meta->Title,0,256);
  memset(meta->Album,0,256);
  memset(meta->Artist,0,256);
  memset(meta->Year,0,256);
  memset(meta->Genre,0,256);
};
