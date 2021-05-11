#pragma once
class Coordinates
{

	unsigned int x;
	unsigned int y;

public:
	Coordinates(unsigned int x = 0, unsigned int y = 0);

	unsigned int getX();
	unsigned int getY();

	void setX(unsigned int x);
	void setY(unsigned int y);
	
};

