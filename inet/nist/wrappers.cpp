#include "wrappers.hpp"

static UUID IID_ISocket3150={0x6B, 0x12, 0x29, 6, 0x1E, 0x90, 0x41, 0xAF, 0xAB, 0xE0, 0xFA, 0x2A, 8, 0x7C, 0xA2, 0xFB};
static UUID IID_ISocket3210={0xAB, 0x82, 0xEA, 0x75, 0xAA, 0x1A, 0x4A, 0x43, 0xA4, 0x11, 0xD7, 0xD9, 0xFE, 0x64, 0xE4, 0x69};
static UUID IID_ISocket2020={0xD1, 0x81, 0xBE, 0x3A, 0, 0x90, 0x47, 0x26, 0x86, 0x22, 0x7A, 0xC4, 0x70, 0x87, 0x72, 0x27};
static UUID IID_ISocket2010={0x40, 0xB8, 0x71, 0xD2, 0x36, 0xEC, 0x4E, 0xAB, 0x85, 0xF1, 0x18, 0xE8, 0xB9, 0x52, 0x90, 0x88};
static UUID IID_ISocketUtility3150_3210={0xC7, 0x4A, 0x5B, 0x91, 3, 0xE6, 0x40, 0x74, 0xA8, 0x97, 0x74, 0x13, 0xAE, 0xC6, 0x7B, 5};
static UUID IID_ISocketUtility2020={0x9F, 0x7B, 0x49, 0xEA, 0xB0, 0xE8, 0x46, 0xA7, 0x9A, 0x64, 0xBC, 0xE2, 0x8E, 0xB1, 0xA7, 0x39};
static UUID IID_ISocketUtility2010={0xD4, 0xD, 0xB5, 0x52, 0x63, 4, 0x49, 0xCA, 0x90, 0x13, 0x36, 0x6A, 0x43, 0x95, 0xD2, 0x4B};

#define WSOCKETMETHOD(method,...) if(psock) return psock->method(__VA_ARGS__); if(psock2020) return psock2020->method(__VA_ARGS__); return E_FAIL;
#define WSOCKETUTILMETHOD(method,...) if(psockutil) return psockutil->method(__VA_ARGS__); if(psockutil2020) return psockutil2020->method(__VA_ARGS__); return E_FAIL;

int WSocket::attach(IUnknown* ptr)
{
	if(!ptr)
		return E_FAIL;
	if( S_OK == ptr->QueryInterface( &IID_ISocket3150, PPINTERFACE(&psock) ) )
		return S_OK;
	if( S_OK == ptr->QueryInterface( &IID_ISocket3210, PPINTERFACE(&psock) ) )
		return S_OK;
	if( S_OK == ptr->QueryInterface( &IID_ISocket2020, PPINTERFACE(&psock2020) ) )
		return S_OK;
	//основные методы вроде совпадают
	if( S_OK == ptr->QueryInterface( &IID_ISocket2010, PPINTERFACE(&psock2020) ) )
		return S_OK;
#ifndef WIN32
	if( ptr )
		debug_printf("\n!!! wsock: unknown interface at %x\n", *(long*)ptr );
#endif
	return E_NOINTERFACE;
}

int WSocket::Open(Address_Family af, Socket_Type type, int protocol)
{
	WSOCKETMETHOD(Open,af, type, protocol);
}
int WSocket::Connect(sockaddr_in * addr)
{
	WSOCKETMETHOD(Connect,addr);
}

int WSocket::Write(int buf_size,char * buf,int * sent_len)
{
	WSOCKETMETHOD(Write, buf_size, buf, sent_len );
}

int WSocket::Read(int buf_size,char * buf,int * readen_len)
{
	WSOCKETMETHOD(Read, buf_size, buf, readen_len);
}

int WSocket::SetOption(int level, int optname, char * optval, int optlen)
{
	WSOCKETMETHOD(SetOption, level, optname, optval, optlen);
}

int WSocket::GetOption(int level, int optname, char * optval, int optlen)
{
	WSOCKETMETHOD(GetOption, level, optname, optval, optlen);
}


int WSocketUtility::attach(IUnknown* ptr)
{
	if(!ptr)
		return E_FAIL;
	if( S_OK == ptr->QueryInterface( &IID_ISocketUtility3150_3210, PPINTERFACE(&psockutil) ) )
		return S_OK;
	if( S_OK == ptr->QueryInterface( &IID_ISocketUtility2020, PPINTERFACE(&psockutil2020) ) )
		return S_OK;
	if( S_OK == ptr->QueryInterface( &IID_ISocketUtility2010, PPINTERFACE(&psockutil2020) ) )
		return S_OK;
#ifndef WIN32
	if( ptr )
		debug_printf("\n!!! wsockutil: unknown interface at %x\n", *(long*)ptr );
#endif
	return E_NOINTERFACE;
}

int WSocketUtility::MakeSocketIpv4Address(in_addr * sin_addr,wchar_t nPortNetworkOrder,sockaddr_in * socketAddress)
{
	WSOCKETUTILMETHOD(MakeSocketIpv4Address, sin_addr, nPortNetworkOrder, socketAddress);
}

int WSocketUtility::MakeSocketIpv6Address(in6_addr * sin_addr,wchar_t nPortNetworkOrder,unsigned long sin6_flowinfo,unsigned long sin6_scope_id,sockaddr_in6 * socketAddress)
{
	WSOCKETUTILMETHOD(MakeSocketIpv6Address, sin_addr, nPortNetworkOrder, sin6_flowinfo, sin6_scope_id, socketAddress);
}

int WSocketUtility::MakeSocketAddress(sockaddr_short * sockAddress,wchar_t nPortNetworkOrder,sockaddr_in * socketAddress)
{
	WSOCKETUTILMETHOD(MakeSocketAddress, sockAddress, nPortNetworkOrder, socketAddress);
}



int CreateSocketManager(void** ret)
{
	if(!ret)
		return E_FAIL;
	*ret = NULL;
	CoCreateInstance(&CID_CSocketManager,&IID_ISocketManager,ret);
	if(!*ret)
		CoCreateInstance(&CID_CSocketManager,&IID_ISocketManager3150,ret);
	if(!*ret)
		CoCreateInstance(&CID_CSocketManager,&IID_ISocketManager2020,ret);
	if(!*ret)
		CoCreateInstance(&CID_CSocketManager,&IID_ISocketManager2010,ret);
	if(!*ret)
		return E_FAIL;
	return S_OK;
}
