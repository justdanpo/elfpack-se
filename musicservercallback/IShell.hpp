#ifndef _ISHELL_H_
#define _ISHELL_H_

class IShell : public IUnknown
{
public:
	virtual int AllocateMemory( int Size, void** ppBuffer );
	virtual int CreateMsg( int Base, int Id, int Size, int** ppMsg );
	virtual bool CompareUUID( UUID* pIID1, UUID* pIID2 );
	virtual int FreeMsg( int** ppMsg );
	virtual int CreateInstance( UUID* pCid, UUID* pIid, IUnknown** ppInterfacePointer );
	virtual int FreeMemory( void** ppBuffer );
	virtual int MarkInterfacesInMsg( int** ppMsg, IUnknown** ppInterfacePointer );
	virtual int ReallocateMemory( unsigned Size, void** ppBuffer );
	virtual int ReleaseInterfacesInMsg( int** ppMsg );
	virtual bool IsKindOfInterface( IUnknown* pInterface, UUID* pIId );
	virtual int InvalidateCallbackInterface( IUnknown* pICB );
	virtual int GetCallerExecutionShell( IUnknown** ppIExecutionShell );
};

#endif
