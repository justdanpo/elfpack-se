#ifndef _MYDISP_H_
#define _MYDISP_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

class CMyDisp:public CDispObjT<CMyDisp>
{
public:
	static char* getName();
	void onDraw(int a,int b,int c);
	void onKey(int key,int,int repeat,int type);
};


#endif
