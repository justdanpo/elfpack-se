#include "..\include\Lib_Clara.h"

#include "crypt.h"
unsigned long mycrc_table[256];

void make_mycrc_table(unsigned long poly)
{
  unsigned long crc;
  for (int i = 0; i < 256; i++)
  {
    crc = i;
    for (int j = 0; j < 8; j++)
      crc = crc & 1 ? (crc >> 1) ^ poly : crc >> 1;
    mycrc_table[i] = crc;
  }
}

unsigned long myCrc32(unsigned char *buf, unsigned long len)
{
  unsigned long crc = 0xFFFFFFFFUL;
  make_mycrc_table(0xEDB89320UL);
  while (len--)
    crc = mycrc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
  return crc ^ 0xFFFFFFFFUL;
};

void *encrypt(void *outbuf, void *inpbuf, unsigned long len, unsigned long poly)
{
  unsigned char *y = (unsigned char *)outbuf;
  unsigned char *x = (unsigned char *)inpbuf;
  make_mycrc_table(poly);
  if(len > 256) len = 256;
  for(int i=0; i<len; i++)
  {
    y[i] = x[i]^mycrc_table[i];
  }
  return outbuf;
}

void *decrypt(void *outbuf, void *inpbuf, unsigned long len, unsigned long poly)
{
  return encrypt(outbuf, inpbuf, len, poly);
}

