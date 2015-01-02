#ifndef __INTERFACE_MENU_H__
#define __INTERFACE_MENU_H__


#include "bg.h"

enum ButtonState
{
	BS_None,
	BS_Active,
	BS_Inactive,
	BS_Selected
};
enum PositionType
{
	PT_None,
	PT_Relative,
	PT_Constant
};


class Button
{
public:
	Button( double x, double y, double xx, double yy, std::string text, PositionType  ptype )
	{
		m_text = text;
		if( ptype == PT_Constant ) m_box = Rect<int>( Point<int>( ( int )x, ( int )y ), Point<int>( ( int )xx, ( int )yy ) );
	};
	~Button() { };
	void click( Point<int> pos )
	{
		if( m_box.getCollision( pos ) ) clickAction();
	};
	void mousepos( Point<int> pos )
	{
		if( m_box.getCollision( pos ) )
		{
			mouseOverAction();
		}
		else
		{
			if( m_state == BS_Selected ) mouseOutAction();
		}
	};
	void unclick()
	{
		unclickAction();
	};
	virtual void clickAction() { };
	virtual void unclickAction() { };
	virtual void mouseOverAction()
	{
		m_state = BS_Selected;
	};
	virtual void mouseOutAction()
	{
		m_state = BS_Active;
	};
	virtual void draw( sf::RenderWindow* window )
	{

		sf::RectangleShape rekt( sf::Vector2f( m_box.getCorner( TOPRIGHT ).m_x - m_box.getCorner( BOTLEFT ).m_x, m_box.getCorner( TOPRIGHT ).m_y - m_box.getCorner( BOTLEFT ).m_y ) );
		rekt.setPosition( m_box.getCorner( BOTLEFT ).m_x, m_box.getCorner( BOTLEFT ).m_y );
		rekt.setFillColor( sf::Color( 255, 0, 0 ) );
		if( m_state == BS_Selected )
			rekt.setFillColor( sf::Color( 0, 255, 0 ) );
		window->draw( rekt );
	};
	virtual void process() { }

protected:
private:
	std::string m_text;
	ButtonState m_state;
	Rect<int> m_box;

};

class Menu
{
public:
	Menu()
	{

	};
	~Menu()
	{

	};
	void draw( sf::RenderWindow* window )
	{
		for( int i = 0; i < m_buttons.size(); i++ )
		{
			m_buttons.at( i )->draw( window );
		}
	}
	void mousepos( Point<int> pos )
	{
		for( int i = 0; i < m_buttons.size(); i++ )
		{
			m_buttons.at( i )->mousepos( pos );
		}
	}
	void click( Point<int> pos )
	{
		for( int i = 0; i < m_buttons.size(); i++ )
		{
			m_buttons.at( i )->click( pos );
		}
	}
	void add( Button* but )
	{
		m_buttons.push_back( but );
	}


protected:
private:
	std::vector<Button*> m_buttons;
};








#endif // __INTERFACE_MENU_H__

