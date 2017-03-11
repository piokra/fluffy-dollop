#ifndef BG_H
#define BG_H

#include <iostream>
#include <SFML/Graphics.hpp>

template <class T> class Vector2;

template <class T>
class Point
{
public:
	Point();
	Point( T x, T y );
	~Point();

	operator Point<float>()
	{
		return Point<float>( ( float )m_x, ( float )m_y );
	};

	operator Point<int>()
	{
		return Point<int>( ( int )m_x, ( int )m_y );
	};

	void display()
	{
		std::cout << "point x|y: " << m_x << " " << m_y << "\n";
	}
	Point<T> operator + ( Point<T> a )
	{
		return Point<T>( a.m_x + m_x, a.m_y + m_y );
	};
	Point<T> operator + ( Vector2<T> a )
	{
		return Point<T>( *this + a );
	};
	Point<T> operator * ( T a )
	{
		return Point<T>( m_x * a, m_y * a );
	};
	bool operator == ( Point<T> a )
	{
		return ( ( a.m_x == m_x ) && ( a.m_y == m_y ) );
	}

	T m_x;
	T m_y;
};



template <class T>
class Vector2
{
public:
	Vector2();
	Vector2( Point<T>, Point<T> );
	Vector2( T x, T y );
	Vector2( Point<T> p );
	~Vector2();

	Point<T> m_p;

	Vector2<T> operator + ( Vector2<T> v )
	{
		return Vector2<T>( m_p + v.m_p );
	};
	Vector2<T> operator + ( Point<T> p )
	{
		return Vector2<T>( m_p + p );
	};
	Vector2<T> operator * ( T a )
	{
		return Vector2<T>( m_p * a );
	};
	operator Vector2<float>()
	{
		return Vector2<float>( ( Point<float> )m_p );
	}

protected:
private:



};

template <class T> T dotProduct( Vector2<T> u, Vector2<T> v );


enum Corner
{
	TOPLEFT = 0,
	TOPRIGHT = 1,
	BOTRIGHT = 2,
	BOTLEFT = 3
};


template <class T>
class Rect
{
public:
	friend class Rect<T>;
	Rect();
	Rect( Point<T> a, Point<T> b );
	~Rect();

	Point<T> getCorner( Corner corner );


	sf::Int8 getCollision( Point<T> p );
	sf::Int8 getCollision( Rect<T> rect );


	void display()
	{
		std::cout << "Rect 4\n";
		m_tr.display();
		m_bl.display();
	};
	//sf::Int8 getCollision(Rect<T> rect, Point<T> t1, Point<T> t2);


	Rect<T> operator +( Point<T> a )
	{
		return Rect<T>( m_tr + a,
						m_bl + a );

	}
	Rect<T> operator *( T r )
	{
		return Rect<T>( m_tr * r, m_bl * r );
	}
	bool operator == ( Rect<T> tc )
	{
		return ( m_tr == tc.m_tr ) && ( m_bl == tc.m_bl );
	};



protected:

private:
	Point<T> m_tr;
	Point<T> m_bl;

};
template <class T>
class ParamLine
{
public:
	ParamLine()
	{

	};
	ParamLine( Point<T> start, Point<T> end )
	{
		m_vector = Vector2<T>( start, end );
		m_orgin = start;

	};
	ParamLine( Point<T> orgin, Vector2<T> vec )
	{
		m_orgin = orgin;
		m_vector = vec;
	};

	void setOrgin( Point<T> p )
	{
		return m_vector;
	};


	float getIntersection( ParamLine<T> line )
	{

		/*
		    { t1*v1x-t2*v2x = o2x-o1x
		    { t1*v1y-t2*v2y = o2y-o1y

		      | v1x -v2x |
		    W=|          |  = v2x*v1y - v1x*v2y
		      | v1y -v2y |

		          | o2x-o1x  -v2x |
		    Wt1 = |               | = v2x(o2y-o1y) - v2y(o2x-o1x)
		          | o2y-o1y  -v2y |

		          | v1x  o2x-o1x |
		    Wt2 = |              | = v1x(o2y-o1y) - v1y(o2x-o1x)
		          | v1y  o2y-o1y |

		    t1 = Wt1/W

		    t2 = Wt2/W

		*/
		float v1x = ( float )m_vector.m_p.m_x;
		float v1y = ( float )m_vector.m_p.m_y;
		float o1x = ( float )m_orgin.m_x;
		float o1y = ( float )m_orgin.m_y;



		float v2x = line.m_vector.m_p.m_x;
		float v2y = line.m_vector.m_p.m_y;
		float o2x = line.m_orgin.m_x;
		float o2y = line.m_orgin.m_y;

		float W = v2x * v1y - v1x * v2y;
		if ( W > -0.01 && W < 0.01 )
		{
			return -1;
		}
		float Wt2 = v1x * ( o2y - o1y ) - v1y * ( o2x - o1x );
		float Wt1 = v2x * ( o2y - o1y ) - v2y * ( o2x - o1x );
		float t1 = Wt1 / W;
		float t2 = Wt2 / W;

		if( t2 >= 0 && t2 <= 1 ) return t1;
		return -1;


	}
	Point<T> getOrgin()
	{
		return m_orgin;
	};
	Vector2<T> getVector()
	{
		return m_vector;
	};

private:
	Point<T> m_orgin;
	Vector2<T> m_vector;

};

#include "bg.cpp"






#endif // BG_H
