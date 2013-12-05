#include "maindisp.hpp"

char* CMainDisp::getName()
{
	return "StandByDrawNew_DISP";
}

void CMainDisp::onDraw(int a, int b, int c)
{
	TEXTID strid = STR("test");
	DrawString(strid, 0,
			   20,25,100,40,
			   0, 0, 0xFF0000FF, 0xFF0000FF);
	TextID_Destroy(strid);
}

void CMainDisp::onLayout( void* layoutstruct )
{
	//координаты
	WindowSetPosition( 50 , 50 );
	WindowSetSize( 60, 60 );

	//прозрачный фон
	SetLayerColor( 0x80000000 );
}
