#ifndef _CRYPT_H_
  #define _CRYPT_H_

void *encrypt(void *outbuf, void *inpbuf, unsigned long len, unsigned long poly);
void *decrypt(void *outbuf, void *inpbuf, unsigned long len, unsigned long poly);
unsigned long myCrc32(unsigned char *buf, unsigned long len);

#endif //_CRYPT_H_
