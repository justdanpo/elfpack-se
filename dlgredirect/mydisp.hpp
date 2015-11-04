#ifndef _LOGDISP_H_
#define _LOGDISP_H_

#include "..\classlib\cl.h"

class CLogDisp:public CDispObjT<CLogDisp>
{
protected:
	TEXTID strid;
	u16 timerid;

	static void _OnTimer(u16 timer, CLogDisp* disp);
public:
	static char* getName();
	void onDraw(int a,int b,int c);

	void onDestroy();
	int onCreate();

	void UpdateText( TEXTID );
};

#endif
