char* strchr(char* str,char c);
u16*  wstrchr(u16* str,u16 c);
char* strrchr(char* str,char c);
//u16* wstrrchr(u16* str,u16 c); //declared in lib
char* strncpy(char* dest,char* src,size_t len);
u16*  wstrncpy(u16* dest,u16* src,size_t len);
int   strncmp(char* s1,char* s2,size_t len);
int   wstrncmp(u16* s1,u16* s2,size_t len);
char  upper(char c);
int   strncmpi(char* s1,char* s2,size_t len);
char* strstr (const char * str1, const char * str2);
u16*  wstrstr (const u16 * str1, const u16 * str2);
