#include "..\classlib\cl.h"
#include "Accelerometer\export\AccelerometerDll.h"

class CMainDisp:public CDispObjT<CMainDisp>
{
	int* ptype;
	
	int xsize;
	int ysize;
	
	int xsize3;
	int ysize3;
	int xgold;
	int ygold;
	
	DISP_OBJ_ONKEY_METHOD oldOnKey;
	ACCELEROMETER_DLL_DATA * acc_dll;
	CALIBRATION_DATA acc_cal;
	CGuiBase* maingui;
	
public:
	static char* getName();
	void onDraw(int a, int b, int c);
	void onLayout( void* layoutstruct );
	int onCreate();
	void onDestroy();
	void onRefresh();
	static void HookedOnKey(CMainDisp*, int scan_code, int unk, int repeat_num, int key_mode);
	
	void Setup(CGuiBase* cameragui, ACCELEROMETER_DLL_DATA * acc_dll, int* _ptype);
};
