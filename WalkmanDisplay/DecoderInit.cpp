#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "header\wdisplay.h"
#include "header\decoder.h"
#include "header\config_data.h"

#define TAG_TITLE 1
#define TAG_ALBUM 2
#define TAG_ARTIST 0
#define TAG_YEAR 3
#define TAG_GENRE 4

wchar_t GetBitmap(wchar_t *path, wchar_t *name);
MUSIC nowmusic;

wchar_t *types[4]={
  L"jpg",
  L"gif",
  L"png",
  L"bmp"
};

#pragma optimize = z 9
wchar_t * GetType_str(int cover_type)
{
  if (cover_type<=3)
  {
    return types[cover_type];
  }
  return 0;
};

#pragma optimize = z 9
wchar_t GetCover(void *TagStructure, wchar_t *path, wchar_t *name, METADATA *meta)
{
  char cover_type;
  int size;
  int cover_offset;
  meta->ImageID=NOIMAGE;
  if (MetaData_Desc_GetCoverInfo(TagStructure,&cover_type,&size,&cover_offset)!=0)
  {
    if (cover_offset>0 && size>0)
    {
      int f;
      if (f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0x0)>=0)
      {
        if (lseek(f,cover_offset,0)>=0)
        {
          char * my_buf=new char[size+1];
          fread(f,my_buf,size);
          
          ImageID_GetIndirect(my_buf,size,0,GetType_str(cover_type),&meta->ImageID);
        }
      }
      fclose(f);
    }
  }

  if (meta->ImageID==NOIMAGE && use_albumpng==true)
  {
    FSTAT fst;
    if (fstat(path,L"album.png",&fst)==0)
    {
      ImageID_Get(path,L"album.png",&meta->ImageID);
    }
    else if (fstat(path,L"cover.png",&fst)==0)
    {
      ImageID_Get(path,L"cover.png",&meta->ImageID);
    }
  }
  return meta->ImageID;
}

void metadata_Read(wchar_t *path, wchar_t *name, METADATA *mt, TAG_REQUEST tr);

#pragma optimize = z 9
void metadata_Get(wchar_t *path, wchar_t *name, METADATA *meta, int format)
{
  metadata_reset(meta);
  if (wstrlen(name)>5)
  {
    wstrncpy(meta->Title,name, wstrlen(name)-4);
  }
  else
  {
    wstrcpy(meta->Title,name);
  }
  TAG_REQUEST tr;
  tr.album=1;
  tr.title=1;
  tr.artist=1;
  tr.genre=1;
  tr.year=1;
  void *TagStructure=0;
  if (TagStructure=MetaData_Desc_Create(path,name))
  {
    meta->ImageID=GetCover(TagStructure, path, name, meta);
    
    wchar_t *tag=0;

    if (tag=MetaData_Desc_GetTags(TagStructure, TAG_TITLE))
    {
      if (tag[0]!=0)
      {
        wstrcpy(meta->Title,tag);
        tr.title=0;
      }
    }
    
    tag=0;
    if (tag=MetaData_Desc_GetTags(TagStructure, TAG_ALBUM))
    {
      if (tag[0]!=0)
      {
        wstrcpy(meta->Album,tag);
        tr.album=0;
      }
    }
    
    tag=0;
    if (tag=MetaData_Desc_GetTags(TagStructure, TAG_ARTIST))
    {
      if (tag[0]!=0)
      {
        wstrcpy(meta->Artist,tag);
        tr.artist=0;
      }
    }
    
    tag=0;
    if (tag=MetaData_Desc_GetTags(TagStructure, TAG_YEAR))
    {
      if (tag[0]!=0)
      {
        wstrcpy(meta->Year,tag);
        if (wstrlen(tag)>1)
        {
          tr.year=0;
        }
      }
    }
    
    tag=0;
    if (tag=MetaData_Desc_GetTags(TagStructure, TAG_GENRE))
    {
      if (tag[0]!=0)
      {
        wstrcpy(meta->Genre,tag);
        tr.genre=0;
      }
    }
    
    if (MetaData_Desc_GetTrackNum(TagStructure, 0)!=-1)
    {
      meta->TrackNum=MetaData_Desc_GetTrackNum(TagStructure, 0);
    }
    else
    {
      meta->TrackNum=0;
    }
    MetaData_Desc_Destroy(TagStructure);
  }
  if ((tr.title || tr.album || tr.artist || tr.year || tr.genre) && format==FORMAT_MP3)
  {
    metadata_Read(path, name, meta, tr);
  }
};

void Cover_Free()
{
  if(nowmusic.meta.ImageID!=NOIMAGE)
  {
    ImageID_Free(nowmusic.meta.ImageID);
  }
  nowmusic.meta.ImageID=NOIMAGE;
};

int getFormat(wchar_t *name)
{
  if (name)
  {
    wchar_t *ext=wstrrchr(name,L'.');
    if (ext)
    {
      ext++;
      if (wstrlen(ext)>1)
      {
        debug_printf("\nWalkmanDisplay: getFormat: %ls extension\n", ext);
        if (wstrcmpi(ext,L"mp3")==0)
        {
          return FORMAT_MP3;
        }
        else if (wstrcmpi(ext,L"m4a")==0)
        {
          return FORMAT_M4A;
        }
        else if (wstrcmpi(ext,L"wma")==0)
        {
          return FORMAT_WMA;
        }
      }
    }
  }
  debug_printf("\nWalkmanDisplay: getFormat: unknown extension or name is empty\n");
  return FORMAT_OTHER;
};

void freemeta(MUSIC *music)
{
  memset(&music->meta,0,sizeof(METADATA));
  music->meta.ImageID=NOIMAGE;
  memset(&music->hdr,0,sizeof(HEADER_INFO));
};

#pragma optimize = s 9
void decoder_Init(wchar_t *path, wchar_t *name)
{
  Cover_Free();
  freemeta(&nowmusic);
  int format=getFormat(name);
  nowmusic.format=format;
  metadata_Get(path, name, &nowmusic.meta, format);
  if (format==FORMAT_MP3 || format==FORMAT_OTHER)
  {
    nowmusic.hdr=ReadMP3Information(path, name);
  }
};

