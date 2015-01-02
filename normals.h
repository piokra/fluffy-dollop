#ifndef __NORMALS_H__
#define __NORMALS_H__

#include "bg.h"
#include <SFML/Graphics.hpp>


struct normals
{

	normals()
	{
		m_norms = 0;
	};
	normals( bool left, bool right, bool bot, bool top )
	{
		selectNormals( left, right, bot, top );
	}
	Vector2<double> getImpulseNorm( Rect<double> staticrect, Rect<double> penrect, Vector2<double> vel );
	void selectNormals( bool left, bool right, bool bot, bool top )
	{
		m_norms = 0;
		if( left ) m_norms += 1;
		if( right ) m_norms += 2;
		if( top ) m_norms += 4;
		if( bot ) m_norms += 8;
	}
private:
	char m_norms;

};
class normal
{
public:

	static double getPenetration( Vector2<double> norm, Rect<double> staticrect, Rect<double> penrect, Vector2<double>vel );
};


#endif // __NORMALS_H__
