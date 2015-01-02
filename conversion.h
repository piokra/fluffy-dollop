#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdlib.h>

#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

template <class T> std::string tostr( T val )
{
	std::ostringstream ss;
	ss << val;
	return ss.str();
}
int strtoint( char* str )
{
	ui len = strlen( str );
	ui t = 1;
	int res = 0;
	for( ui i = len - 1; i != -1; i-- )
	{
		res += ( str[i] - 48 ) * t;
		t *= 10;
	}

	return res;
}

int strtoint( std::string str )
{
	return strtoint( str.c_str() );
}



#endif // CONVERSION_H

