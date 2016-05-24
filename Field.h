#pragma once
#include "Cell.h"
#include <string>
#include <sstream>

using namespace sf;

#define CELL_SIZE 32
#define HEIGHT 3 * CELL_SIZE

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

	field(int W, int H, int Am, int M);

	void Make_sprites();

	void Create_field();

	void Set_texture();

	void Generator(int x, int y);

	void Dfs(int x, int y);

	void Open_All_Around(int x, int y);

	void Open_mines();

	void Close_cells(int mode);

	void Define_Text();

	bool Check();

	void Right_Button_for_m2(int x, int y);

	void Right_Button(int x, int y);

	void Left_Buttun_for_m2(int x, int y);

	void Left_Button(int x, int y);
};

