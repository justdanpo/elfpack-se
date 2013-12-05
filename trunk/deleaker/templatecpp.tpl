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

