#ifndef WORLD_CPP
#define WORLD_CPP

#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <math.h>
#include "world.h"
#include "normals.h"
#include "globals.h"
#include "uqva.h"
#include "itempickup.h"
#include "items.h"
#include "projectileemitter.h"
#include <fstream>
#include "movingworld.h"
#include "worldgenerator.h"

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
			m_blocks[i][j]->setVecPos( i + j * y );
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

errc World::draw( UQVA* uqva, int xoffset, int yoffset, int xrange, int yrange, int bs )
{
	int startx = xoffset / bs - 1;
	int starty = yoffset / bs - 1;
	for( int i = -xrange / 2; i < xrange + 2; i++ )
	{
		for( int j = -yrange / 2; j < yrange + 2; j++ )
		{

			if( i + startx >= 0 && j + starty >= 0 && i + startx < m_x && j + starty < m_y )
				m_blocks[i + startx][j + starty]->draw( uqva, 0, 0, bs );
		}
	}
	return 0;
}


void World::removeFromHolder( unsigned pos )
{
	int x, y;
	x = pos % m_y;
	y = pos / m_y;
	m_blocks[x][y] = 0;
}

void World::addToHolder( Block* block, unsigned x, unsigned y )
{
	if ( m_blocks[x][y] != 0 ) delete m_blocks[x][y];
	m_blocks[x][y] = block;
	block->setVecPos( x + y * m_x );

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
std::vector<Block*>* World::getCollision( Rect<float> cbox )
{
	m_t_bl->clear();
	return getCollision( m_t_bl, cbox );
}
std::vector<Block*>* World::getCollision( std::vector<Block*>* t_bl, Rect<float> cbox )
{

	float bs = Globals::BLOCKSIZE;
	float xmin = cbox.getCorner( BOTLEFT ).m_x / bs;
	float ymin = cbox.getCorner( BOTLEFT ).m_y / bs;

	float xmax = cbox.getCorner( TOPRIGHT ).m_x;
	float ymax = cbox.getCorner( TOPRIGHT ).m_y;

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
unsigned Object::getID()
{
	return 0;
}
unsigned Block::getID()
{
	return 0;
}

void Object::setHolderPos( unsigned pos )
{
	m_holderpos = pos;
}

const unsigned& Object::getHolderPos()
{
	return m_holderpos;
}


int Object::isColliding( Block* block )
{
	Point<float> bp = block->getPos();
	bp.m_x = bp.m_x * Globals::BLOCKSIZE;
	bp.m_y = bp.m_y * Globals::BLOCKSIZE;
	Rect<float> col = Rect<float>( bp, bp + Point<float>( Globals::BLOCKSIZE, Globals::BLOCKSIZE ) );
	return getCbox().getCollision( col );
}
void Object::applyVelocity()
{
	float tp = static_cast<float>( Globals::TIMESTEP ) * 0.1;
	float dxd = m_velocity.m_p.m_x * tp;
	float dyd = m_velocity.m_p.m_y * tp;

	m_p = m_p + Point<float>( dxd, dyd );


}
void Object::resolveBlockCollision()
{
	resolvePartialBlockCollision( getCbox(), m_p );
}

void Object::resolveObjectCollision()
{
	resolvePartialObjectCollision( getCbox(), m_p );
}

void Object::resolvePartialBlockCollision( Rect<float> cbox2, Point<float> p )
{
	std::vector<Block*>* blocks = Globals::GAME->getCollision( this, cbox2 );
	for( int i = 0; i < blocks->size(); i++ )
	{
		if( blocks->at( i ) != NULL )
		{
			if( blocks->at( i )->getPhase() == SOLID )
			{
				onCollision( blocks->at( i ) );
				Point<float> point = blocks->at( i )->getPos();
				Rect<float> cbox = blocks->at( i )->getCbox();

				normals snorm = blocks->at( i )->getAvailNormals();
				Vector2<float> normal =  snorm.getImpulseNorm( cbox, cbox2, m_velocity + ( blocks->at( i )->getVelocity() * -1 ) );

				float normalvel = dotProduct( m_velocity + ( blocks->at( i )->getVelocity() * -1 ), normal );
				//Impulse
				if( normalvel > 0.000 )
				{

					addVelocity( normal * ( -normalvel - 0.6f ) );
					blocks->at( i )->addVelocity( normal * ( normalvel + 0.4 ) );
					//Torque

					float forcenormal = -0.1 * dotProduct( m_force, normal );
					float t = normal.m_p.m_x;
					normal.m_p.m_x = normal.m_p.m_y;
					normal.m_p.m_y = -t;
					normalvel = dotProduct( m_velocity + ( blocks->at( i )->getVelocity() * -1 ), normal );
					;
					if( normalvel > 0 )
					{

						addVelocity( normal * forcenormal * m_invmass );

						//addVelocity( normal * ( -0.15 ) );

					}
					else
					{
						normal = normal * -1;

						addVelocity( normal * forcenormal * m_invmass );

					}
					//if( abs( m_velocity.m_p.m_x ) < 0.15 ) m_velocity.m_p.m_x = 0;
				}

			}
		}
	}
}

void Object::resolvePartialObjectCollision( Rect<float> cbox2, Point<float> p )
{
	std::vector<Object*>* objects = Globals::GAME->col_vec->objectsInRange( p, Globals::BLOCKSIZE * 4 );
	for( int i = 0; i < objects->size(); i++ )
	{

		if( objects->at( i ) != NULL )
		{

			if( cbox2.getCollision( objects->at( i )->getCbox() ) && objects->at( i ) != this  )
			{
				onCollision( objects->at( i ) );
				if ( !objects->at( i )->resolveCollision() ) continue;
				Point<float> point = objects->at( i )->getPos();
				Rect<float> cbox = objects->at( i )->getCbox();
				normals snorm;
				snorm.selectNormals( 1, 1, 1, 1 );
				Vector2<float> normal =  snorm.getImpulseNorm( cbox, cbox2, m_velocity + ( objects->at( i )->m_velocity * -1 ) );
				float normalvel = dotProduct( m_velocity + ( objects->at( i )->m_velocity * -1 ), normal );


				if( normalvel > 0 )
				{

					addVelocity( normal * ( -normalvel * 0.5 - 0.66 ) );
					objects->at( i )->addVelocity( normal * ( normalvel * 0.5 + 0.66 ) );


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
	float tp = Globals::TIMESTEP * 0.1;
	float dxd = m_acceleration.m_p.m_x * tp;
	float dyd = m_acceleration.m_p.m_y * tp;
	m_velocity = m_velocity + Vector2<float>( dxd, dyd );

}

bool Object::isDead()
{
	return m_dead;
}

void Object::kill()
{
	m_dead = true;
}
errc Object::process()
{


	float dist = ( m_p.m_x - xoffset ) * ( m_p.m_x - xoffset ) + ( m_p.m_y - yoffset ) * ( m_p.m_y - yoffset );
	if( dist > Globals::REMOVEDISTANCE ) kill();

	if ( isDead() )
	{
		onDeath();
		return 0;
	}
	onProcess();
	processPhysics();
	countAcceleration();
	addAcceleration();

	applyVelocity();

	resolveBlockCollision();
	resolveObjectCollision();


	m_force = Vector2<float>( 0, 0 );
	m_acceleration = Vector2<float>( 0, 0 );


	return 0;
}

const unsigned& Object::getCVPos()
{
	return m_cvpos;
}

void Object::setCVPos( unsigned pos )
{
	m_cvpos = pos;
}

void Block::onCollision( Object* obj )
{

}

void Block::onCollision( Block* block )
{

}
void Object::setCParent( ObjectHolder* cparent )
{
	m_cparent = cparent;
}

ObjectHolder* Object::getCVparent()
{
	return m_cparent;
}


void Object::onProcess()
{

}

void Object::onSpawn()
{

}

void Object::onDeath()
{
	delete this;
}

void Object::onDamage()
{
	if( m_health <= 0 ) kill();
}

void Object::onCollision( Block* block )
{

}

void Object::onCollision( Object* object )
{

}

bool Object::use()
{
	return false;
}

void Object::damage( float dmg )
{
	m_health -= dmg;
	if( m_health <= 0 ) kill();
	onDamage();
}
extern int xoffset2;
extern int yoffset2;

errc Object::draw( UQVA* uqva )
{

	uqva->addQuad( m_p.m_x + m_cbox.getCorner( BOTLEFT ).m_x, m_p.m_y  + m_cbox.getCorner( BOTLEFT ).m_y, m_cbox.getCorner( TOPRIGHT ).m_x - m_cbox.getCorner( BOTLEFT ).m_x,
				   m_cbox.getCorner( TOPRIGHT ).m_y - m_cbox.getCorner( BOTLEFT ).m_y , m_texture.m_x, m_texture.m_y );
	uqva->changeColor( sf::Color( 0, 255, 100 ), 4 );


	return 0;
}
void Block::kill()
{

	m_dead = true;
}

Block::Block( int x, int y, BlockHolder* parent )
{
	m_itemid = 0;
	m_health = 100;
	m_liquidc = 0;
	m_dead = 0;
	m_vecpos = -1;
	m_parent = parent;

	tw = 0;
	m_x = x;
	m_y = y;
	m_phase = GAS;

}

Block::~Block()
{
	if( m_parent ) if( getVecPos() != -1 ) m_parent->removeFromHolder( getVecPos() );
}

void Block::onProcess()
{

}

void Block::onSpawn()
{

}

void Block::onDeath()
{
	if( m_phase == SOLID )
	{
		auto lambda = []( Point<float>& point, float angle, float tp )
		{
			point.m_x += cosf( angle ) * 3;
			point.m_y += sinf( angle ) * 3;
		} ;
		auto clambda = []( sf::Color&, float, float ) { };
		Object* obj = new ProjectileEmitter( getPos().m_x * Globals::BLOCKSIZE, getPos().m_y * Globals::BLOCKSIZE, sf::Color( 255, 150, 150 ), lambda, clambda );
		Item* item = new BlockPlacer( m_itemid, m_itemtexture, m_texture );
		ItemPickUp* ip = new ItemPickUp( m_x * Globals::BLOCKSIZE, m_y * Globals::BLOCKSIZE, m_itemtexture, item, Globals::GAME->obj_vec );
		Globals::GAME->obj_vec->add( ip );
		Globals::GAME->obj_vec->add( obj );
	}
	setPhase( GAS );

	m_texture = Point<int>();
	m_dead = false;
}

void Block::onDamage()
{

}



bool Block::use()
{
	return false;
}

void Block::damage( float dmg )
{
	m_health -= dmg;
	if( m_health <= 0 ) kill();
	onDamage();
}

errc Block::draw( UQVA* uqva, int xoffsett, int yoffsett, int bs )
{
	if( m_phase == NONE ) return -1;
	m_color = sf::Color( 255, 255, 255 );
	if( m_phase == LIQUID ) m_color = sf::Color( 111, 111, m_liquidc / 4 );
	if( m_phase == SOLID ) m_color = sf::Color( 200, 150, 100 );
	sf::RectangleShape rect( sf::Vector2f( bs, bs ) );
	uqva->addQuad( ( m_x )*bs + xoffsett, ( m_y )*bs + yoffsett, bs, bs, m_texture.m_x, m_texture.m_y );

	uqva->changeColor( m_color, 4 );

	return 0;
}
bool Block::isDead()
{
	return m_dead;
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
	if( isDead() ) onDeath();
	onProcess();
	Block* blk;
	blk = m_parent->getBlock( m_x, m_y + 1 );
	if( blk )
	{
		if( blk->getPhase() != SOLID  )
		{
			float diff = m_liquidc - blk->m_liquidc;
			diff = diff * 0.333;
			if( diff > 0 )
			{


				m_liquidc -= ( diff );
				if( m_liquidc < 1000 ) m_liquidc -= 0.1;
				blk->m_liquidc += diff;
			}
		}
		else
		{
			blk = m_parent->getBlock( m_x + 1, m_y );
			if( blk )
				if( blk->getPhase() != SOLID )
				{
					float diff = m_liquidc - blk->m_liquidc;
					diff -= diff * 0.123 / 2;
					if( diff > 0 )
					{


						m_liquidc -= diff;
						blk->m_liquidc += diff;
					}
				}
			blk = m_parent->getBlock( m_x - 1, m_y );
			if( blk )
				if( blk->getPhase() != SOLID )
				{
					float diff = m_liquidc - blk->m_liquidc;
					diff -= diff * 0.123 / 2;
					if( diff > 0 )
					{


						m_liquidc -= diff;
						blk->m_liquidc += diff;
					}
				}

		}
	}
	if( m_liquidc < 100 && m_phase != SOLID ) setPhase( GAS );
	if( m_liquidc > 100 && m_phase != SOLID ) setPhase( LIQUID );
	//std::cout << liquidc;

	return 0;


}
void Object::heal( float dmg )
{
	m_health += dmg;
	if( m_health >= m_max_health ) m_health = m_max_health;
	onHeal();
}

void Object::onHeal()
{

}

void Block::setVecPos( unsigned i )
{
	m_vecpos = i;
}

const unsigned& Block::getVecPos()
{
	return m_vecpos;
}
void Object::pickUp( ItemPickUp* block, Item* obj )
{

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

void Block::setItemID( unsigned id )
{
	m_itemid = id;
}

void Block::setItemTexture( Point<int> texture )
{
	m_itemtexture = texture;
}
void Block::setTexture( Point<int> texture )
{
	m_texture = texture;
}
float Block::getLiquidCount()
{
	return m_liquidc;
}
void Block::setLiquidCount( float cnt )
{
	m_liquidc = cnt;
}
void World::saveWorld()
{
	if( Globals::GAME->world )
	{

		std::ofstream file;
		file.open( "save.dat" );
		unsigned x, y;
		x = Globals::GAME->world->m_x;
		y = Globals::GAME->world->m_y;
		for( int i = 0; i < x * y; i++ )
		{
			Block* blk = Globals::GAME->world->getBlock( i % y, i / x );
			if( blk->getPhase() == GAS )
				file << 0 << " ";
			else
				if( blk->getPhase() == LIQUID )
					file << 1 << " ";
				else
					file << 2 << " ";
		}
	}
}

void World::loadWorld()
{
	std::ifstream file;
	file.open( "save.dat" );
	if( !file.is_open() ) return;

	CollisionObjectVector* colvec = new CollisionObjectVector( Globals::GAME );
	ObjectVector* objvec = new ObjectVector( Globals::GAME );
	Hero* her = new Hero( 100, 100, objvec, 10, Rect<float>() );
	Object* obj = her;
	obj->setPos( Point<float>( -100, -100 ) );
	objvec->add( her );
	colvec->add( her );
	//Globals::GAME->movingworlds.push_back( her );
	WorldGenerator::selectSeed( "Witam pozdrawiam" );
	WorldGenerator::selectSize( 1000, 1000 );
	World* wrld = WorldGenerator::generateWorld();
	Object* tobj = Globals::GAME->object;
	ObjectVector* tobjv = Globals::GAME->obj_vec;
	World* tworld = Globals::GAME->world;
	CollisionObjectVector* tcolv = Globals::GAME->col_vec;

	Globals::GAME->world = 0;
	Globals::GAME->object = 0;
	Globals::GAME->obj_vec = 0;
	Globals::GAME->col_vec = 0;

	sf::sleep( sf::milliseconds( 10 ) );
	Globals::WPLS = true;
	while( !( Globals::PTR && Globals::RTR ) ) sf::sleep( sf::milliseconds( 1 ) );
	if( tworld ) delete tworld;
	//if( tcolv ) delete tcolv; @TODO ????
	if( tobjv ) delete tobjv;
	unsigned x, y;
	file >> x >> y;
	for( int i = 0; i < x * y; i++ )
	{
		Block* blk = Globals::GAME->world->getBlock( i % y, i / x );
		int in;
		file >> in;
		if( in == 0 )
			blk->setPhase( GAS );
		else
			if( in == 1 )
				blk->setPhase( LIQUID );
			else
				blk->setPhase( SOLID );
	}

	Globals::GAME->world = wrld;
	Globals::GAME->object = her;
	Globals::GAME->obj_vec = objvec;
	Globals::GAME->col_vec = colvec;
	Globals::GAME->movingworlds.clear();
	Globals::WPLS = false;
	Globals::TIMEACCUMULATOR = 0;
}


#endif // WORLD_CPP
