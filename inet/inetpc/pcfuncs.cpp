#include "pctypes.h"
#include "..\common\iunknown.hpp"
#include "..\common\sockets.hpp"
#include "a.hpp"
#include <assert.h>
#include <stdlib.h>

static UUID IID_ISocket = {0x6B, 0x12, 0x29, 6, 0x1E, 0x90, 0x41, 0xAF, 0xAB, 0xE0, 0xFA, 0x2A, 8, 0x7C, 0xA2, 0xFB};
static UUID IID_ISocket2020 = {0xD1, 0x81, 0xBE, 0x3A, 0, 0x90, 0x47, 0x26, 0x86, 0x22, 0x7A, 0xC4, 0x70, 0x87, 0x72, 0x27};
static UUID IID_ISocketUtility = {0xC7, 0x4A, 0x5B, 0x91, 3, 0xE6, 0x40, 0x74, 0xA8, 0x97, 0x74, 0x13, 0xAE, 0xC6, 0x7B, 5};
static UUID IID_ISocketUtility2020 = {0x9F, 0x7B, 0x49, 0xEA, 0xB0, 0xE8, 0x46, 0xA7, 0x9A, 0x64, 0xBC, 0xE2, 0x8E, 0xB1, 0xA7, 0x39};


void CoCreateInstance(PUUID cid, PUUID iid, void** pInterface)
{
	if (!memcmp(cid, &CID_CSocketManager, sizeof(UUID)))
	{
		*pInterface = new ISocketManager();
		return;
	}

	pInterface = NULL;
}


int ISocketManager::CreateSocket(ISocket** pISocket)
{
	*pISocket = new ISocket();
	return 0;
}
int ISocketManager::CreateHostNameResolver(IHostNameResolver** pIHostNameResolver)
{
	*pIHostNameResolver = new IHostNameResolver();
	return 0;
}
int ISocketManager::CreateSocketUtility(ISocketUtility** pISocketUtility)
{
	*pISocketUtility = new ISocketUtility();
	return 0;
}
int ISocketManager::method3()
{
	assert(false);
	return E_FAIL;
}
int ISocketManager::method4()
{
	assert(false);
	return E_FAIL;
}
int ISocketManager::CreateTransportLayerSecurityAgent(ITransportLayerSecurityAgent** pITransportLayerSecurityAgent)
{
	assert(false);
	return E_FAIL;
}
int ISocketManager::CreateSocketUserAgent(wchar_t port, ISocketUserAgent** pISocketUserAgent)
{
	assert(false);
	return E_FAIL;
}
int ISocketManager::CreateZeroCopySocketController(IUnknown** pIZeroCopySocketController)
{
	assert(false);
	return E_FAIL;
}
int ISocketManager::CreateZeroCopySecureSocketController(IUnknown** pIZeroCopySecureSocketController)
{
	assert(false);
	return E_FAIL;
}

static int sock;
int ISocket::Open(Address_Family af, Socket_Type st, int prot)
{
	sock = _socket(af, st, prot);
	return sock == -1;
};

int ISocket::Close()
{
	int ret = _closesocket(sock);
	sock = -1;
	return ret;
}


int ISocket::Accept()
{
	assert(false);
	return E_FAIL;
}
int ISocket::Bind(sockaddr* addr)
{
	assert(false);
	return E_FAIL;
}
int ISocket::Connect(sockaddr* addr)
{
	return _connect(sock, addr, sizeof(sockaddr)) == -1;
}

int ISocket::method4()
{
	assert(false);
	return E_FAIL;
}
int ISocket::method5()
{
	assert(false);
	return E_FAIL;
}
int ISocket::F_GETFL()
{
	assert(false);
	return E_FAIL;
}
int ISocket::F_SETFL(int unk)
{
	assert(false);
	return E_FAIL;
}
int ISocket::getpeername()
{
	assert(false);
	return E_FAIL;
}
int ISocket::GetLocalAddress(sockaddr* addr)
{
	assert(false);
	return E_FAIL;
}
int ISocket::GetOption(int level, int optname, char* optval, int optlen)
{
	assert(false);
	return E_FAIL;
}

int ISocket::SetOption(int level, int optname, char* optval, int optlen)
{
	return _setsockopt(sock, level, optname, optval, optlen);
}

int ISocket::Listen()
{
	assert(false);
	return E_FAIL;
}
int ISocket::Shutdown(int how)
{
	assert(false);
	return E_FAIL;
}
int ISocket::method1()
{
	assert(false);
	return E_FAIL;
}
int ISocket::method2()
{
	assert(false);
	return E_FAIL;
}
int ISocket::RegisterForEvents(ICBSocket* pICBSocket, int MsgBase, void* ClientData, int events)
{
	assert(false);    //пример параметров (NULL, FASTTRACK_APPLICATION_MSGBASE_SOCKET, 0, SOCKET_EVENT_READ | SOCKET_EVENT_WRITE | SOCKET_EVENT_CONNECT | SOCKET_EVENT_CLOSE )
	return E_FAIL;
}

