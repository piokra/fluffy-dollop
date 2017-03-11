#include "uqva.h"
#include <iostream>
void UQVA::addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset )
{
	m_vertices.resize( m_vertices.getVertexCount() + 4 );

	m_vertices[vertice_count].position = sf::Vector2f( x + 0, 0 + y );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0, texture_y + 0 );
	vertice_count++;
	m_vertices[vertice_count].position = sf::Vector2f( x + x_size, 0 + y );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0 + x_size, texture_y + 0 );
	vertice_count++;
	m_vertices[vertice_count].position = sf::Vector2f( x + x_size + x_offset, y_size + y_offset + y );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_size + x_offset, y_size + y_offset + texture_y );
	vertice_count++;
	m_vertices[vertice_count].position = sf::Vector2f( x + x_offset, y_size + y_offset + y );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_offset, y_size + y_offset + texture_y );
	vertice_count++;
}

void UQVA::changeColor( sf::Color color, int32_t count )
{
	for( int i = 0; i < std::min( count, vertice_count ); i++ )
	{
		m_vertices[vertice_count - i - 1].color = color;
	}
}

void UQVA::addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset, float rotation_angle_radian )
{

	m_vertices.resize( m_vertices.getVertexCount() + 4 );

	float angle, xchange, ychange, d;




	angle = rotation_angle_radian;

	float cost = cosf( angle );
	float sint = sinf( angle );




	m_vertices[vertice_count].position = sf::Vector2f( x + 0 , y + 0 );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0, texture_y + 0 );
	vertice_count++;

	/*
	    x=x_size
	    y=0

	    nx = x_size*cost
	    ny = x_size*sint

	*/
	m_vertices[vertice_count].position = sf::Vector2f( x + x_size * cost , y + x_size * sint  );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0 + x_size, texture_y + 0 );
	vertice_count++;

	/*
	    x=x_size
	    y=y_size

	    nx = x_size*cost-y_size*sint
	    ny = x_size*sint+y_size*cost

	*/


	m_vertices[vertice_count].position = sf::Vector2f( x + x_size * cost - y_size * sint, y + x_size * sint + y_size * cost );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_size, y_size + y_offset );
	vertice_count++;

	/*
	    x=0
	    y=y_size

	    nx = -y_size*sint
	    ny = y_size*cost

	*/

	m_vertices[vertice_count].position = sf::Vector2f( x - y_size * sint, y + y_size * cost );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x , y_size + texture_y );
	vertice_count++;
}
void UQVA::addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset, float rotation_angle_radian, float size )
{


}
UQVA::UQVA()
{
	texture = NULL;
	vertice_count = 0;
	m_vertices.clear();
	m_vertices.resize( 0 );
	m_vertices.setPrimitiveType( sf::Quads );
}
void UQVA::addText( float x, float y, std::string text, float scale )
{
	for( int i = 0; i < text.size(); i++ )
	{
		addQuad( x + i * 12, y, 14, 20, 0 + 13 * ( text[i] - 48 ), 275 );

	}

}
