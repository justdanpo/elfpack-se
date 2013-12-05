/* stdlib.h standard header */
#ifndef _STDLIB
#define _STDLIB

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _YVALS
  #include <yvals.h>
#endif
#include <ysizet.h>
#include <xencoding_limits.h>
_C_STD_BEGIN

                /* MACROS */
#ifndef NULL
  #define NULL  _NULL
#endif /* NULL */

#define EXIT_FAILURE    _EXFAIL
#define EXIT_SUCCESS    0

#define MB_CUR_MAX    _ENCODING_CUR_MAX

#if _ILONG
  #define RAND_MAX      0x3fffffff
#else /* _ILONG */
  #define RAND_MAX      0x7fff
#endif /* _ILONG */

                /* TYPE DEFINITIONS */
#ifndef _WCHART
  #define _WCHART
  typedef _Wchart wchar_t;
#endif /* _WCHART */

typedef struct
{       /* result of int divide */
  int quot;
  int rem;
} div_t;

typedef struct
{       /* result of long divide */
  long quot;
  long rem;
} ldiv_t;

#if _DLIB_ADD_C99_SYMBOLS
  #ifdef _LONGLONG
    typedef struct
    {     /* result of long long divide */
      _Longlong quot;
      _Longlong rem;
    } lldiv_t;
#endif
#endif /* _DLIB_ADD_C99_SYMBOLS */

                /* DECLARATIONS */
_EXTERN_C /* low-level functions */
__INTRINSIC int atexit(void (*)(void));
#if _DLIB_ADD_C99_SYMBOLS
  #pragma object_attribute = __noreturn
  __INTRINSIC void _Exit(int) _NO_RETURN; /* added with C99 */
#endif /* _DLIB_ADD_C99_SYMBOLS */
#pragma object_attribute = __noreturn
__INTRINSIC void exit(int) _NO_RETURN;
__INTRINSIC char * getenv(const char *);
__INTRINSIC int system(const char *);
_END_EXTERN_C

_C_LIB_DECL
#pragma object_attribute = __noreturn
__INTRINSIC void abort(void) _NO_RETURN;
__INTRINSIC int abs(int);
__INTRINSIC void * calloc(size_t, size_t);
__INTRINSIC div_t div(int, int);
__INTRINSIC void free(void *);
__INTRINSIC long labs(long);
__INTRINSIC ldiv_t ldiv(long, long);
#if _DLIB_ADD_C99_SYMBOLS
  #ifdef _LONGLONG
    __INTRINSIC long long llabs(long long);
    __INTRINSIC lldiv_t lldiv(long long, long long);
  #endif
#endif /* _DLIB_ADD_C99_SYMBOLS */
//__INTRINSIC void * malloc(size_t);
__INTRINSIC int mblen(const char *, size_t);
__INTRINSIC size_t mbstowcs(wchar_t *, const char *, size_t);
__INTRINSIC int mbtowc(wchar_t *, const char *, size_t);
__INTRINSIC int rand(void);
__INTRINSIC void srand(unsigned int);
__INTRINSIC void * realloc(void *, size_t);
__INTRINSIC long strtol(const char *, char **, int);
__INTRINSIC unsigned long strtoul(const char *, char **, int);
__INTRINSIC size_t wcstombs(char *, const wchar_t *, size_t);
__INTRINSIC int wctomb(char *, wchar_t);
#if _DLIB_ADD_C99_SYMBOLS
  #ifdef _LONGLONG
    __INTRINSIC long long strtoll(const char *, char **, int);
    __INTRINSIC unsigned long long strtoull(const char *, char **, int);
  #endif
#endif /* _DLIB_ADD_C99_SYMBOLS */

#pragma language=save
#pragma language=extended

