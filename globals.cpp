#include "globals.h"
#include "bg.h"
#include "world.h"
#include "movingworld.h"
#include "interfacewindow.h"
#include "interface.h"
std::vector<Block*>* Game::getCollision( Object* q, Rect<float> cbox )
{
	std::vector<Block*>* t_bl = world->getCollision( cbox );
	for( unsigned i = 0; i < movingworlds.size(); i++ )
		movingworlds.at( i )->getCollision( q, t_bl, cbox );
	return t_bl;
}

bool Globals::GAMEPAUSED = false;

const sf::Uint32 Globals::TIMESTEP = 4;
sf::Uint32 Globals::TIMEACCUMULATOR = 0;
const float Globals::BLOCKSIZE = 24;
const float Globals::REMOVEDISTANCE = 90000000;
Game* Globals::GAME = new Game;
Object* Globals::TO = 0;
bool Globals::PTR = false;
bool Globals::RTR = false;
bool Globals::WPLS = false;
unsigned Globals::SCREENHALFH = 0;
unsigned Globals::SCREENHALFW = 0;
int Globals::MOUSEX = 0;
int Globals::MOUSEY = 0;
std::mt19937 Globals::RANDOMGENERATOR = std::mt19937( time( 0 ) );
Interfaces Globals::INTERFACES;
sf::Texture* Globals::TEXTURE0 = 0;
int xoffset = 0;
int yoffset = 0;
extern int xoffset;
extern int yoffset;
extern int xoffset2;
extern int yoffset2;

int mx = 0;
int my = 0;
