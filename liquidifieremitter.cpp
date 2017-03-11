#include "liquidifieremitter.h"

LiquidifierEmitter::LiquidifierEmitter( float x, float y, ObjectHolder* parent ) : Object( x, y, parent, 1, Rect<float>() )
{
	//ctor
}

LiquidifierEmitter::~LiquidifierEmitter()
{
	//dtor
}
bool LiquidifierEmitter::resolveCollision()
{
	return false;
}

void LiquidifierEmitter::onProcess()
{
	m_cycle++;
	m_cycle %= 100;
	if( !m_cycle )
	{
		m_radi += 0.33;
		emit();
	}
}

errc LiquidifierEmitter::draw( UQVA* uqva )
{

	uqva->addQuad( getPos().m_x, getPos().m_y, 32, 32, 0, 0 );
	uqva->changeColor( sf::Color( 0, 0, 0 ), 4 );
}
void LiquidifierEmitter::emit()
{
	Point<float> pos = getPos();
	Object* obj = new LiquidifierBullet( pos.m_x, pos.m_y, m_radi, getParent() );
	getParent()->add( obj );
}

LiquidifierBullet::LiquidifierBullet( float x, float y, float radi, ObjectHolder* parent ) : Object( x, y, parent, 1, Rect<float>( Point<float>( -5, -5 ), Point<float>( 5, 5 ) ) )
{
	m_cyclesleft = 1000;
	Vector2<float>uv = Vector2<float>( cos( radi ), sin( radi ) );
	addVelocity( uv * 100 );
}

LiquidifierBullet::~LiquidifierBullet()
{

}

void LiquidifierBullet::onProcess()
{

	m_cyclesleft--;

	if( m_cyclesleft == 0 ) kill();
}

void LiquidifierBullet::onCollision( Block* block )
{
	kill();
	if( block )
	{
		block->setPhase( LIQUID );
		block->setLiquidCount( 1000 );
	}

}




