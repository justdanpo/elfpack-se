#ifndef _WRAPPERS_HPP_
#define _WRAPPERS_HPP_

#include "..\common\iunknown.hpp"
#include "..\common\sockets.hpp"

class WSocket
{
	ISocket* psock;
	ISocket2020* psock2020;
	
public:

	WSocket(): psock(NULL), psock2020(NULL)
	{
	}

	~WSocket()
	{
		if(psock)psock->Release();
		if(psock2020)psock2020->Release();
	}
	
	int attach(IUnknown* ptr);

	virtual int Open(Address_Family af, Socket_Type type, int protocol);
	virtual int Connect(sockaddr_in * addr);
	virtual int Write(int buf_size,char * buf,int * sent_len);
	virtual int Read(int buf_size,char * buf,int * readen_len);
	virtual int SetOption(int level, int optname, char * optval, int optlen);
	virtual int GetOption(int level, int optname, char * optval, int optlen);
};

class WSocketUtility
{
	ISocketUtility* psockutil;
	ISocketUtility2020* psockutil2020;

public:
	
	WSocketUtility(): psockutil(NULL), psockutil2020(NULL)
	{
	}

	~WSocketUtility()
	{
		if(psockutil)psockutil->Release();
		if(psockutil2020)psockutil2020->Release();
	}
	
	int attach(IUnknown* ptr);
	virtual int MakeSocketIpv4Address(in_addr * sin_addr,wchar_t nPortNetworkOrder,sockaddr_in * socketAddress);
	virtual int MakeSocketIpv6Address(in6_addr * sin_addr,wchar_t nPortNetworkOrder,unsigned long sin6_flowinfo,unsigned long sin6_scope_id,sockaddr_in6 * socketAddress);
	virtual int MakeSocketAddress(sockaddr_short * sockAddress,wchar_t nPortNetworkOrder,sockaddr_in * socketAddress);
};


int CreateSocketManager(void** ret);

#endif
