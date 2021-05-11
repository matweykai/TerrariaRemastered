#pragma once
#include "Coordinates.h"
#include "Item.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Block : Item
{
	Coordinates coordinates;
	Color color;
public:
	Block(unsigned int x, unsigned int y, Color color);
	Coordinates* get_coordinates();
	Color get_color();
};


