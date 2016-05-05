// Sfml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#define CELL_SIZE 32
#define HEIGHT 3 * CELL_SIZE

using namespace sf;

enum state_of_cell
{
	N0,		// 0
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,	
	MINE,	// 9
	CLOSED, // 10
	FLAG,	// 11
	MARK,	// 12
	CL_FLAG // 13
};

class cl_cell
{
public:
	Sprite sprite;
	state_of_cell state;					/* to define which part of texture is used. */
	state_of_cell value;					/* 0 - 8 mines around. 9 if mine. */
	int mine;								/* amount of mines. */
	bool open;								/* state of cell during work of DFS. */
	int dx[8] = { 0,0,1,-1,1,1,-1,-1 };		/* for DFS */
	int dy[8] = { 1,-1,1,1,0,-1,-1,0 };
	cl_cell()
	{
		state = CLOSED;
		value = N0;
		mine = 0;
		open = false;
	};

	void change_value(state_of_cell val)	
	{
		value = val;
	}

	void change_state(state_of_cell st)		/* when Right button pressed. */
	{
		state = st;
		sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
	}

	void change_texture()					/* "open" the cell. */
	{
		state = value;
		sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
	}

};

class field
{
public:
	int width;								/* width of field in pixels. */
	int height;								/* height of field in pixels. */
	int amount_of_mines; 
	int n_x;								/* width of field in cells. */
	int n_y;								/* height of field in cells. */
	bool token;								/* to call generator. */
	bool game;								/* to finish the game. */
	int mode;
	int left;
	cl_cell **Cell;							/* array of cells. */
	Texture texture;
	Texture tex_for_top;
	Texture tex_time;
	Texture tex_smile;
	Sprite top_sprite;
	Sprite count_sp;
	Sprite time_sp;
	Sprite smile;
	Font font;
	Text time;
	Text count;
	Clock game_clock;
	Clock temp;
	Time elapsed_time;

	field(int W, int H, int Am, int M) 
	{
		width = W;					
		height = H;					
		amount_of_mines = Am;
		mode = M;
		token = true;						
		game = true;	
		left = amount_of_mines;
	}

	void make_sprites()
	{
		/* loading the texture for sprites. */

		texture.loadFromFile("images/Texture1.png");
		tex_for_top.loadFromFile("images/top.png");
		tex_time.loadFromFile("images/for_top.png");
		tex_smile.loadFromFile("images/for_top.png");

		/* loading the font. */

		font.loadFromFile("digital-7.ttf");


		top_sprite.setPosition(0, 0);
		top_sprite.setTexture(tex_for_top);
		top_sprite.setTextureRect(sf::IntRect(0, 0, width, HEIGHT));

		smile.setPosition((width / 2) - CELL_SIZE, CELL_SIZE / 2);
		smile.setTexture(tex_smile);
		smile.setTextureRect(sf::IntRect(0, 0, 2 * CELL_SIZE, 2 * CELL_SIZE));

		count_sp.setPosition(width - 3 * CELL_SIZE - 4, CELL_SIZE / 2);
		count_sp.setTexture(tex_smile);
		count_sp.setTextureRect(sf::IntRect(4 * CELL_SIZE, 0, 3 * CELL_SIZE, 2 * CELL_SIZE));

		time_sp.setPosition(4, CELL_SIZE / 2);
		time_sp.setTexture(tex_smile);
		time_sp.setTextureRect(sf::IntRect(4 * CELL_SIZE, 0, 3 * CELL_SIZE, 2 * CELL_SIZE));

		time.setFont(font);
		time.setCharacterSize(75);
		time.setColor(sf::Color::Red);
		time.setPosition(10, 0);

		count.setFont(font);
		count.setCharacterSize(75);
		count.setColor(sf::Color::Red);
		count.setPosition(width - 75, 0);

	}

	/* Create a two-dimensional array of type cl_cell. */

	void create_field()
	{

		/* define the width and the height of the field in cells. */

		n_x = width / CELL_SIZE;
		n_y = (height - HEIGHT) / CELL_SIZE;

		Cell = new cl_cell * [n_y];

		for (int i = 0; i < n_y; i++)
		{
			Cell[i] = new cl_cell[n_x];
		}
	}

	/* Setting texture to each cell. */

	void set_texture()
	{
		for (int i = 0; i < n_y; i++)
		{
			for (int j = 0; j < n_x; j++)
			{
				if (mode == 2)
					Cell[i][j].change_state(CL_FLAG);
				Cell[i][j].sprite.setPosition(j * 32, i * 32 + HEIGHT);
				Cell[i][j].sprite.setTexture(texture);
				Cell[i][j].sprite.setTextureRect(IntRect(Cell[i][j].state * 32, 0, 32, 32));
			}
		}
	}

