#ifndef WORLD_CPP
#define WORLD_CPP

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <math.h>
#include "world.h"
#include "normals.h"




World::World( Game* parent, ui x, ui y )
{
	m_t_bl = new std::vector<Block*>;
	m_parent = parent;
	m_x = x;
	m_y = y;

	srand( 0 );
	m_blocks = ( Block*** )malloc( sizeof( Block** )*x );
	for( ui i = 0; i < y; i++ )
	{
		m_blocks[i] = ( Block** )malloc( sizeof( Block* )*y );
	}
	for( ui i = 0; i < x; i++ )
	{
		for( ui j = 0; j < y; j++ )
		{
			m_blocks[i][j] = new Block( i, j, this );
		}
	}




}

World::~World()
{
	for( int i = 0; i < m_x; i++ )
	{
		for( int j = 0; j < m_y; j++ )
		{
			delete m_blocks[i][j];
		}
		free( m_blocks[i] );
	}
	free( m_blocks );
}

errc World::process( int xoffset, int yoffset, int xrange, int yrange )
{
	int startx = xoffset / Globals::BLOCKSIZE - 1;
	int starty = yoffset / Globals::BLOCKSIZE - 1;
	int count = Globals::TIMEACCUMULATOR / Globals::TIMESTEP;
	while( count-- )
		for( int i = startx; i < xrange + startx + 2; i++ )
		{
			for( int j = starty; j < yrange + starty + 2; j++ )
			{
				if( i > 0 && j > 0 && i < m_x && j < m_y )
					m_blocks[i][j]->process();
			}
		}
	return 0;
}

Block* World::getBlock( int x, int y )
{
	if( x >= 0 && y >= 0 && x < m_x && y < m_y ) return m_blocks[x][y];
	return NULL;
}

errc World::draw( sf::RenderWindow* rw, int xoffset, int yoffset, int xrange, int yrange, int bs )
{
	int startx = xoffset / bs - 1;
	int starty = yoffset / bs - 1;
	for( int i = startx; i < xrange + startx + 2; i++ )
	{
		for( int j = starty; j < yrange + starty + 2; j++ )
		{

			if( i >= 0 && j >= 0 && i < m_x && j < m_y )
				m_blocks[i][j]->draw( rw, -xoffset, -yoffset, bs );
		}
	}
	return 0;
}

Block** World::getNeighbours( int x, int y, Block** buffer )
{

	buffer[0] = getBlock( x, y - 1 );
	buffer[1] = getBlock( x, y + 1 );
	buffer[2] = getBlock( x - 1, y );
	buffer[3] = getBlock( x + 1, y );
	return buffer;
}
std::vector<Block*>* World::getCollision( Object* obj )
{
	return getCollision( obj->getCbox() );
}

std::vector<Block*>* World::getMtbl()
{
	return m_t_bl;
}
std::vector<Block*>* World::getCollision( Rect<double> cbox )
{
	m_t_bl->clear();
	return getCollision( m_t_bl, cbox );
}
std::vector<Block*>* World::getCollision( std::vector<Block*>* t_bl, Rect<double> cbox )
{

	double bs = Globals::BLOCKSIZE;
	double xmin = cbox.getCorner( BOTLEFT ).m_x / bs;
	double ymin = cbox.getCorner( BOTLEFT ).m_y / bs;

	double xmax = cbox.getCorner( TOPRIGHT ).m_x;
	double ymax = cbox.getCorner( TOPRIGHT ).m_y;

	if( xmax == 0 || ymax == 0 ) return t_bl;

	xmax = ceil( xmax / bs );
	ymax = ceil( ymax / bs );

	int xmaxi = xmax;
	int ymaxi = ymax;



	for( int i = xmin; i < xmaxi; i++ )
	{
		for( int j = ymin; j < ymaxi; j++ )
		{
			t_bl->push_back( getBlock( i, j ) );
		}
	}


	return t_bl;
}

int Object::isColliding( Block* block )
{
	Point<double> bp = block->getPos();
	bp.m_x = bp.m_x * Globals::BLOCKSIZE;
	bp.m_y = bp.m_y * Globals::BLOCKSIZE;
	Rect<double> col = Rect<double>( bp, bp + Point<double>( Globals::BLOCKSIZE, Globals::BLOCKSIZE ) );
	return getCbox().getCollision( col );
}
void Object::applyVelocity()
{
	double tp = static_cast<double>( Globals::TIMESTEP ) * 0.1;
	double dxd = m_velocity.m_p.m_x * tp;
	double dyd = m_velocity.m_p.m_y * tp;

	m_p = m_p + Point<double>( dxd, dyd );


}
void Object::resolveBlockCollision()
{
	resolvePartialBlockCollision( getCbox(), m_p );
}

void Object::resolveObjectCollision()
{
	resolvePartialObjectCollision( getCbox(), m_p );
}

void Object::resolvePartialBlockCollision( Rect<double> cbox2, Point<double> p )
{
	std::vector<Block*>* blocks = Globals::GAME->getCollision( this, cbox2 );
	for( int i = 0; i < blocks->size(); i++ )
	{
		if( blocks->at( i ) != NULL )
		{
			if( blocks->at( i )->getPhase() == SOLID )
			{
				Point<double> point = blocks->at( i )->getPos();
				Rect<double> cbox = blocks->at( i )->getCbox();

				normals snorm = blocks->at( i )->getAvailNormals();
				Vector2<double> normal =  snorm.getImpulseNorm( cbox, cbox2, m_velocity + ( blocks->at( i )->getVelocity() * -1 ) );

				double normalvel = dotProduct( m_velocity + ( blocks->at( i )->getVelocity() * -1 ), normal );
				if( normalvel > 0 )
				{

					addVelocity( normal * ( -normalvel - 1 ) );
					blocks->at( i )->addVelocity( normal * ( normalvel + 1 ) );

				}
			}
		}
	}
}

