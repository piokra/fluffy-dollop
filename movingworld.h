#ifndef __MOVINGWORLD_H__
#define __MOVINGWORLD_H__


#include "squaregrowingoutwards.h"
#include "world.h"
#include "counter.h"
#include "uqva.h"
#include "globals.h"
#include "inventory.h"

class MovingWorld : public BlockHolder, public Object
{
public:
	MovingWorld( int x, int y, Game* parent, float mass, Rect<float> cbox );
	virtual ~MovingWorld();
	virtual errc draw( UQVA* window );
	virtual errc process();
	virtual Block* getBlock( int x, int y );
	virtual Rect<float> getCbox();
	virtual std::vector<Block*>* getCollision( Object* q, std::vector<Block*>* t_bl, Rect<float> cbox );
	virtual unsigned getID();
	SquareGrowingOutwards<Block*>& getSGO();
protected:
	virtual void resolveBlockCollision();
	virtual void resolveObjectCollision();
	virtual bool resolveCollision();
private:
	Point<float> m_p;
	SquareGrowingOutwards<Block*> m_sgo;




};

class ShiftedBlock : public Block , public Counter<ShiftedBlock>
{
public:
	ShiftedBlock( int x, int y, BlockHolder* parent, MovingWorld* mwparent ) : Block( x, y, parent )
	{
		m_mw = mwparent;
	};
	~ShiftedBlock() { };
	virtual Rect<float> getCbox();
	virtual Vector2<float> getVelocity();
	virtual void addVelocity( Vector2<float> );
	virtual unsigned getID();

protected:
private:
	MovingWorld* m_mw;

};

enum AnimationPhase
{
	AP_IDLE,
	AP_RUNNING,
	AP_JUMPING
};
class ItemPickUp;
class Hero : public Object
{
public:

	Hero( int x, int y, ObjectHolder* parent, float mass, Rect<float> bounds );
	~Hero()
	{
		Globals::GAME->object = 0;
	};
	virtual errc draw( UQVA* uqva );
	errc customDraw( UQVA* uqva );
	virtual void pickUp( ItemPickUp* pickup, Item* item );
	Inventory& getInventory();
protected:
	virtual void onProcess();
	virtual void taskSelectSlot( unsigned i );
	virtual void taskMove( unsigned i );
	virtual void taskUnmove( unsigned i );
	virtual void taskJump();
	virtual void taskUse();
	virtual void taskUnuse();
	virtual void onCollision( Block* block );
	virtual void onDeath();
	void processTasks();
	virtual void processPhysics()
	{
		addForce( Vector2<float>( 0, 0.5 * m_mass ) );
		addForce( Vector2<float>( -10 * m_left + 10 * m_right, -10 * m_up + 10 * m_down ) );
		Vector2<float> tvel = getVelocity();
		tvel.m_p.m_x = -( 0.01 ) * ( tvel.m_p.m_x * tvel.m_p.m_x ) * sign( tvel.m_p.m_x );
		tvel.m_p.m_y = -( 0.01 ) * ( tvel.m_p.m_y * tvel.m_p.m_y ) * sign( tvel.m_p.m_y );
		tvel = tvel * m_mass;
		addForce( tvel );
	};
private:
	int m_jumpcount;
	int m_dphase;
	int m_left;
	int m_right;
	int m_up;
	int m_down;
	int m_animation_oount;
	bool m_using;
	float m_cooldown;
	Inventory m_inventory;
	AnimationPhase m_animation_phase;
	AnimationPhase m_animation_phase_next;
};





#endif // __MOVINGWORLD_H__
