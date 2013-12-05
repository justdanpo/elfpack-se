#include "vkp.h"

static unsigned hextoint(const char* str)
{
    unsigned retval = 0;
    for(;*str;str++)
    {
        retval <<= 4;
        if( *str > '9' )
            retval |= (*str | 0x20)-'a'+10;
        else
            retval |= *str - '0';
    }
    return retval;
}
//

Vkp::Vkp()
{
    addrs = List_Create();
    values = List_Create();
}

Vkp::~Vkp()
{
    List_Destroy(addrs);
    List_Destroy(values);
}

int cmpaddr(void *r0,void *r1)
{
    return ! (r0 == r1);
}

void Vkp::setbyte( unsigned addr, unsigned char data )
{
    unsigned oldvalue = 0;
    int nPos = List_Find(addrs, (void*) (addr/4), cmpaddr);
    if( nPos != LIST_ERROR )
    {
        oldvalue = (unsigned) List_Get( values, nPos );
        List_RemoveAt( addrs, nPos );
        List_RemoveAt( values, nPos );
    }
    int shiftlen = (addr & 3) << 3;
    oldvalue &= ~ (0xFF << shiftlen );
    oldvalue |= data << shiftlen;
    List_InsertLast( addrs, (void*)(addr/4) );
    List_InsertLast( values, (void*)oldvalue );
}

int Vkp::gettable( void** ret )
{
    int count = 0;
    for( int i = 0; i < addrs->FirstFree; i++ )
    {
        unsigned addr = (unsigned)List_Get( addrs, i );
        if(addr>=0x100 && addr<0x8000)
        {
            count ++;
            if( ret )
            {    
                *ret++ = (void*)addr;
                *ret++ = List_Get( values, i );
            }
        }
    }
    return count;
}


char chhexvalues[]="0123456789ABCDEFabcdef";
char chspace[]="\x20\x09";//space,tab

bool Vkp::isthatchar(const char* addr,const long size,const long position,long& elementsize,const char* ch)
{
    if(position>=size)
        return false;
    
    
    for(const char* str=ch; *str; str++)
        if(*str==addr[position])
        {
            elementsize = 1;
            return true;
        }
    
    return false;
}

bool Vkp::space(const char* addr,long size,long position,long& elementsize)
{
    return isthatchar(addr,size,position,elementsize,chspace);
}

bool Vkp::eol(const char* addr,long size,long position,long& elementsize)
{
    elementsize=0;
    bool retcode=false;
    
    if(position>=size)
        return true;
    
    if(addr[elementsize+position]==13)
    { 
        elementsize++; 
        retcode=true; 
    }
    
    if(position>=size)
        return retcode;
    
    if(addr[elementsize+position]==10)
    { 
        elementsize++; 
        retcode=true; 
    }
    return retcode;
}

bool Vkp::eos(const char* addr,long size,long position,long& elementsize)
{
    long t;
    
    for(elementsize=0 ; space(addr,size,elementsize+position,t) ; elementsize+=t);
    
    if(elementsize+position>=size)
        return true;
    
    if(addr[elementsize+position]==';')
    {
        while(! eol(addr,size,elementsize+position,t))
            elementsize++;
    }
    
    if(eol(addr,size,elementsize+position,t))
    { 
        elementsize+=t; 
        return true; 
    }
    return false;
}

bool Vkp::anyline(const char* addr,long size,long position,long& elementsize)
{
    elementsize=0;
    long t;
    while(! eol(addr,size,elementsize+position,t)) 
        elementsize++;
    //    if(eol(addr,size,elementsize+position,t)){ elementsize+=t; return true; }
    //    return false;
    return true;
}

bool Vkp::deltaoffset(const char* addr,long size,long position,long& elementsize)
{
    if(!isthatchar(addr,size,position,elementsize,"+-"))
        return false;
    
    long t;
    int i;
    //char tmp[10]; 
    //tmp[0]=addr[position];
    for(i=0;i<8;i++)
    {
        if(!isthatchar(addr,size,elementsize+position,t,chhexvalues))
            break;
        //tmp[i+1]=addr[elementsize+position];
        elementsize+=t;
    }
    
    if(i==0)
        return false;
    
    //tmp[i+1]=0;
    if(!eos(addr,size,elementsize+position,t))
        return false;
    
    elementsize+=t;
    //sscanf_s(tmp, "%x", &t, sizeof(t));
    //delta=t;
    return true;
}

bool Vkp::patchstring(const char* addr,long size,long position,long& elementsize)
{
    elementsize=0;
    int i;
    long t;
    char tmpaddr[9];
    unsigned addrvalue;
    
    char tmpval[3]; tmpval[2]=0;
    
    for(i=0;i<8;i++)
    {
        if(!isthatchar(addr,size,elementsize+position,t,chhexvalues))
            break;
        
        tmpaddr[i]=addr[elementsize+position];
        elementsize+=t;
    }
    
    if(i==0)
        return false;
    
    tmpaddr[i]=0; 
    addrvalue = hextoint( tmpaddr );
    
    if(!isthatchar(addr,size,elementsize+position,t,":"))
        return false; 
    
    elementsize+=t;
    
    if(!isthatchar(addr,size,elementsize+position,t," "))
        return false; 
    
    elementsize+=t;
    int hcount=0;
    while(isthatchar(addr,size,elementsize+position,t,chhexvalues))
    {
        tmpval[0]=addr[elementsize+position];
        elementsize+=t;
        
        if(!isthatchar(addr,size,elementsize+position,t,chhexvalues))
            return false;
        
        tmpval[1]=addr[elementsize+position];
        elementsize+=t;
        
        //оригинальные данные не нужны
        //t = hextoint( tmpval );
        //VKPLINE l;
        //l.addr=addrvalue+hcount+delta;
        //l.data[0]=(BYTE)t;
        //vkpline.push_back(l);
        
        hcount++;
    }
    
    if(!isthatchar(addr,size,elementsize+position,t," "))
        return false; 
    
    elementsize+=t;
    
    for(int i=0;i<hcount;i++)
    {
        if(!isthatchar(addr,size,elementsize+position,t,chhexvalues))
            return false;
        
        tmpval[0]=addr[elementsize+position];
        elementsize+=t;
        
        if(!isthatchar(addr,size,elementsize+position,t,chhexvalues))
            return false;
        
        tmpval[1]=addr[elementsize+position];
        elementsize+=t;
        
        setbyte( addrvalue + i, hextoint( tmpval ) );
    }
    
    if(!eos(addr,size,elementsize+position,t))
        return false;
    
    elementsize+=t;
    
    return true;
}

int Vkp::dovkp(const char* addr,long size)
{
    //patchtmp.clear();
    //patch.clear();
    //errorstring="";
    errorline=0;
    //delta=0;
    
    long position;
    int linenum;
    long elementsize;
    for(position=0, linenum=1; position<size; position+=elementsize, linenum++)
    {
        if(eos(addr,size,position,elementsize)
           ||deltaoffset(addr,size,position,elementsize)
               ||patchstring(addr,size,position,elementsize))
            continue;
        anyline(addr,size,position,elementsize);
        //errorstring=std::string(addr+position,elementsize);
        errorline=linenum;
        return linenum;
    }
    return 0;
}