	/* Place mines on the field and count value for each cell. */

	void generator(int x, int y)
	{

		/* place mines and change value for cell that was chosen. */


		/* at first change value of near cells to avoid the posibility of placing mines in them */

		if ((y > 0) && (y + 1 < n_y) && (x > 0) && (x + 1 < n_x))	/* if not a borden cell was chosen. */
		{															
			for (int k = y - 1; k <= y + 1; k++)					
			{														
				for (int m = x - 1; m <= x + 1; m++)					
				{														
					Cell[k][m].value = N1;								
				}													
			}											
		}
		else													/* if a borden cell was chosen. */
		{
			Cell[y][x].value = N1;
			if ((y - 1 >= 0) && (x - 1 >= 0))
			{
				Cell[y - 1][x - 1].value = N1;
			}
			if (y - 1 >= 0)
			{
				Cell[y - 1][x].value = N1;
			}
			if ((y - 1 >= 0) && (x + 1 < n_x))
			{
				Cell[y - 1][x + 1].value = N1;
			}
			if (x + 1 < n_x)
			{
				Cell[y][x + 1].value = N1;
			}
			if (x - 1 >= 0)
			{
				Cell[y][x - 1].value = N1;
			}
			if ((y + 1 < n_y) && (x - 1 >= 0))
			{
				Cell[y + 1][x - 1].value = N1;
			}
			if (y + 1 < n_y)
			{
				Cell[y + 1][x].value = N1;
			}
			if ((y + 1 < n_y) && (x + 1 < n_x))
			{
				Cell[y + 1][x + 1].value = N1;
			}


		}

		/* place mines. */
		
		int new_i, new_j;

		for (int n = 0; n < amount_of_mines; n++)
		{
			std::srand(std::time(NULL));
			do
			{
				new_i = (std::rand() % n_y);
				new_j = (std::rand() % n_x);
			} while ((Cell[new_i][new_j].value == MINE) || (Cell[new_i][new_j].value == N1));
			Cell[new_i][new_j].value = MINE;
		}
		
		for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				if (Cell[k][m].value != MINE)
				Cell[k][m].value = N0;
			}
		}
		
		/* define the amount of mines near for each cell. */

		for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				if (Cell[k][m].value == MINE)
				{
					if ((k - 1 >= 0) && (m - 1 >= 0))
					{
						if (Cell[k - 1][m - 1].value != MINE)
							Cell[k - 1][m - 1].mine++;
					}
					if (k - 1 >= 0)
					{
						if (Cell[k - 1][m].value != MINE)
							Cell[k - 1][m].mine++;
					}
					if ((k - 1 >= 0) && (m + 1 < n_x))
					{
						if (Cell[k - 1][m + 1].value != MINE)
							Cell[k - 1][m + 1].mine++;
					}
					if (m + 1 < n_x)
					{
						if (Cell[k][m + 1].value != MINE)
							Cell[k][m + 1].mine++;
					}
					if (m - 1 >= 0)
					{
						if (Cell[k][m - 1].value != MINE)
							Cell[k][m - 1].mine++;
					}
					if ((k + 1 < n_y) && (m - 1 >= 0))
					{
						if (Cell[k + 1][m - 1].value != MINE)
							Cell[k + 1][m - 1].mine++;
					}
					if (k + 1 < n_y)
					{
						if (Cell[k + 1][m].value != MINE)
							Cell[k + 1][m].mine++;
					}
					if ((k + 1 < n_y) && (m + 1 < n_x))
					{
						if (Cell[k + 1][m + 1].value != MINE)
							Cell[k + 1][m + 1].mine++;
					}
				}
			}
		}

		/* change the value of each cell according to defines amount of mines near. */

		for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				switch (Cell[k][m].mine)
				{
				case 1:
					Cell[k][m].value = N1;
					break;
				case 2:
					Cell[k][m].value = N2;
					break;
				case 3:
					Cell[k][m].value = N3;
					break;
				case 4:
					Cell[k][m].value = N4;
					break;
				case 5:
					Cell[k][m].value = N5;
					break;
				case 6:
					Cell[k][m].value = N6;
					break;
				case 7:
					Cell[k][m].value = N7;
					break;
				case 8:
					Cell[k][m].value = N8;
					break;
				default:
					break;
				}
			}
		}

		/* if we need to show field after generation. */

		/*for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				Cell[k][m].change_texture();
			}
		}*/

		/* call dfs dor opennig empty cell. */

		dfs(x, y);

		/* change value of token to not call generator any more. */

		token = false;

		game_clock.restart();
	}

	/* Depth-First Search -- to open cell without mines with recursion. */

	void dfs(int x, int y)
	{
		const int n_x = width / CELL_SIZE;
		const int n_y = height / CELL_SIZE - HEIGHT / CELL_SIZE;

		if (Cell[y][x].open == true)
			return;
		Cell[y][x].open = true;
		Cell[y][x].change_texture();
		if (Cell[y][x].value == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				if ((x + Cell[y][x].dx[i] >= 0) && (x + Cell[y][x].dx[i] < n_x) && (y + Cell[y][x].dy[i] >= 0) && (y + Cell[y][x].dy[i] < n_y))
					dfs(x + Cell[y][x].dx[i], y + Cell[y][x].dy[i]);
			}
		}
	}

	/* Open all cell near if all flags placed*/

	void Open_All_Around(int x, int y)
	{
		int count_for_flag = 0;
		for (int i = 0; i < 8; i++)
		{
			int new_x = x + Cell[y][x].dx[i];
			int new_y = y + Cell[y][x].dy[i];
			if ((new_x >= 0) && (new_x < n_x) && (new_y >= 0) && (new_y < n_y))
			{
				if (Cell[new_y][new_x].state == FLAG)
					count_for_flag++;
			}
		}
		if (count_for_flag == Cell[y][x].value)
		{
			for (int i = 0; i < 8; i++)
			{
				int new_x = x + Cell[y][x].dx[i];
				int new_y = y + Cell[y][x].dy[i];
				if ((new_x >= 0) && (new_x < n_x) && (new_y >= 0) && (new_y < n_y))
				{
					if (Cell[new_y][new_x].state != FLAG)
					{
						if (Cell[new_y][new_x].value == N0)
							dfs(new_x, new_y);
						Cell[new_y][new_x].change_texture();
						if (Cell[new_y][new_x].value == MINE)
							game = false;
					}
				}
			}
		}
	}

	/* Open all mines when game is over. */

	void Open_mines()
	{
		for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				if (Cell[k][m].value == MINE)
					Cell[k][m].change_texture();
			}
		}
	}

	/* Close all cells. */

	void Close_cells(int mode)
	{
		if (mode == 1)
		{
			for (int k = 0; k < n_y; k++)
			{
				for (int m = 0; m < n_x; m++)
				{
					Cell[k][m].state = CLOSED;
					Cell[k][m].value = N0;
					Cell[k][m].mine = 0;
					Cell[k][m].open = false;
					Cell[k][m].change_state(CLOSED);
				}
			}
		}

		if (mode == 2)
		{
			for (int k = 0; k < n_y; k++)
			{
				for (int m = 0; m < n_x; m++)
				{
					Cell[k][m].state = CL_FLAG;
					Cell[k][m].value = N0;
					Cell[k][m].mine = 0;
					Cell[k][m].open = false;
					Cell[k][m].change_state(CL_FLAG);
				}
			}
		}
		
			

		left = amount_of_mines;
	}

	/* function for depicting time and amount of mines that not found. */

	void Define_Text()
	{
		std::stringstream counter;
		std::string c;
		counter << left;
		counter >> c;
		count.setString(c);

		std::stringstream timer;
		std::string str;

		if (game == 1 && left > 0)
		{
			temp = game_clock;

			elapsed_time = temp.getElapsedTime();
			timer << (int)elapsed_time.asSeconds();
			timer >> str;

			time.setString(str);
		}
	}

	/* check field after play. */

	bool Check()
	{
		int counter = 0;

		for (int k = 0; k < n_y; k++)
		{
			for (int m = 0; m < n_x; m++)
			{
				if (Cell[k][m].value == MINE && Cell[k][m].state == FLAG)
					counter++;
			}
		}
		if (counter == amount_of_mines)
			return true;
		else
			return false;

	}
	
	/* Actions for Right button pressed*/

	void Right_Button_for_m2(int x, int y)
	{
		/* call generator if our field is empty. */

		if (token == true)
		{
			game_clock.restart();
			generator(x, y);

		}
		else
		{
			if (Cell[y][x].state == CL_FLAG)
			{
				if (Cell[y][x].value == N0)
					dfs(x, y);
				else
					Cell[y][x].change_texture();
				if (Cell[y][x].value == MINE)
				{
					game = false;
				}
			}
			/*if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
			Open_All_Around(x, y);*/
		}
	}

	void Right_Button(int x, int y)
	{
		/* for mode 2. */
		if (mode == 2)
		{
			Right_Button_for_m2(x, y);
			return;
		}
		
		/* for mode 1. */
		while (1)
		{
			if (Cell[y][x].state == CLOSED)
			{
				Cell[y][x].change_state(FLAG);
				left--;
				break;
			}
			if (Cell[y][x].state == FLAG)
			{
				Cell[y][x].change_state(MARK);
				left++;
				break;
			}
			if (Cell[y][x].state == MARK)
			{
				Cell[y][x].change_state(CLOSED);
				break;
			}
			break;
		}
	}

	/* Actions for Left button pressed*/

	void Left_Buttun_for_m2(int x, int y)
	{
		if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
			Open_All_Around(x, y);

		while (1)
		{

			if ((Cell[y][x].state == FLAG) &&  (token == false))
			{
				Cell[y][x].change_state(CL_FLAG);
				left++;
				break;
			}
			if ((Cell[y][x].state == CL_FLAG) && (token == false))
			{
				Cell[y][x].change_state(FLAG);
				left--;
				break;
			}
			break;
		}

	}

	void Left_Button(int x, int y)
	{
		/* for mode 2. */
		if (mode == 2)
		{
			Left_Buttun_for_m2(x, y);
			return;
		}
		/* for mode 1. */
		if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
		{
			Open_All_Around(x, y);
		}

		/* call generator if our field is empty. */

		if (token == true)
		{
			game_clock.restart();
			generator(x, y);

		}
		else
		{
			if (Cell[y][x].value == N0)
				dfs(x, y);
			else
				Cell[y][x].change_texture();
			if (Cell[y][x].value == MINE)
			{
				game = false;
			}
		}
	}
};


