#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <stdlib.h>
#include <string.h>
#include <iostream>

template <unsigned maxSizeInBytes>
class DataStream
{
public:
	DataStream()
	{
		unsigned t = maxSizeInBytes;
		while( t-- )
			m_bytes[t] = 0;
		m_streamend = 0;
		//ctor
	};
	virtual ~DataStream()
	{

	};
	template <typename T> DataStream<maxSizeInBytes> operator<<( const T& t )
	{
		//std::cout << t << std::endl;
		if( m_streamend + sizeof( T ) > maxSizeInBytes )
		{
			m_lastoperation = false;
			return *this;
		}
		memcpy( ( void* )m_bytes + m_streamend, ( void* )&t, sizeof( t ) );
		m_streamend += sizeof( T );
		m_lastoperation = true;
		return *this;

	};
	template <typename T> DataStream<maxSizeInBytes> operator>>( T& t )
	{
		if( m_streamend - sizeof( T ) < 0 )
		{
			m_lastoperation = false;
			return *this;
		}
		memcpy( ( void* )&t, ( void* )( m_bytes + m_streamend - sizeof( T ) ), sizeof( T ) );
		m_streamend -= sizeof( T );
		memset( m_bytes + m_streamend, 0, sizeof( T ) );
		m_lastoperation = true;
		return *this;

	};
	void display()
	{
		for( int i = 0; i < maxSizeInBytes; i++ )
			std::cout << ( int )m_bytes[i] << std::endl;
	}
	operator bool()
	{
		return m_lastoperation;
	}
protected:
private:
	typedef signed char OneByte;
	OneByte m_bytes[maxSizeInBytes];
	unsigned m_streamend;
	bool m_lastoperation;
};
typedef DataStream<64> StandardStream;


#endif // DATASTREAM_H
