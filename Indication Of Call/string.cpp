#include "..\\include\Lib_Clara.h"

char* strchr(char* str,char c)
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

char* strrchr(char* str,char c)
{
	char* retpos=NULL;
	for(;*str;str++)if(*str==c)retpos=str;
	return retpos;
}

//u16* wstrrchr(u16* str,u16 c) //declared in lib

char* strncpy(char* dest,char* src,size_t len)
{
	for(; *src && len; len--)*dest++=*src++;
	if(len)*dest=0;
	return dest;
}

u16* wstrncpy(u16* dest,u16* src,size_t len)
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

int wstrncmp(u16* s1,u16* s2,size_t len)
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

int strncmpi(char* s1,char* s2,size_t len)
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
u16* wstrstr (const u16 * str1, const u16 * str2)
{
	u16 *cp = (u16 *) str1;
	u16 *s1, *s2;
	if ( !*str2 )
		return((u16 *)str1);
	while (*cp)
	{
		s1 = cp;
		s2 = (u16 *) str2;
		while ( *s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;
		if (!*s2)
			return(cp);
		cp++;
	}
	return(NULL);
}
