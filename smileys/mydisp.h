#ifndef _MYDISP_H_
#define _MYDISP_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

class CMyDisp:public CDispObjT<CMyDisp>
{
	int m_dx;
	int m_pos;

	u16 m_timer;

	GC* m_hMGC;
	GVI_GC m_smilgc;
	GC* m_hMGC2;
	GVI_GC m_drawgc;
	GVI_BRUSH m_br;
	GVI_GC m_gvigcdisp;

	int m_disph;
	int m_dispw;


public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);

	void onDestroy();
	int onCreate();

	static void onTimer(u16, CMyDisp* pthis);

	void ResetTimer();
};


#endif
