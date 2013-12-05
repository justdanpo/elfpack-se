#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "cunitrect.hpp"
#include "rand.h"
#include "sin.h"

CUnitRect::CUnitRect()
{
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	
	state = rand() % 360;
	x = ( rand()%(xsize-40) ) + 20;
	y = ( rand()%(ysize-40) ) + 20;
	
	dx = rand()%4 - 2;
	dy = rand()%4 - 2;
	
	brush = GVI_CreateSolidBrush(0x8000FF00);
}

CUnitRect::~CUnitRect()
{
	GVI_Delete_GVI_Object(&brush);
}

void CUnitRect::Draw(GVI_GC gc)
{
	POINT p[4];
	
	int r = sin( state * 3 )*10/256 + 20;
	for( int i = 0; i < 4; i++ )
	{
		p[i].x = r;
		p[i].y = 0;
		rotate( i*90 + state, p[i].x, p[i].y );
		p[i].x += x;
		p[i].y += y;
	}
	GVI_FillPolygon( gc, p, 4, brush );
}

void CUnitRect::DoStep()
{
	state = ( state + 1 ) % 360;
	
	x += dx;
	y += dy;
	
	if( x <= 0 || x >= xsize-10 )
		dx = -dx;
	
	if( y <= 0 || y >= ysize )
		dy = -dy;
}
