#include "stdafx.h"
#include "Field.h"

using namespace sf;

field::field(int W, int H, int Am, int M)
{
	width = W;
	height = H;
	amount_of_mines = Am;
	mode = M;
	token = true;
	game = true;
	left = amount_of_mines;
}

void field::Make_sprites()
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

	smile.setPosition((width / 2.0f) - CELL_SIZE, CELL_SIZE / 2.0f);
	smile.setTexture(tex_smile);
	smile.setTextureRect(sf::IntRect(0, 0, 2 * CELL_SIZE, 2 * CELL_SIZE));

	count_sp.setPosition(width - 3.0f * CELL_SIZE - 4, CELL_SIZE / 2.0f);
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
	count.setPosition(width - 75.0f, 0);
}

/* Create a two-dimensional array of type cl_cell. */

void field::Create_field()
{

	/* define the width and the height of the field in cells. */

	n_x = width / CELL_SIZE;
	n_y = (height - HEIGHT) / CELL_SIZE;

	Cell = new cl_cell *[n_y];

	for (int i = 0; i < n_y; i++)
	{
		Cell[i] = new cl_cell[n_x];
	}
}

/* Setting texture to each cell. */

void field::Set_texture()
{
	for (int i = 0; i < n_y; i++)
	{
		for (int j = 0; j < n_x; j++)
		{
			if (mode == 2)
				Cell[i][j].change_state(CL_FLAG);
			Cell[i][j].sprite.setPosition(j * 32.0f, i * 32.0f + HEIGHT);
			Cell[i][j].sprite.setTexture(texture);
			Cell[i][j].sprite.setTextureRect(IntRect(Cell[i][j].state * 32, 0, 32, 32));
		}
	}
}

/* Place mines on the field and count value for each cell. */

void field::Generator(int x, int y)
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
	else	/* if a borden cell was chosen. */
	{
		int new_x, new_y;

		for (int i = 0; i < 8; i++)
		{
			new_x = x + Cell[y][x].dx[i];
			new_y = y + Cell[y][x].dy[i];
			if ((new_x >= 0) && (new_x < n_x) && (new_y >= 0) && (new_y < n_y))
				Cell[new_x][new_y].value = N1;
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

	int new_k, new_m;

	for (int k = 0; k < n_y; k++)
	{
		for (int m = 0; m < n_x; m++)
		{
			if (Cell[k][m].value == MINE)
			{
				for (int i = 0; i < 8; i++)
				{
					new_k = k + Cell[k][m].dy[i];
					new_m = m + Cell[k][m].dx[i];
					if ((new_k >= 0) && (new_k < n_y) && (new_m >= 0) && (new_m < n_x))
						if (Cell[new_k][new_m].value != MINE)
							Cell[new_k][new_m].mine++;
				}
			}
		}
	}

	/* change the value of each cell according to defines amount of mines near. */

	for (int k = 0; k < n_y; k++)
	{
		for (int m = 0; m < n_x; m++)
		{
			//Cell[k][m].value = (state_of_cell)Cell[k][m].mine;
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

	Dfs(x, y);

	/* change value of token to not call generator any more. */

	token = false;

	game_clock.restart();
}

/* Depth-First Search -- to open cell without mines with recursion. */

void field::Dfs(int x, int y)
{
	const int n_x = width / CELL_SIZE;
	const int n_y = height / CELL_SIZE - HEIGHT / CELL_SIZE;

	if (Cell[y][x].open == true)
		return;
	Cell[y][x].open = true;
	Cell[y][x].change_texture();
	int new_x, new_y;
	if (Cell[y][x].value == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			new_x = x + Cell[y][x].dx[i];
			new_y = y + Cell[y][x].dy[i];
			if ((new_x >= 0) && (new_x < n_x) && (new_y >= 0) && (new_y < n_y))
				Dfs(new_x, new_y);
		}
	}
}

/* Open all cell near if all flags placed*/

void field::Open_All_Around(int x, int y)
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
						Dfs(new_x, new_y);
					Cell[new_y][new_x].change_texture();
					if (Cell[new_y][new_x].value == MINE)
						game = false;
				}
			}
		}
	}
}

/* Open all mines when game is over. */

void field::Open_mines()
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

void field::Close_cells(int mode)
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

void field::Define_Text()
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

bool field::Check()
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

void field::Right_Button_for_m2(int x, int y)
{
	/* call generator if our field is empty. */

	if (token == true)
	{
		game_clock.restart();
		Generator(x, y);

	}
	else
	{
		if (Cell[y][x].state == CL_FLAG)
		{
			if (Cell[y][x].value == N0)
				Dfs(x, y);
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

void field::Right_Button(int x, int y)
{
	/* for mode 2. */
	if (mode == 2)
	{
		Right_Button_for_m2(x, y);
		return;
	}

	if (Cell[y][x].state == CLOSED)
	{
		Cell[y][x].change_state(FLAG);
		left--;
	}
	else
		if (Cell[y][x].state == FLAG)
		{
			Cell[y][x].change_state(MARK);
			left++;
		}
		else
			if (Cell[y][x].state == MARK)
			{
				Cell[y][x].change_state(CLOSED);
			}
}

/* Actions for Left button pressed*/

void field::Left_Buttun_for_m2(int x, int y)
{
	if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
		Open_All_Around(x, y);


	if ((Cell[y][x].state == FLAG) && (token == false))
	{
		Cell[y][x].change_state(CL_FLAG);
		left++;
	}
	else
		if ((Cell[y][x].state == CL_FLAG) && (token == false))
		{
			Cell[y][x].change_state(FLAG);
			left--;
		}
}

void field::Left_Button(int x, int y)
{
	/* for mode 2. */
	if (mode == 2)
	{
		Left_Buttun_for_m2(x, y);
		return;
	}
	/* for mode 1. */
	if (Cell[y][x].state != FLAG)
	{
		if ((Cell[y][x].value >= N1) && (Cell[y][x].value <= N8))
			Open_All_Around(x, y);


		/* call generator if our field is empty. */

		if (token == true)
		{
			game_clock.restart();
			Generator(x, y);
		}
		else
		{
			if (Cell[y][x].value == N0)
				Dfs(x, y);
			else
				Cell[y][x].change_texture();
			if (Cell[y][x].value == MINE)
				game = false;
		}
	}
}