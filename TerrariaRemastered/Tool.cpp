#include "Tool.h"

Tool::Tool(string name, Texture* texture)
{
	this->name = name;
	this->texture = texture;
	maxSize = 1;
}