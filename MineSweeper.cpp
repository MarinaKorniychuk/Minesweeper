// Sfml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "string"
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#define CELL_SIZE 32
#define WIDTH 480
#define HEIGHT 512
#define MINES 35

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

using namespace sf;


class cl_cell
{
public:
	Sprite sprite;
	state_of_cell state;					/* to define which part of texture is used. */
	state_of_cell value;					/* 0 - 8 mines around. 9 if mine. */
	int mine;
	bool open;								/* state of cell during work of DFS. */
	int dx[8] = { 0,0,1,-1,1,1,-1,-1 };
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

	void change_state(state_of_cell st)
	{
		state = st;
		sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
	}

	void change_texture()				/* "open" the cell */
	{
		state = value;
		sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
	}


};

class field
{
public:
	int width;
	int height;
	int amount_of_mines; 
	bool token;
	field (int w, int h, int am) 
	{
		width = w;
		height = h;
		amount_of_mines = am;
		token = true;
	}

	/*place mines on the field and count value for each cell. */

	void generator(cl_cell** cell, int i, int j, int x, int y)
	{

		/* place mines and change value for cell that was chosen. */


		/* at first change value of near cells to avoid the posibility of placing mines in them */

		if ((y > 0) && (y + 1 < i) && (x > 0) && (x + 1 < j))	/* if not a borden cell was chosen. */
		{															
			for (int k = y - 1; k <= y + 1; k++)					
			{														
				for (int m = x - 1; m <= x + 1; m++)					
				{														
					cell[k][m].value = N1;								
				}													
			}											
		}
		else													/* if a borden cell was chosen. */
		{
			cell[y][x].value = N1;
			if ((y - 1 >= 0) && (x - 1 >= 0))
			{
				cell[y - 1][x - 1].value = N1;
			}
			if (y - 1 >= 0)
			{
				cell[y - 1][x].value = N1;
			}
			if ((y - 1 >= 0) && (x + 1 < j))
			{
				cell[y - 1][x + 1].value = N1;
			}
			if (x + 1 < j)
			{
				cell[y][x + 1].value = N1;
			}
			if (x - 1 >= 0)
			{
				cell[y][x - 1].value = N1;
			}
			if ((y + 1 < i) && (x - 1 >= 0))
			{
				cell[y + 1][x - 1].value = N1;
			}
			if (y + 1 < i)
			{
				cell[y + 1][x].value = N1;
			}
			if ((y + 1 < i) && (x + 1 < j))
			{
				cell[y + 1][x + 1].value = N1;
			}


		}

		/* place mines. */
		
		int new_i, new_j;

		for (int n = 0; n < MINES; n++)
		{
			std::srand(std::time(NULL));
			do
			{
				new_i = (std::rand() % i);
				new_j = (std::rand() % j);
			} while ((cell[new_i][new_j].value == MINE) || (cell[new_i][new_j].value == N1));
			cell[new_i][new_j].value = MINE;
		}
		
		for (int k = 0; k < i; k++)
		{
			for (int m = 0; m < j; m++)
			{
				if (cell[k][m].value != MINE)
				cell[k][m].value = N0;
			}
		}
		
		/* define the amount of mines near for each cell. */

		for (int k = 0; k < i; k++)
		{
			for (int m = 0; m < j; m++)
			{
				if (cell[k][m].value == MINE)
				{
					if ((k - 1 >= 0) && (m - 1 >= 0))
					{
						if (cell[k - 1][m - 1].value != MINE)
							cell[k - 1][m - 1].mine++;
					}
					if (k - 1 >= 0)
					{
						if (cell[k - 1][m].value != MINE)
							cell[k - 1][m].mine++;
					}
					if ((k - 1 >= 0) && (m + 1 < j))
					{
						if (cell[k - 1][m + 1].value != MINE)
							cell[k - 1][m + 1].mine++;
					}
					if (m + 1 < j)
					{
						if (cell[k][m + 1].value != MINE)
							cell[k][m + 1].mine++;
					}
					if (m - 1 >= 0)
					{
						if (cell[k][m - 1].value != MINE)
							cell[k][m - 1].mine++;
					}
					if ((k + 1 < i) && (m - 1 >= 0))
					{
						if (cell[k + 1][m - 1].value != MINE)
							cell[k + 1][m - 1].mine++;
					}
					if (k + 1 < i)
					{
						if (cell[k + 1][m].value != MINE)
							cell[k + 1][m].mine++;
					}
					if ((k + 1 < i) && (m + 1 < j))
					{
						if (cell[k + 1][m + 1].value != MINE)
							cell[k + 1][m + 1].mine++;
					}
				}
			}
		}

		/* change the value of each cell according to defines amount of mines near. */

		for (int k = 0; k < i; k++)
		{
			for (int m = 0; m < j; m++)
			{
				switch (cell[k][m].mine)
				{
				case 1:
					cell[k][m].value = N1;
					break;
				case 2:
					cell[k][m].value = N2;
					break;
				case 3:
					cell[k][m].value = N3;
					break;
				case 4:
					cell[k][m].value = N4;
					break;
				case 5:
					cell[k][m].value = N5;
					break;
				case 6:
					cell[k][m].value = N6;
					break;
				case 7:
					cell[k][m].value = N7;
					break;
				case 8:
					cell[k][m].value = N8;
					break;
				default:
					break;
				}
			}
		}

		/* if we need to show field after generation. */

		/* change texture. */

		/*for (int k = 0; k < i; k++)
		{
			for (int m = 0; m < j; m++)
			{
				cell[k][m].change_texture();
			}
		}*/
	}

