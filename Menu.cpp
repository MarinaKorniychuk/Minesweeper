#include "stdafx.h"
#include "Cell.h"
#include "Menu.h"

using namespace sf;

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