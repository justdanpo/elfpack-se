#ifndef _IUNKNOWN_H_
#define _IUNKNOWN_H_

#ifndef WIN32
#include "..\..\include\lib_clara.h"
#else
#include "pctypes.h"
#endif

static UUID IID_IRoot={0x53, 0x54, 0x7A, 0x20, 0x01, 0x14, 0x47, 0x32, 0x82, 0xB7, 0xD2, 0xC3, 0x46, 0x49, 0xE4, 0x66};


#define E_NOTIMPL                       0x80004001/*microsoft*/
#define E_NOINTERFACE 0x80080800
#define E_FAIL                          0x80004005/*microsoft*/
#define CLASS_E_NOAGGREGATION           0x80040110/*microsoft*/
#define CLASS_E_CLASSNOTAVAILABLE       0x80040111/*microsoft*/
#define SELFREG_E_CLASS                 0x80040201/*microsoft*/
#define E_OUTOFMEMORY                   0x8007000E/*microsoft*/
#define S_OK                            0
#define S_FALSE                         1


class IUnknownVirtual
{
protected:
	virtual ~IUnknownVirtual(){}
public:
	virtual int QueryInterface(UUID* id, void** ret)=NULL;
	virtual int AddRef()=NULL;
	virtual int Release()=NULL;
};


class IUnknown:public IUnknownVirtual
{
protected:
	~IUnknown(){}
private:
	int count;
public:
	IUnknown() : count(1) {}
	virtual int QueryInterface(UUID* id, void** ret)
	{
		*ret = NULL;
		if( !memcmp(id,&IID_IRoot,sizeof(UUID)) )
		{
			AddRef();
			*ret = this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	virtual int AddRef()
	{
		return ++count;
	}
	virtual int Release()
	{
		int tmp=--count;
		if(!tmp) delete this;
		return tmp;
	}
};

#endif
