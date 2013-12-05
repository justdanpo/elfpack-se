#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "config_data.h"
#include "conf_loader.h"

#define FA_DIRECTORY 0x4000

#define INDEX(x) x/8
#define SHIFT(x) x%8
#define GETBIT(v,x) ((v[INDEX(x)] << SHIFT(x)) & 0x80)>>7

#define INFO_ALBUM L"Альбом: "
#define INFO_ARTIST L"Исполнитель: "
#define INFO_COMMENT L"Комментарий: "
#define INFO_FILENAME L"Имя файла: "
#define INFO_GENRE L"Жанр: "
#define INFO_LOCALISATION L"Расположение: "
#define INFO_TEXT L"Инфо mp3"
#define INFO_TITLE L"Название: "
#define INFO_YEAR L"Год: "
#define INPUT_ALBUM L"Альбом:"
#define INPUT_ARTIST L"Исполнитель:"
#define INPUT_COMMENT L"Комментарий:"
#define INPUT_TITLE L"Название:"
#define INPUT_YEAR L"Год:"
#define MENU_ITEM_TEXT_ALBUM L"Альбом"
#define MENU_ITEM_TEXT_ARTIST L"Исполнитель"
#define MENU_ITEM_TEXT_COMMENT L"Комментарий"
#define MENU_ITEM_TEXT_GENRE L"Жанр"
#define MENU_ITEM_TEXT_INFO L"Инфо"
#define MENU_ITEM_TEXT_TITLE L"Название"
#define MENU_ITEM_TEXT_YEAR L"Год"
#define NAME_EDIT1 L"Ред. ID3v1"
#define NAME_EDIT2 L"Ред. ID3v2"
#define NAME_GENRE L"Выб. жанр"
#define NAME_OPEN_FILE L"Открыть файл"
#define NAME_TAG_EDITOR L"Редактор тегов"
#define QUESTION_SAVE_FILE L"Сохранить перед выходом?"
#define RENAME_ARTIST_NOT_FOUND L"Дорожка"
#define RENAME_TITLE_NOT_FOUND L"Неизвестно"
#define SOFTKEY_STATUS_PLAY L"Плей"
#define SOFTKEY_STATUS_STOP L"Стоп"

typedef struct
{
  BOOK book;
  char * Album;
  char * Album2;
  char * Autor;
  char * Autor2;
  char * buffer;
  char * buffer2;
  char * Comment;
  char * newname;
  char * Title;
  char * Title2;
  char * Year;
  GUI * InputAlbum;
  GUI * InputAlbum2;
  GUI * InputAutor;
  GUI * InputAutor2;
  GUI * InputComment;
  GUI * InputTitle;
  GUI * InputTitle2;
  GUI * InputYear;
  GUI * yn;
  GUI_LIST * GlavMenu;
  GUI_LIST * GList;
  GUI_LIST * MenuList1;
  GUI_LIST * MenuList2;
  int change;
  int genre;
  int i2;
  int ID3;
  int ID3v1;
  int len_id3v2;
  int lenn;
  int lenp;
  int PlayMode;
  int pos2_Album;
  int pos2_Autor;
  int pos2_Title;
  int PosAlbum;
  int posAlbum2;
  int PosAutor;
  int posAutor2;
  int PosComment;
  int PosTitle;
  int posTitle2;
  int PosYear;
  int Run;
  int size;
  int size_id3;
  int size2;
  int sizeAlbum2;
  int sizeAutor2;
  int sizeTitle2;
  DISP_OBJ_ONKEY_METHOD OldKey;
  DISP_OBJ_ONKEY_METHOD OldKey1;
  DISP_OBJ_ONKEY_METHOD OldKey2;
  wchar_t * name;
  wchar_t * path;
  wchar_t * TON;
  wchar_t * UAlbum;
  wchar_t * UAlbum2;
  wchar_t * UAutor;
  wchar_t * UAutor2;
  wchar_t * Ubuffer2;
  wchar_t * UComment;
  wchar_t * UGenre;
  wchar_t * Unewname;
  wchar_t * UTitle;
  wchar_t * UTitle2;
  wchar_t * UYear;
  wchar_t PlayText[10];
}MyBook;

typedef struct
{
  BOOK * book;
}MSG;

void FileRead(wchar_t*path, wchar_t*name);
int onAccept_DB(void * data, BOOK * book);
void CreateGlavMenu(int x);
void CreateML1(int x);
void CreateML2(int x);
MyBook * TagBook;

