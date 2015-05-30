#ifndef _MAIN_H_
#define _MAIN_H_

int (*fs_memmap)( int virtAddr, int physAddr, int size, int flags )=(int(*)( int virtAddr, int physAddr, int size, int flags ))0x4800025C;
int (*fs_GetMemMap)( int virtAddr, int* flags )=(int(*)( int virtAddr, int* flags ))0x48000260;


#define BOOK_NAME "vkp_service"

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


typedef enum VKP_CHECK_RESULT
{
	VKP_CHECK_RESULT_ERROR					=-1,
	VKP_CHECK_RESULT_NOT_INSTALLED	= 0,
	VKP_CHECK_RESULT_INSTALLED			= 1,
}VKP_CHECK_RESULT;


typedef enum CXC_TYPE
{
	CXC_EMP		= 0,
	CXC_APP		= 1,
	RAM				= 2,
}CXC_TYPE;


typedef enum STATIC_TYPE
{
	NOT_STATIC	= 0,
	STATIC			= 1,
}STATIC_TYPE;


typedef enum AREA_TYPE
{
	MAIN						= 0,
	AFTER_MAIN			= 1,
}AREA_TYPE;


typedef struct
{
	int virtAddr;
	CXC_TYPE cxc;
	STATIC_TYPE isStatic;
	AREA_TYPE area;
	int dataSize;
	char* oldData;
	char* newData;
}vkp_list_elem;


typedef struct
{
	wchar_t* patch_name;
	LIST* patch_data;
}patch_list_elem;


typedef struct VKPBook : BOOK
{
	LIST* data_list;
	void (*patch_received)(VKPBook* book);
	patch_list_elem* vkp_in;
	GUI* YesNoQuestion;
	int emp_end_addr;
	int emp_static_start_addr;
	int emp_static_end_addr;
	int app_end_addr;
	int app_static_start_addr;
	int app_static_end_addr;
	wchar_t* SwappedOutFirst;
	wchar_t* NbrOfSwappedInPages;
	wchar_t* NbrOfKickedOutPages;
	wchar_t* NbrOfLockedInPages;
	wchar_t* SwappedInFirst;
	int* fs_PageCacheMaxSize;
	pageCache* PageCacheTbl;
	pagePool* PagePoolTbl;
}VKPBook;


typedef struct
{
	BOOK * book;
}MSG;


int ShowAuthorInfo(void *mess ,BOOK * book);
int IdlePage_EnterAction(void* r0,BOOK* bk);
int VKPInPage_ExitAction(void* r0,BOOK* bk);
int VKPInPage_EnterAction(void* r0,BOOK* bk);

#endif
