#include "worldgenerator.h"
#include "globals.h"
#include "world.h"
#include "conversion.h"


int WorldGenerator::m_x = 0;
int WorldGenerator::m_y = 0;
int WorldGenerator::m_mwd = 15;
int WorldGenerator::m_asrs_x = 10;
int WorldGenerator::m_aers_x = 7;
int WorldGenerator::m_asrs_y = 10;
int WorldGenerator::m_aers_y = 7;
World* WorldGenerator::m_world = 0;
char** WorldGenerator::m_visited = 0;
char WorldGenerator::m_visitedc = 0;
std::string WorldGenerator::m_seed;


void WorldGenerator::selectSize( int x, int y )
{
	m_x = x;
	m_y = y;
	if( m_visited )
	{
		for( int i = 0; i < m_x; i++ )
			free( m_visited[i] );
		free( m_visited );
	}
	m_visited = ( char** )malloc( sizeof( char* )*m_x );
	for( int i = 0; i < m_x; i++ )
		m_visited[i] = ( char* )malloc( sizeof( char ) * m_y );

}

void WorldGenerator::selectMaximumWaterDepth( int depth )
{
	m_mwd = depth;
}

void WorldGenerator::selectAverageSolidRectSize( int x, int y )
{
	m_asrs_x = x;
	m_asrs_y = y;
}

void WorldGenerator::selectAverageEmptyRectSize( int x, int y )
{
	m_aers_x = x;
	m_aers_y = y;
}

void WorldGenerator::selectSeed( std::string seed )
{

	m_seed = seed;

}

World* WorldGenerator::generateWorld()
{
	for( int i = 0; i < m_x; i++ )
		m_visited[i] = ( char* )malloc( sizeof( char ) * m_y );


	m_world = new World( Globals::GAME, m_x, m_y );
	std::uniform_real_distribution<double> dist( 0, 1 );
	int seedi = strtoint( m_seed );
	//std::cout << seedi << std::endl;
	std::mt19937 mt( seedi );
	unsigned fullblockcount = m_x * m_y / ( m_asrs_x * m_asrs_y ) * 1.2;
	int tsizex = m_asrs_x;
	int tsizey = m_asrs_y;
	while ( fullblockcount-- )
	{
		m_asrs_x = tsizex * ( dist( mt ) + 0.5 );
		m_asrs_y = tsizey * ( dist( mt ) + 0.5 );
		generateSolidBlock( ( dist( mt )*m_x ), ( dist( mt )*m_y ) );
	}
	polish();
	unsigned gasblockcount = m_x * m_y / ( tsizex * tsizey ) * 0.4;
	while ( gasblockcount-- )
	{
		m_aers_x = tsizex * ( dist( mt ) + 0.2 );
		m_aers_y = tsizey * ( dist( mt ) + 0.2 );
		generateGasBlock( ( dist( mt )*m_x ), ( dist( mt )*m_y ) );
	}
	unsigned polishc = 10;
	while( polishc-- )
		polish();
	unsigned waterblockcount = m_x * m_y / ( tsizex * tsizey ) * 3.1415;
	while ( waterblockcount-- )
	{
		int tx = dist( mt ) * m_x;
		int ty = dist( mt ) * m_y;
		m_visitedc++;
		m_visitedc = std::max( m_visitedc, ( char )1 );
		if( isLiquidTight( tx, ty, 0 ) )
		{
			fillWithLiquid( tx, ty );
		}
	}

	return m_world;

}
void WorldGenerator::generateSolidBlocks()
{
	// nothing do here
}

void WorldGenerator::generateSolidBlock( int x, int y )
{
	generateBlock( 0, x, y, m_asrs_x, m_asrs_x );

}
void WorldGenerator::generateGasBlock( int x, int y )
{
	generateBlock( 1, x, y, m_aers_x, m_aers_x );
}




void WorldGenerator::generateBlock( int type, int x, int y, int xsize, int ysize )
{
	for( int i = 0; i < xsize; i++ )
	{

		for( int j = 0; j < ysize; j++ )
		{
			Block* blk = m_world->getBlock( x + i, y + j );
			if ( blk )
			{
				if( type == 0 )
					blk->setPhase( SOLID );
				if( type == 1 )
					blk->setPhase( GAS );
				if( type == 2 )
					blk->setPhase( LIQUID );
			}
			else
				return;
		}

	}
}


void WorldGenerator::polish()
{
	for( int i = 0; i < m_x; i++ )
	{
		for( int j = 0; j < m_y; j++ )
		{
			int sc, ac;
			getGasToSolid( i, j, sc, ac );
			Block* blk = m_world->getBlock( i, j );
			if( blk )
			{
				if( ac > sc )
					blk->setPhase( GAS );
				if( sc == 5 )
					blk->setPhase( SOLID );
			}

		}

	}
}
void WorldGenerator::getGasToSolid( int x, int y, int& sc, int&ac )
{
	sc = 0;
	ac = 0;
	for( int i = -1; i < 2; i++ )
	{
		for( int j = -1; j < 2; j++ )
		{
			Block* blk = m_world->getBlock( x + i, y + j );
			if( blk )
			{
				if( blk->getPhase() == SOLID )
				{
					sc++;
					ac--;
				}
			}
			ac++;
		}
	}

}
void WorldGenerator::fillWithLiquid( int x, int y )
{
	Block* blk = m_world->getBlock( x, y );
	if( blk )
	{
		if( blk->getPhase() == GAS )
		{


			blk->setPhase( LIQUID );
			blk->setLiquidCount( 1000 );
			fillWithLiquid( x + 1, y );
			fillWithLiquid( x - 1, y );
			fillWithLiquid( x, y + 1 );

		}
	}

}

bool WorldGenerator::isLiquidTight( int x, int y, int depth )
{

	if( depth >= m_mwd ) return false;
	Block* blk = m_world->getBlock( x, y );
	if( !blk ) return true;
	if ( m_visited[x][y] == m_visitedc ) return true;
	m_visited[x][y] = m_visitedc;
	if ( blk->getPhase() == SOLID ) return true;
	if ( blk->getPhase() == LIQUID ) return true;
	return isLiquidTight( x + 1, y, depth + 1 ) && isLiquidTight( x - 1, y, depth + 1 ) && isLiquidTight( x, y + 1, depth + 1 );

}
void WorldGenerator::resetSettings()
{
	m_x = 0;
	m_y = 0;
}
