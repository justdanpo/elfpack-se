//uncomment the following line to use a PROCESS instead of a book
//#define USE_1INST_PROCESS


#ifndef USE_1INST_PROCESS
typedef BOOK* TASKINSTANCE;
#else
typedef PROCESS TASKINSTANCE;
#endif

TASKINSTANCE newinstance(char* name);
TASKINSTANCE findinstance(char* name);
void deleteinstance(TASKINSTANCE pid);
