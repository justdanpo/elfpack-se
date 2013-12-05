#include "..\include\types.h"

typedef IMAGEID (*GET_IMGID)(IMAGEID old);
typedef void (*GET_OLD_REDR)(DISP_OBJ_ONREDRAW_METHOD);
typedef void (*PAGEHOOK)(void*, BOOK *book, int id);
//typedef int (*ONREDRAW_METHOD)(DISP_OBJ *DO, int a, int b, int c);
typedef struct
{
  BOOK bk;
  DISP_OBJ_ONREDRAW_METHOD redraw;
  GET_IMGID getImgID;
  GET_OLD_REDR getOldRedr;
  PAGEHOOK pageHook;
}MyBOOK;

class CBook : public MyBOOK
{
public:
  CBook(char* pBookName, const PAGE_DESC * bp);
  virtual ~CBook();
  void destroy();
  virtual void onClose(); 

};
