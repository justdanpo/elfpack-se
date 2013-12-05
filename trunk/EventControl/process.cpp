#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\lng.h"
#include "header\process.h"

extern PROCESS proc_;

#define WORKER_SIG 0x7FF0
const SIGSELECT sg[]={1,WORKER_SIG};

struct WORKER_SIGNAL
{
  SIGSELECT signo;
  void(*PROC)(int,void*);
  int   param1;
  void * param2;
};

union SIGNAL
{
  SIGSELECT signo;
  struct WORKER_SIGNAL hsig;
};

OS_PROCESS(worker_entrypoint)
{
  union SIGNAL *sig;
  for(;;)
  {
    sig=receive(sg);
    switch(sig->signo)
    {
    case WORKER_SIG:
      {
	if (sig->hsig.PROC) sig->hsig.PROC(sig->hsig.param1,sig->hsig.param2);
      }
      break;
    }
    free_buf(&sig);
  }
};

void SUBPROC_(void(*PROC)(int,void*),int p1 , void * p2)
{
  union SIGNAL *sig;

  sig = alloc(sizeof(union SIGNAL),WORKER_SIG);
  sig->hsig.PROC=PROC;
  sig->hsig.param1=p1;
  sig->hsig.param2=p2;
  send(&sig,proc_);
};

void SUBPROC_(void(*PROC)(EVENT *),EVENT *p1, void * p2)
{
  union SIGNAL *sig;

  sig = alloc(sizeof(union SIGNAL),WORKER_SIG);
  sig->hsig.PROC=(void (*)(int,void*))PROC;
  sig->hsig.param1=(int)p1;
  sig->hsig.param2=p2;
  send(&sig,proc_);
};
