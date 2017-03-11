#ifndef CTHULU_H
#define CTHULU_H

#include "movingworld.h"

class Cthulu : public MovingWorld
{
public:
	Cthulu( float x, float y );
	virtual ~Cthulu();
	virtual errc draw( UQVA* uqva );
protected:
	void flipCollision();
	virtual void onCollision( Block* block );
	virtual void onCollision( Object* object );
	virtual void onProcess();
private:
};

#endif // CTHULU_H
