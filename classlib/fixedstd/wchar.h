/* wchar.h standard header */
#ifndef _WCHAR
#define _WCHAR

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _YVALS
  #include <yvals.h>
#endif
#include <ysizet.h>
_C_STD_BEGIN

                /* MACROS */
#ifndef NULL
  #define NULL  _NULL
#endif /* NULL */

#define WCHAR_MIN       _WCMIN
#define WCHAR_MAX       _WCMAX
#define WEOF    ((wint_t)(-1))

#if _WCMAX < __UNSIGNED_SHORT_MAX__
  #error "<wchart.h> wchar_t is too small."
#endif

                /* TYPE DEFINITIONS */
typedef _Mbstatet mbstate_t;

struct tm;
struct _Filet;

#ifndef _WCHART
  #define _WCHART
  typedef _Wchart wchar_t;
#endif /* _WCHART */

#ifndef _WINTT
  #define _WINTT
  typedef _Wintt wint_t;
#endif /* _WINT */

_C_LIB_DECL
                /* stdio DECLARATIONS */
#if _DLIB_FILE_DESCRIPTOR
  __INTRINSIC wint_t fgetwc(struct _Filet *);
  __INTRINSIC wchar_t * fgetws(wchar_t *, int, struct _Filet *);
  __INTRINSIC wint_t fputwc(wchar_t, struct _Filet *);
  __INTRINSIC int fputws(const wchar_t *, struct _Filet *);
  __INTRINSIC int fwide(struct _Filet *, int);
  __INTRINSIC int fwprintf(struct _Filet *,
                         const wchar_t *, ...);
  __INTRINSIC int fwscanf(struct _Filet *,
                        const wchar_t *, ...);
  __INTRINSIC wint_t getwc(struct _Filet *);
  __INTRINSIC wint_t putwc(wchar_t, struct _Filet *);
  __INTRINSIC wint_t ungetwc(wint_t, struct _Filet *);
  __INTRINSIC int vfwprintf(struct _Filet *,
                          const wchar_t *, __Va_list);
  #if _DLIB_ADD_C99_SYMBOLS
    __INTRINSIC int vfwscanf(struct _Filet *,
                             const wchar_t *, __Va_list);
  #endif /* _DLIB_ADD_C99_SYMBOLS */

#endif /* _DLIB_FILE_DESCRIPTOR */

__INTRINSIC wint_t getwchar(void);
__INTRINSIC wint_t __ungetwchar(wint_t);
__INTRINSIC wint_t putwchar(wchar_t);
__INTRINSIC int swprintf(wchar_t *, size_t,
                         const wchar_t *, ...);
//__INTRINSIC int swscanf(const wchar_t *,
//                        const wchar_t *, ...);
__INTRINSIC int vswprintf(wchar_t *, size_t,
                          const wchar_t *, __Va_list);
__INTRINSIC int vwprintf(const wchar_t *, __Va_list);
#if _DLIB_ADD_C99_SYMBOLS
  __INTRINSIC int vswscanf(const wchar_t *, const wchar_t *, __Va_list);
  __INTRINSIC int vwscanf(const wchar_t *, __Va_list);
#endif /* _DLIB_ADD_C99_SYMBOLS */
__INTRINSIC int wprintf(const wchar_t *, ...);
__INTRINSIC int wscanf(const wchar_t *, ...);

                /* stdlib DECLARATIONS */
__INTRINSIC size_t mbrlen(const char *, size_t, mbstate_t *);
__INTRINSIC size_t mbrtowc(wchar_t *, const char *, size_t,
                           mbstate_t *);
__INTRINSIC size_t mbsrtowcs(wchar_t *, const char **, size_t,
                             mbstate_t *);
__INTRINSIC int mbsinit(const mbstate_t *);
__INTRINSIC size_t wcrtomb(char *, wchar_t, mbstate_t *);
__INTRINSIC size_t wcsrtombs(char *, const wchar_t **, size_t,
                             mbstate_t *);
__INTRINSIC long wcstol(const wchar_t *, wchar_t **, int);
__INTRINSIC unsigned long wcstoul(const wchar_t *, wchar_t **, int);

                /* string DECLARATIONS */
__INTRINSIC wchar_t * wcscat(wchar_t *, const wchar_t *);
__INTRINSIC int wcscmp(const wchar_t *, const wchar_t *);
__INTRINSIC int wcscoll(const wchar_t *, const wchar_t *);
__INTRINSIC wchar_t * wcscpy(wchar_t *, const wchar_t *);
__INTRINSIC size_t wcscspn(const wchar_t *, const wchar_t *);
__INTRINSIC size_t wcslen(const wchar_t *);
__INTRINSIC wchar_t * wcsncat(wchar_t *,
                              const wchar_t *, size_t);
