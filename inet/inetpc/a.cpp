#include <stdarg.h>
#include <stdio.h>
#include <WinSock2.h>

static WSADATA wsadata;

void init()
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

int _socket(int af, int st, int prot)
{
	return socket(af, st, prot);
}

int _closesocket(int s)
{
	return closesocket(s);
}


typedef struct
{
	unsigned char            sin_length;     // Размер структуры
	unsigned char            sin_family;     // Семейство адресов
	unsigned short int       sin_port;       // Номер порта
	in_addr                  sin_addr;       // IP4-адрес
	unsigned char            sin_zero[8];    // "Дополнение" до размера структуры sockaddr
} __sockaddr_in;

int _connect(int s, void* b, int sz)
{
	__sockaddr_in* inaddr = (__sockaddr_in*)b;
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = inaddr->sin_family;
	sa.sin_port = inaddr->sin_port;
	sa.sin_addr = inaddr->sin_addr;

	int ret = connect(s, (sockaddr*)&sa, sizeof(sa));
	return ret;
}


int _setsockopt(int sock, int level, int optname, char* optval, int optlen)
{
	return setsockopt(sock, level, optname, optval, optlen);
}

int _recv(int sock, char* buf, int buf_size, int flags)
{
	return recv(sock, buf, buf_size, flags);
}

int _GetLastError()
{
	return GetLastError();
}

int _send(int sock, char* buf, int buf_size, int flags)
{
	return send(sock, buf, buf_size, flags);
}

char* _gethostbyname(char* host)
{
	hostent* he = gethostbyname(host);

	if (!he)
	{
		return NULL;
	}

	return &he->h_addr_list[0][0];
}

int _htons(unsigned short p)
{
	return htons(p);
}


int debug_printf(char* formatstring, ...)
{
	va_list args;
	va_start(args, formatstring);

	return vprintf(formatstring, args);
}