int TerminateElf(void * ,BOOK* book)
{
  FreeBook(book);
  return(1);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("TagEditor v1.4\n(c) MoneyMasteR\naka\nDoCent"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onCloseCreateFileBook(BOOK *book)
{
  if(TagBook->PlayMode==1)
  {
    PlaySystemSound(7);
  }
  SUBPROC(elf_exit);
}

static int onReconfigElf(void *mess ,BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  int result=0;
  if (wstrcmpi(reconf->path,successed_config_path)==0 && wstrcmpi(reconf->name,successed_config_name)==0)
  {
    InitConfig();
    result=1;
  }
  return(result);
}



int synchsafeToNormal(char tagSize[4])
{
  int synchsafe, sizeloc, size, power, x;    
  size=sizeloc=0;      
  for(synchsafe=31;synchsafe>=0;synchsafe--)
  {        
    if(GETBIT(tagSize, synchsafe))
    {          
      power=1;            
      for(x=0;x<sizeloc;x++) power*=2;		
      size+=power;		
    }		
    if(synchsafe%8) sizeloc++;	
  }	
  return size;
};

void win12512unicode(wchar_t *ws, char *s, int len)
{
  int c;
  while((c=*s++)&&((len--)>0))
  {
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    *ws++=c;
  }
  *ws=0;
}

char * unicode2win1251(char *s, wchar_t *ws, int len)
{
  char *d=s;
  int c;
  while((c=*ws++)&&((len--)>0))
  {
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    *s++=c;
  }
  *s=0;
  return(d);
}

char * GetGenre(int num)
{
  switch (num) 
  {
    case 0 : return "Blues";
    case 1 : return "Classic Rock";
    case 2 : return "Country";
    case 3 : return "Dance";
    case 4 : return "Disco";
    case 5 : return "Funk";
    case 6 : return "Grunge";
    case 7 : return "Hip-Hop";
    case 8 : return "Jazz";
    case 9 : return "Metal";
    case 10 : return "New Age";
    case 11 : return "Oldies";
    case 12 : return "Other";
    case 13 : return "Pop";
    case 14 : return "R&B";
    case 15 : return "Rap";
    case 16 : return "Reggae";
    case 17 : return "Rock";
    case 18 : return "Techno";
    case 19 : return "Industrial";
    case 20 : return "Alternative";
    case 21 : return "Ska";
    case 22 : return "Death Metal";
    case 23 : return "Pranks";
    case 24 : return "Soundtrack";
    case 25 : return "Euro-Techno";
    case 26 : return "Ambient";
    case 27 : return "Trip-Hop";
    case 28 : return "Vocal";
    case 29 : return "Jazz+Funk";
    case 30 : return "Fusion";
    case 31 : return "Trance";
    case 32 : return "Classical";
    case 33 : return "Instrumental";
    case 34 : return "Acid";
    case 35 : return "House";
    case 36 : return "Game";
    case 37 : return "Sound Clip";
    case 38 : return "Gospel";
    case 39 : return "Noise";
    case 40 : return "Alternative Rock";
    case 41 : return "Bass";
    case 42 : return "Soul";
    case 43 : return "Punk";
    case 44 : return "Space";
    case 45 : return "Meditative";
    case 46 : return "Instrumental Pop";
    case 47 : return "Instrumental Rock";
    case 48 : return "Ethnic";
    case 49 : return "Gothic";
    case 50 : return "Darkwave";
    case 51 : return "Techno-Industrial";
    case 52 : return "Electronic";
    case 53 : return "Pop-Folk";
    case 54 : return "Eurodance";
    case 55 : return "Dream";
    case 56 : return "Southern Rock";
    case 57 : return "Comedy";
    case 58 : return "Cult";
    case 59 : return "Gangsta";
    case 60 : return "Top 40";
    case 61 : return "Christian Rap";
    case 62 : return "Pop/Funk";
    case 63 : return "Jungle";
    case 64 : return "Native US";
    case 65 : return "Cabaret";
    case 66 : return "New Wave";
    case 67 : return "Psychadelic";
    case 68 : return "Rave";
    case 69 : return "Showtunes";
    case 70 : return "Trailer";
    case 71 : return "Lo-Fi";
    case 72 : return "Tribal";
    case 73 : return "Acid Punk";
    case 74 : return "Acid Jazz";
    case 75 : return "Polka";
    case 76 : return "Retro";
    case 77 : return "Musical";
    case 78 : return "Rock & Roll";
    case 79 : return "Hard Rock";
    case 80 : return "Folk";
    case 81 : return "Folk-Rock";
    case 82 : return "National Folk";
    case 83 : return "Swing";
    case 84 : return "Fast Fusion";
    case 85 : return "Bebob";
    case 86 : return "Latin";
    case 87 : return "Revival";
    case 88 : return "Celtic";
    case 89 : return "Bluegrass";
    case 90 : return "Avantgarde";
    case 91 : return "Gothic Rock";
    case 92 : return "Progressive Rock";
    case 93 : return "Psychedelic Rock";
    case 94 : return "Symphonic Rock";
    case 95 : return "Slow Rock";
    case 96 : return "Big Band";
    case 97 : return "Chorus";
    case 98 : return "Easy Listening";
    case 99 : return "Acoustic";
    case 100 : return "Humour";
    case 101 : return "Speech";
    case 102 : return "Chanson";
    case 103 : return "Opera";
    case 104 : return "Chamber Music";
    case 105 : return "Sonata";
    case 106 : return "Symphony";
    case 107 : return "Booty Bass";
    case 108 : return "Primus";
    case 109 : return "Porn Groove";
    case 110 : return "Satire";
    case 111 : return "Slow Jam";
    case 112 : return "Club";
    case 113 : return "Tango";
    case 114 : return "Samba";
    case 115 : return "Folklore";
    case 116 : return "Ballad";
    case 117 : return "Power Ballad";
    case 118 : return "Rhytmic Soul";
    case 119 : return "Freestyle";
    case 120 : return "Duet";
    case 121 : return "Punk Rock";
    case 122 : return "Drum Solo";
    case 123 : return "Acapella";
    case 124 : return "Euro-House";
    case 125 : return "Dance Hall";
    case 126 : return "Goa";
    case 127 : return "Drum & Bass";
    case 128 : return "Club-House";
    case 129 : return "Hardcore";
    case 130 : return "Terror";
    case 131 : return "Indie";
    case 132 : return "BritPop";
    case 133 : return "Negerpunk";
    case 134 : return "Polsk Punk";
    case 135 : return "Beat";
    case 136 : return "Christian Gangsta";
    case 137 : return "Heavy Metal";
    case 138 : return "Black Metal";
    case 139 : return "Crossover";
    case 140 : return "Contemporary C";
    case 141 : return "Christian Rock";
    case 142 : return "Merengue";
    case 143 : return "Salsa";
    case 144 : return "Thrash Metal";
    case 145 : return "Anime";
    case 146 : return "JPop";
    case 147 : return "SynthPop";
  }
  return " ";
}

void FileWrite()
{
  int f=_fopen(TagBook->path,TagBook->name,FSX_O_WRONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  for(int i=10; i<=TagBook->size_id3-1; i++)
  {
    TagBook->buffer2[i]=0;
  }
  if(TagBook->Title2!=0) delete TagBook->Title2;
  if(wstrlen(TagBook->UTitle2)==0)
  {
    TagBook->UTitle2=new wchar_t[wstrlen(L"  ")+1];
    wstrcpy(TagBook->UTitle2, L"  ");
  }
  TagBook->Title2=new char[wstrlen(TagBook->UTitle2)+1];
  unicode2win1251(TagBook->Title2, TagBook->UTitle2, wstrlen(TagBook->UTitle2));
  if(TagBook->Autor2!=0) delete TagBook->Autor2;
  if(wstrlen(TagBook->UAutor2)==0)
  {
    TagBook->UAutor2=new wchar_t[wstrlen(L"  ")+1];
    wstrcpy(TagBook->UAutor2, L"  ");
  }
  TagBook->Autor2=new char[wstrlen(TagBook->UAutor2)+1];
  unicode2win1251(TagBook->Autor2, TagBook->UAutor2, wstrlen(TagBook->UAutor2));
  if(TagBook->Album2!=0) delete TagBook->Album2;
  if(wstrlen(TagBook->UAlbum2)==0)
  {
    TagBook->UAlbum2=new wchar_t[wstrlen(L"  ")+1];
    wstrcpy(TagBook->UAlbum2, L"  ");
  }  
  TagBook->Album2=new char[wstrlen(TagBook->UAlbum2)+1];
  unicode2win1251(TagBook->Album2, TagBook->UAlbum2, wstrlen(TagBook->UAlbum2));
  int size=strlen(TagBook->Title2)+strlen(TagBook->Autor2)+strlen(TagBook->Album2)+11*3+10;
  char*write=new char[size+1];
  for(int i=0; i<=size-1; i++)
  {
    write[i]=0;
  }
  int pos=0;
  write[pos]='T';
  pos++;
  write[pos]='I';
  pos++;
  write[pos]='T';
  pos++;
  write[pos]='2';
  pos++;
  pos++;
  pos++;
  pos++;
  write[pos]=strlen(TagBook->Title2)+1;
  pos++;
  pos++;
  pos++;
  for(int i=0; i<=strlen(TagBook->Title2)-1; i++)
  {
    pos++;
    write[pos]=TagBook->Title2[i];
  }
  pos++;
  write[pos]='T';
  pos++;
  write[pos]='P';
  pos++;
  write[pos]='E';
  pos++;
  write[pos]='1';
  pos++;
  pos++;
  pos++;
  pos++;
  write[pos]=strlen(TagBook->Autor2)+1;
  pos++;
  pos++;
  pos++;
  for(int i=0; i<=strlen(TagBook->Autor2)-1; i++)
  {
    pos++;
    write[pos]=TagBook->Autor2[i];
  }
  pos++;
  write[pos]='T';
  pos++;
  write[pos]='A';
  pos++;
  write[pos]='L';
  pos++;
  write[pos]='B';
  pos++;
  pos++;
  pos++;
  pos++;
  write[pos]=strlen(TagBook->Album2)+1;
  pos++;
  pos++;
  pos++;
  for(int i=0; i<=strlen(TagBook->Album2)-1; i++)
  {
    pos++;
    write[pos]=TagBook->Album2[i];
  }
  for(int i=0; i<=pos; i++){
    TagBook->buffer2[i+10]=write[i];
  }
  fwrite(f, TagBook->buffer2, TagBook->size_id3);
  for(int i=0; i<=29; i++)
  {
    TagBook->Title[i]=0;
    TagBook->Autor[i]=0;
    TagBook->Album[i]=0;
    TagBook->Comment[i]=0;
  }
  for(int i=0; i<=3; i++)
  {
    TagBook->Year[i]=0;
  }
  unicode2win1251(TagBook->Title,TagBook->UTitle,30);
  unicode2win1251(TagBook->Autor,TagBook->UAutor,30);
  unicode2win1251(TagBook->Album,TagBook->UAlbum,30);
  unicode2win1251(TagBook->Year,TagBook->UYear,30);
  unicode2win1251(TagBook->Comment,TagBook->UComment,30);
  for(int i=0; i<=3; i++)
  {
    TagBook->buffer[TagBook->PosYear+i]=TagBook->Year[i];
  }
  for(int i=0; i<=29; i++)
  {
    TagBook->buffer[TagBook->PosTitle+i]=TagBook->Title[i];
    TagBook->buffer[TagBook->PosAutor+i]=TagBook->Autor[i];
    TagBook->buffer[TagBook->PosAlbum+i]=TagBook->Album[i];
    TagBook->buffer[TagBook->PosComment+i]=TagBook->Comment[i];
  }
  TagBook->buffer[127]=TagBook->genre;
  lseek(f, TagBook->size-TagBook->ID3v1*128, 0);
  fwrite(f, TagBook->buffer, 128);  
  fclose(f);
  if(TagBook->name[TagBook->lenn-1]=='t'&&MptToMp3)
  {
    rename(TagBook->path, TagBook->name, TagBook->path, TagBook->Unewname,0);
    TagBook->name[TagBook->lenn-1]='3';
  }
  if(RenameFile)
  {
    wchar_t*new_name;
    if(FromRename==0)
    {
      new_name=new wchar_t[255+255+255+wstrlen(L".mp3")+150];
      if(wstrlen(TagBook->UAutor))
      {
        wstrcpy(new_name, TagBook->UAutor);
      }
      else
      {
        if(wstrcmp(TagBook->UAutor2, L"  "))
        {
          wstrcpy(new_name, TagBook->UAutor2);
        }
        else
        {
          wstrcpy(new_name, RENAME_ARTIST_NOT_FOUND);
        }
      }
      wstrcat(new_name, L" - ");
      if(wstrlen(TagBook->UTitle))
      {
        wstrcat(new_name, TagBook->UTitle);
      }
      else
      {
        if(wstrcmp(TagBook->UTitle2, L"  "))
        {
          wstrcpy(new_name, TagBook->UTitle2);
        }
        else
        {
          wstrcat(new_name, RENAME_TITLE_NOT_FOUND);
        }
      }
      if(wstrlen(TagBook->UAlbum))
      {
        wstrcat(new_name, L" (");
        wstrcat(new_name, TagBook->UAlbum);
        wstrcat(new_name, L").mp3");
      }
      else
      {
        if(wstrlen(TagBook->UAlbum2))
        {
          wstrcat(new_name, L" (");
          wstrcat(new_name, TagBook->UAlbum2);
          wstrcat(new_name, L").mp3");
        }
        else
        {
          wstrcat(new_name, L".mp3");
        }
      }
    }
    rename(TagBook->path, TagBook->name, TagBook->path, new_name,0);
  }
  PAudioControl pAC = AudioControl_Init();
  if( !pAC ) pAC = *GetAudioControlPtr();
  AudioControl_Vibrate(pAC, 400, 50, 1000);
}

void DestroyInputBooks2()
{
  if(TagBook->InputTitle2)
  {
    GUIObject_Destroy(TagBook->InputTitle2);
    TagBook->InputTitle2=0;
  }
  if(TagBook->InputAlbum2)
  {
    GUIObject_Destroy(TagBook->InputAlbum2);
    TagBook->InputAlbum2=0;
  }
  if(TagBook->InputAutor2)
  {
    GUIObject_Destroy(TagBook->InputAutor2);
    TagBook->InputAutor2=0;
  }
}

void OnEntInputTitle2(BOOK *,wchar_t * string,int len)
{
  if(TagBook->UTitle2) delete TagBook->UTitle2;
  TagBook->UTitle2=new wchar_t[len+1];
  wstrcpy(TagBook->UTitle2, string);
  TagBook->change++;
  DestroyInputBooks2();
  GUIObject_Show(TagBook->MenuList2);
}

void OnEntInputAutor2(BOOK *,wchar_t * string,int len)
{
  if(TagBook->UAutor2) delete TagBook->UAutor2;
  TagBook->UAutor2=new wchar_t[len+1];
  wstrcpy(TagBook->UAutor2, string);
  TagBook->change++;
  DestroyInputBooks2();
  GUIObject_Show(TagBook->MenuList2);
}

void OnEntInputAlbum2(BOOK *,wchar_t * string,int len)
{
  if(TagBook->UAlbum2) delete TagBook->UAlbum2;
  TagBook->UAlbum2=new wchar_t[len+1];
  wstrcpy(TagBook->UAlbum2, string);
  TagBook->change++;
  DestroyInputBooks2();
  GUIObject_Show(TagBook->MenuList2);
}

void InputTitleF2()
{
  TagBook->InputTitle2=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks2),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks2),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INFO_TITLE, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UTitle2, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(255),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputTitle2),
                                       0);                                      
                                       
}

