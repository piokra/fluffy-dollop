#include "particlesystem.h"
#include "globals.h"
#include "UQVA.h"
ParticleSystem::ParticleSystem()
{
	//ctor
}

ParticleSystem::~ParticleSystem()
{
	while( m_particles.size() )
		delete m_particles[0];
	//dtor
}
Particle::Particle( Point<float> pos, float angle, PMF pmf, PCF pcf, sf::Color startcolor, float lifetime )
{
	m_pos = pos;
	m_angle = angle;
	m_color = startcolor;
	m_pmf = pmf;
	m_pcf = pcf;
	m_lifetimeleft = lifetime;
	m_lifetime = 0;
	m_holderpos = -1;
	m_parent = 0;
}

Particle::~Particle()
{
	if( m_parent )
		m_parent->removeParticle( m_holderpos );
}

void Particle::draw( UQVA* uqva )
{
	uqva->addQuad( m_pos.m_x + m_x, m_pos.m_y + m_y, 15, 15, 0, 0 );
	uqva->changeColor( m_color, 4 );
}

void Particle::process()
{
	m_lifetime += Globals::TIMESTEP;
	if( m_lifetime >= m_lifetimeleft )
	{
		delete this;
		return;
	}
	m_pmf( m_pos, m_angle, m_lifetime );
	m_pcf( m_color, m_angle, m_lifetime );
}

void ParticleSystem::draw( UQVA* uqva )
{
	for( int i = 0; i < m_particles.size(); i++ )
		m_particles[i]->draw( uqva );
}

void ParticleSystem::process()
{
	for( int i = 0; i < m_particles.size(); i++ )
		m_particles[i]->process();
}
void ParticleSystem::removeParticle( unsigned pos )
{
	if( pos < 0 || pos >= m_particles.size() )
		return;
	m_particles[pos] = m_particles[m_particles.size() - 1];
	m_particles[pos]->m_holderpos = pos;
	m_particles.pop_back();
}
void ParticleSystem::addParticle( Particle* particle )
{
	particle->m_parent = this;
	particle->m_holderpos = m_particles.size();
	m_particles.push_back( particle );

}
