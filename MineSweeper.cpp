// Sfml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string"
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#define CELL_SIZE 32
#define WIDTH_10 320
#define HEIGHT_10 320

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
	FLAG	// 11
};

using namespace sf;

class field
{
public:
	int width;
	int height;
	int amount_of_mines; 
	field (int w, int h, int am) 
	{
		width = w;
		height = h;
		amount_of_mines = am;
	}
};



class cl_cell
{
public:
	Sprite sprite;
	state_of_cell state;					// to define wich part of texture is used
	state_of_cell value;					// 0 - 8 mines around. 9 if mine.
	bool open;
	cl_cell()
	{
		state = CLOSED;
		value = N0;
		open = false;
	};
	void change_value(state_of_cell x)
	{
		value = x;
	}
	void change_texture()
	{
		state = value;
		sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
	}
	
};


const std::string texture = "images/Texture.png";

int main()
{
	
	int amount_of_mines = 10;
	

	/* create the instance of class "field" with specified arguments */

	field game_field(WIDTH_10, HEIGHT_10, amount_of_mines);

	/* create the window with arguments from class. */

	RenderWindow game_field_w(sf::VideoMode(game_field.width, game_field.height), "MineSweeper");

	/* loading the texture for sprite. */ 

	Texture game_texture;
	game_texture.loadFromFile(texture);

	const int n_x =  WIDTH_10 / CELL_SIZE;		
	const int n_y = HEIGHT_10 / CELL_SIZE;		

	/* create a two-dimensional array of type cl_cell. */

	cl_cell Cell[n_y][n_x];

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

	/* change value of each cell. */

	for (int i = 0; i < n_y; i++)
	{
		for (int j = 0; j < n_x; j++)
		{
			Cell[i][j].change_value(MINE);
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
				Cell[y][x].change_texture();
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


    return 0;
}

