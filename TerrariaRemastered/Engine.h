#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "Block.h"
#include "Player.h"
#include <vector>
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define WIDTH 30
#define HEIGHT 10
#define P_HEIGHT 3
#define P_WIDTH 2
#define BACKGROUND 1, 161, 255
#define BLOCKWIDTH 40
#define BLOCKHEIGHT 40
#define INVENTORY_WIDTH 2 * BLOCKWIDTH
#define INVENTORY_HEIGHT 2 * BLOCKHEIGHT
#define RADIUS 3.5

using namespace sf;
using namespace std;

enum TexturesID
{
	Player_texture = 0,
	Inventory_cell
};

class Engine
{
	vector<Block> blocks;
	vector<Texture> textures;
	Player player{ WIDTH / 2, HEIGHT / 2 };
	RenderWindow gameWindow { VideoMode(BLOCKWIDTH * WIDTH, BLOCKHEIGHT * HEIGHT + INVENTORY_HEIGHT), "TerrariaRemastered" };
	bool closing = false;

	mutex mut;

	Music music;

	void init_map();
	void update_frame();
	void control_enter(Event ev);
	void get_textures();
	void Music_On();
	Coordinates* check_click(Vector2i mouse_coordinates);
	void place_block(Coordinates coordinates, Block block);
	bool is_in_range(Coordinates coordinates);

	void movePlayerRight();
	void movePlayerLeft();
	void movePlayerUp();
	void movePlayerDown();

	void falling();
	bool is_collided(unsigned int x, unsigned int y);
public:
	void start_game();
};

