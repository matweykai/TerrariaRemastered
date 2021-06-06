#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "Block.h"
#include "Player.h"
#include "Tool.h"
#include <vector>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define WIDTH 40
#define HEIGHT 20
#define P_HEIGHT 3
#define P_WIDTH 2
#define BACKGROUND 1, 161, 255
#define BLOCKWIDTH 40
#define BLOCKHEIGHT 40
#define INVENTORY_WIDTH 2 * BLOCKWIDTH
#define INVENTORY_HEIGHT 2 * BLOCKHEIGHT
#define RADIUS 3.5
#define MAPFILE "test.txt"
#define INVFILE "inventory.txt"
#define VERTICALTIME 100
#define JUMPHEIGHT 4

using namespace sf;
using namespace std;

enum TexturesID
{
	Player_texture = 0,
	Left_Player_texture,
	Inventory_cell,
	Selected_item,
	Tool_t
};

enum BlockTextures
{
	Dirt = TexturesID::Tool_t + 1,
	Grass,
	Stone,
	Tree,
	Leaves,
	Metal
};

class Engine
{
	vector<Block> blocks;
	vector<Texture> textures;
	Player player { WIDTH / 2, HEIGHT / 2 };
	RenderWindow gameWindow { VideoMode(BLOCKWIDTH * WIDTH, BLOCKHEIGHT * HEIGHT + INVENTORY_HEIGHT), "TerrariaRemastered" , Style::Titlebar | Style::Close};
	bool closing = false;
	Font mainFont;
	int inventory_index = 0;	//Index of selected cell in inventory

	bool is_falling = false;
	bool is_jumping = false;
	bool moving_left = false;

	mutex mut;

	Music music;

	void init_map();
	void update_frame();
	void control_enter(Event ev);
	void get_textures();
	void Music_On();
	Coordinates* check_click(Vector2i mouse_coordinates);
	bool place_block(Coordinates coordinates, Block block);	//True if block was placed
	bool is_in_range(Coordinates coordinates);
	void break_block(Coordinates coordinates);

	void dump_map(string fileName);
	bool download_map(string fileName);
	void dump_inventory(string fileName);
	bool dowload_inventory(string fileName);
	void jump();

	void movePlayerRight();
	void movePlayerLeft();
	void movePlayerUp();
	void movePlayerDown();

	void falling();
	bool is_collided(unsigned int x, unsigned int y);
public:
	void start_game();
};

bool is_end(ifstream* stream);
int find_space(string str, int start_pos);