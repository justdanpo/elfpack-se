#include "..\\include\Lib_Clara.h"

char* strchr(char* str, char c)
{
	for(;*str;str++)if(*str==c)return str;
	return NULL;
}

/*
u16* wstrchr(u16* str,u16 c)
{
	for(;*str;str++)if(*str==c)return str;
	return NULL;
}
*/

char* strrchr(char* str, char c)
{
	char* retpos=NULL;
	for(;*str; str++) if(*str == c) retpos=str;
	return retpos;
}

//u16* wstrrchr(u16* str,u16 c) //declared in lib
/*
char* strncpy(char* dest,char* src,size_t len)
{
	for(; *src && len; len--)*dest++=*src++;
	if(len)*dest=0;
	return dest;
}
*/
char* strncpy(char* dest, const char* src, size_t len)
{
     if (len) 
     {
	  char       *d = dest;
	  const char *s = src;
	  while ((*d++ = *s++) && --len);                /* COPY STRING         */
	  if (len-- > 1) do *d++ = '\0'; while (--len);  /* TERMINATION PADDING */
     }
     return dest; 
}
	 
wchar_t* wstrncpy(wchar_t* dest, const wchar_t* src, size_t len)
{
	for(; *src && len; len--)*dest++=*src++;
	if(len)*dest=0;
	return dest;
}

int strncmp(char* s1,char* s2,size_t len)
{
	for(; *s1 && *s2 && len; len--, s1++, s2++)if(*s1-*s2)return *s1-*s2;
	if(len)return *s1-*s2;
	return 0;
}

int wstrncmp(wchar_t* s1, wchar_t* s2, size_t len)
{
	for(; *s1 && *s2 && len; len--, s1++, s2++)if(*s1-*s2)return *s1-*s2;
	if(len)return *s1-*s2;
	return 0;
}

char upper(char c)
{
	if(c>='a' && c<='z')c-='a'-'A';
	return c;
}

int strncmpi(const char* s1, const char* s2, size_t len)
{
	char c1, c2;
	for(; *s1 && *s2 && len; len--, s1++, s2++)
	{
		c1=upper(*s1);
		c2=upper(*s2);
		if(c1-c2)return c1-c2;
	}
	if(len)return c1-c2;
	return 0;
}

/*

char* strstr (const char * str1, const char * str2)
{
	char *cp = (char *) str1;
	char *s1, *s2;
	if ( !*str2 )
		return((char *)str1);
	while (*cp)
	{
		s1 = cp;
		s2 = (char *) str2;
		while ( *s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;
		if (!*s2)
			return(cp);
		cp++;
	}
	return(NULL);
}
*/
wchar_t* wstrstr (const wchar_t * str1, const wchar_t * str2)
{
	wchar_t *cp = (wchar_t *) str1;
	wchar_t *s1, *s2;
	if ( !*str2 )
		return((wchar_t *)str1);
	while (*cp)
	{
		s1 = cp;
		s2 = (wchar_t *) str2;
		while ( *s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;
		if (!*s2)
			return(cp);
		cp++;
	}
	return(NULL);
}
