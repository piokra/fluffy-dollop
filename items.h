#ifndef ITEMHOLDER_H
#define ITEMHOLDER_H

#include <vector>
#include "bg.h"
class Item;
class Task;
class UQVA;

class ItemHolder


{
public:
	ItemHolder();
	virtual ~ItemHolder();

	virtual void addItem( Item* item ) = 0;
	virtual void removeItem( Item* item ) = 0;
	virtual Item* getItemByID( unsigned ID ) = 0;
	virtual Item* getItemByPos( unsigned pos ) = 0;

protected:
private:
};

class ItemVector : public ItemHolder
{
public:
	ItemVector();
	~ItemVector();
	virtual void addItem( Item* item );
	virtual void removeItem( Item* item );
	virtual Item* getItemByID( unsigned ID );
	virtual Item* getItemByPos( unsigned pos );

protected:
	virtual void removeFromPos( unsigned pos );
private:
	std::vector<Item*> m_items;
};
class CraftingRecipe;
class Item
{
public:
	friend class Item;
	friend class CraftingRecipe;
	friend class Hero;
	friend class ItemVector;
	friend class Inventory;
	friend class BlockPlacer;
	friend class MeleeWeapon;

	virtual float getCooldown()  const;
	Item( unsigned stackcount, unsigned maxstack, unsigned id, Point<int> texturexy );
	virtual ~Item();
	virtual void decreaseCount( unsigned cnt );

	virtual int getItemType();



protected:
	void copyItem( Item* item );
	// getters&setters
	virtual const unsigned&  getID();

	virtual const unsigned& getHolderPos();
	virtual void setHolderPos( unsigned pos );

	virtual const bool isStackable();
	virtual const unsigned& getMaxStack();
	virtual const unsigned& getStackCount();
	virtual void setStackCount( unsigned cnt );

	virtual const Point<int>& getTextureXY();
	virtual void setParent( ItemHolder* holder );


	// game specific

	virtual void onUse( unsigned x, unsigned y );
	virtual void onDeath();
	virtual void onPickUp();
	virtual void onDrop();
	virtual void onDraw( UQVA* uqva );
	virtual void drop( unsigned x, unsigned y );
	virtual void draw( UQVA* uqva, Point<int> pos );


	virtual void use( unsigned x, unsigned y );
private:
	unsigned m_holderpos;
	unsigned m_id;
	int m_stackcount;
	int m_maxstack;
	ItemHolder* m_holder;
	Point<int> m_texture;

};

class BlockPlacer : public Item
{
public:
	friend CraftingRecipe;
	friend BlockPlacer;
	BlockPlacer( unsigned ID, Point<int> texture, Point<int> blocktexture );
	virtual ~BlockPlacer();
protected:
	virtual void onUse( unsigned x, unsigned y );
	virtual void copyItem( BlockPlacer* item );
	virtual int getItemType();
private:
	Point<int> m_blocktexture;
};

class MeleeWeapon : public Item
{
public:
	friend CraftingRecipe;
	friend MeleeWeapon;
	MeleeWeapon( unsigned ID, Point<int> texture, float range, float damage, float attack_delay, float attack_delay_multiplier, float blockmultiplier, float mobmultiplier );
	virtual ~MeleeWeapon();
	virtual void decreaseCount( unsigned cnt );
protected:
	virtual void copyItem( MeleeWeapon* item );
	virtual int getItemType();
	virtual const bool isStackable();
	virtual void  onUse( unsigned x, unsigned y );
	virtual float getCooldown() const;
private:
	float m_damage;
	float m_range;
	float m_blockmultiplier;
	float m_mobmultiplier;
	float m_attackdelay;
	float m_attackdelaymultiplier;


};

class RocketLauncher : public Item
{
public:
	RocketLauncher( unsigned id, float damage, float lifetime, float blockmul, float objmul, float attackdelay, float attackdmul );
	virtual ~RocketLauncher();
protected:
	virtual const bool isStackable();
	virtual void  onUse( unsigned x, unsigned y );
	virtual float getCooldown() const;
private:
	float m_damage;
	float m_lifetime;
	float m_blockmultiplier;
	float m_mobmultiplier;
	float m_attackdelay;
	float m_attackdelaymultiplier;

};

class Rifle : public Item
{
public:
	Rifle( unsigned id, float damage, float lifetime, float blockmul, float objmul, float attackdelay, float attackdmul );
	virtual ~Rifle();
protected:
	virtual const bool isStackable();
	virtual void  onUse( unsigned x, unsigned y );
	virtual float getCooldown() const;
private:
	float m_damage;
	float m_lifetime;
	float m_blockmultiplier;
	float m_mobmultiplier;
	float m_attackdelay;
	float m_attackdelaymultiplier;

};

#endif // ITEMHOLDER_H
