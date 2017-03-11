#include "idtoitemconversion.h"
#include "items.h"
#include "globals.h"
#include <random>
#include <functional>
Item* IDtoItemConversion::createItemFromID( unsigned ID )
{
	std::uniform_real_distribution<float> dist( -0.3, 0.3 );
	auto rng = std::bind( dist, Globals::RANDOMGENERATOR );
	if( ID < 1000 )
	{

		//ITEM TYPE
	}
	else
		if( ID < 10000 )
		{
			ID -= 1000;
			if( ID == 0 )
				return new BlockPlacer( 0, Point<int>( 0, 0 ), Point<int>( 0, 0 ) );
			//BLOCK TYPE
		}
		else
			if( ID < 20000 )
			{

				ID -= 10000;
				if( ID == 0 )
					return new MeleeWeapon( 0, Point<int>( 101, 2 ), 300 + 300 * rng(), 33, 100, 1 + 1 * rng(), 1, 0.1 );

				//MELEE
			}
			else
				if( ID < 30000 )
				{
					ID -= 20000;
					if( ID == 0 )
						return new RocketLauncher( 0, 300 + 300 * rng(), 1000, 1500, 1, 3, 1 );
					//LAUNCHER
				}
				else
					if( ID < 40000 )
					{
						ID -= 30000;
						if( ID == 0 )
							return new Rifle( 0, 300 + 300 * rng(), 1000, 500, 1, 3, 1 );

					}
}

IDtoItemConversion::IDtoItemConversion()
{
	//ctor
}

IDtoItemConversion::~IDtoItemConversion()
{
	//dtor
}
