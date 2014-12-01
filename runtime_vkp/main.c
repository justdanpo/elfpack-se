#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\cfg_items.h"
#include "conf_loader.h"
#include "config_data.h"

#define PATCHES_FOLDER_NAME L"/elf_patches"
#define FILE_FILTER_EXTENSION L"vkp"

#define EMP_START_ADDR 0x10000000
#define APP_START_ADDR 0x14000000

#define PAGE_SIZE 0x400
#define PAGE_ALIGN_MASK 0xFFFFFC00
#define STATIC_PAGE_ALIGN_MASK 0xFFFFF000
#define POOL_SIZE 0x80

#define CXC_BASE_ADDR_MASK 0xFC000000
#define PHYS_BASE_ADDR_MASK 0xFFFF0000

#define NEED_TO_LOCK 0x4

LIST* static_list;
void create_static_list(LIST* vkp_data);


typedef struct
{
	int virtAddr;
	int dataSize;
	char* oldData;	//maybe in future
	char* newData;
}vkp_list_elem;


extern "C" int InterruptsAndFastInterrupts_Off();
extern "C" void InterruptsAndFastInterrupts_Restore(int intrMask);
extern "C" int cp15_write_DAC(int mask);
extern "C" void patch_pcore_static_cache();


//int (*CleanPageCache)(int size,int clean_all_flag)=(int(*)(int size,int clean_all_flag))0x48011329;
//void (*InitPageCache)(int min_size,int max_size)=(void(*)(int min_size,int max_size))0x48011669;


void elf_exit(void)

