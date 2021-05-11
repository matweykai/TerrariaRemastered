#include "Player.h"

Player::Player(unsigned int x, unsigned int y) 
{
	this->coordinates = Coordinates(x, y);
}
Coordinates* Player::get_coordinates() 
{
	return &coordinates;
}
void Player::moveRight()
{
	coordinates.setX(coordinates.getX() + 1);
}
void Player::moveLeft()
{
	coordinates.setX(coordinates.getX() - 1);
}
void Player::moveUp()
{
	coordinates.setY(coordinates.getY() + 1);
}
void Player::moveDown()
{
	coordinates.setY(coordinates.getY() - 1);
}
