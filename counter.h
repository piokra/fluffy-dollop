
#ifndef COUNTER_H
#define COUNTER_H

#include "typedefs.h"


template <class T>
class Counter
{
public:
	Counter()
	{
		++m_count;
	};
	Counter( const Counter& cunt )
	{
		++m_count;
	}

	static ui getCount()
	{
		return m_count;
	}

protected:
	~Counter()
	{
		m_count--;
	}
private:
	static ui m_count;
};

template <class T> ui Counter<T>::m_count( 0 );



#endif // COUNTER_H
