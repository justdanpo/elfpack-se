#include "..\\include\Lib_Clara.h"
#include "single.h"

extern void exitproc();
extern void aboutproc();


#ifndef USE_1INST_PROCESS
//--1st method----------------------------------------------------------------

void bookOnDestroy(BOOK * book)
{
	exitproc();
}

int onExit(void* r0,BOOK* b)
{
	FreeBook(b);
	return 1;
}
int onAbout(void* r0,BOOK* b)
{
	aboutproc();
	return 1;
}
const PAGE_MSG evtlist[] @ "DYN_PAGE"=
{
	ELF_TERMINATE_EVENT     ,onExit,
	ELF_SHOW_INFO_EVENT     ,onAbout,
	NIL_EVENT_TAG,  NULL
};

const PAGE_DESC defaultpage = {"defaultpage",0,evtlist};

TASKINSTANCE newinstance(char* name)
{
	BOOK *myBook=(BOOK*)malloc(sizeof(BOOK));
	memset(myBook,0,sizeof(BOOK));
	CreateBook(myBook,bookOnDestroy,&defaultpage,name,-1,0);
	return myBook;
}

int cmp_proc(BOOK* book,int* param)
{
	if(!strcmp(book->xbook->name, (char*)param)) return(1);
	return(0);
}

TASKINSTANCE findinstance(char* name)
{
	return FindBookEx(cmp_proc, (int*)name);
}

void deleteinstance(TASKINSTANCE pid)
{
	FreeBook(pid);
}




#else
//--2nd method----------------------------------------------------------------


enum { MYPROC_EXIT=1 };

union SIGNAL
{
	SIGSELECT signo;
};

void killprocess(int pid)
{
	kill_proc(pid);
	exitproc();
}

OS_PROCESS(my_proc)
{
	static const SIGSELECT sg[]={1, MYPROC_EXIT};
	union SIGNAL *sig;

	bool stopped=false;
	for(;;)
	{
		sig=receive(sg);
		if(sig)
		{
			if(!stopped)
				switch(sig->signo)
				{
				case MYPROC_EXIT:
					stopped=true;
					MMIPROC(killprocess,current_process());
					break;
				}
			free_buf(&sig);
		}
	}
}

TASKINSTANCE newinstance(char* name)
{
	PROCESS pid=create_process(OS_PRI_PROC,name,my_proc,512,(OSPRIORITY)0 ,0,0,0,0,0);
	start(pid);
	return pid;
}

TASKINSTANCE findinstance(char* name)
{
	PROCESS pid;
	if(hunt(name,NULL,&pid,NULL))return pid;
	return NULL;
}

void deleteinstance(TASKINSTANCE pid)
{
	union SIGNAL* s=alloc(sizeof(SIGNAL), MYPROC_EXIT);
	send(&s,pid);
}

#endif
