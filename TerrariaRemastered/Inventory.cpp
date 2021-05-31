#include "Inventory.h"

void Inventory::config()
{
	inventory.resize(MAX_SIZE);
	for (int i = 0; i < MAX_SIZE; i++)
	{
		this->inventory[i].first = 0;
		this->inventory[i].second = nullptr;
	}
}

Item* Inventory::get_item(int index)
{
	Item* item = nullptr;

	if (index >= MAX_SIZE)
		throw exception("Index is out of range!");

	if (inventory[index].first > 0)
	{
		inventory[index].first--;
		item = inventory[index].second;
		Block* block = dynamic_cast<Block*>(item);
		if (block)
			item = new Block(*block);

		if (this->inventory[index].first == 0)
		{
			delete inventory[index].second;
			inventory[index].second = nullptr;
		}
	}

	return item;
}

void Inventory::put_item(Item* item)
{
	if (!item)
		throw exception("Item is nullptr!");

	bool was_added = false;
	int empty_cell_index = -1;

	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (empty_cell_index == -1 && inventory[i].second == nullptr)
			empty_cell_index = i;

		if (inventory[i].second == nullptr)
			continue;

		if (inventory[i].second->get_name() == item->get_name() && inventory[i].first < item->get_maxSize())
		{
			inventory[i].first++;
			was_added = true;
			delete item;
			break;
		}
	}

	if (!was_added && empty_cell_index != -1)
	{
		inventory[empty_cell_index].first++;
		inventory[empty_cell_index].second = item;
	}
}

