#include "..\include\Lib_Clara.h"
#include "..\include\Types.h"
#include "vars.h"
#include "elfloader.h"

#include "temp\target.h"

#define IDN_ELF_SMALL_ICON _T("CALE_LUNAR_12ANIMALS_1ST_MOUSE_ICN")
#define IDN_ELF_BIG_ICON _T("CALE_LUNAR_12ANIMALS_10TH_ROOSTER_ICN")

#define IDN_FOLDERNAME _T("DB_OTHER_TXT") //old value: _T("DB_APPLICATIONS_TXT")
#define IDN_SAVED_ON_MS _T("DB_SAVED_IN_OTHER_ON_MS_TXT")
#define IDN_SAVED_ON_PH _T("DB_SAVED_IN_OTHER_IN_PH_TXT")
#define IDN_START _T("GUI_SELECT_SK") //old values: _T("OAF_START_SK"), _T("MENU_SYNCH_START_REMOTE_TXT")

#ifdef EP_EXTICON14x14
#include "exticons\14x14.inc"
#endif

#ifdef EP_EXTICON16x20
#include "exticons\16x20.inc"
#endif

#ifdef EP_EXTICON24x24
#include "exticons\24x24.inc"
#endif

#ifdef EP_EXTICON42x32
#include "exticons\42x32.inc"
#endif

#ifdef EP_EXTICON48x36
#include "exticons\48x36.inc"
#endif

#ifdef EP_EXTICON56x42
#include "exticons\56x42.inc"
#endif

const wchar_t ext[]={L"elf"};
const char ctype[]={"application/elf"};

const int ex[2]={(int)ext,0};
const int ct[2]={(int)ctype,0};

const char ers[]={"Elf_Run_Subroutine"};

DB_EXT * OtherDbExt()
{
	int i = List_IndexOf(elfpackdata->DBExtList, LastExtDB());
	return (DB_EXT*)List_Get(elfpackdata->DBExtList, i-1);
}

DB_EXT * CreateDbExt()
{
	DB_EXT * other_db_ext = OtherDbExt();
#if defined(DB2020) || defined(PNX5230) || defined(A2)
	DB_EXT * db_ext = malloc(sizeof(DB_EXT));
	db_ext->type_group = other_db_ext->type_group;
#else
	DB_EXT_2010 * db_ext = malloc(sizeof(DB_EXT_2010));
#endif
	db_ext->content_type = 0;
	db_ext->ext_list = 0;
	db_ext->GetMethods = 0;
	db_ext->sub_execute_size = other_db_ext->sub_execute_size;
	db_ext->dbf = 0;
	db_ext->drm_flags = 0;
	db_ext->unk2 = 0;
	db_ext->unk3 = 0;
	return (DB_EXT*)db_ext;
}

void GetMethods(SUB_EXECUTE * sub_execute)
{
	sub_execute->filesub = elfpackdata->elf_ext_m;
}

int SetSmallIcon(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
	if (elfpackdata->IconSmall == NOIMAGE)
	{
#ifdef USESMALLICON
		void * pexticon_small = malloc(sizeof(exticon_small));
		memcpy(pexticon_small,exticon_small,sizeof(exticon_small));
		ImageID_GetIndirect( pexticon_small, sizeof(exticon_small),0, L"png", &elfpackdata->IconSmall);
#else
		int iconsmall;
		iconidname2id(IDN_ELF_SMALL_ICON,-1,&iconsmall);
		elfpackdata->IconSmall = iconsmall;
#endif
	}
	*iconid = elfpackdata->IconSmall;
	return 0;
}

int SetThumbnailIcon(SUB_EXECUTE * sub_execute, wchar_t * iconid)
{
	if (elfpackdata->IconBig == NOIMAGE)
	{
#ifdef USEBIGICON
		void * pexticon_big = malloc(sizeof(exticon_big));
		memcpy(pexticon_big,exticon_big,sizeof(exticon_big));
		ImageID_GetIndirect( pexticon_big, sizeof(exticon_big),0, L"png", &elfpackdata->IconBig);
#else
		int iconbig;
		iconidname2id(IDN_ELF_BIG_ICON,-1,&iconbig);
		elfpackdata->IconBig = iconbig;
#endif
	}
	*iconid = elfpackdata->IconBig;
	return 0;
}

int Elf_Run_Page_PAGE_ENTER_EVENT(void * r0, BOOK * book)
{
	SUB_EXECUTE * data = BrowserItem_Get_SUB_EXECUTE(book);

	wchar_t* fpath = FILEITEM_GetPath(data->file_item);
	wchar_t* fname = FILEITEM_GetFname(data->file_item);
	wchar_t* filename = malloc((wstrlen(fpath)+wstrlen(fname)+2)*2);

	wstrcpy(filename,fpath);
	wstrcat(filename,L"/");
	wstrcat(filename,fname);

	_printf("Starting %ls",filename);

	elfload_int(filename,0,0,0);

	mfree(filename);
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
	return(1);
};

int Elf_Run_Page_PREVIOUS_EVENT(void * r0, BOOK * book)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
	return(1);
}

