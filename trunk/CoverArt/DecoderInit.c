#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "structs.h"
#include "decoder.h"
#include "config_data.h"
#include "Externals.h"
#include "images.h"

IMG_DESC GetBitmap(wchar_t *path, wchar_t *name);
MUSIC nowmusic;

wchar_t * GetType_str(int cover_type)
{
  if (cover_type<=3)
  {
    switch(cover_type)
    {
      case 0:
        return L"jpg";
      case 1:
        return L"gif";
      case 2:
        return L"png";
      case 3:
        return L"bmp";
    }
    return 0;
  }
  return 0;
}

wchar_t GetCover(void *TagStructure, wchar_t *path, wchar_t *name, METADATA *meta)
{
  IMAGEID imageID = NOIMAGE;
  if (GetCover_SWI==true)
  {
    char cover_type;
    int size;
    int cover_offset;
    if (MetaData_Desc_GetCoverInfo(TagStructure,&cover_type,&size,&cover_offset)!=0)
    {
      int f;
      if (f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0x0)>=0)
      {
        if (lseek(f,cover_offset,0)>=0)
        {
          char * my_buf=new char[size+1];
          fread(f,my_buf,size);
          meta->Handle=false;
          meta->ImageHandle=NOIMAGE;
          if (ImageID_GetIndirect(my_buf,size,0,GetType_str(cover_type),&imageID)==0)
          {
            meta->ImageID=imageID;
            meta->HasImage=true;
          }
          else
          {
            meta->ImageID=NOIMAGE;
            meta->HasImage=false;
          }
        }
      }
      fclose(f);
    }
  }
  else
  {
    IMG_DESC id=GetBitmap(path,name);
    meta->ImageID=id.ImageID;
    meta->HasImage=id.HasImage;
    meta->ImageHandle=id.ImageHandle;
    meta->Handle=id.Handle;
  }
  if (meta->HasImage==false && COVER_GETPNG==true)
  {
    ImageHandler_Get(meta,path,L"album.png");
    if (meta->ImageID!=NOIMAGE)
    {
      meta->HasImage=true;
    }
    else
    {
      meta->HasImage=false;
    }
  }
  return imageID;
}
void metadata_Read(wchar_t *path, wchar_t *name, METADATA *mt, TAG_REQUEST tr);
void metadata_Get(wchar_t *path, wchar_t *name, METADATA *meta)
{
    metadata_reset(meta);
    if (GetCover_SWI!=true)
    {
      GetCover(0, path, name, meta);
    }
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
      if (GetCover_SWI==true)
      {
        GetCover(TagStructure, path, name, meta);
      }
      wchar_t *tag=0;
      if (tag=MetaData_Desc_GetTags(TagStructure,1))
      {
        if (tag[0]!=0)
        {
          wstrcpy(meta->Title,tag);
          tr.title=0;
        }
      }

      tag=0;
      if (tag=MetaData_Desc_GetTags(TagStructure,2))
      {
        if (tag[0]!=0)
        {
          wstrcpy(meta->Album,tag);
          tr.album=0;
        }
      }
      
      tag=0;
      if (tag=MetaData_Desc_GetTags(TagStructure,0))
      {
        if (tag[0]!=0)
        {
          wstrcpy(meta->Artist,tag);
          tr.artist=0;
        }
      }
      
      tag=0;
      if (tag=MetaData_Desc_GetTags(TagStructure,3))
      {
        if (tag[0]!=0)
        {
          wstrcpy(meta->Year,tag);
          tr.year=0;
        }
      }
      
      tag=0;
      if (tag=MetaData_Desc_GetTags(TagStructure,4))
      {
        if (tag[0]!=0)
        {
          wstrcpy(meta->Genre,tag);
          tr.genre=0;
        }
      }
      
      if (MetaData_Desc_GetTrackNum(TagStructure,0)!=-1)
      {
        meta->TrackNum=MetaData_Desc_GetTrackNum(TagStructure,0);
      }
      else
      {
        meta->TrackNum=0;
      }
      MetaData_Desc_Destroy(TagStructure);
    }
    if (tr.title || tr.album || tr.artist || tr.year || tr.genre)
    {
      metadata_Read(path, name, meta, tr);
    }
};

