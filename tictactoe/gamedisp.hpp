#ifndef _GAMEDISP_H_
#define _GAMEDISP_H_

#include "..\classlib\cl.h"
#include "game.h"

class CGameDisp:public CDispObjT<CGameDisp>
{
	int xsize;
	int ysize;

	int v[3];
	int y[3];

	CGame* gameptr;
	int step;
	int steptimeout;

	BOOL isondraw;

public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
	void onRefresh();

	void onDestroy();
	int onCreate();

	void Touch(int n);
	void StartGame(void);
};

#endif
