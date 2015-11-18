#ifndef SCHOOLED_H
#define SCHOOLED_H

#include <Windows.h>

namespace schooled
{
	int const MAP_WIDTH = 60;
	int const MAP_HEIGHT = 20;
	int const SCREEN_WIDTH = 80;
	int const SCREEN_HEIGHT = 25;
	int const FLOOR_WIDTH = 3;
	int const FLOOR_HEIGHT = 3;
	int const ITEM_INDEX_SIZE = 2;

}

struct Tile
{
	char character;
	int colorCode;
	bool isPassable;
	int tileInt;
};

struct Stats{
	int HP;
	int EN;
	int STR;
};

bool operator ==(COORD a, COORD b);
COORD operator +(COORD a, COORD b);

#endif