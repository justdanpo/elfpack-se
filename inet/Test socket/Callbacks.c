#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "..\common\IUnknown.hpp"
#include "..\common\Sockets.hpp"
#include "..\common\ConnectionManager.hpp"

#include "Callbacks.h"
#include "main.h"


void ICBTestSocket::OnOutOfBandData(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnAccept(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnConnect(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnClose(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnSend(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnSecureConnect(int Error, ISocket* pISocket, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}

void ICBTestSocket::OnReceive(int Error, int unk, int BufferLength, void* ClientData)
{
	debug_printf("\nSocket Callback %s\n", __FUNCTION__);

	if (Error < 0)
	{
		debug_printf("\nFailed with error: 0x%08X\n", Error);
		return;
	}
}



void ICBTestConnManager::OnInitialFailure(int AccountID, char Cause, void* ClientData)
{
}

void ICBTestConnManager::OnConnecting(int ConnectionID, int AccountID, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnAuthenticating(int ConnectionID, int AccountID, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnRegisteringToNetwork()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnRegisteringToNetworkFailed()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnConnected(int ConnectionID, int AccountID, void* ClientData)
{
	debug_printf("\nConnManager Callback %s, x1 = 0x%08X, x2 = 0x%08X, x3 = 0x%08X\n", __FUNCTION__, ConnectionID, AccountID, ClientData);

	TestSocketBook* TSBook = (TestSocketBook*)ClientData;

	ISocketManager* pISocketManager = 0;
	ISocketUtility* pISocketUtility = 0;
	IHostNameResolver* pIHostNameResolver = 0;
	ISocketUserAgent* pISocketUserAgent = 0;

	char* hostname = "www.yandex.ru";
	char* ServiceName = "80";

	char data_buf[50];
	//int data_len=0;
	ADDRINFO pHints;
	PADDRINFO pResult = 0;
	sockaddr socket_address;

	sockaddr local_socket_address;
	in_addr local_ip;

	memset(&pHints, 0, sizeof(ADDRINFO));
	memset(&socket_address, 0, sizeof(socket_address));
	memset(&data_buf, 0, sizeof(data_buf));

	memset(&local_socket_address, 0, sizeof(local_socket_address));
	memset(&local_ip, 0, sizeof(in_addr));

	pHints.ai_family = AF_INET;
	pHints.ai_socktype = SOCK_STREAM;

	CoCreateInstance(&CID_CSocketManager, &IID_ISocketManager, PPINTERFACE(&pISocketManager));

	if (!pISocketManager)
	{
		CoCreateInstance(&CID_CSocketManager, &IID_ISocketManager3150, PPINTERFACE(&pISocketManager));
	}

	if (!pISocketManager)
	{
		CoCreateInstance(&CID_CSocketManager, &IID_ISocketManager2020, PPINTERFACE(&pISocketManager));
	}

	if (!pISocketManager)
	{
		CoCreateInstance(&CID_CSocketManager, &IID_ISocketManager2010, PPINTERFACE(&pISocketManager));
	}

	if (!pISocketManager)
	{
		debug_printf("\n!!! bad pISocketManager\n");
		return;
	}

	pISocketManager->CreateSocket(&TSBook->pISocket);
	pISocketManager->CreateSocketUtility(&pISocketUtility);
	pISocketManager->CreateHostNameResolver(&pIHostNameResolver);

	pIHostNameResolver->GetAddressInfo(hostname, ServiceName, &pHints, &pResult);

	//  debug_printf("\nADDRINFO = 0x%08X\n",pResult);
	//  debug_printf("\nFamily = %d, Socktype = %d, \n",pResult->ai_family,pResult->ai_socktype);
	//  debug_printf("\n%03d.%03d.%03d.%03d:%d\n",pResult->ai_addr->sin_addr.s_addr[0],pResult->ai_addr->sin_addr.s_addr[1],pResult->ai_addr->sin_addr.s_addr[2],pResult->ai_addr->sin_addr.s_addr[3],pResult->ai_addr->sin_port);

	pISocketUtility->MakeSocketIpv4Address(&pResult->ai_addr->sin_addr, pResult->ai_addr->sin_port, &socket_address);

	//  debug_printf("\nLength = %d, Family = %d, \n",socket_address.sin_length,socket_address.sin_family);
	//  debug_printf("\n%03d.%03d.%03d.%03d:%d\n",socket_address.sin_addr.s_addr[0],socket_address.sin_addr.s_addr[1],socket_address.sin_addr.s_addr[2],socket_address.sin_addr.s_addr[3],socket_address.sin_port);

	pIHostNameResolver->FreeAddressInfo(pResult);

	linger ling = {0, 0};
	int sndbuf = 100;
	int rcvbuf = 100;
	int unk = 1;

	pISocketUtility->MakeSocketIpv4Address(&local_ip, 2948, &local_socket_address);

	TSBook->pISocket->Open(AF_INET, SOCK_STREAM, 0);
	//!!!на дб2020 метод отсутствует
	//TSBook->pIConnManager->SetSocket(TSBook->pISocket);

	TSBook->pISocket->RegisterForEvents(TSBook->pICBTestSocket, 0, 0, SOCKET_EVENT_READ | SOCKET_EVENT_WRITE | SOCKET_EVENT_ACCEPT | SOCKET_EVENT_CONNECT | SOCKET_EVENT_CLOSE);
	//TSBook->pISocket->Bind(&local_socket_address);
	TSBook->pISocket->SetOption(OPTION_LEVEL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
	TSBook->pISocket->SetOption(OPTION_LEVEL_SOCKET, 3, (char*)&unk, sizeof(unk));
	TSBook->pISocket->SetOption(OPTION_LEVEL_SOCKET, 4, (char*)&unk, sizeof(unk));
	TSBook->pISocket->SetOption(OPTION_LEVEL_SOCKET, SO_SNDBUF, (char*)&sndbuf, sizeof(sndbuf));
	TSBook->pISocket->SetOption(OPTION_LEVEL_SOCKET, SO_RCVBUF, (char*)&rcvbuf, sizeof(rcvbuf));
	TSBook->pISocket->F_SETFL(1);

	TSBook->pISocket->Connect(&socket_address);

	int slen = 0;
	int sret =  TSBook->pISocket->Send(0, 19, "GET /xxx HTTP/1.0\n\n", &slen);
	debug_printf("\n=) sent %d %d\n", sret, slen);

	char x[256];
	slen = 255;
	sret = TSBook->pISocket->Receive(0, 256, x, &slen);
	debug_printf("\n=) received %x %d\n", sret, slen);
	x[255] = 0;
	debug_printf("\n=) got: %s\n", x);

	TSBook->pISocket->Close();

	if (pISocketManager)
	{
		pISocketManager->Release();
	}

	if (pISocketUtility)
	{
		pISocketUtility->Release();
	}

	if (pIHostNameResolver)
	{
		pIHostNameResolver->Release();
	}

	if (pISocketUserAgent)
	{
		pISocketUserAgent->Release();
	}


	/*  IConnManager * x = TSBook->pIConnManager;
	int * y = ((int**)x)[1];
	int z50 = y[20];
	int *z = (int*)y[19];
	int z4c = *z;
	MessageBox(EMPTY_SID, int2strID(z50), NOIMAGE, 0, 0, 0);
	MessageBox(EMPTY_SID, int2strID(z4c), NOIMAGE, 0, 0, 0);*/

}

void ICBTestConnManager::OnModified()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnPDPContextModified(int ConnectionID, int AccountID, int StatusInfo, int EventCode, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnDisconnecting()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnDisconnected(int ConnectionID, int AccountID, int StatusInfo, int EventInfo, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnAlternativeAccount()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnAlternativeAccountAccepted()
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnAuthenticate(int ConnectionID, int AccountID, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnNoTraffic(int ConnectionID, int AccountID, int Direction, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}

void ICBTestConnManager::OnConnectionSessionEnded(int ConnectionID, int AccountID, int SessionEndCause, void* ClientData)
{
	debug_printf("\nConnManager Callback %s\n", __FUNCTION__);
}


void CreateCallbacks(ICBTestConnManager** pICBTestConnManager, ICBTestSocket** pICBTestSocket)
{
	*pICBTestConnManager = new ICBTestConnManager();
	*pICBTestSocket = new ICBTestSocket();
}

