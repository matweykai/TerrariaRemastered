#include "Engine.h"
void Engine::Music_On()
{
	music.openFromFile("MusicTerraria.ogg");
	music.play();
	music.setLoop(true);
}

void Engine::update_frame() 
{
	gameWindow.clear(Color(BACKGROUND));
	//Blocks
	RectangleShape* rectangles = new RectangleShape[blocks.size()];

	for (int i = 0; i < blocks.size(); i++)
	{
		rectangles[i].setTexture(blocks[i].get_texture());
		rectangles[i].setOutlineColor(Color(0, 0, 0));
		//rectangles[i].setOutlineThickness(1);
		rectangles[i].setPosition(blocks[i].get_coordinates()->getX() * BLOCKWIDTH, blocks[i].get_coordinates()->getY() * BLOCKHEIGHT);
		rectangles[i].setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
		gameWindow.draw(rectangles[i]);
	}

	//Player
	RectangleShape s_player;

	mut.lock();
	s_player.setSize(Vector2f(P_WIDTH * BLOCKWIDTH, P_HEIGHT * BLOCKHEIGHT));
	s_player.setTexture(&textures[0]);
	s_player.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, (player.get_coordinates()->getY() - 1) * BLOCKHEIGHT);
	mut.unlock();

	gameWindow.draw(s_player);
	//Showing result
	gameWindow.display();

	delete[] rectangles;
}

void Engine::init_map()
{
	for (int i = 0; i < WIDTH; i++)
	{
		//Dirt blocks
		blocks.push_back(Block(i, 7, &textures[1]));
		//Stone blocks
		blocks.push_back(Block(i, 8, &textures[3]));
		blocks.push_back(Block(i, 9, &textures[3]));
		//Grass blocks
		blocks.push_back(Block(i, 6, &textures[2]));
	}
}
void Engine::start_game() 
{
	
	gameWindow.setVerticalSyncEnabled(true);
	get_textures();
	init_map();

	thread falling_thread(&Engine::falling, this);
  Music_On();



	while (gameWindow.isOpen())
	{
		Event ev;
		while (gameWindow.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				gameWindow.close();
			if (ev.type == Event::KeyPressed)
				control_enter(ev);
		}
		update_frame();
	}
	closing = true;

	falling_thread.join();
}
void Engine::get_textures() 
{
	Texture player;
	player.loadFromFile("Player.png");
	textures.push_back(player);

	Texture dirt;
	dirt.loadFromFile("Dirt.png");
	textures.push_back(dirt);

	Texture grass;
	grass.loadFromFile("Grass.png");
	textures.push_back(grass);

	Texture stone;
	stone.loadFromFile("Stone.png");
	textures.push_back(stone);
}

void Engine::control_enter(Event ev)
{
	if (ev.type == Event::KeyPressed)
	{
		mut.lock();
		switch (ev.key.code)
		{
		case Keyboard::Key::S:
			movePlayerDown();
			break;

		case Keyboard::Key::Space:
			movePlayerUp();
			break;

		case Keyboard::Key::A:
			movePlayerLeft();
			break;

		case Keyboard::Key::D:
			movePlayerRight();
			break;
		}
		mut.unlock();
	}
}

bool Engine::is_collided(unsigned int x, unsigned int y)
{
	vector<Block>::iterator iter = find_if(blocks.begin(), blocks.end(), [x, y](Block obj)
		{ return obj.get_coordinates()->getX() == x && obj.get_coordinates()->getY() == y; });
	return iter != blocks.end();
}
void Engine::movePlayerRight()
{
	int temp_X = player.get_coordinates()->getX();
	int temp_Y = player.get_coordinates()->getY();

	if (temp_X + P_WIDTH + 1 > WIDTH)
		return;

	for(int i = 0; i < P_HEIGHT; i++)
		if(is_collided(temp_X + P_WIDTH, temp_Y + i - 1))
			return;
	
	player.moveRight();
}
void Engine::movePlayerLeft()
{
	int temp_X = player.get_coordinates()->getX();
	int temp_Y = player.get_coordinates()->getY();

	if (temp_X - 1 < 0)
		return;

	for (int i = 0; i < P_HEIGHT; i++)
		if (is_collided(temp_X - 1, temp_Y + i - 1))
			return;

	player.moveLeft();
}
void Engine::movePlayerUp()
{
	int temp_X = player.get_coordinates()->getX();
	int temp_Y = player.get_coordinates()->getY();

	if (temp_Y - 1 <= 0)
		return;

	for (int i = 0; i < P_WIDTH; i++)
		if (is_collided(temp_X + i, temp_Y - 1))
			return;

	player.moveUp();
}
void Engine::movePlayerDown()
{
	int temp_X = player.get_coordinates()->getX();
	int temp_Y = player.get_coordinates()->getY();

	if (temp_Y + P_HEIGHT > HEIGHT)
		return;

	for (int i = 0; i < P_WIDTH; i++)
		if (is_collided(temp_X + i, temp_Y + P_HEIGHT - 1))
			return;

	player.moveDown();
}

void Engine::falling() 
{
	while (!closing)
	{
		mut.lock();
		movePlayerDown();
		mut.unlock();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
}