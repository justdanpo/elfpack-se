#ifndef _SOFTKEYUTIL_H_
#define _SOFTKEYUTIL_H_

#include "..\include\lib_clara.h"

class CSoftKeyDesc
{
private:
	CSoftKeyDesc();
	~CSoftKeyDesc();
	static int getstructtype();
	static int structtype;
public:
	char& GetVisible();
	char& GetEnabled();
	u16& GetAction();
	TEXTID& GetButtonText();
	TEXTID& GetMenuText();
	TEXTID& GetHelpText();
	void (*&GetProc())(BOOK*,GUI*);
	void (*&GetDisabledAction())(BOOK*, GUI*);
	LIST*& GetSubitems();
};

CSoftKeyDesc* FindSoftkey( DISP_OBJ* disp, BOOK* book, TEXTID strid, int sktype=0 );
CSoftKeyDesc* FindActionSoftkey( DISP_OBJ* disp, BOOK* book, u16 action, int sktype=0 );

#endif
