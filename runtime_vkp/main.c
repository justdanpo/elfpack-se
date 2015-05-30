#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "main.h"
#include "conf_loader.h"
#include "config_data.h"


char scanf_mask[]="%x";

LIST* patch_list;


extern "C" int InterruptsAndFastInterrupts_Off();
extern "C" void InterruptsAndFastInterrupts_Restore(int intrMask);
extern "C" int cp15_write_DAC(int mask);
extern "C" void patch_pcore_static_cache();
extern "C" void patch_pcore_nonstatic_cache();

//int (*CleanPageCache)(int size,int clean_all_flag)=(int(*)(int size,int clean_all_flag))0x48011329;
//void (*InitPageCache)(int min_size,int max_size)=(void(*)(int min_size,int max_size))0x48011669;

//================== pages start =====================

const PAGE_MSG vkp_vkpinpage_events[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG,	VKPInPage_EnterAction,
	PAGE_EXIT_EVENT_TAG,	VKPInPage_ExitAction,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC VKPService_VKP_In_Page = { "VKPService_VKP_In_Page", 0, vkp_vkpinpage_events };


const PAGE_MSG vkp_idlepage_events[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG,	IdlePage_EnterAction,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC VKPService_Idle_Page = { "VKPService_Idle_Page", 0, vkp_idlepage_events };


const PAGE_MSG vkp_basepage_events[]@ "DYN_PAGE" ={
	ELF_SHOW_INFO_EVENT,	ShowAuthorInfo,
	//ELF_RECONFIG_EVENT,		ReconfigElf,
	NIL_EVENT_TAG,				0
};

PAGE_DESC VKPService_BasePage ={"VKPService_BasePage",0,vkp_basepage_events};

//================== pages end ======================

void* List_Get_int(LIST* list, int index)
{
	if (index < list->FirstFree)
		return list->listdata[index];
	
	return 0;
}


int List_Find_pages_cmp(void *r0,void *r1)
{
	int i;
	int target_page_addr = (int)r1;
	
	patch_list_elem* patch = (patch_list_elem*)r0;
	for (i=0; i < patch->patch_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch->patch_data,i);
		
		if ((elem->virtAddr&PAGE_ALIGN_MASK) == target_page_addr) return 0;
	}
	
	return 1;
}


int List_Find_pagelist_cmp(void *r0,void *r1)
{
  if (r0==r1) return(0);
  return(1);
}


void vkp_elem_free(void * r0)
{
	vkp_list_elem * elem=(vkp_list_elem *)r0;
	if (elem)
	{
		if (elem->oldData)
			delete elem->oldData;
		
		if (elem->newData)
			delete elem->newData;
		
		delete(elem);
	}
}


void patch_list_elem_destroy(patch_list_elem* vkp)
{
	delete vkp->patch_name;
	List_DestroyElements(vkp->patch_data,0,vkp_elem_free);
	List_Destroy(vkp->patch_data);
	delete vkp;
}


void elf_exit(void)

{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int ShowAuthorInfo(void *mess ,BOOK * book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("VKP Runtime, v1.21\n\n(c) IronMaster"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


char * strchr(char * str,char c)
{
	for(;*str;str++)if(*str==c)return str;
	return NULL;
}


void memcpy_int(char* dest,char* source,int size)
{
	int i;
	for (i=0; i<size; i++)
	{
		dest[i] = source[i];
	}
}


extern "C" int check_static_after_map(int virtAddr,int physAddr)
{
	int i;
	int j;
	int ret = fs_memmap(virtAddr,physAddr,0x1000,FS_MEMMAP_NONBUFFERED|FS_MEMMAP_NOPERMISSIONS|FS_MEMMAP_CACHED|FS_MEMMAP_READ);
	
	for (i=0; i < patch_list->FirstFree; i++)
	{
		patch_list_elem* patch_elem = (patch_list_elem*)List_Get_int(patch_list,i);
		
		for (j=0; j < patch_elem->patch_data->FirstFree; j++)
		{
			vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_elem->patch_data,j);
			
			if ( elem->isStatic == STATIC && virtAddr == (elem->virtAddr&STATIC_PAGE_ALIGN_MASK) )
				memcpy_int((char*)elem->virtAddr,elem->newData,elem->dataSize);   //apply vkp
		}
	}
	
	return ret;
}


extern "C" int check_nonstatic_after_map(int virtAddr,int physAddr,char* need_to_lock)
{
	int i;
	int j;
	char lock=0;
	
	int ret = fs_memmap(virtAddr,physAddr,0x400,FS_MEMMAP_NONBUFFERED|FS_MEMMAP_NOPERMISSIONS|FS_MEMMAP_CACHED|FS_MEMMAP_READ);
	
	for (i=0; i < patch_list->FirstFree; i++)
	{
		patch_list_elem* patch_elem = (patch_list_elem*)List_Get_int(patch_list,i);
		
		for (j=0; j < patch_elem->patch_data->FirstFree; j++)
		{
			vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_elem->patch_data,j);
			
			if ( elem->isStatic == NOT_STATIC && virtAddr == (elem->virtAddr&PAGE_ALIGN_MASK) )
			{
				memcpy_int((char*)elem->virtAddr,elem->newData,elem->dataSize);   //apply vkp
				lock = 1;
			}
		}
	}
	
	*need_to_lock |=lock;
	
	return ret;
}


int get_page_i(VKPBook * vkp_book,int physAddr)
{
	int ret = 0xFFFF;
	int i;
	int max_pool_i = *vkp_book->fs_PageCacheMaxSize / POOL_SIZE;
	
	for (i=0;i<max_pool_i;i++)
	{
		pagePool* pool_p = vkp_book->PagePoolTbl+i;
		int physAddr_align = physAddr & PHYS_BASE_ADDR_MASK;
		if ( pool_p->baseAddr == physAddr_align || pool_p->baseAddr == (physAddr_align-0x10000) )
		{
			char pos_in_pool = (physAddr - pool_p->baseAddr) / PAGE_SIZE;
			ret = i*POOL_SIZE+pos_in_pool;

			return ret;
		}
	}
	return ret;
}


int check_conflict(VKPBook * vkp_book,LIST* patch_data)
{
	int i;
	int j;
	int k;
	int res = -1;
	
	//check if conflict
	for (i=0; i < patch_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_data,i);
		
		for (j=0; j < patch_list->FirstFree; j++)
		{
			patch_list_elem* patch_elem = (patch_list_elem*)List_Get_int(patch_list,j);
			
			for (k=0; k < patch_elem->patch_data->FirstFree; k++)
			{
				vkp_list_elem* patch_line = (vkp_list_elem*)List_Get_int(patch_elem->patch_data,k);
				
				if ( (elem->virtAddr == patch_line->virtAddr) || (elem->virtAddr < patch_line->virtAddr && (elem->virtAddr+elem->dataSize) > patch_line->virtAddr) || (elem->virtAddr > patch_line->virtAddr && elem->virtAddr < (patch_line->virtAddr+patch_line->dataSize)) )
				{
					res = j;
					break;
				}
			}
			if (res != -1) break;
		}
		if (res != -1) break;
	}
	
	return res;
}


VKP_CHECK_RESULT check_vkp_state(VKPBook * vkp_book,LIST* patch_data)
{
	VKP_CHECK_RESULT res;
	int i;
	int need_to_check;
	int cur_page_addr;
	
	int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	//check if not installed
	cur_page_addr=0;
	res = VKP_CHECK_RESULT_NOT_INSTALLED;
	for (i=0; i < patch_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_data,i);
		
		if (elem->cxc == RAM)
		{
			if (cur_page_addr != (elem->virtAddr&PAGE_ALIGN_MASK))
			{
				need_to_check=1;
				cur_page_addr = elem->virtAddr&PAGE_ALIGN_MASK;
				
				if (!fs_GetMemMap(cur_page_addr,0))
				{
					if (elem->area == MAIN)
					{
						fs_demand_cache_page(elem->virtAddr,2,intrMask);
					}
					else
						need_to_check=0;
				}
			}
		}
		//debug_printf("\r\nruntime_vkp: check if not installed, virtAddr =0x%08X",elem->virtAddr);
		//delay(20);
		if (need_to_check && memcmp((void*)elem->virtAddr,elem->oldData,elem->dataSize))
		{
			debug_printf("\r\nruntime_vkp: check if not installed error, virtAddr =0x%08X",elem->virtAddr);
			res = VKP_CHECK_RESULT_ERROR;
			break;
		}
	}
	
	if (res == VKP_CHECK_RESULT_ERROR)
	{
		//check if installed
		cur_page_addr=0;
		res = VKP_CHECK_RESULT_INSTALLED;
		for (i=0; i < patch_data->FirstFree; i++)
		{
			vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_data,i);
			
			if (elem->cxc == RAM)
			{
				if (cur_page_addr != (elem->virtAddr&PAGE_ALIGN_MASK))
				{
					cur_page_addr = elem->virtAddr&PAGE_ALIGN_MASK;
					
					if (!fs_GetMemMap(cur_page_addr,0))
					{
						if (elem->area == MAIN)
						{
							fs_demand_cache_page(elem->virtAddr,2,intrMask);
						}
						else
						{
							debug_printf("\r\nruntime_vkp: check if installed error: no physAddr, virtAddr =0x%08X",elem->virtAddr);
							res = VKP_CHECK_RESULT_ERROR;
							break;
						}
					}
				}
			}
			if (memcmp((void*)elem->virtAddr,elem->newData,elem->dataSize))
			{
				debug_printf("\r\nruntime_vkp: check if installed error, virtAddr =0x%08X",elem->virtAddr);
				res = VKP_CHECK_RESULT_ERROR;
				break;
			}
		}
	}
	
	cp15_write_DAC(old_dac_mask);
	InterruptsAndFastInterrupts_Restore(intrMask);
	
	return res;
}


