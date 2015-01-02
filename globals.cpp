#include "globals.h"
#include "bg.h"
#include "world.h"
#include "movingworld.h"
std::vector<Block*>* Game::getCollision( Object* q, Rect<double> cbox )
{
	std::vector<Block*>* t_bl = world->getCollision( cbox );
	for( unsigned i = 0; i < movingworlds.size(); i++ )
		movingworlds.at( i )->getCollision( q, t_bl, cbox );
	return t_bl;
}



const sf::Uint32 Globals::TIMESTEP = 1;
sf::Uint32 Globals::TIMEACCUMULATOR = 0;
const double Globals::BLOCKSIZE = 32;
Game* Globals::GAME = new Game;
Object* Globals::TO = 0;

int xoffset = 0;
int yoffset = 0;

int mx = 0;
int my = 0;
