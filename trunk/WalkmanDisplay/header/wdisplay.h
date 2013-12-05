
#pragma once

void char2unicode(wchar_t *ws, char const *s, int len);
wchar_t *checkfile_(wchar_t *name, wchar_t *skinpath);

#define __EVENT(x) int x(void *msg, BOOK *book)
#define __NEEDED_BOOK(x) int x(BOOK *book)

#define __ONKEY_METHOD(x) int x(int key, int, int mode)

#pragma pack(1)
typedef  struct
{
  char dummy[0xB6];
  wchar_t imageID;
}DISP_OBJ_WALKMAN;
#pragma pack()

#define FORMAT_MP3 1
#define FORMAT_M4A 2
#define FORMAT_WMA 3
#define FORMAT_OTHER 4

typedef struct
{
  BOOK * bookmanbook;
}BOOKMANMSG;

extern wchar_t intFolder[512];
extern wchar_t extFolder[512];

typedef struct
{
  wchar_t Title[1024];
  wchar_t Album[1024];
  wchar_t Artist[1024];
  wchar_t Year[1024];
  wchar_t Genre[1024];
  int TrackNum;
  IMAGEID ImageID;
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
  HEADER_INFO hdr;
  METADATA meta;
  int format;
} MUSIC;



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
  wchar_t ImageID;
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
  wchar_t tracks_count;
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
#pragma pack()
