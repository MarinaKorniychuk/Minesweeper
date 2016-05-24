#include "stdafx.h"
#include "Cell.h"

using namespace sf;

void cl_cell::change_value(state_of_cell val)
{
	value = val;
}

void cl_cell::change_state(state_of_cell st)		/* when button pressed to open cell. */
{
	state = st;
	sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
}

void cl_cell::change_texture()					/* "open" the cell. */
{
	state = value;
	sprite.setTextureRect(IntRect(state * 32, 0, 32, 32));
}