#include "misc_util.h"

int strncmpi(const char* s1, const char* s2, size_t len)
{
    char c1, c2;
    for(; *s1 && *s2 && len; len--, s1++, s2++)
    {
        c1=toupper(*s1);
        c2=toupper(*s2);
        if(c1-c2)return c1-c2;
    }
    if(len)return c1-c2;
    return 0;
}
