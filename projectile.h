#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "world.h"

class Projectile : public Object
{
public:
	Projectile( float x, float y, float damage, float tomobs, float toblocks, float angle, float vel, float lifetime );
	virtual ~Projectile();
	virtual errc draw ( UQVA* uqva );
protected:
	virtual void onProcess();
	virtual void onCollision( Block* block );
	virtual void onCollision( Object* object );
private:
	float m_lifetime;
	float m_damage;
	float m_mobmultiplier;
	float m_blockmultiplier;
	float m_angle;
};

class Bullet : public Projectile
{
public:
	Bullet( float x, float y, float damage, float tomobs, float toblocks, float angle, float vel, float lifetime );
	virtual ~Bullet();
protected:
	virtual void processPhysics();

private:


};



#endif // PROJECTILE_H
