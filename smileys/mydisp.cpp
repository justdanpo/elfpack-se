#include "..\deleaker\mem2.h"

#include "mydisp.h"

#define TIMEINTERVAL (1000/25)
#define SMILECOUNT 800


char* CMyDisp::getName()
{
	return "MyDisp";
}

void CMyDisp::onKey(int key, int, int repeat, int type)
{
	CBookBase* book = GetGUI()->GetBook();

	if(key==KEY_ESC)
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

	if(type==KBD_SHORT_PRESS)
	{
		if(key==KEY_LEFT)
			m_dx--;
		if(key==KEY_RIGHT)
			m_dx++;
	}
}

void CMyDisp::onTimer(u16, CMyDisp* pthis)
{
	pthis->InvalidateRect( NULL );

	TIMER_RESET(&pthis->m_timer, TIMEINTERVAL, onTimer, pthis);
}

void CMyDisp::onDestroy()
{
	if(m_smilgc!=NULL)
	{
		Timer_Kill( &m_timer );
		GVI_Delete_GVI_Object( &m_br );
		GC_FreeGC( m_hMGC );
		GC_FreeGC( m_hMGC2 );
	}
}

int CMyDisp::onCreate()
{
	m_smilgc = NULL;
	m_dx=1;
	m_pos=0;

	m_dispw = Display_GetWidth(0);
	m_disph = Display_GetHeight(0);

	m_timer=TIMER_SET( TIMEINTERVAL, onTimer, this );

	return 1;
}

//include precalculated sin/cos table
#include "xy.h"

void CMyDisp::onDraw(int a,int b,int c)
{
	//first call initialization
	if(m_smilgc==NULL)
	{
		m_br=GVI_CreateSolidBrush(0xFFFFFFFF);

		m_hMGC = GC_CreateMemoryGC( 20, 20, 16, 0,0,0 );
		CANVAS_Get_GviGC( m_hMGC->pcanvas ,&m_smilgc);

		GVI_FillRectangle(m_smilgc, 0,0, 20,20, m_br);

		int IDsmile;
		iconidname2id(L"SMI_VERY_HAPPY_ICN", -1, &IDsmile);
		GC_PutChar(m_hMGC,0,0,0,0, IDsmile);

		m_hMGC2 = GC_CreateMemoryGC(m_dispw, m_disph, 16, 0, 0, 0);
		CANVAS_Get_GviGC( m_hMGC2->pcanvas ,&m_drawgc );


	}

	GC* gc = get_DisplayGC();
	CANVAS_Get_GviGC( gc->pcanvas, &m_gvigcdisp );

	GVI_FillRectangle( m_drawgc, 0, 0, m_dispw, m_disph, m_br );

	//draw
	for(int i=0; i<SMILECOUNT; i++)
	{
		GVI_BitBlt(m_drawgc,
				   m_dispw/2 + (xy[ ((m_pos+i*10)&255)*2  ]*i)/256/(SMILECOUNT/100), //x calculated from a table
				   m_disph/2 + (xy[ ((m_pos+i*10)&255)*2+1]*i)/256/(SMILECOUNT/100), //y calculated from a table
				   20,20,
				   m_smilgc, 0,0,
				   204,0,0,0);
	}

	GVI_BitBlt(m_gvigcdisp, 0, 0, m_dispw, m_disph, m_drawgc, 0, 0,    204, 0, 0, 0);

	m_pos += m_dx;
}
