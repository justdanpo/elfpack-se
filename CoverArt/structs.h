#ifndef _STRUCTS_H_
  #define _STRUCTS_H_

  typedef struct
  {
    wchar_t Title[1024];
    wchar_t Album[1024];
    wchar_t Artist[1024];
    wchar_t Year[1024];
    wchar_t Genre[1024];
    int TrackNum;
    bool HasImage;
    IMAGEID ImageID;
    u16 ImageHandle;
    bool Handle;
  } METADATA;
  
  typedef struct
  {
    int bitrate;
    bool VBR;
    int LengthInSeconds;
    int lngFileSize;
    int id3size;
    int frequency;
    int mode;
  } HEADER_INFO;
  
  typedef struct
  {
    wchar_t *path;
    wchar_t *name;
    HEADER_INFO hdr;
    METADATA meta;
  } MUSIC;
  
  typedef struct
  {
    bool SCROLL_USE;
    int SCROLL_DEF_X;
    int SCROLL_WHERETO;
    int SCROLL_LEFT;
    int SCROLL_RIGHT;
    int SCROLL_SPEED;
    int SCROLL_LENGTH;
    int SCROLL_STRLEN;
    int SCROLL_FLEN;
    int SCROLL_WHERE;
    int SCROLL_NEED_SCROLL;
  }SCROLL;
  
  typedef struct
  {
    wchar_t label[256];
    int str;
    bool SHOW;
    signed int X;
    signed int Y;
    signed int MaxX;
    signed int MaxY;
    int CT;
    int Font;
    int Color;
    int borderColor;
    SCROLL scr;
    int LINE_ON;
    int LINE_DIST;
    int LINE_SIZE;
    int LINE_MINUS;
    int FONT_SIZE;
    bool UsedInAnim;
    bool Hide;
    bool ChangeEveryTime;
  } LABEL;
  
  typedef struct
  {
    IMAGEID ImageID;
    u16 ImageHandle;
    signed int x;
    signed int y;
    signed int mx;
    signed int my;
    bool isImage;
  }UI_IMG;

  typedef struct
  {
    int lblID;
    signed int X;
    signed int Y;
    signed int maxX;
    signed int maxY;
    int CT;
    int Font;
    int Color;
    int Color2;
    int DrawWithNext;
  } ANIM_ELEM;
  
  typedef struct
  {
    LIST *lst;
    int index;
  } ANIM_LIST;
  
  typedef struct
  {
    BOOK * book;
  } MSG;

  typedef struct
  {
    char dummy[0x24];
    int hour;
    int min;
    int sec;
  } PLAY_TIME;

  typedef struct
  {
    wchar_t *path;
    wchar_t *name;
    int fulltime;
    int pos;
    int tracks_count;
  } TRACK_DESC;
  
  typedef struct
  {
    bool title;
    bool artist;
    bool album;
    bool year;
    bool genre;
  }TAG_REQUEST;
  
  typedef struct
  {
    u16 unk;
    u16 unk2;
    wchar_t * path;
    wchar_t * name;
    wchar_t * MIME;
    int fulltime;
  } BOOK_PLAYER_TRACK_DESC;
  
  typedef struct
  {
    IMAGEID ImageID;
    bool HasImage;
    u16 ImageHandle;
    bool Handle;
  } IMG_DESC;
  
  typedef struct
  {
    char dummy[0x5C];
    wchar_t pos;
    char dummy1[0xE];
    int time;
    char dummy2[0x4];
    BOOK_PLAYER_TRACK_DESC * dsc;
    char dummy3[2];
    wchar_t pos2;
    wchar_t pos3;
    char dummy4[22];
    int tracks_count;
  }BOOK_PLAYER_2020;

#pragma pack(1)
  typedef struct
  {
    char dummy[0x5C];
    wchar_t pos;
    wchar_t unk1;
    char dummy2[0x8];
    int time;
    BOOK_PLAYER_TRACK_DESC *dsc;
    wchar_t unk2;
    wchar_t pos2;
    char unk3[12];
    wchar_t tracks_count;
    wchar_t unk4;
    wchar_t tracks_count2;
  } BOOK_PLAYER_2010;
  /*
  typedef struct
  {
    char dummy[0x5C];
    wchar_t pos;
    wchar_t unk2;
    char dummy2[0x8];
    int time;
    BOOK_PLAYER_TRACK_DESC *dsc;
    char unk3[0x10];
    wchar_t tracks_count;
    wchar_t unk4;
    wchar_t tracks_count2;
  } BOOK_PLAYER_2010;
  */
#endif
