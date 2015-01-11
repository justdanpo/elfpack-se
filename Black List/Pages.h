#ifndef _PAGES_H_
#define _PAGES_H_

int TerminateElf(void * ,BOOK * BlackListBook);
int ShowAuthorInfo(void *mess ,BOOK * BlackListBook);
int BlackListBook_Idle_Page_EnterAction(void * ,BOOK * book);
int BlackListBook_Main_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_Main_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_Main_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_Settings_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_Settings_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_Settings_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_Settings_CallsSMS_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_Settings_CallsSMS_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_Settings_CallsSMS_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_SettingsOOM_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_SettingsOOM_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_SettingsOOM_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_ChooseModeToEdit_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_ChooseModeToEdit_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_ChooseModeToEdit_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_ChooseListToEdit_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_ChooseListToEdit_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_ChooseListToEdit_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_EditBlockList_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_EditBlockList_Page_Exit_Action(void * ,BOOK * book);
int BlackListBook_SearchPB_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_SearchPB_Page_Accept_Action(void * ,BOOK * book);
int BlackListBook_SearchPB_Page_Prev_Action(void * ,BOOK * book);
int BlackListBook_SearchPB_Page_Cancel_Action(void * ,BOOK * book);
int BlackListBook_SI_Page_Enter_Action(void * ,BOOK * book);
int BlackListBook_SI_Page_Exit_Action(void * ,BOOK * book);


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
	ELF_TERMINATE_EVENT, TerminateElf,
	ELF_SHOW_INFO_EVENT, ShowAuthorInfo,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_Base_Page = { "BlackListBook_Base_Page", 0, bk_base_msglst };


const PAGE_MSG bk_idle_events[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG,	BlackListBook_Idle_Page_EnterAction,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_Idle_Page = { "BlackListBook_Idle_Page", 0, bk_idle_events };


const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_Main_Page_Enter_Action,
	ACCEPT_EVENT_TAG, BlackListBook_Main_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_Main_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_Main_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_Main_Page = { "BlackListBook_Main_Page", 0, bk_main_msglst };


const PAGE_MSG bk_settings_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_Settings_Page_Enter_Action,
	ACCEPT_EVENT_TAG, BlackListBook_Settings_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_Settings_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_Settings_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_Settings_Page = { "BlackListBook_Settings_Page", 0, bk_settings_msglst };


const PAGE_MSG bk_settings_calls_sms_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_Settings_CallsSMS_Page_Enter_Action,
	ACCEPT_EVENT_TAG, BlackListBook_Settings_CallsSMS_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_Settings_CallsSMS_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_Settings_CallsSMS_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_Settings_CallsSMS_Page = { "BlackListBook_Settings_CallsSMS_Page", 0, bk_settings_calls_sms_msglst };


const PAGE_MSG bk_settingsoom_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_SettingsOOM_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_SettingsOOM_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_SettingsOOM_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_SettingsOOM_Page = { "BlackListBook_SettingsOOM_Page", 0, bk_settingsoom_msglst };


const PAGE_MSG bk_choosemodetoedit_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_ChooseModeToEdit_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_ChooseModeToEdit_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_ChooseModeToEdit_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_ChooseModeToEdit_Page = { "BlackListBook_EditLists_Page", 0, bk_choosemodetoedit_msglst };


const PAGE_MSG bk_chooselisttoedit_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_ChooseListToEdit_Page_Enter_Action,
	CANCEL_EVENT_TAG, BlackListBook_ChooseListToEdit_Page_Cancel_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_ChooseListToEdit_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_ChooseListToEdit_Page = { "BlackListBook_ChooseListToEdit_Page", 0, bk_chooselisttoedit_msglst };


const PAGE_MSG bk_editblocklist_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_EditBlockList_Page_Enter_Action,
	ACCEPT_EVENT_TAG, BlackListBook_EditBlockList_Page_Enter_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_EditBlockList_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_EditBlockList_Page = { "BlackListBook_EditBlockList_Page", 0, bk_editblocklist_msglst };


const PAGE_MSG bk_searchpb_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_SearchPB_Page_Enter_Action,
	ACCEPT_EVENT_TAG, BlackListBook_SearchPB_Page_Accept_Action,
	PREVIOUS_EVENT_TAG, BlackListBook_SearchPB_Page_Prev_Action,
	CANCEL_EVENT_TAG, BlackListBook_SearchPB_Page_Cancel_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_SearchPB_Page = { "BlackListBook_SearchPB_Page", 0, bk_searchpb_msglst };


const PAGE_MSG bk_si_msglst[] @ "DYN_PAGE" =
{
	PAGE_ENTER_EVENT_TAG, BlackListBook_SI_Page_Enter_Action,
	PAGE_EXIT_EVENT_TAG, BlackListBook_SI_Page_Exit_Action,
	NIL_EVENT_TAG, 0
};

const PAGE_DESC BlackListBook_SI_Page = { "BlackListBook_SI_Page", 0, bk_si_msglst };

#endif