void apply_vkp(VKPBook * vkp_book,LIST* patch_data)
{
	int i;
	int end_static_addr;
	int physAddr;
	wchar_t swap_i;
	int cur_page_addr=0;
	
	int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	for (i=0; i < patch_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_data,i);
		
		if (elem->cxc != RAM)
		{
			if (cur_page_addr != (elem->virtAddr&PAGE_ALIGN_MASK))
			{
				cur_page_addr = elem->virtAddr&PAGE_ALIGN_MASK;
				if (physAddr = fs_GetMemMap(cur_page_addr,0), physAddr==0 )    //if not already created
				{
					if (elem->area == MAIN)
					{
						fs_demand_cache_page(elem->virtAddr,2|NEED_TO_LOCK,intrMask);
					}
					else
					{
						if (swap_i = fs_demand_get_page_i_from_queue(vkp_book->SwappedOutFirst,0,0x20), swap_i == 0xFFFF)
						{
							swap_i = fs_demand_get_page_i_from_queue(vkp_book->SwappedInFirst,0,*vkp_book->NbrOfSwappedInPages);
							fs_demand_kick_out_page(swap_i,intrMask);
						}
						pageCache* page_p = vkp_book->PageCacheTbl+swap_i;
						pagePool* pool_p = vkp_book->PagePoolTbl+(swap_i/POOL_SIZE);
						
						page_p->virtAddr = cur_page_addr;
						if (!pool_p->baseAddr) fs_demand_pagePool_alloc_mem(pool_p,intrMask);
						pool_p->usedPages++;
						int physAddr_page = pool_p->baseAddr+((swap_i&0x7F)*PAGE_SIZE);
						fs_memmap(page_p->virtAddr, physAddr_page, PAGE_SIZE, FS_MEMMAP_WRITE|FS_MEMMAP_READ|FS_MEMMAP_CACHED|FS_MEMMAP_BUFFERED);
						memset((void*)physAddr_page,0,PAGE_SIZE);
						*vkp_book->NbrOfKickedOutPages -=1;
						fs_demand_remove_from_queue(page_p,0xFFFF);
						*vkp_book->NbrOfLockedInPages +=1;
					}
					delay(5);
				}
				else
					//memmap exists
				{
					if (elem->cxc == CXC_EMP) end_static_addr=vkp_book->emp_static_end_addr;
					else end_static_addr=vkp_book->app_static_end_addr;
					
					if (elem->virtAddr >= end_static_addr)
					{
						wchar_t kick_out_i = get_page_i(vkp_book,physAddr);
						pageCache* kick_out_p = vkp_book->PageCacheTbl+kick_out_i;
						
						//check if locked
						if ( kick_out_p->prev_i != 0xFFFF || kick_out_p->next_i != 0xFFFF)
						{
							fs_demand_kick_out_page(kick_out_i,intrMask);
							fs_demand_cache_page(elem->virtAddr,2|NEED_TO_LOCK,intrMask);
							delay(5);
						}
					}
				}
			}
		}
		
		memcpy_int((char*)elem->virtAddr,elem->newData,elem->dataSize);   //apply vkp
	}
	
	cp15_write_DAC(old_dac_mask);
	InterruptsAndFastInterrupts_Restore(intrMask);
}


