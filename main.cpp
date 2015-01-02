/* Includes */

/* Versions */

#include "version.h"
#include "ver_dat.h"


/* std */

#include <string>
#include <vector>
#include <stdio.h>
#include <conio.h>

/* SFML */

#include <SFML/Graphics.hpp>

/* simple */

#include "globals.h"
#include "bg.h"
#include "macro.h"
#include "typedefs.h"
#include "conversion.h"
#include "counter.h"
#include "interface_menu.h"
#include "movingworld.h"



/* game */

extern int xoffset;
extern int yoffset;

extern int mx;
extern int my;




#include "world.h"
#include "UQVA.h"
#include "interface.h"





/* Function declarations */

void eventManagement( sf::Window *window );
void renderingThread( sf::RenderWindow *window );

int main()

{


	Point<double> a( 2, 2 );
	Point<double> b( 4, 4 );
	Point<double> c( 3, 1 );
	Point<double> d( 4, 5 );
	ParamLine<double> l1( a, b );
	ParamLine<double> l2( c, d );
	l1.getOrgin().display();
	l1.getVector().m_p.display();

	l2.getOrgin().display();
	l2.getVector().m_p.display();
	double timei = l1.getIntersection( l2 );
	std::cout << timei << std::endl;


	Rect<double> r( a, b );
	r.display();
	r = r + a;
	r.display();
	( a + b ).display();


	ver_dat::dump_ver();


	sf::RenderWindow window( sf::VideoMode( 1000, 1000 ), "SFML works!" );
	window.setActive( false );

	sf::Thread thread( &renderingThread, &window );
	thread.launch();
	window.setKeyRepeatEnabled( false );

	while ( window.isOpen() )
	{
		eventManagement( &window );


	}

	return 0;
}

void renderingThread( sf::RenderWindow *window )
{

	Globals::GAME->object = NULL;

	sf::Clock timer;
	sf::Font font;

	font.loadFromFile( "font.ttf" );
	sf::CircleShape shape( 100.f );

	shape.setFillColor( sf::Color::Green );
	timer.restart();

	sf::Text text;

	sf::Text o_text;

	std::string text_str;

	text.setFont( font );
	text.setColor( sf::Color( 255, 255, 255 ) );
	text.setPosition( window->getSize().x - 100, window->getSize().y - 40 );

	o_text.setFont( font );
	o_text.setColor( sf::Color( 255, 255, 255 ) );
	o_text.setPosition( 40, window->getSize().y - 40 );

	Texts hello;
	hello.addText( "hello world", 500, 500, 72, 1000 );
	UQVA uqva;
	uqva.addQuad( 750, 750, 69, 42, 0, 0, 0, 0, 1.11 );

	timer.restart();

	ObjectVector* objvec = new ObjectVector( Globals::GAME );
	Hero* her = new Hero( 100, 100, Globals::GAME, 10, Rect<double>() );
	Object* obj = her;
	obj->setPos( Point<double>( 100, 2000 ) );
	Globals::GAME->movingworlds.push_back( her );
	World* wrld = new World( Globals::GAME, 1000, 1000 );
	Globals::GAME->world = wrld;
	Globals::GAME->object = obj;
	Globals::GAME->obj_vec = objvec;
	objvec->add( obj );
	obj = new Object( 300, 2100, Globals::GAME, 1000, Rect<double>( Point<double>( -Globals::BLOCKSIZE, -Globals::BLOCKSIZE ), Point<double>( Globals::BLOCKSIZE, Globals::BLOCKSIZE ) ) );
	objvec->add( obj );
	timer.restart();


	MovingWorld* mw = new MovingWorld( 500, 2000, Globals::GAME, 1, Rect<double>() );
	mw->setPos( Point<double>( 500, 2000 ) );
	objvec->add( mw );
	Globals::GAME->movingworlds.push_back( mw );

	while ( window->isOpen() )
	{
		sf::sleep( sf::milliseconds( 5 ) );
		Globals::TIMEACCUMULATOR += timer.getElapsedTime().asMilliseconds();
		//sf::sleep(sf::milliseconds(8));
		if( Globals::GAME->object != NULL )
		{
			Point<double> tpi = Globals::GAME->object->getPos();
			xoffset = tpi.m_x - 450;
			yoffset = tpi.m_y - 450;
		}


		ui fps;
		if( timer.getElapsedTime().asMilliseconds() == 0 )

			text_str = "1000+";

		else
		{
			fps = 1000000 / timer.getElapsedTime().asMicroseconds();
			text_str = tostr( fps );

		}



		text.setString( text_str );

		window->clear();
		objvec->process();

		wrld->process( xoffset + 250, yoffset + 250, 15, 15 );

		wrld->draw( window, xoffset, yoffset, 30, 30, Globals::BLOCKSIZE );
		objvec->draw( window );


		window->draw( text );
		hello.drawTexts( window, 1 );
		window->draw( uqva );

		window->display();

		timer.restart();

		//sf::sleep(sf::milliseconds(8));

		Globals::TIMEACCUMULATOR = Globals::TIMEACCUMULATOR - Globals::TIMEACCUMULATOR / Globals::TIMESTEP * Globals::TIMESTEP;





	}
	delete wrld;


}

void eventManagement( sf::Window *window )
{

	sf::Event event;
	while ( window->pollEvent( event ) )
	{
		if ( event.type == sf::Event::Closed )
		{


			window->close();

		}
		if ( event.type == sf::Event::KeyPressed )
		{
			if( event.key.code == sf::Keyboard::Left )
				mx -= 1;


			if( event.key.code == sf::Keyboard::Right )
				mx += 1;

			if( event.key.code == sf::Keyboard::Down )
				my += 1;


			if( event.key.code == sf::Keyboard::Up )
				my -= 1;



		}

		if ( event.type == sf::Event::KeyReleased )
		{
			if( event.key.code == sf::Keyboard::Left )
				mx += 1;


			if( event.key.code == sf::Keyboard::Right )
				mx -= 1;

			if( event.key.code == sf::Keyboard::Down )
				my -= 1;


			if( event.key.code == sf::Keyboard::Up )
				my += 1;



		}
		if ( event.type == sf::Event::MouseMoved )
		{
			int xx = event.mouseMove.x;
			int yy = event.mouseMove.y;

		}
		if ( event.type == sf::Event::MouseButtonPressed )
		{
			double xx = event.mouseButton.x + xoffset;
			double yy = event.mouseButton.y + yoffset;
			double xxi = xx / Globals::BLOCKSIZE;
			double yyi = yy / Globals::BLOCKSIZE;
			if( event.mouseButton.button == sf::Mouse::Left )
			{
				if( Globals::GAME->world != NULL )
				{
					Block* blok;
					blok = Globals::GAME->world->getBlock( xxi, yyi );
					if ( blok != NULL )
					{
						blok->m_phase = LIQUID;
					}

				}
			}
			else
				if( event.mouseButton.button == sf::Mouse::Middle )
				{
					if( Globals::GAME->obj_vec != NULL )
					{
						printf( "hello\n" );
						Point<double> click( xx, yy );
						Globals::GAME->obj_vec->objectsInRange( click, 300 );
					}
				}
				else
				{
					if( Globals::GAME->world != NULL )
					{
						Block* blok;
						blok = Globals::GAME->world->getBlock( xxi, yyi );
						if ( blok != NULL )
						{
							blok->m_phase = SOLID;
						}
					}
				}

		}

	}
}

