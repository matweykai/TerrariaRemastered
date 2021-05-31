#include "Item.h"

unsigned int Item::get_maxSize() { return this->maxSize; }
string Item::get_name() { return this->name; }
void Item::set_name(string name) { this->name = name; }

void Item::operator = (const Item item)
{
	this->maxSize = item.maxSize;
	this->name = item.name;
}