#ifndef _PCTYPES_H_
#define _PCTYPES_H_

#include <memory.h>

typedef struct
{
	unsigned char data[16];
}UUID, *PUUID;


// #define IID_PPV_ARG( Type, Expr ) &IID_##Type, reinterpret_cast<void**>( static_cast<Type**>( Expr ) )
#ifdef __cplusplus
template<typename T> void** PPINTERFACE( T** pp )
{
	return reinterpret_cast<void**>( pp );
}
#else
#define PPINTERFACE( v ) (void**)v
#endif


#ifndef _NULL
#define _NULL 0
#endif
#ifndef NULL
#define NULL _NULL
#endif

#define TRUE 1
#define FALSE 0


void CoCreateInstance( PUUID cid, PUUID iid, void** pInterface );


#define TCP_NODELAY         	 0x0001
#define TCP_EXPEDITED_1122  	 0x0002
#define TCP_KEEPALIVE       	 3
#define TCP_MAXSEG          	 4
#define TCP_MAXRT           	 5
#define TCP_STDURG          	 6
#define TCP_NOURG           	 7
#define TCP_ATMARK          	 8
#define TCP_NOSYNRETRIES    	 9
#define TCP_TIMESTAMPS      	 10
#define TCP_OFFLOAD_PREFERENCE	 11
#define TCP_CONGESTION_ALGORITHM 12
#define TCP_DELAY_FIN_ACK        13


#endif