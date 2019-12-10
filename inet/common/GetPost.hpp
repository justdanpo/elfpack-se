#ifndef _GETPOST_H_
#define _GETPOST_H_

#include "IShell.hpp"

//w550...a2
static UUID CID_IGetPostManager={0x4B, 0xB7, 0x2D, 0x95, 0x50, 0xDB, 0x41, 2, 0x92, 0, 0x9E, 0x94, 0x92, 0x63, 0xBE, 0x74};
static UUID IID_IGetPostManager={0x25, 0x40, 0xB, 0xF1, 0x61, 0x4A, 0x44, 0xA6, 0x9B, 0xEC, 0x16, 0x4A, 0xF, 0xBC, 0x10, 0x39};
static UUID IID_ICBGetPostRequest={0x33, 0x57, 0x9A, 0xAF, 0x8A, 0xEF, 0x4C, 0xA3, 0x8D, 0x3C, 0x85, 0xF7, 0xB5, 0x38, 0xF, 0x94};




class IGetPostUtility : public IUnknown
{
public:
	virtual int UserAgentString_GetLength( int null, int null2, int null3, int null4, int* );
	virtual int UserAgentString_Get( int null, int null2, int null3, int null4, int size, char* );
	virtual void UserAgentString_GetBasicLength();
	virtual void UserAgentString_GetBasic();
	virtual int UserAgentProfile_GetLength( int* );
	virtual int UserAgentProfile_Get( int size, char* );
	virtual int RATSpecificUserAgentProfile_GetLength( int networktype, int* );
	virtual int RATSpecificUserAgentProfile_Get( int networktype, int size, char* );
	virtual void Headers_GetHeader();
};


class IGetPostRequest : public IUnknown
{
public:
	virtual int Get(char* url, char* headers);
	virtual void GetToFile();
	virtual void Post();
	virtual void PostFromFile();
	virtual void PostToFile();
	virtual void PostFromFileToFile();
	virtual void Put();
	virtual void PutFromFile();
	virtual void Delete();
	virtual void PostMore();
	virtual void SupplyPassword();
	virtual int ReadHeader(int destbufsize, char* desbuf, void* destarg3, void* destarg4);
	virtual int ReadData(int destbufsize, char* desbuf, void* destarg3, void* destarg4);
	virtual void Cancel();
};


class ICBGetPostRequest : public IUnknown
{
public:
    //на db2020 на один параметр больше
	virtual void OnResult(int DataLength, int HeaderLength, int TotalSize, char MoreData, short StatusCode, int ClientData)=NULL;

	virtual void OnProgress(int CurrPos, int TotSize, int, int status)=NULL;
	virtual void OnSupplyPassword()=NULL;
	virtual void OnSendDataReady()=NULL;
};


struct PROFILENAME
{
	wchar_t profilename[0x12];
};

class IGetPostSession : public IUnknown
{
public:
	virtual int SetIntParameter(char paramnum, int value);
	virtual int SetStringParameter(char paramnum, int strsize, char* str);
	virtual int SetWideStringParameter(char paramnum, int strsize, wchar_t* str);
	virtual int GetIntParameter(char paramnum, int* retvalue);
	virtual int GetStringParameterLength(char paramnum, int* retlen);
	virtual int GetStringParameter(char paramnum, int buflen, char* deststr);
	virtual int GetWideStringParameter(char paramnum, int buflen, wchar_t* deststr);
	virtual void RegisterForSslConnectionEvents();
	virtual int CreateRequest(ICBGetPostRequest* callback, int arg2null, void* arg3null, IGetPostRequest** destarg4);
	virtual void CancelAll(char flag);//???
	virtual int Setup(PROFILENAME name, IUnknown* obj1, int,int,int,int,int,int,int,int,int,int,char* clienttype,char* browsername,char* browserversion,char* profilename,int*);//DB3210: deprecated
	virtual void Cancel();
	virtual int GetMaxDataBufSize(int* destsize1, int* destsize2);
	virtual void Shutdown();
};

class IGetPostSession_DB2020 : public IUnknown
{
public:
	virtual int Setup(PROFILENAME name, IUnknown* obj1, int,int,int,int,int,int,int,int,int,int,char* clienttype,char* browsername,char* browserversion,char* profilename,int*);
	virtual void field_14(...);
	virtual void field_18();
	virtual void field_1C();
	virtual void field_20();
	virtual int CreateRequest(ICBGetPostRequest* callback, u16 arg2null, void* arg3null, IGetPostRequest** destarg4);
	virtual int SetIntParameter(char paramnum, int value);
	virtual void field_2C();
	virtual void field_30();
	virtual void field_34();
	virtual void field_38();
};

class IGetPostManager : public IUnknown
{
public:
	virtual void CreateSession(IShell*, IGetPostSession**);
	virtual void CreateUtility(IShell*, IGetPostUtility**);
};

#endif