void uninstall_patch(BOOK* book,GUI*)
{
	int i;
	int j;
	int cur_page_addr=0;
	
	VKPBook* vkp_book = (VKPBook*)book;
	
	int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	patch_list_elem* patch_to_remove = (patch_list_elem*)List_RemoveAt(patch_list,check_conflict(vkp_book,vkp_book->vkp_in->patch_data));
	
	LIST* unique_pages = List_Create();
	
	//remove patch in static area or ram
	for (i=0; i < patch_to_remove->patch_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_to_remove->patch_data,i);
		
		if (elem->isStatic == STATIC || elem->cxc == RAM)
		{
			if (fs_GetMemMap(elem->virtAddr,0))
				memcpy_int((char*)elem->virtAddr,elem->oldData,elem->dataSize);
		}
		else
		//create list with unique page_i
		{
			if (cur_page_addr != (elem->virtAddr&PAGE_ALIGN_MASK))
			{
				cur_page_addr = elem->virtAddr&PAGE_ALIGN_MASK;
				
				if (fs_GetMemMap(cur_page_addr,0))
				{
					if (List_Find(unique_pages,(void*)cur_page_addr,List_Find_pagelist_cmp) == LIST_ERROR)
					{
						List_InsertLast(unique_pages,(void*)cur_page_addr);
					}
				}
			}
		}
	}
	
	for (i=0; i < unique_pages->FirstFree; i++)
	{
		int kick_out_page_addr = (int)List_Get_int(unique_pages,i);
		
		if (List_Find(patch_list,(void*)kick_out_page_addr,List_Find_pages_cmp) == LIST_ERROR)
		{
			fs_demand_kick_out_page(get_page_i(vkp_book, fs_GetMemMap(kick_out_page_addr,0)),intrMask);
			*vkp_book->NbrOfLockedInPages -=1;
		}
		else
		{
			for (j=0; j < patch_to_remove->patch_data->FirstFree; j++)
			{
				vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch_to_remove->patch_data,j);
				
				if ((elem->virtAddr&PAGE_ALIGN_MASK) == kick_out_page_addr)
					memcpy_int((char*)elem->virtAddr,elem->oldData,elem->dataSize);
			}
		}
	}
	
	debug_printf("\r\nruntime_vkp: %ls uninstalled",patch_to_remove->patch_name);
	patch_list_elem_destroy(patch_to_remove);
	List_Destroy(unique_pages);
	
	cp15_write_DAC(old_dac_mask);
	InterruptsAndFastInterrupts_Restore(intrMask);
	
	BookObj_GotoPage(book,&VKPService_Idle_Page);
}


