#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "gamedisp.hpp"

#define PCSTEPDELAY (1000/25)
#define TIMEINTERVAL (1000/25)

#define STATSFILE L"tttai.bin"

#include "sin.h"
int sin(int i)
{
	while(i<0)
		i+=360;
	while(i>359)
		i-=360;
	return sintable[i];
}
int cos(int i)
{
	return sin(i+90);
}

void rotate(const int ax, const int ay, int &x, int &y, int& z)
{
	int x1=x;
	int y1=y*cos(ax)/256+z*sin(ax)/256;
	int z1=z*cos(ax)/256-y*sin(ax)/256;

	x=x1*cos(ay)/256+z1*sin(ay)/256;
	y=y1;
	z=z1*cos(ay)/256-x1*sin(ay)/256;
}

void translatept(const POINT* frame, POINT& ret)
{
	int x11 = (frame[1].x-frame[0].x)*ret.x/256+frame[0].x;
	int x22 = (frame[2].x-frame[3].x)*ret.x/256+frame[3].x;

	int xn = (x22-x11)*ret.y/256 + x11;

	int y11 = (frame[1].y-frame[0].y)*ret.x/256+frame[0].y;
	int y22 = (frame[2].y-frame[3].y)*ret.x/256+frame[3].y;

	int yn = (y22-y11)*ret.y/256 + y11;

	ret.x = xn;
	ret.y = yn;
}

int getangle(int i)
{
	while(i>180)
		i-=360;

	while(i<-180)
		i+=360;

	return i;
}


char* CGameDisp::getName()
{
	return "GameDisp";
}

void CGameDisp::onKey(int key, int, int repeat, int type)
{
	if( type == KBD_SHORT_PRESS )
	{
		if( CGame::GAMEINPROGRESS != gameptr->GetGameState() )
		{
			if( key == KEY_DIGITAL_0+5 || key == KEY_ENTER )
			{
				Touch(4);
				StartGame();
			}
			if( key == KEY_LEFT_SOFT )
			{
				StartGame();
			}
		}

		if( key> KEY_DIGITAL_0+0 && key <= KEY_DIGITAL_0+9 )
		{
			Touch(key-KEY_DIGITAL_0-1);

			if( CGame::GAMEINPROGRESS == gameptr->GetGameState() )
			{
				if(step%2==1)
				{
					const char* field = gameptr->GetField();

					if(field[key-KEY_DIGITAL_0-1]==CGame::EMPTYFIELD)
					{
						gameptr->DoHumanStep( key-KEY_DIGITAL_0-1 );

						if(gameptr->GetGameState() != CGame::GAMEINPROGRESS )
							gameptr->WriteStats(STATSFILE);

						step++;
						steptimeout = PCSTEPDELAY;
					}
				}
			}
		}
		/*
		if(key==KEY_ESC)
		{
		CBookBase* book = GetGUI()->GetBook();
		if(book)
		{
		book->UI_Event(CANCEL_EVENT);
	}
	}
		*/
	}
}

void CGameDisp::onDestroy()
{
	delete gameptr;
}

int CGameDisp::onCreate()
{
	isondraw = false;
	gameptr = NULL;
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	for(int i=0;i<3; i++)
	{
		v[i]=0;
		y[i]=0;
	}

	SetRefreshTimer( TIMEINTERVAL );
	StartGame();

	return 1;
}

void CGameDisp::onRefresh()
{
	if(IsVisible())
	{

		if(step%2==0 && gameptr->GetGameState()==CGame::GAMEINPROGRESS)
		{
			if(steptimeout)
			{
				steptimeout--;
			}else
			{
				int st=gameptr->DoPCStep();

				if(gameptr->GetGameState() != CGame::GAMEINPROGRESS )
					gameptr->WriteStats(STATSFILE);

				Touch(st);
				step++;
			}
		}


		static int maxvalues[3]={150,170,170};

		for(int i=0;i<3;i++)
		{
			if(v[i]>maxvalues[i])
				v[i]=maxvalues[i];

			if(v[i]<-maxvalues[i])
				v[i]=-maxvalues[i];

			v[i] = (v[i]-y[i]/5)*98/100;

			if(y[i] && !v[i])
				v[i] = ( y[i] < 0 ) ? 1 : -1;

				y[i] += v[i];
		}
		for(int i=1;i<3;i++)
		{
			y[i] = getangle( y[i] );
		}

		if(! isondraw)
			InvalidateRect( NULL );

	}

	SetRefreshTimer( TIMEINTERVAL );
}


