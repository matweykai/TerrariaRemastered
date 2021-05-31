#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Item 
{
	//Downcasting don't work without it
	virtual void waste_func() {};
protected:
	unsigned int maxSize;
	string name;
	Texture* texture;
public:
	unsigned int get_maxSize();
	string get_name();
	Texture* get_texture();
};

