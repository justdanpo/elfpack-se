#include "mydisp.h"

char* CMyDisp::getName()
{
	return "MyDisp";
}

void CMyDisp::onKey(int key,int,int repeat,int type)
{
	CBookBase* book= GetGUI()->GetBook();

	if(key==KEY_ESC || key==KEY_DEL)
	{
		if(book)
		{
			book->UI_Event(CANCEL_EVENT);
		}
	}

	if(key==KEY_LEFT_SOFT)
	{
		if(book)
		{
			book->UI_Event(ACCEPT_EVENT);
		}
	}
}

void CMyDisp::onDraw(int a,int b,int c)
{
	CPaintGC gc;

	gc.DrawFRect(gc.GetPenColor(),0,0,60,60) ;
}
