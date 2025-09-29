#ifndef _MAIN_H_
#define _MAIN_H_

#if defined(DB3150_R1)
int (*fs_memmap)(int virtAddr, int physAddr, int size, int flags) = (int (*)(int virtAddr, int physAddr, int size, int flags))0x28B0025C;
int (*fs_GetMemMap)(int virtAddr, int *flags) = (int (*)(int virtAddr, int *flags))0x28B00260;
#elif defined(C702_R3EF001)
int (*fs_memmap)(int virtAddr, int physAddr, int size, int flags) = (int (*)(int virtAddr, int physAddr, int size, int flags))0x2800025C;
int (*fs_GetMemMap)(int virtAddr, int *flags) = (int (*)(int virtAddr, int *flags))0x28000260;
#else
int (*fs_memmap)(int virtAddr, int physAddr, int size, int flags) = (int (*)(int virtAddr, int physAddr, int size, int flags))0x4800025C;
int (*fs_GetMemMap)(int virtAddr, int *flags) = (int (*)(int virtAddr, int *flags))0x48000260;
#endif

#define BOOK_NAME "vkp_service"

#define PATCHES_FOLDER_NAME L"/elf_patches"
#define FILE_FILTER_EXTENSION L"vkp"

#if defined(DB3200) || defined(DB3210) ||defined(DB3350)
#define EMP_START_ADDR 0x10000000
#define APP_START_ADDR 0x14000000
#else
#define APP_START_ADDR 0x10000000
#endif

#define PAGE_SIZE 0x400
#define PAGE_ALIGN_MASK 0xFFFFFC00
#define STATIC_PAGE_SIZE 0x1000
#define STATIC_PAGE_ALIGN_MASK 0xFFFFF000
#define POOL_SIZE 0x80

#define CXC_BASE_ADDR_MASK 0xFC000000
#define PHYS_BASE_ADDR_MASK 0xFFFF0000

#define NEED_TO_LOCK 0x4

#define SWAPPED_OUT_FIRST_OFFSET 0x2
#define NBR_SWAPPED_IN_OFFSET 0x6
#define NBR_KICKED_OUT_OFFSET 0xC
#define NBR_LOCKED_IN_OFFSET 0xE

#if defined(DB3200) || defined(DB3210)
#define EMP_STATIC_START_ADDR_OFFSET 0xCA8
#define EMP_STATIC_SIZE_OFFSET 0xCB4
#define APP_STATIC_START_ADDR_OFFSET 0xCAC
#define APP_STATIC_SIZE_OFFSET 0xCB8
#define PAGE_CACHE_TBL_OFFSET 0x50
#define FS_PAGE_CACHE_MAXSIZE_OFFSET 0x58
#define PAGE_POOL_TBL_OFFSET 0x60
#define SWAPPED_IN_FIRST_OFFSET 0x10

#define STR_AUTHOR_INFO STR("VKP Runtime, v1.35\nDB3210\n\n(c) IronMaster")

#elif defined(DB3350)
#define EMP_STATIC_START_ADDR_OFFSET 0x28
#define EMP_STATIC_SIZE_OFFSET 0x34
#define APP_STATIC_START_ADDR_OFFSET 0x2C
#define APP_STATIC_SIZE_OFFSET 0x38
#define PAGE_CACHE_TBL_OFFSET 0x10
#define FS_PAGE_CACHE_MAXSIZE_OFFSET 0x18
#define PAGE_POOL_TBL_OFFSET 0x4C
#define SWAPPED_IN_FIRST_OFFSET 0xC4C

#define STR_AUTHOR_INFO STR("VKP Runtime, v1.35\nDB3350\n\n(c) IronMaster")

#elif defined(DB3150_R1)
#define APP_STATIC_START_ADDR_OFFSET 0x98
#define APP_STATIC_FS_PAGE_COUNT_OFFSET 0x54
#define FS_PAGE_CACHE_MAXSIZE_OFFSET 0x50
#define PAGE_CACHE_TBL_OFFSET 0x12240
#define PAGE_POOL_TBL_OFFSET 0x240
#define SWAPPED_IN_FIRST_OFFSET 0x10

#define STR_AUTHOR_INFO STR("VKP Runtime, v1.35\nDB3150.R1\n\n(c) IronMaster")

#elif defined(DB3150_R2)
#define APP_STATIC_START_ADDR_OFFSET 0xCA8
#define APP_STATIC_SIZE_OFFSET 0xCB4
#define PAGE_CACHE_TBL_OFFSET 0x50
#define FS_PAGE_CACHE_MAXSIZE_OFFSET 0x58
#define PAGE_POOL_TBL_OFFSET 0x60
#define SWAPPED_IN_FIRST_OFFSET 0x10

#define STR_AUTHOR_INFO STR("VKP Runtime, v1.35\nDB3150.R2\n\n(c) IronMaster")

#else 
// DB3150_R2 New
#define APP_STATIC_START_ADDR_OFFSET 0x60
#define APP_STATIC_SIZE_OFFSET 0x6C
#define PAGE_CACHE_TBL_OFFSET 0x50
#define FS_PAGE_CACHE_MAXSIZE_OFFSET 0x58
#define PAGE_POOL_TBL_OFFSET 0x74
#define SWAPPED_IN_FIRST_OFFSET 0x10

#define STR_AUTHOR_INFO STR("VKP Runtime, v1.35\nDB3150.R2_New\n\n(c) IronMaster")
#endif

typedef enum VKP_CHECK_RESULT
{
	VKP_CHECK_RESULT_ERROR = -1,
	VKP_CHECK_RESULT_NOT_INSTALLED = 0,
	VKP_CHECK_RESULT_INSTALLED = 1,
} VKP_CHECK_RESULT;

typedef enum CXC_TYPE
{
#if defined(DB3200) || defined(DB3210) ||defined(DB3350)
	CXC_EMP,
#endif
	CXC_APP,
	RAM,
} CXC_TYPE;

typedef enum STATIC_TYPE
{
	NOT_STATIC,
	STATIC,
	TYPE_RAM,
} STATIC_TYPE;

typedef enum AREA_TYPE
{
	MAIN,
	AFTER_MAIN,
	AREA_RAM,
} AREA_TYPE;

typedef struct
{
	int virtAddr;
	CXC_TYPE cxc;
	STATIC_TYPE isStatic;
	AREA_TYPE area;
	int dataSize;
	char *oldData;
	char *newData;
} vkp_list_elem;

typedef struct
{
	wchar_t *patch_name;
	LIST *patch_data;
} patch_list_elem;

typedef struct VKPBook : BOOK
{
	LIST *data_list;
	void (*patch_received)(VKPBook *book);
	patch_list_elem *vkp_in;
	GUI *YesNoQuestion;
	int emp_end_addr;
	int emp_static_start_addr;
	int emp_static_end_addr;
	int app_end_addr;
	int app_static_start_addr;
	int app_static_end_addr;
	wchar_t *SwappedOutFirst;
	wchar_t *NbrOfSwappedInPages;
	wchar_t *NbrOfKickedOutPages;
	wchar_t *NbrOfLockedInPages;
	wchar_t *SwappedInFirst;
	int *fs_PageCacheMaxSize;
	pageCache *PageCacheTbl;
	pagePool *PagePoolTbl;
} VKPBook;

typedef struct
{
	BOOK *book;
} MSG;

int ShowAuthorInfo(void *mess, BOOK *book);
int IdlePage_EnterAction(void *r0, BOOK *bk);
int VKPInPage_ExitAction(void *r0, BOOK *bk);
int VKPInPage_EnterAction(void *r0, BOOK *bk);

#endif
