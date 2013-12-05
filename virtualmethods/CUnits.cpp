#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "cunits.hpp"

#include "cunitrect.hpp"
#include "cunitround.hpp"
#include "cunitstars.hpp"
#include "cunitsine.hpp"

#include "rand.h"

enum
{
	UNITRECT,
	UNITROUND,
	UNITSTARS,
	UNITSINE,
	
	UNITTYPES
};

CUnit* GetRandomUnit()
{
	CUnitRectFactory rectFactory;
	CUnitRoundFactory roundFactory;
	CUnitStarsFactory starsFactory;
	CUnitSineFactory sineFactory;
	
	CUnitFactory* factory = NULL;
	
	switch(rand()%UNITTYPES)
	{
	case UNITRECT:
		factory = &rectFactory;
		break;
	case UNITROUND:
		factory = &roundFactory;
		break;
	case UNITSTARS:
		factory = &starsFactory;
		break;
	case UNITSINE:
		factory = &sineFactory;
		break;
	}
	
	return factory->GetUnit();
}
