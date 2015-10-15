#include <iostream>
#include <cassert>
#include <conio.h>
#include "Console Library/Console.h"
using namespace std;
namespace schooled
{
	int const MAP_WIDTH = 20;
	int const MAP_HEIGHT = 15;
	int const TILE_FLOOR = 1;
	int const TILE_WALL = 2;
}

#define NDEBUG


struct Tile
{
	char character;
	int colourCode;
	bool isPassable;
};

// Draws the map to the screen
void drawMap();

// Draws a tile to the screen
void drawTile(int x, int y);

// Checks if the next tile is passable
bool isPassable(int nMapX, int nMapY);


// Global variables
int nMapArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
Tile sTileIndex[] = {
	{ '.', 7, true },	// (0) TILE_FLOOR
	{ '|', 7, false }	// (1) TILE_WALL
};

int main()
{
	console.SetTitle("Schooled V0.1");

	// Initialization
	// Initialize the player's on-screen location
	int nPlayerX = 0, nPlayerY = 0;
	int nDeltaX, nDeltaY;

	// Main program loop
	while (true)
	{
		// Wipe console clear
		console.Clear();

		// Output phase
		drawMap();
		console.Position(nPlayerX, nPlayerY);
		console << '8';

		// Input phase
		KEYPRESS sKeyPress = console.WaitForKeypress();

		// Processing phase
		switch (sKeyPress.eCode)
		{
			// move down
		case CONSOLE_KEY_DOWN:
			nDeltaX = 0;
			nDeltaY = 1;
			break;

			// move left
		case CONSOLE_KEY_LEFT:
			nDeltaX = -1;
			nDeltaY = 0;
			break;

			// move right
		case CONSOLE_KEY_RIGHT:
			nDeltaX = 1;
			nDeltaY = 0;
			break;

			// move up
		case CONSOLE_KEY_UP:
			nDeltaX = 0;
			nDeltaY = -1;
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			return 0;

			// Ignore any other key
		default:
			break;
		}

		// Check if the player can move in specified direction
		if (isPassable(nPlayerX + nDeltaX, nPlayerY + nDeltaY))
		{
			// If allowed, move in specified direction
			nPlayerX += nDeltaX;
			nPlayerY += nDeltaY;
		}

	}
	return 0;
}

void drawMap() {
	for (int y = 0; y < schooled::MAP_HEIGHT; y++)
	{
		for (int x = 0; x < schooled::MAP_WIDTH; x++)
		{
			drawTile(x, y);
		}
	}
}


void drawTile(int x, int y)
{
	console.Position(x, y);
	int nType = nMapArray[y][x];
	//console.Color(sTileIndex[nType].colourCode);
	console << sTileIndex[nType].character;
}

bool isPassable(int nMapX, int nMapY)
{
	if (nMapX < 0 || nMapX >= schooled::MAP_WIDTH || nMapY < 0 || nMapY >= schooled::MAP_HEIGHT)
		return false;
	
	int nTileValue = nMapArray[nMapY][nMapX];
	if (sTileIndex[nTileValue].isPassable)
		return true;
	return false;
}