#pragma once
#include "Item.h"
#include "Coordinates.h"
#include <vector>

class Player
{
	vector<Item*> Inventory;
	Coordinates coordinates;
public:
	Player(unsigned int x = 5, unsigned int y = 5);

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	Coordinates* get_coordinates();
};