#define __HEAP_MEM_HELPER1__(M, I)                              \
__INTRINSIC void M##_free(void M *);                            \
__INTRINSIC void M * M##_malloc(M##_size_t);                    \
__INTRINSIC void M * M##_calloc(M##_size_t, M##_size_t);        \
__INTRINSIC void M * M##_realloc(void M *, M##_size_t);
__HEAP_MEMORY_LIST1__()
#undef __HEAP_MEM_HELPER1__

#ifndef __DEF_HEAP_MEM__
#define __DEF_HEAP_MEM__ __DEF_PTR_MEM__
#endif

#pragma inline
void free(void * _P)
{
  _GLUE(__DEF_HEAP_MEM__,_free(_P));
}
//#pragma inline
//void * malloc(size_t _S)
//{
//  return _GLUE(__DEF_HEAP_MEM__,_malloc(_S));
//
//}
#pragma inline
void * realloc(void * _P, size_t _S)
{
  return _GLUE(__DEF_HEAP_MEM__,_realloc(_P, _S));
}
#pragma inline
void * calloc(size_t _N, size_t _S)
{
  return _GLUE(__DEF_HEAP_MEM__,_calloc(_N, _S));
}

#pragma language=restore


__INTRINSIC unsigned long _Stoul(const char *, char **, int);
__INTRINSIC float _Stof(const char *, char **, long);
__INTRINSIC double _Stod(const char *, char **, long);
__INTRINSIC long double _Stold(const char *, char **, long);
#ifdef _LONGLONG
  __INTRINSIC _Longlong _Stoll(const char *, char **, int);
  __INTRINSIC _ULonglong _Stoull(const char *, char **, int);
#endif

typedef int _Cmpfun(const void *, const void *);
__INTRINSIC void * bsearch(const void *, const void *, size_t, size_t,
                           _Cmpfun *);
__INTRINSIC void qsort(void *, size_t, size_t, _Cmpfun *);
__INTRINSIC void __qsortbbl(void *, size_t, size_t, _Cmpfun *);
__INTRINSIC double atof(const char *);
__INTRINSIC int atoi(const char *);
__INTRINSIC long atol(const char *);
#if _DLIB_ADD_C99_SYMBOLS
  #ifdef _LONGLONG
    __INTRINSIC long long atoll(const char *);
  #endif
  __INTRINSIC float strtof(const char *, char **);
  __INTRINSIC long double strtold(const char *, char **);
#endif /* _DLIB_ADD_C99_SYMBOLS */
__INTRINSIC double strtod(const char *, char **);
__INTRINSIC size_t _Mbcurmax(void);

