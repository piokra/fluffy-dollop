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
#include "test_obj.h"
#include "interfacewindow.h"
#include "worldgenerator.h"
#include "liquidifieremitter.h"
#include "datastream.h"
#include "cthulu.h"
#include "inventory.h"
#include "itempickup.h"
#include "idtoitemconversion.h"
#include "craftingmenu.h"
#include "particlesystem.h"

extern int xoffset;
extern int yoffset;


extern int mx;
extern int my;
sf::Font font;


#include "world.h"
#include "UQVA.h"
#include "interface.h"
std::string operator "" _s ( const char* chars, std::size_t )
{
	return std::string( chars );
}

int xoffset2 = 0 , yoffset2 = 0;
Menu menu;

/* Function declarations */
BasicCrafting craft;
void eventManagement( sf::Window *window );
void renderingThread( sf::RenderWindow *window );

int main()
{
	std::cout << sf::Texture::getMaximumSize() << std::endl;
	menu = Menu();
	menu.activate();
	craft.deactivate();

	Globals::INTERFACES.addInterface( &craft );
	Globals::INTERFACES.addInterface( &menu );

	sf::RenderWindow window( sf::VideoMode( 10, 10 ), "Swiat Szalonych Kwadratow!", sf::Style::Fullscreen );
	sf::Vector2u ssize;
	ssize = window.getSize();
	Globals::SCREENHALFH = ssize.y / 2;
	Globals::SCREENHALFW = ssize.x / 2;
	window.setActive( false );

	sf::Thread thread( &renderingThread, &window );
	thread.launch();
	window.setKeyRepeatEnabled( false );
	sf::sleep( sf::milliseconds( 1000 ) );
	sf::Clock timer;
	timer.restart();
	sf::Uint32 rem = 0;
	Globals::TIMEACCUMULATOR = 0;

	while ( window.isOpen() )
	{
		sf::sleep( sf::milliseconds( 5 ) );
		Globals::TIMEACCUMULATOR += timer.getElapsedTime().asMilliseconds();
		rem += Globals::TIMEACCUMULATOR % 5;
		Globals::TIMEACCUMULATOR -= Globals::TIMEACCUMULATOR % 5;
		Globals::TIMEACCUMULATOR += rem;
		rem = rem % 5;
		Globals::TIMEACCUMULATOR /= 5;
		timer.restart();
		//std::cout << Globals::TIMEACCUMULATOR << " " << rem << std::endl;

		int count = Globals::TIMEACCUMULATOR;
		while( count-- )
		{


			if( Globals::GAME->object != NULL )
			{
				//std::cout << " hello " << std::endl;
				Point<float> tpi = Globals::GAME->object->getPos();
				xoffset = tpi.m_x - Globals::SCREENHALFW;
				yoffset = tpi.m_y - Globals::SCREENHALFH;
			}


			Globals::PTR = false;
			if( !Globals::GAMEPAUSED ) if( Globals::GAME->obj_vec )Globals::GAME->obj_vec->process();
			if( !Globals::GAMEPAUSED ) if( Globals::GAME->world )Globals::GAME->world->process( xoffset, yoffset, Globals::SCREENHALFW * 2 / Globals::BLOCKSIZE + 15,
							Globals::SCREENHALFW * 2 / Globals::BLOCKSIZE + 15 );
			Globals::PTR = true;
			eventManagement( &window );


		}
		while( Globals::WPLS ) sf::sleep( sf::milliseconds( 1 ) );
		Globals::TIMEACCUMULATOR = 0;





	}

	return 0;
}

