

#ifndef VER_DAT_H
#define VER_DAT_H

#include "version.h"
#include <time.h>
#include <stdio.h>




namespace ver_dat
{



int dump_ver()
{
	time_t vtime = time( NULL );
	struct tm* ztime = gmtime( &vtime );

	FILE* file = fopen( "ver.dat", "w" );
	if( file == NULL ) return -1;
#ifdef VERSION_H

	fprintf( file, "%s-%s-%s\n", AutoVersion::MONTH, AutoVersion::DATE, AutoVersion::YEAR );

	fclose( file );
#endif

	fprintf( file, "%i-%i-%i\n", ztime->tm_mon, ztime->tm_mday, 1900 + ztime->tm_year );



	fclose( file );
	return 0;

}

}


#endif // VER_DAT_H
