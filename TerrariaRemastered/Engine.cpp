#include "Engine.h"

void Engine::update_frame() 
{
	gameWindow.clear(Color(BACKGROUND));
	//������ �����
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

	//������ ���������
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
	//���������� �� ������
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
}