#ifndef TEST_OBJ_H
#define TEST_OBJ_H


#include "world.h"

class TestObj : public Object
{
public:

	TestObj( int x, int y, ObjectHolder* parent, float mass, Rect<float> cbox ) : Object( x, y, parent, mass, cbox ) { };
	virtual ~TestObj() { };

	virtual void onCollision( Block* block )
	{
		if ( block ) if( block->getID() ) kill();
	}
	virtual void onCollision( Object* object )
	{
		if ( object ) if( object->getID() ) kill();
	}
protected:
private:
};


#endif // TEST_OBJ_H

