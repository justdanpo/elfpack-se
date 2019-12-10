void init();

int _socket(int af,int st,int prot);
int _closesocket(int s);
int _connect(int s, void* b, int sz);
int _setsockopt( int sock, int level, int optname, char* optval, int optlen);
int _recv( int sock, char* buf, int buf_size, int flags );
int _GetLastError();
int _send( int sock, char* buf, int buf_size, int flags );
char* _gethostbyname(char* host);
int _htons(unsigned short p);

int debug_printf(char*,...);

