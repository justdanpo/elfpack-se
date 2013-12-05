#pragma once

#include "..\include\types.h"

class CUnit
{
public:
	CUnit(){}
	virtual ~CUnit(){}
	
	virtual void Draw(GVI_GC gc) = 0;
	virtual void DoStep() = 0;
};

class CUnitFactory
{
public:
	CUnit* GetUnit()
	{
		return CreateUnit();
	}
protected:
	virtual CUnit* CreateUnit() = 0;
};
