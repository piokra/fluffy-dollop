#ifndef WORLD_H
#define WORLD_H

#include "globals.h"
#include "counter.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "bg.h"
#include "normals.h"




extern int mx;
extern int my;

template <class T> T sign( T a )
{
	return ( ( a > 0 ) ? 1 : ( a < 0 ) ? -1 : 0 );
}


extern int xoffset;
extern int yoffset;

class Block;
class World;
class Object;
class ObjectHolder;
class ObjectVector;
class BlockHolder;
class Game;
class MovingWorld;

enum Phase
{
	NONE,
	GAS,
	LIQUID,
	SOLID
};




class Block : public Counter<Block>
{
public:
	Block( int x, int y, BlockHolder* parent );
	~Block();
	virtual errc draw( sf::RenderWindow* rw, int, int, int );
	virtual errc process();
	Phase getPhase()
	{
		return m_phase;
	}
	Point<double> getPos()
	{
		return Point<double>( m_x, m_y );
	}
	virtual Rect<double> getCbox()
	{

		return Rect<double>( Point<double>( m_x * Globals::BLOCKSIZE, m_y * Globals::BLOCKSIZE ), Point<double>( m_x * Globals::BLOCKSIZE + Globals::BLOCKSIZE,
							 m_y * Globals::BLOCKSIZE + Globals::BLOCKSIZE ) );
	}
	void setPhase( Phase phase )
	{
		m_phase = phase;
	}
	BlockHolder* getParent()
	{
		return m_parent;
	};
	normals getAvailNormals();
	float gettw();
	float settw( float ntw );
	Phase m_phase;
	virtual Vector2<double> getVelocity()
	{
		return Vector2<double>( 0, 0 );
	};
	virtual void addVelocity( Vector2<double> ) { };
	virtual double getInvMass()
	{
		return 0;
	};
protected:
private:
	sf::Color m_color;
	int m_x;
	int m_y;
	float tw;
	BlockHolder* m_parent;


};




class ObjectHolder
{
public:
	ObjectHolder( Game* parent )
	{
		m_parent = parent;
	};
	~ObjectHolder()
	{

	};
	virtual const std::vector<Object*>* objectsInRange( Point<double> pos, double r )
	{
		return NULL;
	}; // use sparingly
	virtual errc process() { };
	virtual errc draw( sf::RenderWindow* ) { };
	virtual errc add( Object* ) { };

protected:
private:
	Game* m_parent;
};




class BlockHolder
{
public:
	BlockHolder()
	{

	};
	virtual ~BlockHolder()
	{

	};
	virtual Block* getBlock( int x, int y )
	{
		printf( "Pure fagvirutal function called\n" );
		return NULL;
	};
	virtual Block** getNeighbours( int x, int y, Block* buffer[4] )
	{
		printf( "Pure virutal function called\n" );
		return NULL;
	};
	virtual std::vector<Block*>* getCollision( Object* obj )
	{
		return NULL;
	};
	virtual Game* getParent()
	{

	};
	virtual std::vector<Block*>* getCollision( Rect<double> rect )
	{

	};

};


class World : public BlockHolder
{
public:
	World( Game* parent, ui x, ui y );
	~World();
	virtual Block* getBlock( int x, int y );
	virtual Block** getNeighbours( int x, int y, Block* buffer[4] );
	errc draw( sf::RenderWindow* rw, int, int, int, int, int );
	errc process( int, int, int, int );
	std::vector<Block*>* getCollision( Object* obj );
	Game* getParent()
	{
		return m_parent;
	}
	virtual std::vector<Block*>* getCollision( Rect<double> rect );
	virtual std::vector<Block*>* getCollision( std::vector<Block*>* t_bl, Rect<double> rect );

protected:
	std::vector<Block*>* getMtbl();
private:
	Game* m_parent;
	std::vector<Block*>* m_t_bl;
	Block*** m_blocks;
	int m_x;
	int m_y;
};

