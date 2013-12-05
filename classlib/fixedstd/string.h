/* string.h standard header */
#ifndef _STRING
#define _STRING

#ifndef _SYSTEM_BUILD
#pragma system_include
#endif

#ifndef _YVALS
  #include <yvals.h>
#endif
#include <ysizet.h>
_C_STD_BEGIN

                /* macros */
#ifndef NULL
  #define NULL  _NULL
#endif /* NULL */

                /* declarations */
_C_LIB_DECL
__INTRINSIC int memcmp(const void *, const void *, size_t);
__INTRINSIC void * memcpy(void *, const void *, size_t);
__INTRINSIC void * memmove(void *, const void *, size_t);
//__INTRINSIC void * memset(void *, int, size_t);
//__INTRINSIC char * strcat(char *, const char *);
//__INTRINSIC int strcmp(const char *, const char *);
__INTRINSIC int strcoll(const char *, const char *);
//__INTRINSIC char * strcpy(char *, const char *);
__INTRINSIC size_t strcspn(const char *, const char *);
__INTRINSIC char * strerror(int);
//__INTRINSIC size_t strlen(const char *);
__INTRINSIC char * strncat(char *, const char *, size_t);
__INTRINSIC int strncmp(const char *, const char *, size_t);
__INTRINSIC char * strncpy(char *, const char *, size_t);
__INTRINSIC size_t strspn(const char *, const char *);
__INTRINSIC char * strtok(char *, const char *);
__INTRINSIC size_t strxfrm(char *, const char *, size_t);
_END_C_LIB_DECL

                /* The implementations. */
_C_LIB_DECL
__INTRINSIC void *_Memchr(const void *, int, size_t);
__INTRINSIC char *_Strchr(const char *, int);
__INTRINSIC char *_Strerror(int, char *);
__INTRINSIC char *_Strpbrk(const char *, const char *);
__INTRINSIC char *_Strrchr(const char *, int);
__INTRINSIC char *_Strstr(const char *, const char *);
_END_C_LIB_DECL

/* IAR, we can't use the stratagem that Dinkum uses for memchr,... */
#ifdef __cplusplus
  __INTRINSIC const void *memchr(const void *_S, int _C, size_t _N);
  __INTRINSIC const char *strchr(const char *_S, int _C);
  __INTRINSIC const char *strpbrk(const char *_S, const char *_P);
  __INTRINSIC const char *strrchr(const char *_S, int _C);
//  __INTRINSIC const char *strstr(const char *_S, const char *_P);
  __INTRINSIC void *memchr(void *_S, int _C, size_t _N);
  __INTRINSIC char *strchr(char *_S, int _C);
  __INTRINSIC char *strpbrk(char *_S, const char *_P);
  __INTRINSIC char *strrchr(char *_S, int _C);
  __INTRINSIC char *strstr(char *_S, const char *_P);
#else /* !__cplusplus */
  __INTRINSIC void *memchr(const void *_S, int _C, size_t _N);
  __INTRINSIC char *strchr(const char *_S, int _C);
  __INTRINSIC char *strpbrk(const char *_S, const char *_P);
  __INTRINSIC char *strrchr(const char *_S, int _C);
  __INTRINSIC char *strstr(const char *_S, const char *_P);
#endif /* __cplusplus */

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES
                /* inlines and overloads, for C and C++ */
  _STD_BEGIN
  #ifdef __cplusplus
                /* First the const overloads for C++. */
    #pragma inline 
    const void *memchr(const void *_S, int _C, size_t _N)
    {
      return (_Memchr(_S, _C, _N)); 
    }

    #pragma inline 
    const char *strchr(const char *_S, int _C)
    {
      return (_Strchr(_S, _C)); 
    }

    #pragma inline 
    const char *strpbrk(const char *_S, const char *_P)
    {
      return (_Strpbrk(_S, _P)); 
    }

    #pragma inline 
    const char *strrchr(const char *_S, int _C)
    {
      return (_Strrchr(_S, _C)); 
    }

