#include "projectileemitter.h"
#include "globals.h"
ProjectileEmitter::ProjectileEmitter( float x, float y, sf::Color color, PMF pmf, PCF pcf ) : Object( x, y, 0, 0, Rect<float>() )
{
	m_lifetime = 10;
	auto dist = std::uniform_real_distribution<float>( 0, 6.2830 );
	auto rndangle = std::bind( dist, Globals::RANDOMGENERATOR );
	unsigned count = 30;
	while( count-- )
	{

		m_particlesystem.addParticle( new Particle( getPos(), rndangle(), pmf, pcf, color, 1000 ) );


	}
	//ctor
}

ProjectileEmitter::~ProjectileEmitter()
{
	//dtor
}
errc ProjectileEmitter::draw( UQVA* uqva )
{
	m_particlesystem.draw( uqva );
}

void ProjectileEmitter::onProcess()
{

	m_lifetime--;
	m_particlesystem.process();
	if( m_lifetime <= 0 ) kill();
}



bool ProjectileEmitter::resolveCollision()
{
	return false;
}