void InputAutorF2()
{
    TagBook->InputAutor2=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks2),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks2),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_ARTIST, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UAutor2, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(255),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputAutor2),
                                       0);   
}

void InputAlbumF2()
{
    TagBook->InputAlbum2=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks2),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks2),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_ALBUM, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UAlbum2, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(255),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputAlbum2),
                                       0); 
}

void OnEntML2( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(TagBook->MenuList2);
  if(item==0)
  {
    InputTitleF2();
  }
  if(item==1)
  {
    InputAutorF2();
  }
  if(item==2)
  {
    InputAlbumF2();
  }
}

void ExitML2( BOOK*, GUI* )
{
  GUIObject_Destroy(TagBook->MenuList2);
  TagBook->MenuList2=0;
  CreateGlavMenu(1);
}

int SetTitleML2(GUI_MESSAGE * msg)
{
  int item;
  int imgID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_TITLE,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UTitle2,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconr,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==1)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_ARTIST,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UAutor2,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icont,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==2)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_ALBUM,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UAlbum2,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icony,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }      
  }
  return(1);        
}

void PlayF2( BOOK* bk, GUI* )
{
  if(TagBook->PlayMode==0)
  {
    PlayFile(TagBook->path,TagBook->name);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_STOP);
    GUIObject_SoftKeys_SetText(TagBook->MenuList2,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=1;
  }
  else
  {
    PlaySystemSound(7);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_PLAY);
    GUIObject_SoftKeys_SetText(TagBook->MenuList2,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=0;
  }
}

void DeleteTag2(int x){
  int i;
  if(x==0)
  {
    for(i=0; i<=(wstrlen(TagBook->UTitle2)-1); i++)
    {
      TagBook->UTitle2[i]=0;
    }
  }
  if(x==1)
  {
    for(i=0; i<=(wstrlen(TagBook->UAutor2)-1); i++)
    {
      TagBook->UAutor2[i]=0;
    }
  }
  if(x==2)
  {
    for(i=0; i<=(wstrlen(TagBook->UAlbum2)-1); i++)
    {
      TagBook->UAlbum2[i]=0;
    }
  }
  TagBook->change++;
  if(TagBook->MenuList2) GUIObject_Destroy(TagBook->MenuList2);
  CreateML2(x);
}

int ShowInfo()
{
  wchar_t * info_mp3=new wchar_t[wstrlen(TagBook->path)+1+wstrlen(TagBook->name)+1+wstrlen(TagBook->UTitle)+1+wstrlen(TagBook->UAutor)+1+wstrlen(TagBook->UAlbum)+1+wstrlen(TagBook->UYear)+1+wstrlen(TagBook->UComment)+1+strlen(GetGenre(TagBook->genre))+1+150+wstrlen(TagBook->UTitle2)+1+wstrlen(TagBook->UAutor2)+1+wstrlen(TagBook->UAlbum2)+100];
  wstrcpy(info_mp3,INFO_FILENAME);
  wstrcat(info_mp3,TagBook->name);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_LOCALISATION);
  wstrcat(info_mp3,TagBook->path);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,L"ID3v1:\n");
  wstrcat(info_mp3,INFO_TITLE);
  wstrcat(info_mp3,TagBook->UTitle);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_ARTIST);
  wstrcat(info_mp3,TagBook->UAutor);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_ALBUM);
  wstrcat(info_mp3,TagBook->UAlbum);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_YEAR);
  wstrcat(info_mp3,TagBook->UYear);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_COMMENT);
  wstrcat(info_mp3,TagBook->UComment);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_GENRE);  
  wchar_t*Gen=new wchar_t[strlen(GetGenre(TagBook->genre))+1];
  win12512unicode(Gen,GetGenre(TagBook->genre),strlen(GetGenre(TagBook->genre)));
  wstrcat(info_mp3,Gen);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,L"ID3v2:\n");
  wstrcat(info_mp3,INFO_TITLE);
  wstrcat(info_mp3,TagBook->UTitle2);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_ARTIST);
  wstrcat(info_mp3,TagBook->UAutor2);
  wstrcat(info_mp3,L"\n");
  wstrcat(info_mp3,INFO_ALBUM);
  wstrcat(info_mp3,TagBook->UAlbum2);
  wstrcat(info_mp3,L"\n");
  MessageBox(TextID_Create(INFO_TEXT,ENC_UCS2,TEXTID_ANY_LEN),TextID_Create(info_mp3,ENC_UCS2,TEXTID_ANY_LEN), NOIMAGE, 2 ,0,0);
  delete info_mp3;
  delete Gen;
  return 1;
}

