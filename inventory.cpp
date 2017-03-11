#include "inventory.h"
#include "UQVA.h"
Inventory::Inventory()
{
	m_selected = 0;
	//ctor
}

Inventory::~Inventory()
{
	//dtor
}
errc Inventory::draw( UQVA* uqva )
{
	Item* item;
	for( int i = 0; i < 10; i++ )
	{
		uqva->addQuad( 10 + i * 70, 10, 50, 50, 0, 26 );
		if( m_selected != i )
			uqva->changeColor( sf::Color( 255, 255, 255, 100 ), 4 );
		else
			uqva->changeColor( sf::Color( 155, 255, 155, 100 ), 4 );
		item = getItemByPos( i );
		if( item )

			item->draw( uqva, Point<int>( 20 + i * 70, 20 ) );

	}
}

void Inventory::setActivateSlot( unsigned slot )
{
	m_selected = slot;
}
void Inventory::use( unsigned x, unsigned y )
{
	Item* item = getItemByPos( m_selected );
	if( item )
		item->use( x, y );
}
Item* const Inventory::getActiveItem()
{
	return getItemByPos( m_selected );
}