_END_C_LIB_DECL

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES
  _EXTERN_C
  typedef void _Atexfun(void);
  _END_EXTERN_C
  #if _HAS_STRICT_LINKAGE && defined(__cplusplus)

    typedef int _Cmpfun2(const void *, const void *);

    #pragma inline
    int atexit(void (*_Pfn)(void))
    {     // register a function to call at exit
      return (atexit((_Atexfun *)_Pfn));
    }

    #pragma inline
    void * bsearch(const void *_Key, const void *_Base,
                   size_t _Nelem, size_t _Size, _Cmpfun2 *_Cmp)
    {     // search by binary chop
      return (bsearch(_Key, _Base, _Nelem, _Size, (_Cmpfun *)_Cmp));
    }

    #pragma inline
    void qsort(void *_Base, size_t _Nelem, size_t _Size, _Cmpfun2 *_Cmp)
    {     // sort
      qsort(_Base, _Nelem, _Size, (_Cmpfun *)_Cmp);
    }
  #endif /* _HAS_STRICT_LINKAGE */

                /* INLINES, FOR C and C++ */
  #pragma inline
  double atof(const char *_S)
  {      /* convert string to double */
    return (_Stod(_S, 0, 0));
  }

  #pragma inline
  int atoi(const char *_S)
  {      /* convert string to int */
    return ((int)_Stoul(_S, 0, 10));
  }

  #pragma inline
  long atol(const char *_S)
  {      /* convert string to long */
    return ((long)_Stoul(_S, 0, 10));
  }

  #if _DLIB_ADD_C99_SYMBOLS
    #ifdef _LONGLONG
      #pragma inline
      long long atoll(const char *_S)
      {      /* convert string to long long */
        return ((long long)_Stoull(_S, 0, 10));
      }
    #endif
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #pragma inline
  double strtod(const char *_S, char **_Endptr)
  {      /* convert string to double, with checking */
    return (_Stod(_S, _Endptr, 0));
  }

  #if _DLIB_ADD_C99_SYMBOLS
    #pragma inline
    float strtof(const char *_S, char **_Endptr)
    {      /* convert string to float, with checking */
      return (_Stof(_S, _Endptr, 0));
    }

    #pragma inline
    long double strtold(const char *_S, char **_Endptr)
    {      /* convert string to long double, with checking */
      return (_Stold(_S, _Endptr, 0));
    }
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #pragma inline
  unsigned long strtoul(const char *_S, char **_Endptr, int _Base)
  {      /* convert string to unsigned long, with checking */
    return (_Stoul(_S, _Endptr, _Base));
  }

  #if _DLIB_ADD_C99_SYMBOLS
    #ifdef _LONGLONG
      #pragma inline
      long long strtoll(const char *_S, char **_Endptr, int _Base)
      {      /* convert string to long long, with checking */
        return (_Stoll(_S, _Endptr, _Base));
      }

      #pragma inline
      unsigned long long strtoull(const char *_S, char **_Endptr, int _Base)
      {      /* convert string to unsigned long long, with checking */
        return (_Stoull(_S, _Endptr, _Base));
      }
    #endif /* _LONGLONG */
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #pragma inline
  int abs(int i)
  {      /* compute absolute value of int argument */
    return (i < 0 ? -i : i);
  }

  #pragma inline
  long labs(long i)
  {      /* compute absolute value of long argument */
    return (i < 0 ? -i : i);
  }

  #if _DLIB_ADD_C99_SYMBOLS
    #ifdef _LONGLONG
      #pragma inline
      long long llabs(long long i)
      {      /* compute absolute value of long long argument */
        return (i < 0 ? -i : i);
      }
    #endif
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #ifdef __cplusplus
    #pragma inline
    long abs(long _X)     /* OVERLOADS */
    {     /* compute abs */
      return (labs(_X));
    }

    #pragma inline
    ldiv_t div(long _X, long _Y)
    {     /* compute quotient and remainder */
      return (ldiv(_X, _Y));
    }
  #endif /* __cplusplus */
#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

_C_STD_END
#endif /* _STDLIB */

#if defined(_STD_USING) && defined(__cplusplus)
  using _CSTD div_t; using _CSTD ldiv_t;

  using _CSTD abort; using _CSTD abs; using _CSTD atexit;
  using _CSTD atof; using _CSTD atoi; using _CSTD atol;
  using _CSTD bsearch; using _CSTD calloc; using _CSTD div;
  using _CSTD exit; using _CSTD free; using _CSTD getenv;
  using _CSTD labs; using _CSTD ldiv; using _CSTD malloc;
  using _CSTD mblen; using _CSTD mbstowcs; using _CSTD mbtowc;
  using _CSTD qsort; using _CSTD rand; using _CSTD realloc;
  using _CSTD srand; using _CSTD strtod; 
  using _CSTD strtol; using _CSTD strtoul; using _CSTD system;
  using _CSTD wcstombs; using _CSTD wctomb;
  #if _DLIB_ADD_C99_SYMBOLS
    using _CSTD strtold; using _CSTD strtof;
    #ifdef _LONGLONG
      using _CSTD lldiv_t;

      using _CSTD atoll; using _CSTD strtoll; using _CSTD strtoull;
      using _CSTD llabs; using _CSTD lldiv;
    #endif
  #endif /* _DLIB_ADD_C99_SYMBOLS */
#endif /* defined(_STD_USING) && defined(__cplusplus) */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V3.12:0576 */
