
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <SFML/Graphics.hpp>

class World;
class Block;
class Object;
class ObjectVector;
class MovingWorld;
template <class T> class Rect;


class Game
{
public:
	Game()
	{
		world = 0;
		object = 0;
		obj_vec = 0;
	};
	~Game()
	{

	};
	std::vector<Block*>* getCollision( Object* q, Rect<double> cbox );
	World* world;
	Object* object;
	ObjectVector* obj_vec;
	std::vector<MovingWorld*> movingworlds;
protected:
private:
};

enum GameState
{
	GS_None,
	GS_PlayLoop,
	GS_Mainmenu

};

class Globals
{
public:

	const static sf::Uint32  TIMESTEP;
	static sf::Uint32 TIMEACCUMULATOR;
	const static double BLOCKSIZE;
	static Game* GAME;
	static Object* TO;
};



#endif // __GLOBALS_H__
