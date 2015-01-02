#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <SFML/Graphics.hpp>



class Texts;

class Text
{
public:
	Text()
	{
		m_permanent = false;
		m_duration = 0;
		m_timepassed = 0;
		m_parent = NULL;
		m_font->loadFromFile( "font.ttf" );
	};
	Text( std::string content, float xpos, float ypos, float charactersize, float duration, sf::Color color, int id, Texts *parent, sf::Font* font )
	{
		m_font = font;
		m_content = content;
		m_xpos = xpos;
		m_ypos = ypos;
		m_charactersize = charactersize;
		if ( duration < 0 )
		{
			m_permanent = true;
			m_duration = 1000;
		}
		else
		{
			m_permanent = false;
			m_duration = duration;
		}
		m_color = color;
		m_id = id;
		m_parent = parent;
		m_relative = false;

		updateText();
	}
	~Text() { };

	void updateText()
	{
		m_text.setFont( *m_font );
		m_text.setCharacterSize( m_charactersize );
		m_text.setPosition( m_xpos, m_ypos );
		m_text.setString( m_content );
		m_text.setColor( m_color );
	}

	void deleteText();

	void drawText( sf::RenderWindow *window, int32_t timepassed );

	void changeID( int newid )
	{
		m_id = newid;
	};

	void setRelative()
	{
		m_relative = true;
	};
	bool isRelative()
	{
		return m_relative;
	};

private:
	sf::Font* m_font;
	sf::Text m_text;
	Texts *m_parent;
	std::string m_content;
	bool m_permanent;
	bool m_relative;
	float m_duration;
	float m_timepassed;
	float m_xpos;
	float m_ypos;
	float m_charactersize;
	int m_id;
	sf::Color m_color;

};



class Texts
{
public:
	Texts()
	{
		m_texts.clear();
		m_textcount = 0;
		m_font.loadFromFile( "font.ttf" );
	};
	~Texts() { };

	void addText( std::string content, float xpos, float ypos )
	{
		addText( content, xpos, ypos, 24 );
	};
	void addText( std::string content, float xpos, float ypos, float charactersize )
	{
		addText( content, xpos, ypos, charactersize, -1 );
	};
	void addText( std::string content, float xpos, float ypos, float charactersize, float duration )
	{
		addText( content, xpos, ypos, charactersize, duration, sf::Color::Black );
	};
	void addText( std::string content, float xpos, float ypos, float charactersize, float duration, sf::Color color );
	void addText( std::string content, float xpos, float ypos, float charactersize, float duration, sf::Color color, bool relative );
	void drawTexts( sf::RenderWindow *window, int32_t timepassed )
	{
		for( int i = 0; i < m_textcount; i++ )
		{
			m_texts[i].drawText( window, timepassed );
		}
	}

	void deleteText( int id );


private:
	sf::Font m_font;
	std::vector <Text> m_texts;
	int m_textcount;
};
#endif



