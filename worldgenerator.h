#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H
#include <string>


class World;

class WorldGenerator
{
public:
	static void selectSize( int x, int y );
	static void selectMaximumWaterDepth( int depth );
	static void selectAverageSolidRectSize( int x, int y );
	static void selectAverageEmptyRectSize( int x, int y );
	static void selectSeed( std::string seed );

	static World* generateWorld();
protected:
	static void resetSettings();
	static void generateSolidBlocks();

	static void generateSolidBlock( int x, int y );
	static void generateGasBlock( int x, int y );

	static void fillWithLiquid( int x, int y );

	static void polish();

	static void getGasToSolid( int x, int y, int& sc, int& ac );

	static void generateBlock( int type, int x, int y, int xsize, int ysize );

	static bool isLiquidTight( int x, int y, int depth );


private:
	static int m_x;
	static int m_y;
	static int m_mwd;
	static int m_asrs_x;
	static int m_asrs_y;
	static int m_aers_x;
	static int m_aers_y;
	static std::string m_seed;
	static World* m_world;
	static char** m_visited;
	static char m_visitedc;
};

#endif // WORLDGENERATOR_H
