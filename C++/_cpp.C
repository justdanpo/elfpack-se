#include "..\inc\swilib.h"


//pure virtual function may show error
void __pure_virtual_called(void)
{
  LockSched();
  ShowMSG(2,(int)"ABSTRACT VIRTUAL FUNCTION CALLED!");
  UnlockSched();
};

typedef void (*tf)(void *);

//vec=new TObject[1024];
void * __data_vec_new(int u1 ,int count ,int size,tf constructor)
{
char * p,*q;
p=(char*)malloc(size*count+4);
*((int*)p)=count;
p+=4;
q=p;
for(int i=0;i<count;i++)
  {
  constructor(q);  
  q+=size;  
  };
return p;
};

//;vec=new TObject[1024];
//delete vec
void __data_vec_delete(char * vector,int u1,int size,tf destructor)
{
int count=*((int*)(vector-4));
char * s=vector-4;
for(int i=0;i<count;i++)
  {
  destructor(vector);  
  vector+=size;  
  };
mfree(s);
};
