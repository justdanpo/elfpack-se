#include "strutils.hpp"
#include <vector>

const string tostring( TEXTID strid )
{
	vector<char> tmp;
	tmp.resize( TextID_GetLength( strid ) + 13 );
	TextID_GetString( strid, &tmp[0], tmp.size() );
	return &tmp[0];
}

TEXTID tostrid( const string& str )
{
	return TextID_Create( str.c_str(), ENC_LAT1, str.size() );
}