void drawfig(GVI_GC gc, POINT* frame, int x, int y, int type, int color)
{
	if(!type)
		return;

	POINT newframe[4];
	newframe[0].x = 256*x/3;
	newframe[0].y = 256*y/3;

	newframe[1].x = 256*(x+1)/3;
	newframe[1].y = 256*y/3;

	newframe[2].x = 256*(x+1)/3;
	newframe[2].y = 256*(y+1)/3;

	newframe[3].x = 256*(x)/3;
	newframe[3].y = 256*(y+1)/3;

	for(int i=0;i<4;i++)
		translatept(frame, newframe[i]);

	switch(type)
	{
	case CGame::XVALUE:
		{
			GVI_PEN pen = GVI_CreatePen(3, 0xFF000000 | (color * 0x10000) );

			POINT xp[4];
			xp[0].x=256/5;
			xp[0].y=256/5;

			xp[1].x=256*4/5;
			xp[1].y=256*4/5;

			xp[2].x=xp[0].x;
			xp[2].y=xp[1].y;

			xp[3].x=xp[1].x;
			xp[3].y=xp[0].y;

			for(int i=0;i<4;i++)
				translatept(newframe, xp[i]);

			GVI_DrawLine(gc, xp[0].x, xp[0].y, xp[1].x, xp[1].y, pen);
			GVI_DrawLine(gc, xp[2].x, xp[2].y, xp[3].x, xp[3].y, pen);

			GVI_Delete_GVI_Object(&pen);
		}
		break;
	case CGame::OVALUE:
		{
			GVI_PEN pen = GVI_CreatePen(3, 0xFF000000 | (color * 0x1) );

#define OPOINTSCOUNT 10

			POINT xp[OPOINTSCOUNT];

			for(int i = 0; i < OPOINTSCOUNT; i++)
			{
				xp[i].x = 128+128*4/5* cos(i*360/OPOINTSCOUNT)/256;
				xp[i].y = 128-128*4/5* sin(i*360/OPOINTSCOUNT)/256;

				translatept(newframe, xp[i]);
			}

			for(int i = 0; i < OPOINTSCOUNT-1; i++)
				GVI_DrawLine(gc, xp[i].x, xp[i].y, xp[i+1].x, xp[i+1].y, pen);

			GVI_DrawLine(gc, xp[OPOINTSCOUNT-1].x, xp[OPOINTSCOUNT-1].y, xp[0].x, xp[0].y, pen);

			GVI_Delete_GVI_Object(&pen);
		}
		break;
	}
}


//#pragma swi_number=0x81ED
//__swi __arm int IsDrawString();

