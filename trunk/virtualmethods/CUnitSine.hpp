#pragma once

#include "cunit.hpp"


class CUnitSine : public CUnit
{
protected:
	int state;
	int x, dx;
	int xsize, ysize;
	GVI_PEN pen;
public:
	CUnitSine();
	~CUnitSine();
	void Draw(GVI_GC gc);
	void DoStep();
};

class CUnitSineFactory : public CUnitFactory
{
protected:
	CUnit* CreateUnit()
	{
		return new CUnitSine();
	}
};
