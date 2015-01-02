#include "interface.h"

void Text::drawText( sf::RenderWindow *window, int32_t timepassed )
{
	if ( !m_permanent )
	{
		m_timepassed += timepassed;
	}

	if ( m_timepassed < m_duration )
	{
		m_ypos = m_ypos - ( timepassed / m_duration * 23 );
		m_text.setPosition( m_xpos, m_ypos );
		sf::Color color;
		color = m_text.getColor();
		color.a = 255 - 0.6 * 255 * m_timepassed / m_duration;
		m_text.setColor( color );

		{


			window->draw( m_text );

		}
	}
	else
	{
		deleteText();
	}
}


void Texts::addText( std::string content, float xpos, float ypos, float charactersize, float duration, sf::Color color )
{
	m_texts.push_back( Text( content, xpos, ypos, charactersize, duration, color, m_textcount, this, &m_font ) );
	m_textcount++;
}

void Texts::addText( std::string content, float xpos, float ypos, float charactersize, float duration, sf::Color color, bool relative )
{
	addText( content, xpos, ypos, charactersize, duration, color );
	m_texts.back().setRelative();
}

void Text::deleteText()
{
	if ( m_parent != NULL )
	{
		m_parent->deleteText( m_id );
	}
	else
	{
		delete this;
	}
}

void Texts::deleteText( int id )
{
	std::swap( m_texts[id], m_texts[m_textcount - 1] );
	m_texts[id].changeID( id );
	m_textcount--;
	m_texts.erase( m_texts.end() );
	m_texts.resize( m_textcount );
};



