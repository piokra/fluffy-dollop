#include "world.h"
#include "movingworld.h"


MovingWorld::MovingWorld( int x, int y, Game* parent, double mass, Rect<double>cbox ) : BlockHolder(), Object( x, y, parent, mass, cbox )
{
	//ctor
	m_sgo.setXY( new ShiftedBlock( 1, 1, this, this ), 1, 1 );

	m_sgo.setXY( new ShiftedBlock( -1, 0, this, this ), -1, 0 );
	m_sgo.setXY( new ShiftedBlock( -1, -1, this, this ), -1, -1 );
	m_sgo.setXY( new ShiftedBlock( 1, 0, this, this ), 1, 0 );
	m_sgo.getXY( 1, 1 )->setPhase( SOLID );
	m_sgo.getXY( -1, -1 )->setPhase( SOLID );
	m_sgo.setXY( new ShiftedBlock( 0, 0, this, this ), 0 , 0 );
	m_sgo.getXY( 0 , 0 )->setPhase( SOLID );
	m_sgo.setXY( new ShiftedBlock( 1, -1, this, this ), 1, -1 );
	m_sgo.setXY( new ShiftedBlock( 0, -1, this, this ), 0, -1 );
	m_sgo.setXY( new ShiftedBlock( 0, 1, this, this ), 0, 1 );
	m_sgo.setXY( new ShiftedBlock( -1, 1, this, this ), -1, 1 );
	m_sgo.setXY( new ShiftedBlock( 2, -2, this, this ), 2, -2 );
	m_sgo.getXY( 2, -2 )->setPhase( SOLID );
}

MovingWorld::~MovingWorld()
{
	//dtor
}
errc MovingWorld::process()
{

	Object::process();

}
Block* MovingWorld::getBlock( int x, int y )
{
	return m_sgo.getXY( x, y );
}

Vector2<double> ShiftedBlock::getVelocity()
{
	return m_mw->getVelocity();
}

void ShiftedBlock::addVelocity( Vector2<double> vel )
{

	m_mw->addVelocity( vel );
}


std::vector<Block*>* MovingWorld::getCollision( Object* q, std::vector<Block*>* t_bl, Rect<double> cbox )
{

	if ( q == this ) return t_bl;

	cbox = cbox + ( getPos() * -1 );


	double bs = Globals::BLOCKSIZE;
	double xmin = cbox.getCorner( BOTLEFT ).m_x / bs;
	double ymin = cbox.getCorner( BOTLEFT ).m_y / bs;

	double xmax = cbox.getCorner( TOPRIGHT ).m_x;
	double ymax = cbox.getCorner( TOPRIGHT ).m_y;


	xmax = ceil( xmax / bs );
	ymax = ceil( ymax / bs );

	int xmaxi = xmax;
	int ymaxi = ymax;



	for( int i = xmin; i < xmaxi; i++ )
	{
		for( int j = ymin; j < ymaxi; j++ )
		{

			Block* ta = m_sgo.getXY( i, j );
			if( ta != 0 )
				t_bl->push_back( ta );
		}
	}


	return t_bl;
}

void MovingWorld::resolveObjectCollision()
{

	int nex, ney, nwx, nwy, sex, sey, swx, swy;
	nex = m_sgo.getNEx();
	ney = m_sgo.getNEy();
	nwx = m_sgo.getNWx();
	nwy = m_sgo.getNWy();
	sex = m_sgo.getSEx();
	sey = m_sgo.getSEy();
	swx = m_sgo.getSWx();
	swy = m_sgo.getSWy();

	for( int i = 0; i < nex; i++ )
	{
		for( int j = 0; j < ney; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, j );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{

					resolvePartialObjectCollision( t->getCbox(), pos );

				}
			}
		}
	}
	for( int i = 0; i < sex; i++ )
	{
		for( int j = 0; j < sey; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, -j - 1 );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{

					resolvePartialObjectCollision( t->getCbox(), pos );
				}
			}
		}
	}
	for( int i = 0; i < nwx; i++ )
	{
		for( int j = 0; j < nwy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, j );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{

					resolvePartialObjectCollision( t->getCbox(), pos );
				}
			}
		}
	}
	for( int i = 0; i < swx; i++ )
	{
		for( int j = 0; j < swy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, -j - 1 );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{


					resolvePartialObjectCollision( t->getCbox(), pos );
				}
			}
		}
	}
}
bool MovingWorld::resolveCollision()
{
	return false;
}

