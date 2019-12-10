#ifndef _MYBOOK_H_
#define _MYBOOK_H_

#include "..\include\Lib_Clara.h"
#include "..\classlib\cl.h"
#include "mydisp.h"

class CMyBook:public CBook
{
	u16 timer; // ID таймера
	int lamp; // текуща€ €ркость лампы
	CGuiT<CMyDisp>* gui;

	DECLARE_PAGE_DESC_MEMBER(base_page);
protected:
	~CMyBook();
public:
	static void onTimer (u16 unk , CMyBook* book);
	CMyBook();

	static int Accept(void *msg, CMyBook* book);
	static int Cancel(void *msg, CMyBook* book);
};

#endif