void Object::resolvePartialObjectCollision( Rect<double> cbox2, Point<double> p )
{
	std::vector<Object*>* objects = Globals::GAME->obj_vec->objectsInRange( p, 100000 );
	for( int i = 0; i < objects->size(); i++ )
	{

		if( objects->at( i ) != NULL )
		{
			if( cbox2.getCollision( objects->at( i )->getCbox() ) && objects->at( i ) != this && objects->at( i )->resolveCollision() )
			{


				Point<double> point = objects->at( i )->getPos();
				Rect<double> cbox = objects->at( i )->getCbox();
				normals snorm;
				snorm.selectNormals( 1, 1, 1, 1 );
				Vector2<double> normal =  snorm.getImpulseNorm( cbox, cbox2, m_velocity + ( objects->at( i )->m_velocity * -1 ) );
				double normalvel = dotProduct( m_velocity + ( objects->at( i )->m_velocity * -1 ), normal );


				if( normalvel > 0 )
				{

					addVelocity( normal * ( -normalvel - sign( normalvel ) * 3 / 4 ) );
					objects->at( i )->addVelocity( normal * ( normalvel + sign( normalvel ) * 3 / 4 ) );

				}

			}

		}
	}
}
bool Object::resolveCollision()
{
	return true;
}

void Object::addAcceleration()
{
	double tp = Globals::TIMESTEP * 0.1;
	double dxd = m_acceleration.m_p.m_x * tp;
	double dyd = m_acceleration.m_p.m_y * tp;
	m_velocity = m_velocity + Vector2<double>( dxd, dyd );

}

errc Object::process()
{

	processPhysics();
	countAcceleration();
	addAcceleration();

	applyVelocity();
	m_force = Vector2<double>( 0, 0 );
	m_acceleration = Vector2<double>( 0, 0 );
	resolveBlockCollision();
	resolveObjectCollision();





	return 0;
}

errc Object::draw( sf::RenderWindow* window )
{

	sf::RectangleShape rekt( sf::Vector2f( m_cbox.getCorner( TOPRIGHT ).m_x - m_cbox.getCorner( BOTLEFT ).m_x, m_cbox.getCorner( TOPRIGHT ).m_y - m_cbox.getCorner( BOTLEFT ).m_y ) );
	rekt.setFillColor( sf::Color( 0, 255, 100 ) );
	rekt.setPosition( m_p.m_x - xoffset + m_cbox.getCorner( BOTLEFT ).m_x, m_p.m_y - yoffset + m_cbox.getCorner( BOTLEFT ).m_y );
	window->draw( rekt );
	inrange = 0;

	return 0;
}


Block::Block( int x, int y, BlockHolder* parent )
{
	m_parent = parent;
	tw = 0;
	m_x = x;
	m_y = y;
	m_phase = GAS;
	if( y > 100 ) m_phase = LIQUID;
	if( y > 200 ) m_phase = SOLID;
}

Block::~Block()
{

}


errc Block::draw( sf::RenderWindow* window, int xoffset, int yoffset, int bs )
{
	if( m_phase == NONE ) return -1;
	m_color = sf::Color( 255, 255, 255 );
	if( m_phase == LIQUID ) m_color = sf::Color( 0, 255, 50 );
	if( m_phase == SOLID ) m_color = sf::Color( 200, 150, 100 );
	sf::RectangleShape rect( sf::Vector2f( bs, bs ) );
	Rect<double> tr;


	rect.setFillColor( m_color );

	rect.setPosition( ( m_x )*bs + xoffset, yoffset + ( m_y )*bs );
	window->draw( rect );
	return 0;
}

normals Block::getAvailNormals()
{
	bool left = 0, right = 0, bot = 0, top = 0;
	if( m_parent->getBlock( m_x, m_y + 1 ) ) top = ( m_parent->getBlock( m_x, m_y + 1 )->getPhase() == SOLID ) ? true : false;

	if( ( m_parent->getBlock( m_x, m_y - 1 ) ) ) bot = ( m_parent->getBlock( m_x, m_y - 1 )->getPhase() == SOLID ) ? true : false;

	if( ( m_parent->getBlock( m_x + 1, m_y ) ) )left = ( m_parent->getBlock( m_x + 1, m_y )->getPhase() == SOLID ) ? true : false;

	if( ( ( m_parent->getBlock( m_x - 1, m_y ) ) ) ) right = ( m_parent->getBlock( m_x - 1, m_y )->getPhase() == SOLID ) ? true : false;
	return normals( !left, !right, !bot, !top );

}
errc Block::process()
{
	Block* blocks[4];
	m_parent->getNeighbours( m_x, m_y, blocks );
	int count = Globals::TIMEACCUMULATOR / Globals::TIMESTEP;
	while( count-- )
		for( int i = 0; i < 4; i++ )
		{
			if( blocks[i] != NULL )
			{
				float tw1 = gettw();
				float tw2 = blocks[i]->gettw();
				float dif = tw2 - tw1;
				dif *= 0.01;
				settw( tw1 + dif );
				blocks[i]->settw( tw2 - dif );




			}


		}

	return 0;


}

float Block::gettw()
{
	return tw;
}
float Block::settw( float ntw )
{
	tw = ntw;
	return tw;
}


#endif // WORLD_CPP
