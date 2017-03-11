#include "itempickup.h"

ItemPickUp::ItemPickUp( float x, float y, Point<int> texture,  Item* item, ObjectHolder* parent ) : Object( x, y, parent, 1, Rect<float>( Point<float>( 0, 0 ), Point<float>( 16, 16 ) ) )
{
	setTexture( texture );
	m_item = item;
	//ctor
}

ItemPickUp::~ItemPickUp()
{
	//dtor
}
bool ItemPickUp::resolveCollision()
{
	return false;
}


const Point<int>& ItemPickUp::getTexture()
{
	return m_texture;
}
void ItemPickUp::onCollision( Object* obj )
{
	if( !isDead() )
		obj->pickUp( this, m_item );
}
