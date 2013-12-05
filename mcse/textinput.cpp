#include "inc\mc.h"
#include "inc\mui.h"

void KillTextInput()
{
  FREE_GUI(MCBook->textinp);
}


void OkTextInput(BOOK *,wchar_t * string,int len)
{
  wstrncpy(wsbuf,string,MAXELEMS(wsbuf)-1);
  if (!MCBook->tstname || TestFileName(wsbuf))
  {
    MCBook->retproc(wsbuf);
    FREE_GUI(MCBook->textinp);
  }
  else
  {
    MsgBoxError(muitxt(ind_err_badname));
    CorFileName(wsbuf);
    TEXTID txt=TextID_Create(wsbuf,ENC_UCS2,TEXTID_ANY_LEN);
    StringInput_DispObject_SetText(GUIObject_GetDispObject(MCBook->textinp),txt);
  }
}


void TextInput(wchar_t *prmpt, int TestFileName, const wchar_t *wsname, IFN_RET_PROC retproc)
{
  TEXTID pr, str;
  pr=TextID_Create(prmpt,ENC_UCS2,TEXTID_ANY_LEN);
  str=TextID_Create(wsname,ENC_UCS2,TEXTID_ANY_LEN);
  MCBook->tstname=TestFileName;
  MCBook->retproc=retproc;
  MCBook->textinp=CreateStringInputVA(0,
                                    VAR_PREV_ACTION_PROC(KillTextInput),
                                    VAR_LONG_BACK_PROC(KillTextInput),
                                    VAR_BOOK(MCBook),
                                    VAR_OK_PROC(OkTextInput),
                                    VAR_STRINP_FIXED_TEXT(pr),
                                    VAR_STRINP_TEXT(str),
                                    VAR_STRINP_MIN_LEN(0),
                                    VAR_STRINP_MAX_LEN(MAX_PATH),
                                    0);
  
}
