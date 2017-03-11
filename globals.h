
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <SFML/Graphics.hpp>
#include "taskqueue.h"
#include "interface.h"
#include <random>
#include <ctime>
class World;
class Block;
class Object;
class ObjectVector;
class MovingWorld;
class InterfaceWindow;
class Interfaces;
class CollisionObjectVector;
class TaskQueue;
class Hero;

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
	std::vector<Block*>* getCollision( Object* q, Rect<float> cbox );
	World* world;
	Hero* object;
	ObjectVector* obj_vec;
	CollisionObjectVector* col_vec;
	std::vector<MovingWorld*> movingworlds;
	TaskQueue taskqueue;
	Texts text;
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
	const static float BLOCKSIZE;
	static Game* GAME;
	static Object* TO;
	static Interfaces INTERFACES;
	static bool PTR;
	static bool RTR;
	static bool WPLS;
	static bool GAMEPAUSED;
	static unsigned SCREENHALFW;
	static unsigned SCREENHALFH;
	static int MOUSEX;
	static int MOUSEY;
	static std::mt19937 RANDOMGENERATOR;
	const static float REMOVEDISTANCE;
	static sf::Texture* TEXTURE0;
};



#endif // __GLOBALS_H__
