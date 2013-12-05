#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "cunitstars.hpp"
#include "rand.h"
#include "sin.h"

CUnitStars::CUnitStars()
{
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	
	state = rand() % 360;
	x = ( rand()%(xsize-40) ) + 20;
	y = ( rand()%(ysize-40) ) + 20;
	
	dx = rand()%4 - 2;
	dy = rand()%4 - 2;
	
	pen = GVI_CreatePen(1, 0xFF0000FF);
}

CUnitStars::~CUnitStars()
{
	GVI_Delete_GVI_Object(&pen);
}

void CUnitStars::Draw(GVI_GC gc)
{
	int rmax = sin( state * 3 )*5/256 + 10;
	for( int i = 0; i < 10; i++ )
	{
		int a = rand() % 360;
		int r = rand() % rmax;
		int xx = r * cos( a ) / 256 + x;
		int yy = r * sin( a ) / 256 + y;
		
		GVI_DrawLine( gc, xx, yy, xx, yy, pen );
	}
}

void CUnitStars::DoStep()
{
	state = ( state + 1 ) % 360;
	
	x += dx;
	y += dy;
	
	if( x <= 0 || x >= xsize-10 )
		dx = -dx;
	
	if( y <= 0 || y >= ysize )
		dy = -dy;
}
