#pragma once
#include "Coordinates.h"
#include "Item.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Block : Item
{
	Coordinates coordinates;
	Texture* texture;

public:
	Block(unsigned int x, unsigned int y, Texture* texture);
	Coordinates* get_coordinates();
	Texture* get_texture();

};