void renderingThread( sf::RenderWindow *window )
{
    sf::Image flash;
    flash.loadFromFile("/img/bg.png");
    sf::Texture textr;
    textr.loadFromImage(flash);
    sf::Sprite sprite;
    sprite.setTexture(textr);
	std::uniform_real_distribution<float> ang( 0, 6.2830 );
	auto lambda1 = []( Point<float>& pos, float angle, float timepassed )
	{
		pos.m_x += cosf( angle ) * 3;
		pos.m_y += sinf( angle ) * 3;
	};
	auto lambda2 = []( sf::Color & color, float angle, float timepassed ) { };
	auto randomangle = std::bind( ang, Globals::RANDOMGENERATOR );
	//Globals::GAME->object = NULL;


	sf::Clock timer;


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

    //window->draw(sprite);
    //window->display();
    //sf::sleep(sf::milliseconds(3000));
	timer.restart();




	//mw->setPos( Point<float>( 500, 2000 ) );
	//objvec->add( mw );
	//Globals::GAME->movingworlds.push_back( mw );
	std::string ocounts;
	std::string bcounts;
	sf::Text bcount;
	bcount.setFont( font );
	bcount.setColor( sf::Color( 255, 255, 255 ) );
	bcount.setPosition( window->getSize().x - 100, 50 );

	sf::Text ocount;
	ocount.setFont( font );
	ocount.setColor( sf::Color( 255, 255, 255 ) );
	ocount.setPosition( window->getSize().x - 100, 100 );
	timer.restart();
	sf::Uint32 rem = 0;
	//uqva.scale(0.7,0.7);
	//uqva.move(250,250);
	sf::Image img;
	img.loadFromFile( "\img\\texture.png" );

	img.createMaskFromColor( sf::Color( 255, 0, 128 ) );
	sf::Texture texture;

	texture.loadFromImage( img );
	Globals::TEXTURE0 = &texture;
	UQVA uqva;
	uqva.setTexture( &texture );
	while ( window->isOpen() )
	{
		//sf::sleep( sf::milliseconds( 5 ) );

		//std::cout << Globals::TIMEACCUMULATOR << " " << rem << std::endl;
		//sf::sleep(sf::milliseconds(8));


		ui fps;
		if( timer.getElapsedTime().asMilliseconds() == 0 )

			text_str = "1000+";

		else
		{
			fps = 1000000 / timer.getElapsedTime().asMicroseconds();
			text_str = tostr( fps );

		}

		timer.restart();
		ocount.setString( tostr( Counter<Object>::getCount() ) );
		bcount.setString( tostr( Counter<ShiftedBlock>::getCount() ) );



		text.setString( text_str );
		uqva.setOrigin( xoffset2, yoffset2 );

		Globals::RTR = false;
		if( Globals::GAME->world )Globals::GAME->world->draw( &uqva, xoffset, yoffset, Globals::SCREENHALFW * 2. / Globals::BLOCKSIZE + 3, Globals::SCREENHALFW * 2. / Globals::BLOCKSIZE + 3,
					Globals::BLOCKSIZE );
		if( Globals::GAME->obj_vec )Globals::GAME->obj_vec->draw( &uqva );
		Globals::RTR = true;
		while( Globals::WPLS ) sf::sleep( sf::milliseconds( 1 ) );


		window->clear();
		window->draw( uqva );
		uqva.empty();
		uqva.setOrigin( 0, 0 );
		if( Globals::GAME->object ) Globals::GAME->object->customDraw( &uqva );
		window->draw( uqva );
		uqva.empty();
		window->draw( text );

		Globals::INTERFACES.draw( window );
		window->draw( bcount );
		window->draw( ocount );
		Globals::GAME->text.drawTexts( window, 5 );
		window->display();






		sf::sleep( sf::milliseconds( 8 ) );






	}

	Globals::MOUSEX = sf::Mouse::getPosition().x - window->getPosition().x;

}

