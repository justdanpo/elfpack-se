#include "ptrlist.h"

bool operator <(MAPPAIR a1,MAPPAIR a2)
{
        if(a1.first<a2.first)return true;
        if(a1.first==a2.first)return a1.second<a2.second;
        return false;
}

PtrList::PtrList()
{
}

void PtrList::insert(ea_t addr,ea_t offs)
{
	firstoffs.insert( std::make_pair(offs,addr) );
	firstaddr.insert( std::make_pair(addr,offs) );
}

SETI PtrList::find(ea_t addr,ea_t offs)
{
	if(offs)
	{//firstoffs...   f->first=offset, f->second=addr
		SETI f=firstoffs.lower_bound(std::make_pair(offs,addr));
		if(f==firstoffs.end())          return firstoffs.end();
		if(offs!=f->first)                      return firstoffs.end();
		if(addr && addr!=f->second)     return firstoffs.end();
		return f;
	}
	//firstaddr...   f->first=addr, f->second=offset
	SETI f=firstaddr.lower_bound(std::make_pair(addr,0));
	if(f==firstaddr.end())                  return firstaddr.end();
	if(addr!=f->first)                              return firstaddr.end();
	return f;
}

void PtrList::remove(ea_t addr)
{
	SETI f=find(addr,0);
	SETI f2=find(addr,f->second);
	if(f!=firstaddr.end() && f2!=firstoffs.end())
	{
		firstaddr.erase(f);
		firstoffs.erase(f2);
	}
}
