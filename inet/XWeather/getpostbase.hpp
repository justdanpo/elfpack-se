#ifndef _GETPOSTBASE_HPP_
#define _GETPOSTBASE_HPP_

#include "..\common\IUnknown.hpp"
#include "..\common\GetPost.hpp"

class CGetPostBase : public ICBGetPostRequest
{
protected:
	IGetPostRequest* preq;//мож метод сделать? типа int GetGetPostRequest(IGetPostRequest** dest);

	void OnResult(int DataLength, int HeaderLength, int TotalSize, char MoreData, short StatusCode, int ClientData);
	void OnProgress(int CurrPos, int TotSize, int, int status);
	void OnSupplyPassword();
	void OnSendDataReady();

	~CGetPostBase();
public:
	int QueryInterface(UUID* id, void** ret);
	int Release();
	int AddRef();

	CGetPostBase();

	int Get(char* url, char* headers);
};


#endif //_GETPOSTBASE_HPP_