class Object : public Counter<Object>
{
public:
	friend class Object;
	friend class Hero;
	Object( int x, int y, Game* parent, double mass, Rect<double> bounds )
	{
		m_acceleration = Vector2<double>( 0, 0 );
		m_velocity = Vector2<double>( 0, 0 );
		m_p = Point<double>( x, y );
		m_parent = parent;
		m_cbox = bounds;
		m_mass = mass;
		m_invmass = 1 / mass;


	};
	virtual ~Object()
	{

	};
	virtual errc draw( sf::RenderWindow* window );

	virtual errc process();

	Point<double> getPos()
	{
		return m_p;
	};
	void setPos( Point<double> a )
	{
		m_p = a;
	};
	Rect<double> getCbox()
	{
		return m_cbox + m_p;
	}
	virtual int isColliding( Object* obj )
	{
		return getCbox().getCollision( obj->getCbox() );
	}
	virtual int isColliding( Block* block );

	virtual void addForce( Vector2<double> force )
	{
		m_force = m_force + force;
	}

	virtual Vector2<double> getVelocity()
	{
		return m_velocity;
	}
	virtual Point<double> getPreviousPos()
	{
		return m_pp;
	}
	virtual void addVelocity( Vector2<double> vel )
	{
		m_velocity = m_velocity + vel;
	}
	int inrange;

protected:
	virtual bool resolveCollision();
	virtual void countAcceleration()
	{
		m_acceleration = m_force * m_invmass;
	};
	virtual void addAcceleration();
	virtual void processPhysics()
	{
		addForce( Vector2<double>( 0, 1 * m_mass ) );
		Vector2<double> tvel = getVelocity();
		tvel.m_p.m_x = -( 0.01 ) * ( tvel.m_p.m_x * tvel.m_p.m_x ) * sign( tvel.m_p.m_x );
		tvel.m_p.m_y = -( 0.01 ) * ( tvel.m_p.m_y * tvel.m_p.m_y ) * sign( tvel.m_p.m_y );
		tvel = tvel * m_mass;
		addForce( tvel );
	};
	virtual void applyVelocity();

	virtual void move( Vector2<double> vec )
	{
		m_p = m_p + vec;
	}
	virtual void resolveBlockCollision();

	virtual void resolveObjectCollision();

	virtual void resolvePartialBlockCollision( Rect<double> cbox, Point<double> );

	virtual void resolvePartialObjectCollision( Rect<double> cbox, Point<double> );

private:
	Rect<double> m_cbox;
	Game* m_parent;
	double m_mass;
	double m_invmass;
	Point<double> m_p;
	Point<double> m_pp;
	Vector2<double> m_velocity;
	Vector2<double> m_acceleration;
	Vector2<double> m_force;
};


class ObjectVector : public ObjectHolder
{
public:
	ObjectVector( Game* parent ): ObjectHolder( parent )
	{

	} ;
	virtual errc process ()
	{
		int count = Globals::TIMEACCUMULATOR / Globals::TIMESTEP;
		while( count-- )
			for( int i = 0; i < m_objects.size(); i++ )
			{
				if( m_objects.at( i ) != NULL ) m_objects.at( i )->process();
			}
		return 0;
	};
	virtual errc draw( sf::RenderWindow* window )
	{
		for( int i = 0; i < m_objects.size(); i++ )
		{
			if( m_objects.at( i ) != NULL ) m_objects.at( i )->draw( window );
		}
		return 0;
	}
	virtual errc add( Object* obj )
	{
		m_objects.push_back( obj );
		return 0;
	};
	virtual std::vector<Object*>* objectsInRange( Point<double> center, double r )
	{
		m_t_obj.clear();
		r = r * r;
		Point<double> tc;
		double dc = 0;
		for( int i = 0; i < m_objects.size(); i++ )
		{
			if( m_objects.at( i ) != NULL )
			{
				tc = m_objects.at( i )->getPos();
				dc = ( center.m_x - tc.m_x ) * ( center.m_x - tc.m_x ) + ( center.m_y - tc.m_y ) * ( center.m_y - tc.m_y );
				if ( dc <= r )
				{
					m_t_obj.push_back( m_objects.at( i ) );
					m_objects.at( i )->inrange = 1;
				}
			}

		}

		return &m_t_obj;
	}
	~ObjectVector();
protected:
private:

	std::vector<Object*> m_objects;
	std::vector<Object*> m_t_obj;
};






#endif // WORLD_H

