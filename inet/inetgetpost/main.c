#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "..\common\IUnknown.hpp"
#include "..\common\IShell.hpp"
#include "..\common\GetPost.hpp"

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

//класс-колбэк. вызывается по приёму
class MyCB : public ICBGetPostRequest
{
public:
	IGetPostRequest* preq;//!!!!!!!!

	int QueryInterface(UUID* id, void** ret)
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


	MyCB() : ICBGetPostRequest(), preq(NULL)
	{
	}

	//вроде там много больше аргументов
	virtual void OnResult(int DataLength, int HeaderLength, char MoreData, short StatusCode, int ClientData)
	{
		char* temp;

		if (HeaderLength)
		{
			temp = new char[HeaderLength + 1];
			memset(temp, 0, HeaderLength + 1);
			int i3 = 0, i4 = 0;

			if (preq->ReadHeader(HeaderLength, temp, &i3, &i4) >= 0)
			{
				debug_printf("\n!!!!!! =) header i3/i4 %x %x s %s\n", i3, i4, temp);
				delay(300);
			}

			delete temp;
		}

		if (DataLength)
		{
			temp = new char[DataLength + 1];
			memset(temp, 0, DataLength + 1);
			int i3 = 0, i4 = 0;

			if (preq->ReadData(DataLength, temp, &i3, &i4) >= 0)
			{
				debug_printf("\n!!!!!! =) data i3/i4 %x %x s %s\n", i3, i4, temp);
				delay(300);
			}

			delete temp;
		}
	}

	virtual void OnProgress(int CurrPos, int TotSize, int, int status)
	{
		debug_printf("\n!!!!!!!!!! %s\n", __FUNCTION__);
	}

	virtual void OnSupplyPassword()
	{
		debug_printf("\n!!!!!!!!!! %s\n", __FUNCTION__);
	}
	virtual void OnSendDataReady()
	{
		debug_printf("\n!!!!!!!!!! %s\n", __FUNCTION__);
	}

};


//typedef void (*OSE_GetShell_t)(IShell**);
//OSE_GetShell_t OSE_GetShell=(OSE_GetShell_t)0x10E80331;

wchar_t log[16384];
int logpos = 0;

int main(void)
{

	log[0] = 0;

	IShell* pshell = 0;

	OSE_GetShell(PPINTERFACE(&pshell));

	if (pshell)
	{

		IGetPostManager* pgetpostmanager = 0;
		CoCreateInstance(&CID_IGetPostManager, &IID_IGetPostManager, PPINTERFACE(&pgetpostmanager));

		if (pgetpostmanager)
		{
			IGetPostUtility* pgetpostutility = 0;
			pgetpostmanager->CreateUtility(pshell, &pgetpostutility);

			if (pgetpostutility)
			{
				logpos += snwprintf(&log[logpos], 256, _T("pgetpostutility %x\n"), *(long*)pgetpostutility);

				pgetpostutility->Release();
			}

			IGetPostSession* pgetpostsession = 0;
			pgetpostmanager->CreateSession(pshell, &pgetpostsession);

			if (pgetpostsession)
			{
				logpos += snwprintf(&log[logpos], 256, _T("pgetpostsession %x\n"), *(long*)pgetpostsession);

				MyCB* cb = NULL;
				cb = new MyCB();

				IGetPostRequest* preq = NULL;
				pgetpostsession->CreateRequest(cb, NULL, NULL, &preq);

				if (preq)
				{
					preq->AddRef();
					cb->preq = preq;

					logpos += snwprintf(&log[logpos], 256, _T("preq %x\n"), *(long*)preq);

					if (preq->Get("http://justdanpo.dyndns.org", "Accept: text/html") >= 0)
					{
						logpos += snwprintf(&log[logpos], 256, _T("get ok\n"));
					}

					//preq->Release();
				}

				//if(cb)cb->Release();

				//pgetpostsession->Release();
			}


			//pgetpostmanager->Release();
		}

		pshell->Release();
	}
	else
	{
		logpos += snwprintf(&log[logpos], 256, _T("pshell == NULL"));
	}

	MessageBox(0x6FFFFFFF, Str2ID(log, 0, logpos), 0xFFFF, 1, 0, 0);





	//    SUBPROC(elf_exit);
	return (0);
}