int Elf_Run_Page_ACCEPT_EVENT(void * r0, BOOK * book)
{
	BookObj_ReturnPage(book,PREVIOUS_EVENT);
	return(1);
}

int Elf_Run_Page_CANCEL_EVENT(void * r0, BOOK * book)
{
	BookObj_ReturnPage(book,CANCEL_EVENT);
	return(1);
}

int Elf_Run_Page_PAGE_EXIT_EVENT(void * r0, BOOK * book)
{
	return(1);
}

const PAGE_MSG erp_msg[]={
	PAGE_ENTER_EVENT,  Elf_Run_Page_PAGE_ENTER_EVENT,
	PREVIOUS_EVENT,    Elf_Run_Page_PREVIOUS_EVENT,
	ACCEPT_EVENT,      Elf_Run_Page_ACCEPT_EVENT,
	CANCEL_EVENT,      Elf_Run_Page_CANCEL_EVENT,
	PAGE_EXIT_EVENT,   Elf_Run_Page_PAGE_EXIT_EVENT,
	0,0
};

const PAGE_DESC erun_page={"Elf_Run_Page",0,erp_msg};

const int subrout[]={(int)&ers,(int)&erun_page,0,0};

int Elf_Run_Subroutine(SUB_EXECUTE * sub_execute)
{
	BookObj_CallSubroutine(sub_execute->BrowserItemBook,(void*)subrout);
	return(1);
};

int Elf_Run_Check(SUB_EXECUTE * sub_execute)
{
	return 2;
}

void ELFExtrRegister()
{
	int ofs_count = 0;
	FILESUBROUTINE * OtherFileSub;
	FILESUBROUTINE * NewFileSub;

	GetOtherExtMethods(&OtherFileSub);
	while (OtherFileSub[ofs_count++].ON_CMD);
	elfpackdata->elf_ext_m = NewFileSub = (FILESUBROUTINE*)malloc((ofs_count+1)*sizeof(FILESUBROUTINE));
	memcpy(NewFileSub+1, OtherFileSub, ofs_count*sizeof(FILESUBROUTINE));

	NewFileSub->cmd = 1;
	NewFileSub->ON_CMD_RUN = Elf_Run_Subroutine;
	NewFileSub->ON_CMD_RUN_CHECK = Elf_Run_Check;
	textidname2id(IDN_START,-1,&NewFileSub->StrID);


	while (NewFileSub->ON_CMD)
	{
		int cmd = NewFileSub->cmd;
		if (cmd == DB_CMD_SETSMALLICON)
		{
			NewFileSub->ON_CMD_ICON = SetSmallIcon;
		}
#ifdef DB_CMD_SETTHUMBNAILICON
		else if(cmd == DB_CMD_SETTHUMBNAILICON)
		{
			NewFileSub->ON_CMD_ICON = SetThumbnailIcon;
		}
#endif
#ifdef DB_CMD_SETSAVEDICON
		else if(cmd == DB_CMD_SETSAVEDICON)
		{
			NewFileSub->ON_CMD_ICON = SetThumbnailIcon;
		}
#endif
		NewFileSub++;
	}
	NewFileSub = elfpackdata->elf_ext_m;

#ifndef DAEMONS_INTERNAL
	int efnum=3;
#else
	int efnum=2;
#endif
	DB_EXT_FOLDERS * dbfolders = malloc(sizeof(DB_EXT_FOLDERS)*efnum--);
	//last empty record
	dbfolders[efnum].StrID_FolderName = EMPTY_TEXTID;
	dbfolders[efnum].StrID_SavedTo = EMPTY_TEXTID;
	dbfolders[efnum].Path = NULL;
	dbfolders[efnum--].isInternal = NULL;
	//internal folder
	textidname2id(IDN_FOLDERNAME, -1, &dbfolders[efnum].StrID_FolderName);
	textidname2id(IDN_SAVED_ON_PH, -1, &dbfolders[efnum].StrID_SavedTo);
	dbfolders[efnum].Path = PATH_ELF_ROOT_INT;
	dbfolders[efnum--].isInternal = 1;
#ifndef DAEMONS_INTERNAL
	//external folder
	textidname2id( IDN_FOLDERNAME, -1, &dbfolders[efnum].StrID_FolderName);
	textidname2id( IDN_SAVED_ON_MS, -1, &dbfolders[efnum].StrID_SavedTo);
	dbfolders[efnum].Path = PATH_ELF_ROOT_EXT;
	dbfolders[efnum].isInternal = 0;
#endif

	DB_EXT * db_ext = CreateDbExt();
	db_ext->content_type = (char**)ct;
	db_ext->ext_list = (wchar_t**)ex;
	db_ext->GetMethods = GetMethods;
	db_ext->dbf = dbfolders;

	List_InsertFirst(elfpackdata->DBExtList, db_ext);
}

LIST *CreateDBExtList()
{
	LIST * dbe = List_Create();
	DB_EXT ** old = (DB_EXT**)EXT_TABLE;
	int db_ext_count = 0;
	DB_EXT * last_ext = LastExtDB();
	do
	{
		List_InsertLast(dbe, old[db_ext_count]);
	}while(old[db_ext_count++] != last_ext);
	return dbe;
}