__INTRINSIC int wcsncmp(const wchar_t *, const wchar_t *, size_t);
__INTRINSIC wchar_t * wcsncpy(wchar_t *,
                              const wchar_t *, size_t);
__INTRINSIC size_t wcsspn(const wchar_t *, const wchar_t *);
__INTRINSIC wchar_t * wcstok(wchar_t *, const wchar_t *,
                             wchar_t **);
__INTRINSIC size_t wcsxfrm(wchar_t *,
                           const wchar_t *, size_t);
__INTRINSIC int wmemcmp(const wchar_t *, const wchar_t *, size_t);
__INTRINSIC wchar_t * wmemcpy(wchar_t *,
                              const wchar_t *, size_t);
__INTRINSIC wchar_t * wmemmove(wchar_t *, const wchar_t *, size_t);
__INTRINSIC wchar_t * wmemset(wchar_t *, wchar_t, size_t);

                /* time DECLARATIONS */
__INTRINSIC size_t wcsftime(wchar_t *, size_t,
                            const wchar_t *, const struct tm *);


__INTRINSIC wint_t btowc(int);
#if _DLIB_ADD_C99_SYMBOLS
  __INTRINSIC float wcstof(const wchar_t *, wchar_t **);
  __INTRINSIC long double wcstold(const wchar_t *, wchar_t **);
#endif /* _DLIB_ADD_C99_SYMBOLS */
__INTRINSIC double wcstod(const wchar_t *, wchar_t **);
__INTRINSIC int wctob(wint_t);

__INTRINSIC wint_t _Btowc(int);
__INTRINSIC int _Wctob(wint_t);
__INTRINSIC double _WStod(const wchar_t *, wchar_t **, long);
__INTRINSIC float _WStof(const wchar_t *, wchar_t **, long);
__INTRINSIC long double _WStold(const wchar_t *, wchar_t **, long);
__INTRINSIC unsigned long _WStoul(const wchar_t *, wchar_t **, int);

__INTRINSIC wchar_t * _Wmemchr(const wchar_t *, wchar_t, size_t);
__INTRINSIC wchar_t * _Wcschr(const wchar_t *, wchar_t);
__INTRINSIC wchar_t * _Wcspbrk(const wchar_t *, const wchar_t *);
__INTRINSIC wchar_t * _Wcsrchr(const wchar_t *, wchar_t);
__INTRINSIC wchar_t * _Wcsstr(const wchar_t *, const wchar_t *);
_END_C_LIB_DECL

/* IAR, can't use the Dinkum stratagem for wmemchr,... */

#ifdef __cplusplus
  __INTRINSIC const wchar_t * wmemchr(const wchar_t *, wchar_t, size_t);
  __INTRINSIC const wchar_t * wcschr(const wchar_t *, wchar_t);
  __INTRINSIC const wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
  __INTRINSIC const wchar_t * wcsrchr(const wchar_t *, wchar_t);
  __INTRINSIC const wchar_t * wcsstr(const wchar_t *, const wchar_t *);
  __INTRINSIC wchar_t * wmemchr(wchar_t *, wchar_t, size_t);
  __INTRINSIC wchar_t * wcschr(wchar_t *, wchar_t);
  __INTRINSIC wchar_t * wcspbrk(wchar_t *, const wchar_t *);
  __INTRINSIC wchar_t * wcsrchr(wchar_t *, wchar_t);
  __INTRINSIC wchar_t * wcsstr(wchar_t *, const wchar_t *);
#else /* !__cplusplus */
  __INTRINSIC wchar_t * wmemchr(const wchar_t *, wchar_t, size_t);
  __INTRINSIC wchar_t * wcschr(const wchar_t *, wchar_t);
  __INTRINSIC wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
  __INTRINSIC wchar_t * wcsrchr(const wchar_t *, wchar_t);
  __INTRINSIC wchar_t * wcsstr(const wchar_t *, const wchar_t *);
