/* stdio.h standard header */
#ifndef _STDIO
#define _STDIO

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _YVALS
  #include <yvals.h>
#endif
#include <ysizet.h>
_C_STD_BEGIN

                /* Module consistency. */
#pragma rtmodel="__dlib_file_descriptor",_STRINGIFY(_DLIB_FILE_DESCRIPTOR)

                /* macros */
#ifndef NULL
  #define NULL          _NULL
#endif /* NULL */

#define _IOFBF          0
#define _IOLBF          1
#define _IONBF          2

#define BUFSIZ          512
#define EOF             (-1)
#define FILENAME_MAX    _FNAMAX
#define FOPEN_MAX       _FOPMAX
#define L_tmpnam        _TNAMAX
#define TMP_MAX         32

#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

#if _DLIB_FILE_DESCRIPTOR
#define stdin           (&_CSTD _Stdin)
#define stdout          (&_CSTD _Stdout)
#define stderr          (&_CSTD _Stderr)
#endif /* _DLIB_FILE_DESCRIPTOR */

#if _MULTI_THREAD
  #define _Lockfile(str)        _Lockfilelock(str)
  #define _Unlockfile(str)      _Unlockfilelock(str)

#else /* _MULTI_THREAD */
  #define _Lockfile(x)          (void)0
  #define _Unlockfile(x)        (void)0
#endif /* _MULTI_THREAD */

                /* type definitions */
typedef _Fpost fpos_t;

                /* printf and scanf pragma support */
#pragma language=save
#pragma language=extended

#ifdef _HAS_PRAGMA_PRINTF_ARGS
  #define __PRINTFPR _Pragma("__printf_args")
  #define __SCANFPR  _Pragma("__scanf_args")
#else
  #define __PRINTFPR
  #define __SCANFPR
#endif


#if _DLIB_FILE_DESCRIPTOR
  #ifndef _FD_TYPE
    #define _FD_TYPE      signed char
  #endif  /* _FD_TYPE */

  typedef struct _Filet
  {       /* file control information */
    unsigned short _Mode;
    unsigned char _Lockno;
    _FD_TYPE _Handle;

    unsigned char *_Buf, *_Bend, *_Next;
    unsigned char *_Rend, *_Wend, *_Rback;

    _Wchart *_WRback, _WBack[2];
    unsigned char *_Rsave, *_WRend, *_WWend;

    struct _Mbstatet _Wstate;
    char *_Tmpnam;
    unsigned char _Back[_MBMAX], _Cbuf;
  } FILE;

                /* declarations */
  _C_LIB_DECL
  extern FILE _Stdin, _Stdout, _Stderr;

  __INTRINSIC void clearerr(FILE *);
//  __INTRINSIC int fclose(FILE *);
  __INTRINSIC int feof(FILE *);
  __INTRINSIC int ferror(FILE *);
  __INTRINSIC int fflush(FILE *);
  __INTRINSIC int fgetc(FILE *);
  __INTRINSIC int fgetpos(FILE *, fpos_t *);
  __INTRINSIC char * fgets(char *, int, FILE *);
//  __INTRINSIC FILE * fopen(const char *, const char *);
  __PRINTFPR __INTRINSIC int fprintf(FILE *, const char *, ...);
  __INTRINSIC int fputc(int, FILE *);
  __INTRINSIC int fputs(const char *, FILE *);
  __INTRINSIC size_t fread(void *, size_t, size_t, FILE *);
  __INTRINSIC FILE * freopen(const char *, const char *, FILE *);
  __SCANFPR __INTRINSIC int fscanf(FILE *, const char *, ...);
  __INTRINSIC int fseek(FILE *, long, int);
  __INTRINSIC int fsetpos(FILE *, const fpos_t *); 
  __INTRINSIC long ftell(FILE *);
  __INTRINSIC size_t fwrite(const void *, size_t, size_t, FILE *);
  __INTRINSIC void rewind(FILE *);
  __INTRINSIC void setbuf(FILE *, char *);
  __INTRINSIC int setvbuf(FILE *, char *, int, size_t);
  __INTRINSIC FILE * tmpfile(void);
  __INTRINSIC int ungetc(int, FILE *);
  __PRINTFPR __INTRINSIC int vfprintf(FILE *, const char *, __Va_list);
  #if _DLIB_ADD_C99_SYMBOLS
    __SCANFPR  __INTRINSIC int vfscanf(FILE *, const char *, __Va_list);
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #if _DLIB_ADD_EXTRA_SYMBOLS
    __INTRINSIC FILE * fdopen(_FD_TYPE, const char *);
    __INTRINSIC _FD_TYPE fileno(FILE *);
  #endif /* _DLIB_ADD_EXTRA_SYMBOLS */


  __INTRINSIC int _Nnl(FILE *, unsigned char *, unsigned char *);
  __INTRINSIC long _Fgpos(FILE *, fpos_t *);
  __INTRINSIC int _Flocale(FILE *, const char *, int);
  __INTRINSIC void _Fsetlocale(FILE *, int);
  __INTRINSIC int _Fspos(FILE *, const fpos_t *, long, int);

  #if _MULTI_THREAD
    __INTRINSIC void _Lockfilelock(_Filet *);
    __INTRINSIC void _Unlockfilelock(_Filet *);
  #endif /* _MULTI_THREAD */

  extern FILE *_Files[FOPEN_MAX];

  __INTRINSIC int getc(FILE *);
  __INTRINSIC int putc(int, FILE *);
  _END_C_LIB_DECL
