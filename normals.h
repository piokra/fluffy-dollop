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
	Vector2<float> getImpulseNorm( Rect<float> staticrect, Rect<float> penrect, Vector2<float> vel );
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

	static float getPenetration( Vector2<float> norm, Rect<float> staticrect, Rect<float> penrect, Vector2<float>vel );
};


#endif // __NORMALS_H__