int ISocket::Read(int buf_size, char* buf, int* readen_len)
{
	*readen_len = _recv(sock, buf, buf_size, 0);

	if (*readen_len == 0)
	{
		return _GetLastError();
	}

	return 0;
}

int ISocket::Receive(int flags, int buf_size, char* buf, int* received_len)
{
	assert(false);
	return E_FAIL;
}
int ISocket::ReceiveFrom(int flags, sockaddr* socketAddress, int buf_size, char* buf, int* received_len)
{
	assert(false);
	return E_FAIL;
}
int ISocket::recvmsg()
{
	assert(false);
	return E_FAIL;
}
int ISocket::Send(int flags, int buf_size, char* buf, int* sent_len)
{
	assert(false);
	return E_FAIL;
}
int ISocket::sendmsg()
{
	assert(false);
	return E_FAIL;
}
int ISocket::SendTo(int flags, sockaddr* socketAddress, int buf_size, char* buf, int* sent_len)
{
	assert(false);
	return E_FAIL;
}

int ISocket::Write(int buf_size, char* buf, int* sent_len)
{
	*sent_len = _send(sock, buf, buf_size, 0);

	if (*sent_len == 0)
	{
		return _GetLastError();
	}

	return 0;
}

int ISocket::SecureConnect(wchar_t* pNodeName, wchar_t port)
{
	assert(false);
	return E_FAIL;
}



int IHostNameResolver::method1(sockaddr_short* socketAddress)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::method2(char* pNodeName)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::method3(char* pNodeName, int unk)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::SetHostXXX(char* pNodeName)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::GetHostXXX(char* pNodeName)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::GetFirstHost(char* pNodeName)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::GetLastHost(char* pNodeName)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::GetFirstAddress(sockaddr_short* socketAddress)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::GetLastAddress(sockaddr_short* socketAddress)
{
	assert(false);
	return E_FAIL;
}
int IHostNameResolver::method10()
{
	assert(false);
	return E_FAIL;
}

int IHostNameResolver::GetAddressInfo(char* pNodeName, char* pPortNum, ADDRINFO* pHints, PADDRINFO* pResult)
{
	char* he = _gethostbyname(pNodeName);
	PADDRINFO pres = *pResult = new ADDRINFO;
	memset(pres, 0, sizeof(ADDRINFO));
	pres->ai_addr = new sockaddr_in;
	memset(pres->ai_addr, 0, sizeof(sockaddr_in));

	memcpy(&(pres->ai_addr->sin_addr),
		   he,
		   4);

	pres->ai_addr->sin_port = _htons(atoi(pPortNum));

	pres->ai_addr->sin_family = pHints->ai_family;
	pres->ai_socktype = pHints->ai_socktype;
	return 0;
}

int IHostNameResolver::FreeAddressInfo(addrinfo* ai)
{
	delete ai->ai_addr;
	delete ai;
	return S_OK;
}



int ISocketUtility::NetworkToHost16(wchar_t nPortNetworkOrder, wchar_t* nPortHostOrder)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::NetworkToHost32(int IPNetworkOrder, int* IPHostOrder)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::HostToNetwork16(wchar_t nPortHostOrder, wchar_t* nPortNetworkOrder)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::HostToNetwork32(int IPHostOrder, int* IPNetworkOrder)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::method5()
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::inet_ntop()
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::Host_GetNetworkAddress(int IPHostOrder, int* NetworkAddress)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::MakeSocketIpv4Address(in_addr* sin_addr, wchar_t nPortNetworkOrder, sockaddr* socketAddress)
{
	memcpy(&reinterpret_cast<sockaddr_in*>(socketAddress)->sin_addr, sin_addr, sizeof(*sin_addr));
	&reinterpret_cast<sockaddr_in*>(socketAddress)->sin_port = nPortNetworkOrder;
	socketAddress->sin_family = AF_INET;
	return 0;
}
int ISocketUtility::MakeSocketIpv6Address(in6_addr* sin_addr, wchar_t nPortNetworkOrder, unsigned long sin6_flowinfo, unsigned long sin6_scope_id, sockaddr* socketAddress)
{
	assert(false);
	return E_FAIL;
}
int ISocketUtility::MakeSocketAddress(sockaddr_short* sockAddress, wchar_t nPortNetworkOrder, sockaddr* socketAddress)
{
	assert(false);
	return E_FAIL;
}


int ISocket::QueryInterface(UUID* id, void** ret)
{
	*ret = NULL;

	if (!memcmp(id, &IID_ISocket, sizeof(UUID)))
	{
		AddRef();
		*ret = this;
		return S_OK;
	}

	return IUnknown::QueryInterface(id, ret);
}

int ISocketUtility::QueryInterface(UUID* id, void** ret)
{
	*ret = NULL;

	if (!memcmp(id, &IID_ISocketUtility, sizeof(UUID)))
	{
		AddRef();
		*ret = this;
		return S_OK;
	}

	return IUnknown::QueryInterface(id, ret);
}
