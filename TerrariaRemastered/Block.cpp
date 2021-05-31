#include "Block.h"

Coordinates* Block::get_coordinates() 
{
	return &coordinates;
}
Block::Block(unsigned int x, unsigned int y, string name, Texture* texture)
{
	this->coordinates = Coordinates(x, y);
	this->texture = texture;
	this->name = name;
	this->maxSize = 20;
}