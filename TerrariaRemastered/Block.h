#pragma once
#include "Coordinates.h"
#include "Item.h"

using namespace sf;

class Block : public Item
{
	Coordinates coordinates;

public:
	Block(unsigned int x, unsigned int y, string name, Texture* texture);
	Coordinates* get_coordinates();
};


