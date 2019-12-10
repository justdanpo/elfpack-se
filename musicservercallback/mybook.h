#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"

#include "elfcount.h"

#include "IUnknown.hpp"
#include "IShell.hpp"


static UUID IID_ICBMusicServer = {0x12,0x14,0x43,0x5D,0xCC,0xDC,0x4b,0xbf,0xBC,0x70,0x09,0x4C,0xD9,0x53,0xDC,0xD6};

class ICBMusicServer : public IUnknown
{
public:
	virtual void OnInitiated( void* statusCode, int nbrOfItems, bool random, bool loop, void* clientData )=NULL;
	virtual void OnDestroy( void* clientData )=NULL;
	virtual void OnDestroyed( void* clientData)=NULL;
	virtual void OnStopped( int cause, int time, void* clientData )=NULL;
	virtual void OnStateChanged( int state, int elapsedTime, void* clientData)=NULL;
	virtual void OnTimeFeedback( int elapsedTime, void* clientData)=NULL;
	virtual void OnAudioMuted( int muteReason, void* clientData)=NULL;
	virtual void OnNbrOfItemsChanged( int nbrOfItems, void* clientData, void* result)=NULL;
	virtual void OnItemChanged(
							   /*
							   [in] RVoid statusCode,
							   [in] FUint32 itemIndex,
							   [in] TMusicServer_Time resumePosition,
							   [in] TMusicServer_Action currentPlayerAction,
							   [in, size_is(lengthOfURI)] TChar* pURI,
							   [in] FUint32 lengthOfURI,
							   [in, size_is(lengthOfTitle)] TChar* pTitle,
							   [in] FUint32 lengthOfTitle,
							   [in, size_is(lengthOfArtist)] TChar* pArtist,
							   [in] FUint32 lengthOfArtist,
							   [in, size_is(lengthOfAlbum)] TChar* pAlbum,
							   [in] FUint32 lengthOfAlbum,
							   [in] TMusicServer_Time playLength,
							   [in] TBool containsAlbumart,
							   [in] TBool isDRMProtected,
							   [in] TBool isRealMediaFile,
							   [in] TClientData clientData*/
							   )=NULL;
	virtual void OnApplicationStarted( bool started, void* clientData )=NULL;
	virtual void OnError( void* errorCode, void* clientData)=NULL;
};



class IMusicServer: public IUnknown
{
public:
	virtual int Start(ICBMusicServer* pICBMusicServer, int msgbase, void* clientData)=NULL;//,bool, int)=NULL;
	virtual int Destroy()=NULL;
	virtual int Subscribe( ICBMusicServer* pICBMusicServer, int msgbase, void* clientData, void** subscriptionhandle, void** )=NULL;
	virtual int Unsubscribe( void* subscriptionhandle )=NULL;
	virtual int StartApplication( int action );
	virtual int StopApplication();
};


class IMusicServerManager: public IUnknown
{
public:
	virtual int CreateServer(IShell* pshell, IMusicServer** dest);
	virtual void unk();
};


class CCBMusicServerBase : public ICBMusicServer
{
public:
	int QueryInterface(UUID* id, void** ret);
	int Release();
	int AddRef();

	CCBMusicServerBase();

	virtual void OnInitiated( void* statusCode, int nbrOfItems, bool random, bool loop, void* clientData );
	virtual void OnDestroy( void* clientData );
	virtual void OnDestroyed( void* clientData);
	virtual void OnStopped( int cause, int time, void* clientData );
	virtual void OnStateChanged( int state, int elapsedTime, void* clientData);
	virtual void OnTimeFeedback( int elapsedTime, void* clientData);
	virtual void OnAudioMuted( int muteReason, void* clientData);
	virtual void OnNbrOfItemsChanged( int nbrOfItems, void* clientData, void* result);
	virtual void OnItemChanged(
							   /*
							   [in] RVoid statusCode,
							   [in] FUint32 itemIndex,
							   [in] TMusicServer_Time resumePosition,
							   [in] TMusicServer_Action currentPlayerAction,
							   [in, size_is(lengthOfURI)] TChar* pURI,
							   [in] FUint32 lengthOfURI,
							   [in, size_is(lengthOfTitle)] TChar* pTitle,
							   [in] FUint32 lengthOfTitle,
							   [in, size_is(lengthOfArtist)] TChar* pArtist,
							   [in] FUint32 lengthOfArtist,
							   [in, size_is(lengthOfAlbum)] TChar* pAlbum,
							   [in] FUint32 lengthOfAlbum,
							   [in] TMusicServer_Time playLength,
							   [in] TBool containsAlbumart,
							   [in] TBool isDRMProtected,
							   [in] TBool isRealMediaFile,
							   [in] TClientData clientData*/
							   );
	virtual void OnApplicationStarted( bool started, void* clientData );
	virtual void OnError( void* errorCode, void* clientData);

};


class CMyBook: public CBook
{
	DECLARE_PAGE_DESC_MEMBER(base_page);

	IMusicServer* pserver;
	CCBMusicServerBase* callback;
	void* subshandle;

protected:
	~CMyBook();

	static int FindBookExCB_Name(BOOK* book ,int* param);
	static int FindBookExCB_Book(BOOK* book ,int* param);

	static int _HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
	static int _HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event);
	int HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc);
	int HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc);

public:
	CMyBook();

	void createmusicserver();
	void killmusicserver();

	static int TerminateElf(CBook** pbookmanbook, CMyBook* book);
	static int ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book);

	static CMyBook* GetExisting();
};

#endif
