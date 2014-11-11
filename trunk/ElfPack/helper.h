#ifndef _HELPER_H_
#define _HELPER_H_

void StartHelper(void);
void MMIPROCExec (void(*PROC)(int,void*),int p1 , void * p2);

struct HELPER_SIGNAL
{
	SIGSELECT signo;
	void (*PROC)(int, void*);
	int param1;
	void* param2;
};

#endif
