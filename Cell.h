#pragma once
#include <SFML\Graphics.hpp>
#include "StateOfCell.h"

using namespace sf;

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

	void change_value(state_of_cell val);

	void change_state(state_of_cell st);		/* when button pressed to open cell. */

	void change_texture();						/* "open" the cell. */

};