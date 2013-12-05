#ifndef _MYDISP_H_
#define _MYDISP_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "cunits.hpp"

#define NUMUNITS 20

class CMyDisp:public CDispObjT<CMyDisp>
{
protected:
	u16 m_timer;
	
	int ysize;
	int xsize;
	BOOL isondraw;
	
	CUnit* units[NUMUNITS];
	
public:
	static char* getName();
	void onDraw(int a,int b,int c);
	
	void onDestroy();
	int onCreate();
	void onRefresh();
};


#endif
