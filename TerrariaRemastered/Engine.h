#pragma once
#define WIDTH 10
#define HEIGHT 10
//Íóæíû äëÿ ïåðåìåùåíèÿ êóðñîðà
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define BACKGROUND 1, 161, 255
#define BLOCKWIDTH 40
#define BLOCKHEIGHT 40

#include "Block.h"
#include "Player.h"
#include <vector>
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace sf;

class Engine
{
	vector<Block> blocks;
	Player player{ WIDTH / 2, HEIGHT / 2 };
	RenderWindow gameWindow { VideoMode(BLOCKWIDTH * WIDTH, BLOCKHEIGHT * HEIGHT), "TerrariaRemastered" };

	char current_frame[HEIGHT][WIDTH];

	void init_map();
	void update_frame();
	void control_enter(Event ev);
public:
	void start_game();

	bool is_collided(unsigned int x, unsigned int y);

	void movePlayerRight();
	void movePlayerLeft();
	void movePlayerUp();
	void movePlayerDown();
};

