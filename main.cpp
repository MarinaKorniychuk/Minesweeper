// Sfml.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Cell.h"
#include "Game.h"
#include "Field.h"
#include "Menu.h"


int main()
{
	while (1)
	{
		if (token == 0)
			if (!Menu())
				break;
		if (token == 1)
			if (!Game())
				break;
	}
	return 0;
}