void CopyFromName2(int x)
{
  int i;
  if(x==0)
  {
    if(wstrlen(TagBook->name)<=0xff)
    {
      TagBook->UTitle2=new wchar_t[wstrlen(TagBook->name)+1];
      wstrcpy(TagBook->UTitle2, TagBook->name);
    }
    else
    {
      TagBook->UTitle2=new wchar_t[255+1];
      for(i=0; i<=255; i++)
      {
        TagBook->UTitle2[i]=TagBook->name[i];
      }
    }      
  }
  if(x==1)
  {
    if(wstrlen(TagBook->name)<=0xff)
    {
      TagBook->UAutor2=new wchar_t[wstrlen(TagBook->name)+1];
      wstrcpy(TagBook->UAutor2, TagBook->name);
    }
    else
    {
      TagBook->UAutor2=new wchar_t[255+1];
      for(i=0; i<=255; i++)
      {
        TagBook->UAutor2[i]=TagBook->name[i];
      }
    }
  }
  if(x==2)
  {
    if(wstrlen(TagBook->name)<=0xff)
    {
      TagBook->UAlbum2=new wchar_t[wstrlen(TagBook->name)+1];
      wstrcpy(TagBook->UAlbum2, TagBook->name);
    }
    else
    {
      TagBook->UAlbum2=new wchar_t[255+1];
      for(i=0; i<=255; i++)
      {
        TagBook->UAlbum2[i]=TagBook->name[i];
      }
    }
  }
  TagBook->change++;
  if(TagBook->MenuList2) GUIObject_Destroy(TagBook->MenuList2);
  CreateML2(x);
}

void CopyFromID3v1(int x)
{
  int i;
  if(x==0)
  {
    TagBook->UTitle2=new wchar_t[31];
    for(i=0; i<=29; i++)
    {
      TagBook->UTitle2[i]=TagBook->UTitle[i];
    }
  }
  if(x==1)
  {
    TagBook->UAutor2=new wchar_t[31];
    for(i=0; i<=29; i++)
    {
      TagBook->UAutor2[i]=TagBook->UAutor[i];
    }
  }
  if(x==2)
  {
    TagBook->UAlbum2=new wchar_t[31];
    for(i=0; i<=29; i++)
    {
      TagBook->UAlbum2[i]=TagBook->UAlbum[i];
    }
  }
  TagBook->change++;
  if(TagBook->MenuList2) GUIObject_Destroy(TagBook->MenuList2);
  CreateML2(x);
}
  
void NewKey2(DISP_OBJ * p, int i1, int i2, int i3, int i4)
{
  TagBook->OldKey2(p, i1, i2, i3, i4);
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DEL)
  {
    int sel=ListMenu_GetSelectedItem(TagBook->MenuList2);
    if(sel<=2)
    {
      DeleteTag2(sel);
    }
  }
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DIGITAL_0)
  {
    PlayF2((BOOK*)TagBook,0);
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+1))
  {
    InputTitleF2();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+2))
  {
    InputAutorF2();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+3))
  {
    InputAlbumF2();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+10))
  {
    ShowInfo();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+11))
  {
    CopyFromID3v1(ListMenu_GetSelectedItem(TagBook->MenuList2));
  }
  if(i4==KBD_LONG_PRESS && i1==(KEY_DIGITAL_0+11))
  {
    CopyFromName2(ListMenu_GetSelectedItem(TagBook->MenuList2));
  }
}

void CreateML2(int x)
{
  GUI_LIST*MList2=CreateListMenu((BOOK*)TagBook,0);
  TagBook->MenuList2=MList2;
  GUIObject_SetTitleText(MList2, TextID_Create(NAME_EDIT2,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(MList2, 3);
  OneOfMany_SetOnMessage(MList2,SetTitleML2);
  ListMenu_SetCursorToItem(MList2,x);
  ListMenu_SetItemStyle(MList2,3);
  GUIObject_SoftKeys_SetAction(MList2,ACTION_BACK,ExitML2);
  GUIObject_SoftKeys_SetAction(MList2,ACTION_LONG_BACK,ExitML2);
  GUIObject_SoftKeys_SetAction(MList2,ACTION_SELECT1,OnEntML2);	
  GUIObject_SoftKeys_SetAction(MList2,1,PlayF2);
  GUIObject_SoftKeys_SetText(MList2,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
  TagBook->OldKey2 = DispObject_GetOnKey( GUIObject_GetDispObject(MList2) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(MList2) ), NewKey2);
  GUIObject_Show(MList2);
}

void ExitMG( BOOK*, GUI* )
{
  GUIObject_Destroy(TagBook->GList);
  TagBook->GList=0;
  GUIObject_Show(TagBook->MenuList1);
}

void OnEntMG( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(TagBook->GList);
  for(int i=0;i<=147;i++)
  {
    if(item==i)
    {
      TagBook->genre=i;
    }
  }
  TagBook->change++;
  ExitMG(bk,0);
}

int SetTitlePunktsMG(GUI_MESSAGE * msg)
{
  int item;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      for(int i=0; i<=147; i++)
      {
        if(item==i)
        {
          wchar_t * Genre=new wchar_t[strlen(GetGenre(i))+1];
          win12512unicode(Genre,GetGenre(i),strlen(GetGenre(i))+1);
          GUIonMessage_SetMenuItemText(msg,TextID_Create(Genre,ENC_UCS2,TEXTID_ANY_LEN));
          delete Genre;
        }
      }
  }
  return(1);
}
      
int MGList()
{
  GUI_LIST*MGList=CreateListMenu((BOOK*)TagBook,0);
  TagBook->GList=MGList;
  GUIObject_SetTitleText(MGList, TextID_Create(NAME_GENRE,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(MGList, 148);
  OneOfMany_SetOnMessage(MGList,SetTitlePunktsMG);
  ListMenu_SetCursorToItem(MGList,TagBook->genre);
  GUIObject_SoftKeys_SetAction(MGList,ACTION_BACK,ExitMG);
  GUIObject_SoftKeys_SetAction(MGList,ACTION_LONG_BACK,ExitMG);
  GUIObject_SoftKeys_SetAction(MGList,ACTION_SELECT1,OnEntMG);	
  GUIObject_Show(MGList);
  return(0);
}

void DestroyInputBooks()
{
  if(TagBook->InputTitle)
  {
    GUIObject_Destroy(TagBook->InputTitle);
    TagBook->InputTitle=0;
  }
  if(TagBook->InputAlbum)
  {
    GUIObject_Destroy(TagBook->InputAlbum);
    TagBook->InputAlbum=0;
  }
  if(TagBook->InputAutor)
  {
    GUIObject_Destroy(TagBook->InputAutor);
    TagBook->InputAutor=0;
  }
  if(TagBook->InputYear)
  {
    GUIObject_Destroy(TagBook->InputYear);
    TagBook->InputYear=0;
  }
  if(TagBook->InputComment)
  {
    GUIObject_Destroy(TagBook->InputComment);
    TagBook->InputComment=0;
  }
}

void OnEntInputTitle(BOOK *,wchar_t * string,int len)
{
  for(int i=0; i<=29; i++)
  {
    TagBook->UTitle[i]=0;
  }
  for(int i=0; i<=len; i++)
  {
    TagBook->UTitle[i]=string[i];
  }
  TagBook->change++;
  DestroyInputBooks();
  GUIObject_Show(TagBook->MenuList1);
}

void OnEntInputAutor(BOOK *,wchar_t * string,int len)
{
  for(int i=0; i<=29; i++)
  {
    TagBook->UAutor[i]=0;
  }
  for(int i=0; i<=len; i++)
  {
    TagBook->UAutor[i]=string[i];
  }
  TagBook->change++;
  DestroyInputBooks();
  GUIObject_Show(TagBook->MenuList1);
}

void OnEntInputAlbum(BOOK *,wchar_t * string,int len)
{
  for(int i=0; i<=29; i++)
  {
    TagBook->UAlbum[i]=0;
  }
  for(int i=0; i<=len; i++)
  {
    TagBook->UAlbum[i]=string[i];
  }
  TagBook->change++;
  DestroyInputBooks();
  GUIObject_Show(TagBook->MenuList1);
}

void OnEntInputYear(BOOK *,wchar_t * string,int len)
{
  for(int i=0; i<=3; i++)
  {
    TagBook->UYear[i]=0;
  }
  for(int i=0; i<=len; i++)
  {
    TagBook->UYear[i]=string[i];
  }
  TagBook->change++;
  DestroyInputBooks();
  GUIObject_Show(TagBook->MenuList1);
}

void OnEntInputComment(BOOK *,wchar_t * string,int len)
{
  for(int i=0; i<=29; i++)
  {
    TagBook->UComment[i]=0;
  }
  for(int i=0; i<=len; i++)
  {
    TagBook->UComment[i]=string[i];
  }
  TagBook->change++;
  DestroyInputBooks();
  GUIObject_Show(TagBook->MenuList1);
}

void InputTitleF()
{
  TagBook->InputTitle=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_TITLE, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UTitle, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(30),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputTitle),
                                       0);                                      
                                       
}

void InputAutorF()
{
    TagBook->InputAutor=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_ARTIST, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UAutor, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(30),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputAutor),
                                       0);   
}

