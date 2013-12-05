#ifndef _MAIN_H_
  #define _MAIN_H_

#define IMSI_DATA_BYTE_LEN  (9)

void StartHoursTimer(void);
void SendCashReq(void);
extern "C" long strtol(const char *, char **, int);


class CDbgPrint
{
  char* fl;
  char* fn;
public:
  CDbgPrint(char* file,char* func){debug_printf("\nfunction enter: %s/%s\n",fl=file,fn=func);}
  ~CDbgPrint(){debug_printf("\nfunction leave: %s/%s\n",fl,fn);}
};
#ifdef NDEBUG
#define FUNCTION
#else
#define FUNCTION CDbgPrint __function((char*)__FILE__,(char*)__FUNCTION__);
#endif


#endif
