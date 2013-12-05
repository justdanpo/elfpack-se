#ifdef NDEBUG

#include "..\include\Lib_Clara.h"

void* operator new(size_t sz){return malloc(sz);};
void* operator new[](size_t sz){return malloc(sz);};
void operator delete(void * p){mfree(p);};
void operator delete[](void * p){mfree(p);};

void* operator new(size_t size, void* p){ return p; }
void* operator new[](size_t size, void* p){ return p; }
void operator delete (void*, void*) { }
void operator delete[] (void*, void*) { }

#else

#include "mem2.h"
#include "..\include\dir.h"
#include "lib_clara_original.h"
#undef new
#undef delete


static char* leaktypes[]={
	"memory",
	"strid",
	"iconid",
	"timer",
	"file",
	"hook",
	"dll",
	"gc",
	"gvi",
	"gui",
	"book",
	"process",
	"ose buffer",
	"metadatadesc",
	"fileitemstruct",
	"w_dir",

	"unallocated"
};

static bool started=false;

static LIST* buffers[trace_typescount];

#define LISTDATACOUNT 3

static int iconid_start=0xE800;
static int iconid_end=0xE8FF;


enum
{
	trace_list_timers_id,
	trace_list_timers_proc,
	trace_list_file,
	trace_list_line,

	trace_list_size
};
static LIST* buffers2[trace_list_size];

static wchar_t logname[64]=_T("memory.txt");


void trace_init(wchar_t* arg_logname)
{
	if(arg_logname)
		wstrncpy(logname,arg_logname,MAXELEMS(logname));

	for(int i=0;i<trace_typescount;i++)
		buffers[i]=__original_List_Create();
	started=true;

	for(int i=0;i<trace_list_size;i++)
		buffers2[i]=__original_List_Create();

	int chipid = GetChipID() & CHIPID_MASK;

	if( chipid == CHIPID_DB2000 || chipid == CHIPID_DB2010 )
	{
		int max=0;
		int iconid;

		if( iconidname2id( _T("RN_SHORTCUT_LIST_CALL_LIST_ICN"), TEXTID_ANY_LEN, &iconid ) )
			if( max < iconid )
				max = iconid;
		if( iconidname2id( _T("ZERO_ROW_SEPARATOR_ICN"), TEXTID_ANY_LEN, &iconid ) )
			if( max < iconid )
				max = iconid;
		if( iconidname2id( _T("WV_YAHOO_STATUSBAR_SYSTEMMSG_ICN"), TEXTID_ANY_LEN, &iconid ) )
			if( max < iconid )
				max = iconid;

		if( max )
		{
			iconid_start = ( max + 0xFF ) & ~0xFF;
			iconid_end = 0xFFFE; //сомнительно
		}
	}
}

void trace_done()
{
	char tmp[256];

	started=false;

	BOOL detected=FALSE;

	__original_w_chdir(GetDir(DIR_OTHER|MEM_INTERNAL));
	int f=__original_w_fopen(logname,WA_Write+WA_Create+WA_Truncate,0x1FF,0);

	for(int memtype=0;memtype<trace_typescount;memtype++)
	{
		if(buffers[memtype]->FirstFree)
		{
			detected=TRUE;

			__original_w_fwrite(f,tmp,__original_sprintf(tmp,"leak type \"%s\"\n",leaktypes[memtype]));

			for(int j=0;j<buffers[memtype]->FirstFree;j+=LISTDATACOUNT)
			{
				__original_w_fwrite(f,tmp,
						__original_sprintf(tmp,"- %s:%d (%x)\n",
						__original_List_Get(buffers[memtype],j+1),//file
						__original_List_Get(buffers[memtype],j+2),//line
						__original_List_Get(buffers[memtype],j)//value
						)
					);
			}
		}
		__original_List_Destroy(buffers[memtype]);
	}

	if(!detected)
		__original_w_fwrite(f,tmp, __original_sprintf(tmp,"no leaks detected\n") );

	if(buffers2[trace_list_file]->FirstFree)
		__original_w_fwrite(f,tmp, __original_sprintf(tmp,"new/delete counter broken\n") );

	__original_w_fclose(f);

	while(buffers2[trace_list_timers_id]->FirstFree)
	{
		u16 timerid=(u16)(int)__original_List_RemoveAt(buffers2[trace_list_timers_id],0);
		__original_List_RemoveAt(buffers2[trace_list_timers_proc],0);
		__original_Timer_Kill(&timerid);//pervent reboots
	}

	for(int i=0;i<trace_list_size;i++)
		__original_List_Destroy(buffers2[i]);
}

void trace_alloc(int mt, void* ptr, const char* file, int line)
{
	__original_List_InsertLast(buffers[mt],ptr);
	__original_List_InsertLast(buffers[mt],(void*)file);
	__original_List_InsertLast(buffers[mt],(void*)line);
}

void* trace_alloc_ret(int mt, void* ptr, void* badvalue, const char* file, int line)
{
	if( ptr != badvalue )
		trace_alloc( mt, ptr, file, line);
	return ptr;
}


void trace_free(int mt,void* p, const char* file, int line)
{
	if(started)
	{
		bool found=false;
		for(int i=0;i<buffers[mt]->FirstFree;i+=LISTDATACOUNT)
		{
			if(__original_List_Get(buffers[mt],i)==p)
			{
				for(int j=0;j<LISTDATACOUNT;j++)
					__original_List_RemoveAt(buffers[mt],i);
				found=true;
				break;
			}
		}
		if(!found)
		{
			__original_List_InsertLast(buffers[trace_unallocated],(void*)p);
			__original_List_InsertLast(buffers[trace_unallocated],(void*)file);
			__original_List_InsertLast(buffers[trace_unallocated],(void*)line);
		}
	}
}


static int findtimercb(void* listitem, void* itemtofind)
{
	return ! (listitem==itemtofind);
}