void install_patch(BOOK* book,GUI*)
{
	VKPBook* vkp_book = (VKPBook*)book;
	
	List_InsertLast(patch_list,vkp_book->vkp_in);
	apply_vkp(vkp_book,vkp_book->vkp_in->patch_data);
	debug_printf("\r\nruntime_vkp: %ls installed",vkp_book->vkp_in->patch_name);
	vkp_book->vkp_in=0;
	
	BookObj_GotoPage(book,&VKPService_Idle_Page);
}


void no_action(BOOK* book,GUI*)
{
	BookObj_GotoPage(book,&VKPService_Idle_Page);
}


int VKPInPage_EnterAction(void* r0,BOOK* bk)
{
	VKPBook* vkp_book = (VKPBook*)bk;
	
	VKP_CHECK_RESULT check_res = check_vkp_state(vkp_book,vkp_book->vkp_in->patch_data);
	
	if (check_res == VKP_CHECK_RESULT_NOT_INSTALLED || check_res == VKP_CHECK_RESULT_INSTALLED)
	{
		TEXTID prequestion;
		TEXTID question;
		void (*yes_action)(BOOK*,GUI*);
		
		if (check_res == VKP_CHECK_RESULT_NOT_INSTALLED)
		{
			prequestion = TextID_Create("Patch not installed",ENC_LAT1,TEXTID_ANY_LEN);
			question = TextID_Create("Install?",ENC_LAT1,TEXTID_ANY_LEN);
			yes_action = install_patch;
		}
		else
		{
			prequestion = TextID_Create("Patch installed",ENC_LAT1,TEXTID_ANY_LEN);
			question = TextID_Create("Uninstall?",ENC_LAT1,TEXTID_ANY_LEN);
			yes_action = uninstall_patch;
		}
		
		vkp_book->YesNoQuestion = CreateYesNoQuestionVA( 0,
																										VAR_BOOK( vkp_book ),
																										VAR_YESNO_PRE_QUESTION( prequestion ),
																										VAR_YESNO_QUESTION( question ),
																										0 );
		GUIObject_SoftKeys_SetAction(vkp_book->YesNoQuestion,ACTION_YES,yes_action);
		GUIObject_SoftKeys_SetAction(vkp_book->YesNoQuestion,ACTION_NO,no_action);
		GUIObject_SoftKeys_SetAction(vkp_book->YesNoQuestion,ACTION_BACK,no_action);
	}
	else
	//check_res == VKP_CHECK_RESULT_ERROR
	{
		int conflict_res = check_conflict(vkp_book,vkp_book->vkp_in->patch_data);
		if (conflict_res != -1)
		{
			patch_list_elem* elem = (patch_list_elem*)List_Get_int(patch_list,conflict_res);
			char buf[100];
			sprintf(buf,"Conflict with %ls",elem->patch_name);
			MessageBox(EMPTY_TEXTID,TextID_Create(buf,ENC_LAT1,TEXTID_ANY_LEN), NOIMAGE, 1, 5000,0);
		}
		else
			MessageBox(EMPTY_TEXTID,STR("Original data missmatch"), NOIMAGE, 1, 5000,0);
		
		BookObj_GotoPage(vkp_book,&VKPService_Idle_Page);
	}
	
	return 0;
}


