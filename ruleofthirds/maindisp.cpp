#include "maindisp.hpp"
#include "globals.h"
#include "math.h"
#include "config_data.h"

#define TIMEINTERVAL (1000/10)


int GetCameraMode()
{
	//убрать для конкретной прошивки
	return 1;
	
	char* ptr=(char*)FindBook(get_IsCameraBook());
	if(!ptr)
		return 0;
	
	//W995_R1HA035
	//char* pshotmode=ptr+0x1EC;//1-photo, 2-video
	
	//C702_R3EF001
	//char* pshotmode=ptr+0x260;//1-photo, 2-video
	
	
	if(!pshotmode)
		return 0;
	
	return *pshotmode;
}


char* CMainDisp::getName()
{
	return MYDISPNAME;
}

void CMainDisp::onRefresh()
{
	InvalidateRect(NULL);
	SetRefreshTimer( TIMEINTERVAL );
}

void CMainDisp::onDestroy()
{
	//todo: проверить, существует ли DspCamera?
	DISP_DESC_SetOnKey( maingui->GetDISPObj()->GetDESC(), oldOnKey );
	if(acc_dll)
		ACCELEROMETER_Close(acc_dll);
}

void CMainDisp::Setup(CGuiBase* cameragui, ACCELEROMETER_DLL_DATA * _acc_dll, int* _ptype)
{
	ptype = _ptype;
	maingui = cameragui;
	
	acc_dll = _acc_dll;
	if(acc_dll)
	{
		ACCELEROMETER_Open(acc_dll);
		ACCELEROMETER_GetGlobalCal(acc_dll, &acc_cal);
		SetRefreshTimer( TIMEINTERVAL );	
	}
	
	oldOnKey = DispObject_GetOnKey( cameragui->GetDISPObj() );
	DISP_DESC_SetOnKey( cameragui->GetDISPObj()->GetDESC(), (DISP_OBJ_ONKEY_METHOD)HookedOnKey );
}


void CMainDisp::HookedOnKey(CMainDisp* thisobj, int scan_code, int unk, int repeat_num, int key_mode)
{
	CBookBase* book = thisobj->GetGUI()->GetBook();
	size_t guicnt = book->xguilist->guilist->FirstFree;
	
	for(size_t curgui = 0; curgui< guicnt; ++curgui)
	{
		CGuiBase* gui = reinterpret_cast<CGuiBase*>( List_Get(book->xguilist->guilist, curgui) );
		CMainDisp* disp = static_cast<CMainDisp*>( gui->GetDISPObj() );
		if(!strcmp(disp->GetDESC()->name, MYDISPNAME))
		{
			if(GetCameraMode()==1)
			{
				if( (key_mode==KeyFrameMode) && (scan_code==KeyFrame) )
				{
					*disp->ptype^=1;
					disp->InvalidateRect(NULL);
				}
				
				if( (key_mode==KeyHorizontMode) && (scan_code==KeyHorizont) )
				{
					*disp->ptype^=2;						
					disp->InvalidateRect(NULL);
				}
			}
			disp->oldOnKey(thisobj, scan_code, unk, repeat_num, key_mode);
			return;
		}
	}
}

int CMainDisp::onCreate()
{
	xsize = Display_GetWidth(0);
	ysize = Display_GetHeight(0);
	
	xsize3 = xsize / 3;
	ysize3 = ysize / 3;
	
	xgold = xsize * 38 / 100;
	ygold = ysize * 38 / 100;
	
	return 1;
}


