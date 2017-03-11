#ifndef LIQUIDIFIEREMITTER_H
#define LIQUIDIFIEREMITTER_H

#include "world.h"
#include "UQVA.h"


class LiquidifierEmitter : public Object
{
public:
	LiquidifierEmitter( float x, float y, ObjectHolder* parent );
	virtual ~LiquidifierEmitter();
	virtual bool resolveCollision();

	virtual errc draw( UQVA* uqva );




protected:
	void emit();
	virtual void processPhysics() { };
	virtual void countAcceleration() { };
	virtual void addAcceleration() { };
	virtual void applyVelocity() { };
	virtual void resolveBlockCollision() { };
	virtual void resolveObjectCollision() { };
	virtual void onProcess();
private:
	int m_cycle;
	float m_radi;

};

class LiquidifierBullet : public Object
{
public:
	LiquidifierBullet( float x, float y, float radi, ObjectHolder* parent );
	virtual ~LiquidifierBullet();
	virtual void onProcess();
	virtual void onCollision( Block* block );

protected:
private:
	int m_cyclesleft;




};

#endif // LIQUIDIFIEREMITTER_H