void trace_timerkill(u16* timerid)
{
	int idx=__original_List_Find(buffers2[trace_list_timers_id],(void*)*timerid,findtimercb);
	if(idx!=LIST_ERROR)
	{
		__original_List_RemoveAt(buffers2[trace_list_timers_id],idx);
		__original_List_RemoveAt(buffers2[trace_list_timers_proc],idx);
	}
	__original_Timer_Kill(timerid);
}

static void trace_onTimer(u16 timerID,LPARAM lparam)
{
	int idx=__original_List_Find(buffers2[trace_list_timers_id],(void*)timerID,findtimercb);
	if(idx!=LIST_ERROR)
	{
		void(*onTimer)(u16,LPARAM)=(void(*)(u16,LPARAM))__original_List_Get(buffers2[trace_list_timers_proc],idx);
		onTimer(timerID,lparam);
		trace_timerkill(&timerID);
	}
}

u16 trace_timerset(int time, void(*onTimer)(u16 timerID,LPARAM lparam), LPARAM lparam)
{
	u16 ret=__original_Timer_Set(time,trace_onTimer,lparam);
	if(ret)
	{
		__original_List_InsertLast(buffers2[trace_list_timers_id],(void*)ret);
		__original_List_InsertLast(buffers2[trace_list_timers_proc],(void*)onTimer);
	}
	return ret;
}


bool isallocatedstrid(TEXTID strid)
{
	return (strid!=EMPTY_TEXTID) && ((strid&0xFFFF0000)!=0);
}

bool isallocatediconid(IMAGEID iconid)
{
	return iconid >= iconid_start && iconid <= iconid_end;
}


void __deleaker_pushfileline( const char* __file__, int __line__ )
{
	__original_List_InsertLast(buffers2[trace_list_file],(void*)__file__);
	__original_List_InsertLast(buffers2[trace_list_line],(void*)__line__);
}

void __deleaker_popfileline( const char*& __file__, int& __line__ )
{
	if(0==buffers2[trace_list_file]->FirstFree)
	{
		__file__="internal error";
		__line__=-1;
	}else
	{
		__file__=(char const*)__original_List_RemoveAt(buffers2[trace_list_file],buffers2[trace_list_file]->FirstFree-1);
		__line__=(int)__original_List_RemoveAt(buffers2[trace_list_line],buffers2[trace_list_line]->FirstFree-1);
	}
}

void operator delete(void* p)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	__deleaker_mfree( file, line, p );
}

void operator delete[](void* p)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	__deleaker_mfree( file, line, p );
}

void* operator new(size_t sz)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	return __deleaker_malloc( file, line, sz);
};

void* operator new[](size_t sz)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	return __deleaker_malloc( file, line, sz);
};

void operator delete (void*, void*)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
}

void operator delete[] (void*, void*)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
}

void* operator new(size_t size, void* p)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	return p;
}

void* operator new[](size_t size, void* p)
{
	const char* file;
	int line;
	__deleaker_popfileline(file,line);
	return p;
}

//---------------------------------------------------------------------------