void CMainDisp::onDraw(int a, int b, int c)
{
	GetCameraMode();	
	
	if(GetCameraMode()==1)
	{
		GC* _displaygc = get_DisplayGC();
		GVI_GC displaygc;
		CANVAS_Get_GviGC( _displaygc->pcanvas, &displaygc );
		
		//	GVI_PEN pen = GVI_CreatePen(1, 0xFF000000 | (0xFF * 0x10000) );
		
		GVI_PEN blackpen = GVI_CreateDashedPen( 1, clBlack, 1, 5, 0 );
		
		if(*ptype & 1)
		{
			//правило третей	
			GVI_PEN pen = GVI_CreateDashedPen( 1, clRed, 1, 5, 0 );
			
			GVI_DrawLine(displaygc, 0, ysize3, xsize, ysize3, pen);
			GVI_DrawLine(displaygc, 0, ysize-ysize3, xsize, ysize-ysize3, pen);
			
			GVI_DrawLine(displaygc, xsize3, 0, xsize3, ysize, pen);
			GVI_DrawLine(displaygc, xsize-xsize3, 0, xsize-xsize3, ysize, pen);
			
			GVI_DrawLine(displaygc, 0, ysize3+1, xsize, ysize3+1, blackpen);
			GVI_DrawLine(displaygc, 0, ysize-ysize3+1, xsize, ysize-ysize3+1, blackpen);
			
			GVI_DrawLine(displaygc, xsize3+1, 0, xsize3+1, ysize, blackpen);
			GVI_DrawLine(displaygc, xsize-xsize3+1, 0, xsize-xsize3+1, ysize, blackpen);
			
			GVI_Delete_GVI_Object(&pen);
			
			//золотое сечени е
			pen = GVI_CreateDashedPen( 1, clGreen, 1, 5, 0 );
			
			GVI_DrawLine(displaygc, 0, ygold, xsize, ygold, pen);
			GVI_DrawLine(displaygc, 0, ysize-ygold, xsize, ysize-ygold, pen);
			
			GVI_DrawLine(displaygc, xgold, 0, xgold, ysize, pen);
			GVI_DrawLine(displaygc, xsize-xgold, 0, xsize-xgold, ysize, pen);
			
			GVI_DrawLine(displaygc, 0, ygold+1, xsize, ygold+1, blackpen);
			GVI_DrawLine(displaygc, 0, ysize-ygold+1, xsize, ysize-ygold+1, blackpen);
			
			GVI_DrawLine(displaygc, xgold+1, 0, xgold+1, ysize, blackpen);
			GVI_DrawLine(displaygc, xsize-xgold+1, 0, xsize-xgold+1, ysize, blackpen);
			
			GVI_Delete_GVI_Object(&pen);
		}
		
		
		if(*ptype & 2)
			if(acc_dll) {
				int  x, y, z;
				if(ACCELEROMETER_GetCoords(acc_dll, &x, &y, &z, 1) >= 0)
				{
					//calibration
					//y = (y-acc_cal.zero[1])*1024/(acc_cal.g[1]-acc_cal.zero[1]);
					
					if( (y>-700) && (y<700) )
					{
						int yd = 0.5*xsize*tan(
											   //3.14159/2*
											   1.055 *  y/1024.0
												   );
						
						GVI_PEN pen;
						
						if( (y>-60) && (y<60) )
							pen = GVI_CreateDashedPen( 1, clGreen, 7, 5, 0 );
						else
							pen = GVI_CreateDashedPen( 1, clRed, 7, 5, 0 );
						
						GVI_DrawLine(displaygc, 0, ysize/2+yd, xsize, ysize/2-yd, pen);
						
						GVI_Delete_GVI_Object(&pen);
						
					}
					
					/*
					//debug accelerometer
					{
					TEXTID a[6];
					a[0] = TextID_Create("X: ", ENC_LAT1, TEXTID_ANY_LEN);
					a[1] = TextID_CreateIntegerID(x);
					a[2] = TextID_Create("\nY: ", ENC_LAT1, TEXTID_ANY_LEN);
					a[3] = TextID_CreateIntegerID(y);
					a[4] = TextID_Create("\nZ: ", ENC_LAT1, TEXTID_ANY_LEN);
					a[5] = TextID_CreateIntegerID(z);
					
					TEXTID id = TextID_Create(a, ENC_TEXTID, 6);
					
					DrawString(id, 0,
					0,0,200,200,
					0, 0, 0xFF0000FF, 0xFF0000FF);
					TextID_Destroy(id);
				}
					*/
				}
			}
		
		
		GVI_Delete_GVI_Object(&blackpen);
		
	}
	
}

void CMainDisp::onLayout( void* layoutstruct )
{	
	//координаты
	WindowSetPosition( 0 , 0 );
	WindowSetSize( xsize, ysize );
	
	//прозрачный фон
	SetLayerColor( 0x00000000 );
}
