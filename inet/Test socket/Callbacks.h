#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

class ICBTestConnManager : public ICBConnManager
{
public:
  virtual void OnInitialFailure(int AccountID,char Cause,void * ClientData);
  virtual void OnConnecting(int ConnectionID,int AccountID,void * ClientData);
  virtual void OnAuthenticating(int ConnectionID,int AccountID,void * ClientData);
  virtual void OnRegisteringToNetwork();
  virtual void OnRegisteringToNetworkFailed();
  virtual void OnConnected(int ConnectionID,int AccountID,void * ClientData);
  virtual void OnModified();
  virtual void OnPDPContextModified(int ConnectionID,int AccountID,int StatusInfo,int EventCode,void * ClientData);
  virtual void OnDisconnecting();
  virtual void OnDisconnected(int ConnectionID,int AccountID,int StatusInfo,int EventInfo,void * ClientData);
  virtual void OnAlternativeAccount();
  virtual void OnAlternativeAccountAccepted();
  virtual void OnAuthenticate(int ConnectionID,int AccountID,void * ClientData);
  virtual void OnNoTraffic(int ConnectionID,int AccountID,int Direction,void * ClientData);
  virtual void OnConnectionSessionEnded(int ConnectionID,int AccountID,int SessionEndCause,void * ClientData);
};


class ICBTestSocket : public ICBSocket
{
public:
  virtual void OnOutOfBandData(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnAccept(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnConnect(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnClose(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnSend(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnSecureConnect(int Error,ISocket * pISocket,void * ClientData);
  virtual void OnReceive(int Error,int unk,int BufferLength,void * ClientData);
};

void CreateCallbacks(ICBTestConnManager ** pICBTestConnManager,ICBTestSocket ** pICBTestSocket);

#endif