void* __deleaker_malloc( const char* __file__, int __line__, int size )
{
	void* ret = __original_malloc(size);
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

void mfree_(void* p)
{
	mfree(p);
}

void* __deleaker_mfree_adr( const char* __file__, int __line__ )
{
	return (void*)mfree_;
}

void __deleaker_mfree( const char* __file__, int __line__, void* p )
{
	trace_free(trace_memory, p, __file__, __line__);
	__original_mfree(p);
}

int __deleaker_CreateBook( const char* __file__, int __line__, BOOK* pbook, void (*onClose)( BOOK* ), const PAGE_DESC* bp, const char* name, int ParentBookID, const APP_DESC* appdesc )
{
	trace_free(trace_memory, pbook, __file__, __line__);
	trace_alloc(trace_book, pbook, __file__, __line__);
	return __original_CreateBook(pbook, onClose, bp, name, ParentBookID, appdesc);
}

void __deleaker_FreeBook( const char* __file__, int __line__, BOOK* book )
{
	trace_free(trace_book, book, __file__, __line__);
	__original_FreeBook(book);
}

void __deleaker_BookObj_KillBook( const char* __file__, int __line__, BOOK* book )
{
	trace_free(trace_book, book, __file__, __line__);
	__original_BookObj_KillBook(book);
}

LIST* __deleaker_List_Create( const char* __file__, int __line__ )
{
	LIST* ret = __original_List_Create();
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_List_Destroy( const char* __file__, int __line__, LIST* lst )
{
	trace_free(trace_memory, lst, __file__, __line__);
	__original_List_Destroy(lst);
}

char* __deleaker_manifest_GetParam( const char* __file__, int __line__, const char* buf, const char* param_name, int unk )
{
	char* ret = __original_manifest_GetParam(buf, param_name, unk);
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

DIR_HANDLE* __deleaker_AllocDirHandle( const char* __file__, int __line__, const wchar_t* path )
{
	DIR_HANDLE* ret = __original_AllocDirHandle(path);
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_DestroyDirHandle( const char* __file__, int __line__, DIR_HANDLE* handle )
{
	trace_free(trace_memory, handle, __file__, __line__);
	__original_DestroyDirHandle(handle);
}

int __deleaker_GUIObject_Create( const char* __file__, int __line__, GUI* __unknwnargname1, void (*GuiDestroy)( GUI* ), void (*DispDescCreate)( DISP_DESC* ), BOOK* __unknwnargname4, void (*DispObjCallBack)( DISP_OBJ*, void* msg, GUI* ), int display, int size_of_gui )
{
	trace_free(trace_memory, __unknwnargname1, __file__, __line__);
	trace_alloc(trace_gui, __unknwnargname1, __file__, __line__);
	return __original_GUIObject_Create(__unknwnargname1, GuiDestroy, DispDescCreate, __unknwnargname4, DispObjCallBack, display, size_of_gui);
}

GUI* __deleaker_GUIObject_Destroy( const char* __file__, int __line__, GUI* __unknwnargname1 )
{
	trace_free(trace_gui, __unknwnargname1, __file__, __line__);
	return __original_GUIObject_Destroy(__unknwnargname1);
}

int __deleaker__fopen( const char* __file__, int __line__, const wchar_t* filpath, const wchar_t* filname, unsigned int mode, unsigned int rights, FILELISTITEM* destfname )
{
	int ret = __original__fopen(filpath, filname, mode, rights, destfname);
	if(ret!=-1)trace_alloc(trace_file, (void*)ret, __file__, __line__);
	return ret;
}

int __deleaker_fopen( const char* __file__, int __line__, const wchar_t* fname, int mode, int rights )
{
	int ret = __original_fopen(fname, mode, rights);
	if(ret!=-1)trace_alloc(trace_file, (void*)ret, __file__, __line__);
	return ret;
}

int __deleaker_fclose( const char* __file__, int __line__, int file )
{
	trace_free(trace_file, (void*)file, __file__, __line__);
	return __original_fclose(file);
}

int __deleaker_w_fopen( const char* __file__, int __line__, const wchar_t* name, int attr, int rights, int err )
{
	//тот же trace_file?
	int ret = __original_w_fopen(name, attr, rights, err);
	if(ret!=-1)trace_alloc(trace_file, (void*)ret, __file__, __line__);
	return ret;
}

int __deleaker_w_fclose( const char* __file__, int __line__, int f )
{
	trace_free(trace_file, (void*)f, __file__, __line__);
	return __original_w_fclose(f);
}

GUI_LIST* __deleaker_CreateListMenu( const char* __file__, int __line__, BOOK* __unknwnargname1, int display )
{
	GUI_LIST* ret = __original_CreateListMenu(__unknwnargname1, display);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

GUI_NOFMANY* __deleaker_CreateNOfMany( const char* __file__, int __line__, BOOK* book )
{
	GUI_NOFMANY* ret = __original_CreateNOfMany(book);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

GUI_ONEOFMANY* __deleaker_CreateOneOfMany( const char* __file__, int __line__, BOOK* book )
{
	GUI_ONEOFMANY* ret = __original_CreateOneOfMany(book);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

GUI_TABMENUBAR* __deleaker_CreateTabMenuBar( const char* __file__, int __line__, BOOK* book )
{
	GUI_TABMENUBAR* ret = __original_CreateTabMenuBar(book);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

GUI* __deleaker_CreateProgressBar( const char* __file__, int __line__, BOOK* book, int display )
{
	GUI* ret = __original_CreateProgressBar(book, display);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_TextID_Create( const char* __file__, int __line__, const void* wstr, TEXT_ENCODING flag, int len )
{
	TEXTID ret = __original_TextID_Create(wstr, flag, len);
	if(isallocatedstrid(ret))
	{
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
		if(flag==ENC_TEXTID)
		{
			for(int i=0;i<len;i++)
				trace_free(trace_strid, (void*)((TEXTID*)wstr)[i], __file__, __line__);
		}
	}
	return ret;
}

TEXTID __deleaker_TextID_Copy( const char* __file__, int __line__, TEXTID __unknwnargname1 )
{
	TEXTID ret = __original_TextID_Copy(__unknwnargname1);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_TextID_CreateIntegerID( const char* __file__, int __line__, int num )
{
	TEXTID ret = __original_TextID_CreateIntegerID(num);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_TextID_Destroy( const char* __file__, int __line__, TEXTID __unknwnargname1 )
{
	if(isallocatedstrid(__unknwnargname1))trace_free(trace_strid, (void*)__unknwnargname1, __file__, __line__ );
	return __original_TextID_Destroy(__unknwnargname1);
}

void __deleaker_GUIObject_SetTitleText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIObject_SetTitleText(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SoftKeys_SetText( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_GUIObject_SoftKeys_SetText(__unknwnargname1, actionID, __unknwnargname3);
}

char __deleaker_GUIonMessage_SetMenuItemText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIonMessage_SetMenuItemText(msg, __unknwnargname2);
}

char __deleaker_GUIonMessage_SetMenuItemSecondLineText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIonMessage_SetMenuItemSecondLineText(msg, __unknwnargname2);
}

char __deleaker_GUIonMessage_SetMenuItemInfoText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIonMessage_SetMenuItemInfoText(msg, __unknwnargname2);
}

char __deleaker_GUIonMessage_SetMenuItemUnavailableText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIonMessage_SetMenuItemUnavailableText(msg, __unknwnargname2);
}

int __deleaker_MessageBox( const char* __file__, int __line__, TEXTID header_text, TEXTID message_text, IMAGEID __unknwnargname3, int style, int time, BOOK* book )
{
	if(isallocatedstrid(header_text))trace_free(trace_strid, (void*)header_text, __file__, __line__ );
	if(isallocatedstrid(message_text))trace_free(trace_strid, (void*)message_text, __file__, __line__ );
	if(isallocatediconid(__unknwnargname3))trace_free(trace_iconid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_MessageBox(header_text, message_text, __unknwnargname3, style, time, book);
}

void __deleaker_BookObj_SoftKeys_SetText( const char* __file__, int __line__, BOOK* book, int actionID, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_BookObj_SoftKeys_SetText(book, actionID, __unknwnargname3);
}

void __deleaker_DataBrowserDesc_SetHeaderText( const char* __file__, int __line__, void* DataBrowserDesc, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_DataBrowserDesc_SetHeaderText(DataBrowserDesc, __unknwnargname2);
}

void __deleaker_DataBrowserDesc_SetOKSoftKeyText( const char* __file__, int __line__, void* DataBrowserDesc, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_DataBrowserDesc_SetOKSoftKeyText(DataBrowserDesc, __unknwnargname2);
}

void __deleaker_DispObject_SetTitleText( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_DispObject_SetTitleText(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SoftKeys_AddErrorStr( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_GUIObject_SoftKeys_AddErrorStr(__unknwnargname1, actionID, __unknwnargname3);
}

void __deleaker_ProgressBar_SetText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID text )
{
	if(isallocatedstrid(text))trace_free(trace_strid, (void*)text, __file__, __line__ );
	return __original_ProgressBar_SetText(__unknwnargname1, text);
}

void __deleaker_StatusIndication_Item8_SetText( const char* __file__, int __line__, TEXTID __unknwnargname1 )
{
	if(isallocatedstrid(__unknwnargname1))trace_free(trace_strid, (void*)__unknwnargname1, __file__, __line__ );
	return __original_StatusIndication_Item8_SetText(__unknwnargname1);
}

void __deleaker_StatusIndication_SetItemText( const char* __file__, int __line__, GUI* __unknwnargname1, int item, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_StatusIndication_SetItemText(__unknwnargname1, item, __unknwnargname3);
}

void __deleaker_StatusIndication_ShowNotes( const char* __file__, int __line__, TEXTID __unknwnargname1 )
{
	if(isallocatedstrid(__unknwnargname1))trace_free(trace_strid, (void*)__unknwnargname1, __file__, __line__ );
	return __original_StatusIndication_ShowNotes(__unknwnargname1);
}

void __deleaker_StringInput_DispObject_SetText( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_StringInput_DispObject_SetText(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SoftKeys_SetInfoText( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_GUIObject_SoftKeys_SetInfoText(__unknwnargname1, actionID, __unknwnargname3);
}

void __deleaker_ListMenu_SetItemSecondLineText( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, int elem_num, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_ListMenu_SetItemSecondLineText(__unknwnargname1, elem_num, __unknwnargname3);
}

void __deleaker_GUIonMessage_SubItem_SetText( const char* __file__, int __line__, GUI_MESSAGE* msg, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIonMessage_SubItem_SetText(msg, __unknwnargname2);
}

void __deleaker_Feedback_SetTextExtended( const char* __file__, int __line__, GUI_FEEDBACK* __unknwnargname1, TEXTID text, int where )
{
	if(isallocatedstrid(text))trace_free(trace_strid, (void*)text, __file__, __line__ );
	return __original_Feedback_SetTextExtended(__unknwnargname1, text, where);
}

void __deleaker_GUIObject_SoftKeys_SetTexts( const char* __file__, int __line__, GUI* __unknwnargname1, u16 actionID, TEXTID short_text, TEXTID full_text )
{
	if(isallocatedstrid(short_text))trace_free(trace_strid, (void*)short_text, __file__, __line__ );
	if(isallocatedstrid(full_text))trace_free(trace_strid, (void*)full_text, __file__, __line__ );
	return __original_GUIObject_SoftKeys_SetTexts(__unknwnargname1, actionID, short_text, full_text);
}

TEXTID __deleaker_PNUM2Name( const char* __file__, int __line__, BCD_TEXT pnum, int isIconNeeded, int only_full_number_flag )
{
	TEXTID ret = __original_PNUM2Name(pnum, isIconNeeded, only_full_number_flag);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_Date2ID( const char* __file__, int __line__, DATE* __unknwnargname1, int DateFormat, int __unknwnargname3 )
{
	TEXTID ret = __original_Date2ID(__unknwnargname1, DateFormat, __unknwnargname3);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_Time2ID( const char* __file__, int __line__, TIME* __unknwnargname1, char TimeFormat, int isSec )
{
	TEXTID ret = __original_Time2ID(__unknwnargname1, TimeFormat, isSec);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_KeyCode2Name( const char* __file__, int __line__, int key_code )
{
	TEXTID ret = __original_KeyCode2Name(key_code);
	if(isallocatedstrid(ret))trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_NOfMany_SetTexts( const char* __file__, int __line__, GUI_NOFMANY* __unknwnargname1, TEXTID* strids, int items_count )
{
	for(int i=0;i<items_count;i++)
		if(isallocatedstrid(strids[i]))trace_free(trace_strid, (void*)strids[i], __file__, __line__ );
	__original_NOfMany_SetTexts(__unknwnargname1, strids, items_count);
}

void __deleaker_OneOfMany_SetTexts( const char* __file__, int __line__, GUI_ONEOFMANY* __unknwnargname1, TEXTID* strids, int Count )
{
	for(int i=0;i<Count;i++)
		if(isallocatedstrid(strids[i]))trace_free(trace_strid, (void*)strids[i], __file__, __line__ );
	__original_OneOfMany_SetTexts(__unknwnargname1, strids, Count);
}

void __deleaker_Feedback_SetText( const char* __file__, int __line__, GUI_FEEDBACK* __unknwnargname1, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_Feedback_SetText(__unknwnargname1, __unknwnargname2);
}

int __deleaker_Gif2ID( const char* __file__, int __line__, u16 IMAGEHANDLE, const wchar_t* path, const wchar_t* fname, IMAGEID* __unknwnargname4 )
{//проверить код ошибки
	int ret = __original_Gif2ID(IMAGEHANDLE, path, fname, __unknwnargname4);
	if(ret)trace_alloc(trace_iconid, (void*)*__unknwnargname4, __file__, __line__);
	return ret;
}

void __deleaker_GUIObject_SetTitleIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_GUIObject_SetTitleIcon(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIInput_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_GUIInput_SetIcon(__unknwnargname1, __unknwnargname2);
}

void __deleaker_ProgressBar_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_ProgressBar_SetIcon(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIonMessage_SetMenuItemIcon( const char* __file__, int __line__, GUI_MESSAGE* msg, int align, IMAGEID __unknwnargname3 )
{
	if(isallocatediconid(__unknwnargname3))trace_free(trace_iconid, (void*)__unknwnargname3, __file__, __line__ );
	__original_GUIonMessage_SetMenuItemIcon(msg, align, __unknwnargname3);
}

void __deleaker_TabMenuBar_SetTabIcon( const char* __file__, int __line__, GUI_TABMENUBAR* __unknwnargname1, int tab, IMAGEID __unknwnargname3, int for_state )
{
	if(isallocatediconid(__unknwnargname3))trace_free(trace_iconid, (void*)__unknwnargname3, __file__, __line__ );
	__original_TabMenuBar_SetTabIcon(__unknwnargname1, tab, __unknwnargname3, for_state);
}

void __deleaker_VCALL_SetNameIcon( const char* __file__, int __line__, void* vc, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_VCALL_SetNameIcon(vc, __unknwnargname2);
}

int __deleaker_ModifyKeyHook( const char* __file__, int __line__, KEYHOOKPROC proc, int mode, LPARAM lparam )
{
	int ret = __original_ModifyKeyHook(proc, mode, lparam);
	if(mode==KEY_HOOK_REMOVE)trace_free(trace_hook, (void*)proc, __file__, __line__ );
	if(mode==KEY_HOOK_ADD)trace_alloc(trace_hook, (void*)proc, __file__, __line__);
	return ret;
}

int __deleaker_ModifyUIPageHook( const char* __file__, int __line__, int event, PAGEHOOKPROC proc, LPARAM ClientData, int mode )
{
	int ret = __original_ModifyUIPageHook(event, proc, ClientData, mode);
	if(mode==PAGE_HOOK_REMOVE)trace_free(trace_hook, (void*)proc, __file__, __line__ );
	if(mode==PAGE_HOOK_ADD_BEFORE || mode==PAGE_HOOK_ADD_AFTER)trace_alloc(trace_hook, (void*)proc, __file__, __line__);
	return ret;
}

int __deleaker_ImageID_Get( const char* __file__, int __line__, const wchar_t* fpath, const wchar_t* fname, IMAGEID* __unknwnargname3 )
{
	int ret = __original_ImageID_Get(fpath, fname, __unknwnargname3);
	if(ret>=0 && isallocatediconid(*__unknwnargname3))
		trace_alloc(trace_iconid, (void*)(*__unknwnargname3), __file__, __line__);
	return ret;
}

int __deleaker_ImageID_GetIndirect( const char* __file__, int __line__, void* buf_image, int size, int __NULL, wchar_t* image_type, IMAGEID* __unknwnargname5 )
{//проверить возвращаемое значение
	int ret = __original_ImageID_GetIndirect(buf_image, size, __NULL, image_type, __unknwnargname5);
	trace_free(trace_memory, buf_image, __file__, __line__);
	trace_alloc(trace_iconid, (void*)(*__unknwnargname5), __file__, __line__);
	return ret;
}

void __deleaker_ImageID_Free( const char* __file__, int __line__, IMAGEID __unknwnargname1 )
{
	if(isallocatediconid(__unknwnargname1))trace_free(trace_iconid, (void*)__unknwnargname1, __file__, __line__);
	__original_ImageID_Free(__unknwnargname1);
}

GC* __deleaker_GC_CreateMemoryGC( const char* __file__, int __line__, int xsize, int ysize, int bpp, int unk, void* somefn, int unk2 )
{
	GC* ret = __original_GC_CreateMemoryGC(xsize, ysize, bpp, unk, somefn, unk2);
	if(ret)trace_alloc(trace_gc, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_GC_FreeGC( const char* __file__, int __line__, GC* gc )
{
	trace_free(trace_gc, gc, __file__, __line__);
	__original_GC_FreeGC(gc);
}

GVI_PEN __deleaker_GVI_CreateDashedPen( const char* __file__, int __line__, char thikness, int color, int bitmask, int step, int offset )
{
	GVI_PEN ret = __original_GVI_CreateDashedPen(thikness, color, bitmask, step, offset);
	if(ret)trace_alloc(trace_gvi, (void*)ret, __file__, __line__);
	return ret;
}

GVI_PEN __deleaker_GVI_CreatePen( const char* __file__, int __line__, char thikness, int color )
{
	GVI_PEN ret = __original_GVI_CreatePen(thikness, color);
	if(ret)trace_alloc(trace_gvi, (void*)ret, __file__, __line__);
	return ret;
}

GVI_BRUSH __deleaker_GVI_CreateSolidBrush( const char* __file__, int __line__, int color )
{
	GVI_BRUSH ret = __original_GVI_CreateSolidBrush(color);
	if(ret)trace_alloc(trace_gvi, (void*)ret, __file__, __line__);
	return ret;
}

GVI_BMP __deleaker_GVI_CreateBitmap( const char* __file__, int __line__, int xsize, int ysize, int bpp )
{
	GVI_BMP ret = __original_GVI_CreateBitmap(xsize, ysize, bpp);
	if(ret)trace_alloc(trace_gvi, (void*)ret, __file__, __line__);
	return ret;
}

GVI_GC __deleaker_GVI_CreateMemoryGC( const char* __file__, int __line__, GVI_BMP bitmap )
{
	GVI_GC ret = __original_GVI_CreateMemoryGC(bitmap);
	if(ret)trace_alloc(trace_gvi, (void*)ret, __file__, __line__);
	return ret;
}

BOOL __deleaker_GVI_Delete_GVI_Object( const char* __file__, int __line__, GVI_OBJ* __unknwnargname1 )
{
	trace_free(trace_gvi, *__unknwnargname1, __file__, __line__);
	return __original_GVI_Delete_GVI_Object(__unknwnargname1);
}

void __deleaker_GVI_DeleteMemoryGC( const char* __file__, int __line__, GVI_GC gc )
{
	trace_free(trace_gvi, gc, __file__, __line__);
	return __original_GVI_DeleteMemoryGC(gc);
}

GUI_FEEDBACK* __deleaker_TextFeedbackWindow( const char* __file__, int __line__, BOOK* book, int zero )
{
	GUI_FEEDBACK* ret = __original_TextFeedbackWindow(book, zero);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

void* __deleaker_DataBrowserDesc_Create( const char* __file__, int __line__ )
{
	void* ret = __original_DataBrowserDesc_Create();
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_DataBrowserDesc_Destroy( const char* __file__, int __line__, void* DataBrowserDesc )
{
	trace_free(trace_memory, DataBrowserDesc, __file__, __line__);
	return __original_DataBrowserDesc_Destroy(DataBrowserDesc);
}

GUI_FEEDBACK* __deleaker_CreateMonitorFeedback( const char* __file__, int __line__, TEXTID __unknwnargname1, BOOK* __unknwnargname2, void (*onbusy)(BOOK*), void (*onedit)(BOOK*), void (*ondelete)(BOOK*) )
{
	GUI_FEEDBACK* ret = __original_CreateMonitorFeedback(__unknwnargname1, __unknwnargname2, onbusy, onedit, ondelete);
	if(ret)trace_alloc(trace_gui, (void*)ret, __file__, __line__);
	return ret;
}

void* __deleaker_LoadDLL( const char* __file__, int __line__, wchar_t* DllName )
{
	void* ret = __original_LoadDLL(DllName);
	if(ret)trace_alloc(trace_dll, (void*)ret, __file__, __line__);
	return ret;
}

int __deleaker_UnLoadDLL( const char* __file__, int __line__, void* DllData )
{
	trace_free(trace_dll, DllData, __file__, __line__);
	return __original_UnLoadDLL(DllData);
}

void __deleaker_GUIObject_SetSecondRowTitleText( const char* __file__, int __line__, GUI* __unknwnargname1, TEXTID __unknwnargname2 )
{
	if(isallocatedstrid(__unknwnargname2))trace_free(trace_strid, (void*)__unknwnargname2, __file__, __line__ );
	return __original_GUIObject_SetSecondRowTitleText(__unknwnargname1, __unknwnargname2);
}

void __deleaker_ListMenu_SetNoItemText( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, TEXTID str )
{
	if(isallocatedstrid(str))trace_free(trace_strid, (void*)str, __file__, __line__ );
	return __original_ListMenu_SetNoItemText(__unknwnargname1, str);
}

void __deleaker_TabMenuBar_SetTabTitle( const char* __file__, int __line__, GUI_TABMENUBAR* __unknwnargname1, int tab_num, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	return __original_TabMenuBar_SetTabTitle(__unknwnargname1, tab_num, __unknwnargname3);
}

void __deleaker_DispObject_SetBackgroundImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_DispObject_SetBackgroundImage(__unknwnargname1, __unknwnargname2);
}

void __deleaker_DispObject_SetCursorImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_DispObject_SetCursorImage(__unknwnargname1, __unknwnargname2);
}

void __deleaker_DispObject_SetTitleBackgroundImage( const char* __file__, int __line__, DISP_OBJ* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_DispObject_SetTitleBackgroundImage(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SetBackgroundImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_GUIObject_SetBackgroundImage(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SetCursorImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_GUIObject_SetCursorImage(__unknwnargname1, __unknwnargname2);
}

void __deleaker_GUIObject_SetTitleBackgroundImage( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_GUIObject_SetTitleBackgroundImage(__unknwnargname1, __unknwnargname2);
}

TEXTID __deleaker_Shortcut_Get_MenuItemName( const char* __file__, int __line__, void* __unknwnargname1 )
{
	TEXTID ret = __original_Shortcut_Get_MenuItemName(__unknwnargname1);
	trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_YesNoQuestion_SetIcon( const char* __file__, int __line__, GUI* __unknwnargname1, IMAGEID __unknwnargname2 )
{
	if(isallocatediconid(__unknwnargname2))trace_free(trace_iconid, (void*)__unknwnargname2, __file__, __line__ );
	__original_YesNoQuestion_SetIcon(__unknwnargname1, __unknwnargname2);
}

int __deleaker_ListMenu_SetItemIcon( const char* __file__, int __line__, GUI_LIST* __unknwnargname1, int Item, wchar_t unk_FFFF, int mode, IMAGEID __unknwnargname5 )
{
	//!!!unknown arg unk_FFFF
	if(isallocatediconid(__unknwnargname5))trace_free(trace_iconid, (void*)__unknwnargname5, __file__, __line__ );
	return __original_ListMenu_SetItemIcon(__unknwnargname1, Item, unk_FFFF, mode, __unknwnargname5);
}

IMAGEID __deleaker_Shortcut_Get_MenuItemIconID( const char* __file__, int __line__, void* __unknwnargname1 )
{
	IMAGEID ret = __original_Shortcut_Get_MenuItemIconID(__unknwnargname1);
	if(isallocatediconid(ret))trace_alloc(trace_iconid, (void*)ret, __file__, __line__);
	return ret;
}

PROCESS __deleaker_create_process( const char* __file__, int __line__, PROCESS_TYPE proc_type, char* name, OSENTRYPOINT* entrypoint, OSADDRESS stack_size, OSPRIORITY priority, OSTIME timeslice, PROCESS pid_block, void* redir_table, OSVECTOR vector, OSUSER user )
{
	PROCESS ret = __original_create_process(proc_type, name, entrypoint, stack_size, priority, timeslice, pid_block, redir_table, vector, user);
	trace_alloc(trace_process, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_kill_proc( const char* __file__, int __line__, PROCESS pid )
{
	trace_free(trace_process, (void*)pid, __file__, __line__);
	return __original_kill_proc(pid);
}

void __deleaker_MediaPlayer_SoftKeys_AddHelpStr( const char* __file__, int __line__, GUI* player_gui, int item, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	__original_MediaPlayer_SoftKeys_AddHelpStr(player_gui, item, __unknwnargname3);
}

void __deleaker_MediaPlayer_SoftKeys_SetText( const char* __file__, int __line__, GUI* player_gui, int actionID, TEXTID __unknwnargname3 )
{
	if(isallocatedstrid(__unknwnargname3))trace_free(trace_strid, (void*)__unknwnargname3, __file__, __line__ );
	__original_MediaPlayer_SoftKeys_SetText(player_gui, actionID, __unknwnargname3);
}

wchar_t* __deleaker_MenuBook_Desktop_GetSelectedItemID( const char* __file__, int __line__, BOOK* MenuBook_Desktop )
{
	wchar_t* ret = __original_MenuBook_Desktop_GetSelectedItemID(MenuBook_Desktop);
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

char* __deleaker_CreateURI( const char* __file__, int __line__, wchar_t* fpath, wchar_t* fname, char* URIScheme )
{
	char* ret = __original_CreateURI(fpath, fname, URIScheme);
	if(ret)trace_alloc(trace_memory, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_Timer_Kill( const char* __file__, int __line__, u16* timerID )
{
	trace_free(trace_timer, (void*)*timerID, __file__, __line__);
	trace_timerkill(timerID);
}

u16 __deleaker_Timer_Set( const char* __file__, int __line__, int time, TIMERPROC onTimer, LPARAM lparam )
{
	u16 ret = trace_timerset(time,(void(*)(u16,LPARAM))onTimer,(LPARAM)lparam);
	if(ret)trace_alloc(trace_timer, (void*)ret, __file__, __line__);
	return ret;
}

void __deleaker_Timer_ReSet( const char* __file__, int __line__, u16* timer, int time, TIMERPROC onTimer, LPARAM lparam )
{
	trace_free(trace_timer, (void*)*timer, __file__, __line__);
	trace_timerkill(timer);

	u16 ret = trace_timerset(time,(void(*)(u16,LPARAM))onTimer,(LPARAM)lparam);
	if(ret)trace_alloc(trace_timer, (void*)ret, __file__, __line__);
	*timer=ret;
}

union SIGNAL* __deleaker_alloc( const char* __file__, int __line__, OSBUFSIZE size, SIGSELECT signo )
{
	union SIGNAL* ret = __original_alloc(size, signo);
	if(ret)trace_alloc(trace_osebuff, ret, __file__, __line__);
	return ret;
}

union SIGNAL* __deleaker_receive( const char* __file__, int __line__, const SIGSELECT* sigsel )
{
	union SIGNAL* ret = __original_receive(sigsel);
	if(ret)trace_alloc(trace_osebuff, ret, __file__, __line__);
	return ret;
}

union SIGNAL* __deleaker_receive_w_tmo( const char* __file__, int __line__, OSTIME timeout, SIGSELECT* sel )
{
	union SIGNAL* ret = __original_receive_w_tmo(timeout, sel);
	if(ret)trace_alloc(trace_osebuff, ret, __file__, __line__);
	return ret;
}

void __deleaker_free_buf( const char* __file__, int __line__, union SIGNAL** sig )
{
	trace_free(trace_osebuff, *sig, __file__, __line__);
	__original_free_buf(sig);
}

void __deleaker_send( const char* __file__, int __line__, union SIGNAL** sig, PROCESS to )
{
	trace_free(trace_osebuff, *sig, __file__, __line__);
	__original_send(sig, to);
}

OSBOOLEAN __deleaker_hunt( const char* __file__, int __line__, const char* name, OSUSER user, PROCESS* name_, union SIGNAL** hunt_sig )
{
	if(hunt_sig)
		trace_free(trace_osebuff, *hunt_sig, __file__, __line__);
	return __original_hunt(name, user, name_, hunt_sig);
}

int __deleaker_JavaApp_LogoImageID_Get( const char* __file__, int __line__, wchar_t* fullpath, IMAGEID* __unknwnargname2 )
{
	int ret = __original_JavaApp_LogoImageID_Get(fullpath, __unknwnargname2);
	if(ret>=0 && isallocatediconid(*__unknwnargname2))
		trace_alloc(trace_iconid, (void*)(*__unknwnargname2), __file__, __line__);
	return ret;
}

void __deleaker_ObexSendFile( const char* __file__, int __line__, SEND_OBEX_STRUCT* __unknwnargname1 )
{
	if(isallocatedstrid(__unknwnargname1->send))trace_free(trace_strid, (void*)__unknwnargname1->send, __file__, __line__ );
	if(isallocatedstrid(__unknwnargname1->sent))trace_free(trace_strid, (void*)__unknwnargname1->sent, __file__, __line__ );
	__original_ObexSendFile(__unknwnargname1);
}

TEXTID __deleaker_JavaSession_GetName( const char* __file__, int __line__ )
{
	TEXTID ret = __original_JavaSession_GetName();
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

void* __deleaker_MetaData_Desc_Create( const char* __file__, int __line__, wchar_t* path, wchar_t* name )
{
	void* ret = __original_MetaData_Desc_Create(path, name);
	if(ret)trace_alloc(trace_metadatadesc, ret, __file__, __line__);
	return ret;
}

void __deleaker_MetaData_Desc_Destroy( const char* __file__, int __line__, void* MetaData_Desc )
{
	trace_free(trace_metadatadesc, MetaData_Desc, __file__, __line__);
	__original_MetaData_Desc_Destroy(MetaData_Desc);
}

FILEITEM* __deleaker_FILEITEM_Create( const char* __file__, int __line__ )
{
	FILEITEM* ret = __original_FILEITEM_Create();
	if(ret)trace_alloc(trace_fileitemstruct, ret, __file__, __line__);
	return ret;
}

FILEITEM* __deleaker_FILEITEM_CreateCopy( const char* __file__, int __line__, FILEITEM* __unknwnargname1 )
{
	FILEITEM* ret = __original_FILEITEM_CreateCopy(__unknwnargname1);
	if(ret)trace_alloc(trace_fileitemstruct, ret, __file__, __line__);
	return ret;
}

void __deleaker_FILEITEM_Destroy( const char* __file__, int __line__, FILEITEM* __unknwnargname1 )
{
	trace_free(trace_fileitemstruct, __unknwnargname1, __file__, __line__);
	__original_FILEITEM_Destroy(__unknwnargname1);
}

int __deleaker_w_dirclose( const char* __file__, int __line__, void* __unknwnargname1 )
{
	trace_free(trace_w_dir, __unknwnargname1, __file__, __line__);
	return __original_w_dirclose(__unknwnargname1);
}

void* __deleaker_w_diropen( const char* __file__, int __line__, const wchar_t* dir )
{
	void* ret = __original_w_diropen(dir);
	if(ret)trace_alloc(trace_w_dir, ret, __file__, __line__);
	return ret;
}

void* __deleaker_SoundRecorderDesc_Create( const char* __file__, int __line__ )
{
	void* ret = __original_SoundRecorderDesc_Create();
	if(ret)trace_alloc(trace_memory, ret, __file__, __line__);
	return ret;
}

void __deleaker_SoundRecorderDesc_Destroy( const char* __file__, int __line__, void* desc )
{
	trace_free(trace_memory, desc, __file__, __line__);
	__original_SoundRecorderDesc_Destroy(desc);
}

TEXTID __deleaker_AB_NAME_ITEM2TextID( const char* __file__, int __line__, AB_STR_ITEM* ab_name )
{
	TEXTID ret = __original_AB_NAME_ITEM2TextID(ab_name);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}

TEXTID __deleaker_AB_NUM_ITEM2TextID( const char* __file__, int __line__, AB_NUM_ITEM* ab_num_item )
{
	TEXTID ret = __original_AB_NUM_ITEM2TextID(ab_num_item);
	if(isallocatedstrid(ret))
		trace_alloc(trace_strid, (void*)ret, __file__, __line__);
	return ret;
}
//__swi __arm GUI* CreateDateInputVA( int zero, ... );
//__swi __arm GUI* CreatePercentInputVA( int zero, ... );
//__swi __arm GUI* CreateStringInputVA( int, ... );
//__swi __arm GUI* CreateTimeInputVA( int zero, ... );
//__swi __arm GUI* CreateYesNoQuestionVA( int zero, ... );
//__swi __arm LIST* DataBrowserBook_GetCurrentFoldersList( BOOK* DataBrowserBook );
//__swi __arm SUB_EXECUTE* DataBrowser_CreateSubExecute( int BookID, FILEITEM* );
//__swi __arm int DataBrowser_ExecuteSubroutine( SUB_EXECUTE* sub, int action, u16* unk );
//__swi __arm IMAGEID ImageID_Copy( IMAGEID );
//__swi __arm int JavaAppDesc_Get( int unk1, void** JavaDesc );
//__swi __arm int JavaAppDesc_GetFirstApp( void* JavaDesc );
//__swi __arm int JavaAppDesc_GetJavaAppFullpath( void* JavaDesc, JavaAppFullpath* );
//__swi __arm int JavaAppDesc_GetJavaAppID( void* JavaDesc );
//__swi __arm int JavaAppDesc_GetJavaAppInfo( void* JavaDesc, int ID, wchar_t** wstr );
//__swi __arm int JavaAppDesc_GetNextApp( void* JavaDesc );
//__swi __arm void JavaDialog_Close( int unk1 );
//__swi __arm int JavaDialog_Open( int unk1, char* unk2, void** JavaDesc );
//__swi __arm int MSG_SendMessage_AddRecipient( void*, wchar_t*, wchar_t*, int, int );
//__swi __arm int MSG_SendMessage_CreateMessage( int, void* );
//__swi __arm int MSG_SendMessage_DestroyMessage( void* );
//__swi __arm int MSG_SendMessage_Start( int, void*, int );
//__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_GETHANDLE( const int* sync, u16* ImageHandler, char* unk );
//__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_REGISTER( const int* sync, u16 ImageHandler, wchar_t* path, wchar_t* fname, int unk, IMAGEID*, char* error );
//__swi __arm int REQUEST_IMAGEHANDLER_INTERNAL_UNREGISTER( const int* sync, u16 ImageHandler, u16*, u16*, IMAGEID, int unk_1, char* error );
//__swi __arm int REQUEST_PHONEBOOK_ACCESSSTATUS_TOTAL_GET( const int* sync, int* );
//__swi __arm int REQUEST_PROFILE_GETPROFILENAME( const int* sync, int unk, TEXTID_DATA*, char* error );
//__swi __arm int Request_EventChannel_Subscribe( const int* sync, int mode, int event );
//__swi __arm int SoundRecorder_Create( void* desc );
//__swi __arm int SoundRecorder_RecordCall( BOOK* OngoingCallBook );
//__swi __arm int inflate( z_streamp strm, int flush );
//__swi __arm int inflateEnd( z_streamp strm );
//__swi __arm int inflateInit2_( z_streamp strm, int windowBits, const char* version, int stream_size );
//__swi __arm png_infop png_create_info_struct( png_structp png_ptr );
//__swi __arm png_structp png_create_read_struct_2( png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn );
//__swi __arm void png_destroy_info_struct( png_structp png_ptr, png_infopp info_ptr_ptr );
//__swi __arm void png_destroy_read_struct( png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr );
//__swi __arm png_uint_32 png_get_IHDR( png_structp png_ptr, png_infop info_ptr, png_uint_32* width, png_uint_32* height, int* bit_depth, int* color_type, int* interlace_method, int* compression_method, int* filter_method );
//__swi __arm void png_process_data( png_structp png_ptr, png_infop info_ptr, png_bytep buffer, png_size_t buffer_size );
//__swi __arm void png_progressive_combine_row( png_structp png_ptr, png_bytep old_row, png_bytep new_row );
//__swi __arm void png_read_update_info( png_structp png_ptr, png_infop info_ptr );
//__swi __arm void png_set_filler( png_structp png_ptr, png_uint_32 filler, int flags );
//__swi __arm void png_set_progressive_read_fn( png_structp png_ptr, png_voidp progressive_ptr, png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn, png_progressive_end_ptr end_fn );
//__swi __arm int png_sig_cmp( png_bytep sig, png_size_t start, png_size_t num_to_check );

#endif
