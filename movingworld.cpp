#include "world.h"
#include "movingworld.h"
#include "globals.h"
#include "taskqueue.h"
#include "itempickup.h"
#include "cthulu.h"
extern int xoffset2;
extern int yoffset2;

class Inventory;

template<> SquareGrowingOutwards<Block*>::~SquareGrowingOutwards()
{


	for( unsigned i = 0; i < getNEx(); i++ )
	{
		for( unsigned j = 0; j < getNEy(); j++ )
		{
			if( m_NE.at( i ).at( j ) != 0 ) delete m_NE.at( i ).at( j );
		}
	}
	for( unsigned i = 0; i < getSEx(); i++ )
	{
		for( unsigned j = 0; j < getSEy(); j++ )
		{
			if( m_SE.at( i ).at( j ) != 0 ) delete m_SE.at( i ).at( j );
		}
	}
	for( unsigned i = 0; i < getSWx(); i++ )
	{
		for( unsigned j = 0; j < getSWy(); j++ )
		{
			if( m_SW.at( i ).at( j ) != 0 ) delete m_SW.at( i ).at( j );
		}
	}
	for( unsigned i = 0; i < getNWx(); i++ )
	{
		for( unsigned j = 0; j < getNWy(); j++ )
		{
			if( m_NW.at( i ).at( j ) != 0 ) delete m_NW.at( i ).at( j );
		}
	}
}


MovingWorld::MovingWorld( int x, int y, Game* parent, float mass, Rect<float>cbox ) : BlockHolder(), Object( x, y, parent->obj_vec, mass, cbox )
{
	//ctor

}

MovingWorld::~MovingWorld()
{

	//dtor
	return;
	unsigned pos;
	for( unsigned i = 0; i < Globals::GAME->movingworlds.size(); i++ )
	{
		if( Globals::GAME->movingworlds.at( i ) == this )
		{
			pos = i;
			break;
		}
	}
	Globals::GAME->movingworlds[pos] = Globals::GAME->movingworlds[Globals::GAME->movingworlds.size() - 1];
	Globals::GAME->movingworlds.pop_back();

}
errc MovingWorld::process()
{

	Object::process();

}
Block* MovingWorld::getBlock( int x, int y )
{
	return m_sgo.getXY( x, y );
}

Vector2<float> ShiftedBlock::getVelocity()
{
	return m_mw->getVelocity();
}

void ShiftedBlock::addVelocity( Vector2<float> vel )
{

	m_mw->addVelocity( vel );
}


