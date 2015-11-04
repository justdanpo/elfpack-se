#ifndef _IREGISTRY_HPP_
#define _IREGISTRY_HPP_

#include "..\include\lib_clara.h"
#include "IUnknown.hpp"

static UUID IID_ISEMCRegistryManager={0x42, 0x71, 0xC7, 0x45, 0x0F, 0x21, 0x46, 0xB7, 0x9A, 0x29, 0x99, 0xF4, 0x21, 0x6A, 0x26, 0x2E};
static UUID CID_CSEMCRegistryManager={0x84, 0xA2, 0x98, 0x5A, 0x91, 0x3E, 0x4B, 0x7E, 0xBE, 0xB3, 0x7E, 0xFC, 0xFC, 0xF0, 0xB0, 0xEB};


class IRegistry : public IUnknown
{
};

class IRegistryAccess : public IUnknown
{
public:
	virtual int ReadUnsignedInt(char const* absoluteValuePath, unsigned* ret);
	virtual int ReadString(char const* absoluteValuePath, unsigned maxLength, char* stringBuffer, unsigned* pActualLength);
	virtual int ReadWString(char const* absoluteValuePath, unsigned maxLength, wchar_t* stringBuffer, unsigned* pActualLength);
	virtual int ReadBinary(char const* absoluteValuePath, unsigned dataSize, void* pData, unsigned* pActualSize);
	virtual int WriteUnsignedInt(char const* absoluteValuePath, unsigned value);
	virtual int WriteString(char const* absoluteValuePath, char* strVal);
	virtual int WriteWString(char const* absoluteValuePath, wchar_t* strVal);
	virtual int WriteBinary(char const* absoluteValuePath, void const* pData, unsigned dataSize);
};

class ISEMCRegistryManager : public IUnknown
{
public:
	virtual int CreateRegistry(IRegistry** ppIRegistry);
	virtual int CreateRegistryAccess(IRegistryAccess** ppIRegistryAccess);
	virtual int CreateRegistryNotificationsProvider(void** ppIRegistryNotificationsProvider);
};

#endif
