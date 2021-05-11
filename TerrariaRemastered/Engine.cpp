#include "Engine.h"

void Engine::update_frame() 
{
	gameWindow.clear(Color(BACKGROUND));
	//Рисуем блоки
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

	//Рисуем персонажа
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
	//Отображаем на экране
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
		
		if (!(counter++ % 5))
		{
			switch (rand() % 4)
			{
			case 0:
				player.moveDown();
				break;
			case 1:
				player.moveUp();
				break;
			case 2:
				player.moveLeft();
				break;
			case 3:
				player.moveRight();
				break;
			}
		}
		Sleep(500);
	}
}