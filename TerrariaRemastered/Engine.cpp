#include "Engine.h"

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
		rectangles[i].setPosition(blocks[i].get_coordinates()->getX() * BLOCKWIDTH, blocks[i].get_coordinates()->getY() * BLOCKHEIGHT + INVENTORY_HEIGHT);
		rectangles[i].setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
		gameWindow.draw(rectangles[i]);
	}

	//Player
	RectangleShape s_player;

	mut.lock();
	s_player.setSize(Vector2f(P_WIDTH * BLOCKWIDTH, P_HEIGHT * BLOCKHEIGHT));
	s_player.setTexture(&textures[TexturesID::Player_texture]);
	s_player.setPosition(player.get_coordinates()->getX() * BLOCKWIDTH, (player.get_coordinates()->getY() - 1) * BLOCKHEIGHT + INVENTORY_HEIGHT);
	mut.unlock();

	gameWindow.draw(s_player);

	//Inventory
	RectangleShape inventory_cells[INVENTORY_SIZE];
	//We get temp inventory, only for drawing
	vector<pair<int, Item*>> temp_inventory = player.get_inventory().get_inventory();
	for (int i = 0; i < INVENTORY_SIZE; i++)
	{
		inventory_cells[i].setSize(Vector2f(INVENTORY_WIDTH, INVENTORY_HEIGHT));
		inventory_cells[i].setTexture(&textures[TexturesID::Inventory_cell]);
		inventory_cells[i].setPosition(INVENTORY_HEIGHT * i, 0);
		gameWindow.draw(inventory_cells[i]);

		if (i == inventory_index)
		{
			RectangleShape selected_item;
			selected_item.setSize(Vector2f(INVENTORY_WIDTH, INVENTORY_HEIGHT));
			selected_item.setTexture(&textures[TexturesID::Selected_item]);
			selected_item.setPosition(INVENTORY_HEIGHT * i, 0);

			gameWindow.draw(selected_item);
		}

		//Draw item icon
		if (temp_inventory[i].second != nullptr)
		{
			RectangleShape item_icon;
			item_icon.setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
			//We set item origin as the left corner of rectangle
			item_icon.setOrigin(inventory_cells[i].getPosition());
			item_icon.setPosition(INVENTORY_WIDTH / 2 - BLOCKWIDTH / 2, INVENTORY_HEIGHT / 2 - BLOCKHEIGHT / 2);
			item_icon.setTexture(temp_inventory[i].second->get_texture());

			Text count_label(to_string(temp_inventory[i].first), mainFont, 15);
			count_label.setOrigin(inventory_cells[i].getPosition());
			count_label.setPosition(INVENTORY_WIDTH / 2 - BLOCKWIDTH / 2, INVENTORY_HEIGHT / 2);
			count_label.setFillColor(Color::White);

			gameWindow.draw(item_icon);
			gameWindow.draw(count_label);
		}
	}

	//Showing result
	gameWindow.display();

	delete[] rectangles;
}

