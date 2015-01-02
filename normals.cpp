#include "normals.h"



Vector2<double> normals::getImpulseNorm( Rect<double> staticrect, Rect<double> penrect, Vector2<double> vel )
{
	double normin = 0;
	double curnorm = 0;
	Vector2<double> retnorm( 0, 0 );

	if( m_norms & 1 )
	{
		curnorm = normal::getPenetration( Vector2<double>( -1, 0 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<double>( -1, 0 );
		}

	}
	if( m_norms & 2 )
	{

		curnorm = normal::getPenetration( Vector2<double>( 1, 0 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<double>( 1, 0 );
		}

	}
	if( m_norms & 4 )
	{

		curnorm = normal::getPenetration( Vector2<double>( 0, -1 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{
			normin = curnorm;
			retnorm = Vector2<double>( 0, -1 );
		}

	}
	if( m_norms & 8 )
	{

		curnorm = normal::getPenetration( Vector2<double>( 0, 1 ), staticrect, penrect, vel );
		if( curnorm < normin )
		{

			normin = curnorm;
			retnorm = Vector2<double>( 0, 1 );
		}

	}

	return retnorm;
}

double normal::getPenetration( Vector2<double> norm, Rect<double> staticrect, Rect<double> penrect, Vector2<double>vel )
{
	double penetration;

	if( norm.m_p.m_x == 0 )
	{
		//y norm
		if( norm.m_p.m_y == 1 )
		{
			//down
			penetration = penrect.getCorner( BOTLEFT ).m_y - staticrect.getCorner( TOPRIGHT ).m_y;

		}
		else
		{
			//up
			penetration = penrect.getCorner( TOPRIGHT ).m_y - staticrect.getCorner( BOTLEFT ).m_y;
			penetration = -penetration;
		}
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
		//penetration*=abs(vel.m_p.m_x);
	}
	return penetration;
}
