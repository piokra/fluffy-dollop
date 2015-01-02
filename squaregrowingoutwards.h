#ifndef SQUAREGROWINGOUTWARDS_H
#define SQUAREGROWINGOUTWARDS_H

#include <iostream>

template <class T>
class SquareGrowingOutwards
{
public:
	SquareGrowingOutwards() : m_defi( -1 ), m_deft( 0 )
	{

		m_NEx = -1;
		m_NEy = -1;

		m_SEx = -1;
		m_SEy = 0;

		m_NWx = 0;
		m_NWy = -1;

		m_SWx = 0;
		m_SWy = 0;

	};
	const int& getNEx()
	{
		return m_NE.size();
	}
	const int& getNEy()
	{
		if( m_NE.size() ) return m_NE.at( 0 ).size();
		return m_defi;
	}
	const int& getSEx()
	{
		return m_SE.size();
	}
	const int& getSEy()
	{
		if( m_SE.size() ) return m_SE.at( 0 ).size();
		return m_defi;
	}
	const int& getNWx()
	{
		return m_NW.size();
	}
	const int& getNWy()
	{
		if( m_NW.size() ) return m_NW.at( 0 ).size();
		return m_defi;
	}
	const int& getSWx()
	{
		return m_SW.size();
	}
	const int& getSWy()
	{
		if( m_SW.size() ) return m_SW.at( 0 ).size();
		return m_defi;
	}
	T& getXY( int x, int y )
	{
		if( x >= 0 )
		{
			//SE or NE
			if( y >= 0 )
			{
				//NE
				if( getNEx() > x && getNEy() > y )
					return m_NE[x][y];
			}
			else
			{
				//SE
				if( getSEx() > x && getSEy() >= -y )
					return m_SE[x][-y - 1];

			}
		}
		else
		{
			//SW or NW
			if( y >= 0 )
			{
				//NW
				if( getNWx() >= -x && getNWy() > y )
					return m_NW[-x - 1][y];
			}
			else
			{
				//SW
				if( getSWx() >= -x && getSWy() >= -y )
					return m_SW[-x - 1][-y - 1];

			}
		}
		return m_deft;
	};
	void setXY( T obj, int x, int y )
	{
		std::cout << x << " " << y << std::endl;
		if( x >= 0 )
		{
			//SE or NE
			if( y >= 0 )
			{
				//NE
				extendByXY( m_NE, x - getNEx() + 1, y - getNEy() + 1 );
				m_NE[x][y] = obj;


			}
			else
			{
				//SE

				extendByXY( m_SE, x - getSEx() + 1, -getSEy() - y );
				m_SE[x][-1 - y] = obj;

			}
		}
		else
		{
			//SW or NW
			if( y >= 0 )
			{
				//NW

				extendByXY( m_NW, -getNWx() - x, y - getNWy() + 1 );

				m_NW[-1 - x][y] = obj;
			}
			else
			{
				//SW
				extendByXY( m_SW, -getSWx() - x, -getSWy() - y );
				m_SW[-1 - x][-1 - y] = obj;

			}
		}

	};
	friend std::ostream& operator<<( std::ostream& out, const SquareGrowingOutwards& s )
	{
		for( unsigned i = 0; i < s.m_NE.size(); i++ )
		{
			for( unsigned j = 0; j < s.m_NE.at( i ).size(); j++ )
			{
				out << s.m_NE.at( i ).at( j );
			}
			out << std::endl;
		}
		out << std::endl;
		out << std::endl;
		for( unsigned i = 0; i < s.m_SE.size(); i++ )
		{
			for( unsigned j = 0; j < s.m_SE.at( i ).size(); j++ )
			{
				out << s.m_SE.at( i ).at( j );
			}
			out << std::endl;
		}
		out << std::endl;
		out << std::endl;
		for( unsigned i = 0; i < s.m_NW.size(); i++ )
		{
			for( unsigned j = 0; j < s.m_NW.at( i ).size(); j++ )
			{
				out << s.m_NW.at( i ).at( j );
			}
			out << std::endl;
		}
		out << std::endl;
		out << std::endl;
		for( unsigned i = 0; i < s.m_SW.size(); i++ )
		{
			for( unsigned j = 0; j < s.m_SW.at( i ).size(); j++ )
			{
				out << s.m_SW.at( i ).at( j );
			}
			out << std::endl;
		}
		out << std::endl;
		out << std::endl;

		return out;
	}


protected:
	void extendByXY( std::vector<std::vector<T>>& vec, int x, int y )
	{
		std::cout << "Ytx:" << y << std::endl;
		int newy = y;
		int maxx;
		int i;
		for( i = 0; i < vec.size(); i++ )
		{

			newy = vec.at( i ).size() + y;
			for( int j = 0; j < y; j++ )
				vec.at( i ).push_back( T() );

		}
		std::cout << "Ytx:" << y << std::endl;

		maxx = i;
		for( i = 0; i < x; i++ )
		{
			vec.push_back( std::vector<T>() );
			for( int j = 0; j < newy; j++ )
				vec.at( i + maxx ).push_back( T() );

		}
	}
private:
	std::vector<std::vector<T> > m_NE;
	std::vector<std::vector<T> > m_NW;
	std::vector<std::vector<T> > m_SE;
	std::vector<std::vector<T> > m_SW;

	int m_NEx;
	int m_NEy;

	int m_NWx;
	int m_NWy;

	int m_SEx;
	int m_SEy;

	int m_SWx;
	int m_SWy;

	T m_deft;
	int m_defi;
};

#endif // SQUAREGROWINGOUTWARDS_H
