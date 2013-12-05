#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "structs.h"
#include "Externals.h"

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

void metadata_Read(wchar_t *path, wchar_t *name, METADATA *meta, TAG_REQUEST tr)
{
  FSTAT fst;
  if (wstrcmpni(L"mp3",&name[wstrlen(name)-3],3)!=0)
  {
    return;
  }
  if (fstat(path,name,&fst)==0)
  {
    int file;
    if ((file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0))>=0)
    {
      if (fst.fsize>128)
      {
        char wtag[4];
        char title[31];
        char artist[31];
        char album[31];
        char year[5];
        char comment[31];
        char genre;
        lseek(file,fst.fsize-128,0);
        fread(file,wtag,3);
        wtag[3]='\0';
        if (strcmp(wtag,"TAG")==0)
        {
          fread(file,title,30);
          fread(file,artist,30);
          fread(file,album,30);
          fread(file,year,4);
          fread(file,comment,30);
          fread(file,&genre,1);
          title[30]='\0';
          artist[30]='\0';
          album[30]='\0';
          year[4]='\0';
          comment[30]='\0';
          if (tr.title && title[0]!=0)
          {
            char2unicode(meta->Title,title,TEXTID_ANY_LEN);
          }
          if (tr.artist && artist[0]!=0)
          {
            char2unicode(meta->Artist,artist,TEXTID_ANY_LEN);
          }
          if (tr.album && album[0]!=0)
          {
            char2unicode(meta->Album,album,TEXTID_ANY_LEN);
          }
          if (tr.year && year[0]!=0)
          {
            char2unicode(meta->Year,year,TEXTID_ANY_LEN);
          }
          if (tr.genre && year[0]!=0)
          {
            char2unicode(meta->Genre,GetGenre(genre),TEXTID_ANY_LEN);
          }
        }
      }
    }
    fclose(file);
  }
}
