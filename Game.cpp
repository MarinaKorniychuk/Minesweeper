#include "stdafx.h"
#include "Game.h"

#define CELL_SIZE 32
#define HEIGHT 3 * CELL_SIZE

using namespace sf;

/* window with game. */

bool Game()
{
	/* create the instance of class "field" with specified arguments */

	field game_field(Width, Height + HEIGHT, Mines, Mode);

	/* create the window with arguments from class. */

	RenderWindow game_field_w(sf::VideoMode(game_field.width, game_field.height), "MineSweeper");

	/* making sprites. */

	game_field.Make_sprites();

	/* create a two-dimensional array of type cl_cell. */

	game_field.Create_field();

	/* setting texture to each sprite. */

	game_field.Set_texture();

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
			game = 1;
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