#endif /* __cplusplus */

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES
  #ifdef __cplusplus
                /* INLINES AND OVERLOADS, FOR C++ */

    inline const wchar_t * wmemchr(const wchar_t *_S, wchar_t _C, size_t _N)
    {
      return (_Wmemchr(_S, _C, _N));
    }

    inline const wchar_t * wcschr(const wchar_t *_S, wchar_t _C)
    {
      return (_Wcschr(_S, _C));
    }

    inline const wchar_t * wcspbrk(const wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcspbrk(_S, _P));
    }

    inline const wchar_t * wcsrchr(const wchar_t *_S, wchar_t _C)
    {
      return (_Wcsrchr(_S, _C));
    }

    inline const wchar_t * wcsstr(const wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcsstr(_S, _P));
    }

    inline wchar_t * wmemchr(wchar_t *_S, wchar_t _C, size_t _N)
    {
      return (_Wmemchr(_S, _C, _N));
    }

    inline wchar_t * wcschr(wchar_t *_S, wchar_t _C)
    {
      return (_Wcschr(_S, _C));
    }

    inline wchar_t * wcspbrk(wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcspbrk(_S, _P));
    }

    inline wchar_t * wcsrchr(wchar_t *_S, wchar_t _C)
    {
      return (_Wcsrchr(_S, _C));
    }

    inline wchar_t * wcsstr(wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcsstr(_S, _P));
    }

  #else /* __cplusplus */
    #pragma inline
    wchar_t * wmemchr(const wchar_t *_S, wchar_t _C, size_t _N)
    {
      return (_Wmemchr(_S, _C, _N));
    }

    #pragma inline
    wchar_t * wcschr(const wchar_t *_S, wchar_t _C)
    {
      return (_Wcschr(_S, _C));
    }

    #pragma inline
    wchar_t * wcspbrk(const wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcspbrk(_S, _P));
    }

    #pragma inline
    wchar_t * wcsrchr(const wchar_t *_S, wchar_t _C)
    {
      return (_Wcsrchr(_S, _C));
    }

    #pragma inline
    wchar_t * wcsstr(const wchar_t *_S, const wchar_t *_P)
    {
      return (_Wcsstr(_S, _P));
    }
  #endif /* __cplusplus */

  #pragma inline
  wint_t btowc(int _C)
  {       /* convert single byte to wide character */
    return (_Btowc(_C));
  }

  #if _DLIB_ADD_C99_SYMBOLS
    #pragma inline
    float wcstof(const wchar_t *_S,
                 wchar_t **_Endptr)
    {       /* convert wide string to double */
      return (_WStof(_S, _Endptr, 0));
    }

    #pragma inline
    long double wcstold(const wchar_t *_S,
                        wchar_t **_Endptr)
    {       /* convert wide string to double */
      return (_WStold(_S, _Endptr, 0));
    }
  #endif /* _DLIB_ADD_C99_SYMBOLS */

  #pragma inline
  double wcstod(const wchar_t *_S,
                wchar_t **_Endptr)
  {       /* convert wide string to double */
    return (_WStod(_S, _Endptr, 0));
  }


  #pragma inline
  unsigned long wcstoul(const wchar_t *_S,
                        wchar_t **_Endptr, int _Base)
  {       /* convert wide string to unsigned long */
    return (_WStoul(_S, _Endptr, _Base));
  }

  #pragma inline
  int wctob(wint_t _Wc)
  {       /* convert wide character to single byte */
    return (_Wctob(_Wc));
  }

#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

#pragma inline
static wchar_t _WLC(wchar_t _C)
{  /* Convert wide character to lower case. */
  return (_C | (L'a' - L'A'));
}

_C_STD_END
#endif /* _WCHAR */

#if defined(_STD_USING) && defined(__cplusplus)
  using _CSTD mbstate_t; using _CSTD tm; using _CSTD wint_t;

  using _CSTD btowc; using _CSTD fgetwc; using _CSTD fgetws; using _CSTD fputwc;
  using _CSTD fputws; using _CSTD fwide; using _CSTD fwprintf;
  using _CSTD fwscanf; using _CSTD getwc; using _CSTD getwchar;
  using _CSTD mbrlen; using _CSTD mbrtowc; using _CSTD mbsrtowcs;
  using _CSTD mbsinit; using _CSTD putwc; using _CSTD putwchar;
  using _CSTD swprintf; using _CSTD swscanf; using _CSTD ungetwc;
  using _CSTD vfwprintf; using _CSTD vswprintf; using _CSTD vwprintf;
  using _CSTD wcrtomb; using _CSTD wprintf; using _CSTD wscanf;
  using _CSTD wcsrtombs; using _CSTD wcstol; using _CSTD wcscat;
  using _CSTD wcschr; using _CSTD wcscmp; using _CSTD wcscoll;
  using _CSTD wcscpy; using _CSTD wcscspn; using _CSTD wcslen;
  using _CSTD wcsncat; using _CSTD wcsncmp; using _CSTD wcsncpy;
  using _CSTD wcspbrk; using _CSTD wcsrchr; using _CSTD wcsspn;
  using _CSTD wcstod;
  using _CSTD wcstoul; using _CSTD wcsstr;
  using _CSTD wcstok; using _CSTD wcsxfrm; using _CSTD wctob;
  using _CSTD wmemchr; using _CSTD wmemcmp; using _CSTD wmemcpy;
  using _CSTD wmemmove; using _CSTD wmemset; using _CSTD wcsftime;
  #if _DLIB_ADD_C99_SYMBOLS
    using _CSTD vfwscanf; using _CSTD vswscanf; using _CSTD vwscanf;
    using _CSTD wcstof; using _CSTD wcstold;
  #endif /* _DLIB_ADD_C99_SYMBOLS */
#endif /* defined(_STD_USING) && defined(__cplusplus) */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V3.12:0576 */
