#include "Engine.h"

void Engine::update_frame() 
{
	gameWindow.clear(Color(BACKGROUND));
	//Blocks
	RectangleShape* rectangles = new RectangleShape[blocks.size()];

	for (int i = 0; i < blocks.size(); i++)
	{
		rectangles[i].setFillColor(blocks[i].get_color());
		rectangles[i].setOutlineColor(Color(0, 0, 0));
		rectangles[i].setOutlineThickness(1);
		rectangles[i].setPosition(blocks[i].get_coordinates()->getX() * BLOCKWIDTH, blocks[i].get_coordinates()->getY() * BLOCKHEIGHT);
		rectangles[i].setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
		gameWindow.draw(rectangles[i]);
	}

	//Player
	CircleShape head;
	RectangleShape body;

	mut.lock();
	head.setFillColor(Color::Yellow);
	head.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, (player.get_coordinates()->getY() - 1) * BLOCKHEIGHT);
	head.setRadius(BLOCKWIDTH / 2);

	body.setFillColor(Color::Cyan);
	body.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, player.get_coordinates()->getY() * BLOCKHEIGHT);
	body.setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
	mut.unlock();

	gameWindow.draw(head);
	gameWindow.draw(body);
	//Showing result
	gameWindow.display();

	delete[] rectangles;
}

void Engine::init_map()
{
	for (int i = 0; i < WIDTH; i++)
	{
		//Green blocks
		blocks.push_back(Block(i, 5, Color::Green));
		blocks.push_back(Block(i, 6, Color::Green));
		//Brown blocks
		blocks.push_back(Block(i, 7, Color(50, 30, 1)));	
		blocks.push_back(Block(i, 8, Color(50, 30, 1)));
		blocks.push_back(Block(i, 9, Color(50, 30, 1)));
	}
}
void Engine::start_game() 
{
	
	gameWindow.setVerticalSyncEnabled(true);

	init_map();

	thread falling_thread(&Engine::falling, this);

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
	if (!is_collided(player.get_coordinates()->getX() + 1, player.get_coordinates()->getY()) &&
		player.get_coordinates()->getX() + 1 < WIDTH)
		player.moveRight();
}
void Engine::movePlayerLeft()
{
	if (!is_collided(player.get_coordinates()->getX() - 1, player.get_coordinates()->getY()) &&
		player.get_coordinates()->getX() > 0)
		player.moveLeft();
}
void Engine::movePlayerUp()
{
	if (!is_collided(player.get_coordinates()->getX(), player.get_coordinates()->getY() - 1) &&
		player.get_coordinates()->getY() > 1 && !is_collided(player.get_coordinates()->getX(), player.get_coordinates()->getY() - 2))
		player.moveUp();
}
void Engine::movePlayerDown()
{
	if (!is_collided(player.get_coordinates()->getX(), player.get_coordinates()->getY() + 1) &&
		player.get_coordinates()->getY() + 1 < HEIGHT)
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