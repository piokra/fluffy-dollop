#ifndef __INTERFACE_MENU_H__
#define __INTERFACE_MENU_H__


#include "bg.h"
extern sf::Font font;
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
	Button( float x, float y, float xx, float yy, std::string text, PositionType  ptype )
	{
		m_text = text;
		if( ptype == PT_Constant ) m_box = Rect<int>( Point<int>( ( int )x, ( int )y ), Point<int>( ( int )xx, ( int )yy ) );
	};
	~Button() { };
	bool click( Point<int> pos )
	{
		if( m_box.getCollision( pos ) )
		{
			clickAction();
			return true;
		}
		return false;
	};
	bool mousepos( Point<int> pos )
	{
		if( m_box.getCollision( pos ) )
		{
			mouseOverAction();
		}
		else
		{
			if( m_state == BS_Selected ) mouseOutAction();
		}
		return false;
	};
	bool unclick()
	{
		unclickAction();
		return false;
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
		sf::Text text;
		text.setString( m_text );
		text.setFont( font );
		text.setPosition( m_box.getCorner( BOTLEFT ).m_x + 10, m_box.getCorner( BOTLEFT ).m_y + 10 );
		sf::RectangleShape rekt( sf::Vector2f( m_box.getCorner( TOPRIGHT ).m_x - m_box.getCorner( BOTLEFT ).m_x, m_box.getCorner( TOPRIGHT ).m_y - m_box.getCorner( BOTLEFT ).m_y ) );
		rekt.setPosition( m_box.getCorner( BOTLEFT ).m_x, m_box.getCorner( BOTLEFT ).m_y );
		rekt.setFillColor( sf::Color( 255, 122, 122 ) );
		if( m_state == BS_Selected )
			rekt.setFillColor( sf::Color( 122, 255, 122 ) );
		window->draw( rekt );
		window->draw( text );
	};
	virtual void process() { }

protected:
private:
	std::string m_text;
	ButtonState m_state;
	Rect<int> m_box;

};






#endif // __INTERFACE_MENU_H__

