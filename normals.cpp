#include "normals.h"



Vector2<float> normals::getImpulseNorm( Rect<float> staticrect, Rect<float> penrect, Vector2<float> vel )
{
	float normin = 0;
	float curnorm = 0;
	Vector2<float> retnorm( 0, 0 );

	if( m_norms & 1 )
	{
		curnorm = normal::getPenetration( Vector2<float>( -1, 0 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<float>( -1, 0 );
		}

	}
	if( m_norms & 2 )
	{

		curnorm = normal::getPenetration( Vector2<float>( 1, 0 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<float>( 1, 0 );
		}

	}
	if( m_norms & 4 )
	{

		curnorm = normal::getPenetration( Vector2<float>( 0, -1 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<float>( 0, -1 );
		}

	}
	if( m_norms & 8 )
	{

		curnorm = normal::getPenetration( Vector2<float>( 0, 1 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{

			normin = curnorm;
			retnorm = Vector2<float>( 0, 1 );
		}

	}

	return retnorm;
}

float normal::getPenetration( Vector2<float> norm, Rect<float> staticrect, Rect<float> penrect, Vector2<float>vel )
{
	float penetration;

	if( norm.m_p.m_x == 0 )
	{
		//y norm
		if( norm.m_p.m_y == 1 )
		{
			//down
			penetration = ( penrect.getCorner( BOTLEFT ).m_y - staticrect.getCorner( TOPRIGHT ).m_y );


		}
		else
		{
			//up
			penetration = penrect.getCorner( TOPRIGHT ).m_y - staticrect.getCorner( BOTLEFT ).m_y;
			penetration = -penetration;
		}
		penetration /= penrect.getCorner( TOPRIGHT ).m_y - penrect.getCorner( BOTLEFT ).m_y;
		//penetration*=vel.m_p.m_y;
	}
	else
	{
		//x norm
		if( norm.m_p.m_x == 1 )
		{
			//right
			penetration = penrect.getCorner( BOTLEFT ).m_x - staticrect.getCorner( TOPRIGHT ).m_x;
		}
		else
		{
			//left
			penetration = penrect.getCorner( TOPRIGHT ).m_x - staticrect.getCorner( BOTLEFT ).m_x;
			penetration = -penetration;
		}
		penetration /= penrect.getCorner( TOPRIGHT ).m_x - penrect.getCorner( BOTLEFT ).m_x;
		//penetration*=abs(vel.m_p.m_x);
	}
	return penetration;
}
