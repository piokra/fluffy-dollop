//Universal Quad Vertex array
#ifndef UQVA_H
#define UQVA_H
#include <SFML/Graphics.hpp>




#ifndef M_PI
#define M_PI 3.14159265359
#endif // M_PI
class UQVA : public sf::Drawable, public sf::Transformable
{
public:
	UQVA();

	void combineUQVA( UQVA *UQVA );
	void empty()
	{
		m_vertices.clear();
		vertice_count = 0;
	};
	void setTexture( sf::Texture *Texture )
	{
		texture = Texture;
	};

	void addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y )
	{
		addQuad( x, y, x_size, y_size, texture_x, texture_y, 0.0f, 0.0f );
	};
	void addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset );
	void addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset, float rotation_angle_radian );
	void addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset, float rotation_angle_radian, float size );
	void changeColor( sf::Color color, int32_t count );
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const
	{
		states.transform *= getTransform();

		states.texture = texture;

		target.draw( m_vertices, states );
	}
	void addText( float x, float y, std::string text, float scale );
	int vertice_count;
private:
	sf::VertexArray m_vertices;

	sf::Texture *texture;


};

#endif // UQVA_H
