#ifndef PROJECTILEEMITTER_H
#define PROJECTILEEMITTER_H

#include "particlesystem.h"
#include "world.h"
class UQVA;
class ProjectileEmitter : public Object
{
public:
	ProjectileEmitter( float x, float y, sf::Color color, PMF pmf, PCF pcf );
	virtual ~ProjectileEmitter();
	virtual errc draw( UQVA* uqva );
protected:
	virtual void onProcess();
	virtual bool resolveCollision();
private:
	float m_lifetime;
	ParticleSystem m_particlesystem;
	PMF m_pmf;
	PCF m_pcf;
};

#endif // PROJECTILEEMITTER_H
