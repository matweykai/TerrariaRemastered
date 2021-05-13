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
}
void Engine::start_game() 
{
	gameWindow.setVerticalSyncEnabled(true);

	init_map();

	control_enter();

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
	while (gameWindow.isOpen()) 
	{
		Event ev;
		while (gameWindow.pollEvent(ev))
		{
			if (ev.type == Event::KeyPressed)
			{
				switch (ev.key.code)
				{
				case Keyboard::Key::Down:
					movePlayerDown();
					break;

				case Keyboard::Key::Up:
					movePlayerUp();
					break;

				case Keyboard::Key::Left:
					movePlayerLeft();
					break;

				case Keyboard::Key::Right:
					movePlayerRight();
					break;
				}
				Sleep(500);
			}
		}
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
		player.get_coordinates()->getX() >= 0)
		player.moveLeft();
}

void Engine::movePlayerUp()
{
	if (!is_collided(player.get_coordinates()->getX(), player.get_coordinates()->getY() - 1) &&
		player.get_coordinates()->getY() >= 0)
		player.moveUp();
}

void Engine::movePlayerDown()
{
	if (!is_collided(player.get_coordinates()->getX(), player.get_coordinates()->getY() + 1) &&
		player.get_coordinates()->getY() < HEIGHT)
		player.moveDown();
}