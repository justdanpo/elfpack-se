// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS TRUE
#define _CRT_NON_CONFORMING_SWPRINTFS TRUE

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <string>


typedef std::basic_string<TCHAR> tcstring;

