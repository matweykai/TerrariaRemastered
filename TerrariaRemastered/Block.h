#pragma once
#include "Coordinates.h"
#include "Item.h"
#include <SFML/Graphics.hpp>

using namespace sf;

enum BlockTextures 
{
	Dirt = 1,
	Grass,
	Stone
};

class Block : public Item
{
	Coordinates coordinates;
	Texture* texture;

public:
	Block(unsigned int x, unsigned int y, string name, Texture* texture);
	Coordinates* get_coordinates();
	Texture* get_texture();

};