{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


char * strchr(char * str,char c)
{
	for(;*str;str++)if(*str==c)return str;
	return NULL;
}


extern "C" int check_static_after_map(int virtAddr,int physAddr)
{
	int i;
	int ret = fs_memmap(virtAddr,physAddr,0x1000,FS_MEMMAP_NONBUFFERED|FS_MEMMAP_NOPERMISSIONS|FS_MEMMAP_CACHED|FS_MEMMAP_READ);
	
	for (i=0; i < static_list->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get(static_list,i);
		
		if (virtAddr == (elem->virtAddr&STATIC_PAGE_ALIGN_MASK) )
			memcpy((void*)elem->virtAddr,elem->newData,elem->dataSize);   //apply vkp
	}
	
	return ret;
}


int get_page_i(int physAddr)
{
	char* swap_base = getSWAP_DATA_BASE();
	pagePool* PagePoolTbl_p = (pagePool*)(swap_base+0x60);
	int* fs_PageCacheMaxSize = (int*)(swap_base+0x58);
	
	int ret = 0xFFFF;
	int i;
	int max_pool_i = *fs_PageCacheMaxSize / POOL_SIZE;
	
	for (i=0;i<max_pool_i;i++)
	{
		pagePool* pool_p = PagePoolTbl_p+i;
		int physAddr_align = physAddr & PHYS_BASE_ADDR_MASK;
		if ( pool_p->baseAddr == physAddr_align || pool_p->baseAddr == physAddr_align-0x10000 )
		{
			char pos_in_pool = (physAddr - pool_p->baseAddr) / PAGE_SIZE;
			ret = i*POOL_SIZE+pos_in_pool;
			//debug_printf("page physAddr = 0x%08X, pool_i = %d, pool baseAddr = 0x%08X, page_i = %d\r\n",physAddr,i,pool_p->baseAddr,ret);
			return ret;
		}
	}
	return ret;
}


void apply_vkp(LIST* vkp_data)
{
	int EMP_SIZE;
	int APP_SIZE;
	int PCORE_TO_PATCH;
	int PCORE_TO_PATCH_STATIC_CACHE;
	int PCORE_TO_PATCH_STATIC_CACHE_RETURN;
	
	sscanf((char*)&EMP_SIZE_str,"%x",&EMP_SIZE);
	sscanf((char*)&APP_SIZE_str,"%x",&APP_SIZE);
	sscanf((char*)&PCORE_TO_PATCH1_str,"%x",&PCORE_TO_PATCH);
	sscanf((char*)&PCORE_TO_PATCH2_str,"%x",&PCORE_TO_PATCH_STATIC_CACHE);
	sscanf((char*)&PCORE_TO_PATCH3_str,"%x",&PCORE_TO_PATCH_STATIC_CACHE_RETURN);
	
	int EMP_END_ADDR = EMP_START_ADDR+EMP_SIZE;
	int APP_END_ADDR = APP_START_ADDR+APP_SIZE;
	
	char* swap_base = getSWAP_DATA_BASE();
	wchar_t* SwappedOutFirst = (wchar_t*)(swap_base+0x2);
	wchar_t* NbrOfSwappedInPages = (wchar_t*)(swap_base+0x6);
	wchar_t* NbrOfKickedOutPages = (wchar_t*)(swap_base+0xC);
	wchar_t* NbrOfLockedInPages = (wchar_t*)(swap_base+0xE);
	wchar_t* SwappedInFirst_p = (wchar_t*)(swap_base+0x10);
	pageCache** PageCacheTbl_p = (pageCache**)(swap_base+0x50);
	pagePool* PagePoolTbl_p = (pagePool*)(swap_base+0x60);
	int EMP_STATIC_START = *(int*)(swap_base+0xCA8);
	int APP_STATIC_START = *(int*)(swap_base+0xCAC);
	int EMP_STATIC_SIZE = *(int*)(swap_base+0xCB4);
	int APP_STATIC_SIZE = *(int*)(swap_base+0xCB8);
  
	int i;
	int end_addr;
	int end_static_addr;
	int physAddr;
	wchar_t swap_i;
	int cur_page_addr=0;
	
	int intrMask = InterruptsAndFastInterrupts_Off();
	int old_dac_mask = cp15_write_DAC(0xFFFFFFFF);
	
	//patch pcore to remove page pool free
	*((char*)PCORE_TO_PATCH)=0xE0;
		
	for (i=0; i < vkp_data->FirstFree; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get(vkp_data,i);
		if (cur_page_addr != (elem->virtAddr&PAGE_ALIGN_MASK))
		{
			cur_page_addr = elem->virtAddr&PAGE_ALIGN_MASK;
			if (physAddr = fs_GetMemMap(cur_page_addr,0), physAddr==0 )    //if not already created
			{
				if (elem->virtAddr & CXC_BASE_ADDR_MASK == EMP_START_ADDR) end_addr=EMP_END_ADDR;
				else end_addr=APP_END_ADDR;
				
				if (elem->virtAddr < end_addr)
				{
					fs_demand_cache_page(elem->virtAddr,2|NEED_TO_LOCK,intrMask);
				}
				else
				{
					if (swap_i = fs_demand_get_page_i_from_queue(SwappedOutFirst,0,0x20), swap_i == 0xFFFF)
					{
						swap_i = fs_demand_get_page_i_from_queue(SwappedInFirst_p,0,*NbrOfSwappedInPages);
						fs_demand_kick_out_page(swap_i,intrMask);
					}
					
					pageCache* page_p = *PageCacheTbl_p+swap_i;
					pagePool* pool_p = PagePoolTbl_p+(swap_i/POOL_SIZE);
					
					page_p->virtAddr = cur_page_addr;
					if (!pool_p->baseAddr) fs_demand_pagePool_alloc_mem(pool_p,intrMask);
					pool_p->usedPages++;
					fs_memmap(page_p->virtAddr, pool_p->baseAddr+((swap_i&0x7F)*PAGE_SIZE), PAGE_SIZE, FS_MEMMAP_WRITE|FS_MEMMAP_READ|FS_MEMMAP_CACHED|FS_MEMMAP_BUFFERED);
					*NbrOfKickedOutPages-=1;
					fs_demand_remove_from_queue(page_p,0xFFFF);
					*NbrOfLockedInPages+=1;
				}
				delay(5);
			}
			else
			//memmap exists
			{
				if (elem->virtAddr & CXC_BASE_ADDR_MASK == EMP_START_ADDR) end_static_addr=EMP_STATIC_START+EMP_STATIC_SIZE;
				else end_static_addr=APP_STATIC_START+APP_STATIC_SIZE;
				
				if (elem->virtAddr >= end_static_addr)
				{
					wchar_t kick_out_i = get_page_i(physAddr);
					pageCache* kick_out_p = *PageCacheTbl_p+kick_out_i;
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
		memcpy((void*)elem->virtAddr,elem->newData,elem->dataSize);   //apply vkp
	}
	
	create_static_list(vkp_data);
	
	//patch pcore to catch static page cache
	((int*)patch_pcore_static_cache)[5] = PCORE_TO_PATCH_STATIC_CACHE_RETURN+1;	//change return address
	
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[0] = 0x00;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[1] = 0x49;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[2] = 0x08;
	((char*)PCORE_TO_PATCH_STATIC_CACHE)[3] = 0x47;
	((int*)PCORE_TO_PATCH_STATIC_CACHE)[1] = (int)patch_pcore_static_cache;
  
	cp15_write_DAC(old_dac_mask);
	InterruptsAndFastInterrupts_Restore(intrMask);
}


void vkp_elem_free(void * r)
{
	vkp_list_elem * elem=(vkp_list_elem *)r;
	if (elem)
	{
		if (elem->oldData)
			delete elem->oldData;
		
		if (elem->newData)
			delete elem->newData;
		
		delete(elem);
	}
}


void create_static_list(LIST* vkp_data)
{
	int i;
	int end_static_addr;
	int elem_count = vkp_data->FirstFree;
	
	char* swap_base = getSWAP_DATA_BASE();
	int EMP_STATIC_START = *(int*)(swap_base+0xCA8);
	int APP_STATIC_START = *(int*)(swap_base+0xCAC);
	int EMP_STATIC_SIZE = *(int*)(swap_base+0xCB4);
	int APP_STATIC_SIZE = *(int*)(swap_base+0xCB8);
	
	static_list = List_Create();
	
	for (i=0; i < elem_count; i++)
	{
		vkp_list_elem* elem = (vkp_list_elem*)List_Get(vkp_data,i);
		
		if (elem->virtAddr & CXC_BASE_ADDR_MASK == EMP_START_ADDR) end_static_addr=EMP_STATIC_START+EMP_STATIC_SIZE;
		else end_static_addr=APP_STATIC_START+APP_STATIC_SIZE;
		
		if (elem->virtAddr < end_static_addr)
			List_InsertFirst(static_list,elem);
		else
			vkp_elem_free(elem);
	}
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


int vkp_parse(wchar_t* path,wchar_t* name,LIST* vkp_data)
{
	FSTAT _fstat;
	int fd;
	int file_size;
	char* position;
	char* new_line;
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
				List_InsertLast(vkp_data,elem);
				
				sscanf(position,"%x:",&elem->virtAddr);
				elem->virtAddr = elem->virtAddr+incr;
				position = strchr(position,':')+2;  //miss ": "
				elem->dataSize = (strchr(position,' ')-position)/2;
				elem->oldData=0;
				position = position+elem->dataSize*2+1; //skip " " between old and new
				elem->newData = new char[elem->dataSize];

				//int chr;				
				count=0;
				while (count < elem->dataSize)
				{
					//sscanf(position+count*2,"%2x,",&chr);
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


int main (void)
{
	debug_printf("\r\nruntime_vkp: start\r\n");
	
	InitConfig();
	wchar_t path[250];
	wstrcpy(path,GetDir(DIR_OTHER|MEM_EXTERNAL));
	wstrcat(path,PATCHES_FOLDER_NAME);
	
	FILELISTITEM* fli = (FILELISTITEM*)malloc(sizeof(FILELISTITEM));
  
	DIR_HANDLE* handle = AllocDirHandle(path);
  
	if (handle)
	{
		LIST* vkp_data = List_Create();
		int vkp_count=0;
    
		//fill list with data
		while (GetFname(handle,fli))
		{
			if (!wstrcmp(getFileExtention(fli->fname),FILE_FILTER_EXTENSION))
			{
				vkp_parse(fli->path,fli->fname,vkp_data);
				vkp_count++;
			}
		}
		
		debug_printf("\r\nruntime_vkp: %d files found\r\n",vkp_count);
		
		apply_vkp(vkp_data);
		
		List_Destroy(vkp_data);
		
		DestroyDirHandle(handle);
		mfree(fli);
	}
	
	debug_printf("\r\nruntime_vkp: end\r\n");
  
	SUBPROC(elf_exit);
}
