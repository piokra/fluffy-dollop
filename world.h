#ifndef WORLD_H
#define WORLD_H

#include "globals.h"
#include "counter.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "bg.h"
#include "normals.h"

class UQVA;



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
	friend class World;
	friend class Block;
	friend class BlockPlacer;
	Block( int x, int y, BlockHolder* parent );
	virtual ~Block();
	virtual errc draw( UQVA*, int, int, int );
	virtual errc process();
	Phase getPhase()
	{
		return m_phase;
	}
	Point<float> getPos()
	{
		return Point<float>( m_x, m_y );
	}
	virtual Rect<float> getCbox()
	{

		return Rect<float>( Point<float>( m_x * Globals::BLOCKSIZE, m_y * Globals::BLOCKSIZE ), Point<float>( m_x * Globals::BLOCKSIZE + Globals::BLOCKSIZE,
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
	virtual Vector2<float> getVelocity()
	{
		return Vector2<float>( 0, 0 );
	};
	virtual void addVelocity( Vector2<float> ) { };
	virtual float getInvMass()
	{
		return 0;
	};
	virtual void onProcess();
	virtual void onSpawn();
	virtual void onDeath();
	virtual void onDamage();
	virtual void onCollision( Object* obj );
	virtual void onCollision( Block* block );
	virtual bool isDead();
	virtual bool use();
	virtual void  kill();
	virtual void damage( float dmg );
	virtual unsigned getID();
	void setLiquidCount( float cnt );
	float getLiquidCount();



protected:
	void setVecPos( unsigned i );
	const unsigned& getVecPos();
	void setItemID( unsigned id );
	void setItemTexture( Point<int> texture );
	virtual void setTexture( Point<int> texture );

private:
	float m_liquidc;
	float m_health;
	bool m_dead;
	sf::Color m_color;
	int m_x;
	int m_y;
	unsigned m_vecpos;
	float tw;
	BlockHolder* m_parent;
	Point<int> m_texture;
	Point<int> m_itemtexture;
	unsigned m_itemid;


};




class ObjectHolder
{
public:
	ObjectHolder( Game* parent )
	{
		m_parent = parent;
	};
	virtual ~ObjectHolder()
	{

	};
	virtual const std::vector<Object*>* objectsInRange( Point<float> pos, float r )
	{
		return NULL;
	}; // use sparingly
	virtual errc process()
	{
		return -1;
	};
	virtual errc draw( UQVA* )
	{
		return -1;
	};
	virtual errc add( Object* )
	{
		return -1;
	};
	virtual void removeFromPos( unsigned pos ) { };

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
		//printf( "Pure fagvirutal function called\n" );
		return NULL;
	};
	virtual Block** getNeighbours( int x, int y, Block* buffer[4] )
	{
		//printf( "Pure virutal function called\n" );
		return NULL;
	};
	virtual std::vector<Block*>* getCollision( Object* obj )
	{
		std::cout << "BH::getCollision\n";
		return NULL;
	};
	virtual Game* getParent()
	{
		std::cout << "BH::getParent\n";
	};
	virtual std::vector<Block*>* getCollision( Rect<float> rect )
	{
		std::cout << "BH::getCollision\n";
	};
	virtual void removeFromHolder( unsigned pos )
	{
		std::cout << "BH::removeFromHolder\n";
	};
	virtual void addToHolder( Block* block, unsigned x, unsigned y )
	{
		std::cout << "BH::addToHolder\n";
	};
};


class World : public BlockHolder
{
public:
	friend World;
	World( Game* parent, ui x, ui y );
	~World();
	virtual Block* getBlock( int x, int y );
	errc draw( UQVA*, int, int, int, int, int );
	errc process( int, int, int, int );
	std::vector<Block*>* getCollision( Object* obj );
	Game* getParent()
	{
		return m_parent;
	}

	virtual std::vector<Block*>* getCollision( Rect<float> rect );
	virtual std::vector<Block*>* getCollision( std::vector<Block*>* t_bl, Rect<float> rect );
	virtual Block** getNeighbours( int x, int y, Block** buffer );
	virtual void removeFromHolder( unsigned pos );
	virtual void addToHolder( Block* block, unsigned x, unsigned y );
	static void saveWorld();
	static void loadWorld();
protected:
	std::vector<Block*>* getMtbl();
private:
	Game* m_parent;
	std::vector<Block*>* m_t_bl;
	Block*** m_blocks;
	int m_x;
	int m_y;
};
class Item;
class ItemPickUp;
class Object : public Counter<Object>
{
public:
	friend class CollisionObjectVector;
	friend class ObjectVector;
	friend class Object;
	friend class Hero;
	friend class MeleeWeapon;
	Object( int x, int y, ObjectHolder* parent, float mass, Rect<float> bounds )
	{
		m_max_health = 100;
		m_health = 100;
		m_dead = 0;
		m_holderpos = -1;
		m_acceleration = Vector2<float>( 0, 0 );
		m_velocity = Vector2<float>( 0, 0 );
		m_p = Point<float>( x, y );
		m_parent = parent;
		m_cbox = bounds;
		m_mass = mass;
		m_invmass = 0;
		if( m_mass )
			m_invmass = 1 / mass;
		m_cparent = 0;


	};
	virtual ~Object()
	{
		m_parent->removeFromPos( getHolderPos() );
		if( m_cparent ) m_cparent->removeFromPos( getCVPos() );
	};
	virtual errc draw( UQVA* uqva );

	virtual errc process();

	const Point<float>& getPos()
	{
		return m_p;
	};
	void setPos( Point<float> a )
	{
		m_p = a;
	};
	Rect<float> getCbox()
	{
		return m_cbox + m_p;
	}
	virtual int isColliding( Object* obj )
	{
		return getCbox().getCollision( obj->getCbox() );
	}
	virtual int isColliding( Block* block );

	virtual void addForce( Vector2<float> force )
	{
		m_force = m_force + force;
	}

	virtual Vector2<float> getVelocity()
	{
		return m_velocity;
	}
	virtual Point<float> getPreviousPos()
	{
		return m_pp;
	}
	virtual void addVelocity( Vector2<float> vel )
	{
		m_velocity = m_velocity + vel;
	}
	virtual unsigned getID();
	virtual void pickUp( ItemPickUp* block, Item* obj );
	virtual void damage( float dmg );
protected:
	virtual void setParent( ObjectHolder* holder )
	{
		m_parent = holder;
	}
	virtual bool resolveCollision();
	virtual void countAcceleration()
	{
		m_acceleration = m_force * m_invmass;
	};
	virtual void addAcceleration();
	virtual void processPhysics()
	{
		addForce( Vector2<float>( 0, 1 * m_mass ) );
		Vector2<float> tvel = getVelocity();
		tvel.m_p.m_x = -( 0.01 ) * ( tvel.m_p.m_x * tvel.m_p.m_x ) * sign( tvel.m_p.m_x );
		tvel.m_p.m_y = -( 0.01 ) * ( tvel.m_p.m_y * tvel.m_p.m_y ) * sign( tvel.m_p.m_y );
		tvel = tvel * m_mass;
		addForce( tvel );
	};
	virtual void applyVelocity();

	virtual void move( Vector2<float> vec )
	{
		m_p = m_p + vec;
	}
	virtual void resolveBlockCollision();

	virtual void resolveObjectCollision();

	virtual void resolvePartialBlockCollision( Rect<float> cbox, Point<float> );

	virtual void resolvePartialObjectCollision( Rect<float> cbox, Point<float> );
	void setHolderPos( unsigned pos );
	const unsigned& getHolderPos();
	const unsigned& getCVPos();
	void setCVPos ( unsigned pos );
	void setCParent( ObjectHolder* cparent );
	ObjectHolder* getCVparent();
	ObjectHolder* getParent()
	{
		return m_parent;
	}
	void setTexture( Point<int> texturexy )
	{
		m_texture = texturexy;
	}
	virtual void onProcess();
	virtual void onSpawn();
	virtual void onDeath();
	virtual void onDamage();
	virtual void onCollision( Object* obj );
	virtual void onCollision( Block* block );


	virtual bool use();


	virtual bool isDead();
	virtual void kill();

	virtual void setMaxHealth( float maxh )
	{
		m_max_health = maxh;
	};
	virtual void heal( float dmg );
	virtual void onHeal();


	;
private:
	unsigned m_holderpos;
	Rect<float> m_cbox;
	ObjectHolder* m_parent;
	ObjectHolder* m_cparent;
	float m_mass;
	float m_invmass;
	Point<float> m_p;
	Point<float> m_pp;
	Vector2<float> m_velocity;
	Vector2<float> m_acceleration;
	Vector2<float> m_force;
	Point<int> m_texture;
	float m_health;
	float m_max_health;


	unsigned m_cvpos;
	bool m_dead;
};


class ObjectVector : public ObjectHolder
{

public:
	ObjectVector( Game* parent ): ObjectHolder( parent )
	{

	} ;
	virtual errc process ()
	{

		for( int i = 0; i < m_objects.size(); i++ )
		{
			if( m_objects.at( i ) != NULL ) m_objects.at( i )->process();
		}
		return 0;
	};
	virtual errc draw( UQVA* window )
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
		( obj->*getSetParentFunction() )( this );
		( obj->*getSetPosFunction() )( m_objects.size() - 1 );
		return 0;
	};
	virtual std::vector<Object*>* objectsInRange( Point<float> center, float r )
	{
		m_t_obj.clear();
		r = r * r;
		Point<float> tc;
		float dc = 0;
		for( int i = 0; i < m_objects.size(); i++ )
		{
			if( m_objects.at( i ) != NULL )
			{
				tc = m_objects.at( i )->getPos();
				dc = ( center.m_x - tc.m_x ) * ( center.m_x - tc.m_x ) + ( center.m_y - tc.m_y ) * ( center.m_y - tc.m_y );
				if ( dc <= r )
				{
					m_t_obj.push_back( m_objects.at( i ) );

				}
			}

		}

		return &m_t_obj;
	}
	virtual void removeFromPos( unsigned pos )
	{
		if ( pos == -1 ) return;
		Object* t = m_objects.at( pos );
		m_objects.at( pos ) = m_objects.at( m_objects.size() - 1 );
		if ( m_objects.at( pos ) ) m_objects.at( pos )->setHolderPos( pos );
		m_objects.pop_back();
	};
	virtual ~ObjectVector()
	{
		unsigned vsize = m_objects.size();
		while( vsize-- ) if ( m_objects.at( 0 ) != 0 ) deleteFromPos( 0 );
	}

protected:
	typedef const unsigned& ( Object::*HolderGetPosFunction )();
	typedef void ( Object::*HolderSetPosFunction )( unsigned );
	typedef ObjectHolder* ( Object::*HolderGetParentFunction )();
	typedef void ( Object::*HolderSetParentFunction )( ObjectHolder* );

	virtual HolderSetPosFunction getSetPosFunction()
	{
		return &Object::setHolderPos;
	}
	virtual HolderGetPosFunction getGetPosFunction()
	{
		return &Object::getHolderPos;
	}
	virtual HolderGetParentFunction getGetParentFunction()
	{
		return &Object::getParent;
	}
	virtual HolderSetParentFunction getSetParentFunction()
	{
		return &Object::setParent;
	}
	virtual void deleteFromPos( unsigned pos )
	{
		delete m_objects.at( pos );
	}

private:

	std::vector<Object*> m_objects;
	std::vector<Object*> m_t_obj;
};

class CollisionObjectVector : public ObjectVector
{

public:

	CollisionObjectVector( Game* game ) : ObjectVector( game )
	{

	}
	virtual ~CollisionObjectVector()
	{

	}
protected:
	virtual HolderSetPosFunction getSetPosFunction()
	{
		return &Object::setCVPos;


	}
	virtual HolderGetPosFunction getGetPosFunction()
	{
		return &Object::getCVPos;

	}
	virtual HolderGetParentFunction getGetParentFunction()
	{
		return &Object::getCVparent;

	}
	virtual HolderSetParentFunction getSetParentFunction()
	{
		return &Object::setCParent;

	}
	virtual void deleteFromPos( unsigned pos )
	{

	}
private:


};





#endif // WORLD_H