//    #pragma inline 
//    const char *strstr(const char *_S, const char *_P)
//    {
//      return (_Strstr(_S, _P)); 
//    }
                /* Then the non-const overloads for C++. */
    #pragma inline 
    void *memchr(void *_S, int _C, size_t _N)
    {
      return (_Memchr(_S, _C, _N)); 
    }

    #pragma inline 
    char *strchr(char *_S, int _C)
    {
      return (_Strchr(_S, _C)); 
    }

    #pragma inline 
    char *strpbrk(char *_S, const char *_P)
    {
      return (_Strpbrk(_S, _P)); 
    }

    #pragma inline 
    char *strrchr(char *_S, int _C)
    {
      return (_Strrchr(_S, _C)); 
    }

    #pragma inline 
    char *strstr(char *_S, const char *_P)
    {
      return (_Strstr(_S, _P)); 
    }

  #else /* !__cplusplus */
                /* Then the overloads for C. */
    #pragma inline 
    void *memchr(const void *_S, int _C, size_t _N)
    {
      return (_Memchr(_S, _C, _N)); 
    }

    #pragma inline 
    char *strchr(const char *_S, int _C)
    {
      return (_Strchr(_S, _C)); 
    }

    #pragma inline 
    char *strpbrk(const char *_S, const char *_P)
    {
      return (_Strpbrk(_S, _P)); 
    }

    #pragma inline 
    char *strrchr(const char *_S, int _C)
    {
      return (_Strrchr(_S, _C)); 
    }

    #pragma inline 
    char *strstr(const char *_S, const char *_P)
    {
      return (_Strstr(_S, _P)); 
    }
  #endif /* __cplusplus */

  #pragma inline 
  char *strerror(int _Err)
  {
    return (_Strerror(_Err, 0)); 
  }

  #ifdef _STRING_MORE_INLINES
    #pragma inline 
    int memcmp(const void *s1, const void *s2, size_t n) 
      /* Copied from memcmp.c */
    {       /* compare unsigned char s1[n], s2[n] */
      const unsigned char *su1 = (const unsigned char *)s1;
      const unsigned char *su2 = (const unsigned char *)s2;

      for (; 0 < n; ++su1, ++su2, --n)
        if (*su1 != *su2)
          return (*su1 < *su2 ? -1 : +1);
      return (0);
    }

    #pragma inline 
    void *memcpy(void *s1, const void *s2, size_t n)
      /* Copied from memcpy.c */
    {       /* copy char s2[n] to s1[n] in any order */
      char *su1 = (char *)s1;
      const char *su2 = (const char *)s2;
  
      for (; 0 < n; ++su1, ++su2, --n)
        *su1 = *su2;
      return (s1);
    }

