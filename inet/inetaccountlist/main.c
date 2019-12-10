#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "..\common\IUnknown.hpp"
#include "..\common\InternetAccount.hpp"

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


//typedef void (*OSE_GetShell_t)(IShell**);
//OSE_GetShell_t OSE_GetShell=(OSE_GetShell_t)0x10E80331;

wchar_t log[16384];
int logpos = 0;

class CInternetAccountManager
{
	IInternetAccountManager2020* pam2020;
	IInternetAccountManager_old2010* pam_old2010;
	IInternetAccountManager* pam;

#define INTERNETACCOUNTMANAGERMETHOD(method,...) if(pam_old2010) return pam_old2010->method(__VA_ARGS__); if(pam2020) return pam2020->method(__VA_ARGS__); if(pam)return pam->method(__VA_ARGS__); return E_FAIL;
#define INTERNETACCOUNTMANAGERMETHOD0(method) if(pam_old2010) return pam_old2010->method(); if(pam2020) return pam2020->method(); if(pam)return pam->method(); return E_FAIL;

public:
	CInternetAccountManager(): pam(NULL), pam2020(NULL), pam_old2010(NULL)
	{
		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager_old2010, PPINTERFACE(&pam_old2010));
		}

		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager_2010, PPINTERFACE(&pam_old2010));
		}

		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager2020, PPINTERFACE(&pam2020));
		}

		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager, PPINTERFACE(&pam));
		}

		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager2, PPINTERFACE(&pam));
		}

		if (!pam2020 && !pam_old2010 && !pam)
		{
			CoCreateInstance(&CID_CInternetAccountManager, &IID_IInternetAccountManager3, PPINTERFACE(&pam));
		}
	}
	~CInternetAccountManager()
	{
		if (pam)
		{
			pam->Release();
		}

		if (pam_old2010)
		{
			pam_old2010->Release();
		}

		if (pam2020)
		{
			pam2020->Release();
		}
	}
	//    virtual void CreatePSAccount();
	//    virtual void CreateCSAccount();
	//    virtual void CreateBTAccount();
	//    virtual void CreateBTPanAccount();
	//    virtual void CreateUSBInetAccount();
	//    virtual void CreateDialInAccount();
	int OpenAccount(int num, IInternetAccount_Common** account)
	{
		INTERNETACCOUNTMANAGERMETHOD(OpenAccount, num, account);
	}
	int DestroyAccounts(void)//no this
	{
		INTERNETACCOUNTMANAGERMETHOD0(DestroyAccounts);
	}
	int GetAccounts(IList** list)
	{
		INTERNETACCOUNTMANAGERMETHOD(GetAccounts, list);
	}
	int GetNrOfAccounts(int* num)
	{
		INTERNETACCOUNTMANAGERMETHOD(GetNrOfAccounts, num);
	}
	//    virtual void sub_1145BC18();
	int OpenConnection(int num, IUnknown** connection)
	{
		INTERNETACCOUNTMANAGERMETHOD(OpenConnection, num, connection);
	}
	int DisconnectConnections()//no this
	{
		INTERNETACCOUNTMANAGERMETHOD0(DisconnectConnections);
	}
	int GetConnections(IList** list)
	{
		INTERNETACCOUNTMANAGERMETHOD(GetConnections, list);
	}
	int GetNrOfConnections(int* num)
	{
		INTERNETACCOUNTMANAGERMETHOD(GetNrOfConnections, num);
	}
	//    virtual void sub_1145BEB8();
	//    virtual void SubscribeToDataCounterEvents();
	//    virtual void UnsubscribeFromDataCounterEvents();
	//    virtual void sub_1145BFBC();
	//    virtual void SubscribeToStatusEvents();
	//    virtual void UnubscribeFromoStatusEvents();
	//    virtual void SubscribeToAuthenticationEvents();
	//    virtual void UnsubscribeFromAuthenticationEvents();
};


int main(void)
{
	log[0] = 0;

	CInternetAccountManager am;

	int x = 0;

	if (am.GetNrOfAccounts(&x) >= 0)
	{
		//logpos+=snwprintf(&log[logpos],256,_T("%d accounts\n"),x);

		IList* list;

		if (am.GetAccounts(&list) >= 0 && list)
		{

			for (int i = 0; i < x; i++)
			{
				int* paccountnum;
				list->Get(i, (TCollectionElement**)&paccountnum);

				if (paccountnum)
				{
					IInternetAccount_Common* pAccount = 0;
					am.OpenAccount(*paccountnum, &pAccount);

					if (pAccount)
					{
						wchar_t tmp[64];
						pAccount->GetName(tmp);
						logpos += snwprintf(&log[logpos], 256, _T("%x: %ls\n"), *paccountnum, tmp);

						pAccount->Release();
					}
					else
					{
						logpos += snwprintf(&log[logpos], 256, _T("can't open %x\n"), *paccountnum);
					}
				}
			}

			list->Release();
		}

	}
	else
	{
		logpos += snwprintf(&log[logpos], 256, _T("wrong accountmanager?\n"));
	}

	if (am.GetNrOfConnections(&x) >= 0)
	{
		IList* list;

		am.GetConnections(&list);

		for (int i = 0; i < x; i++)
		{
			int* paccountnum = 0;
			list->Get(i, (TCollectionElement**)&paccountnum);

			if (paccountnum)
			{
				int pAccount = 0;
				am.OpenConnection(*paccountnum, (IUnknown**)&pAccount);
				logpos += snwprintf(&log[logpos], 256, _T("conn %x, %x %x\n"), *paccountnum, pAccount, *(long*)pAccount);
			}
		}

		list->Release();
	}

	MessageBox(0x6FFFFFFF, TextID_Create(log, ENC_UCS2, logpos), 0xFFFF, 1, 0, 0);



	SUBPROC(elf_exit);
	return (0);
}
