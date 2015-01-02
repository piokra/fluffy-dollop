#ifndef __MOVINGWORLD_H__
#define __MOVINGWORLD_H__


#include "squaregrowingoutwards.h"
#include "world.h"



class MovingWorld : public BlockHolder, public Object
{
public:
	MovingWorld( int x, int y, Game* parent, double mass, Rect<double> cbox );
	virtual ~MovingWorld();
	virtual errc draw( sf::RenderWindow* window );
	virtual errc process();
	virtual Block* getBlock( int x, int y );
	virtual Rect<double> getCbox();
	virtual std::vector<Block*>* getCollision( Object* q, std::vector<Block*>* t_bl, Rect<double> cbox );

protected:
	virtual void resolveBlockCollision();
	virtual void resolveObjectCollision();
	virtual bool resolveCollision();
private:
	Point<double> m_p;
	SquareGrowingOutwards<Block*> m_sgo;




};

class ShiftedBlock : public Block
{
public:
	ShiftedBlock( int x, int y, BlockHolder* parent, MovingWorld* mwparent ) : Block( x, y, parent )
	{
		m_mw = mwparent;
	};
	~ShiftedBlock() { };
	virtual Rect<double> getCbox();
	virtual Vector2<double> getVelocity();
	virtual void addVelocity( Vector2<double> );

protected:
private:
	MovingWorld* m_mw;

};
class Hero : public MovingWorld
{
public:

	Hero( int x, int y, Game* parent, double mass, Rect<double> bounds );
	~Hero() {};

protected:
	virtual void processPhysics()
	{
		addForce( Vector2<double>( 0, 0.5 * m_mass ) );
		addForce( Vector2<double>( 10 * mx, 10 * my ) );
		Vector2<double> tvel = getVelocity();
		tvel.m_p.m_x = -( 0.01 ) * ( tvel.m_p.m_x * tvel.m_p.m_x ) * sign( tvel.m_p.m_x );
		tvel.m_p.m_y = -( 0.01 ) * ( tvel.m_p.m_y * tvel.m_p.m_y ) * sign( tvel.m_p.m_y );
		tvel = tvel * m_mass;
		addForce( tvel );
	};
private:
};





#endif // __MOVINGWORLD_H__
