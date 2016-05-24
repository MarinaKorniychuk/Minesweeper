#pragma once

enum state_of_cell
{
	N0,		// 0
	N1,		// 1
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

extern int Mode;
extern int Mines;
extern int Width;
extern int Height;
extern int game;			// 1 - continue; 0 - failed;
extern int token;			// define Menu/Game	

							/* choosing game mode. */

