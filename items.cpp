#include "items.h"
#include "UQVA.h"
#include "conversion.h"
#include "globals.h"
#include "world.h"
#include "projectile.h"
#include "movingworld.h"
Item::Item( unsigned stackcount, unsigned maxstack, unsigned id, Point<int> texturexy )
{

	m_holder = 0;
	m_stackcount = stackcount;
	m_maxstack = maxstack;
	m_id = id;
	m_texture = texturexy;

}

Item::~Item()
{
	if( m_holder )
		m_holder->removeItem( m_holder->getItemByPos( m_holderpos ) );
}


ItemHolder::ItemHolder()
{
	//ctor
}

ItemHolder::~ItemHolder()
{
	//dtor
}

ItemVector::ItemVector()
{

}

ItemVector::~ItemVector()
{

}
void ItemVector::addItem( Item* item )
{
	if( !item ) return;
	Item* t = getItemByID( item->getID() );
	if( !t || !item->isStackable() )
	{
		m_items.push_back( item );
		item->setParent( this );
		item->setHolderPos( m_items.size() - 1 );
	}
	else
	{
		if( item->getMaxStack() - t->getStackCount() <= item->getStackCount() )
			t->setStackCount( t->getMaxStack() );
		else
			t->setStackCount( t->getStackCount() + item->getStackCount() );
		delete item;
		item = 0;
	}
}

void ItemVector::removeItem( Item* item )
{
	unsigned pos = item->getHolderPos();
	if( m_items[pos] == item )
	{
		m_items[pos] = m_items[m_items.size() - 1];
		m_items[pos]->setHolderPos( pos );
		m_items.pop_back();
	}
}

Item* ItemVector::getItemByID( unsigned ID )
{
	for( int i = 0; i < m_items.size(); i++ )
		if( m_items[i]->getID() == ID ) return m_items[i];
}

Item* ItemVector::getItemByPos( unsigned pos )
{
	if( pos >= m_items.size() ) return 0;
	return m_items[pos];
}
void ItemVector::removeFromPos( unsigned pos )
{
	removeItem( getItemByPos( pos ) );
}


const unsigned& Item::getID()
{
	return m_id;
}

const unsigned& Item::getHolderPos()
{
	return m_holderpos;
}

void Item::setHolderPos( unsigned pos )
{
	m_holderpos = pos;
}

const bool Item::isStackable()
{
	return true;
}

const unsigned& Item::getMaxStack()
{
	return m_maxstack;
}

const unsigned& Item::getStackCount()
{
	return m_stackcount;
}

void Item::setStackCount( unsigned cnt )
{
	m_stackcount = cnt;
}
void Item::decreaseCount( unsigned cnt )
{
	m_stackcount -= cnt;
	if( m_stackcount <= 0 ) delete this;
}
const Point<int>& Item::getTextureXY()
{
	return m_texture;
}

void Item::onUse( unsigned x, unsigned y )
{

}

void Item::onDeath()
{

}

void Item::onPickUp()
{

}

void Item::onDrop()
{

}

void Item::onDraw( UQVA* uqva )
{

}



void Item::use( unsigned x, unsigned y )
{
	m_stackcount--;
	onUse( x, y );


	if( !m_stackcount ) delete this;
}

void Item::drop( unsigned x, unsigned y )
{

}
void Item::draw( UQVA* uqva, Point<int> pos )
{
	std::string ic = tostr( m_stackcount );
	uqva->addQuad( pos.m_x, pos.m_y, 24, 24, m_texture.m_x, m_texture.m_y );

	onDraw( uqva );
	uqva->addText( pos.m_x + ( 3 - ic.size() ) * 12, pos.m_y + 15, ic, 0 );
}
void Item::setParent( ItemHolder* holder )
{
	m_holder = holder;
}
BlockPlacer::BlockPlacer( unsigned ID, Point<int> texture, Point<int> blocktexture ) : Item( 1, 999, 1000 + ID, texture )
{
	m_blocktexture = blocktexture;
}
BlockPlacer::~BlockPlacer()
{

}
extern int xoffset;
extern int yoffset;
void BlockPlacer::onUse( unsigned x, unsigned y )
{

	float dist = 0;
	dist = ( x - Globals::SCREENHALFW ) * ( x - Globals::SCREENHALFW ) + ( y - Globals::SCREENHALFH ) * ( y - Globals::SCREENHALFH );
	std::cout << x << " " <<  y << std::endl;
	if( dist > 166 * 166 )
	{
		m_stackcount++;
		return;
	}
	float xx = x + xoffset;
	float yy = y + yoffset;

	xx /= Globals::BLOCKSIZE;
	yy /= Globals::BLOCKSIZE;

	Block* block = Globals::GAME->world->getBlock( xx, yy );
	if( block )
	{
		if( block->getPhase() == GAS )
		{
			block->setPhase( SOLID );
			block->setTexture( m_blocktexture );
		}
		else
		{
			m_stackcount++;
		}
	}
	else
	{
		m_stackcount++;
	}


}
MeleeWeapon::MeleeWeapon( unsigned ID, Point<int> texture, float range, float damage, float attack_delay, float attack_delay_multiplier, float blockmultiplier, float mobmultiplier ) : Item( 999, 999,
			ID + 10000, texture )
{
	m_damage = damage;
	m_range = range;
	m_blockmultiplier = blockmultiplier;
	m_mobmultiplier = mobmultiplier;
	m_attackdelay = attack_delay;
	m_attackdelaymultiplier = attack_delay_multiplier;
}