IMG_DESC GetBitmap(wchar_t *path, wchar_t *name)
{
  IMG_DESC id;
  id.ImageID=NOIMAGE;
  id.HasImage=false;
  id.ImageHandle=NOIMAGE;
  id.Handle=false;

  int error;
  FileDelete(GetDir(DIR_INI),L"album.png",&error);
  int inf=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  char CHUCK[100+1];
  fread(inf,CHUCK,100);
  if (CHUCK[0]=='I' && CHUCK[1]=='D' && CHUCK[2]=='3') //Присутствуют ли тэги версии 2.x?
  {
	char tagSize[4];
	tagSize[0]=CHUCK[6];
	tagSize[1]=CHUCK[7];
	tagSize[2]=CHUCK[8];
	tagSize[3]=CHUCK[9];
	int size=synchsafeToNormal(tagSize);
        int file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
	char cm[4];
	bool HasPic=false;
	bool isMid=false;
	int NEEDPOS=0;
	char *buff=new char[size+1];
	fread(file,buff,size);
	while(NEEDPOS<size)
	{
          if (buff[NEEDPOS]=='P' && buff[NEEDPOS+1]=='I' && buff[NEEDPOS+2]=='C')
          {
            HasPic=true; //Есть картинка
            break;
          }
	  NEEDPOS++;
	}	
	NEEDPOS++;
	mfree(buff);
	lseek(file,NEEDPOS-2,SEEK_SET);
	if (HasPic==true)
        { 
	  int TotalSize;
	  fread(file,cm,1);
	  if(cm[0]==')')
	  {
            lseek(file,3,SEEK_CUR);
            fread(file,cm,3);
            TotalSize = (cm[0]<<16)|(cm[1]<<8)|cm[2];
	  }
	  if(cm[0]=='A')
	  {
            lseek(file,3,SEEK_CUR);
            fread(file,cm,4);
            TotalSize = (cm[0]<<24)|(cm[1]<<16)|(cm[2]<<8)|cm[3];
            lseek(file,3+12,SEEK_CUR);
            char checker;
            fread(file,&checker,1);
            if (checker!=0)
            {
              isMid=true;
              lseek(file,-1,SEEK_CUR);
            }
	  }
          if (GetCover_SWI==0)
          {
            //Сохраняем в файл
            int psize=0;
            if (isMid==false){
              psize=TotalSize-1;
            }
            else
            {
              psize=TotalSize-12-1;
            }
            char *picture=new char[psize+1];
            fread(file,picture,psize);
            int f2;
            f2 = _fopen(GetDir(DIR_INI),L"album.png",FSX_O_RDWR,FSX_S_IREAD|FSX_S_IWRITE,0);
            fwrite(f2,picture,psize);
            fclose(f2);
            delete(picture);
            ImageHandler_Get(&id,GetDir(DIR_INI),L"album.png");
          }
          else if (GetCover_SWI==2)
          {
            //Берём напрямую из буфера
            IMAGEID imageID;
            int psize;
            if (isMid==false){
              psize=TotalSize-1;
            }
            else
            {
              psize=TotalSize-12-1;
            }
            char *picture=new char[psize+1];
            fread(file,picture,psize);
            nowmusic.meta.Handle=false;
            if (ImageID_GetIndirect(picture,psize,0,L"png",&imageID)==0)
            {
              id.ImageID=imageID;
              id.HasImage=true;
              id.Handle=false;
              id.ImageHandle=NOIMAGE;
            }
            else
            {
              id.ImageID=NOIMAGE;
              id.HasImage=false;
              id.Handle=false;
              id.ImageHandle=NOIMAGE;
              nowmusic.meta.HasImage=false;
            }
          }
	}
	fclose(file);
  }
  fclose(inf);
  return id;
};



void Cover_Free()
{
  if(nowmusic.meta.HasImage==true)
  {
    if (!nowmusic.meta.Handle)
    {
      ImageID_Free(nowmusic.meta.ImageID);
    }
    else
    {
      ImageHandler_Free(&nowmusic.meta);
    }
  }
  nowmusic.meta.ImageID=NOIMAGE;
  nowmusic.meta.HasImage=false;
  nowmusic.meta.Handle=false;
  nowmusic.meta.ImageHandle=NOIMAGE;
};

void decoder_Init(wchar_t *path, wchar_t *name)
{
  Cover_Free();
  metadata_Get(path, name, &nowmusic.meta);
  nowmusic.hdr=ReadMP3Information(path, name);
};

