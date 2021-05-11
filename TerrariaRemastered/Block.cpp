#include "Block.h"

Coordinates* Block::get_coordinates() 
{
	return &coordinates;
}
Color Block::get_color() 
{
	return color;
}
Block::Block(unsigned int x, unsigned int y, Color color)
{
	this->coordinates = Coordinates(x, y);
	this->color = color;
}