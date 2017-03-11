#ifndef IDTOITEMCONVERSION_H
#define IDTOITEMCONVERSION_H

class Item;

class IDtoItemConversion
{
public:
	static Item* createItemFromID( unsigned ID );
protected:
private:
	//THIS IS A SINGLETON
	IDtoItemConversion();
	~IDtoItemConversion();
};

#endif // IDTOITEMCONVERSION_H
