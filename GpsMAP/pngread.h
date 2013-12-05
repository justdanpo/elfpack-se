#ifndef _PNGREAD_H_
  #define _PNGREAD_H_





typedef struct {
  int index_x;
  int index_y;
  int width;
  int height;
  char *image;
}PNG_LIST;

typedef struct {
  LIST *png_files;
  png_structp png_ptr;
  png_infop info_ptr;
  png_uint_32 width;
  png_uint_32 height;
  png_uint_32 bytes_in_row;
  png_bytep row_pointer;
  int index_x;
  int index_y;
  char *image;
}PNG_CONVERT;


PNG_CONVERT *InitPng(LIST *pnglist, int index_x, int index_y);
void Destr_Png(PNG_CONVERT *usr_png);
void RemovePngListElement(PNG_LIST *png_list);
void process_data(PNG_CONVERT *usr_png, char *buffer, unsigned int length);
int get_png_info(const wchar_t *path, const wchar_t *name, int *width, int *height);

#endif
