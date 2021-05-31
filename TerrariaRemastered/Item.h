#pragma once
#include <string>

using namespace std;

class Item 
{
	//Downcasting don't work without it
	virtual void waste_func() {};
protected:
	unsigned int maxSize;
	string name;
public:
	unsigned int get_maxSize();
	string get_name();
};

