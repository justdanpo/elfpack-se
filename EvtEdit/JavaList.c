#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\structs.h"
#include "header\pages.h"
#include "header\externals.h"

typedef struct
{
  wchar_t * name;
  wchar_t * fullpath;
  int appID;
  IMAGEID imageID;
}java_list_elem;

LIST * java_list;

void onEnter_JavaList(BOOK * book, GUI* )
{
  MyBOOK *mbk=(MyBOOK*)book;
  
  java_list_elem * elem=(java_list_elem *)List_Get(java_list,ListMenu_GetSelectedItem(mbk->javalst));

  DELETE(mbk->selev->param);
  mbk->selev->param=new wchar_t[wstrlen(elem->name)+1];
  wstrcpy(mbk->selev->param,elem->name);
  BookObj_ReturnPage(book,ACCEPT_EVENT);
  BookObj_CallPage(book,&bk_main);
};

void ExitJavaList(BOOK * book, GUI* )
{
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
  BookObj_ReturnPage(book,NIL_EVENT);
};

void DestroyJavaList(BOOK * book, GUI* )
{
  BookObj_ReturnPage(book,NIL_EVENT);
};

void elem_free(void * elem)
{
  java_list_elem * lm=(java_list_elem *)elem;
  DELETE(lm->name);
  DELETE(lm->fullpath);
  if (lm->imageID) ImageID_Free(lm->imageID);
  delete(lm);
};

int elem_filter(void * elem)
{
  if (elem) return(1);
  return(0);
};


int onExit_JavaList(void *data, BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  FREE_GUI(mbk->javalst);
  if (java_list)
  {
    List_DestroyElements(java_list,elem_filter,elem_free);
    List_Destroy(java_list);
    java_list=0;
  }
  return(0);
};


int java_list_callback(GUI_MESSAGE * msg)
{
  switch( GUIonMessage_GetMsg(msg) )
  {
  case 1:
    java_list_elem * elem=(java_list_elem*)List_Get(java_list,GUIonMessage_GetCreatedItemIndex(msg));
    GUIonMessage_SetMenuItemText(msg,TextID_Create(elem->name,ENC_UCS2,TEXTID_ANY_LEN));
    JavaApp_LogoImageID_Get(elem->fullpath,&elem->imageID);
    GUIonMessage_SetMenuItemIcon(msg,0,elem->imageID);
  }
  return(1);
};


java_list_elem * CreateElem(void * JavaDesc)
{
  java_list_elem * elem=new java_list_elem;
  wchar_t * sp;
  JavaAppDesc_GetJavaAppInfo(JavaDesc,0,&sp);
  elem->name=sp;
  JavaAppDesc_GetJavaAppInfo(JavaDesc,5,&sp);
  elem->fullpath=sp;
  elem->appID=JavaAppDesc_GetJavaAppID(JavaDesc);
  return(elem);
};


int CreateJavaList(void *data, BOOK * book)
{
  MyBOOK *mbk=(MyBOOK*)book;
  if (java_list)
  {
    List_DestroyElements(java_list,elem_filter,elem_free);
    List_Destroy(java_list);
  }
  java_list=List_Create();
  char sp1;
  void * JavaDesc;
  JavaDialog_Open(0,&sp1,&JavaDesc);
  if (!JavaAppDesc_GetFirstApp(JavaDesc))
  {
    int result=0;
    while (!result)
    {
      List_InsertLast(java_list,CreateElem(JavaDesc));
      result=JavaAppDesc_GetNextApp(JavaDesc);
    }
  }
  JavaDialog_Close(sp1);
  FREE_GUI(mbk->javalst);
  mbk->javalst=CreateListMenu(book,0);
  GUIObject_SetTitleText(mbk->javalst,GetStrID(L"OAF_APPLICATIONS_TXT"));
  ListMenu_SetItemCount(mbk->javalst,java_list->FirstFree);
  OneOfMany_SetOnMessage((GUI_ONEOFMANY*)mbk->javalst,java_list_callback);
  ListMenu_SetCursorToItem(mbk->javalst,0);
  GUIObject_SoftKeys_SetAction(mbk->javalst,ACTION_BACK,DestroyJavaList);
  GUIObject_SoftKeys_SetAction(mbk->javalst,ACTION_LONG_BACK,ExitJavaList);
  GUIObject_SoftKeys_SetAction(mbk->javalst,ACTION_SELECT1,onEnter_JavaList);
  GUIObject_Show(mbk->javalst);
  return(0);
};

const PAGE_MSG SetJava_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateJavaList,
  PAGE_EXIT_EVENT_TAG,onExit_JavaList,
  NIL_EVENT_TAG,0
};

const PAGE_DESC SetJava_page ={"SetJava_Page",0,SetJava_PageEvents};