int VKPInPage_ExitAction(void* r0,BOOK* bk)
{
	VKPBook* vkp_book = (VKPBook*)bk;
	
	if (vkp_book->YesNoQuestion)
	{
		GUIObject_Destroy(vkp_book->YesNoQuestion);
		vkp_book->YesNoQuestion=0;
	}
	return 0;
}


//return line len
int get_line_len(char* buf)
{
	int i=0;
	int len = strlen(buf);
	
	while (buf[i]!='\n' && i<len)
	{
		i++;
	}
  
	if (i == len || i+1 == len) return -1;
  
	return i+1;
}


char toupper(char chr)
{
	if (chr >= 'a' & chr <= 'z') return chr-0x20;
	return chr;
}


char xdig2char(char chr)
{
	if (chr >= '0' & chr <= '9') return chr-0x30;
	return toupper(chr)-0x37;
}


char xdig2byte(char chr1,char chr2)
{
	return xdig2char(chr1)<<4 | xdig2char(chr2);
}


int vkp_parse(VKPBook * vkp_book,wchar_t* path,wchar_t* name,LIST* patch_data)
{
	FSTAT _fstat;
	int fd;
	int file_size;
	char* position;
	char* new_line;
	int end_static_addr;
	int end_addr;
	int count;
	int line_len=0;
	int incr=0;
	int elem_count=0;
	
	fstat(path,name,&_fstat);
	
	file_size = _fstat.fsize;
	
	char* file_buf = new char[file_size+1];
	
	fd = _fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
	fread(fd,file_buf,file_size);
	fclose(fd);
	file_buf[file_size]=0;
	
	new_line = file_buf;
	
	do
	{
		new_line = new_line+line_len;
		position = new_line;
		
		if (position[0] != ';' && position[0] != '\r' && position[0] != '\n')
		{
			if (position[0] == '+' || position[0] == '-')
			{
				if (position[0] == '+')
					sscanf(position,"+%x",&incr);
				
				if (position[0] == '-')
				{
					sscanf(position,"-%x",&incr);
					incr = ~(incr-1);
				}
			}
			else
			{
				vkp_list_elem* elem = new vkp_list_elem;
				List_InsertLast(patch_data,elem);
				
				sscanf(position,"%x:",&elem->virtAddr);
				elem->virtAddr = elem->virtAddr+incr;
				
				if ((elem->virtAddr & CXC_BASE_ADDR_MASK) == EMP_START_ADDR)
					elem->cxc=CXC_EMP;
				else if ((elem->virtAddr & CXC_BASE_ADDR_MASK) == APP_START_ADDR)
					elem->cxc=CXC_APP;
				else
					elem->cxc=RAM;
				
				if (elem->cxc != RAM)
				{
					if (elem->cxc == CXC_EMP) 
					{
						end_static_addr=vkp_book->emp_static_end_addr;
						end_addr=vkp_book->emp_end_addr;
					}
					else
					{
						end_static_addr=vkp_book->app_static_end_addr;
						end_addr=vkp_book->app_end_addr;
					}
					
					if (elem->virtAddr < end_static_addr)
						elem->isStatic = STATIC;
					else
						elem->isStatic = NOT_STATIC;
					
					if (elem->virtAddr < end_addr)
						elem->area = MAIN;
					else
						elem->area = AFTER_MAIN;
				}
				
				position = strchr(position,':')+2;  //skip ": "
				elem->dataSize = (strchr(position,' ')-position)/2;
				elem->oldData = new char[elem->dataSize];
				
				count=0;
				while (count < elem->dataSize)
				{
					elem->oldData[count] = xdig2byte(position[count<<1],position[(count<<1)+1]);
					count=count+1;
				}
				
				position = position+elem->dataSize*2+1; //skip " " between old and new
				elem->newData = new char[elem->dataSize];

				count=0;
				while (count < elem->dataSize)
				{
					elem->newData[count] = xdig2byte(position[count<<1],position[(count<<1)+1]);
					count=count+1;
				}
				elem_count++;
			}
		}
	}
	while (line_len=get_line_len(new_line), line_len != -1);
	
	delete file_buf;
	
	return elem_count;
}


