#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "single.h"
#define __time_t_defined
#include <vector>
#include "../classlib/softkeyutil.hpp"

#define ELFNAME "showgui"
#define LELFNAME L"showgui"
#define LELFVERSION L" v0.2\nby den_po"


char myappname[]=ELFNAME; //my process name

void elf_exit(void)
{
    trace_done();
    SUBPROC( mfree_adr(), &ELF_BEGIN );
}


int keyhookproc(int key, int r1 , int mode, LPARAM, DISP_OBJ*)
{
    if(mode==KBD_SHORT_PRESS)
    {
        int k=0;
        if(key==KEY_VOL_UP)k=1;
        if(key==KEY_VOL_DOWN)k=2;
        //if(key==KEY_LEFT)k=1;
        //if(key==KEY_RIGHT)k=2;
        if(k)
        {
            if(DISP_OBJ* dobj = Display_GetFocusedDispObject(0))
            {
                if(char* name = DispObject_GetName(dobj))
                {
                    if(UI_APP_SESSION * apsess=root_list_get_session(root_list_get_session_count()-1))
                    {
                        BOOK *book=SESSION_GetTopBook(apsess);
                        if(book)
                        {
                            wchar_t temp[256];
                            vector<TEXTID> outbuff;

                            outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("book:%s\n"), book->xbook->name)) );
                            outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("current page:%s\n"), book->current_page ? book->current_page->name : "NULL")) );
                            outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("base page:%s\n"), book->base_page ? book->base_page->name : "NULL")) );
                            outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("focused disp:%s\n"), name)) );

                            for(size_t i=0; i<book->xguilist->guilist->FirstFree; i++)
                            {
                        		GUI* gui = reinterpret_cast<GUI*>( List_Get(book->xguilist->guilist, i) );
                        		DISP_OBJ* disp = static_cast<DISP_OBJ*>( GUIObject_GetDispObject(gui) );
                                outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("disp%d:%s\n"), i, DispObject_GetDESC( disp )->name)) );
                            }

                            for(int i=0;i<3;i++)
                            {
                                LIST* softkeys = DispObject_SoftKeys_GetList( dobj, book, i );
                                if( softkeys )
                                {
                                    if(softkeys->FirstFree)
                                    {
                                        const char* sktype[]={"disp","book","system"};
                                        outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("\n%s softkeys:\n"), sktype[i], book->xbook->name)) );
                                    }

                                    for( int i=0; i< softkeys->FirstFree; i++ )
                                    {
                                        CSoftKeyDesc* sk = reinterpret_cast<CSoftKeyDesc*>( List_Get( softkeys, i ) );

                                        outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T(" act:0x%X, v:%d, e:%d, p:%x\n"), sk->GetAction(), sk->GetVisible(), sk->GetEnabled(), sk->GetProc() )) );

                                        TEXTID tmptext = sk->GetButtonText();
                                        outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T(" bt:%x, \""), tmptext )) );
                                        outbuff.push_back( tmptext );

                                        tmptext = sk->GetMenuText();
                                        outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("\"\n mt:%x, \""), tmptext )) );
                                        outbuff.push_back( tmptext );
                                        outbuff.push_back( TextID_Create(temp, ENC_UCS2, snwprintf(temp, MAXELEMS(temp), _T("\"\n\n") )) );
                                    }
                                }
                            }


                            MessageBox(EMPTY_TEXTID, TextID_Create(&outbuff[0], ENC_TEXTID, outbuff.size()), NOIMAGE, 2, 0, 0);
                            return -1;
                        }
                    }
                }
            }
        }
    }
    if(mode==KBD_SHORT_RELEASE)
    {
        if(key==KEY_VOL_UP || key==KEY_VOL_DOWN)
            return -1;
    }
    return 0;
}


void aboutproc()
{
    MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME LELFVERSION,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,5000,0);
}

void exitproc()
{
    ModifyKeyHook(keyhookproc,KEY_HOOK_REMOVE,NULL);
    MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME L" has been stopped",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,1000,0);
    elf_exit();
}

void startproc(bool quiet)
{
    ModifyKeyHook(keyhookproc,KEY_HOOK_ADD,NULL);
    if(!quiet)
        MessageBox(EMPTY_TEXTID,TextID_Create( LELFNAME L" has been started",ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,1000,0);
}

int main (wchar_t* filename)
{
    trace_init(L"showgui_deleaker.txt");
    TASKINSTANCE old=findinstance(myappname);
    if(old)
    {
        //stop it if so
        deleteinstance(old);
        elf_exit();
    }else
    {
        newinstance(myappname);
        startproc(wstrwstr(filename,GetDir(DIR_ELFS_DAEMONS)));
    }
    return 0;
}
