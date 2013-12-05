#define USEPNG
#include "..\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "map_object.h"
#include "pngread.h"



void* xmalloc(int x,int n)
{
  return malloc(n);
}

void xmfree(int x,void* ptr)
{
  mfree(ptr);
}

void info_callback(png_structp png_ptr, png_infop info)
{
  int bit_depth, color_type, row_bytes;
  PNG_CONVERT *usr_png=(PNG_CONVERT *)png_get_progressive_ptr(png_ptr);
  
  
  png_get_IHDR(png_ptr, info, &(usr_png->width), &(usr_png->height), &bit_depth, &color_type, 0, 0, 0);
  if (bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
  if (png_get_valid(png_ptr, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);
  if (bit_depth == 16) png_set_strip_16(png_ptr);
  //if (bit_depth < 8) png_set_packing(png_ptr);
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_GRAY)
    png_set_gray_to_rgb(png_ptr);
  png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);
  png_read_update_info(png_ptr, info);
  row_bytes=png_get_rowbytes(png_ptr, info);
  usr_png->bytes_in_row=(row_bytes+3)&~3;
  usr_png->row_pointer=new png_byte[usr_png->bytes_in_row];
  usr_png->image=new char[usr_png->width*usr_png->height];
  memset((void *)usr_png->image,0,usr_png->width*usr_png->height*sizeof(char));
}

void row_callback(png_structp png_ptr, png_bytep new_row,
                  png_uint_32 row_num, int pass)
{
  PNG_CONVERT *usr_png=(PNG_CONVERT *)png_get_progressive_ptr(png_ptr);
  if (row_num>usr_png->height) return;
  png_progressive_combine_row(png_ptr,usr_png->row_pointer,new_row);
  char *image=usr_png->image+usr_png->width*row_num;
  for (int i=0; i<usr_png->width; i++)
  {
    if (usr_png->row_pointer[i*4+3])
    {
      unsigned int color1=usr_png->row_pointer[i*4+0] &0xE0;
      color1|=((usr_png->row_pointer[i*4+1]>>3)&0x1C);
      color1|=((usr_png->row_pointer[i*4+2]>>6)&3);
      image[i]=color1;
    }
    else
    {
      image[i]=0xFF;
    }
  }
}


void end_callback(png_structp png_ptr, png_infop info)
{
  PNG_LIST *png_list;
  PNG_CONVERT *usr_png=(PNG_CONVERT *)png_get_progressive_ptr(png_ptr);
  delete usr_png->row_pointer;
  usr_png->row_pointer=NULL;
  png_list=new PNG_LIST;
  png_list->index_x=usr_png->index_x;
  png_list->index_y=usr_png->index_y;
  png_list->width=usr_png->width;
  png_list->height=usr_png->height;
  png_list->image=usr_png->image;
  usr_png->image=NULL;
  List_InsertFirst(usr_png->png_files,png_list);
}

PNG_CONVERT *InitPng(LIST *pnglist, int index_x, int index_y)
{ 
  PNG_CONVERT *usr_png=new PNG_CONVERT;
  usr_png->png_ptr = png_create_read_struct_2("1.2.5",0,0,0,0,(png_malloc_ptr)xmalloc,(png_free_ptr)xmfree);
  usr_png->info_ptr = png_create_info_struct(usr_png->png_ptr);
  if (!usr_png->info_ptr)
  {
    png_destroy_read_struct(&(usr_png->png_ptr), (png_infopp)NULL,(png_infopp)NULL);
    delete usr_png;
    return 0;
  }
  usr_png->width=0;
  usr_png->height=0;
  usr_png->bytes_in_row=0;
  usr_png->row_pointer=NULL;
  usr_png->image=NULL;
  usr_png->index_x=index_x;
  usr_png->index_y=index_y;
  usr_png->png_files=pnglist;
  
  png_set_progressive_read_fn(usr_png->png_ptr, (void *)usr_png,
                              info_callback, row_callback, end_callback);
  return usr_png;
}

void RemovePngListElement(PNG_LIST *png_list)
{
  delete png_list->image;
  delete png_list;
}

void Destr_Png(PNG_CONVERT *usr_png)
{
  png_destroy_read_struct(&(usr_png->png_ptr), &(usr_png->info_ptr),(png_infopp)NULL);
  delete usr_png->row_pointer;
  delete usr_png->image;
  delete usr_png;
}

void process_data(PNG_CONVERT *usr_png, char *buffer, unsigned int length)
{
  png_process_data(usr_png->png_ptr, usr_png->info_ptr, (png_bytep)buffer, length);
}

unsigned int get_uint(const unsigned char *p)
{
  return (((unsigned int)(*p)<<24)|((unsigned int)(*(p+1))<<16)|((unsigned int)(*(p+2))<<8)|(unsigned int)(*(p+3)));
}

int get_png_info(const wchar_t *path, const wchar_t *name, int *width, int *height)
{
  int result=0;
  int f; 
  unsigned char tmp[128];
  const char IHDR[]={ 73,  72,  68,  82, '\0'};
  unsigned int length;
  if ((f=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,NULL))>=0)
  {
    if (fread(f,tmp,8)==8)
    {
      if (!png_sig_cmp((png_bytep)tmp,0,8))
      {
        for(;;)
        {
          if (fread(f,tmp,4)!=4) break;
          length=get_uint(tmp);
          if (fread(f,tmp,4)!=4) break;
          if (!memcmp(tmp,IHDR,4))
          {
            unsigned int _width, _height;
            fread(f,tmp,13);
            _width=get_uint(tmp);
            _height=get_uint(tmp+4);
            if (width) *width=_width;
            if (height) *height=_height;
            result=1;
            break;
          }
          else
          {
            if (lseek(f,length+4,1)!=(length+4)) break;
          }
        }
      }
    }
    fclose(f);
  }
  return (result);
}
