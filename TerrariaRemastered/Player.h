#pragma once

#include <vector>
#include "Item.h"
#include "Coordinates.h"
#include "Inventory.h"

class Player
{
	Inventory inventory;
	Coordinates coordinates;

public:
	Player(unsigned int x = 5, unsigned int y = 5);
	Inventory& get_inventory();

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	Coordinates* get_coordinates();
};


