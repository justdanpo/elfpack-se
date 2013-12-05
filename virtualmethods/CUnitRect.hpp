#pragma once

#include "cunit.hpp"


class CUnitRect : public CUnit
{
protected:
	int state;
	int x, y, dx, dy;
	int xsize, ysize;
	GVI_BRUSH brush;
public:
	CUnitRect();
	~CUnitRect();
	void Draw(GVI_GC gc);
	void DoStep();
};

class CUnitRectFactory : public CUnitFactory
{
protected:
	CUnit* CreateUnit()
	{
		return new CUnitRect();
	}
};
