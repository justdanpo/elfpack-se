#ifndef _CONNECTIONMANAGER_H_
#define _CONNECTIONMANAGER_H_

#include "iunknown.hpp"
#include "sockets.hpp"

static UUID IID_IConnManagerManager={0xC9,0x38,0x8C,0x88,0xFB,0xD5,0x4C,0x75,0xA5,0x58,0x33,0xC7,0xFF,0x1E,0x31,0xC2};
static UUID CID_CConnManagerManager={0x82,0x01,0x15,0x84,0xC9,0xBC,0x40,0xF9,0x86,0xC0,0xD8,0xC5,0x42,0x31,0x75,0x3E};
static UUID IID_IConnManagerBaseParameters={0x4D,0xBD,0x5A,0x5F,0xD5,0x7B,0x4B,0xE7,0x96,0x64,0xFB,0xAE,0x9C,0xBF,0x8C,0x70};

class IConnManagerManager;
class IConnManagerParameters;
class IConnManager;
class ICBConnManager;


class IConnManagerManager: public IUnknown
{
public:
  virtual int OpenConnection(ICBConnManager * pICBConnManager,wchar_t unk,void * ClientData,IUnknown * pConnManagerBaseParameters,IConnManager ** pIConnManager);
  virtual int CreateConnManagerStatus();
  virtual int CreateConnManagerResourceStatus();
  virtual int CreateConnManagerDataTransferStatus();
  virtual int CreateConnManagerParameters(int accountID,IConnManagerParameters ** pConnManagerParameters);
  virtual int CreateConnManagerSecondaryContextParameters();
  virtual int CreateConnManagerConfiguration();
};


class IConnManager: public IUnknown
{
public:
  virtual int CloseConnection();
  virtual int method2();
  virtual int method3();
  virtual int GetInternetConnection(IUnknown ** pIInternetConnection);
  virtual int GetClientID(int * ClientID);
  virtual int method6();
  virtual int method7();
  virtual int method8();
  virtual int method9();
  virtual int method10();
  virtual int method11();
  virtual int method12();
  virtual int method13();
  virtual int SetSocket(ISocket * pISocket);	//название немного некорректное, там какая-то опция устанавливается
  virtual int GetProxy(int unk,IUnknown ** pIProxy);
  virtual int method16();
};


class ICBConnManager : public IUnknown
{
public:
  virtual void OnInitialFailure(int AccountID,char Cause,void * ClientData)=NULL;
  virtual void OnConnecting(int ConnectionID,int AccountID,void * ClientData)=NULL;
  virtual void OnAuthenticating(int ConnectionID,int AccountID,void * ClientData)=NULL;
  virtual void OnRegisteringToNetwork()=NULL;
  virtual void OnRegisteringToNetworkFailed()=NULL;
  virtual void OnConnected(int ConnectionID,int AccountID,void * ClientData)=NULL;
  virtual void OnModified()=NULL;
  virtual void OnPDPContextModified(int ConnectionID,int AccountID,int StatusInfo,int EventCode,void * ClientData)=NULL;
  virtual void OnDisconnecting()=NULL;
  virtual void OnDisconnected(int ConnectionID,int AccountID,int StatusInfo,int EventInfo,void * ClientData)=NULL;
  virtual void OnAlternativeAccount()=NULL;
  virtual void OnAlternativeAccountAccepted()=NULL;
  virtual void OnAuthenticate(int ConnectionID,int AccountID,void * ClientData)=NULL;
  virtual void OnNoTraffic(int ConnectionID,int AccountID,int Direction,void * ClientData)=NULL;
  virtual void OnConnectionSessionEnded(int ConnectionID,int AccountID,int SessionEndCause,void * ClientData)=NULL;
};


class IConnManagerBaseParameters : public IUnknown
{
public:
	virtual int SetConnectionNotify(short); // 0 - show dialog
	virtual int Set3A(char);
	virtual int Set3B(char);
	virtual int Set3C(char);
	virtual int Set50(char);
	virtual int Set4C(int);
	virtual int Set40(int);
	virtual int Set48_44(int flag,int value);
	virtual int Set51(char);
	virtual int Set52(char);
	virtual int GetConnectionNotify(short*);
	virtual int Get3A(char*);
	virtual int Get3B(char*);
	virtual int Get3C(char*);
	virtual int Get50(char*);
	virtual int Get4C(int*);
	virtual int Get40(int*);
	virtual int Get44_48(int flag,int* ret);
	virtual int Get51(char*);
	virtual int Get52(char*);
	virtual int Get54(int*);
};


class IConnManagerParameters : public IConnManagerBaseParameters
{
public:
	virtual int Set58_5C(char,int);
	virtual int Get58_5C(char*,int*);
};

#endif