void load_patches(VKPBook * vkp_book)
{
	wchar_t path[250];
	wstrcpy(path,GetDir(DIR_OTHER|MEM_EXTERNAL));
	wstrcat(path,PATCHES_FOLDER_NAME);
	
	FILELISTITEM* fli = (FILELISTITEM*)malloc(sizeof(FILELISTITEM));
  
	DIR_HANDLE* handle = AllocDirHandle(path);
  
	if (handle)
	{
		int vkp_count=0;
    
		//fill list with data
		while (GetFname(handle,fli))
		{
			if (!wstrcmp(getFileExtention(fli->fname),FILE_FILTER_EXTENSION))
			{
				patch_list_elem* elem = new patch_list_elem;
				elem->patch_data = List_Create();
				elem->patch_name = new wchar_t[wstrlen(fli->fname)+1];
				wstrcpy(elem->patch_name,fli->fname);
				
				vkp_parse(vkp_book,fli->path,fli->fname,elem->patch_data);
				
				List_InsertLast(patch_list,elem);
				vkp_count++;
			}
		}
		
		debug_printf("\r\nruntime_vkp: %d vkp found",vkp_count);
		
		DestroyDirHandle(handle);
		mfree(fli);
	}
}


void patch_pcore_before()
{
	int PCORE_TO_PATCH;
	
	sscanf((char*)&PCORE_TO_PATCH1_str,scanf_mask,&PCORE_TO_PATCH);
	
	//int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	//patch pcore to remove page pool free
	*((char*)PCORE_TO_PATCH)=0xE0;
	
	debug_printf("\r\nruntime_vkp: pcore patched (CleanPageCache)");
	
	cp15_write_DAC(old_dac_mask);
	//InterruptsAndFastInterrupts_Restore(intrMask);
}