void MovingWorld::resolveBlockCollision()
{
	int nex, neyy, nwx, nwy, sex, sey, swx, swy;
	nex = m_sgo.getNEx();
	neyy = m_sgo.getNEy();
	nwx = m_sgo.getNWx();
	nwy = m_sgo.getNWy();
	sex = m_sgo.getSEx();
	sey = m_sgo.getSEy();
	swx = m_sgo.getSWx();
	swy = m_sgo.getSWy();

	for( int i = 0; i < nex; i++ )
	{
		for( int j = 0; j < neyy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, j );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{


					resolvePartialBlockCollision( t->getCbox(), pos );
				}

			}
		}
	}
	for( int i = 0; i < sex; i++ )
	{
		for( int j = 0; j < sey; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, -j - 1 );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{

					resolvePartialBlockCollision( t->getCbox(), pos );
				}
			}
		}
	}
	for( int i = 0; i < nwx; i++ )
	{
		for( int j = 0; j < nwy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, j );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{

					resolvePartialBlockCollision( t->getCbox(), pos );
				}
			}
		}
	}
	for( int i = 0; i < swx; i++ )
	{
		for( int j = 0; j < swy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, -j - 1 );
			if( t != 0 )
			{
				if( t->getPhase() == SOLID )
				{


					resolvePartialBlockCollision( t->getCbox(), pos );
				}
			}
		}
	}

}


Rect<double> MovingWorld::getCbox()
{
	return Rect<double>();
}


errc MovingWorld::draw( sf::RenderWindow* window )
{

	int nex, ney, nwx, nwy, sex, sey, swx, swy;
	nex = m_sgo.getNEx();
	ney = m_sgo.getNEy();
	nwx = m_sgo.getNWx();
	nwy = m_sgo.getNWy();
	sex = m_sgo.getSEx();
	sey = m_sgo.getSEy();
	swx = m_sgo.getSWx();
	swy = m_sgo.getSWy();

	for( int i = 0; i < nex; i++ )
	{
		for( int j = 0; j < ney; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, j );
			if( t != 0 )
			{
				t->draw( window, pos.m_x - xoffset, pos.m_y - yoffset, Globals::BLOCKSIZE );

			}
		}
	}
	for( int i = 0; i < sex; i++ )
	{
		for( int j = 0; j < sey; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( i, -j - 1 );
			if( t != 0 )
			{
				t->draw( window, pos.m_x - xoffset, pos.m_y - yoffset, Globals::BLOCKSIZE );

			}
		}
	}
	for( int i = 0; i < nwx; i++ )
	{
		for( int j = 0; j < nwy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, j );
			if( t != 0 )
			{
				t->draw( window, pos.m_x - xoffset, pos.m_y - yoffset, Globals::BLOCKSIZE );

			}
		}
	}
	for( int i = 0; i < swx; i++ )
	{
		for( int j = 0; j < swy; j++ )
		{
			Point<double> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, -j - 1 );
			if( t != 0 )
			{
				t->draw( window, pos.m_x - xoffset, pos.m_y - yoffset, Globals::BLOCKSIZE );

			}
		}
	}


	return 0;
}

Rect<double> ShiftedBlock::getCbox()
{
	return Block::getCbox() + m_mw->getPos();
}

Hero::Hero( int x, int y, Game* parent, double mass, Rect<double> bounds ): MovingWorld( x, y, parent, mass, Rect<double>() ) { }

