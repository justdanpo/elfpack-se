#include "mybook.h"

#define MYBOOKNAME "musicservertest"
#define MYBOOKBASEPAGE "musicservertest"
#define ABOUTTEXT "musicservertest"


CCBMusicServerBase::CCBMusicServerBase() : ICBMusicServer()
{
	elf_addref();
}

int CCBMusicServerBase::QueryInterface(UUID* id, void** ret)
{
	*ret = NULL;
	if( !memcmp(id,&IID_ICBMusicServer,16) )
	{
		AddRef();
		*ret = this;
		return S_OK;
	}
	return ICBMusicServer::QueryInterface(id,ret);
}

int CCBMusicServerBase::Release()
{
	elf_release();
	return ICBMusicServer::Release();
}

int CCBMusicServerBase::AddRef()
{
	elf_addref();
	return ICBMusicServer::AddRef();
}


void CCBMusicServerBase::OnInitiated( void* statusCode, int nbrOfItems, bool random, bool loop, void* clientData )
{
	debug_printf("\n=)  callback %s status %d nbrofitems %d random %d loop %d\n",__FUNCTION__, statusCode, nbrOfItems, random, loop);
}
void CCBMusicServerBase::OnDestroy( void* clientData ){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnDestroyed( void* clientData)
{
	debug_printf("\n=)  callback %s\n",__FUNCTION__);
}
void CCBMusicServerBase::OnStopped( int cause, int time, void* clientData ){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnStateChanged( int state, int elapsedTime, void* clientData){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnTimeFeedback( int elapsedTime, void* clientData){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnAudioMuted( int muteReason, void* clientData){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnNbrOfItemsChanged( int nbrOfItems, void* clientData, void* result){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnItemChanged(
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
									   )
{ debug_printf("\n=)  callback %s\n",__FUNCTION__); }

void CCBMusicServerBase::OnApplicationStarted( bool started, void* clientData ){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }
void CCBMusicServerBase::OnError( void* errorCode, void* clientData){ debug_printf("\n=)  callback %s\n",__FUNCTION__); }





int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
	book->Free();
	return 1;
}

int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
	MessageBox(EMPTY_TEXTID, STR(ABOUTTEXT), NOIMAGE, 1 ,5000, *pbookmanbook);
	return 1;
}


DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, MYBOOKBASEPAGE)
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END


CMyBook::CMyBook()  :CBook(MYBOOKNAME, &base_page),
pserver(NULL), callback(NULL), subshandle(NULL)
{
	elf_addref();

	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter, this, PAGE_HOOK_ADD_AFTER);
	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _HookCB_PageExit, this, PAGE_HOOK_ADD_BEFORE);

	if(FindBookEx( FindBookExCB_Name, (int*)"MusicApplication_Book" ))
		createmusicserver();
}


CMyBook::~CMyBook()
{
	MODIFYUIPAGEHOOK(PAGE_ENTER_EVENT, _HookCB_PageEnter, this, PAGE_HOOK_REMOVE);
	MODIFYUIPAGEHOOK(PAGE_EXIT_EVENT, _HookCB_PageExit, this, PAGE_HOOK_REMOVE);

	killmusicserver();

	elf_release();
}


int CMyBook::FindBookExCB_Name( BOOK* book , int* param )
{
	return 0 == strcmp( book->xbook->name, (char*)param );
}

int CMyBook::FindBookExCB_Book( BOOK* book , int* param )
{
	return book == (BOOK*)param;
}


CMyBook* CMyBook::GetExisting()
{
	return (CMyBook*) FindBookEx( FindBookExCB_Name, (int*)MYBOOKNAME );
}


int CMyBook::_HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB_PageEnter( msg, book, page_desc );
}

int CMyBook::_HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc, CMyBook* mybook, u16 event)
{
	return mybook->HookCB_PageExit( msg, book, page_desc );
}



int CMyBook::HookCB_PageEnter(void *msg, BOOK* book, PAGE_DESC * page_desc)
{
	bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );
	if(exists)
	{
		if( (!strcmp(book->xbook->name,"MusicApplication_Book") && !strcmp(page_desc->name,"MusicApplication_Main_Page")) )
		{
			createmusicserver();
		}
	}

	return 0;
}


int CMyBook::HookCB_PageExit(void *msg, BOOK* book, PAGE_DESC * page_desc)
{
	bool exists = NULL!=FindBookEx( FindBookExCB_Book, (int*) book );
	if(exists)
	{
		if( (!strcmp(book->xbook->name,"MusicApplication_Book") && !strcmp(page_desc->name,"MusicApplication_Main_Page")) )
		{
			killmusicserver();
		}
	}
	return 0;
}

void CMyBook::killmusicserver()
{
	if(pserver)
	{
		if(subshandle)
			pserver->Unsubscribe( subshandle );

//		pserver->Destroy();
		pserver->Release();
		pserver=NULL;
	}

	if(callback)
	{
		callback->Release();
		callback=NULL;
	}
}

void CMyBook::createmusicserver()
{
	IShell* pshell=0;
	OSE_GetShell( PPINTERFACE(&pshell));
	if(pshell)
	{
		static UUID IID_IMusicServer_Manager={0xAE, 0x67, 0x65, 0x71, 0xB3, 0x99, 0x44, 0xBE, 0xA5, 0xF4, 0xE6, 0x96, 0xCA, 0x63, 0x18, 0x6B};
		static UUID CID_CMusicServer_Manager={9, 0xE5, 0x76, 0x91, 0x7F, 0x36, 0x4B, 0x59, 0x82, 0xB9, 0x89, 0x19, 0xEA, 0x18, 0xB6, 0xF8};

		IMusicServerManager* obj1=NULL;
		pserver=NULL;
		CoCreateInstance(&CID_CMusicServer_Manager,&IID_IMusicServer_Manager,PPINTERFACE(&obj1));
		if(obj1)
		{
			if( (S_OK == obj1->CreateServer( pshell, &pserver )) && pserver)
			{
				callback = new CCBMusicServerBase();

//				if( S_OK == pserver->Start( NULL, 0, 0 ) )
				{
					if( S_OK != pserver->Subscribe(callback, 0, 0, &subshandle, NULL) )
					{
						debug_printf("\n!!! can't subscribe\n");
					}
				}
			}else
			{
				debug_printf("\n!!! musicserver error\n");
			}

			obj1->Release();
		}else
		{
			debug_printf("\n!!! musicservermanager error\n");
		}

		pshell->Release();
	}else
	{
		debug_printf("\n!!! pshell error\n");
	}
}