void patch_pcore_after()
{
	int PCORE_TO_PATCH_STATIC_CACHE;
	int PCORE_TO_PATCH_STATIC_CACHE_RETURN;
	int PCORE_TO_PATCH_NONSTATIC_CACHE;
	int PCORE_TO_PATCH_NONSTATIC_CACHE_RETURN;
	
	sscanf((char*)&PCORE_TO_PATCH2_str,scanf_mask,&PCORE_TO_PATCH_STATIC_CACHE);
	sscanf((char*)&PCORE_TO_PATCH3_str,scanf_mask,&PCORE_TO_PATCH_STATIC_CACHE_RETURN);
	sscanf((char*)&PCORE_TO_PATCH4_str,scanf_mask,&PCORE_TO_PATCH_NONSTATIC_CACHE);
	sscanf((char*)&PCORE_TO_PATCH5_str,scanf_mask,&PCORE_TO_PATCH_NONSTATIC_CACHE_RETURN);
	
	int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	//patch pcore to catch static page cache
	((int*)patch_pcore_static_cache)[5] = PCORE_TO_PATCH_STATIC_CACHE_RETURN+1;	//change return address
	
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[0] = 0x00;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[1] = 0x49;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[2] = 0x08;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[3] = 0x47;
	((int*)PCORE_TO_PATCH_STATIC_CACHE)[1] = (int)patch_pcore_static_cache;
	
	debug_printf("\r\nruntime_vkp: pcore patched (static pages)");
	
	//patch pcore to catch nonstatic page cache
	((int*)patch_pcore_nonstatic_cache)[6] = PCORE_TO_PATCH_NONSTATIC_CACHE_RETURN+1;	//change return address
	
	((char*)PCORE_TO_PATCH_NONSTATIC_CACHE)[0] = 0x00;
	((char*)PCORE_TO_PATCH_NONSTATIC_CACHE)[1] = 0x49;
	((char*)PCORE_TO_PATCH_NONSTATIC_CACHE)[2] = 0x08;
	((char*)PCORE_TO_PATCH_NONSTATIC_CACHE)[3] = 0x47;
	((int*)PCORE_TO_PATCH_NONSTATIC_CACHE)[1] = (int)patch_pcore_nonstatic_cache;
	
	debug_printf("\r\nruntime_vkp: pcore patched (nonstatic pages)");
	
	cp15_write_DAC(old_dac_mask);
	InterruptsAndFastInterrupts_Restore(intrMask);
}


int IdlePage_EnterAction(void* r0,BOOK* bk)
{
	VKPBook* vkp_book = (VKPBook*)bk;
	
	if (vkp_book->vkp_in)
	{
		patch_list_elem_destroy(vkp_book->vkp_in);
		vkp_book->vkp_in=0;
	}

	BookObj_WindowSetWantsFocus(vkp_book,0,FALSE);
	BookObj_Hide(vkp_book,0);
	
	return 0;
}


void _patch_received(VKPBook* vkp_book)
{
	BookObj_WindowSetWantsFocus(vkp_book,0,TRUE);
	BookObj_SetFocus(vkp_book,0);
	BookObj_GotoPage(vkp_book,&VKPService_VKP_In_Page);
}


void onCloseBook(BOOK * book)
{
}


VKPBook * StartService()
{
	debug_printf("\r\nruntime_vkp: start");
	
	VKPBook * vkp_book = new VKPBook;
	CreateBook(vkp_book,onCloseBook,&VKPService_BasePage,BOOK_NAME,-1,0);
	
	int EMP_SIZE;
	int APP_SIZE;
	
	sscanf((char*)&EMP_SIZE_str,scanf_mask,&EMP_SIZE);
	sscanf((char*)&APP_SIZE_str,scanf_mask,&APP_SIZE);
	
	vkp_book->emp_end_addr = EMP_START_ADDR+EMP_SIZE;
	vkp_book->app_end_addr = APP_START_ADDR+APP_SIZE;
	
	char* swap_base = getSWAP_DATA_BASE();
	vkp_book->emp_static_start_addr = *(int*)(swap_base+0xCA8);
	vkp_book->emp_static_end_addr = vkp_book->emp_static_start_addr + *(int*)(swap_base+0xCB4);
	vkp_book->app_static_start_addr = *(int*)(swap_base+0xCAC);
	vkp_book->app_static_end_addr = vkp_book->app_static_start_addr + *(int*)(swap_base+0xCB8);
	
	vkp_book->SwappedOutFirst = (wchar_t*)(swap_base+0x2);
	vkp_book->NbrOfSwappedInPages = (wchar_t*)(swap_base+0x6);
	vkp_book->NbrOfKickedOutPages = (wchar_t*)(swap_base+0xC);
	vkp_book->NbrOfLockedInPages = (wchar_t*)(swap_base+0xE);
	vkp_book->SwappedInFirst = (wchar_t*)(swap_base+0x10);
	vkp_book->fs_PageCacheMaxSize = (int*)(swap_base+0x58);
	vkp_book->PageCacheTbl = *(pageCache**)(swap_base+0x50);
	vkp_book->PagePoolTbl = (pagePool*)(swap_base+0x60);
	
	patch_list = List_Create();
	vkp_book->data_list=patch_list;
	vkp_book->vkp_in=0;
	
	vkp_book->patch_received=_patch_received;
	vkp_book->YesNoQuestion=0;
	
	return vkp_book;
}


