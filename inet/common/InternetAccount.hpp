#ifndef _INTERNETACCOUNT_H_
#define _INTERNETACCOUNT_H_

static UUID CID_CInternetAccountManager={0x34, 0xA0, 0xD2, 0xDA, 0xA7, 0x67, 0x42, 0x55, 0xBA, 0xF1, 0x2F, 0x33, 0x93, 0x37, 0x36, 0x60};
//a2
static UUID IID_IInternetAccountManager={0xC8, 0xC3, 0x79, 0x1C, 0x5E, 0x13, 0x4E, 0x23, 0x8B, 0x17, 0xF8, 0x3D, 0xE5, 0x28, 0x65, 0x77};
static UUID IID_IInternetAccountManager2={0x10, 0xCA, 0x64, 0x68, 0x6B, 0x75, 0x45, 0x13, 0xB1, 0x28, 0xCC, 0xA0, 0xCD, 0xD5, 0x28, 0x49};
static UUID IID_IInternetAccountManager3={0xD8, 0x3C, 0x96, 0x6F, 0x2D, 0x6D, 0x42, 0x85, 0x92, 0x56, 0xA4, 0x2B, 0x41, 0xBF, 0xE0, 0x47};
//db2020
static UUID IID_IInternetAccountManager2020={0x30, 0xDB, 0xFE, 0xE5, 0x19, 0x8F, 0x4C, 0xBB, 0xA1, 0x64, 0x93, 0x3E, 0xF7, 0x1C, 0x11, 0x60};
//K600,w550,W600,W800
static UUID IID_IInternetAccountManager_old2010={0x20, 0xD4, 0x7F, 0x5D, 0xB6, 0xE5, 0x4B, 0xAB, 0x8C, 0x20, 0x12, 0x96, 0xC3, 0xE2, 0xB0, 0x28};
//K750,W200,W300,W700,W810,Z530,Z550
static UUID IID_IInternetAccountManager_2010={0xDD, 0xEA, 0x29, 0xA0, 0x6B, 0x9C, 0x4A, 0x51, 0xAC, 0xBC, 0xFB, 0xFB, 0x87, 0xD0, 0x6B, 0xB5};


typedef void TCollectionElement;


class IIterator : public IUnknown
{
	virtual void GetCurrent( TCollectionElement** ppElement );
	virtual void GetCurrentPosition( unsigned* pPosition );
	virtual void SetPosition( unsigned Position );
	virtual void GetFirst( TCollectionElement** ppElement );
	virtual void GetLast( TCollectionElement** ppElement );
	virtual void GetNext( TCollectionElement** ppElement );
	virtual void GetPrevious( TCollectionElement** ppElement );
	virtual bool HasNext(void);
	virtual bool HasPrevious(void);
};


class ICollection : public IUnknown
{
public:
	virtual void DeleteAll(void);
	virtual void CreateIterator( IIterator** ppIIterator );
	virtual void GetCardinality( unsigned*  pCardinality );
};


class IList : public ICollection
{
public:
	virtual void Add( TCollectionElement* pElement );
	virtual void Delete( unsigned Position );
	virtual void Get( unsigned Position, TCollectionElement** ppElement );
	virtual void Insert( TCollectionElement* pElement, unsigned* pPosition );
	virtual void Remove( unsigned Position );
	virtual void Set( TCollectionElement* pElement, unsigned Position, TCollectionElement** ppPreviousElement );
};



class IInternetAccount_Common: public IUnknown
{
public:
	virtual void sub_1146A298();
	virtual int GetName(wchar_t* name);
	virtual int SetName(wchar_t* name);
};


class IInternetAccountManager: public IUnknown
{
public:
	virtual void CreatePSAccount();
	virtual void CreateCSAccount();
	virtual void CreateBTAccount();
	virtual void CreateBTPanAccount();
	virtual void CreateUSBInetAccount();
	virtual void CreateDialInAccount();
	virtual void sub_1145B8FC();
	virtual void sub_1145B900();
	virtual void sub_1145B908();
	virtual int OpenAccount(int accountId, IInternetAccount_Common** account);
	virtual int DestroyAccounts(void);//no this
	virtual int GetAccounts(IList** list);
	virtual int GetNrOfAccounts(int* num);
	virtual void sub_1145BC18();
	virtual int OpenConnection(int connectionId, IUnknown** pIInternetConnection);//connect?
	virtual int DisconnectConnections();//no this
	virtual int GetConnections(IList** list);
	virtual int GetNrOfConnections(int* num);
	virtual void sub_1145BEB8();
	virtual void SubscribeToDataCounterEvents();
	virtual void UnsubscribeFromDataCounterEvents();
	virtual void sub_1145BFBC();
	virtual void SubscribeToStatusEvents();
	virtual void UnubscribeFromoStatusEvents();
	virtual void SubscribeToAuthenticationEvents();
	virtual void UnsubscribeFromAuthenticationEvents();
	virtual void sub_1145C18C();
	virtual void sub_1145C1A8();
	virtual void CInternetAccountManager_impc_1145C1C4();
	virtual void CInternetAccountManager_impc_1145C24C();
};


class IInternetAccountManager2020: public IUnknown
{
public:
	virtual void CreatePSAccount();
	virtual void CreateCSAccount();
	virtual void CreateBTAccount();
	virtual void CreateBTPanAccount();
	virtual void CreateUSBInetAccount();
	virtual void CreateDialInAccount();
	virtual int OpenAccount(int num, IInternetAccount_Common** account);
	virtual int DestroyAccounts(void);//no this
	virtual int GetAccounts(IList** list);
	virtual int GetNrOfAccounts(int* num);
	virtual void sub_1145BC18();
	virtual int OpenConnection(int num, IUnknown** connection);//connect?
	virtual int DisconnectConnections();//no this
	virtual int GetConnections(IList** list);
	virtual int GetNrOfConnections(int* num);
	virtual void sub_1145BEB8();
	virtual void SubscribeToDataCounterEvents();
	virtual void UnsubscribeFromDataCounterEvents();
	virtual void sub_1145BFBC();
	virtual void SubscribeToStatusEvents();
	virtual void UnubscribeFromoStatusEvents();
	virtual void SubscribeToAuthenticationEvents();
	virtual void UnsubscribeFromAuthenticationEvents();
	virtual void sub_1145C18C();
	virtual void sub_1145C1A8();
};


class IInternetAccountManager_old2010: public IUnknown
{
public:
	virtual void CreatePSAccount();
	virtual void CreateCSAccount();
	virtual void CreateBTAccount();
	virtual void CreateBTPanAccount();
	//virtual void CreateUSBInetAccount();//
	virtual void CreateDialInAccount();

	virtual int OpenAccount(int num, IInternetAccount_Common** account);
	virtual int DestroyAccounts(void);//no this
	virtual int GetAccounts(IList** list);
	virtual int GetNrOfAccounts(int* num);
	virtual void sub_1145BC18();
	virtual int OpenConnection(int num, IUnknown** connection);//connect?
	virtual int DisconnectConnections();//no this
	virtual int GetConnections(IList** list);
	virtual int GetNrOfConnections(int* num);
	virtual void sub_1145BEB8();
	virtual void SubscribeToDataCounterEvents();
	virtual void UnsubscribeFromDataCounterEvents();
	virtual void sub_1145BFBC();
	virtual void SubscribeToStatusEvents();
	virtual void UnubscribeFromoStatusEvents();
	virtual void SubscribeToAuthenticationEvents();
	virtual void UnsubscribeFromAuthenticationEvents();
	virtual void sub_1145C18C();
	virtual void sub_1145C1A8();
};

#endif