void CGameDisp::onDraw(int a, int b, int c)
{
	isondraw = true;

	GC* _displaygc = get_DisplayGC();
	GVI_GC displaygc;
	CANVAS_Get_GviGC( _displaygc->pcanvas, &displaygc );

	GC* _gc = GC_CreateMemoryGC(xsize, ysize, 16, 0, 0, 0);
	GVI_GC gc;
	CANVAS_Get_GviGC( _gc->pcanvas ,&gc );


	GVI_BRUSH backbrush = GVI_CreateSolidBrush(0xFFFFFFFF);

	int boardcolor = ( cos(y[1]) + cos(y[2]) )/2;

	GVI_BRUSH boardbrush = GVI_CreateSolidBrush(0xFF000000 | ( boardcolor * 0x10101) );
	//GVI_BRUSH boardbrush = GVI_CreateSolidBrush(0xFFC0C0C0);

	GVI_PEN blackpen = GVI_CreatePen(1, 0xFF000000);

	GVI_FillRectangle(gc,0,0,xsize,ysize,backbrush);

	int ax=y[1];
	int ay=y[2];

	POINT p[4];
	int z[4];
	for(int i=0;i<4;i++)
		z[i]=y[0];

	int xscrmax=256;
	int xmax=200;

	p[0].x=-xmax;
	p[0].y=-xmax;
	p[1].x=xmax;
	p[1].y=-xmax;
	p[2].x=xmax;
	p[2].y=xmax;
	p[3].x=-xmax;
	p[3].y=xmax;

	for(int i=0;i<4;i++)
	{
		int x1=p[i].x;
		int y1=p[i].y;
		int z1=z[i];
		rotate(ax,ay,x1,y1,z1);

		int x2=p[i].x;
		int y2=p[i].y;
		int z2=z[i];
		rotate(ay,ax,y2,x2,z2);

		p[i].x = (x1+x2)/2;
		p[i].y = (y1+y2)/2;
		z[i]= (z1+z2)/2;
	}

	for(int i=0;i<4;i++)
	{
		p[i].x = (p[i].x + p[i].x* z[i]/4/xmax + xscrmax)*xsize/2/xscrmax;
		p[i].y = (p[i].y + p[i].y* z[i]/4/xmax + xscrmax)*xsize/2/xscrmax;
	}

	GVI_FillPolygon(gc,p,4,boardbrush);
	GVI_DrawPolyLine(gc,p,4,blackpen,1);

	int ax2=ax+90; while(ax2>180)ax2-=360; while(ax2<-180)ax2+=360;
	int ay2=ay+90; while(ay2>180)ay2-=360; while(ay2<-180)ay2+=360;

	if(ax2*ay2>0)
	{
		for(int i=1;i<=2;i++)
		{
			POINT pt[4];
			pt[0].x = 256*i/3;
			pt[0].y = 0;

			pt[1].x = 256*i/3;
			pt[1].y = 256;

			pt[2].x = 0;
			pt[2].y = 256*i/3;

			pt[3].x = 256;
			pt[3].y = 256*i/3;
			translatept(p,pt[0]);
			translatept(p,pt[1]);
			translatept(p,pt[2]);
			translatept(p,pt[3]);

			GVI_DrawLine(gc, pt[0].x, pt[0].y, pt[1].x, pt[1].y, blackpen);
			GVI_DrawLine(gc, pt[2].x, pt[2].y, pt[3].x, pt[3].y, blackpen);
		}

		const char* field = gameptr->GetField();

		int colors[3];
		colors[CGame::XVALUE]=boardcolor;
		colors[CGame::OVALUE]=boardcolor;

		switch(gameptr->GetGameState())
		{
		case CGame::HUMANWIN:
			colors[CGame::XVALUE]/=2;
			break;
		case CGame::PCWIN:
			colors[CGame::OVALUE]/=2;
			break;
		case CGame::TIE:
			colors[CGame::XVALUE]/=2;
			colors[CGame::OVALUE]/=2;
		}

		for(int y=0; y<SIDESIZE; y++)
			for(int x=0; x<SIDESIZE; x++)
				drawfig(gc, p, x, y, field[y*SIDESIZE + x], colors[field[y*SIDESIZE + x]]);

	}

	GVI_BitBlt(displaygc, 0, 0, xsize, ysize, gc, 0, 0,    204, 0, 0, 0);

	GVI_Delete_GVI_Object(&blackpen);
	GVI_Delete_GVI_Object(&boardbrush);
	GVI_Delete_GVI_Object(&backbrush);

	GC_FreeGC( _gc );

	if(gameptr->GetGameState() != CGame::GAMEINPROGRESS)
		{
			int font=FONT_E_16R;
			if(font==0xFFFFFFFF)
				font=FONT_E_20R;

			int font_old = SetFont( font );

			wchar_t tmp[16];

			int ysize = GetWindowHeight();

			TEXTID strid = TextID_Create(tmp, ENC_UCS2, snwprintf( tmp, MAXELEMS(tmp), L"%d", gameptr->GetHumanScore() ) );
			DrawString(strid, 0, xsize/10, ysize*9/10, xsize*9/10, ysize, 0, 0, 0xFF0000FF, 0xFF0000FF);
			TextID_Destroy(strid);

			strid = TextID_Create(tmp, ENC_UCS2, snwprintf( tmp, MAXELEMS(tmp), L"%d", gameptr->GetPCScore() ) );
			DrawString(strid, 1, xsize/10, ysize*9/10, xsize*9/10, ysize, 0, 0, 0xFFFF0000, 0xFFFF0000);
			TextID_Destroy(strid);

			strid = TextID_Create(tmp, ENC_UCS2, snwprintf( tmp, MAXELEMS(tmp), L"%d", gameptr->GetTies() ) );
			DrawString(strid, 2, xsize/10, ysize*9/10, xsize*9/10, ysize, 0, 0, 0xFF000000, 0xFF000000);
			TextID_Destroy(strid);

			SetFont( font_old );
		}

	isondraw = false;
}

void CGameDisp::Touch(int n)
{
#define ALPHA 17
	int dx[9]={ -ALPHA,      0,  ALPHA,  -ALPHA,   0,  ALPHA,  -ALPHA,      0,  ALPHA };
	int dy[9]={ -ALPHA, -ALPHA, -ALPHA,       0,   0,      0,   ALPHA,  ALPHA,  ALPHA };
	int dz[9]={ -40, -60, -40,  -60, -80, -60,  -40, -60, -40 };

	v[0]+=dz[n]/2;

	v[2]+=dx[n];
	v[1]+=dy[n];

	/*
	PAudioControl pAC = AudioControl_Init();
	if( !pAC )
	pAC = *GetAudioControlPtr();
	AudioControl_Vibrate(pAC,200,0,200);
	*/
}

void CGameDisp::StartGame(void)
{
	if(gameptr)
		delete gameptr;

	gameptr = new CGame();

	gameptr->ReadStats(STATSFILE);

	step = 0;
	steptimeout = PCSTEPDELAY;
}
