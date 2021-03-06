#pragma once
#include <vector>
#include "Item.h"
#include "Block.h"

using namespace std;

#define INVENTORY_SIZE 10

class Inventory
{
	vector<pair<int,Item*>> inventory;

public:
	void config();
	Item* get_item(int index);
	void put_item(Item* item);
	vector<pair<int, Item*>> get_inventory();
};