void InputAlbumF()
{
    TagBook->InputAlbum=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_ALBUM, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UAlbum, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(30),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputAlbum),
                                       0); 
}

void InputYearF()
{
    TagBook->InputYear=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_YEAR, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UYear, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(4),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputYear),
                                       0); 
}

void InputCommentF()
{
  TagBook->InputComment=CreateStringInputVA(0,
                                       VAR_PREV_ACTION_PROC(DestroyInputBooks),
                                       VAR_LONG_BACK_PROC(DestroyInputBooks),
                                       VAR_BOOK(TagBook),
                                       VAR_STRINP_FIXED_TEXT(TextID_Create(INPUT_COMMENT,ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_TEXT(TextID_Create(TagBook->UComment, ENC_UCS2, TEXTID_ANY_LEN)),
                                       VAR_STRINP_MAX_LEN(30),
                                       VAR_STRINP_MIN_LEN(0),
                                       VAR_OK_PROC(OnEntInputComment),
                                       0); 
}

void OnEntML1( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(TagBook->MenuList1);
  if(item==0)
  {
    InputTitleF();
  }
  if(item==1)
  {
    InputAutorF();
  }
  if(item==2)
  {
    InputAlbumF();
  }
  if(item==3){
    InputYearF();
  }
  if(item==4){
    InputCommentF();
  }
  if(item==5){
    MGList();
  }
}

void ExitML1( BOOK*, GUI* )
{
  GUIObject_Destroy(TagBook->MenuList1);
  TagBook->MenuList1=0;
  CreateGlavMenu(0);
}

int SetTitleML1(GUI_MESSAGE * msg)
{
  int item;
  int imgID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_TITLE,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UTitle,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconr,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==1)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_ARTIST,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UAutor,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icont,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==2)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_ALBUM,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UAlbum,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icony,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==3)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_YEAR,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UYear,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconu,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==4)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_COMMENT,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UComment,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconi,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==5)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_GENRE,ENC_UCS2,TEXTID_ANY_LEN));
        TagBook->UGenre=new wchar_t[strlen(GetGenre(TagBook->genre))+1];
        win12512unicode(TagBook->UGenre,GetGenre(TagBook->genre),strlen(GetGenre(TagBook->genre))+1);
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->UGenre,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icono,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
  }
  return(1);        
}

void PlayF1( BOOK* bk, GUI* )
{
  if(TagBook->PlayMode==0)
  {
    PlayFile(TagBook->path,TagBook->name);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_STOP);
    GUIObject_SoftKeys_SetText(TagBook->MenuList1,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=1;
  }
  else
  {
    PlaySystemSound(7);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_PLAY);
    GUIObject_SoftKeys_SetText(TagBook->MenuList1,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=0;
  }
}

void DeleteTag1(int x)
{
  int i;
  if(x==0)
  {
    for(i=0; i<=29; i++)
    {
      TagBook->UTitle[i]=0;
    }
  }
  if(x==1)
  {
    for(i=0; i<=29; i++)
    {
      TagBook->UAutor[i]=0;
    }
  }
  if(x==2)
  {
    for(i=0; i<=29; i++)
    {
      TagBook->UAlbum[i]=0;
    }
  }
  if(x==3)
  {
    for(i=0; i<=3; i++)
    {
      TagBook->UYear[i]=0;
    }
  }
  if(x==4)
  {
    for(i=0; i<=29; i++)
    {
      TagBook->UComment[i]=0;
    }
  }
  TagBook->change++;
  if(TagBook->MenuList1) GUIObject_Destroy(TagBook->MenuList1);
  CreateML1(x);
}

void CopyFromName1(int x)
{
  int i;
  if(x==0)
  {
    int len=wstrlen(TagBook->name);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UTitle[i]=TagBook->name[i];
      }
      else
      {
        TagBook->UTitle[i]=0;
      }
    }
  }
  if(x==1)
  {
    int len=wstrlen(TagBook->name);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UAutor[i]=TagBook->name[i];
      }
      else
      {
        TagBook->UAutor[i]=0;
      }
    }
  }
  if(x==2)
  {
    int len=wstrlen(TagBook->name);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UAlbum[i]=TagBook->name[i];
      }
      else
      {
        TagBook->UAlbum[i]=0;
      }
    }
  }
  TagBook->change++;
  if(TagBook->MenuList1) GUIObject_Destroy(TagBook->MenuList1);
  CreateML1(x);
}

void CopyFromID3v2(int x)
{
  int i;
  if(x==0)
  {
    int len=wstrlen(TagBook->UTitle2);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UTitle[i]=TagBook->UTitle2[i];
      }
      else
      {
        TagBook->UTitle[i]=0;
      }
    }
  }
  if(x==1)
  {
    int len=wstrlen(TagBook->UAutor2);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UAutor[i]=TagBook->UAutor2[i];
      }
      else
      {
        TagBook->UAutor[i]=0;
      }
    }
  }
  if(x==2)
  {
    int len=wstrlen(TagBook->UAlbum2);
    for(i=0; i<=29; i++)
    {
      if(i<len)
      {
        TagBook->UAlbum[i]=TagBook->UAlbum2[i];
      }
      else
      {
        TagBook->UAlbum[i]=0;
      }
    }
  }
  TagBook->change++;
  if(TagBook->MenuList1) GUIObject_Destroy(TagBook->MenuList1);
  CreateML1(x);
}
      
  

void NewKey1(DISP_OBJ* p, int i1, int i2, int i3, int i4)
{
  TagBook->OldKey1(p, i1, i2, i3, i4);
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DEL)
  {
    int sel=ListMenu_GetSelectedItem(TagBook->MenuList1);
    if(sel<=4)
    {
      DeleteTag1(sel);
    }
  }
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DIGITAL_0)
  {
    PlayF1((BOOK*)TagBook,0);
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+1))
  {
    InputTitleF();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+2))
  {
    InputAutorF();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+3))
  {
    InputAlbumF();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+4))
  {
    InputYearF();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+5))
  {
    InputCommentF();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+6))
  {
    MGList();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+10))
  {
    ShowInfo();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+11))
  {
    CopyFromID3v2(ListMenu_GetSelectedItem(TagBook->MenuList1));
  }
  if(i4==KBD_LONG_PRESS && i1==(KEY_DIGITAL_0+11))
  {
    CopyFromName1(ListMenu_GetSelectedItem(TagBook->MenuList1));
  }
}

