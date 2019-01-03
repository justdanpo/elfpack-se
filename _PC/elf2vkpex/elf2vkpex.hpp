#pragma once

#include <set>
#include <vector>
#include <tchar.h>
#include "mystring.h"

struct CXCHDR
{
	uint8_t unk[0x20];
	uint16_t ver1;
	uint16_t ver2;
	uint8_t hash_hashtable[0x14];
	uint32_t hashtable_offset;//raw?       //update
	uint32_t hashtable_len;				//update
	uint32_t hashbaseaddr;
	uint32_t cxcbodyoffset;//1000
	uint32_t cxclen;						//update
	uint32_t baseaddr;
	uint8_t hash_cxc[0x14];
	uint32_t cxclenforhash;				//update
};

union CXC1000
{
	CXCHDR hdr;
	uint8_t padding[0x1000];
};


struct PTE_BUFF
{
	uint32_t addr;
	std::vector<uint8_t> buff;
};

bool operator <(const PTE_BUFF &p1, const PTE_BUFF &p2)
{
	return p1.addr < p2.addr;
}

typedef std::set<PTE_BUFF> pteset;
typedef std::set<PTE_BUFF>::iterator pteset_i;
typedef std::set<PTE_BUFF>::const_iterator pteset_ci;

enum PATCHFILETYPE
{
	PFT_NONE,
	PFT_PHONE_APP,
	PFT_PHONE_EMP_APP,
	PFT_PHONE_ACC
};

char* patchfilenames[]=
{
	"",
	"/boot/phone_app.cxc",
	"/boot/phone_emp_app.cxc",
	"/boot/phone_acc.cxc"
};

TCHAR* patchfilenamesshort[]=
{
	_T(""),
	_T("app"),
	_T("emp_app"),
	_T("acc")
};

class FWSECTIONINFO
{
public:
	uint32_t addr;
	uint32_t size;
	uint32_t fileoffset;
	ustring filename;
	PATCHFILETYPE patchfile;
	int filenum;

	FWSECTIONINFO() : addr(0), size(0), fileoffset(0), patchfile(PFT_NONE), filenum(0)
	{
	}
};

bool operator <(const FWSECTIONINFO &p1, const FWSECTIONINFO &p2)
{
	if( p1.addr < p2.addr )
		return true;
/*
	if( p1.addr == p2.addr )
		if( p1.size < p2.size )
			return true;
*/
	return false;

}

typedef std::set<FWSECTIONINFO> fwiset;
typedef std::set<FWSECTIONINFO>::iterator fwiset_i;
typedef std::set<FWSECTIONINFO>::const_iterator fwiset_ci;