int Mode = 1;
int Mines = 0;
int Width;
int Height;
int game = 1;			// 1 - continue; 0 - failed;
int token = 0;			// define Menu/Game		

/* choosing game mode. */

void choose_mode(int &m, int &w, int &h, int x, int y)
{
	
	int flag = 0;

	if (x >= 305 && x <= 496 && y >= 115 && y <= 188)
		flag = 1;
	if (x >= 305 && x <= 496 && y >= 198 && y <= 273)
		flag = 2;
	if (x >= 305 && x <= 496 && y >= 283 && y <= 358)
		flag = 3;
	
	switch (flag)
	{
	case 1:
		m = 15;
		w = 320;
		h = 320;
		break;
	case 2:
		m = 40;
		w = 480;
		h = 480;
		break;
	case 3:
		m = 80;
		w = 640;
		h = 640;
		break;
	
	default:
		break;
	}
}

/* create window with menu. choose field size and game mode. */

bool Menu()
{
	int tex = 1;  /* to define texture. */

	RenderWindow menu_window(sf::VideoMode(800, 450), "MineSweeper_Menu");
	Sprite Menu;
	Texture tex_menu;
	tex_menu.loadFromFile("images/menu.png");
	Menu.setPosition(0, 0);
	Menu.setTexture(tex_menu);
	Menu.setTextureRect(sf::IntRect(800, 0, 1600, 450));


	while (menu_window.isOpen())
	{
		Event event;
		while (menu_window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				menu_window.close();
				return false;
				break;
			case Event::MouseButtonPressed:
				int x = Mouse::getPosition(menu_window).x;
				int y = Mouse::getPosition(menu_window).y;

				if (x >= 215 && x <= 262 && y >= 381 && y <= 427)
					while (1)
					{
						if (tex == 1)
						{
							Menu.setTextureRect(sf::IntRect(0, 0, 800, 450));
							Mode = 2;
							tex = 0;
							break;
						}
						if (tex == 0)
						{
							Menu.setTextureRect(sf::IntRect(800, 0, 1600, 450));
							Mode = 1;
							tex = 1;
							break;
						}
						break;
					}
				else
					choose_mode(Mines, Width, Height, x, y);
				if (Mines != 0)		/* field sized was chosen, start the game. */
				{
					token = 1;
					menu_window.close();
				}
			}
		}

		menu_window.clear();
		menu_window.draw(Menu);
		menu_window.display();
	}
	return true;
}