void eventManagement( sf::Window *window )
{

	Task task;
	StandardStream stream;
	Globals::MOUSEX = sf::Mouse::getPosition().x - window->getPosition().x;
	Globals::MOUSEY = sf::Mouse::getPosition().y - window->getPosition().y;

	sf::Event event;
	while ( window->pollEvent( event ) )
	{
		if( Globals::INTERFACES.processEvent( event ) ) continue;
		if ( event.type == sf::Event::Closed )
		{


			window->close();

		}
		if( event.type == sf::Event::MouseMoved )
		{
			Globals::MOUSEX = event.mouseMove.x;
			Globals::MOUSEY = event.mouseMove.y;
		}
		else
			if ( event.type == sf::Event::KeyPressed )
			{
				if( event.key.code == sf::Keyboard::A )
				{
					stream << 0;
					task = Task( "move"_s, stream );
					Globals::GAME->taskqueue.addTask( task );
				}
				else
					if( event.key.code == sf::Keyboard::D )
					{
						stream << 1;
						task = Task( "move"_s, stream );
						Globals::GAME->taskqueue.addTask( task );
					}
					else

						if( event.key.code == sf::Keyboard::W )
						{
							task = Task( "jump"_s, stream );
							Globals::GAME->taskqueue.addTask( task );
						}
						else
							if ( event.key.code == sf::Keyboard::Space )
							{
								//Object* obj;
								//obj  = new LiquidifierEmitter( xoffset + 500, yoffset + 500, Globals::GAME->obj_vec );
								//Globals::GAME->obj_vec->add( obj );
							}
							else
								if( event.key.code == sf::Keyboard::Z )
								{
									static bool tc = true;
									if( !tc ) craft.deactivate();
									else craft.activate();
									tc = !tc;
								}
								else
									if( event.key.code == sf::Keyboard::X )
									{
										Object* obj = new ItemPickUp( xoffset + 700, yoffset + 500, Point<int>(), new MeleeWeapon( 0, Point<int>( 101, 1 ), 300, 33, 50, 1, 1, 1 ), 0 );
										Globals::GAME->obj_vec->add( obj );
									}
									else
										if ( event.key.code >= 26 && event.key.code < 36 )
										{
											std::cout << event.key.code << std::endl;
											stream << ( int )( event.key.code - 26 );
											task = Task( "select_slot", stream );
											Globals::GAME->taskqueue.addTask( task );
										}

			}

		if ( event.type == sf::Event::KeyReleased )
		{
			if( event.key.code == sf::Keyboard::A )
			{
				stream << 0;
				task = Task( "unmove"_s, stream );
				Globals::GAME->taskqueue.addTask( task );
			}
			else
				if( event.key.code == sf::Keyboard::D )
				{
					stream << 1;
					task = Task( "unmove"_s, stream );
					Globals::GAME->taskqueue.addTask( task );
				}


				else
					if( event.key.code == sf::Keyboard::Escape )
					{
						static bool t = false;
						if( !t ) menu.deactivate();
						else menu.activate();
						t = !t;
						Globals::GAMEPAUSED = !t;
					}






		}
		else
			if ( event.type == sf::Event::MouseMoved )
			{
				int xx = event.mouseMove.x;
				int yy = event.mouseMove.y;

			}
			else
				if ( event.type == sf::Event::MouseButtonPressed )
				{
					float xx = event.mouseButton.x + xoffset;
					float yy = event.mouseButton.y + yoffset;
					float xxi = xx / Globals::BLOCKSIZE;
					float yyi = yy / Globals::BLOCKSIZE;
					if( event.mouseButton.button == sf::Mouse::Left )
					{
						if( Globals::GAME->world != NULL )
						{
							task = Task( "use", stream );
							Globals::GAME->taskqueue.addTask( task );

						}
					}
					else
						if( event.mouseButton.button == sf::Mouse::Middle )
						{
							Block* blok;
							blok = Globals::GAME->world->getBlock( xxi, yyi );
							if ( blok != NULL )
							{
								blok->damage( 100000 );
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
				else
					if ( event.type == sf::Event::MouseButtonReleased )
					{
						if( event.mouseButton.button == sf::Mouse::Left )
						{
							if( Globals::GAME->world != NULL )
							{
								task = Task( "unuse", stream );
								Globals::GAME->taskqueue.addTask( task );

							}
						}
					}

	}
}

