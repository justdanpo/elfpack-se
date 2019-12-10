#include "getpostbase.hpp"
#include "..\common\IShell.hpp"
#include "elfcount.h"

void CGetPostBase::OnResult(int DataLength, int HeaderLength, int TotalSize, char MoreData, short StatusCode, int ClientData)
{
}

void CGetPostBase::OnProgress(int CurrPos, int TotSize, int, int status)
{
}

void CGetPostBase::OnSupplyPassword()
{
}

void CGetPostBase::OnSendDataReady()
{
}

int CGetPostBase::QueryInterface(UUID* id, void** ret)
{
	*ret = NULL;

	if (!memcmp(id, &IID_ICBGetPostRequest, sizeof(UUID)))
	{
		AddRef();
		*ret = this;
		return S_OK;
	}

	return ICBGetPostRequest::QueryInterface(id, ret);
}

int CGetPostBase::Release()
{
	elf_release();
	return ICBGetPostRequest::Release();
}

int CGetPostBase::AddRef()
{
	elf_addref();
	return ICBGetPostRequest::AddRef();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
CGetPostBase::CGetPostBase() : ICBGetPostRequest(), preq(NULL)
{
	elf_addref();

	IShell* pshell = 0;
	OSE_GetShell(PPINTERFACE(&pshell));

	if (pshell)
	{
		IGetPostManager* pgetpostmanager = 0;
		CoCreateInstance(&CID_IGetPostManager, &IID_IGetPostManager, PPINTERFACE(&pgetpostmanager));

		if (pgetpostmanager)
		{
			IGetPostSession* pgetpostsession = 0;
			pgetpostmanager->CreateSession(pshell, &pgetpostsession);

			if (pgetpostsession)
			{
				switch (GetChipID()&CHIPID_MASK)
				{
				case CHIPID_DB2020:
				{
					IGetPostSession_DB2020* pgetpostsession2020 = (IGetPostSession_DB2020*)pgetpostsession;


					static UUID IID_4584E594 = {0xD1, 0xFF, 0x27, 0xD5, 6, 0x3A, 0x46, 0xA, 0xA0, 0x7A, 0xE8, 0xAE, 0xB0, 0x53, 0x77, 2};
					static UUID CID_4584B0F0 = {0xF3, 0xF6, 0x97, 0x25, 0x63, 0x70, 0x48, 0xCA, 0x88, 0x9F, 0x8D, 0xD5, 0xF0, 3, 0x21, 0x90};

					IUnknown* obj1;
					CoCreateInstance(&CID_4584B0F0, &IID_4584E594, PPINTERFACE(&obj1));

					PROFILENAME x =
					{
						_T("TELE2 GPRS")
					};

#define PREDEFINED( x ) x

					int tmp = 0;
					pgetpostsession2020->Setup(x,

											   //4504B81C                 PUSH    {R0-R3}
											   obj1, PREDEFINED(0), 3 , PREDEFINED(1) ,

											   //4504B812                 PUSH    {R0-R3}
											   PREDEFINED(2), PREDEFINED(0), PREDEFINED(1), PREDEFINED(0) ,

											   //4504B808                 PUSH    {R0-R3}
											   PREDEFINED(0), PREDEFINED(1), 14, "Browser" ,

											   //4504B7FE                 PUSH    {R0-R3}
											   "NetFront", "3.3", "Profile/MIDP-2.0 Configuration/CLDC-1.1", &tmp);

					//на а1 порядок параметров другой =(
					//pgetpostsession2020->SetIntParameter( 0, 60000 );

					int cr = pgetpostsession2020->CreateRequest(this, NULL, NULL, &preq);
					debug_printf("\n!!! createrequest returned %x, prequest=%x\n", cr, preq);
				}
				break;

				case CHIPID_DB3150:
				case CHIPID_DB3210:
				{
					int cr = pgetpostsession->CreateRequest(this, NULL, NULL, &preq);
					debug_printf("\n!!! createrequest returned %x, prequest=%x\n", cr, preq);
				}
				break;

				default:
					debug_printf("\n!!! unknown session structure\n");
				}

				pgetpostsession->Release();
			}

			pgetpostmanager->Release();
		}

		pshell->Release();
	}
}

CGetPostBase::~CGetPostBase()
{
	if (preq)
		if (0 == preq->Release())
		{
			preq = 0;
		}
}

int CGetPostBase::Get(char* url, char* headers)
{
	if (0 == preq)
	{
		return E_FAIL;
	}

	return preq->Get(url, headers);
}