	/* --------------------------------------------------------------------------------------------------------------------------- */

	/*Depth-First Search -- to open cell without mines with recursion. */

	void dfs(cl_cell **cell, int x, int y)
	{
		const int n_x = WIDTH / CELL_SIZE;
		const int n_y = HEIGHT / CELL_SIZE;

		if (cell[y][x].open == true)
			return;
		cell[y][x].open = true;
		cell[y][x].change_texture();
		if (cell[y][x].value == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				if ((x + cell[y][x].dx[i] >= 0) && (x + cell[y][x].dx[i] < n_x) && (y + cell[y][x].dy[i] >= 0) && (y + cell[y][x].dy[i] < n_y))
					dfs(cell, x + cell[y][x].dx[i], y + cell[y][x].dy[i]);
			}
		}
	}
};


const std::string texture = "images/Texture.png";		

int main()
{
	
	int amount_of_mines = MINES;

	/* create the instance of class "field" with specified arguments */

	field game_field(WIDTH, HEIGHT, amount_of_mines);

	/* create the window with arguments from class. */

	RenderWindow game_field_w(sf::VideoMode(game_field.width, game_field.height), "MineSweeper");

	/* loading the texture for sprite. */ 

	Texture game_texture;
	game_texture.loadFromFile(texture);

	/* define the width and the height of the field in cells. */

	const int n_x = WIDTH / CELL_SIZE;		
	const int n_y = HEIGHT / CELL_SIZE;	

	/* create a two-dimensional array of type cl_cell. */

	cl_cell **Cell = new cl_cell *[n_y];

	for (int i = 0; i < n_y; i++)
	{
		Cell[i] = new cl_cell[n_x];
	}

	/* set initial texture to each cell of field. */

	for (int i = 0; i < n_y; i++)
	{
		for (int j = 0; j < n_x; j++)
		{
			Cell[i][j].sprite.setPosition(j * 32, i * 32);
			Cell[i][j].sprite.setTexture(game_texture);
			Cell[i][j].sprite.setTextureRect(IntRect(Cell[i][j].state * 32, 0, 32, 32));
		}
	}

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

				/* place FLAG/QUESTION. */

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << Cell[y][x].value << std::endl;
					while(1)
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


				if (event.mouseButton.button == sf::Mouse::Left)
				{
					int count = 0;

					/* call generator if our field is empty. */

					if (game_field.token == true) 
					{
						game_field.generator(Cell, n_y, n_x, x, y);
						game_field.dfs(Cell, x, y);
						game_field.token = false;
					}
					else
					{
						if (Cell[y][x].value == N0)
							game_field.dfs(Cell, x, y);
						else
						Cell[y][x].change_texture();
						if (Cell[y][x].value == MINE)
						{
							std::cout << "Game Over";
							game_field_w.close();
						}
					}
				}
				
				break;
			
			}
		}


		game_field_w.clear();

		for (int i = 0; i < n_y; i++)
		{
			for (int j = 0; j < n_x; j++)
			{
				game_field_w.draw(Cell[i][j].sprite);
			}
		}
		game_field_w.display();
	}

	getchar();
    return 0;
}
