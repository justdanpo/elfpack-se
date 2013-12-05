#ifndef __OPENDIALOG_H__
#define __OPENDIALOG_H__

#include <stdarg.h>

typedef struct
{
  BOOK book;
  BOOK * parrent_book;
  int flags;
  wchar_t * filters;
  TEXTID htext;
  int parrent_bookID;
  int folders;
  BOOK * DataBrowserBook;
  LIST * folders_list;
  char * actions;
}OFD_BOOK;

typedef struct
{
  BOOK book;
  int bookID;
}DB_BOOK;

extern const PAGE_DESC odf_msglist;


#define OFD_SHOW_FILES                  0x00000001
#define OFD_SHOW_FOLDERS                0x00000002

#define OFD_SELECT_FOLDER               0x00000004
#define OFD_NEW_FOLDERS_ENABLE          0x00000008
#define OFD_EMPTY_FOLDER_ENABLE         0x00000010

#define OFD_INSERT_ON_OK_KEY            0x00000020


#define OFD_ACT_PREVIEW_ENABLE          0x00010000
#define OFD_ACT_DELETE_ENABLE           0x00020000
#define OFD_ACT_INFO_ENABLE             0x00040000




#define OFD_OPEN_FILE_DIALOG (OFD_SHOW_FILES|OFD_SHOW_FOLDERS|OFD_ACT_PREVIEW_ENABLE|OFD_ACT_INFO_ENABLE)
#define OFD_OPEN_FOLDER_DIALOG (OFD_SHOW_FOLDERS|OFD_SELECT_FOLDER)
#define OFD_SAVE_FILE_DIALOG (OFD_SHOW_FILES|OFD_SHOW_FOLDERS|OFD_NEW_FOLDERS_ENABLE|OFD_ACT_PREVIEW_ENABLE|OFD_ACT_INFO_ENABLE|OFD_EMPTY_FOLDER_ENABLE|OFD_INSERT_ON_OK_KEY)



wchar_t * CreateFileDialog (
                            BOOK * book,
                          int flags,
                          wchar_t * header, // wchar_t либо TEXTID
                          wchar_t * filters,
                          va_list ap  // folders list: wchar_t либо DIR_*
                          );

#endif
