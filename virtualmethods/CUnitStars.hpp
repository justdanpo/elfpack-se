#pragma once

#include "cunit.hpp"


class CUnitStars : public CUnit
{
protected:
	int state;
	int x, y, dx, dy;
	int xsize, ysize;
	GVI_PEN pen;
public:
	CUnitStars();
	~CUnitStars();
	void Draw(GVI_GC gc);
	void DoStep();
};

class CUnitStarsFactory : public CUnitFactory
{
protected:
	CUnit* CreateUnit()
	{
		return new CUnitStars();
	}
};
