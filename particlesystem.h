#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "bg.h"
#include <SFML/Graphics.hpp>

class UQVA;
class ParticleSystem;
typedef void( *PMF )( Point<float>& , float , float ); //Particle Movement function;
typedef void( *PCF )( sf::Color&, float , float );
class Particle
{
public:

	friend class ParticleSystem;
	Particle( Point<float> pos, float angle, PMF pmf, PCF pcf, sf::Color startcolor, float lifetime );
	virtual ~Particle();
	void draw( UQVA* uqva );
	void process();
	void setOffset( float x, float y );
protected:
private:
	float m_x;
	float m_y;
	Point<float> m_pos;
	float m_angle;
	sf::Color m_color;
	PMF m_pmf;
	PCF m_pcf;
	float m_lifetime;
	float m_lifetimeleft;
	unsigned m_holderpos;
	ParticleSystem* m_parent;

};


class ParticleSystem
{
public:
	friend class Particle;
	ParticleSystem();
	virtual ~ParticleSystem();
	void draw( UQVA* uqva );
	void process();
	void addParticle( Particle* particle );
protected:
	void removeParticle( unsigned pos );
private:
	std::vector<Particle*> m_particles;
};

#endif // PARTICLESYSTEM_H
