//Universal Quad Vertex array

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
private:
	sf::VertexArray m_vertices;
	int vertice_count;
	sf::Texture *texture;


};

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


	d = sqrtf( x_size * x_size + y_size * y_size );
	d = d / 4;

	angle = rotation_angle_radian;

	xchange = d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0, texture_y + 0 );
	vertice_count++;

	angle = rotation_angle_radian + M_PI / 2;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0 + x_size, texture_y + 0 );
	vertice_count++;

	angle = rotation_angle_radian + M_PI;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_size + x_offset, y_size + y_offset + texture_y );
	vertice_count++;

	angle = rotation_angle_radian + 3 * M_PI / 2;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_offset, y_size + y_offset + texture_y );
	vertice_count++;
}
void UQVA::addQuad( float x, float y, float x_size, float y_size, float texture_x, float texture_y, float x_offset, float y_offset, float rotation_angle_radian, float size )
{

	m_vertices.resize( m_vertices.getVertexCount() + 4 );

	float angle, xchange, ychange, d;


	d = sqrtf( x_size * x_size + y_size * y_size ) * size;
	d = d / 4;

	angle = rotation_angle_radian;

	xchange = d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0, texture_y + 0 );
	vertice_count++;

	angle = rotation_angle_radian + M_PI / 2;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + 0 + x_size, texture_y + 0 );
	vertice_count++;

	angle = rotation_angle_radian + M_PI;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_size + x_offset, y_size + y_offset + texture_y );
	vertice_count++;

	angle = rotation_angle_radian + 3 * M_PI / 2;
	xchange =  d * cosf( angle );
	ychange = d * sinf( angle );

	m_vertices[vertice_count].position = sf::Vector2f( x + xchange + 0 + xchange, 0 + y + ychange + ychange );
	m_vertices[vertice_count].texCoords = sf::Vector2f( texture_x + x_offset, y_size + y_offset + texture_y );
	vertice_count++;
}
UQVA::UQVA()
{
	texture = NULL;
	vertice_count = 0;
	m_vertices.clear();
	m_vertices.resize( 0 );
	m_vertices.setPrimitiveType( sf::Quads );
}
