/* ioapi.c -- IO base function header for compress/uncompress .zip
   files using zlib + zip or unzip API

   Version 1.01e, February 12th, 2005

   Copyright (C) 1998-2005 Gilles Vollant
*/

#include "..\..\..\include\types.h"
#include "..\zlib.h"
#include "ioapi.h"

voidpf fopen_file_func (
   voidpf opaque,
   const wchar_t* name,
   int mode);

uLong fread_file_func (
   voidpf opaque,
   voidpf stream,
   void* buf,
   uLong size);

uLong fwrite_file_func (
   voidpf opaque,
   voidpf stream,
   const void* buf,
   uLong size);

long ftell_file_func (
   voidpf opaque,
   voidpf stream);

long fseek_file_func (
   voidpf opaque,
   voidpf stream,
   uLong offset,
   int origin);

int fclose_file_func (
   voidpf opaque,
   voidpf stream);

int ferror_file_func (
   voidpf opaque,
   voidpf stream);

void clear_read_buf(FILE* pFile)
{
	pFile->buflen = 0;
	pFile->bufpos = 0;
}

voidpf fopen_file_func (
   voidpf opaque,
   const wchar_t* name,
   int mode)
{
	FILE* pFile = malloc(sizeof(FILE));
	memset(pFile,0, sizeof(FILE));
	unsigned int flags = 0;
	unsigned int fmode = 0;
	if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
	{
		flags = WA_Read ;
		fmode = 0x1FF;
	}
	else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
	{
		flags = WA_Read | WA_Read ;
		fmode = 0x1FF;
	}
	else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
	{
		flags = WA_Read | WA_Read| WA_Create | WA_Truncate;
		fmode = 0x1FF;
	}

	if (name && pFile)
	{
		pFile->handle = w_fopen(name, flags, fmode, NULL);
		clear_read_buf(pFile);
	}

	if (pFile->handle == -1)
	{
		mfree(pFile);
		pFile = NULL;
	}

	return (voidpf)pFile;
}

uLong fread_file_func (
   voidpf opaque,
   voidpf stream,
   void* buf,
   uLong size)
{
	uLong ret = 0;
	FILE* pFile = (FILE*)stream;
	
	if (pFile)
	{
		// 1. Выбираем сколько можно из буфера
		uLong inbuf = pFile->buflen - pFile->bufpos;
		if (inbuf > 0)
		{
			uLong min = (inbuf < size) ? inbuf : size;
			memcpy(buf, &pFile->readbuf[pFile->bufpos], min);
			pFile->bufpos += min;
			ret += min;
			buf = (void*)((int)buf + min);
			size -= min;
			
			if (pFile->bufpos >= pFile->buflen)
				clear_read_buf(pFile);
		}
		
		// 2. Если буфер пуст и не все еще считали, то считываем
		if (size > 0)
		{
			if (size < IOAPI_READ_BUF_SIZE)
			{
				pFile->buflen = (uLong)w_fread(pFile->handle, pFile->readbuf,
					(size_t)IOAPI_READ_BUF_SIZE);
				pFile->bufpos = 0;

				uLong min = (pFile->buflen < size) ? pFile->buflen : size;
				memcpy(buf, pFile->readbuf, min);
				pFile->bufpos += min;
				ret += min;
				buf = (void*)((int)buf + min);
				size -= min;
			}
			else
			{
				ret += (uLong)w_fread(pFile->handle, buf, (size_t)size);
			}
		}
	}
	return ret;
}

uLong fwrite_file_func (
   voidpf opaque,
   voidpf stream,
   const void* buf,
   uLong size)
{
	uLong ret = 0;
	FILE* pFile = (FILE*)stream;
	if (pFile)
	{
		ret = (uLong)w_fwrite(pFile->handle, (void *)buf, (size_t)size);
		clear_read_buf(pFile);
	}
	return ret;
}

long ftell_file_func (
   voidpf opaque,
   voidpf stream)
{
	long ret = -1;
	FILE* pFile = (FILE*)stream;
	if (pFile)
	{
		ret = w_lseek(pFile->handle, 0, WSEEK_CUR);
		clear_read_buf(pFile);
	}

	return ret;
}

long fseek_file_func (
   voidpf opaque,
   voidpf stream,
   uLong offset,
   int origin)
{
	int fseek_origin = 0;
	long ret = -1;
	FILE* pFile = (FILE*)stream;
	if (pFile)
	{
		switch (origin)
		{
		case ZLIB_FILEFUNC_SEEK_CUR :
			fseek_origin = WSEEK_CUR;
			break;
		case ZLIB_FILEFUNC_SEEK_END :
			fseek_origin = WSEEK_END;
			break;
		case ZLIB_FILEFUNC_SEEK_SET :
			fseek_origin = WSEEK_SET;
			break;
		default:
			return -1;
		}
		w_lseek(pFile->handle, offset, fseek_origin);
		ret = 0;
		clear_read_buf(pFile);
	}
	return ret;
}

int fclose_file_func (
   voidpf opaque,
   voidpf stream)
{
	FILE* pFile = (FILE*)stream;
	if (pFile)
	{
		w_fclose(pFile->handle);
		mfree(pFile);
	}
	return 0;
}

int ferror_file_func (
   voidpf opaque,
   voidpf stream)
{
	int ret = -1;
	FILE* pFile = (FILE*)stream;
	if (pFile)
		ret = pFile->err;
	return ret;
}

void fill_fopen_filefunc (
  zlib_filefunc_def* pzlib_filefunc_def)
{
	pzlib_filefunc_def->zopen_file = fopen_file_func;
	pzlib_filefunc_def->zread_file = fread_file_func;
	pzlib_filefunc_def->zwrite_file = fwrite_file_func;
	pzlib_filefunc_def->ztell_file = ftell_file_func;
	pzlib_filefunc_def->zseek_file = fseek_file_func;
	pzlib_filefunc_def->zclose_file = fclose_file_func;
	pzlib_filefunc_def->zerror_file = ferror_file_func;
	pzlib_filefunc_def->opaque = NULL;
}