/* window with game. */

bool Game()
{
	/* create the instance of class "field" with specified arguments */

	field game_field(Width, Height + HEIGHT, Mines, Mode);

	/* create the window with arguments from class. */

	RenderWindow game_field_w(sf::VideoMode(game_field.width, game_field.height), "MineSweeper");

	/* making sprites. */

	game_field.make_sprites();

	/* create a two-dimensional array of type cl_cell. */

	game_field.create_field();

	/* setting texture to each sprite. */

	game_field.set_texture();

	/* main cycle. */

	while (game_field_w.isOpen())
	{
		Event event;
		while (game_field_w.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				game_field_w.close();
				return false;

			case Event::MouseButtonPressed:

				/* back to menu. */

				if ((Mouse::getPosition(game_field_w).x >= 0) &&
					(Mouse::getPosition(game_field_w).x <= 48) &&
					(Mouse::getPosition(game_field_w).y >= 0) &&
					(Mouse::getPosition(game_field_w).y <= 16))
				{
					token = 0;
					break;
				}

				/* start new game (with a smile). */

				if ((Mouse::getPosition(game_field_w).x >= game_field.width / 2 - CELL_SIZE) &&
					(Mouse::getPosition(game_field_w).x <= game_field.width / 2 + CELL_SIZE) &&
					(Mouse::getPosition(game_field_w).y >= CELL_SIZE / 2) &&
					(Mouse::getPosition(game_field_w).y <= CELL_SIZE / 2 + 2 * CELL_SIZE))
				{
					game_field.smile.setTextureRect(sf::IntRect(0, 0, 2 * CELL_SIZE, 2 * CELL_SIZE));
					game_field.Close_cells(Mode);
					game_field.token = true;
					game = 1;
					game_field.game_clock.restart();
					game_field.game = true;
					break;
				}


				int x = Mouse::getPosition(game_field_w).x / CELL_SIZE;
				int y = Mouse::getPosition(game_field_w).y / CELL_SIZE - HEIGHT / CELL_SIZE;
				if ((y >= 0) && (game == 1))
				{
					if (event.mouseButton.button == Mouse::Right)

						game_field.Right_Button(x, y);

					if (event.mouseButton.button == Mouse::Left)

						game_field.Left_Button(x, y);
				}
				break;
			}
		}

		if (token == 0)
		{
			Mode = 1;
			Mines = 0;
			Width = 0;
			Height = 0;
			game_field_w.close();
			break;
		}

		game_field.Define_Text();

		game_field_w.clear();

		for (int i = 0; i < game_field.n_y; i++)
		{
			for (int j = 0; j < game_field.n_x; j++)
			{
				game_field_w.draw(game_field.Cell[i][j].sprite);
			}
		}

		game_field_w.draw(game_field.top_sprite);
		game_field_w.draw(game_field.smile);
		game_field_w.draw(game_field.time_sp);
		game_field_w.draw(game_field.count_sp);
		if (game_field.token == false)
		{
			game_field_w.draw(game_field.time);
			game_field_w.draw(game_field.count);
		}

		game_field_w.display();

		/* if all flags are placed. */

		if (game_field.left == 0)
			if (game_field.Check())
			{
				RenderWindow win_w(sf::VideoMode(250, 120), "Congratulations");
				Sprite win;
				Texture tex_win;
				tex_win.loadFromFile("images/WIN.png");
				win.setTexture(tex_win);

				while (win_w.isOpen())
				{
					Event w_event;
					while (win_w.pollEvent(w_event))
					{
						switch (w_event.type)
						{
						case Event::Closed:
							win_w.close();
							break;
						}
					}
					win_w.clear();
					win_w.draw(win);
					win_w.display();
				}
				game_field.left = game_field.amount_of_mines;
				break;
			}
			else
				game_field.game = false;

		/* If game is over. */

		if (game_field.game == false)
		{
			game_field.Open_mines();

			/* Redraw window after openning mines. */

			game_field_w.clear();

			game_field.smile.setTextureRect(sf::IntRect(2 * CELL_SIZE, 0, 2 * CELL_SIZE, 2 * CELL_SIZE));

			for (int i = 0; i < game_field.n_y; i++)
			{
				for (int j = 0; j < game_field.n_x; j++)
				{
					game_field_w.draw(game_field.Cell[i][j].sprite);
				}
			}

			game_field_w.draw(game_field.top_sprite);
			game_field_w.draw(game_field.smile);
			game_field_w.draw(game_field.time_sp);
			game_field_w.draw(game_field.count_sp);
			game_field_w.draw(game_field.time);
			game_field_w.draw(game_field.count);

			game_field_w.display();

			game_field.elapsed_time = game_field.game_clock.getElapsedTime();
			game = 0;
			continue;
		}
	}
	return true;
}

int main()
{
	while (1)
	{
		if (token == 0)
			if (!Menu())
				break;
		if (token == 1)
			if (!Game())
				break;
	}
	return 0;
}