//    #pragma inline 
//    void *memset(void *s, int c, size_t n) /* Copied from memset.c */
//    {       /* store c throughout unsigned char s[n] */
//      const unsigned char uc = c;
//      unsigned char *su = (unsigned char *)s;
//        
//      for (; 0 < n; ++su, --n)
//       *su = uc;
//      return (s);
//    }

    #pragma inline 
    char *strcat(char *s1, const char *s2) /* Copied from strcat.c */
    {       /* copy char s2[] to end of s1[] */
      char *s;
  
      for (s = s1; *s != '\0'; ++s)
        ;                   /* find end of s1[] */
      for (; (*s = *s2) != '\0'; ++s, ++s2)
        ;                   /* copy s2[] to end */
      return (s1);
    }

    #pragma inline 
    int strcmp(const char *s1, const char *s2) /* Copied from strcmp.c */
    {       /* compare unsigned char s1[], s2[] */
      for (; *s1 == *s2; ++s1, ++s2)
        if (*s1 == '\0')
          return (0);
      return (*(unsigned char *)s1 < *(unsigned char *)s2
              ? -1 : +1);
    }

    #pragma inline 
    char *strcpy(char *s1, const char *s2) /* Copied from strcpy.c */
    {       /* copy char s2[] to s1[] */
      char *s = s1;
  
      for (s = s1; (*s++ = *s2++) != '\0'; )
        ;
      return (s1);
    }

    #pragma inline 
    size_t strcspn(const char *s1, const char *s2)
      /* Copied from strcspn.c */
    {       /* find index of first s1[i] that matches any s2[] */
      const char *sc1, *sc2;

      for (sc1 = s1; *sc1 != '\0'; ++sc1)
        for (sc2 = s2; *sc2 != '\0'; ++sc2)
          if (*sc1 == *sc2)
            return (sc1 - s1);
      return (sc1 - s1);    /* terminating nulls match */
    }

    #pragma inline 
    size_t strlen(const char *s) /* Copied from strlen.c */
    {       /* find length of s[] */
      const char *sc;

      for (sc = s; *sc != '\0'; ++sc)
        ;
      return (sc - s);
    }

    #pragma inline 
    char *strncat(char *s1, const char *s2, size_t n) 
      /* Copied from strncat.c */
    {       /* copy char s2[max n] to end of s1[] */
      char *s;
  
      for (s = s1; *s != '\0'; ++s)
        ;   /* find end of s1[] */
      for (; 0 < n && *s2 != '\0'; --n)
        *s++ = *s2++;       /* copy at most n chars from s2[] */
      *s = '\0';
      return (s1);
    }

    #pragma inline 
    int strncmp(const char *s1, const char *s2, size_t n) 
      /* Copied from strncmp.c */
    {       /* compare unsigned char s1[max n], s2[max n] */
      for (; 0 < n; ++s1, ++s2, --n)
        if (*s1 != *s2)
          return (  *(unsigned char *)s1
                  < *(unsigned char *)s2 ? -1 : +1);
        else if (*s1 == '\0')
          return (0);
      return (0);
    }

    #pragma inline 
    char *strncpy(char *s1, const char *s2, size_t n) 
      /* Copied from strncpy.c */
    {       /* copy char s2[max n] to s1[n] */
      char *s;
        
      for (s = s1; 0 < n && *s2 != '\0'; --n)
        *s++ = *s2++;       /* copy at most n chars from s2[] */
      for (; 0 < n; --n)
        *s++ = '\0';
      return (s1);
    }

    #pragma inline 
    size_t strspn(const char *s1, const char *s2) /* Copied from strspn.c */
    {       /* find index of first s1[i] that matches no s2[] */
      const char *sc1, *sc2;
        
      for (sc1 = s1; *sc1 != '\0'; ++sc1)
        for (sc2 = s2; ; ++sc2)
          if (*sc2 == '\0')
            return (sc1 - s1);
          else if (*sc1 == *sc2)
            break;
      return (sc1 - s1);    /* null doesn't match */
    }

    #pragma inline 
    void *_Memchr(const void *s, int c, size_t n) /* Copied from memchr.c */
    {       /* find first occurrence of c in s[n] */
      const unsigned char uc = c;
      const unsigned char *su = (const unsigned char *)s;
        
      for (; 0 < n; ++su, --n)
        if (*su == uc)
          return ((void *)su);
      return (0);
    }

    #pragma inline 
    char *_Strchr(const char *s, int c) /* Copied from strchr.c */
    {       /* find first occurrence of c in char s[] */
      const char ch = c;
        
      for (; *s != ch; ++s)
        if (*s == '\0')
          return (0);
      return ((char *)s);
    }

    #pragma inline 
    char *_Strpbrk(const char *s1, const char *s2) 
      /* Copied from strpbrk.c */
    {       /* find index of first s1[i] that matches any s2[] */
      const char *sc1, *sc2;
  
      for (sc1 = s1; *sc1 != '\0'; ++sc1)
        for (sc2 = s2; *sc2 != '\0'; ++sc2)
          if (*sc1 == *sc2)
            return ((char *)sc1);
      return (0);   /* terminating nulls match */
    }

    #pragma inline 
    char *_Strrchr(const char *s, int c) /* Copied from strrchr.c */
    {       /* find last occurrence of c in char s[] */
      const char ch = c;
      const char *sc;
  
      for (sc = 0; ; ++s)
      {     /* check another char */
        if (*s == ch)
          sc = s;
        if (*s == '\0')
          return ((char *)sc);
      }
    }

    #pragma inline 
    char *_Strstr(const char *s1, const char *s2) /* Copied from strstr.c */
    {       /* find first occurrence of s2[] in s1[] */
      if (*s2 == '\0')
        return ((char *)s1);
      for (; (s1 = _Strchr(s1, *s2)) != 0; ++s1)
      {     /* match rest of prefix */
        const char *sc1, *sc2;
          
        for (sc1 = s1, sc2 = s2; ; )
          if (*++sc2 == '\0')
            return ((char *)s1);
          else if (*++sc1 != *sc2)
            break;
      }
      return (0);
    }
  #endif /* _STRING_MORE_INLINES */
  _STD_END
#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

#endif /* _STRING */

#if defined(_STD_USING) && defined(__cplusplus)
  using _CSTD memchr; using _CSTD memcmp;
  using _CSTD memcpy; using _CSTD memmove; using _CSTD memset;
  using _CSTD strcat; using _CSTD strchr; using _CSTD strcmp;
  using _CSTD strcoll; using _CSTD strcpy; using _CSTD strcspn;
  using _CSTD strerror; using _CSTD strlen; using _CSTD strncat;
  using _CSTD strncmp; using _CSTD strncpy; using _CSTD strpbrk;
  using _CSTD strrchr; using _CSTD strspn; using _CSTD strstr;
  using _CSTD strtok; using _CSTD strxfrm;
#endif /* defined(_STD_USING) && defined(__cplusplus) */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V3.12:0576 */
