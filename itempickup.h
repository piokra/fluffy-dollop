#ifndef ITEMPICKUP_H
#define ITEMPICKUP_H

#include "world.h"


class ItemPickUp : public Object
{
public:
	ItemPickUp( float x, float y, Point<int> texture, Item* item, ObjectHolder* parent );
	virtual ~ItemPickUp();

	const Point<int>& getTexture();
protected:

	virtual bool resolveCollision();
	virtual void onCollision( Object* obj );
private:
	Item* m_item;
	unsigned m_id;
	unsigned m_stackcount;
	Point<int> m_texture;
};

#endif // ITEMPICKUP_H
