#ifndef _DECODER_H_
  #define _DECODER_H_

  HEADER_INFO ReadMP3Information(wchar_t *path, wchar_t *name);
  int synchsafeToNormal(char tagSize[4]);
  void metadata_reset(METADATA *meta);
  
#endif

