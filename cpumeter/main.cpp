#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "single.h"

#define _REFRESH_TIME 500

DISP_OBJ * StatusRow;
void (*SIonRedraw)(DISP_OBJ *,int r1,int r2,int r3);

u16 timer; // ID таймера
PROCESS pid;

int count=0;

int _max=128;
int _cur=0;

bool processpaused;
OS_PROCESS(my_proc)
{
	while(true)
	{
		count++;
		delay(1);
	}
}


void elf_exit(void)
{
	SUBPROC( mfree_adr(), &ELF_BEGIN );
}


void Draw(DISP_OBJ * db,int r1, int r2,int r3)
{
	SIonRedraw(db,r1,r2,r3);
	{
		int maxx=Display_GetWidth(0);
		int curx=(_max-_cur)*maxx/_max;

		int oColor=GetThemeColor(0, THEMEITEM_SCROLLBAR_BACKGROUND);
		int cColor=GetThemeColor(0, THEMEITEM_SCROLLBAR_SLIDER);

		DrawRect(0,0,maxx,2,oColor,oColor) ;
		DrawRect(0,0,curx,1,cColor,cColor) ;
	}
}

void onTimer(u16 r0, LPARAM)
{
	bool ss = FindBook(get_IsScreenSaverBook())!=NULL;
	if(processpaused!=ss)
	{
		if(ss)
			stop(pid);
		else
			start(pid);
		processpaused=ss;
		count=0;
	}

	if(!ss)
	{
		_cur=count;
		count=0;
		DispObject_InvalidateRect(StatusRow,0);
	}
	Timer_ReSet(&timer,_REFRESH_TIME,onTimer,0);
}


void aboutproc()
{
	MessageBox(STR("about elf"),STR("cpu meter\n(c) guess who =)"),NOIMAGE,1,5000,0);
}

void exitproc()
{
	DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusRow),SIonRedraw);
	kill_proc(pid);
	Timer_Kill(&timer);

	MessageBox(EMPTY_TEXTID,STR("elf stopped"),NOIMAGE,1,1000,0);
	elf_exit();
}

void startproc(wchar_t* filename)
{
	StatusRow=*StatusRow_p();
	SIonRedraw=DispObject_GetOnRedraw(StatusRow);
	DISP_DESC_SetOnRedraw(DispObject_GetDESC(StatusRow),Draw);
	timer=Timer_Set( _REFRESH_TIME, onTimer,0 );

	//pid=create_process(OS_BG_PROC,"CPUMeter",my_proc,1024,(OSPRIORITY)0 ,0,0,0,0,0);
	pid=create_process(OS_PRI_PROC,"cpumeter",my_proc,1024,(OSPRIORITY)31 ,0,0,0,0,0);
	start(pid);

	if(!wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)))
		MessageBox(EMPTY_TEXTID,STR("elf started"),NOIMAGE,1,1000,0);
}


int main(wchar_t* filename)
{
	static char myappname[]="cpu meter";
	TASKINSTANCE old=findinstance(myappname);
	if(old)
	{
		deleteinstance(old);
		elf_exit();
	}else
	{
		newinstance(myappname);
		startproc(filename);
		processpaused=false;
	}
	return 0;

}
