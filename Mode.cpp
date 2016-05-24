#include "stdafx.h"
#include "Mode.h"

/* choosing game mode. */

void choose_mode(int &m, int &w, int &h, int x, int y)
{

	int flag = 0;

	if (x >= 305 && x <= 496 && y >= 115 && y <= 188)
		flag = 1;
	if (x >= 305 && x <= 496 && y >= 198 && y <= 273)
		flag = 2;
	if (x >= 305 && x <= 496 && y >= 283 && y <= 358)
		flag = 3;

	switch (flag)
	{
	case 1:
		m = 15;
		w = 320;
		h = 320;
		break;
	case 2:
		m = 40;
		w = 480;
		h = 480;
		break;
	case 3:
		m = 80;
		w = 640;
		h = 640;
		break;

	default:
		break;
	}
}
