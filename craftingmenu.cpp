#include "craftingmenu.h"
#include "inventory.h"
#include "items.h"
#include "UQVA.h"
#include "idtoitemconversion.h"
#include "globals.h"
#include "movingworld.h"
CraftingRecipe::CraftingRecipe( Item* item )
{
	m_crafting_result = item;
}

CraftingRecipe::~CraftingRecipe()
{


}

void CraftingRecipe::craft( Inventory& inv )
{
	std::cout << "hello\n";
	if( meetsRequirements( inv ) )
	{


		for( int i = 0; i < m_requirements.size(); i++ )
		{
			Item* item = inv.getItemByID( m_requirements[i]->getID() );
			item->decreaseCount( m_requirements[i]->getStackCount() );
		}
		inv.addItem( IDtoItemConversion::createItemFromID( m_crafting_result->getID() ) );
	}
}

void CraftingRecipe::addRequirement( Item* item )
{
	m_requirements.push_back( item );
}

bool CraftingRecipe::meetsRequirements( Inventory& inv )
{
	bool result = true;
	for( int i = 0; i < m_requirements.size(); i++ )
	{
		std::cout << m_requirements[i]->getID() << std::endl;
		Item* item = inv.getItemByID( m_requirements[i]->getID() );
		if( !item ) return false;
		result = result && ( item->getStackCount() >= m_requirements[i]->getStackCount() );
	}
	std::cout << result << std::endl;
	return result;
}

CraftingMenu::CraftingMenu()
{

}

CraftingMenu::~CraftingMenu()
{

}
errc CraftingRecipe::draw( UQVA* uqva, unsigned x, unsigned y )
{
	uqva->addQuad( 10 + x, 10 + y, 50, 50, 0, 26 );
	uqva->changeColor( sf::Color( 255, 255, 255, 100 ), 4 );
	m_crafting_result->draw( uqva, Point<int>( 20 + x, 20 + y ) );
	Item* item;
	for( int i = 1; i <= m_requirements.size(); i++ )
	{
		uqva->addQuad( 10 + i * 70 + x, 10 + y, 50, 50, 0, 26 );
		uqva->changeColor( sf::Color( 255, 255, 255, 100 ), 4 );
		item = m_requirements[i - 1];
		if( item )

			item->draw( uqva, Point<int>( 20 + i * 70 + x, 20 + y ) );

	}
}
void CraftingMenu::onDraw( sf::RenderWindow* window )
{
	UQVA uqva;
	uqva.setTexture( Globals::TEXTURE0 );
	for( int i = 0; i < m_recipes.size(); i++ )
	{
		m_recipes[i]->draw( &uqva, 200, 200 + i * 100 );
	}
	window->draw( uqva );
}
CraftButton::CraftButton( float x, float y, CraftingRecipe* target ) : Button( x, y, x + 100, y + 60, "Craft!", PT_Constant )
{
	m_recipe = target;
}

CraftButton::~CraftButton()
{

}

void CraftButton::clickAction()
{
	if( Globals::GAME->world )
		m_recipe->craft( Globals::GAME->object->getInventory() );
}
BasicCrafting::BasicCrafting()
{
	CraftingRecipe* recipe = new CraftingRecipe( IDtoItemConversion::createItemFromID( 20000 ) );
	recipe->addRequirement( IDtoItemConversion::createItemFromID( 1000 ) );
	CraftButton* but = new CraftButton( 500, 200, recipe );
	addButton( but );
	addRecipe( recipe );

	recipe = new CraftingRecipe( IDtoItemConversion::createItemFromID( 30000 ) );
	recipe->addRequirement( IDtoItemConversion::createItemFromID( 1000 ) );
	but = new CraftButton( 500, 300, recipe );
	addButton( but );
	addRecipe( recipe );

	recipe = new CraftingRecipe( IDtoItemConversion::createItemFromID( 10000 ) );
	recipe->addRequirement( IDtoItemConversion::createItemFromID( 1000 ) );
	but = new CraftButton( 500, 400, recipe );
	addButton( but );
	addRecipe( recipe );

}

BasicCrafting::~BasicCrafting()
{

}
void CraftingMenu::addRecipe( CraftingRecipe* recipe )
{
	m_recipes.push_back( recipe );
}
