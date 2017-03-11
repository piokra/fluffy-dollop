#include "interfacewindow.h"
#include <iostream>
#include "world.h"
#include "movingworld.h"
#include "worldgenerator.h"

InterfaceWindow::InterfaceWindow()
{
	//ctor
}

InterfaceWindow::~InterfaceWindow()
{
	//dtor
}
bool InterfaceWindow::processEvent( sf::Event event )
{
	if( m_state == IS_Inactive ) return false;
	bool result = false;
	int xx, yy;

	if ( event.type == sf::Event::MouseMoved )
	{
		xx = event.mouseMove.x;
		yy = event.mouseMove.y;
		for( int i = 0; i < m_buttons.size(); i++ )
		{
			result = result || m_buttons.at( i )->mousepos( Point<int>( xx, yy ) );
		}

	}
	else
		if ( event.type == sf::Event::MouseButtonPressed )
		{
			xx = event.mouseButton.x;
			yy = event.mouseButton.y;
			for( int i = 0; i < m_buttons.size(); i++ )
			{
				result = result || m_buttons.at( i )->click( Point<int>( xx, yy ) );
			}


		}
		else
			if ( event.type == sf::Event::MouseButtonReleased )
			{

				for( int i = 0; i < m_buttons.size(); i++ )
				{
					m_buttons.at( i )->unclick();
				}
				result = false;
			}
	return result;
}

void InterfaceWindow::draw( sf::RenderWindow* window )
{
	if( m_state == IS_Active )
	{


		for( int i = 0; i < m_buttons.size(); i++ )
			m_buttons.at( i )->draw( window );
		onDraw( window );
	}


}

void InterfaceWindow::activate()
{
	m_state = IS_Active;
}

void InterfaceWindow::deactivate()
{
	m_state = IS_Inactive;
}


void QuitButton::clickAction()
{
	exit( 0 );
}
void InterfaceWindow::addButton( Button* but )
{
	m_buttons.push_back( but );
}

bool Interfaces::processEvent( sf::Event event )
{
	bool result = false;
	for( int i = 0; i < m_interfaces.size(); i++ )
		result = result || m_interfaces.at( i )->processEvent( event );
	return result;
}

void Interfaces::draw( sf::RenderWindow* window )
{
	for( int i = 0; i < m_interfaces.size(); i++ )
		m_interfaces.at( i )->draw( window );
}

QuitButton::QuitButton( float x, float y ) : Button( x, y, x + 300, y + 100, "Quit game", PT_Constant )
{
	std::cout << "hello\n";
}

QuitButton::~QuitButton()
{

}
Menu::Menu()
{
	Button* but = new QuitButton( 500, 700 );
	addButton( but );
	but = new SaveButton( 500, 300 );
	addButton( but );
	but = new LoadButton( 500, 500 );
	addButton( but );
	but = new NewGameButton( 500, 100 );
	addButton( but );

}
void Interfaces::addInterface( InterfaceWindow* interface )
{
	m_interfaces.push_back( interface );
}

Menu::~Menu()
{

}

NewGameButton::NewGameButton( float x, float y ) : Button( x, y, x + 300, y + 100, "New Game", PT_Constant )
{

}

NewGameButton::~NewGameButton()
{

}

void NewGameButton::clickAction()
{
	CollisionObjectVector* colvec = new CollisionObjectVector( Globals::GAME );
	ObjectVector* objvec = new ObjectVector( Globals::GAME );
	Hero* her = new Hero( 100, 100, objvec, 10, Rect<float>() );
	Object* obj = her;
	obj->setPos( Point<float>( -100, -100 ) );
	objvec->add( her );
	colvec->add( her );
	//Globals::GAME->movingworlds.push_back( her );
	WorldGenerator::selectSeed( "Witam pozdrawiam" );
	WorldGenerator::selectSize( 1000, 1000 );
	World* wrld = WorldGenerator::generateWorld();
	Object* tobj = Globals::GAME->object;
	ObjectVector* tobjv = Globals::GAME->obj_vec;
	World* tworld = Globals::GAME->world;
	CollisionObjectVector* tcolv = Globals::GAME->col_vec;

	Globals::GAME->world = 0;
	Globals::GAME->object = 0;
	Globals::GAME->obj_vec = 0;
	Globals::GAME->col_vec = 0;

	sf::sleep( sf::milliseconds( 10 ) );
	Globals::WPLS = true;
	while( !( Globals::PTR && Globals::RTR ) ) sf::sleep( sf::milliseconds( 1 ) );
	if( tworld ) delete tworld;
	//if( tcolv ) delete tcolv; @TODO ????
	if( tobjv ) delete tobjv;


	Globals::GAME->world = wrld;
	Globals::GAME->object = her;
	Globals::GAME->obj_vec = objvec;
	Globals::GAME->col_vec = colvec;
	Globals::GAME->movingworlds.clear();
	Globals::WPLS = false;
	Globals::TIMEACCUMULATOR = 0;


}
InterfaceState InterfaceWindow::getState()
{
	return m_state;
}
void InterfaceWindow::onDraw( sf::RenderWindow* window )
{

}

SaveButton::SaveButton( float x, float y ) : Button( x, y, x + 300, y + 100, "Save Game", PT_Constant )
{

}

SaveButton::~SaveButton()
{

}

void SaveButton::clickAction()
{
	World::saveWorld();
}

LoadButton::LoadButton( float x, float y ) : Button( x, y, x + 300, y + 100, "Load Game", PT_Constant )
{

}

LoadButton::~LoadButton()
{

}

void LoadButton::clickAction()
{
	World::loadWorld();
}

