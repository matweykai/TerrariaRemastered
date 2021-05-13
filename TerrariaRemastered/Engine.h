#pragma once
#define WIDTH 10
#define HEIGHT 10
#define WIDTH 10
#define PLAYER_UPPER '0'
#define PLAYER_DOWN 'A'
#define BLOCK_SYM 'O'
#define WALL_SYM '#'
//Нужны для перемещения курсора
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define BACKGROUND 255, 255, 255
#define BLOCKWIDTH 50
#define BLOCKHEIGHT 50

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

	void init_map();
	void update_frame();
	void control_enter();
public:
	void start_game();

	bool is_collided(unsigned int x, unsigned int y);

<<<<<<< Updated upstream
	void movePlayerRight(unsigned int x, unsigned int y);
	void movePlayerLeft(unsigned int x, unsigned int y);
	void movePlayerUp(unsigned int x, unsigned int y);
	void movePlayerDown(unsigned int x, unsigned int y);
=======
	void movePlayerRight();
	void movePlayerLeft();
	void movePlayerUp();
	void movePlayerDown();
>>>>>>> Stashed changes
};