void CreateML1(int x)
{
  GUI_LIST*MList1=CreateListMenu((BOOK*)TagBook,0);
  TagBook->MenuList1=MList1;
  GUIObject_SetTitleText(MList1, TextID_Create(NAME_EDIT1,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(MList1, 6);
  OneOfMany_SetOnMessage(MList1,SetTitleML1);
  ListMenu_SetCursorToItem(MList1,x);
  ListMenu_SetItemStyle(MList1,3);
  GUIObject_SoftKeys_SetAction(MList1,ACTION_BACK,ExitML1);
  GUIObject_SoftKeys_SetAction(MList1,ACTION_LONG_BACK,ExitML1);
  GUIObject_SoftKeys_SetAction(MList1,ACTION_SELECT1,OnEntML1);	
  GUIObject_SoftKeys_SetAction(MList1,1,PlayF1);
  GUIObject_SoftKeys_SetText(MList1,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
  TagBook->OldKey1 = DispObject_GetOnKey( GUIObject_GetDispObject(MList1) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(MList1) ), NewKey1);
  GUIObject_Show(MList1);
}
  
  

void OnEntGM( BOOK* bk, GUI* )
{
  int item=ListMenu_GetSelectedItem(TagBook->GlavMenu);
  if(item==0)
  {
    CreateML1(0);
  }
  if(item==1)
  {
    CreateML2(0);
  }
  if(item==2)
  {
    ShowInfo();
  }
  if(item!=2)
  {
    GUIObject_Destroy(TagBook->GlavMenu);
    TagBook->GlavMenu=0;
  }
}

int onAccept_DB(void * data, BOOK * book)
{
  wchar_t *name=new wchar_t[wstrlen( FILEITEM_GetFname((FILEITEM*)data) )+1];
  wchar_t *path=new wchar_t[wstrlen( FILEITEM_GetPath((FILEITEM*)data) )+1];
  wstrcpy(name, FILEITEM_GetFname((FILEITEM*)data) );
  wstrcpy(path, FILEITEM_GetPath((FILEITEM*)data) );
  FileRead(path, name);
  return (0);
};

static int onCancelDB(void *data, BOOK * book)
{
  FreeBook(book);
  return(1);
};

int isDir(const wchar_t *path, const wchar_t *name)
{
  wchar_t fullname[256];
  W_FSTAT fs;
  wstrcpy(fullname,path);
  wstrcat(fullname,L"/");
  wstrcat(fullname,name);
  w_fstat(fullname,&fs);
  return(fs.attr&FA_DIRECTORY);
}

int Mp3Filter(const wchar_t *ExtList, const wchar_t *ItemPath, const wchar_t *ItemName)
{
  if(DataBrowser_isFileInListExt(ExtList,ItemPath,ItemName)) return 1;
  if(isDir(ItemPath, ItemName)) return 1;
  return 0;
}

static int CreateDB(void *, BOOK * book)
{
  if(TagBook->PlayMode==1){
    PlaySystemSound(7);
    TagBook->PlayMode=0;
  }
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  folder_list[0]=GetDir(DIR_AUDIO|MEM_INTERNAL);
  folder_list[1]=GetDir(DIR_AUDIO|MEM_EXTERNAL);
  folder_list[2]=0;
  DataBrowserDesc_SetHeaderText(DB_Desc,TextID_Create(NAME_OPEN_FILE, ENC_UCS2, TEXTID_ANY_LEN));
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetSelectActionOnFolders(DB_Desc,1);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetFileExtList(DB_Desc,L"*.mp3");
  DataBrowserDesc_SetItemFilter(DB_Desc,Mp3Filter);
  DataBrowser_Create(DB_Desc);
  return(0);
};

const PAGE_MSG bk_msglst_db[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,     CreateDB,
  ACCEPT_EVENT_TAG,         onAccept_DB,
  PREVIOUS_EVENT_TAG,       onCancelDB,
  CANCEL_EVENT_TAG,         onCancelDB,
  NIL_EVENT_TAG,            NULL
};

const PAGE_DESC base_db = {"TagEditor_openFile",0,bk_msglst_db};

void YesPress( BOOK* bk, GUI* )
{
  GUIObject_Destroy(TagBook->yn);
  TagBook->yn=0;
  GUIObject_Destroy(TagBook->GlavMenu);
  TagBook->GlavMenu=0;
  FileWrite();
  BookObj_GotoPage(&TagBook->book, &base_db);
}

void NoPress( BOOK* bk, GUI* )
{
  if(TagBook->Run!=0)
  {
    elfload(TagBook->TON,0,0,0);
  }
  GUIObject_Destroy(TagBook->yn);
  TagBook->yn=0;
  GUIObject_Destroy(TagBook->GlavMenu);
  TagBook->GlavMenu=0;
  BookObj_GotoPage(&TagBook->book, &base_db);
}

void ExitYN( BOOK* bk, GUI* )
{
  GUIObject_Destroy(TagBook->yn);
  TagBook->yn=0;
  GUIObject_Show(TagBook->GlavMenu);
}

void YesNo()
{
  if(TagBook->PlayMode==1){
    PlaySystemSound(7);
    TagBook->PlayMode=0;
  }
  TagBook->yn=CreateYesNoQuestionVA(0,
                                    VAR_BOOK(TagBook),
                                    VAR_YESNO_QUESTION(TextID_Create(QUESTION_SAVE_FILE, ENC_UCS2, TEXTID_ANY_LEN)),
                                    0);
  GUIObject_SoftKeys_SetAction(TagBook->yn, ACTION_YES, YesPress);
  GUIObject_SoftKeys_SetAction(TagBook->yn, ACTION_NO, NoPress);
  GUIObject_SoftKeys_SetAction(TagBook->yn, ACTION_BACK, ExitYN);
  GUIObject_SoftKeys_SetAction(TagBook->yn, ACTION_LONG_BACK, ExitYN);
}

void ExitGM( BOOK* bk, GUI* )
{
  if(TagBook->change)
  {
    YesNo();
  }
  else
  {
    GUIObject_Destroy(TagBook->GlavMenu);
    TagBook->GlavMenu=0;
    BookObj_GotoPage(&TagBook->book, &base_db);
  }
}

int SetTitlePunktsGM(GUI_MESSAGE * msg)
{
  int item;
  int imgID;
  switch( GUIonMessage_GetMsg(msg) )
  {
    case 1:
      item=GUIonMessage_GetCreatedItemIndex(msg);
      if(item==0)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"ID3v1",ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(NAME_EDIT1,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconq,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
      if(item==1)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(L"ID3v2",ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(NAME_EDIT2,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Iconw,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
        
      }
      if(item==2)
      {
        GUIonMessage_SetMenuItemText(msg,TextID_Create(MENU_ITEM_TEXT_INFO,ENC_UCS2,TEXTID_ANY_LEN));
        GUIonMessage_SetMenuItemSecondLineText(msg,TextID_Create(TagBook->name,ENC_UCS2,TEXTID_ANY_LEN));
        iconidname2id(Icone,-1,&imgID);
        GUIonMessage_SetMenuItemIcon(msg, 0, imgID);
      }
  }
  return(1);        
}

void PlayF( BOOK* bk, GUI* )
{
  if(TagBook->PlayMode==0)
  {
    PlayFile(TagBook->path,TagBook->name);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_STOP);
    GUIObject_SoftKeys_SetText(TagBook->GlavMenu,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=1;
  }else{
    PlaySystemSound(7);
    wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_PLAY);
    GUIObject_SoftKeys_SetText(TagBook->GlavMenu,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
    TagBook->PlayMode=0;
  }
}

void DeleteTag(int x)
{
  if(x==0)
  {
    for(int i=0; i<=29; i++)
    {
      TagBook->UTitle[i]=0;
      TagBook->UAutor[i]=0;
      TagBook->UAlbum[i]=0;
      TagBook->UComment[i]=0;
    }
    for(int i=0; i<=3; i++)
    {
      TagBook->UYear[i]=0;
    }
  }
  if(x==1)
  {
    for(int i=0; i<=(wstrlen(TagBook->UTitle2)-1); i++)
    {
      TagBook->UTitle2[i]=0;
    }
    for(int i=0; i<=(wstrlen(TagBook->UAutor2)-1); i++)
    {
      TagBook->UAutor2[i]=0;
    }
    for(int i=0; i<=(wstrlen(TagBook->UAlbum2)-1); i++)
    {
      TagBook->UAlbum2[i]=0;
    }
  }
  TagBook->change++;
}

void NewKey(DISP_OBJ* p, int i1, int i2, int i3, int i4)
{
  TagBook->OldKey(p, i1, i2, i3, i4);
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DEL)
  {
    int sel=ListMenu_GetSelectedItem(TagBook->GlavMenu);
    if(sel<=4)
    {
      DeleteTag(sel);
    }
  }
  if(i4==KBD_SHORT_RELEASE && i1==KEY_DIGITAL_0)
  {
    PlayF((BOOK*)TagBook,0);
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+1))
  {
    CreateML1(0);
    GUIObject_Destroy(TagBook->GlavMenu);
    TagBook->GlavMenu=0;
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+2))
  {
    CreateML2(0);
    GUIObject_Destroy(TagBook->GlavMenu);
    TagBook->GlavMenu=0;
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+3))
  {
    ShowInfo();
  }
  if(i4==KBD_SHORT_RELEASE && i1==(KEY_DIGITAL_0+11))
  {
    ShowInfo();
  }
}

