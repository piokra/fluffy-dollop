#include "projectile.h"
#include "UQVA.h"
void Projectile::onCollision( Block* block )
{
	block->damage( m_damage * m_blockmultiplier );
	damage( 33 );
}

void Projectile::onCollision( Object* object )
{
	object->damage( m_damage * m_mobmultiplier );
	damage( 33 );
}



void Bullet::processPhysics()
{

}

void Projectile::onProcess()
{
	Vector2<float> force = Vector2<float>( cosf( m_angle ) * 3, sinf( m_angle ) * 1 );
	addForce( force );
	m_lifetime -= Globals::TIMESTEP;
	if( m_lifetime <= 0 ) kill();
}


Projectile::Projectile( float x, float y, float damage, float tomobs, float toblocks, float angle, float vel, float lifetime ) : Object( x, y, 0, 1, Rect<float>( Point<float>( 0, 0 ),
			Point<float>( 15, 15 ) ) )
{

	m_damage = damage;
	m_mobmultiplier = tomobs;
	m_blockmultiplier = toblocks;
	m_angle = angle;
	m_lifetime = lifetime;
	Vector2<float> vel2 = Vector2<float>( cosf( angle ) * vel, sinf( angle ) * vel );
	addVelocity( vel2 );
}
errc Projectile::draw( UQVA* uqva )
{

	uqva->addQuad( getPos().m_x, getPos().m_y, 24, 24, 103, 46, 0, 0, m_angle );
}

Projectile::~Projectile()
{

}

Bullet::Bullet( float x, float y, float damage, float tomobs, float toblocks, float angle, float vel, float lifetime ) : Projectile( x, y, damage, tomobs, toblocks, angle, vel, lifetime )
{
	setMaxHealth( 33 );
}

Bullet::~Bullet()
{

}
