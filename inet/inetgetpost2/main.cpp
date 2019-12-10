#include "..\..\include\Lib_Clara.h"
#include "..\..\include\Dir.h"

#include "elfcount.h"
#include "getpostbase.hpp"


class MyGetPost: public CGetPostBase
{
	void OnResult(int DataLength, int HeaderLength, int TotalSize, char MoreData, short StatusCode, int ClientData)
	{
		debug_printf("\n!!! onresult %x %x %x %x status %d error %d!\n", DataLength, HeaderLength, TotalSize, MoreData, StatusCode, ClientData);

		char* temp;

		if(HeaderLength)
		{
			temp = new char[HeaderLength+1];
			memset(temp,0,HeaderLength+1);
			int i3=0,i4=0;
			if(preq->ReadHeader(HeaderLength, temp, &i3, &i4)>=0)
			{
				debug_printf("\n!!!!!! =) header i3/i4 %x %x s %s\n",i3,i4,temp);
			}
			delete temp;
		}
		if(DataLength)
		{
			temp = new char[DataLength+1];
			memset(temp,0,DataLength+1);
			int i3=0,i4=0;
			if(preq->ReadData(DataLength, temp, &i3, &i4)>=0)
			{
				debug_printf("\n!!!!!! =) data i3/i4 %x %x s %s\n",i3,i4,temp);

				MessageBox(EMPTY_TEXTID,TextID_Create(temp, ENC_GSM, DataLength),NOIMAGE,1,0,0);
			}
			delete temp;

			//убьём себя после приёма данных. эльф тоже умрёт.
			Release();
		}

		//нет больше данных - убиваемся
		if(!MoreData)
		{
			Release();
		}
	}

	void OnProgress(int CurrPos, int TotSize, int, int status)
	{
		debug_printf("\n!!!!!!!!!! %s\n",__FUNCTION__);
	}
};


int main (void)
{
	MyGetPost* mygp = new MyGetPost(); //тестовый объект убьёт себя сам после приёма данных

	if( NULL != mygp && 0 <= mygp->Get( "http://justdanpo.ru", "Accept: text/html" ) )
	{
		debug_printf("\n!!!!!!!!!! %s: get ok\n",__FUNCTION__);
	}else
	{
		debug_printf("\n!!!!!!!!!! %s: get error\n",__FUNCTION__);
		mygp->Release();
	}


	//завершение работы эльфа. реально эльф умрёт, если живых колбэков в памяти не останется
	elf_release();
}
