#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "cunitround.hpp"
#include "rand.h"
#include "sin.h"

CUnitRound::CUnitRound()
{
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	
	state = rand() % 360;
	x = ( rand()%(xsize-40) ) + 20;
	y = ( rand()%(ysize-40) ) + 20;
	
	dx = rand()%4 - 2;
	dy = rand()%4 - 2;
	
	brush = GVI_CreateSolidBrush(0x80FF0000);
}

CUnitRound::~CUnitRound()
{
	GVI_Delete_GVI_Object(&brush);
}

void CUnitRound::Draw(GVI_GC gc)
{
	int r = sin( state * 5 )*10/256 + 20;
	
	GVI_FillEllipse( gc, x, y, r, r, brush );
}

void CUnitRound::DoStep()
{
	state = ( state + 1 ) % 360;
	
	x += dx;
	y += dy;
	
	if( x <= 0 || x >= xsize-10 )
		dx = -dx;
	
	if( y <= 0 || y >= ysize )
		dy = -dy;
}
