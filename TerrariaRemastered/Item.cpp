#include "Item.h"

unsigned int Item::get_maxSize() { return this->maxSize; }
string Item::get_name() { return this->name; }
Texture* Item::get_texture()
{
	return texture;
}