void CreateGlavMenu(int x)
{
  GUI_LIST*GM=CreateListMenu((BOOK*)TagBook,0);
  TagBook->GlavMenu=GM;
  GUIObject_SetTitleText(GM, TextID_Create(NAME_TAG_EDITOR,ENC_UCS2,TEXTID_ANY_LEN));
  ListMenu_SetItemCount(GM, 3);
  OneOfMany_SetOnMessage(GM,SetTitlePunktsGM);
  ListMenu_SetCursorToItem(GM,x);
  ListMenu_SetItemStyle(GM,3);
  GUIObject_SoftKeys_SetAction(GM,ACTION_BACK,ExitGM);
  GUIObject_SoftKeys_SetAction(GM,ACTION_LONG_BACK,ExitGM);
  GUIObject_SoftKeys_SetAction(GM,ACTION_SELECT1,OnEntGM);	
  GUIObject_SoftKeys_SetAction(GM,1,PlayF);
  GUIObject_SoftKeys_SetText(GM,1,TextID_Create(TagBook->PlayText,ENC_UCS2,TEXTID_ANY_LEN));
  TagBook->OldKey = DispObject_GetOnKey( GUIObject_GetDispObject(GM) );
  DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject(GM) ), NewKey);
  GUIObject_Show(GM);
}
  

void ID1()
{
  TagBook->Title=new char[31];
  TagBook->Autor=new char[31];
  TagBook->Album=new char[31];
  TagBook->Year=new char[5];
  TagBook->Comment=new char[31];
  TagBook->UTitle=new wchar_t[31];
  TagBook->UAutor=new wchar_t[31];
  TagBook->UAlbum=new wchar_t[31];
  TagBook->UYear=new wchar_t[5];
  TagBook->UComment=new wchar_t[31];
  TagBook->PosTitle=128-125;
  TagBook->PosAutor=128-125+30;
  TagBook->PosAlbum=128-125+60;
  TagBook->PosYear=128-125+90;
  TagBook->PosComment=128-125+94;
  if(TagBook->buffer[0]!='T' && TagBook->buffer[1]!='A' && TagBook->buffer[2]!='G')
  {
    TagBook->ID3v1=0;
    TagBook->buffer[0]='T';
    TagBook->buffer[1]='A';
    TagBook->buffer[2]='G';
    TagBook->genre=0;
    for(int i=0;i<=29;i++)
    {
      TagBook->Title[i]=0;
      TagBook->Autor[i]=0;
      TagBook->Album[i]=0;
      TagBook->Comment[i]=0;
    }
    for(int i=0; i<=3; i++)
    {
      TagBook->Year[i]=0;
    }
  }
  else
  {
    TagBook->ID3v1=1;
    TagBook->genre=TagBook->buffer[127];
    for(int i=0;i<=29;i++)
    {
      TagBook->Title[i]=TagBook->buffer[TagBook->PosTitle+i];
      TagBook->Autor[i]=TagBook->buffer[TagBook->PosAutor+i];
      TagBook->Album[i]=TagBook->buffer[TagBook->PosAlbum+i];
      TagBook->Comment[i]=TagBook->buffer[TagBook->PosComment+i];
    }
    for(int i=0; i<=3; i++)
    {
      TagBook->Year[i]=TagBook->buffer[TagBook->PosYear+i];
    }
  }
  if(TagBook->Title[0]==0)
  {
    win12512unicode(TagBook->UTitle, TagBook->newname, 30);
  }
  else
  {
    win12512unicode(TagBook->UTitle, TagBook->Title, 30);
  }
  win12512unicode(TagBook->UAutor, TagBook->Autor, 30);
  win12512unicode(TagBook->UAlbum, TagBook->Album, 30);
  win12512unicode(TagBook->UYear, TagBook->Year, 4);
  win12512unicode(TagBook->UComment, TagBook->Comment, 30);
} 

static int CreateGui(void *data, BOOK * book)
{
  CreateGlavMenu(0);
  return(0);
}

