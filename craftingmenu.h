#ifndef CRAFTINGMENU_H
#define CRAFTINGMENU_H
#include <vector>
#include "interfacewindow.h"
#include <SFML/Graphics.hpp>
typedef unsigned char errc;
class Item;
class Inventory;
class UQVA;
class CraftingRecipe
{
public:
	CraftingRecipe( Item* item );
	virtual ~CraftingRecipe();
	void craft( Inventory& inv );
	void addRequirement( Item* item );
	virtual errc draw( UQVA* uqva, unsigned x, unsigned y );
protected:

	bool meetsRequirements( Inventory& inv );

private:
	Item* m_crafting_result;
	std::vector<Item*> m_requirements;

};

class CraftingMenu : public InterfaceWindow
{
public:
	CraftingMenu();
	virtual ~CraftingMenu();
	void addRecipe( CraftingRecipe* recipe );
protected:
	virtual void onDraw( sf::RenderWindow* window );
private:
	std::vector<CraftingRecipe*> m_recipes;

};

class CraftButton : public Button
{
public:
	CraftButton( float x, float y, CraftingRecipe* target );
	virtual ~CraftButton();
protected:
	virtual void clickAction();
private:
	CraftingRecipe* m_recipe;


};
class BasicCrafting : public CraftingMenu
{
public:
	BasicCrafting();
	~BasicCrafting();
protected:
private:
};

#endif // CRAFTINGMENU_H
