
#include "..\common\iunknown.hpp"
#include "..\common\sockets.hpp"

#ifdef WIN32
#include "a.hpp"
#endif

#include "wrappers.hpp"

int main(int argc, char* argv[])
{
#ifdef WIN32
	init();	
#endif
	
	ISocketManager * pISocketManager = NULL;
	ISocket* pSocket = NULL;
	ISocketUtility* pSocketUtility = NULL;
	IHostNameResolver* pIHostNameResolver = NULL;

	CreateSocketManager(PPINTERFACE(&pISocketManager));
	if(pISocketManager)
	{
		if( S_OK == pISocketManager->CreateSocket(&pSocket) )
		{
			WSocket sock;
			if(S_OK == sock.attach(pSocket) )
			{
				if( S_OK == pISocketManager->CreateSocketUtility(&pSocketUtility) )
				{
					WSocketUtility sockutil;
					if(S_OK == sockutil.attach(pSocketUtility) )
					{	
						if( S_OK == pISocketManager->CreateHostNameResolver(&pIHostNameResolver) )
						{
							ADDRINFO hint;
							memset(&hint, 0, sizeof(hint));
							hint.ai_family = AF_INET;
							hint.ai_socktype = SOCK_STREAM;
							ADDRINFO* paddr;
							sockaddr_in socket_address;
							
							if( S_OK == pIHostNameResolver->GetAddressInfo("jabber.ru","5222/tcp",&hint, &paddr) )
							{
								memset(&socket_address, 0, sizeof(socket_address));
								sockutil.MakeSocketIpv4Address(&paddr->ai_addr->sin_addr,paddr->ai_addr->sin_port,&socket_address);
								//???sockutil.MakeSocketIpv6Address(&paddr->ai6_addr->sin6_addr,paddr->ai_addr->sin_port,0,0,(sockaddr_in6*)&socket_address);
								pIHostNameResolver->FreeAddressInfo(paddr);
								
								if( S_OK == sock.Open(AF_INET,SOCK_STREAM,0) )
								{
									int tcpnodelayflag=TRUE;
									if( S_OK == sock.SetOption(OPTION_LEVEL_TCP,TCP_NODELAY,(char*)&tcpnodelayflag,sizeof(tcpnodelayflag)) )
									{
										if( S_OK == sock.Connect(&socket_address) )
										{

											int sent;
											sock.Write(4, ">123\n", &sent);
											
											char x[256];
											sock.Read(sizeof(x)-1,x, &sent);
											x[sent]=0;
											
											debug_printf("\n!!! %s\n",x);
											
										}//if( S_OK == sock.Connect(&socket_address) )
									}//if( S_OK == sock.SetOption(OPTION_LEVEL_TCP,TCP_NODELAY,(char*)&tcpnodelayflag,sizeof(tcpnodelayflag)) )
								}//if( S_OK == sock.Open(AF_INET,SOCK_STREAM,0) )
								
							}//if( S_OK == pIHostNameResolver->GetAddressInfo("jabber.ru","5222",&hint, &paddr) )
							
							pIHostNameResolver->Release();
						}//if( S_OK == pISocketManager->CreateHostNameResolver(&pIHostNameResolver) )
					}//if(S_OK == sockutil.attach(pSocketUtility) )
					pSocketUtility->Release();
				}//if( S_OK == pISocketManager->CreateSocketUtility(&pSocketUtility) )
			}//if(S_OK == sock.attach(pSocket) )
			pSocket->Release();
		}//if( S_OK == pISocketManager->CreateSocket(&pSocket) )
		pISocketManager->Release();
		
	}//if(pISocketManager)
	
	return 0;
}

