#pragma once

#include <pro.h>
#include <set>
#include <map>

typedef std::pair<ea_t,ea_t> MAPPAIR;

typedef std::set<MAPPAIR> SET;
typedef std::set<MAPPAIR>::iterator SETI;

class PtrList
{
public:
        SET firstoffs;//
        SET firstaddr;

        PtrList();

        void insert(ea_t addr,ea_t offs);

        SETI find(ea_t addr,ea_t offs);

        void remove(ea_t addr);
};
