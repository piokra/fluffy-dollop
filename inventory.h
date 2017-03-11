#ifndef INVENTORY_H
#define INVENTORY_H

#include "items.h"



class UQVA;
typedef unsigned char errc;

class Inventory : public ItemVector
{
public:
	Inventory();
	virtual ~Inventory();

	errc draw( UQVA* uqva );
	void setActivateSlot( unsigned slot );
	virtual void use( unsigned x, unsigned y );
	Item* const getActiveItem();


protected:
private:
	unsigned m_selected;

};

#endif // INVENTORY_H
