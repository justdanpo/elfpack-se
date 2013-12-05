#include "..\deleaker\mem2.h"

#include "mydisp.h"

#define TIMEINTERVAL (1000/25)
#define SMILECOUNT 800


char* CMyDisp::getName()
{
	return "MyDisp";
}

void CMyDisp::onRefresh()
{
	if(IsVisible())
	{
		for( int i = 0; i < NUMUNITS; i++ )
		{
			units[ i ]->DoStep();
		}


		if( ! isondraw )
			InvalidateRect( NULL );
	}
	SetRefreshTimer( TIMEINTERVAL );
}

void CMyDisp::onDestroy()
{
	for( int i = 0; i < NUMUNITS; i++ )
	{
		delete units[ i ];
	}
}

int CMyDisp::onCreate()
{
	isondraw = false;

	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);

	for( int i = 0; i < NUMUNITS; i++ )
	{
		units[ i ] = GetRandomUnit();
	}

	SetRefreshTimer( TIMEINTERVAL );

	return 1;
}

void CMyDisp::onDraw(int a,int b,int c)
{
	isondraw = true;

	GVI_GC displaygc;
	GVI_GC gc; //memory gc

	GC* _displaygc = get_DisplayGC();
	CANVAS_Get_GviGC( _displaygc->pcanvas, &displaygc );

	GC* _gc = GC_CreateMemoryGC( xsize, ysize, 16, 0, 0, 0 );
	CANVAS_Get_GviGC( _gc->pcanvas ,&gc );

	{
		GVI_BMP backbrush = GVI_CreateSolidBrush(0xFF000000);

		GVI_FillRectangle(gc,0,0,xsize,ysize,backbrush);


		for( int i = 0; i < NUMUNITS; i++ )
		{
			units[ i ]->Draw( gc );
		}

		GVI_Delete_GVI_Object(&backbrush);
	}

	GVI_BitBlt(displaygc, 0, 0, xsize, ysize, gc, 0, 0,    204, 0, 0, 0);

	GC_FreeGC( _gc );

	isondraw = false;
}
