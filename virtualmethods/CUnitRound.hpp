#pragma once

#include "cunit.hpp"


class CUnitRound : public CUnit
{
protected:
	int state;
	int x, y, dx, dy;
	int xsize, ysize;
	GVI_BRUSH brush;
public:
	CUnitRound();
	~CUnitRound();
	void Draw(GVI_GC gc);
	void DoStep();
};

class CUnitRoundFactory : public CUnitFactory
{
protected:
	CUnit* CreateUnit()
	{
		return new CUnitRound();
	}
};