std::vector<Block*>* MovingWorld::getCollision( Object* q, std::vector<Block*>* t_bl, Rect<float> cbox )
{

	if ( q == this ) return t_bl;
	Point<float> cboxp = getPos();
	cbox = cbox + ( cboxp * -1 );


	float bs = Globals::BLOCKSIZE;
	float xmin = cbox.getCorner( BOTLEFT ).m_x / bs;
	float ymin = cbox.getCorner( BOTLEFT ).m_y / bs;

	float xmax = cbox.getCorner( TOPRIGHT ).m_x;
	float ymax = cbox.getCorner( TOPRIGHT ).m_y;


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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
void Hero::onDeath()
{
    Globals::GAME->text.addText("You are dead.", Globals::SCREENHALFW-200,Globals::SCREENHALFH-100,24,1000,sf::Color(255,50,50));
	Globals::GAMEPAUSED = true;
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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

SquareGrowingOutwards<Block*>& MovingWorld::getSGO()
{
	return m_sgo;
}
Rect<float> MovingWorld::getCbox()
{
	return Rect<float>();
}


errc MovingWorld::draw( UQVA* window )
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
	std::cout << " hello \n";

	for( int i = 0; i < nex; i++ )
	{
		for( int j = 0; j < ney; j++ )
		{
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
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
			Point<float> pos = getPos();
			Block* t = m_sgo.getXY( -i - 1, -j - 1 );
			if( t != 0 )
			{
				t->draw( window, pos.m_x - xoffset, pos.m_y - yoffset, Globals::BLOCKSIZE );

			}
		}
	}


	return 0;
}

Rect<float> ShiftedBlock::getCbox()
{
	return Block::getCbox() + m_mw->getPos();
}
unsigned MovingWorld::getID()
{
	return 1;
}

unsigned ShiftedBlock::getID()
{
	return 1;
}
errc Hero::draw( UQVA* uqva )
{
	Point<float> p = getPos();
	xoffset2 = xoffset2 + ( p.m_x - Globals::SCREENHALFW );
	xoffset2 /= 2;
	yoffset2 = yoffset2 + ( p.m_y - Globals::SCREENHALFH );
	yoffset2 /= 2;

}

errc Hero::customDraw( UQVA* uqva )
{
	if( !Globals::GAME->world ) return -1;
	m_dphase++;
	m_dphase %= 40;
	if( m_animation_phase != m_animation_phase_next )
		m_animation_oount--;

	float angle1 = atan2( ( float )Globals::MOUSEY - ( ( float )Globals::SCREENHALFH - 21.f ), ( float )Globals::MOUSEX - ( ( float )Globals::SCREENHALFW - 8.f ) );
	float angle2 = 3 * 3.1415 / 2 + atan2( ( float )Globals::MOUSEY - ( ( float )Globals::SCREENHALFH - 21.f ), ( float )Globals::MOUSEX - ( ( float )Globals::SCREENHALFW + 18.f ) );
	float angle_head = 0;
	float angle_lleg = 0;
	float angle_rleg = 0;
	if( m_animation_phase == AP_RUNNING )
	{
		Vector2<float> vel = getVelocity();
		if( vel.m_p.m_x > 0 )
		{
			angle_head = -0.05;
			angle_rleg = ( float )( m_dphase % 3 - 1 ) * ( 0.15 );
			angle_lleg = -( float )( m_dphase % 3 - 1 ) * ( 0.15 );
		}
		else
		{
			angle_head = 0.05;
			angle_rleg = -( float )( m_dphase % 3 - 1 ) * ( 0.15 );
			angle_lleg = ( float )( m_dphase % 3 - 1 ) * ( 0.15 );
		}
		if( abs( vel.m_p.m_x ) < 0.1 )
		{
			angle_head = 0;
			angle_lleg = 0;
			angle_rleg = 0;
		}
	}


	//std::cout << (float)Globals::MOUSEX-((float)Globals::SCREENHALFW-8.f) << " " << (float)Globals::MOUSEY-((float)Globals::SCREENHALFH-21.f) << " " << angle1/3.1415*180 << std::endl;
	//TORSO

	uqva->addQuad( Globals::SCREENHALFW, Globals::SCREENHALFH, 17, 35, 69, 37 );
	//HEAD
	uqva->addQuad( Globals::SCREENHALFW - 2, Globals::SCREENHALFH - 21, 22, 17, 67, 19, 0, 0, angle_head );
	//LEFT ARM
	uqva->addQuad( Globals::SCREENHALFW + 18, Globals::SCREENHALFH, 7, 37, 88, 42, 0, 0, angle2 );


	//HELD ITEM
	//uqva->addQuad(Globals::SCREENHALFW+18+28*cos(angle1),Globals::SCREENHALFH+28*sin(angle1),20,20,100,2,1,0,0);
	//RIGHT ARM
	angle1 += 3 * 3.1415 / 2;
	uqva->addQuad( Globals::SCREENHALFW - 8, Globals::SCREENHALFH, 7, 37, 60, 42, 0, 0, angle1 );
	//LEFT LEG

	uqva->addQuad( Globals::SCREENHALFW + 1, Globals::SCREENHALFH + 39, 7, 23, 67, 77, 0, 0, angle_lleg );


	//RIGHT LEG
	uqva->addQuad( Globals::SCREENHALFW + 8, Globals::SCREENHALFH + 39, 7, 23, 81, 77, 0, 0, angle_rleg );

	if( !m_animation_oount ) m_animation_phase = m_animation_phase_next;
	m_inventory.draw( uqva );
}


void Hero::onProcess()
{
    auto dist = std::uniform_real_distribution<float>(0,100);
    auto rng = std::bind(dist,Globals::RANDOMGENERATOR);
    if(rng()<1)
    {
        std::cout << "cthulu spawned\n" << std::endl;
        Object* cthulu = new Cthulu(getPos().m_x-1500,getPos().m_y-1500);
        Globals::GAME->obj_vec->add(cthulu);
    }
	m_cooldown -= Globals::TIMESTEP;
	if( m_cooldown <= 0 )
		if( m_using ) taskUse();
	processTasks();
}

void Hero::processTasks()
{
	Task task;
	while( Globals::GAME->taskqueue.getTask( task ) )
	{


		if( task.tag == "move" )
		{


			unsigned i;
			task.arguments >> i;
			taskMove( i );
		}
		else
			if( task.tag == "unmove" )
			{


				unsigned i;
				task.arguments >> i;
				taskUnmove( i );

			}
			else
				if( task.tag == "jump" )
				{
					taskJump();
				}
				else
					if( task.tag == "select_slot" )
					{
						unsigned i;
						task.arguments >> i;
						taskSelectSlot( i );
					}
					else
						if( task.tag == "use" )
						{
							m_using = true;
							taskUse();
						}
						else
							if( task.tag == "unuse" )
							{
								taskUnuse();
							}
	}
}

void Hero::taskMove( unsigned i )
{
	m_animation_oount = 10;
	m_animation_phase_next = AP_RUNNING;
	if( i == 0 )
	{
		m_left = 1;
	}
	else
		if( i == 1 )
		{
			m_right = 1;
		}
		else
			if( i == 2 )
			{
				m_up = 1;
			}
			else
				if( i == 3 )
				{
					m_down = 1;
				}

}

void Hero::taskUnmove( unsigned i )
{
	m_animation_phase_next = AP_IDLE;
	if( i == 0 )
	{
		m_left = 0;
	}
	else
		if( i == 1 )
		{
			m_right = 0;
		}
		else
			if( i == 2 )
			{
				m_up = 0;
			}
			else
				if( i == 3 )
				{
					m_down = 0;
				}
}
void Hero::taskJump()
{
	m_animation_oount = 10;
	m_animation_phase_next = AP_JUMPING;
	if( m_jumpcount )
	{

		addVelocity( Vector2<float>( 0, -30 ) );
		m_jumpcount--;
	}
}


void Hero::onCollision( Block* block )
{
	Point<float> pos = block->getPos();
	if( pos.m_y * Globals::BLOCKSIZE > getPos().m_y ) m_jumpcount = 2;
}


Hero::Hero( int x, int y, ObjectHolder* parent, float mass, Rect<float> bounds ): Object( x, y, parent, mass, Rect<float>( Point<float>( -8, -21 ), Point<float>( 27, 59 ) ) )
{
	m_using = false;
	m_animation_phase = AP_IDLE;
	m_animation_phase_next = AP_IDLE;
	m_animation_oount = 0;
	m_left = false;
	m_right = false;
	m_up = false;
	m_down = false;
	m_dphase = 0;
}
void Hero::taskSelectSlot( unsigned i )
{
	if( i != 0 )
		m_inventory.setActivateSlot( i - 1 );
	else
		m_inventory.setActivateSlot( 9 );
}
void Hero::pickUp( ItemPickUp* pickup, Item* item )
{

	m_inventory.addItem( item );
	pickup->kill();

}
void Hero::taskUse()
{
	if( m_inventory.getActiveItem() )
		m_cooldown = m_inventory.getActiveItem()->getCooldown();
	m_inventory.use( Globals::MOUSEX , Globals::MOUSEY );
}
void Hero::taskUnuse()
{
	m_using = false;
}
Inventory& Hero::getInventory()
{
	return m_inventory;
}
