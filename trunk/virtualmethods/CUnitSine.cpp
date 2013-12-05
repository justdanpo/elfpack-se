#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "cunitsine.hpp"
#include "rand.h"
#include "sin.h"

CUnitSine::CUnitSine()
{
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	
	state = rand() % 360;
	x = ( rand()%(xsize-40) ) + 20;
	
	dx = rand()%4 + 4;
	
	pen = GVI_CreatePen(5, 0x8000FFFF);
}

CUnitSine::~CUnitSine()
{
	GVI_Delete_GVI_Object(&pen);
}

void CUnitSine::Draw(GVI_GC gc)
{
	int rmax = sin( state * 3 )*20/256 + 40;
	
	POINT p[10];
	for(int i=0; i< MAXELEMS(p); i++)
	{
		p[i].y = i * ysize / ( MAXELEMS(p)-1 );
		p[i].x = rmax * sin( i *20 + x )/256 + xsize/2;
	}
	GVI_DrawPolyLine(gc, p, 10, pen, 0);
}

void CUnitSine::DoStep()
{
	state = ( state + 1 ) % 360;
	
	x = (x + dx) % 360;
}
