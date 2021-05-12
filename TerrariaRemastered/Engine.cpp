#include "Engine.h"

void Engine::update_frame() 
{
	gameWindow.clear(Color(BACKGROUND));
	//Ðèñóåì áëîêè
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

	//Ðèñóåì ïåðñîíàæà
	CircleShape head;
	RectangleShape body;

	head.setFillColor(Color::Yellow);
	head.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, (player.get_coordinates()->getY() - 1) * BLOCKHEIGHT);
	head.setRadius(BLOCKWIDTH / 2);

	body.setFillColor(Color::Cyan);
	body.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, player.get_coordinates()->getY() * BLOCKHEIGHT);
	body.setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));

	gameWindow.draw(head);
	gameWindow.draw(body);
	//Îòîáðàæàåì íà ýêðàíå
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
	//blocks.push_back(Block());
}
void Engine::start_game() 
{
	gameWindow.setVerticalSyncEnabled(true);

	init_map();

	int counter = 0;	//FOR TESTING

	while (gameWindow.isOpen())
	{
		Event ev;
		while (gameWindow.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				gameWindow.close();
		}

		update_frame();

		Sleep(500);
	}
}

void Engine::control_enter()
{

	if (ev.type == Event::KeyPressed)
	{
		switch (ev.key.code)
		{
		case Keyboard::Key::Down:
			player.moveDown();
			break;
			
		case Keyboard::Key::Up:
			player.moveUp();
			break;

		case Keyboard::Key::Left:
			player.moveLeft();
			break;

		case Keyboard::Key::Right:
			player.moveRight();
			break;
		}

		Sleep(500);
	}
}

bool Engine::is_collided(unsigned int x, unsigned int y)
{
	vector<Block>::iterator iter = find_if(blocks.begin(), blocks.end(), [x, y](Block obj)
		{ return obj.get_coordinates()->getX() == x && obj.get_coordinates()->getY() == y; });
	return iter != blocks.end();
}

void Engine::movePlayerRight(unsigned int x, unsigned int y)
{
	if (!is_collided(x, y) && x < WIDTH)
		player.moveRight();
}

void Engine::movePlayerLeft(unsigned int x, unsigned int y)
{
	if (!is_collided(x, y) && x >= 0)
		player.moveLeft();
}

void Engine::movePlayerUp(unsigned int x, unsigned int y)
{
	if (!is_collided(x, y) && y < HEIGHT)
		player.moveUp();
}

void Engine::movePlayerDown(unsigned int x, unsigned int y)
{
	if (!is_collided(x, y) && y >= 0)
		player.moveDown();
}