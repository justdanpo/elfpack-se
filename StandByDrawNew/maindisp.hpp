#include "..\classlib\cl.h"

class CMainDisp:public CDispObjT<CMainDisp>
{
public:
	static char* getName();
	void onDraw(int a, int b, int c);
	void onLayout( void* layoutstruct );
};
