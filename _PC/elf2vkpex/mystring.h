#pragma once

#include <string>
#include <Windows.h>

typedef std::basic_string<char> oemstring;
typedef std::basic_string<wchar_t> ustring;

const oemstring& tooemstring(const oemstring& s, int cp=CP_OEMCP);
const oemstring tooemstring(const ustring& s, int cp=CP_OEMCP);
const ustring toustring(const oemstring& s, int cp=CP_OEMCP);
const ustring& toustring(const ustring& s, int cp=CP_OEMCP);

#ifdef _UNICODE
typedef ustring cstring;
#define tocstring toustring
#else
typedef oemstring cstring;
#define tocstring tooemstring
#endif

cstring fmt(const LPCTSTR fstr,...);
