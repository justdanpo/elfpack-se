#include "mydisp.hpp"

char* CLogDisp::getName()
{
	return "CLogDisp";
}

void CLogDisp::_OnTimer(u16 timer, CLogDisp* disp)
{
	disp->GetGUI()->GetBook()->UI_Event( ACCEPT_EVENT );
}

void CLogDisp::onDraw(int a,int b,int c)
{
	GC* _displaygc = get_DisplayGC();
	GVI_GC displaygc;
	CANVAS_Get_GviGC( _displaygc->pcanvas, &displaygc );

	int font=FONT_E_12B;
	if(font==0xFFFFFFFF)
		font=FONT_E_16B;

	int font_old = SetFont( font );

	int xsize = GetWindowWidth();
	int ysize = GetWindowHeight();

	GVI_BRUSH backbrush = GVI_CreateSolidBrush( 0xFF000000 );
	GVI_FillRectangle( displaygc, 0, 0, xsize, ysize, backbrush );
	GVI_Delete_GVI_Object( &backbrush );

	DrawString(strid, 0,  0, 1, xsize, ysize, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF);

	SetFont( font_old );
}

void CLogDisp::onDestroy()
{
	if( timerid )
		Timer_Kill( &timerid );

	if( strid != EMPTY_TEXTID )
		TextID_Destroy( strid );
}

int CLogDisp::onCreate()
{
	strid = EMPTY_TEXTID;

	return 1;
}

void CLogDisp::UpdateText( TEXTID newstrid )
{
	if( strid != EMPTY_TEXTID )
		TextID_Destroy( strid );
	strid = newstrid;
	InvalidateRect( 0 );

	if( !timerid )
		timerid = TIMER_SET( 3000, _OnTimer, this );
	else
		TIMER_RESET( &timerid, 3000, _OnTimer, this );
}