void Engine::init_map()
{
	for (int i = 0; i < WIDTH; i++)
	{
		//Dirt blocks
		blocks.push_back(Block(i, 7, "Dirt", &textures[BlockTextures::Dirt]));
		//Stone blocks
		blocks.push_back(Block(i, 8, "Stone", &textures[BlockTextures::Stone]));
		blocks.push_back(Block(i, 9, "Stone", &textures[BlockTextures::Stone]));
		//Grass blocks
		blocks.push_back(Block(i, 6, "Grass", &textures[BlockTextures::Grass]));
	}
}
void Engine::start_game() 
{
	gameWindow.setVerticalSyncEnabled(true);
	get_textures();
	init_map();
	mainFont.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");

	thread falling_thread(&Engine::falling, this);
	Music_On();

	//Only for testing
	Block test_block(0, 0, "Grass", &textures[BlockTextures::Grass]);
	for (int i = 0; i < 10; i++)
		player.get_inventory().put_item(new Block(test_block));

	while (gameWindow.isOpen())
	{
		Event ev;
		while (gameWindow.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				gameWindow.close();
			if (ev.type == ev.MouseButtonReleased && ev.mouseButton.button == Mouse::Right)
			{
				Coordinates* clicked_block = check_click(Mouse::getPosition(gameWindow));
				if (clicked_block != nullptr)
				{
					Block* temp_block = dynamic_cast<Block*>(player.get_inventory().get_inventory()[inventory_index].second);
					if (temp_block != nullptr)
						if(place_block(*clicked_block, *temp_block))
							delete player.get_inventory().get_item(inventory_index);	//Here inventory returns new object, we don't need it, so we delete it immediately
					
					delete clicked_block;
				}
			}
			if (ev.type == Event::KeyPressed)
				control_enter(ev);
			if (ev.type == Event::MouseWheelScrolled)
			{
				if (ev.mouseWheelScroll.delta > 0)
					inventory_index--;
				else if (ev.mouseWheelScroll.delta < 0)
					inventory_index++;

				if (inventory_index == INVENTORY_SIZE)
					inventory_index = 0;
				else if (inventory_index < 0)
					inventory_index = INVENTORY_SIZE - 1;
			}
		}
		update_frame();
	}
	closing = true;

	falling_thread.join();
}
void Engine::get_textures() 
{
	textures.resize(BlockTextures::Stone + 1);

	Texture player;
	player.loadFromFile("Player.png");
	textures[TexturesID::Player_texture] = player;

	Texture inv_cell;
	inv_cell.loadFromFile("Inventory_cell.png");
	textures[TexturesID::Inventory_cell] = inv_cell;

	Texture selected_item;
	selected_item.loadFromFile("SelectedItem.png");
	textures[TexturesID::Selected_item] = selected_item;

	Texture dirt;
	dirt.loadFromFile("Dirt.png");
	textures[BlockTextures::Dirt] = dirt;

	Texture grass;
	grass.loadFromFile("Grass.png");
	textures[BlockTextures::Grass] = grass;

	Texture stone;
	stone.loadFromFile("Stone.png");
	textures[BlockTextures::Stone] = stone;
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
		if (is_collided(temp_X + i, temp_Y - 2))
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
void Engine::Music_On()
{
	music.openFromFile("8_Bit_Surf.ogg");
	music.play();
	music.setLoop(true);
}
Coordinates* Engine::check_click(Vector2i mouse_coordinates) 
{
	if (mouse_coordinates.y <= INVENTORY_HEIGHT)
		return nullptr;
	return new Coordinates(mouse_coordinates.x / BLOCKWIDTH, (mouse_coordinates.y - INVENTORY_HEIGHT) / BLOCKHEIGHT);
}
bool Engine::place_block(Coordinates coordinates, Block block) 
{
	int X_0 = player.get_coordinates()->getX();
	int Y_0 = player.get_coordinates()->getY();
	int X = coordinates.getX();
	int Y = coordinates.getY();

	if (!is_in_range(coordinates) || (X >= X_0 && X <= X_0 + P_WIDTH - 1 && Y >= Y_0 - 1 && Y < Y_0 + P_HEIGHT - 1))
		return false;

	vector<Block>::iterator found = find_if(blocks.begin(), blocks.end(), 
		[&coordinates](Block temp) 
		{ 
			return temp.get_coordinates()->getX() == coordinates.getX()
				&& temp.get_coordinates()->getY() == coordinates.getY(); 
		});
	
	if (found != blocks.end())
		return false;

	block.get_coordinates()->setX(coordinates.getX());
	block.get_coordinates()->setY(coordinates.getY());

	blocks.push_back(block);

	return true;
}
bool Engine::is_in_range(Coordinates coordinates)
{
	double x_0 = (double)player.get_coordinates()->getX() + (P_WIDTH - 1) / 2.;
	double y_0 = (double)player.get_coordinates()->getY() + (P_HEIGHT - 3) / 2.;

	return sqrt(pow(coordinates.getX() - x_0, 2) + pow(coordinates.getY() - y_0, 2)) <= RADIUS;
}