int myFind(BOOK* book)
{
	if(!strcmp(book->xbook->name,BOOK_NAME)) return(1);
	return(0);
}


void print_info(VKPBook* vkp_book)
{
	int i;
	int j;
	
	debug_printf("\r\nruntime_vkp: %d patches installed",vkp_book->data_list->FirstFree);
	
	for (i=0; i < vkp_book->data_list->FirstFree; i++)
	{
		patch_list_elem* patch = (patch_list_elem*)List_Get_int(vkp_book->data_list,i);
		
		debug_printf("\r\n\r\nruntime_vkp: %ls",patch->patch_name);
		
		for (j=0; j < patch->patch_data->FirstFree; j++)
		{
			vkp_list_elem* elem = (vkp_list_elem*)List_Get_int(patch->patch_data,j);
			
			int physAddr = fs_GetMemMap(elem->virtAddr,0);
			
			if (physAddr == 0)
				debug_printf("\r\nruntime_vkp: virtAddr=0x%08X, physAddr = 0, Page not in cache",elem->virtAddr);
			else
			{
				wchar_t cur_page_i = get_page_i(vkp_book,physAddr);
				pageCache* page_p = vkp_book->PageCacheTbl + cur_page_i;
				
				debug_printf("\r\nruntime_vkp: virtAddr=0x%08X, physAddr = 0x%08X, page_i = 0x%04X, prev_i = 0x%04X, next_i = 0x%04X",elem->virtAddr,physAddr,cur_page_i,page_p->prev_i,page_p->next_i);
			}
			delay(10);
		}
		debug_printf("\r\n");
	}
}


int main (wchar_t* elfname,wchar_t* vkp_path,wchar_t* vkp_name)
{
	VKPBook* vkp_book = (VKPBook*)FindBook(myFind);
	if (vkp_book)
	{
		if ( vkp_path && vkp_name)
		{
			vkp_book->vkp_in = new patch_list_elem;
			vkp_book->vkp_in->patch_data = List_Create();
			vkp_parse(vkp_book,vkp_path,vkp_name,vkp_book->vkp_in->patch_data);
			vkp_book->vkp_in->patch_name = new wchar_t[wstrlen(vkp_name)+1];
			wstrcpy(vkp_book->vkp_in->patch_name,vkp_name);
			
			vkp_book->patch_received(vkp_book);
		}
		else
		{
			print_info(vkp_book);
			MessageBox(EMPTY_TEXTID,STR("Already runed"), NOIMAGE, 1, 5000,0);
		}
		
		SUBPROC(elf_exit);
	}
	else
	{
		InitConfig();
		vkp_book = StartService();
		patch_pcore_before();
		load_patches(vkp_book);
		
		int i;
		for (i=0; i < patch_list->FirstFree; i++)
		{
			patch_list_elem* elem = (patch_list_elem*)List_Get_int(patch_list,i);
			apply_vkp(vkp_book,elem->patch_data);
			debug_printf("\r\nruntime_vkp: %ls installed",elem->patch_name);
		}
		patch_pcore_after();
		
		BookObj_GotoPage(vkp_book,&VKPService_Idle_Page);
	}
	
	return 0;

	//SUBPROC(elf_exit);
}
