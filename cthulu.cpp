#include "cthulu.h"

Cthulu::Cthulu( float x, float y ) : MovingWorld( x, y, Globals::GAME, 1000, Rect<float>() )
{
	Block* blok = 0;

	getSGO().setXY( 0, 10, 10 );

	blok = new ShiftedBlock( 0, 4, this, this );
	getSGO().setXY( blok, 0, 4 );
	blok->setPhase( SOLID );
	//return;
	//return;
	for( int i = 0; i < 4; i++ )
	{
		blok = new ShiftedBlock( 1 + i, 1, this, this );
		getSGO().setXY( blok, 1 + i, 1 );
		blok->setPhase( SOLID );
	}

	for( int i = 0; i < 6; i++ )
	{
		blok = new ShiftedBlock( i, 2, this, this );
		getSGO().setXY( blok, i, 2 );
		blok->setPhase( SOLID );
	}

	for( int i = 0; i < 6; i++ )
	{
		blok = new ShiftedBlock( 1 + i, 3, this, this );
		getSGO().setXY( blok, 1 + i, 3 );
		blok->setPhase( SOLID );
	}
	for( int i = 0; i < 2; i++ )
	{
		blok = new ShiftedBlock( 5 + i, 4, this, this );
		getSGO().setXY( blok, 5 + i, 4 );
		blok->setPhase( SOLID );
	}

	blok = new ShiftedBlock( 7, 5, this, this );
	getSGO().setXY( blok, 7, 5 );
	blok->setPhase( SOLID );


}

Cthulu::~Cthulu()
{
	//dtor
}
errc Cthulu::draw( UQVA* uqva )
{
	if( isDead() ) MovingWorld::draw( uqva );
	else
	{

		Vector2<float> vel = getVelocity();
		Vector2<float> pos = getPos();
		int orientation = ( vel.m_p.m_x > 0 ) ? 1 : 0;
		if( abs( vel.m_p.m_x ) < 0.1 ) orientation = 0;

		uqva->addQuad( pos.m_p.m_x, pos.m_p.m_y, 200, 150, 0, 118 );
		//std::cout << 118 + orientation * 160 << " " << pos.m_p.m_x << " " << pos.m_p.m_y << " " <<  std::endl;

	}
}

void Cthulu::onCollision( Block* block )
{
	Vector2<float> vel = getVelocity();
	float dmg = vel.m_p.m_x * vel.m_p.m_x + vel.m_p.m_y * vel.m_p.m_y;
	block->damage( dmg * 2 );
}
void Cthulu::flipCollision()
{
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 6; j++ )
		{
			Block* t = getSGO().getXY( i, j );
			getSGO().setXY( getSGO().getXY( 7 - i, j ), i, j );
			getSGO().setXY( t, 7 - i, j );
		}
	}
}
void Cthulu::onCollision( Object* object )
{
	Vector2<float> vel = getVelocity();
	float dmg = vel.m_p.m_x * vel.m_p.m_x + vel.m_p.m_y * vel.m_p.m_y;
	object->damage( dmg * 2 );
}

void Cthulu::onProcess()
{
	Point<float> hpos = Globals::GAME->object->getPos();
	Point<float> cpos = getPos();
	hpos = hpos + ( cpos * -1 );
	float len = hpos.m_x * hpos.m_x + hpos.m_y * hpos.m_y;
	len = sqrtf( len );
	hpos = hpos * ( 1.5 / len );
	addVelocity( Vector2<float>( Point<float>( 0, 0 ), hpos ) );
}
