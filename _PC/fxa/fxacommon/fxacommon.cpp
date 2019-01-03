#include "fxacommon.h"

#include <ctype.h>

unsigned int gethash(const wchar_t* str)
{
	unsigned int hash=0x1505;
	for(;str && *str>=' '; str++)
		hash = hash*0x21 + towlower(*str);
	if(!hash)
		return 1;
	return hash;
}

uint32_t INTBE::get()
{ 
	return (((((data[0]<<8)+data[1])<<8)+data[2])<<8)+data[3]; 
}

void INTBE::set(uint32_t d)
{
	data[0]=d>>24;
	data[1]=d>>16;
	data[2]=d>>8;
	data[3]=d;
}