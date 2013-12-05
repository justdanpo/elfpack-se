#pragma once

#include "..\..\include\lib_clara.h"

class Vkp
{
private:
    //unsigned delta;
    
    bool isthatchar(const char* addr,const long size,const long position,long& elementsize,const char* ch);
    bool space(const char* addr,long size,long position,long& elementsize);
    bool eol(const char* addr,long size,long position,long& elementsize);
    bool eos(const char* addr,long size,long position,long& elementsize);
    
    bool deltaoffset(const char* addr,long size,long position,long& elementsize);
    bool patchstring(const char* addr,long size,long position,long& elementsize);
    bool anyline(const char* addr,long size,long position,long& elementsize);
    
    void setbyte( unsigned addr, unsigned char data );
    
    LIST* addrs;
    LIST* values;
    int errorline;
public:
    Vkp();
    ~Vkp();
    
    //std::string errorstring;
    int  dovkp(const char* addr,long size);
    int gettable( void** ret );
};
