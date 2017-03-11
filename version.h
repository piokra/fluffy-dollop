#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] =  "15.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 1;
	static const long BUILD  = 2301;
	static const long REVISION  = 12697;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4175;
	#define RC_FILEVERSION 3,1,2301,12697
	#define RC_FILEVERSION_STRING "3, 1, 2301, 12697\0"
	static const char FULLVERSION_STRING [] = "3.1.2301.12697";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 1;
	

}
#endif //VERSION_H
