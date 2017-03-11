#ifndef INTERFACEWINDOW_H
#define INTERFACEWINDOW_H

#include <vector>
#include "interface_menu.h"
#include "interface.h"
#include <SFML/Graphics.hpp>

enum InterfaceState
{
	IS_Active,
	IS_Inactive

};

class InterfaceWindow
{
public:
	InterfaceWindow();
	virtual ~InterfaceWindow();
	bool processEvent( sf::Event event );
	virtual void draw( sf::RenderWindow* window );
	void activate();
	void deactivate();
	void addButton( Button* but );
	InterfaceState getState();
protected:
	virtual void onDraw( sf::RenderWindow* window );
private:
	std::vector<Button*> m_buttons;
	InterfaceState m_state;

};

class Interfaces
{
public:
	bool processEvent( sf::Event event );
	void draw( sf::RenderWindow* window );
	void addInterface( InterfaceWindow* inter );
protected:
private:
	std::vector<InterfaceWindow*> m_interfaces;

};

class NewGameButton : public Button
{
public:
	NewGameButton( float x, float y );
	virtual ~NewGameButton( );
	virtual void clickAction();
private:

};

class QuitButton : public Button
{

public:
	QuitButton( float x, float y );
	virtual ~QuitButton();
	virtual void clickAction();
private:
};

class SaveButton : public Button
{

public:
	SaveButton( float x, float y );
	virtual ~SaveButton();
	virtual void clickAction();
private:
};

class LoadButton : public Button
{

public:
	LoadButton( float x, float y );
	virtual ~LoadButton();
	virtual void clickAction();
private:
};

class Menu : public InterfaceWindow
{
public:
	Menu();
	virtual ~Menu();
protected:
private:


};

#endif // INTERFACEWINDOW_H
