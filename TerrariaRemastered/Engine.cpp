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
	if(moving_left)
		s_player.setTexture(&textures[TexturesID::Left_Player_texture]);
	else
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
			selected_item.setPosition(INVENTORY_WIDTH * i, 0);

			gameWindow.draw(selected_item);
		}

		//Draw item icon
		if (temp_inventory[i].second != nullptr)
		{
			RectangleShape item_icon;
			item_icon.setSize(Vector2f(BLOCKWIDTH, BLOCKHEIGHT));
			item_icon.setTexture(temp_inventory[i].second->get_texture());
			item_icon.setPosition(INVENTORY_WIDTH / 2 - BLOCKWIDTH / 2 + INVENTORY_WIDTH * i, INVENTORY_HEIGHT / 2 - BLOCKHEIGHT / 2);

			Text count_label(to_string(temp_inventory[i].first), mainFont, 15);
			count_label.setPosition(INVENTORY_WIDTH / 2 - BLOCKWIDTH / 2 + INVENTORY_WIDTH * i, INVENTORY_HEIGHT / 2);
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
	if (!download_map(MAPFILE))
	{
		for (int i = 0; i < WIDTH; i++)
		{
			//Dirt blocks
			blocks.push_back(Block(i, HEIGHT - 3, "Dirt", &textures[BlockTextures::Dirt]));
			//Stone blocks
			blocks.push_back(Block(i, HEIGHT - 2, "Stone", &textures[BlockTextures::Stone]));
			blocks.push_back(Block(i, HEIGHT - 1, "Stone", &textures[BlockTextures::Stone]));
			//Grass blocks
			blocks.push_back(Block(i, HEIGHT - 4, "Grass", &textures[BlockTextures::Grass]));
		}
	}
}
void Engine::start_game() 
{
	get_textures();
	mainFont.loadFromFile("C:/WINDOWS/Fonts/arial.ttf");
	
	if (!dowload_inventory(INVFILE))
		player.get_inventory().put_item(new Tool("Pickaxe", &textures[TexturesID::Tool_t]));

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
			if (ev.type == ev.MouseButtonReleased && ev.mouseButton.button == Mouse::Left)
			{
				Coordinates* clicked_block = check_click(Mouse::getPosition(gameWindow));
				if (clicked_block != nullptr)
				{
					Tool* temp_tool = dynamic_cast<Tool*>(player.get_inventory().get_inventory()[inventory_index].second);
					if(temp_tool != nullptr)
						break_block(*clicked_block);

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
	dump_inventory(INVFILE);
	dump_map(MAPFILE);
}
void Engine::get_textures() 
{
	textures.resize(BlockTextures::Stone + 1);

	//Player textures
	Texture player;
	player.loadFromFile("Player.png");
	textures[TexturesID::Player_texture] = player;

	Texture left_player;
	left_player.loadFromFile("Player_Left.png");
	textures[TexturesID::Left_Player_texture] = left_player;

	//Inventory textures
	Texture inv_cell;
	inv_cell.loadFromFile("Inventory_cell.png");
	textures[TexturesID::Inventory_cell] = inv_cell;

	Texture selected_item;
	selected_item.loadFromFile("SelectedItem.png");
	textures[TexturesID::Selected_item] = selected_item;

	//Blocks textures
	Texture dirt;
	dirt.loadFromFile("Dirt.png");
	textures[BlockTextures::Dirt] = dirt;

	Texture grass;
	grass.loadFromFile("Grass.png");
	textures[BlockTextures::Grass] = grass;

	Texture stone;
	stone.loadFromFile("Stone.png");
	textures[BlockTextures::Stone] = stone;

	Texture leaves;
	leaves.loadFromFile("Stone.png");
	textures[BlockTextures::Leaves] = leaves;

	Texture tree;
	tree.loadFromFile("Stone.png");
	textures[BlockTextures::Tree] = tree;

	Texture metal;
	metal.loadFromFile("Stone.png");
	textures[BlockTextures::Metal] = metal;


	//Tools textures
	Texture pickaxe;
	pickaxe.loadFromFile("Pickaxe.png");
	textures[TexturesID::Tool_t] = pickaxe;
}

void Engine::control_enter(Event ev)
{
	if (ev.type == Event::KeyPressed)
	{
		if (ev.key.code == Keyboard::Key::Space)
		{
			thread jump_thr(&Engine::jump, this);
			jump_thr.detach();
		}
		else if (ev.key.code == Keyboard::Key::A)
		{
			movePlayerLeft();
			moving_left = true;
		}
		else if (ev.key.code == Keyboard::Key::D)
		{
			movePlayerRight();
			moving_left = false;
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
	{
		is_jumping = false;
		return;
	}

	for (int i = 0; i < P_WIDTH; i++)
		if (is_collided(temp_X + i, temp_Y - 2))
		{
			is_jumping = false;
			return;
		}

	player.moveUp();
	is_jumping = true;
}
void Engine::movePlayerDown()
{
	int temp_X = player.get_coordinates()->getX();
	int temp_Y = player.get_coordinates()->getY();

	if (temp_Y + P_HEIGHT > HEIGHT)
	{
		is_falling = false;
		return;
	}
		
	for (int i = 0; i < P_WIDTH; i++)
		if (is_collided(temp_X + i, temp_Y + P_HEIGHT - 1))
		{
			is_falling = false;
			return;
		}

	player.moveDown();
	is_falling = true;
}

void Engine::falling() 
{
	while (!closing)
	{
		if (!is_jumping)
		{
			mut.lock();
			movePlayerDown();
			mut.unlock();
		}
		this_thread::sleep_for(chrono::milliseconds(VERTICALTIME));
	}
}
void Engine::jump() 
{
	mut.lock();

	if (is_falling)
	{
		mut.unlock();
		return;
	}

	is_jumping = true;
	mut.unlock();

	for (int i = 0; i < JUMPHEIGHT; i++)
	{
		if (!is_jumping)
			return;
		mut.lock();
		movePlayerUp();
		is_falling = true;
		mut.unlock();
		this_thread::sleep_for(chrono::milliseconds(VERTICALTIME));
	}

	mut.lock();
	is_jumping = false;
	mut.unlock();
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
bool Engine::download_map(string fileName) 
{
	ifstream file;
	file.open(fileName);

	if (file.is_open())
	{
		unsigned int x_coord = 0, y_coord = 0;
		
		while (!is_end(&file))
		{
			string text_str;
			getline(file, text_str);

			int start_pos = 0, end_pos = 0;
			while (true) 
			{
				if (text_str[end_pos] != ' ' && ++end_pos != text_str.length())
					end_pos;
				else
				{
					string temp_str = text_str.substr(start_pos, end_pos - start_pos);
					int text_as_num = atoi(temp_str.c_str());
					string block_name;

					if (!text_as_num)
						return false;

					switch (text_as_num) 
					{
					case BlockTextures::Dirt:
						block_name = "Dirt";
						break;
					case BlockTextures::Stone:
						block_name = "Stone";
						break;
					case BlockTextures::Grass:
						block_name = "Grass";
						break;
					}

					if(block_name != "")
						blocks.push_back(Block(x_coord, y_coord, block_name, &textures[text_as_num]));

					x_coord++;
					start_pos = ++end_pos;
				}

				if (end_pos > text_str.length())
				{
					y_coord++;
					x_coord = 0;
					break;
				}
			}
		}
		
		file.close();

		return true;
	}

	return false;
}
void Engine::dump_map(string fileName)
{
	ofstream file;

	file.open(fileName);

	if (file.is_open())
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				vector<Block>::iterator iter = find_if(blocks.begin(), blocks.end(), [i, j](Block bl) { return bl.get_coordinates()->getX() == j && bl.get_coordinates()->getY() == i; });
				if (iter == blocks.end())
					file << 1;
				else
				{
					string name = iter->get_name();

					if (name == "Dirt")
						file << BlockTextures::Dirt;
					else if (name == "Stone")
						file << BlockTextures::Stone;
					else if (name == "Grass")
						file << BlockTextures::Grass;

					blocks.erase(iter);
				}

				file << " ";
			}
			file << endl;
		}
		file.close();
	}
}
bool Engine::dowload_inventory(string fileName) 
{
	ifstream file;

	file.open(fileName);

	if (file.is_open()) 
	{
		while (!is_end(&file))
		{
			string row;

			int start_pos = 0, space_pos = 0;
			getline(file, row);

			space_pos = find_space(row, start_pos);
			string name = row.substr(start_pos, space_pos);

			string count_text = row.substr(name.length(), row.length() - space_pos);

			int count = atoi(count_text.c_str());

			if (name == "Dirt")
				for(int i = 0; i < count; i++)
					player.get_inventory().put_item(new Block(0, 0, name, &textures[BlockTextures::Dirt]));
			
			else if (name == "Grass")
				for (int i = 0; i < count; i++)
					player.get_inventory().put_item(new Block(0, 0, name, &textures[BlockTextures::Grass]));

			else if (name == "Stone")
				for (int i = 0; i < count; i++)
					player.get_inventory().put_item(new Block(0, 0, name, &textures[BlockTextures::Stone]));

			else if (name == "Pickaxe")
				for (int i = 0; i < count; i++)
					player.get_inventory().put_item(new Tool(name, &textures[TexturesID::Tool_t]));
		}
		file.close();

		return true;
	}

	return false;
}
void Engine::dump_inventory(string fileName)
{
	ofstream file;

	file.open(fileName);

	if (file.is_open()) 
	{
		vector<pair<int, Item*>> temp_inventory = player.get_inventory().get_inventory();

		for (int i = 0; i < INVENTORY_SIZE; i++)
			if (temp_inventory[i].second != nullptr)
				file << temp_inventory[i].second->get_name() << " " << temp_inventory[i].first << endl;

		file.close();
	}
}
bool is_end(ifstream* stream)
{
	streampos base = stream->tellg();
	stream->seekg(0, ios::end);
	streampos end_pos = stream->tellg();
	bool result = base == end_pos;
	stream->clear();
	stream->seekg(base);

	return result;
}
int find_space(string str, int start_pos) 
{
	for (int i = start_pos; i < str.length(); i++)
		if (str[i] == ' ')
			return i;

	return -1;
}
void Engine::break_block(Coordinates coordinates)
{
	int X_0 = player.get_coordinates()->getX();
	int Y_0 = player.get_coordinates()->getY();
	int X = coordinates.getX();
	int Y = coordinates.getY();

	if (!is_in_range(coordinates))
		return;

	vector<Block>::iterator found = find_if(blocks.begin(), blocks.end(),
		[&coordinates](Block temp)
		{
			return temp.get_coordinates()->getX() == coordinates.getX()
				&& temp.get_coordinates()->getY() == coordinates.getY();
		});

	if (found == blocks.end())
		return;

	player.get_inventory().put_item(new Block(*found));

	blocks.erase(found);
}