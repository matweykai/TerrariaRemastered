#pragma once
#include <vector>
#include "Item.h"

using namespace std;

class Inventory
{
	vector<Item*> items;
	vector<int> item_count;

public:
	Item* get_item(int index);
	void put_item(Item* item);
};