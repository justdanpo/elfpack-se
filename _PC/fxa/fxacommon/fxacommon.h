#ifndef _FXACOMMON_H_
#define _FXACOMMON_H_

#include "..\..\common\stdint.h"

unsigned int gethash(const wchar_t* str);

class INTBE
{
	uint8_t data[4];
public:
	uint32_t get();
	void set(uint32_t d);
};

struct FXAITEM
{
	INTBE offset;
	INTBE size;
	INTBE path;
	INTBE file;
};

#endif

