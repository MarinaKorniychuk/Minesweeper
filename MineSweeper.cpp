// Sfml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "string"
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#define CELL_SIZE 32

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
	MARK	// 12
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
	cl_cell **Cell;							/* array of cells. */
	Texture texture;
	Clock game_clock;
	Time elapsed_time;
	field(int W, int H, int Am, int M) 
	{
		width = W;					
		height = H;					
		amount_of_mines = Am;
		mode = M;
		token = true;						
		game = true;						
	}

	
	/* Create a two-dimensional array of type cl_cell. */

	void create_field()
	{
		Cell = new cl_cell *[n_y];

		for (int i = 0; i < n_y; i++)
		{
			Cell[i] = new cl_cell[n_x];
		}
	}

	/* Setting texture to each cell. */

	void set_texture()
	{
		if (mode == 1)
		{
				for (int i = 0; i < n_y; i++)
			{
				for (int j = 0; j < n_x; j++)
				{
					Cell[i][j].sprite.setPosition(j * 32, i * 32);
					Cell[i][j].sprite.setTexture(texture);
					Cell[i][j].sprite.setTextureRect(IntRect(Cell[i][j].state * 32, 0, 32, 32));
				}
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
				cell[k][m].change_texture();
			}
		}*/

		/* call dfs dor opennig empty cell. */

		dfs(x, y);

		/* change value of token to not call generator any more. */

		token = false;
	}

	/* Depth-First Search -- to open cell without mines with recursion. */

	void dfs(int x, int y)
	{
		const int n_x = width / CELL_SIZE;
		const int n_y = height / CELL_SIZE;

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

	/* Actions for Right button pressed*/

	void Right_Button(int x, int y)
	{
		/* place FLAG/QUESTION/CLOSED. */

		while (1)
		{
			if (Cell[y][x].state == CLOSED)
			{
				Cell[y][x].change_state(FLAG);
				break;
			}
			if (Cell[y][x].state == FLAG)
			{
				Cell[y][x].change_state(MARK);
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

	void Left_Button(int x, int y)
	{

		if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
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
				//game_field_w.close();
			}
		}
	}
};

/* Choosing game mode. */

void choose_mode(int &m, int &w, int &h, int &Mode)
{
	std::cout << "Chosee field size and amount of mines:\n";
	std::cout << "1. 10x10; 10m.\n2. 15x15; 50m.\n3. 20x20; 150m.\n4. Special.\n\n";

	int mode;
	scanf_s("%d", &mode);
	switch (mode)
	{
	case 1:
		m = 10;
		w = 320;
		h = 320;
		break;
	case 2:
		m = 40;
		w = 480;
		h = 480;
		break;
	case 3:
		m = 150;
		w = 640;
		h = 640;
		break;
	case 4:
		std::cout << "Enter the width ( <=20 ): ";
		scanf_s("%d", &w);
		w = w * CELL_SIZE;
		std::cout << "\nEnter the height ( <=20 ): ";
		scanf_s("%d", &h);
		h = h * CELL_SIZE;
		std::cout << "\nEnter the amount of mines ( <400 ): ";
		scanf_s("%d", &m);
		break;
	default:
		break;
	}

	std::cout << "\nChose the game mode (1 or 2): ";
	scanf_s("%d", &Mode);
}


const std::string texture = "images/Texture.png";		

int main()
{
	int Mode;
	int Mines;
	int Width;
	int Height;

	choose_mode(Mines, Width, Height, Mode);

	/* create the instance of class "field" with specified arguments */

	field game_field(Width, Height, Mines, Mode);

	/* create the window with arguments from class. */

	RenderWindow game_field_w(sf::VideoMode(game_field.width, game_field.height), "MineSweeper");

	/* loading the texture for sprite. */ 

	game_field.texture.loadFromFile(texture);

	/* define the width and the height of the field in cells. */

	game_field.n_x = Width / CELL_SIZE;		
	game_field.n_y = Height / CELL_SIZE;	

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
				break;

			case Event::MouseButtonPressed:

				int x = Mouse::getPosition(game_field_w).x / CELL_SIZE;
				int y = Mouse::getPosition(game_field_w).y / CELL_SIZE;

				if (event.mouseButton.button == Mouse::Right)
				
					game_field.Right_Button(x, y);

				if (event.mouseButton.button == Mouse::Left)
		
					game_field.Left_Button(x, y);
				
				break;
			}
		}


		game_field_w.clear();

		for (int i = 0; i < game_field.n_y; i++)
		{
			for (int j = 0; j < game_field.n_x; j++)
			{
				game_field_w.draw(game_field.Cell[i][j].sprite);
			}
		}
		game_field_w.display();

		if (game_field.game == false)
		{
			game_field_w.close();
			game_field.elapsed_time = game_field.game_clock.getElapsedTime();
			std::cout << "Game Over\nYour time: " << (int)game_field.elapsed_time.asSeconds() << "sec";
		}
	}

	getchar();
    return 0;
}