#endif /* _DLIB_FILE_DESCRIPTOR */

_C_LIB_DECL
/* Corresponds to fgets(char *, int, stdin); */
__INTRINSIC char * __gets(char *, int);
__INTRINSIC char * gets(char *);
__INTRINSIC void perror(const char *);
__PRINTFPR __INTRINSIC int printf(const char *, ...);
__INTRINSIC int puts(const char *);
__INTRINSIC int remove(const char *);
__INTRINSIC int rename(const char *, const char *);
__SCANFPR  __INTRINSIC int scanf(const char *, ...);
//__PRINTFPR __INTRINSIC int sprintf(char *, const char *, ...);
//__SCANFPR  __INTRINSIC int sscanf(const char *, const char *, ...);
__INTRINSIC char * tmpnam(char *);
/* Corresponds to "ungetc(c, stdout)" */
__INTRINSIC int __ungetchar(int);
__PRINTFPR __INTRINSIC int vprintf(const char *, __Va_list);
#if _DLIB_ADD_C99_SYMBOLS
  __SCANFPR  __INTRINSIC int vscanf(const char *, __Va_list);
  __SCANFPR  __INTRINSIC int vsscanf(const char *, const char *, __Va_list);
#endif /* _DLIB_ADD_C99_SYMBOLS */
__PRINTFPR __INTRINSIC int vsprintf(char *, const char *, __Va_list);
/* Corresponds to fwrite(p, x, y, stdout); */
__INTRINSIC size_t __write_array(const void *, size_t, size_t);
#if _DLIB_ADD_C99_SYMBOLS
  __PRINTFPR __INTRINSIC int snprintf(char *, size_t, const char *, ...);
  __PRINTFPR __INTRINSIC int vsnprintf(char *, size_t, const char *, __Va_list);
#endif /* _DLIB_ADD_C99_SYMBOLS */

__INTRINSIC int getchar(void);
__INTRINSIC int putchar(int);

_END_C_LIB_DECL

#pragma language=restore

#if !(_MULTI_THREAD && _FILE_OP_LOCKS)
  #ifndef _NO_DEFINITIONS_IN_HEADER_FILES
    #if _DLIB_FILE_DESCRIPTOR
                /* inlines, for C and C++ */
      #pragma inline
      int (getc)(FILE *_Str)
      {
        return fgetc(_Str);
      }

      #pragma inline
      int (putc)(int _C, FILE *_Str)
      {
        return fputc(_C, _Str);
      }
    #endif

  #endif /* _NO_DEFINITIONS_IN_HEADER_FILES */
#endif /* !(_MULTI_THREAD && _FILE_OP_LOCKS) */
_C_STD_END
#endif /* _STDIO */

#if defined(_STD_USING) && defined(__cplusplus)
  using _CSTD fpos_t;
  using _CSTD clearerr; using _CSTD fclose; using _CSTD feof;
  using _CSTD ferror; using _CSTD fflush; using _CSTD fgetc;
  using _CSTD fgetpos; using _CSTD fgets; using _CSTD fopen;
  using _CSTD fprintf; using _CSTD fputc; using _CSTD fputs;
  using _CSTD fread; using _CSTD freopen; using _CSTD fscanf;
  using _CSTD fseek; using _CSTD fsetpos; using _CSTD ftell;
  using _CSTD fwrite; using _CSTD getc; using _CSTD getchar;
  using _CSTD gets; using _CSTD perror;
  using _CSTD putc; using _CSTD putchar;
  using _CSTD printf; using _CSTD puts; using _CSTD remove;
  using _CSTD rename; using _CSTD rewind; using _CSTD scanf;
  using _CSTD setbuf; using _CSTD setvbuf; using _CSTD sprintf;
  using _CSTD sscanf; using _CSTD tmpfile; using _CSTD tmpnam;
  using _CSTD ungetc; using _CSTD vfprintf; using _CSTD vprintf;
  using _CSTD vsprintf;
  #if _DLIB_ADD_EXTRA_SYMBOLS
    using _CSTD fdopen; using _CSTD fileno;
  #endif /* _DLIB_ADD_EXTRA_SYMBOLS */
  #if _DLIB_ADD_C99_SYMBOLS
    using _CSTD snprintf; using _CSTD vsnprintf;
    using _CSTD vscanf; using _CSTD vsscanf;
    using _CSTD vfscanf; 
  #endif /* _DLIB_ADD_C99_SYMBOLS */


  #if _DLIB_FILE_DESCRIPTOR
    using _CSTD FILE;
  #endif
#endif /* defined(_STD_USING) && defined(__cplusplus) */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V3.12:0576 */
