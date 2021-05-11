#include "Coordinates.h"

Coordinates::Coordinates(unsigned int x, unsigned int y)
{
	this->x = x;
	this->y = y;
}

unsigned int Coordinates::getX()
{
	return this->x;
}

unsigned int Coordinates::getY()
{
	return this->y;
}

void Coordinates::setX(unsigned int x)
{
	this->x = x;
}

void Coordinates::setY(unsigned int y)
{
	this->y = y;
}