MeleeWeapon::~MeleeWeapon()
{

}


const bool MeleeWeapon::isStackable()
{
	return false;
}

void MeleeWeapon::onUse( unsigned x, unsigned y )
{
	float dist = 0;
	dist = ( x - Globals::SCREENHALFW ) * ( x - Globals::SCREENHALFW ) + ( y - Globals::SCREENHALFH ) * ( y - Globals::SCREENHALFH );
	//std::cout << x << " " <<  y << std::endl;
	if( dist > m_range * m_range )
	{
		m_stackcount++;
		return;
	}
	float xx = x + xoffset;
	float yy = y + yoffset;
	std::vector<Object*>* objs = Globals::GAME->col_vec->objectsInRange( Point<float>( xx, yy ), 15 );
	for( int i = 0; i < objs->size(); i++ )
		if( objs->at( i ) )
			objs->at( i )->damage( m_damage * m_mobmultiplier );



	xx /= Globals::BLOCKSIZE;
	yy /= Globals::BLOCKSIZE;

	Block* block = Globals::GAME->world->getBlock( xx, yy );
	if( block )
	{
		if( block->getPhase() == SOLID )
		{
			block->damage( m_damage * m_blockmultiplier );
		}
		else
		{
			m_stackcount++;
		}
	}
	else
	{
		m_stackcount++;
	}

}
float Item::getCooldown() const
{
	return 0.f;
}

float MeleeWeapon::getCooldown() const
{
	return m_attackdelay * m_attackdelaymultiplier;
}

int Item::getItemType()
{
	return 0;
}

void Item::copyItem( Item* item )
{
	m_holder = 0;
	m_holderpos = -1;
	m_id = item->m_id;
	m_stackcount = item->m_stackcount;
	m_maxstack = item->m_maxstack;
	m_texture = item->m_texture;
}
void BlockPlacer::copyItem( BlockPlacer* item )
{
	m_blocktexture = item->m_blocktexture;
	Item::copyItem( item );
}

int BlockPlacer::getItemType()
{
	return 1;
}
void MeleeWeapon::copyItem( MeleeWeapon* item )
{
	m_damage = item->m_damage;
	m_range = item->m_range;
	m_blockmultiplier = item->m_blockmultiplier;
	m_mobmultiplier = item->m_mobmultiplier;
	m_attackdelay = item->m_attackdelay;
	m_attackdelaymultiplier = item->m_attackdelaymultiplier;
	copyItem( item );
}
void MeleeWeapon::decreaseCount( unsigned cnt )
{
	delete this;
}

int MeleeWeapon::getItemType()
{
	return 2;
}
RocketLauncher::RocketLauncher( unsigned id, float damage, float lifetime, float blockmul, float objmul, float attackdelay, float attackdmul ) : Item( 999, 999, id + 20000, Point<int>( 151, 2 ) )
{
	m_damage = damage;
	m_lifetime = lifetime;
	m_blockmultiplier = blockmul;
	m_mobmultiplier = objmul;
	m_attackdelay = attackdelay;
	m_attackdelaymultiplier = attackdmul;
}

RocketLauncher::~RocketLauncher()
{

}

const bool RocketLauncher::isStackable()
{
	return false;
}

void RocketLauncher::onUse( unsigned x, unsigned y )
{
	Point<float> pos = Globals::GAME->object->getPos();
	float angle1 = atan2( ( float )Globals::MOUSEY - ( ( float )Globals::SCREENHALFH ), ( float )Globals::MOUSEX - ( ( float )Globals::SCREENHALFW  ) );
	Object* proj = new Projectile( pos.m_x + cos( angle1 ) * 100, pos.m_y + sin( angle1 ) * 100, m_damage, m_mobmultiplier, m_blockmultiplier, angle1, 30, 100000 );
	Globals::GAME->obj_vec->add( proj );
}

float RocketLauncher::getCooldown() const
{
	return m_attackdelay * m_attackdelaymultiplier;
}

Rifle::Rifle( unsigned id, float damage, float lifetime, float blockmul, float objmul, float attackdelay, float attackdmul ) : Item( 999, 999, id + 30000, Point<int>( 127, 2 ) )
{
	m_damage = damage;
	m_lifetime = lifetime;
	m_blockmultiplier = blockmul;
	m_mobmultiplier = objmul;
	m_attackdelay = attackdelay;
	m_attackdelaymultiplier = attackdmul;
}

Rifle::~Rifle()
{

}

const bool Rifle::isStackable()
{
	return false;
}

void Rifle::onUse( unsigned x, unsigned y )
{
	Point<float> pos = Globals::GAME->object->getPos();
	float angle1 = atan2( ( float )Globals::MOUSEY - ( ( float )Globals::SCREENHALFH ), ( float )Globals::MOUSEX - ( ( float )Globals::SCREENHALFW  ) );
	Object* proj = new Bullet( pos.m_x + 50 * cosf( angle1 ), pos.m_y + 50 * sinf( angle1 ), m_damage, m_mobmultiplier, m_blockmultiplier, angle1, 3, 1000 );
	Globals::GAME->obj_vec->add( proj );
}

float Rifle::getCooldown() const
{
	return m_attackdelay * m_attackdelaymultiplier;
}
