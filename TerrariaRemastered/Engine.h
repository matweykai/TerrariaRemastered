#pragma once
#define WIDTH 10
#define HEIGHT 10
#define P_HEIGHT 3
#define P_WIDTH 2
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
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Engine
{
	vector<Block> blocks;
	vector<Texture> textures;
	Player player{ WIDTH / 2, HEIGHT / 2 };
	RenderWindow gameWindow { VideoMode(BLOCKWIDTH * WIDTH, BLOCKHEIGHT * HEIGHT), "TerrariaRemastered" };
	bool closing = false;

	mutex mut;

	void init_map();
	void update_frame();
	void control_enter(Event ev);
	void get_textures();

	void movePlayerRight();
	void movePlayerLeft();
	void movePlayerUp();
	void movePlayerDown();

	void falling();
	bool is_collided(unsigned int x, unsigned int y);
public:
	void start_game();
};

