#include <memory>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include "mystring.h"

const oemstring& tooemstring(const oemstring& s, int cp)
{
	return s;
}

const oemstring tooemstring(const ustring& s, int cp)
{
	std::vector<char> buf;
	buf.resize( s.size()+1 );

	WideCharToMultiByte(cp, 0, s.data(), static_cast<int>( s.size() ), &buf[0], static_cast<int>( s.size() ), 0, NULL);

	return oemstring(&buf[0], s.size());
}

const ustring toustring(const oemstring& s, int cp)
{
	std::vector<wchar_t> buf;
	buf.resize( s.size()+1 );

	MultiByteToWideChar(cp, 0, s.c_str(), static_cast<int>( s.size() ), &buf[0], static_cast<int>( s.size() ) );

	return ustring(&buf[0], s.size());
}

const ustring& toustring(const ustring& s, int cp)
{
	return s;
}


cstring fmt(const LPCTSTR fstr,...)
{
	va_list args;
	va_start( args, fstr );
	int len = _vsctprintf(fstr, args);
	TCHAR* buf = new TCHAR[ len+1 ];
	_vsntprintf_s( buf, len+1, _TRUNCATE, fstr, args ); 
	cstring tmp(buf);
	delete[] buf;
	return tmp;
}