const PAGE_MSG bk_msglst_work[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    CreateGui,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC base_page_work = {"TagEditor_base_work",0,bk_msglst_work};

void FileRead(wchar_t*path,wchar_t*name)
{
  FSTAT _fstat;
  fstat(path,name,&_fstat);
  TagBook->lenn=wstrlen(name);
  TagBook->name=new wchar_t[TagBook->lenn+1];    
  wstrcpy(TagBook->name, name);
  TagBook->newname=new char[TagBook->lenn+1];
  unicode2win1251(TagBook->newname,TagBook->name,TagBook->lenn);
  TagBook->newname[TagBook->lenn-1]='3';
  TagBook->newname[TagBook->lenn-2]='p';
  TagBook->newname[TagBook->lenn-3]='m';   
  TagBook->newname[TagBook->lenn-4]='.';
  TagBook->Unewname=new wchar_t[TagBook->lenn+1];
  win12512unicode(TagBook->Unewname,TagBook->newname,TagBook->lenn);
  TagBook->lenp=wstrlen(path);
  TagBook->path=new wchar_t[TagBook->lenp+1];
  wstrcpy(TagBook->path, path);        
  TagBook->size=_fstat.fsize;
  TagBook->change=0;
  TagBook->PlayMode=0;
  wstrcpy(TagBook->PlayText,SOFTKEY_STATUS_PLAY);
  int file;
  file=_fopen(path,name, FSX_O_RDONLY, FSX_S_IREAD|FSX_S_IWRITE, 0);
  TagBook->buffer=(char*)malloc(128+1);
  lseek(file, TagBook->size-128, 0);
  fread(file, TagBook->buffer, 128);
  ID1();
  char BUF[10+1];
  lseek(file,0,0);
  fread(file, BUF, 10);
  TagBook->ID3=0;
  TagBook->pos2_Title=0;
  TagBook->pos2_Autor=0;
  TagBook->pos2_Album=0;
  TagBook->sizeTitle2=0;
  TagBook->sizeAutor2=0;
  TagBook->sizeAlbum2=0;
  TagBook->Title2=0;
  TagBook->Autor2=0;
  TagBook->Album2=0;
  TagBook->UTitle2=0;
  TagBook->UAutor2=0;
  TagBook->UAlbum2=0;
  if(BUF[0]=='I'&&BUF[1]=='D'&&BUF[2]=='3')
  {
    TagBook->ID3=1;
    char tagSize[4];
    tagSize[0]=BUF[6];
    tagSize[1]=BUF[7];
    tagSize[2]=BUF[8];
    tagSize[3]=BUF[9];
    TagBook->size_id3=synchsafeToNormal(tagSize);
    lseek(file,0,0);
    TagBook->buffer2=new char[TagBook->size_id3+1];
    fread(file,TagBook->buffer2,TagBook->size_id3);
    TagBook->pos2_Title=-1;
    TagBook->pos2_Autor=-1;
    TagBook->pos2_Album=-1;
    for(int i=0; i<=TagBook->size_id3-4; i++)
    {
      if(TagBook->buffer2[i]=='T' && TagBook->buffer2[i+1]=='I' && TagBook->buffer2[i+2]=='T' && TagBook->buffer2[i+3]=='2')
      {
        TagBook->pos2_Title=i;
      }
    }
    if(TagBook->pos2_Title)
    {
      if(TagBook->buffer2[TagBook->pos2_Title+7])
      {
        TagBook->sizeTitle2=TagBook->buffer2[TagBook->pos2_Title+7]-1;
      }
      else
      {
        TagBook->sizeTitle2=TagBook->buffer2[TagBook->pos2_Title+4]-1;
      }
      if(TagBook->sizeTitle2>1)
      {
        if(TagBook->buffer2[TagBook->pos2_Title+10]==0x01)
        {
          TagBook->UTitle2=new wchar_t[(TagBook->sizeTitle2-3)/2+1];
          lseek(file,TagBook->pos2_Title+13,0);
          for(int i=0; i<=(TagBook->sizeTitle2-3)/2; i++)
          {
            fread(file,&TagBook->UTitle2[i],2);
          }
          TagBook->Title2=new char[(TagBook->sizeTitle2-3)/2+1];
          unicode2win1251(TagBook->Title2,TagBook->UTitle2, (TagBook->sizeTitle2-3)/2);
        }
        else
        {
          TagBook->Title2=new char[TagBook->sizeTitle2+1];
          lseek(file,TagBook->pos2_Title+11,0);
          fread(file, TagBook->Title2, TagBook->sizeTitle2);
          TagBook->UTitle2=new wchar_t[TagBook->sizeTitle2+1];
          win12512unicode(TagBook->UTitle2, TagBook->Title2, TagBook->sizeTitle2);
        }
      }
      else
      {
        TagBook->Title2=new char[strlen(TagBook->Title)+1];
        strcpy(TagBook->Title2, TagBook->Title);
        TagBook->UTitle2=new wchar_t[wstrlen(TagBook->UTitle)+1];
        wstrcpy(TagBook->UTitle2, TagBook->UTitle);
      }
    }
    for(int i=0; i<=TagBook->size_id3-4; i++)
    {
      if(TagBook->buffer2[i]=='T' && TagBook->buffer2[i+1]=='P' && TagBook->buffer2[i+2]=='E' && TagBook->buffer2[i+3]=='1')
      {
        TagBook->pos2_Autor=i;
      }
    }
    if(TagBook->pos2_Autor)
    {
      if(TagBook->buffer2[TagBook->pos2_Autor+7])
      {
        TagBook->sizeAutor2=TagBook->buffer2[TagBook->pos2_Autor+7]-1;
      }
      else
      {
        TagBook->sizeAutor2=TagBook->buffer2[TagBook->pos2_Autor+4]-1;
      }
      if(TagBook->sizeAutor2>1)
      {
        if(TagBook->buffer2[TagBook->pos2_Autor+10]==0x01)
        {
          TagBook->UAutor2=new wchar_t[(TagBook->sizeAutor2-3)/2+1];
          lseek(file,TagBook->pos2_Autor+13,0);
          for(int i=0; i<=(TagBook->sizeAutor2-3)/2; i++)
          {
            fread(file,&TagBook->UAutor2[i],2);
          }
          TagBook->Autor2=new char[(TagBook->sizeAutor2-3)/2+1];
          unicode2win1251(TagBook->Autor2,TagBook->UAutor2, (TagBook->sizeAutor2-3)/2);
        }
        else
        {
          TagBook->Autor2=new char[TagBook->sizeAutor2+1];
          lseek(file,TagBook->pos2_Autor+11,0);
          fread(file, TagBook->Autor2, TagBook->sizeAutor2);
          TagBook->UAutor2=new wchar_t[TagBook->sizeAutor2+1];
          win12512unicode(TagBook->UAutor2, TagBook->Autor2, TagBook->sizeAutor2);
        }
      }
      else
      {
        TagBook->Autor2=new char[strlen(TagBook->Autor)+1];
        strcpy(TagBook->Autor2, TagBook->Autor);
        TagBook->UAutor2=new wchar_t[wstrlen(TagBook->UAutor)+1];
        wstrcpy(TagBook->UAutor2, TagBook->UAutor);
      }
    }
    for(int i=0; i<=TagBook->size_id3-4; i++)
    {
      if(TagBook->buffer2[i]=='T' && TagBook->buffer2[i+1]=='A' && TagBook->buffer2[i+2]=='L' && TagBook->buffer2[i+3]=='B')
      {
        TagBook->pos2_Album=i;
      }
    }
    if(TagBook->pos2_Album)
    {
      if(TagBook->buffer2[TagBook->pos2_Album+7])
      {
        TagBook->sizeAlbum2=TagBook->buffer2[TagBook->pos2_Album+7]-1;
      }
      else
      {
        TagBook->sizeAlbum2=TagBook->buffer2[TagBook->pos2_Album+4]-1;
      }
      if(TagBook->sizeAlbum2>1)
      {
        if(TagBook->buffer2[TagBook->pos2_Album+10]==0x01)
        {
          TagBook->UAlbum2=new wchar_t[(TagBook->sizeAlbum2-3)/2+1];
          lseek(file,TagBook->pos2_Album+13,0);
          for(int i=0; i<=(TagBook->sizeAlbum2-3)/2; i++)
          {
            fread(file,&TagBook->UAlbum2[i],2);
          }
          TagBook->Album2=new char[(TagBook->sizeAlbum2-3)/2+1];
          unicode2win1251(TagBook->Album2,TagBook->UAlbum2, (TagBook->sizeAlbum2-3)/2);
        }
        else
        {
          TagBook->Album2=new char[TagBook->sizeAlbum2+1];
          lseek(file,TagBook->pos2_Album+11,0);
          fread(file, TagBook->Album2, TagBook->sizeAlbum2);
          TagBook->UAlbum2=new wchar_t[TagBook->sizeAlbum2+1];
          win12512unicode(TagBook->UAlbum2, TagBook->Album2, TagBook->sizeAlbum2);
        }
      }
      else
      {
        TagBook->Album2=new char[strlen(TagBook->Album)+1];
        strcpy(TagBook->Album2, TagBook->Album);
        TagBook->UAlbum2=new wchar_t[wstrlen(TagBook->UAlbum)+1];
        wstrcpy(TagBook->UAlbum2, TagBook->UAlbum);
      }
    }
  }
  else
  {
    TagBook->Album2=new char[strlen(TagBook->Album)+1];
    strcpy(TagBook->Album2, TagBook->Album);
    TagBook->UAlbum2=new wchar_t[wstrlen(TagBook->UAlbum)+1];
    wstrcpy(TagBook->UAlbum2, TagBook->UAlbum);
    TagBook->Autor2=new char[strlen(TagBook->Autor)+1];
    strcpy(TagBook->Autor2, TagBook->Autor);
    TagBook->UAutor2=new wchar_t[wstrlen(TagBook->UAutor)+1];
    wstrcpy(TagBook->UAutor2, TagBook->UAutor);
    TagBook->Title2=new char[strlen(TagBook->Title)+1];
    strcpy(TagBook->Title2, TagBook->Title);
    TagBook->UTitle2=new wchar_t[wstrlen(TagBook->UTitle)+1];
    wstrcpy(TagBook->UTitle2, TagBook->UTitle);
    char tagSize[4];
    tagSize[0]=0x00;
    tagSize[1]=0x00;
    tagSize[2]=0x1e;
    tagSize[3]=0x00;
    TagBook->size_id3=synchsafeToNormal(tagSize);
    TagBook->buffer2=new char[TagBook->size_id3+1];
    for(int i=0; i<=TagBook->size_id3; i++)
    {
      TagBook->buffer2[i]=0;
    }
    TagBook->buffer2[0]='I';
    TagBook->buffer2[1]='D';
    TagBook->buffer2[2]='3';
    TagBook->buffer2[3]=0x04;
    TagBook->buffer2[8]=0x1e;
  }
  fclose(file);
  TagBook->InputTitle=0;
  TagBook->InputAutor=0;
  TagBook->InputAlbum=0;
  TagBook->InputTitle2=0;
  TagBook->InputAutor2=0;
  TagBook->InputAlbum2=0;
  TagBook->InputYear=0;
  TagBook->InputComment=0;
  TagBook->yn=0;
  TagBook->GlavMenu=0;
  TagBook->MenuList1=0;
  TagBook->MenuList2=0;
  TagBook->change=0;
  BookObj_GotoPage(&TagBook->book, &base_page_work);
}

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,     TerminateElf,
  ELF_SHOW_INFO_EVENT,     ShowAuthorInfo,
  ELF_RECONFIG_EVENT,      onReconfigElf,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC base_page_main = {"TagEditor_base_main",0,bk_msglst_base};

int main(wchar_t * elfname, wchar_t * path, wchar_t *name, wchar_t*TagOpen)
{
  TagBook=new MyBook;
  CreateBook((BOOK*)TagBook,onCloseCreateFileBook,&base_page_main,"TagEditor",-1,0);
  InitConfig();
  TagBook->PlayMode=0;
  if(path && name)
  {
    char * ext=new char[wstrlen(name)+1];
    unicode2win1251(ext,name,wstrlen(name));
    if(ext[strlen(ext)-1]=='3'&&ext[strlen(ext)-2]=='p'&&ext[strlen(ext)-3]=='m'||ext[strlen(ext)-1]=='t'&&ext[strlen(ext)-2]=='p'&&ext[strlen(ext)-3]=='m')
    {
      delete ext;
      FileRead(path,name);
    }
    else
    {
      BookObj_GotoPage(&TagBook->book, &base_db);
    }
  }
  else
  {
    BookObj_GotoPage(&TagBook->book, &base_db);
  }
  return(0);
}
