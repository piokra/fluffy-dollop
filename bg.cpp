#ifndef BG_CPP
#define BG_CPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "bg.h"
#include "macro.h"

template <class T> Point<T>::Point()
{
	m_x = 0;
	m_y = 0;
}

template <class T> Point<T>::Point( T x, T y )
{
	m_x = x;
	m_y = y;
}

template <class T> Point<T>::~Point() { }



template <class T> Vector2<T>::Vector2() { };

template <class T> Vector2<T>::Vector2( T x, T y )
{
	m_p = Point<T>( x, y );
};

template <class T> Vector2<T>::Vector2( Point<T> p )
{
	m_p = p;
};

template <class T> Vector2<T>::~Vector2() { };

template <class T> Vector2<T>::Vector2( Point<T> a, Point<T> b )
{
	m_p = Point<T>( b.m_x - a.m_x, b.m_y - a.m_y );
}

template <class T> T dotProduct( Vector2<T> u, Vector2<T> v )
{
	return u.m_p.m_x * v.m_p.m_x + u.m_p.m_y * v.m_p.m_y;
}

template <class T> Rect<T>::Rect() { };

template <class T> Rect<T>::Rect( Point<T> a, Point<T> b )
{
	T x1 = MAX( a.m_x, b.m_x );
	T y1 = MAX( a.m_y, b.m_y );
	T x2 = MIN( a.m_x, b.m_x );
	T y2 = MIN( a.m_y, b.m_y );

	/*T x1 = a.m_x;
	T y1 = a.m_y;
	T x2 = b.m_x;
	T y2 = b.m_y;*/

	Point<T> t1 = Point<T>( x1, y1 );
	Point<T> t2 = Point<T>( x2, y2 );

	m_tr = t1;
	m_bl = t2;

};

template <class T> Rect<T>::~Rect() { };

template <class T> Point<T> Rect<T>::getCorner( Corner corner )
{

	if ( corner == TOPRIGHT )
		return m_tr;
	if ( corner == BOTLEFT )
		return m_bl;
	if ( corner == BOTRIGHT )
		return Point<T>( m_tr.m_x, m_bl.m_y );
	if ( corner == TOPLEFT )
		return Point<T>( m_bl.m_x, m_tr.m_y );
	return Point<T>();
}

template <class T> sf::Int8 Rect<T>::getCollision( Point<T> p )
{

	return
		(
			p.m_x <= m_tr.m_x &&
			p.m_y <= m_tr.m_y &&
			p.m_x >= m_bl.m_x &&
			p.m_y >= m_bl.m_y

		);
}

template <class T> sf::Int8 Rect<T>::getCollision( Rect<T> rect )
{

	Point<T> bl = rect.getCorner( BOTLEFT );
	Point<T> tr = rect.getCorner( TOPRIGHT );






	return
		(

			(
				bl.m_x < m_tr.m_x &&
				bl.m_y < m_tr.m_y &&
				tr.m_x >= m_bl.m_x &&
				tr.m_y >= m_bl.m_y

			)
			||
			(

				bl.m_x >= m_tr.m_x &&
				bl.m_y >= m_tr.m_y &&
				tr.m_x < m_bl.m_x &&
				tr.m_y < m_bl.m_y



			)

		);
}






#endif // BG_CPP

